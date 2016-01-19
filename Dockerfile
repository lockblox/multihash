FROM centos:latest
MAINTAINER Jonathan Brooker <jonathan.brooker@gmail.com>

RUN apt-get install -yy --no-install-recommends libboost-filesystem-dev \
    libboost-system-dev libboost-test-dev libboost-program-options-dev \
    libssl-dev \
 && apt-get -yy autoremove \
 && apt-get -yy clean

COPY . /home/cpp-multihash
WORKDIR /home/cpp-multihash

RUN cd build \
 && rm -rf * \
 && cmake -DCMAKE_BUILD_TYPE=Release .. \
 && scan-build make \
 && make test CTEST_OUTPUT_ON_FAILURE=TRUE 
