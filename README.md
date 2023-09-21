# F' Deployment for the Artemis Cubesat Kit

This reposiotry includes the flight software for the Artemis CubeSat. 

The Artemis CubeSat is a low cost, spaceflight ready, 1U CubeSat aimed at broadening accessibility to space and aerospace engineering education.

For more information about the Artemis CubeSat, please visit our [website](mahinaaerospace.com)

# F' Requirements:
F´ depends on several items before the user should attempt to install it. These requirements are listed below and the user should ensure they are installed before proceeding with this guide.

1. Linux or WSL on Windows
2. git
3. CMake 3.16 or newer. CLI tool must be available on the system path.
4. CLang or GNU C and C++ compilers (e.g. gcc and g++)
5. Python 3.8+, virtual environments, and PIP

Once you have installed all of the requirements, proceed with the rest of the project instructions. 

# Install F'
Before continung with these instructions install F'
```shell
pip install -U fprime-tools
```

# Project Requirements

## Install arduino-cli
```shell
mkdir -p ~/.local/bin
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh
```

This command downloads `arduino-cli` and installs the binary.

## Add `~/.local/bin` to PATH
```shell
sudo nano ~/.bashrc
```

Add the following line at the end of the file:
```
export PATH=~/.local/bin:$PATH
```

Save, exit, and run the following:
```shell
source ~/.bashrc  # or restart your terminal
```

## Install arduino-cli-wrapper
```shell
pip install arduino-cli-cmake-wrapper
```

## Setup arduino-cli for select Arduino boards

Initialize the arduino-cli configuration file.
```shell
arduino-cli config init
```

Below is the board manager URL for Teensy board. 
```shell
arduino-cli config add board_manager.additional_urls https://www.pjrc.com/teensy/package_teensy_index.json
```

Install the new board packages.
```shell
arduino-cli core update-index
arduino-cli core install teensy:avr
```

## Adding udev rules (Linux Only)
Add udev rules. Download/save the `.rules` files located [here](./rules/) for your selected board(s) into `/etc/udev/rules.d/`.

## Raspberry Pi Tooolchian 

Download Raspberry Pi Zero cross-compiler toolchain [here](https://sourceforge.net/projects/raspberry-pi-cross-compilers/files/Raspberry%20Pi%20GCC%20Cross-Compiler%20Toolchains/Bullseye/GCC%2010.3.0/Raspberry%20Pi%201%2C%20Zero/cross-gcc-10.3.0-pi_0-1.tar.gz/download)

Extract the `.tar.gz` into `/opt/`

```shell
export RPI_TOOLCHAIN_DIR=/opt/cross-pi-gcc-10.3.0-0/
```
Note:
  - Everytime a new terminal is opened, you must run this command

# Using This Project

Download the project
```shell
git clone https://github.com/fprime-community/fprime-artemis-cubesat.git
```

Change directories to the project
```sh
cd fprime-artemis-cubesat
```

This project also uses submodules, so you will need to navigate to where you cloned the project and run 
```shell
git submodule update --init --recursive
```
Next, install the required F´ tools version 
```shell
pip install -r fprime/requirements.txt
```
Finally, install the `artemis-cubesat` Arduino Library

```shell
arduino-cli lib install artemis-cubesat
```

## Building the Project

The next step is to set up and build the project. 

```bash
# In fprime-artemis-cubesat
fprime-util generate
fprime-util build
```
> `fprime-util build` has two available arguments 'teensy41' and 'raspberrypi' 

> `fprime-util generate` sets up the build environment for a project/deployment. It only needs to be done once. It also has two available arguments 'teensy41' and 'raspberrypi'. 

## Combine RPi and Teensy Dictionaries

```shell
cp docs/scripts/combine_dictionaries ~/.local/bin
chmod +x ~/.local/bin/combine_dictionaries
```
Note:
  - You only need to run this script once. 

```shell
combine_dictionaries build-artifacts/teensy41/TeensyFSW/dict/TeensyFSWTopologyAppDictionary.xml build-artifacts/raspberrypi/RpiFSW/dict/RpiFSWTopologyAppDictionary.xml dictionary.xml
```

# Programming and Running on Hardware

## Uploading hex file for the Teensy
The Teensyduino application should have appeared after running `fprime-util build teensy41`. Choose the hex file to load into Teensyduino located in `./build-artifacts/teensy41/TeensyFSW/bin/`. Manually press the reset button on the Teensy to upload the program.

## Using GDS over serial
```sh
fprime-gds -n --dictionary dictionary.xml --comm-adapter uart --uart-device /dev/ttyACM0 --uart-baud 115200
```
Note:
  - If you have more than one device connected, or if you are using a different OS, `/dev/ttyACM0` may differ for your system.

[comment]: <> (using the rpi deployment)

