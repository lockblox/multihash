FROM lockblox/orthodox:latest

RUN cd ${HOME} \
 && git clone https://github.com/cpp-ipfs/vcpkg.git \
 && cd vcpkg \
 && ./bootstrap-vcpkg.sh \
 && ./vcpkg integrate install \
 && ./vcpkg install gtest cryptopp ms-gsl varint

ENV CMAKE_CONFIG_ARGS "-DCMAKE_TOOLCHAIN_FILE=/root/vcpkg/scripts/buildsystems/vcpkg.cmake"

COPY . /root/src