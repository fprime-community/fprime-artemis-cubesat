# F' Deployment for the Artemis Cubesat Kit

This repository contains the flight software developed for the Artemis CubeSat, a spaceflight-ready 1U CubeSat designed with the goal of enhancing space accessibility. Our mission is to minimize barriers to entry by not only reducing costs but also streamlining timelines and addressing integration challenges.

For more detailed information about the Artemis CubeSat, please visit our [website](https://sites.google.com/mahinaaerospace.com/home).

This project offers two installation options: utilizing Docker or opting for a manual installation. We highly recommend the Docker environment since it streamlines the process by incorporating all necessary dependencies and installations needed for development. However, if you prefer a manual installation, detailed instructions for that alternative are readily available.

# Prerequisistes: 

Before proceeding with either installation method, ensure that you have completed all necessary steps required to work with the Teensy and Raspberry Pi:

- Linux, macOS, or WSL1 on Windows
  - *On Windows, WSL1 is a requirement; WSL2 is not compatible.*
  - *On MacOS, manual installation prevents native RPiFSW compilation.*
- [Visual Studio](https://code.visualstudio.com/Download), an integrated development environment (IDE). With the [FPP](https://marketplace.visualstudio.com/items?itemName=jet-propulsion-laboratory.fpp), [Dev Containers](vscode:extension/ms-vscode-remote.remote-containers) (if you are using Docker), and [WSL](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl) (if you are using WSL) extensions.
- [Arduino IDE and Teensyduino](https://www.pjrc.com/teensy/td_download.html)
- [A configured Raspberry Pi Zero](/docs/SettingUpRPi.md)

# Installation Methods

Please refer to the relevant set of instructions that aligns with your preferred installation method:

- [Docker Installation](#Docker-Installation)
- [Manual Installation](/docs/ManualInstallation.md)

Upon successfully installing the project through one of the aforementioned methods, proceed to the supplied instructions for building and deploying the project.

- [Building and Running the Project on Hardware](#building-the-project-1)

# Docker Installation

Prior to proceeding with the Docker installation instructions, ensure that you have the [Docker Desktop application](https://www.docker.com/products/docker-desktop/) installed and properly configured.

## Clone the Repository 

1. Open a command line interface.
2. Navigate to the directory where you want to clone the F' - Artemis Cubesat repository. 

For example:
```shell
cd /path/to/your/desired/directory
```
Clone the F'- Artemis Cubesat repository: 

```shell
git clone https://github.com/fprime-community/fprime-artemis-cubesat.git
```

## Build the Docker Image

1. In your command line interface, navigate to the directory containing the recently clone F' - Artemis Cubesat repository:

```shell
cd /path/to/fprime-artemis-cubesat
```

2. Build the Docker image:
```shell
docker build -t fprime-artemiscubesat-image .
```
## Start a Docker Container

1. Run the following command to start a Docker container using the image you just built:
```shell
docker run -it --name fprime-artemiscubesat fprime-artemiscubesat-image /bin/bash
```

## Open the Source Code
- Open [Visual Studio Code](https://code.visualstudio.com/)
- If you havent already, install the extension: [Dev Containers](vscode:extension/ms-vscode-remote.remote-containers)
- Click *Open a Remote Window* on the bottom left corner of Visual Studio Code. The icon looks like this:  <sub>></sub><sup><</sup>
- Select *Attach to Running Container*. Select */fprime-artemiscubesat*
- Select *Open...* and go to */fprime-artemis-cubesat/* 
- Click *OK*
- Open a new terminal with *Terminal->New Terminal*

After installing the project and its dependenices continue with the [Building and Running the Project on Hardware](#building-the-project-1) instrcutions. 

# [Building and Running the Project on Hardware](#building-the-project-1)

## Building for Hardware

After successfully following one of the installation methods and ensuring that the project is fully set up, along with all its dependencies, proceed with the following steps to commence building the project.

```bash
# In fprime-artemis-cubesat
fprime-util generate
fprime-util build
```
> `fprime-util build` has two available arguments 'teensy41' and 'raspberrypi' append the appropriate one for the desired deployment to build. 

> `fprime-util generate` sets up the build environment for a project/deployment. It also accepts 'teensy41' and 'raspberrypi' as arguments.

### Combine RPi and Teensy Dictionaries

To use the Ground Data System (GDS) with the Artemis CubeSat, it is essential to merge the dictionaries from each of the two On-Board Computers (OBCs), each associated with different deployments, into a single comprehensive dictionary.

To merge the RPi and Teensy dictionaries in your project, navigate to the root directory and run the following command to combine them. Ensure that you have previously built and generated deployments for both RPi and Teensy before running the script:

```shell
python3 scripts/combine_dictionaries build-artifacts/teensy41/TeensyFSW/dict/TeensyFSWTopologyAppDictionary.xml build-artifacts/raspberrypi/RpiFSW/dict/RpiFSWTopologyAppDictionary.xml dictionary.xml
```

Note:
  - You'll just have to execute this script once, unless you make changes to the topology. In that case, you'll need to run it again.

If you are using the Docker method, you'll need to transfer the `dictionary.xml` file to your host machine. To achieve this, simply run the following command. Make sure to replace `DOCKER_ID` with your container's ID and `/path/to/directory/` with the destination folder where you intend to store the file:

```bash
docker cp DOCKER_ID:/root/fprime-artemis-cubesat/dictionary.xml /path/to/directory/
```

Note:
  - You can find your docker container's ID by using the command:
  ```bash
  docker ps
  ```
  - All  `docker` commands must be run on your host machine's terminal, not inside the docker contianer. 

# Running on Hardware

## Running on the Teensy

### Native Linux
If you are compiling natively on Linux, the Teensyduino application should appear. Choose the hex file located in ./build-artifacts/teensy41/TeensyFSW/bin/ to load into Teensyduino. Manually press the reset button on the Teensy to upload the program.

### Docker Container
- If you are using the Docker container, you'll need to transfer the `TeensyFSW.hex` file to your host machine. To achieve this, simply run the following command in your host machines command line interface. Make sure to replace `DOCKER_ID` with your container's ID and `/path/to/directory/` with the destination folder where you intend to store the file. 

```bash
docker cp DOCKER_ID:/root/fprime-artemis-cubesat/build-artifacts/teensy41/TeensyFSW/bin/TeensyFSW.hex /path/to/directory/
```

After successfully transferring the file or after locating the file on your host machine, follow these steps:

1. Launch the 'Teensy Loader' application.
2. Click on the 'Open HEX File' button.
3. Locate the Teensy .hex file you copied over previously
4. Load the identified .hex file into the application.
5. Manually press the reset button on the Teensy to initiate the program upload.

### WSL
- TODO: Transfer from WSL to Host Machine

## Running on the Raspberry Pi

### Native Linux 

If you are building natively on Linux and want to transfer the vuild file to the Raspberry Pi, follow these steps. This assumes that your Raspberry Pi is connected to your computer, and you can access it locally via SSH.

After a successful build, find the compiled `RpiFSW` file that you want to transfer to your Raspberry Pi. This file is located in `/fprime-artemis-cubesat/build-artifacts/raspberrypi/RpiFSW/bin/`

Transfer the file to Raspberry Pi:
Use the scp command to copy the file to your Raspberry Pi. Replace the paths and filenames accordingly. This command assumes that your Raspberry Pi is connected to your computer and you are able to access it locally via SSH.


```shell
scp /path/to/your/project/bin/RpiFSW pi@raspberrypi.local:~
```

### Docker Container 
- If you are using the Docker container, you'll need to transfer the `RpiFSW` file to your Raspberry Pi. To achieve this, simply run the following command. This command assumes that your Raspberry Pi is connected to your computer and you are able to access it locally via SSH.

```bash
scp /root/fprime-artemis-cubesat/build-artifacts/raspberrypi/RpiFSW/bin/RpiFSW pi@raspberrypi.local:~
```

### Running the Build File

Once you have transfered the build file over you can run it by navigating to where the build file is located and running following command on the Raspberry Pi:

```shell
cd /path/to/build/file
./RpiFSW
```

## Using GDS over Serial

### Teensy

To use GDS over serial, run the following command:
```sh
fprime-gds -n --dictionary /path/to/dictionary.xml --comm-adapter uart --uart-device /dev/TEENSY_NAME --uart-baud 115200
```
Note:
  - Make sure to edit the command to reflect the correct path to your `dictionary.xml.` file and replace `TEENSY_NAME` with the correct name for your Teensy. 
  
  You can find your Teensy's name on MacOS/Linux using:
  ```bash
  ls /dev/tty* 
  ```
### Raspberry Pi

TODO

