FROM ubuntu:22.04

ENV DEBIAN_FRONTEND noninteractive
ENV PICO_SDK_PATH=/pico/pico-sdk

WORKDIR /tmp

RUN apt update \
    && apt install -y minicom curl wget git gdb-multiarch cmake make pkgconf autoconf libreadline-dev tk-dev \
    build-essential libtool automake libftdi-dev libusb-1.0-0-dev gcc texinfo checkinstall \
    libssl-dev libsqlite3-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev \
    gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib clang-tidy \
    python3 python3-minimal python3-pkg-resources python3-pygments python3-yaml python3.10 python3.10-minimal \
    && apt -y clean \
    && apt -y autoremove \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --branch rp2040 --recursive --depth=1  https://github.com/raspberrypi/openocd.git  \
    && cd openocd \
    && ./bootstrap \
    && ./configure \
    && make \
    && make install \
    && cd .. \
    && rm -rf openocd

WORKDIR /pico

RUN git clone --depth 1 --branch 1.4.0 https://github.com/raspberrypi/pico-sdk.git pico-sdk \
    && cd pico-sdk \
    && git submodule update --init \ 
    && cd /lib \
    && rm -rf tinyusb \
    && git clone --depth 1 --branch 0.14.0 https://github.com/hathach/tinyusb.git

RUN git clone https://github.com/raspberrypi/openocd.git openocd

WORKDIR /