#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <multihash/multihash.h>
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
    // Declare the supported options.
    std::ostringstream os;
    os << "Usage: multihash [OPTION]... [FILE]...\n"
       << "Print cryptographic digests.\n"
       << "With no FILE or when file is -, read standard input";
    po::options_description desc(os.str());
    desc.add_options()("help", "display help message");
    desc.add_options()("hash-type", po::value<std::string>(),
                       "algorithm, e.g sha1");
    desc.add_options()("list-hash-types", "list all available algorithms");

    po::variables_map vm;

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }
    if (vm.count("help") > 0)
    {
        std::cout << desc << std::endl;
        return 0;
    }
    if (vm.count("list-hash-types") > 0)
    {
        for (auto hash_type : multihash::hashTypes())
        {
            std::cout << hash_type.name() << std::endl;
        }
        return 0;
    }

    /** Actually do some work if an algo is specified */
    std::string algo;
    if (vm.count("hash-type") == 1)
    {
        algo = vm["hash-type"].as<std::string>();
    }
    else
    {
        std::cerr << "Must specify a hash type" << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }

    try
    {
        auto hash_function = multihash::HashFunction(algo);

        std::ios_base::sync_with_stdio(false); // enable fast io

        std::vector<std::string> filenames;
        if (argc > 3)
        {
            filenames.assign(&argv[3], &argv[argc]);
        }

        auto num_files = filenames.size();
        if ((num_files == 1 && (filenames.front() == "-")) or num_files == 0)
        {
            auto hash = hash_function(std::cin);
            std::cout << hash << " -" << std::endl;
        }
        else
        {
            for (auto filename : filenames)
            {
                if (!boost::filesystem::exists(filename))
                {
                    std::cerr << "multihash: " << filename
                              << ": No such file or directory" << std::endl;
                    continue;
                }
                else if (boost::filesystem::is_directory(filename))
                {
                    std::cerr << "multihash: " << filename
                              << ": Is a directory" << std::endl;
                    continue;
                }
                std::ifstream filestream(filename);
                if (filestream.good())
                {
                    auto hash = hash_function(filestream);
                    std::cout << hash << " " << filename << std::endl;
                }
                else
                {
                    std::cerr << "multihash: " << filename << ": "
                              << "Permission denied" << std::endl;
                }
            }
        }
    }
    catch (multihash::InvalidHashException& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cout << "Available hash types: " << std::endl;
        for (auto hash_type : multihash::hashTypes())
        {
            std::cout << hash_type.name() << std::endl;
        }
    }
    return 0;
}
