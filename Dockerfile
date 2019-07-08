FROM lockblox/orthodox:latest

RUN apt-get install dos2unix

RUN cd ${HOME} \
 && git clone https://github.com/cpp-ipfs/vcpkg.git \
 && cd vcpkg \
 && ./bootstrap-vcpkg.sh \
 && ./vcpkg integrate install \
 && ./vcpkg install gtest cryptopp ms-gsl \
 && ./vcpkg install --head varint

ENV CMAKE_CONFIG_ARGS "-DCMAKE_TOOLCHAIN_FILE=/root/vcpkg/scripts/buildsystems/vcpkg.cmake"

COPY . /root/src

# Ensure test file does not have DOS line endings
RUN dos2unix /root/src/test/foo