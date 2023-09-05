#!/bin/sh

# Function to display usage instructions
usage() {
    echo "Usage: $0 [install]"
    echo "       install - Downloads and installs the tools."
    echo "       No argument or any other argument - Only exports PATH."
}

# Update all sub-repos
git submodule update --init --recursive

# Import paths to ESP toolchains
export IDF_PATH="$(pwd)/ESP8266_RTOS_SDK"
echo "IDF_PATH set to: ${IDF_PATH}"

# If "install" argument is passed, then download and install
if [ "$#" -eq 1 ] && [ "$1" = "install" ]; then
    # Install and import ESP IDF tools
    ./ESP8266_RTOS_SDK/install.sh

    # Download xtensa-tools
    wget https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz

    # Unzip xtensa-toolchain
    rm -rf build
    mkdir -p build
    tar -xzf xtensa*.gz -C $(pwd)/build

    # Remove remnant zip file
    rm -rf xtensa-*.gz*
elif [ "$#" -eq 1 ] && [ "$1" != "install" ]; then
    usage
    exit 1
fi

# Export PATH to xtensa by default
export PATH="$PATH:$(pwd)/build/xtensa-lx106-elf/bin"

# Validate if PATH contains xtensa
echo "PATH set to: ${PATH}"
echo ${PATH} | grep xtensa
