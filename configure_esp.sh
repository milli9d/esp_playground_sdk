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

    # Create or activate the virtual environment
    create_or_activate_venv

    # Install requirements.txt under ESP8266_RTOS_SDK directory
    pip install -r ESP8266_RTOS_SDK/requirements.txt

    # Add ESP IDF path to the venv permanently
    echo "export IDF_PATH=${IDF_PATH}" >> .venv/bin/activate
    echo "export PATH="$PATH:$(pwd)/build/xtensa-lx106-elf/bin"" >> .venv/bin/activate

elif [ "$#" -eq 1 ] && [ "$1" != "install" ]; then
    usage
    exit 1
else
    # Create or activate the virtual environment if no argument is provided
    create_or_activate_venv
fi

# Validate if PATH contains xtensa
echo "PATH set to: ${PATH}"
echo ${PATH} | grep xtensa
