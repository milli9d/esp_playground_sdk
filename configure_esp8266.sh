#!/bin/sh

# Function to display usage instructions
usage() {
    echo "Usage: $0 [install]"
    echo "       install - Downloads and installs the tools."
    echo "       No argument or any other argument - Only exports PATH."
}

# Function to create or activate a virtual environment
create_or_activate_venv() {
    if [ -d ".venv" ]; then
        # Activate the existing virtual environment
        source .venv/bin/activate
    else
        # Create a Python 3 virtual environment
        python3 -m venv .venv
        
        # Activate the newly created virtual environment
        source .venv/bin/activate
    fi
}
# Create or activate the virtual environment
# create_or_activate_venv

# Update all sub-repos
git submodule update --init --recursive

# Import paths to ESP toolchains
export IDF_PATH="$(pwd)/ESP8266_RTOS_SDK"

echo "IDF_PATH set to: ${IDF_PATH}"

create_or_activate_venv

# If "install" argument is passed, then download and install
if [ "$#" -eq 1 ] && [ "$1" = "install" ]; then
    sudo apt-get install git wget flex bison gperf python-is-python3 python3 python3-pip python3-venv cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0

    # Install and import ESP IDF tools
    ${IDF_PATH}/install.sh
    . ${IDF_PATH}/export.sh
       
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
else
    # just export paths
    . ${IDF_PATH}/export.sh
fi

# Validate if PATH contains xtensa
echo "PATH set to: ${PATH}"
echo ${PATH} | grep xtensa