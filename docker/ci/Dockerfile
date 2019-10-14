FROM ubuntu:19.04


RUN apt update -y && apt install -y cmake gcc g++ python3-pip && \
    pip3 install conan && \
    pip3 install gcovr

WORKDIR /tmp/expresscpp
COPY . /tmp/expresscpp

RUN rm -rf build || true && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j
