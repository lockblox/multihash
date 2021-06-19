#include <getopt.h>
#include <multihash/algorithm_registry.h>
#include <multihash/multihash.h>
#include <sys/stat.h>

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

template <typename InputIterator>
std::ostream& print_hex(std::ostream& os, InputIterator first,
                        InputIterator last) {
  for (; first != last; ++first) {
    auto uc = uint8_t(*first);
    os << std::hex << std::setfill('0') << std::setw(2);
    os << static_cast<int>(uc);
  }
  return os;
}

void output_hash_types(std::ostream& os) {
  auto& factories = multihash::algorithm_registry::default_instance();
  for (const auto& factory : factories) os << factory.name() << std::endl;
}

}  // namespace

int main(int argc, char* argv[]) {
  // Declare the supported options.
  std::ostringstream os;
  os << "Usage: multihash [OPTION]... [FILE]..." << std::endl
     << "Print cryptographic digests." << std::endl
     << "With no FILE or when file is -, read standard input" << std::endl
     << "    --help               Display help message" << std::endl
     << "    --hash-type          algorithm to use, e.g. sha1" << std::endl
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

  while (true) {
    int option_index = 0;
    c = getopt_long(argc, argv, "ht:l", long_options.data(), &option_index);

    /* Detect the end of the options. */
    if (c == -1) {
      break;
    }

    switch (c) {
      case 0:
        /* If not a flag-setting long option, print help */
        if (long_options.at(option_index).flag == nullptr) {
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

  if (1 == help_flag) {
    std::cout << usage << std::endl;
    return 0;
  }
  if (1 == list_flag) {
    output_hash_types(std::cout);
    return 0;
  }

  /** Actually do some work if an algo is specified */
  if (algo.empty()) {
    std::cerr << "Must specify a hash type" << std::endl;
    std::cerr << usage << std::endl;
    return 1;
  }

  try {
    auto& registry = multihash::algorithm_registry::default_instance();
    auto& factory = registry.at(algo);
    auto algorithm = factory.make_algorithm();

    std::ios_base::sync_with_stdio(false);  // enable fast io

    std::vector<std::string> filenames;
    if (argc > 3) {
      filenames.assign(&argv[3], &argv[argc]);
    }

    auto num_files = filenames.size();
    if ((num_files == 1 && (filenames.front() == "-")) or num_files == 0) {
      std::istreambuf_iterator<char> first(std::cin);
      std::istreambuf_iterator<char> last;
      auto digest = algorithm->digest(first, last);
      std::cout << digest << " -" << std::endl;
    } else {
      for (auto filename : filenames) {
        struct stat stat_record;
        if (-1 == stat(filename.c_str(), &stat_record)) {
          std::cerr << "multihash: " << filename
                    << ": No such file or directory" << std::endl;
          continue;
        } else if (S_IFDIR == (stat_record.st_mode & S_IFMT)) {
          std::cerr << "multihash: " << filename << ": Is a directory"
                    << std::endl;
          continue;
        }
        std::ifstream filestream(filename);
        std::istreambuf_iterator<char> first(filestream);
        std::istreambuf_iterator<char> last;
        if (filestream.good()) {
          auto digest = algorithm->digest(first, last);
          auto identifier = varint::uleb128<std::string>{factory.identifier()};
          auto id_view =
              multihash::code_type{static_cast<std::string_view>(identifier)};
          auto multihash_digest =
              multihash::multihash<std::string>{id_view, digest};
          print_hex(std::cout, multihash_digest.begin(), multihash_digest.end());
        } else {
          std::cerr << "multihash: " << filename << ": "
                    << "Permission denied" << std::endl;
        }
      }
    }
  } catch (std::invalid_argument& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    std::cout << "Available hash types: " << std::endl;
    output_hash_types(std::cout);
  }
  return 0;
}