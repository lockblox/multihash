# multihash

[![Build Status](https://travis-ci.org/cpp-ipfs/cpp-multihash.svg?branch=master)](https://travis-ci.org/cpp-ipfs/cpp-multihash)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/cpp-ipfs/cpp-multihash?svg=true)](https://ci.appveyor.com/project/lockblox/cpp-multihash)
[![Coverage Status](https://coveralls.io/repos/github/cpp-ipfs/cpp-multihash/badge.svg?branch=master)](https://coveralls.io/github/cpp-ipfs/cpp-multihash?branch=master)

[multihash](//github.com/jbenet/multihash) implementation in c++.

Multihash is delivered as

1. A library: libmultihash
2. An executable: [bin/multihash](multihash/main.cpp)

## Build

### Building with Docker

The supplied dockerfile provides a reproducable build image which can be used to compile the source:
```bash
docker build -t lockblox/multihash multihash
docker run -it --rm -v multihash:/root/src lockblox/multihash 
```

### Building Manually

#### Installing Prerequisites

Multihash requires some dependencies to be provided before building, which are provided via vcpkg as follows:
```bash
 git clone https://github.com/lockblox/vcpkg.git \
 cd vcpkg \
 ./bootstrap-vcpkg.sh \
 ./vcpkg integrate install \
 ./vcpkg install gtest cryptopp ms-gsl varint
```

#### Building with CMake ####

Multihash uses cmake for builds. In order for cmake to find the required dependency, the `-DCMAKE_TOOLCHAIN_FILE` option should be supplied, for example:
```bash
 cmake -DCMAKE_TOOLCHAIN_FILE=../vcpkg/scripts/buildsystems/vcpkg.cmake" ../multihash
 cmake --build .
 ```

## Usage

### Algorithm

At the lowest level, there is the abstract type `multihash::algorithm`.

A `multihash::algorithm` defines the steps required to compute a hash from
input data.

`multihash::algorithm` instances are created by a factory and each factory is
registered under a unique hash code identifier.

```cpp
auto code = multihash::code::sha3_256;
auto algorithm = multihash::algorithm::create(code);
auto buffer = std::string{"hello, world"};
algorithm_->update(buffer);
auto result = std::string(256, '=');
algorithm_->digest(result);
```

### Digest

A `multihash::digest` functor uses the methods of a hash algorithm to compute the
hash of data and can be used where a plain (non-multihash) digest is required.

A `multihash::digest` can be created directly from an algorithm or via the factory
registry.

```cpp
auto input = std::string{"hello, world"};
auto output = std::string{};
auto digest = multihash::digest(algorithm)(input.begin(), input.end(),
                                           std::back_inserter(output));
```

### Function

A `multihash::function` is a hash functor which combines a `multihash::code` and a `multihash::digest` to create a `multihash::multihash` object proper.

```cpp
auto multihash = multihash::function(code)(input.begin(), input.end());
```

### Multihash

A `multihash::multihash` combines the hash code and digest in a binary payload.

```cpp
auto code = multihash.code();
auto digest = multihash.digest();
```

## License

multihash is released under the [MIT License](LICENSE.txt).
