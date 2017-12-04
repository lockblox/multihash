#include <array>
#include <fstream>
#include <getopt.h>
#include <shax/shax.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    // Declare the supported options.
    std::ostringstream os;
    os << "Usage: shax [OPTION]... [FILE]..." << std::endl
       << "Print cryptographic digests." << std::endl
       << "With no FILE or when file is -, read standard input" << std::endl
       << "    --help               Display help message" << std::endl
       << "    --hash-type          Algorithm to use, e.g. sha1" << std::endl
       << "    --list-hash-types    List available algorithms" << std::endl;
    std::string usage = os.str();

    int c = 0;
    int help_flag = 0;
    int list_flag = 0;
    std::string algo;

    std::array<option, 4> long_options{
        {option{"help", no_argument, &help_flag, 1},
         option{"hash-type", required_argument, nullptr, 't'},
         option{"list-hash-types", no_argument, &list_flag, 1},
         option{nullptr, 0, nullptr, 0}}};

    while (true)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, "ht:l", long_options.data(),
                        &option_index);

        /* Detect the end of the options. */
        if (c == -1)
        {
            break;
        }

        switch (c)
        {
            case 0:
                /* If not a flag-setting long option, print help */
                if (long_options.at(option_index).flag == nullptr)
                {
                    help_flag = 1;
                }
                break;
            case 't':
                algo = optarg;
                break;
            case '?':
                break;
            default:
                help_flag = 1;
        }
    }

    if (1 == help_flag)
    {
        std::cout << usage << std::endl;
        return 0;
    }
    if (1 == list_flag)
    {
        for (auto hash_type : x::hashi::HashType::types())
        {
            std::cout << hash_type.name() << std::endl;
        }
        return 0;
    }

    /** Actually do some work if an algo is specified */
    if (algo.empty())
    {
        std::cerr << "Must specify a hash type" << std::endl;
        std::cerr << usage << std::endl;
        return 1;
    }

    try
    {
        auto hash_function = x::hashi::HashFunction(algo);

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
                struct stat stat_record;
                if (-1 == stat(filename.c_str(), &stat_record))
                {
                    std::cerr << "shax: " << filename
                              << ": No such file or directory" << std::endl;
                    continue;
                }
                else if (S_IFDIR == (stat_record.st_mode & S_IFMT))
                {
                    std::cerr << "shax: " << filename << ": Is a directory"
                              << std::endl;
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
                    std::cerr << "shax: " << filename << ": "
                              << "Permission denied" << std::endl;
                }
            }
        }
    }
    catch (x::hashi::InvalidHashException& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cout << "Available hash types: " << std::endl;
        for (auto hash_type : x::hashi::HashType::types())
        {
            std::cout << hash_type.name() << std::endl;
        }
    }
    return 0;
}
