FROM ubuntu:latest

RUN echo 'APT::Install-Suggests "0";' >> /etc/apt/apt.conf.d/00-docker
RUN echo 'APT::Install-Recommends "0";' >> /etc/apt/apt.conf.d/00-docker

RUN DEBIAN_FRONTEND=noninteractive \
	apt-get update && \
	apt-get clean &&  \
        apt-get install ca-certificates -y && \
        update-ca-certificates && \
	apt-get install -y \
        build-essential \
        pkg-config \
        libusb-1.0-0-dev \
        libboost-all-dev \
        cmake \
        wget \
        curl \
        git  \
        python3 \
        vim \
        nano

WORKDIR /home/badge

RUN apt-get -y install cmake \
        gcc-arm-none-eabi \
        libnewlib-arm-none-eabi \
        libstdc++-arm-none-eabi-newlib

# Install the Pico SDK
RUN git clone https://github.com/raspberrypi/pico-sdk.git /home/sdk/pico-sdk && \
        cd /home/sdk/pico-sdk && \
        git submodule update --init && \
        cd /home/badge

ENV PICO_SDK_PATH=/home/sdk/pico-sdk

# Install Picotool
RUN git clone https://github.com/raspberrypi/picotool.git /home/picotool/ && \
        cd /home/picotool && \
        git submodule update --init && \
        mkdir build && \
        cd build && \
        cmake .. && \
        make && \
        make install && \
        cd /home/badge
