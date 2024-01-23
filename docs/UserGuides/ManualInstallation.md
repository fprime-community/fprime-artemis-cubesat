# Manual Installation

## Starting Requirements

Before attempting to install F', please ensure that you have the following prerequisites:

1. Operating System: Linux or Windows Subsystem for Linux (WSL)
2. Git
3. CMake 3.16 or newer (command-line tool must be accessible from the system path)
4. CLang or GNU C and C++ compilers (e.g., gcc and g++)
5. Python 3.8+ with PIP installed
6. Curl
7. Default JRE (Java Runtime Environment)
8. Meson build system
9. Ninja build system
10. pkg-config
11. libyaml-dev (YAML library development files)
12. python3-yaml (Python bindings for YAML)
13. python3-ply (Python Lex & Yacc parsing tools)
14. python3-jinja2 (Python template engine)
15. Build-essential (essential packages for building software)
16. GDB (GNU Debugger)
17. python3-venv (Python 3 virtual environment support)
18. libssl-dev (SSL development libraries)

## Setting Up the Development Environment
The ecosystem of tools supporting F´ is installed as python packages available via PIP. 
To use these tools we will use a Python virtual environment which is helpful in isolating project dependencies, ensuring version control, and avoiding conflicts between different projects. It streamlines development by providing a clean and reproducible environment, making it easier to manage dependencies. 

Start by opening the appropriate command line interface for your operating system. 

*Note:  If you are utilizing WSL1, please ensure that you have the VS Code WSL extension installed. Next, launch VS Code, press F1, and choose "WSL: Connect to WSL" from the menu.*

Create a project folder and navigate to it:

```shell
mkdir fprime_project
cd fprime_project
```

Create the virtual environment:

```shell
python3 -m venv fprime-venv
```

Activate the virtual environment

```shell
. fprime-venv/bin/activate
```

## Installing F'

*Note: Remember to activate the virtual environment whenever you work with this F´ project or start a new command line interface session.*

If you're not already inside the directory you created earlier , simply use the 'cd' command to navigate to the newly created 'fprime_project directory' in your command line interface.

*Note: Remember to activate the virtual environment whenever you work with this F´ project or start a new command line interface session.*

Once you are there the next step is to install the F’ tools: 

```shell
pip install -U fprime-tools
```
Once you have successfully met these starting requirements, you can proceed with the project instructions.

# Project Dependencies

*Note: Remember to activate the virtual environment whenever you work with this F´ project or start a new command line interface session.*

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

In your command line interface, navigate to the directory containing the recently fprime_project directory:

```
cd /path/to/fprime-artemis-cubesat
```

Then, clone the F' - Artemis Cubesat project:
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
Install the artemis-cubesat Arduino Library:

```shell
arduino-cli lib install artemis-cubesat
```

## Install libcamera Dependencies and Cross Compile for ARM Linux

Debian/Ubuntu packages are essential for building libcamera. If you have installed all the packages mentioned in the "Starting Requirements," you should have all the necessary components. In case you haven't, please refer to the [libcamera repository](https://github.com/raspberrypi/libcamera) for the required dependencies.

1. Change to the "lib/raspberrypi" directory
```shell
cd lib/raspberrypi
```

2. Clone the libcamera repository from GitHub
``` shell
git clone https://github.com/lukeclements/libcamera.git
```

3. Set the RPI_TOOLS environment variable to the specified path
``` shell
export RPI_TOOLS=/opt/cross-pi-gcc-10.3.0-0/
```

4. Add the RPI_TOOLS path to the system PATH
``` shell 
export PATH=$RPI_TOOLS:$PATH
```

5. Change to the "libcamera" directory
``` shell
cd libcamera
```

6. Set up the Meson build system with specific configuration options
``` shell
meson setup build -Dprefix=/fprime-artemis-cubesat/lib/raspberrypi/libcamera/build -Dpipelines=rpi/vc4 -Dipas=rpi/vc4 --cross-file ../libcamera-pi0-x86.txt
```

7. Change to the "build" directory
``` shell
cd build
```

8. Build the libcamera project using Ninja
``` shell 
ninja
```

9. Install the libcamera project
``` shell
ninja install
```
10. Set the PKG_CONFIG_PATH environment variable to the specified path
``` shell
export PKG_CONFIG_PATH=/root/fprime-artemis-cubesat/lib/raspberrypi/libcamera/build/lib/pkgconfig/
``` 

## Open the Source Code

TODO

<br>

<span style="font-size: 1.5em;">After completing the aforementioned instructions, you should have everything necessary to commence working with the F' - Artemis Cubesat project. You can now proceed to the supplied instructions for building and deploying the project.

