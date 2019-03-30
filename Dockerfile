FROM lockblox/orthodox:latest

RUN git clone https://github.com/cpp-ipfs/vcpkg.git \
 && cd vcpkg \
 && ./bootstrap-vcpkg.sh \
 && ./vcpkg integrate install \
 && ./vcpkg install gtest cryptopp ms-gsl varint

COPY . /root/src