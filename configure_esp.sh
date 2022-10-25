#!/bin/sh

# update all sub-repos
git submodule update --init --recursive

# import paths to ESP toolchains
export IDF_PATH="$(pwd)/ESP8266_RTOS_SDK"
echo ${IDF_PATH}

# install and import ESP IDF tools
./ESP8266_RTOS_SDK/install.sh

# get xtensa-tools
wget https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz

# unzip xtensa-toolchain
rm -rf build
mkdir -p build
tar -xzf xtensa*.gz -C $(pwd)/build

# export PATH to xtensa
export PATH="$PATH:$(pwd)/build/xtensa-lx106-elf/bin"
echo ${PATH} | grep xtensa

# remove remnant zip file
rm -rf xtensa-*.gz*