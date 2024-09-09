# USB_HUB Project

## Overview

The **USB_HUB** is a custom-designed device that expands a single USB-C port into four USB-A ports. It is built using the **USB2514B-I/M2** controller, designed for **USB 2.0** communication. This USB hub is enhanced with additional features like power consumption monitoring for each of the four USB ports, displayed on a **0.96-inch IPS screen**. The device can also function as a timer, ideal for activities such as focused work sessions (e.g., a 45-minute concentration timer) or as a countdown for random tasks like boiling eggs :). Navigating through different options is done by using the knob located on the top of the case.
The hub's functionality is managed by an **STM32F103C8T6** microcontroller. 
This README outlines the hardware, software, and overall functionality of the project.

![Starting_Screen](images/starting_screen.jfif)
![Menu_Screen](images/menu.jfif)
![Power_Screen](images/power.jfif)
![Timer_Screen](images/timer.jfif)

## Main Components

- **USB2514B-I/M2**: This is the core USB 2.0 controller that provides a four-port USB hub.
- **STM32F103C8T6**: A microcontroller that handles display control, USB communication, and power monitoring logic.
- **0.96-inch IPS ST7789 Display**: A small display module used to show power consumption data and countdown timers. More details about this display module can be found in the [datasheet](http://www.lcdwiki.com/0.96inch_IPS_ST7789_Module).
- **INA219AIDR**: A current and power monitoring sensor responsible for measuring the power consumption of each USB port.
- **USB-C Input Port**: For connecting the device to a host device.
- **4x USB-A Output Ports**: For connecting USB devices

## Features

- **USB Hub**: Expands one USB-C port into four USB-A ports, supporting USB 2.0.
- **Power Monitoring**: Displays real-time power consumption for each of the four USB ports on the 0.96-inch screen.
- **Timer Functionality**: Can be used as a countdown timer, useful for focused work sessions or cooking tasks.
- **3D Printed Enclosure**: A custom 3D-printed enclosure with snap-fit cover design.


## Project Files

The project repository is organized into two main folders:

1. **USB_HUB_Hardware**
   - **STL Files**: 3D model files for the enclosure, including the base with M3 inserts and a snap-fit cover.
   - **Fabrication Files**: Design files for the PCB.
   - **Schematic**: The schematic showing all the components and connections.

2. **USB_HUB_Software**
   - The code uploaded to the STM32 microcontroller, written in STM32CUBEIDE.


## Enclosure Design

The enclosure for the USB hub is 3D-printed, uses M3 inserts and the top cover snaps into place.

![Case](images/case.jfif)
![Housing](images/housing.jfif)
![Lid](images/lid.jfif)

## PCB Design

This is a 4-layer PCB which is designed in EazyEDA software, schematic and fabrication files are located in USB_HUB_Hardware folder.
![Lid](images/PCB.jfif)
More details on PCB can be found [here](https://oshwlab.com/karlobrzica1997/new-project_2024-07-08_19-28-44).

## Special Thanks

A special thanks goes to **Phils_lab**, as this project is based on his original USB hub design. You can check out his informative video [here](https://www.youtube.com/watch?v=iR2hYpq0KI0&t=583s). I have added a unique twist to his design by incorporating power consumption monitoring and a countdown timer.
[Phil's Lab GitHub](https://github.com/pms67)

## Resources

- [ST7789-STM32 GitHub Floyd-Fish Repository](https://github.com/Floyd-Fish/ST7789-STM32)

## Additional Notes

Feel free to modify and expand on the project.
Use the files, hardware, and software given in this repository at your own risk


