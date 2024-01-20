# Use a base image with the latest Ubuntu OS
FROM ubuntu:latest

# Set non-interactive environment variables to prevent prompts during installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and upgrade the system to ensure it's up to date
RUN apt update && apt-get upgrade -y

# Install required packages and tools
RUN apt install -y \
    git \
    cmake \
    clang \
    curl \
    nano \
    default-jre \
    python3 \
    python3-pip \
    meson \
    ninja-build \
    pkg-config \
    libyaml-dev \
    python3-yaml \
    python3-ply \
    python3-jinja2 \
    build-essential \
    gdb \
    python3-venv \
    libssl-dev

# Upgrade pip and install fprime-tools (a set of tools for embedded systems development)
RUN pip3 install --upgrade pip && pip3 install -U fprime-tools

# Install Arduino CLI and configure it
RUN mkdir -p /root/.local/bin && \
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/root/.local/bin sh && \
    echo 'export PATH=/root/.local/bin:$PATH' >> ~/.bashrc

# Update the environment with the new PATH variable
ENV PATH=/root/.local/bin:$PATH

# Install the arduino-cli-cmake-wrapper and initialize Arduino CLI
RUN pip install arduino-cli-cmake-wrapper && \
    arduino-cli config init

# Add the Teensy board manager URL
RUN arduino-cli config add board_manager.additional_urls https://www.pjrc.com/teensy/package_teensy_index.json

# Update the core index
RUN arduino-cli core update-index

# Install the Teensy core
RUN arduino-cli core install teensy:avr

# Install udev rules for Teensy boards
RUN curl -o /tmp/00-teensy.rules -L https://github.com/fprime-community/fprime-baremetal-reference/raw/main/docs/rules/00-teensy.rules

# Copy the copy_rules script into the container
COPY docs/scripts/copy_rules.sh /copy_rules.sh

# Make the copy_rules script executable
RUN chmod +x /copy_rules.sh

# Install Raspberry Pi cross-compiler toolchain
RUN curl -LJO https://sourceforge.net/projects/raspberry-pi-cross-compilers/files/Raspberry%20Pi%20GCC%20Cross-Compiler%20Toolchains/Bullseye/GCC%2010.3.0/Raspberry%20Pi%201%2C%20Zero/cross-gcc-10.3.0-pi_0-1.tar.gz && \
    tar -xzvf cross-gcc-10.3.0-pi_0-1.tar.gz -C /opt/ && \
    echo 'export RPI_TOOLCHAIN_DIR=/opt/cross-pi-gcc-10.3.0-0/' >> ~/.bashrc

# Clone the fprime-artemis-cubesat repository and set it up
RUN git clone https://github.com/fprime-community/fprime-artemis-cubesat.git && \
    cd fprime-artemis-cubesat && \
    git checkout dev && git pull && \ 
    git submodule update --init --recursive && \
    pip3 install -r fprime/requirements.txt && \
    arduino-cli lib install artemis-cubesat

# Install libcamera dependencies and cross compile for ARM Linux
RUN cd fprime-artemis-cubesat/lib/raspberrypi/ && \
    echo 'export RPI_TOOLS=/opt/cross-pi-gcc-10.3.0-0/' >> ~/.bashrc && \
    echo 'export PATH=$RPI_TOOLS:$PATH' >> ~/.bashrc && \
    cd libcamera && \
    meson setup build -Dprefix=/fprime-artemis-cubesat/lib/raspberrypi/libcamera/build -Dpipelines=rpi/vc4 -Dipas=rpi/vc4 --cross-file ../libcamera-pi0-x86.txt && \
    cd build && \
    ninja && \
    ninja install && \
    echo 'export PKG_CONFIG_PATH=/fprime-artemis-cubesat/lib/raspberrypi/libcamera/build/lib/pkgconfig/' >> ~/.bashrc

# Set the working directory inside the container
WORKDIR /fprime-artemis-cubesat

# Specify the default command to run when the container starts
CMD [ "/bin/bash", "/copy_rules.sh"]
