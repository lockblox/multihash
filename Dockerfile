FROM centos:latest
MAINTAINER Jonathan Brooker <jonathan.brooker@gmail.com>

RUN apt-get install -yy --no-install-recommends libboost-filesystem-dev \
    libboost-system-dev libboost-test-dev libboost-program-options-dev \
    libssl-dev \
 && apt-get -yy autoremove \
 && apt-get -yy clean

COPY . /work/cpp-multihash
WORKDIR /work/

RUN mkdir multihash_build \
 && cd multihash_build \
 && cmake -DCMAKE_BUILD_TYPE=Release /work/cpp-multihash \
 && scan-build make \
 && make test CTEST_OUTPUT_ON_FAILURE=TRUE 
