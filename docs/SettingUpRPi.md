# Setting up the Raspberry Pi Zero

# Getting started 

The subsequent instructions will guide you through the process of setting up the Raspberry Pi for the Artemis CubeSat Kit. This involves configuring the Raspberry Pi to operate in headless mode, wherein the operating system functions without a graphical display, and all control is executed through a command line interface.

# Installing the RPI’s operating system

To initiate the Raspberry Pi setup, begin by installing an operating system onto its SD Card. We recommend employing the Raspberry Pi Imager for this task. Developed by Raspberry Pi, this graphical SD card writing tool is compatible with Mac OS, Ubuntu 18.04, and Windows. It provides a straightforward option for most users, automatically downloading the image and installing it onto the SD card. All you require is a computer equipped with an SD card reader or an adapter to facilitate the image installation process.

1. Download the [Raspberry Pi Imager](https://www.raspberrypi.com/software/)
2. Insert the micro SD card into your computer
3. Open the Raspberry Pi Imager application
4. Select `CHOOSE DEVICE` 
5. Select `Raspberry Pi Zero`
6. Next, select `CHOOSE OS` 
7. Select `Raspberry Pi OS (Legacy) Lite`, you can find it in the `Raspberry Pi OS (other)` section
    - To utilize the Raspberry Pi Camera with F', it is imperative to have the Bullseye OS installed. Confirm that the operating system explicitly indicates `A port of Debian Bullseye` to ensure compatibility.
8. Select `CHOOSE STORAGE` and choose the SD card you wish to write your image to
9. Then, select `NEXT`
10. On the popup that has appeared select `EDIT SETTINGS`
    - This will open an `OS Customization` menu, enabling you to pre-configure settings on the Raspberry Pi.
10. In this menu there is going to be a few things that you need to select:
    - Select the checkbox next to `Set hostname`, leave the default name
    - Select the checkbox next to `Set username and password`, and proceed to define the desired username and password. For the purposes of these instructions, we will use the following, although you are free to customize it as you prefer:
        - Username: pi
        - Password: pi
    - Select the checkbox next to `Set locale settings` and select your timezone and preffered keyboard layout.
    - In the `SERVICES` tab select the checkbox next to `Enable SSH`, leave the default selections
    - Once you have completed all the aforementioned steps, click on `SAVE`

With the SD card settings configured and verified according to the above steps, proceed to select `YES` This action will inscribe the configured settings and the operating system onto your SD card.

*Note: If using Raspberry Pi Imager on Windows 10 with controlled folder access enabled, you will need to explicitly allow Raspberry Pi Imager permission to write the SD card. If this is not done, the imaging process will fail with a "failed to write" error.*

# Enabling SSH over USB
For communication with the Raspberry Pi, we will utilize SSH, also known as Secure Shell. SSH is a network protocol that provides a secure means to access a computer over an unsecured network.

Enabling SSH on the Raspberry Pi allows for remote control from another computer. To achieve this, configure the Raspberry Pi to recognize the USB port as an Ethernet port. Begin by mounting the micro SD card in a computer and opening it in a code editor.

1. Once you have the SD card files open in your code editor, locate and open a file situated at the root of the SD card named `config.txt`.

    Within this file, append the following line at the very bottom:

    ```shell
    dtoverlay=dwc2
    ```

    Save the file. 

2. The aforementioned line prepares us for the modification of the next file, `cmdline.txt`, with a slight variation. Unlike newlines or commas, parameters in this file are delimited by space characters.

    After "rootwait," append the following text, ensuring there is only one space between "rootwait" and the new text to ensure proper parsing:

    ```shell
    modules-load=dwc2,g_ether
    ```

    If there was any text following the new text, ensure there is only one space between that existing text and the new text.

    Save the file.

    *Note: The formatting of cmdline.txt is highly strict. Commands must be separated by spaces, and newlines are not permitted.*

3. Finally, create a new file inside the SD card called `ssh`.
    
    Save the file.


Having made these edits to the files, eject the SD card and insert it into the Raspberry Pi.    

# Accessing the RPi through SSH

*Note: The following steps will be different based on your host machine’s operating system.* 

## MacOS

If you are using macOS, no additional software is required.

Power on the Pi Zero using the USB data cable. Ensure you connect it to the port labeled USB, not PWR, as this port enables both powering the Pi Zero and facilitating data transfer. Allow some time for initial configuration once connected.

After waiting for a moment, input the following command in your Mac's Terminal:

```shell
ssh pi@raspberrypi.local
```

Note: In this example, `pi` is the username we utilized. If you modified the username during the previous steps, remember to adjust the command accordingly to reflect the username you set.

If prompted with a warning just type `yes` and then hit enter.

This command will prompt you for the Raspberry Pi's password. Enter the password—this should be the password you set during the previous steps.

You are now connected to your Raspberry Pi.

## Windows

For Windows you will need to install [Bonjour Print Services for Windows v2.0.2](https://support.apple.com/kb/dl999?locale=en_US).

Power on the Pi Zero using the USB data cable. Ensure you connect it to the port labeled USB, not PWR, as this port enables both powering the Pi Zero and facilitating data transfer. Allow some time for initial configuration once connected.

Navigate to the device manager and examine the section labeled "Network adapters." Verify if there is a device labeled "USB Ethernet/RNDIS Gadget." If it is present, you are good to proceed; if not, please refer to the troubleshooting section at the bottom, which addresses common issues and provides solutions.

After waiting for a moment, input the following command in your command line interface:

``` shell
ssh pi@raspberrypi.local
```

Note: In this example, `pi` is the username we utilized. If you modified the username during the previous steps, remember to adjust the command accordingly to reflect the username you set.

If prompted with a warning just type `yes` and then hit enter.

This command will prompt you for the Raspberry Pi's password. Enter the password—this should be the password you set during the previous steps.

You are now connected to your Raspberry Pi.

# Sharing Internet Between Host Machine and Raspberry Pi

Now that you can interact with your Raspberry Pi by following the previous steps, let's explore how to share the internet from your host device.

Before proceeding, it is assumed that you have configured your Raspberry Pi Zero and can successfully SSH into the device without an internet connection. If you haven't reached this point, please follow the previous instructions.

The upcoming instructions will be categorized by operating system, so please refer to the instruction set corresponding to your OS.

## macOS

Begin by connecting your RPI to your computer using the USB port labeled USB, not PWR.

Once connected, access System Settings on your Mac and navigate to the sharing menu. Here, configure Internet Sharing.

The setup is straightforward. Choose the device you want to share your internet with; in our case, it's the Ethernet gadget, representing the Raspberry Pi Zero emulating Ethernet over USB. After selecting the device, ensure to enable internet sharing overall. There should be two checked boxes in total, and you will receive a confirmation dialog afterward.

After enabling internet sharing between your host computer and the device, you may need to restart your Raspberry Pi.

## Windows

To begin, connect your RPI to your computer using the USB port labeled USB, not PWR.

Next, press Win-X, go to Network Connections, and select Change adapter options.

Right-click the connection that provides internet access, click Properties, and navigate to the second tab labeled "Sharing." Choose "Allow other network users to connect through this computer’s Internet connection" and select the corresponding network from the drop-down menu. It should be the Ethernet connection named USB Ethernet/RNDIS Gadget #x in your Network Connections.

After clicking OK, your laptop/computer will function as a router for this connection. As a result, your SSH connection will drop, and you'll need to wait for DHCP to assign the Pi a new IP. After a minute or so, attempt to SSH into the RPI again. It should connect normally, and your RPI will now have internet access.

# Enabling serial connection

The last step in setting up the Raspberry Pi is enabling the serial connection, which is crucial for communication with the Teensy. To activate hardware serial on a Raspberry Pi from the terminal, follow these steps:


In the command line interface for your Raspberry PI run the 'raspi-config' utility: 

```shell
sudo raspi-config
```

Within the raspi-config menu, proceed with the following steps:
1. Select `Interface Options`
2. Select `Serial Port`
3. Choose `No` for "Would you like a login shell to be accessible over serial?"
4. Choose `Yes` for "Would you like the serial port hardware to be enabled?"
5. Go back to `Interface Options`
6. Select `SPI`
7. Choose `Yes` for "Would you like the SPI interface to be enabled?"
6. Navigate to `Finish` and press `Enter`
7. Select `Yes` to reboot when prompted

After the reboot, hardware serial will be enabled on your Raspberry Pi.

# Troubleshooting:

## Host Does Not Exist Windows 

If you encounter an error stating that the host does not exist when attempting to SSH into the RPi on a Windows system, follow these steps:

Go to the device manager and expand the section labeled "Other devices."
If you find a device labeled "USB Serial Device" under this section, it indicates that Ethernet drivers need to be installed for this device.
Right-click on the "USB Serial Device" and select "Update driver."
Choose "Browse my computer for drivers," then select "Let me pick from a list of available drivers on my computer."
Click on "Show compatible hardware," choose "USB Ethernet/RNDIS Gadget," and press next. The driver will automatically install, enabling Ethernet over USB and allowing you to SSH into the RPi.
In case the USB is not recognized by the system at all, indicated by the absence of the typical USB connect sound, this may be due to the local network's protection or potential misconfiguration of the "config" or "cmdline" files.

Take the SD card out of the RPi and insert it back into your computer. Open the Raspberry Pi Imager, re-select the correct operating system and SD card, and repeat the steps to install the RPi OS and enable SSH over USB. However, under "Advanced options," enable "configure wireless LAN." Input your local network's name and password (spelling matters), and then select the country your connection resides in through "Wifi country."

Follow the subsequent instructions as usual.

## Removing known_hosts

If you encounter the following message:

```shell
WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!
```

To resolve this, follow these simple steps:

Firstly, run this command to remove the previous host identification details:

```shell
rm -f .ssh/known_hosts
```

Then, attempt to re-establish your SSH connection. You will be presented with a new message:

```shell
"This key is not known by any other names. Are you sure you want to continue connecting (yes/no/[fingerprint])?"
``` 

Simply respond by typing 'yes' and hitting enter. The system will then prompt you for your password. Input it, and you're all set.

 
## Incorrect Time

If the date on your Raspberry Pi is incorrect and causing internet-related issues, here is a step-by-step guide to fix it. Remember to adjust the date and time in the command to the current date and time.
 
Verify Current Date and Time
To check the current date and time setting of your Raspberry Pi, use the command:

```shell
date
```
This will display the current date and time.

### Set Date and Time Manually

To set the date and time manually, use the date command with the -s option (which stands for 'set'). The format for the date and time should be 'YYYY-MM-DD HH:MM:SS'. Here's an example command:

```shell
sudo date -s '2023-05-17 14:00:00'
```

After making the changes, restart your Raspberry Pi for the changes to take effect:

```shell
sudo reboot
```

After rebooting, execute the date command once more to confirm that the changes have been successfully applied. Your Raspberry Pi should now display the correct date and time, allowing you to engage in internet-related activities without encountering any issues.


