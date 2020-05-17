FROM ubuntu:19.10

RUN apt update -y && apt install -y cmake gcc-8 g++-8 autotools-dev autoconf python3-pip && \
    pip3 install conan && \
    pip3 install gcovr

WORKDIR /tmp/expresscpp
COPY . /tmp/expresscpp

RUN export CC=/usr/bin/gcc-8 && \
    export CXX=/usr/bin/g++-8 && \
    rm -rf build || true && \
    mkdir -p build && \
    cd build && \
    cmake .. -DEXPRESSCPP_USE_CONAN_DEPENDENCIES=ON && \
    make -j && \
    cd ../ && \
    rm -rf ./build
