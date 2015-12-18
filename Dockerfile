FROM jbrooker/clang-toolchain:latest
MAINTAINER Jonathan Brooker <jonathan.brooker@gmail.com>

COPY . /home/cpp-multihash

RUN apt-get install -yy --no-install-recommends libboost-filesystem-dev \
    libboost-system-dev libboost-test-dev libboost-program-options-dev \
    libssl-dev \
 && apt-get -yy autoremove \
 && apt-get -yy clean

WORKDIR /home/cpp-multihash

RUN cd build \
 && rm -rf * \
 && cmake .. \
 && make     \
 && make test
