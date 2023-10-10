# F' Deployment for the Artemis Cubesat Kit

This repository contains the flight software for the Artemis CubeSat, a cost-effective, spaceflight-ready 1U CubeSat designed to expand accessibility to space and aerospace engineering education.

For more detailed information about the Artemis CubeSat, please visit our [website](https://sites.google.com/mahinaaerospace.com/home).

This project offers two installation options: you can either utilize Docker or opt for a manual installation. Please consult the respective instructions that align with your preferred installation method.

- [Docker Installation](#Docker-Installation)
- [Manual Installation](#manual-installation)

After successfully installing the project using one of the methods mentioned above, proceed with the provided instructions to build and deploy the project.

- [Building and Running the Project on Hardware](#building-the-project-1)

# Docker Installation

We recommend using this deployment via Docker. By using Docker containers you will get all the F' and ACK project dependencies automatically resolved.

Prior to proceeding with the installation instructions, ensure that you have the [Docker Desktop application](https://www.docker.com/products/docker-desktop/) installed and properly configured.

## Clone the Repository 

1. Open your terminal or command prompt.
2. Navigate to the directory where you want to clone the repository using the cd command. 

For example:
```shell
cd /path/to/your/desired/directory
```
Clone the F'- Artemis CubeSat repository: 

```shell
git clone https://github.com/fprime-community/fprime-artemis-cubesat.git
```

## Build the Docker Image

1. In the terminal, navigate to the directory containing the existing Dockerfile within the cloned repository.

2. Run the following command to build a Docker image:
```shell
docker build -t fprime-artemiscubesat-image .
```
## Start a Docker Container

1. Run the following command to start a Docker container using the image you just built:
```shell
docker run -it --name fprime-artemiscubesat fprime-artemiscubesat-image /bin/bash
```
* `docker run`, This is the Docker command used to create and start a new container based on a specified image.
* `-it`, Run the container in interactive mode
* `--name fprime-artemiscubesat`, This option specifies a name for the container, in this case `fprime-artemiscubesat`
* `fprime-artemiscubesat-image`, This is the name of the Docker image that the container is based on. 
* `/bin/bash`, This is the command that is executed inside the container when it starts. In this case, it's running the Bash shell (/bin/bash) in an interactive mode, which allows you to enter commands and interact with the container.

## Exiting the Docker Container
When you're finished working inside the Docker container, you can exit the container's interactive shell. To do this, simply type:
```shell
exit
```
This will return you to your host machine's terminal.

You can also use the docker desktop to stop/start the container. 

## Cleaning Up

If you want to remove the Docker container and the associated Docker image, you can follow these steps:

1. Open a new terminal window on your host machine (not inside the Docker container).
2. Run the following command to stop and remove the Docker container:
```shell
docker rm -f fprime-artemiscubesat
```
Next, remove the Docker image:
```shell
docker rm -f fprime-artemiscubesat-image
```
Now, your system should be clean of the F' Artemis CubeSat Docker container and image.

## Open the Source Code
- Start [Visual Studio Code](https://code.visualstudio.com/)
- Install the extension: [Dev Containers](vscode:extension/ms-vscode-remote.remote-containers)
- Click *Open a Remote Window* on the bottom left corner of Visual Studio Code. The icon looks like this:  <sub>></sub><sup><</sup>
- Select *Attach to Running Container*. Select */fprime-artemiscubesat*
- Select *Open...* and go to */fprime-artemis-cubesat/* 
- Click *OK*
- Open a new terminal with *Terminal->New Terminal*

## Building the Project

After installing the project and its dependenices continue with the [Building and Running the Project on Hardware](#building-the-project-1) instrcutions. 
# Manual Installation

## Starting Requirements

Before attempting to install F', please ensure that you have the following prerequisites:

1. Operating System: Linux or Windows Subsystem for Linux (WSL)
2. Git
3. CMake 3.16 or newer (command-line tool must be accessible from the system path)
4. CLang or GNU C and C++ compilers (e.g., gcc and g++)
5. Python 3.8+ with virtual environments and PIP installed
6. Curl
7. Nano (or your choice of command line text editor)

### Installing F'

To install F', running the following command:

```shell
pip install -U fprime-tools
```
Once you have successfully met these prerequisites, you can proceed with the project instructions.

## Project Dependencies

### Installing arduino-cli

To get started, you'll need to download and install arduino-cli:
```shell
mkdir -p ~/.local/bin ; curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=~/.local/bin sh
```

### Adding `~/.local/bin` to your PATH

To make arduino-cli easily accessible from your terminal, add ~/.local/bin to your PATH. This command will first append the line `export PATH=~/.local/bin:$PATH` to the .bashrc file and then immediately source the .bashrc file to apply the changes:

```shell
echo 'export PATH=~/.local/bin:$PATH' >> ~/.bashrc && source ~/.bashrc
```

### Installing Arduino-CLI Wrapper

Next, you'll need to install the arduino-cli-cmake-wrapper:
```shell
pip install arduino-cli-cmake-wrapper
```

### Setting Up Arduino-CLI for Your Arduino Boards

Now, configure Arduino-CLI for the Artemis CubeSats arduino board:

Initialize the arduino-cli configuration file:
```shell
arduino-cli config init
```

For Teensy boards, add the board manager URL:
```shell
arduino-cli config add board_manager.additional_urls https://www.pjrc.com/teensy/package_teensy_index.json
```

Install the new board packages:
```shell
arduino-cli core update-index
arduino-cli core install teensy:avr
```

### Adding udev rules (Linux Only)
If you're using Linux, you'll need to add udev rules for the Teensy. This command will download the `.rules` files from [here](https://github.com/fprime-community/fprime-baremetal-reference/tree/main/docs/rules/00-teensy.rules) and save them into `/etc/udev/rules.d/`.

```shell
mkdir -p /etc/udev/ && curl -o /etc/udev/rules.d -L https://github.com/fprime-community/fprime-baremetal-reference/raw/main/docs/rules/00-teensy.rules
```

### Raspberry Pi Tooolchian 

For the Raspberry Pi deployment, you'll need to set up the cross-compiler toolchain. To do so download the [Raspberry Pi Zero cross-compiler toolchain](https://sourceforge.net/projects/raspberry-pi-cross-compilers/files/Raspberry%20Pi%20GCC%20Cross-Compiler%20Toolchains/Bullseye/GCC%2010.3.0/Raspberry%20Pi%201%2C%20Zero/cross-gcc-10.3.0-pi_0-1.tar.gz/download):

```shell
curl -LJO https://sourceforge.net/projects/raspberry-pi-cross-compilers/files/Raspberry%20Pi%20GCC%20Cross-Compiler%20Toolchains/Bullseye/GCC%2010.3.0/Raspberry%20Pi%201%2C%20Zero/cross-gcc-10.3.0-pi_0-1.tar.gz
```

Extract the `.tar.gz` into `/opt/`:

```shell
tar -xzvf cross-gcc-10.3.0-pi_0-1.tar.gz -C /opt/
```
Then, set the RPI_TOOLCHAIN_DIR environment variable:
```shell
export RPI_TOOLCHAIN_DIR=/opt/cross-pi-gcc-10.3.0-0/
```
Note:
  - You'll need to run this command every time you open a new terminal.

### Cloning & Setting up the F' Artemis Cubesat Deployment

Download the project into a directory of your choice:
```shell
git clone https://github.com/fprime-community/fprime-artemis-cubesat.git
```

Change directories to the project:
```sh
cd fprime-artemis-cubesat
```

This project uses submodules, so navigate to where you cloned the project and run:
```shell
git submodule update --init --recursive
```
Install the required F' tools version: 
```shell
pip install -r fprime/requirements.txt
```
Finally, install the artemis-cubesat Arduino Library:

```shell
arduino-cli lib install artemis-cubesat
```

# Building the Project

After successfully following one of the installation methods and ensuring that the project is fully set up, along with all its dependencies, proceed with the following steps to commence building the project.

```bash
# In fprime-artemis-cubesat
fprime-util generate
fprime-util build
```
> `fprime-util build` has two available arguments 'teensy41' and 'raspberrypi' 

> fprime-util generate` sets up the build environment for a project/deployment. It only needs to be done once and also accepts 'teensy41' and 'raspberrypi' arguments.

### Combine RPi and Teensy Dictionaries

Run this script to combine RPi and Teensy dictionaries:

```shell
cp docs/scripts/combine_dictionaries ~/.local/bin
chmod +x ~/.local/bin/combine_dictionaries
```

```shell
combine_dictionaries build-artifacts/teensy41/TeensyFSW/dict/TeensyFSWTopologyAppDictionary.xml build-artifacts/raspberrypi/RpiFSW/dict/RpiFSWTopologyAppDictionary.xml dictionary.xml
```

Note:
  - You only need to run this script once. 

# Programming and Running on Hardware

## Uploading hex file for the Teensy
After running fprime-util build teensy41, open up the Teensyduino application. Choose the hex file located in ./build-artifacts/teensy41/TeensyFSW/bin/ to load into Teensyduino. Manually press the reset button on the Teensy to upload the program.

## Using GDS over serial

To use GDS over serial, run the following command:
```sh
fprime-gds -n --dictionary dictionary.xml --comm-adapter uart --uart-device /dev/ttyACM0 --uart-baud 115200
```
Note:
  - If you have more than one device connected, or if you are using a different OS, `/dev/ttyACM0` may differ for your system.

[comment]: <> (using the rpi deployment)

