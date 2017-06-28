[![Build Status](https://travis-ci.org/campx/hashix.svg?branch=master)](https://travis-ci.org/campx/hashix)

# hashix

[multihash](//github.com/jbenet/multihash) implementation in c++.

## Example

Run [bin/hashix](src/main.cpp)

```
cd build
cmake ..
make
echo -n "foo" | ../bin/hashix --hash-type sha1
11140beec7b5ea3f0fdbc95d0dd47f3c5bc275da8a33 -
```

## License

MIT
