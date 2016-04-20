FROM centos:latest
MAINTAINER Jonathan Brooker <jonathan.brooker@gmail.com>

RUN apt-get install -yy --no-install-recommends libstdc++-4.9-dev libssl-dev \
    git \
 && apt-get -yy autoremove \
 && apt-get -yy clean

WORKDIR /work/

RUN mkdir build
 
ENTRYPOINT /work/source/build.sh

#example run: docker run \
#   -v /home/jonathanb/Documents/Code/cpp-multihash/:/work/source/ \
#   --privileged --name multihash-build -it jbrooker/cpp-multihash
# docker start -i multihash-build
