#!/bin/bash

# Create the udev rules directory if it doesn't exist
mkdir -p /etc/udev/rules.d

# Copy the custom udev rule to the rules directory
cp /tmp/00-teensy.rules /etc/udev/rules.d/00-teensy.rules
