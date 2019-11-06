FROM ubuntu:19.10

RUN apt update -y && apt install -y cmake gcc-9 g++-9 clang-9 autotools-dev autoconf clang-tidy-9 python3-pip && \
    pip3 install conan && \
    pip3 install gcovr && \
    conan remote add vthiery "https://api.bintray.com/conan/vthiery/conan-packages" 

WORKDIR /tmp/expresscpp
COPY . /tmp/expresscpp

RUN export CC=/usr/bin/clang-9 && \
    export CXX=/usr/bin/clang++-9 && \
    rm -rf build || true && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j && \
    cd ../ && \
    rm -rf ./build