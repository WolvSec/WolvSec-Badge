FROM ubuntu:22.04
RUN     apt update && apt install -y \
        pkg-config \
        libusb-1.0-0-dev \
        libboost-all-dev \
        cmake \
        git  \
        python3 \
        gcc-arm-none-eabi \
        g++ \
        libnewlib-arm-none-eabi \
        libstdc++-arm-none-eabi-newlib && \
        rm -rf /var/lib/apt/lists/* && \
        mkdir /home/badge

WORKDIR /home/badge
