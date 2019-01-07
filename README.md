[![Build Status](https://travis-ci.org/cpp-ipfs/cpp-multihash.svg?branch=master)](https://travis-ci.org/cpp-ipfs/cpp-multihash)
[![Build Status](https://ci.appveyor.com/api/projects/status/github/cpp-ipfs/cpp-multihash?svg=true)](https://ci.appveyor.com/project/jbrooker/cpp-multihash)
[![Coverage Status](https://coveralls.io/repos/github/cpp-ipfs/cpp-multihash/badge.svg?branch=master)](https://coveralls.io/github/cpp-ipfs/cpp-multihash?branch=master)

# multihash

[multihash](//github.com/jbenet/multihash) implementation in c++.

## Example

Run [bin/multihash](multihash/main.cpp)

```
cd build
cmake ..
make
echo -n "foo" | ../bin/multihash --hash-type sha1
11140beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33 -
```

## License

MIT
