[![Build Status](https://travis-ci.org/cpp-ipfs/cpp-multihash.svg?branch=master)](https://travis-ci.org/cpp-ipfs/cpp-multihash)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/cpp-ipfs/cpp-multihash?svg=true)](https://ci.appveyor.com/project/jbrooker/cpp-multihash)
[![Coverage Status](https://coveralls.io/repos/github/cpp-ipfs/cpp-multihash/badge.svg?branch=master)](https://coveralls.io/github/cpp-ipfs/cpp-multihash?branch=master)

# multihash

[multihash](//github.com/jbenet/multihash) implementation in c++.

Multihash is delivered as 
1. A library: libmultihash
2. An executable: [bin/multihash](multihash/main.cpp)

# Build
```
cd build
cmake ..
make
echo -n "foo" | ../bin/multihash --hash-type sha1
11140beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33 -
```

# Usage
## libmultihash

At the lowest level, there is a hash algorithm.

A hash algorithm defines the steps required to compute a hash some from 
input data.

Hash algorithm instances are created by a factory and each factory is
registered under a unique hash code identifier.

A hash function uses the methods of a hash algorithm to compute the 
hash of data.

A hash function can be created directly from an algorithm or via the factory 
registry.


A multihash combines the hash code and digest in a binary payload.


```cpp
auto input = "Hello, World!"s;
auto hash = multihash::sha3{};
auto output = multihash::sha3{}(input);
auto digest_size = multihash::sha3::digest_size();
```

```cpp
auto hash_name = "sha3-512";
auto hash_function = multihash::hash_function(hash_name);
auto output = hash_function("data");
std::cout << output.digest() << std::endl;;
```

multihash::sha3 is an algorithm?

# License

MIT
