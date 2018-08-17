FROM jbrooker/clang-toolchain:latest
MAINTAINER Jonathan Brooker <jonathan.brooker@gmail.com>

RUN apt-get update \
 && apt-get install -yy --no-install-recommends \
            g++ \
            libstdc++-4.9-dev \
            libssl-dev \
            git \
 && apt-get -yy autoremove \
 && apt-get -yy clean

WORKDIR /work/build

ENTRYPOINT /work/source/build.sh
