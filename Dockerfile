FROM ubuntu:14.04
MAINTAINER Jonathan Brooker <jonathan.brooker@gmail.com>

COPY . /home/cpp-multihash

RUN apt-get update \
 && apt-get install -y \
	    	gcc \
            libboost-all-dev \
            cmake \
            build-essential \
            libssl-dev \
            pkg-config

WORKDIR /home/cpp-multihash

RUN cd build \
 && rm -rf * \
 && cmake .. \
 && make     \
 && make test
