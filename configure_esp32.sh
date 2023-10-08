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
export IDF_PATH="$(pwd)/ESP32_RTOS_SDK"

echo "IDF_PATH set to: ${IDF_PATH}"

# If "install" argument is passed, then download and install
if [ "$#" -eq 1 ] && [ "$1" = "install" ]; then
    # Install and import ESP IDF tools
    ${IDF_PATH}/install.sh
    . ${IDF_PATH}/export.sh
       
elif [ "$#" -eq 1 ] && [ "$1" != "install" ]; then
    usage
    exit 1
else
    # just export paths
    . ./${IDF_PATH}/export.sh
fi

# Validate if PATH contains xtensa
echo "PATH set to: ${PATH}"
echo ${PATH} | grep xtensa
