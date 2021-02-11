FROM lockblox/orthodox:latest

RUN apt-get install dos2unix

RUN cd ${VCPKG_ROOT} \
 && git remote add lockblox https://github.com/lockblox/vcpkg.git \
 && git config user.name "engineering" \
 && git config user.email "engineering@lockblox.com" \
 && git pull lockblox master \
 && ./vcpkg install cryptopp ms-gsl \
 && ./vcpkg install --head varint

COPY . /root/src

# Ensure test file does not have DOS line endings
RUN dos2unix /root/src/test/foo
