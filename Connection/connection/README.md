| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
# __connection Project__

**This project demonstrates UART communication between two ESP32 devices. It includes functions for sending and receiving 'charArray', 'char', and 'uint8' data types,and functions for control singnal:sendRequest、acceptRequest、sendInit、ackInit、sendSync、ackSync ,as well as functions for clearing the buffer, setting a timeout, and checking buffer availability.**
**To achieve this, the RX and TX pins of the UART are connected to GPIO16 and GPIO17 respectively.**

## Installation and Setup  
To set up the project, follow these steps:  
For windows users:step 3,4,5,6 can use gui
1. _Set up the ESP-IDF environment:_ 
   Follow the [ESP-IDF setup guide](https://docs.espressif.com/projects/esp-idf/en/release-v5.2/esp32/get-started/index.html) for your operating system.

2. _Clone the repository and navigate to the project directory:_  
    ```bash
   git clone git@github.com:${Username}/NTUEECamp_2024_Gameconsole.git #using ssh    
   cd NTUEECamp_2024_Gameconsole/Connection/connection
3. _Copy the project:_  
    copy the project in your device and open the project in your workplace
4. _Build the project:_
    ```bash
    idf.py build
5. _Flash the firmware to your ESP32:_
    ```bash
    idf.py -p /dev/ttyUSB0 flash

Replace /dev/ttyUSB0 with your serial port.

6. _Monitor the serial output using:_
    ```bash
    idf.py -p /dev/ttyUSB0 monitor

## Project Structure
The connection Project consists of two source files and one header file written in C. The main application logic is implemented in [main.c](main/main.c), located within the main directory.<br><br> Additionally, the functionality is extended by [uartFunction.c](components/uartFunction/src/uartFunction.c) and its corresponding header file [uartFunction.h](components/uartFunction/include/uartFunction.h).<br><br>
This project utilizes ESP-IDF and is built using the CMake build system. The configuration for building the project can be found in the three CMakeLists.txt file.

The project is organized as follows:

connection/  
├── components/  
│  &nbsp;&nbsp;&nbsp;&nbsp; └── uartFunction/  
│      &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; ├── CMakeLists.txt  
│       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;├── include/  
│       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;│   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── uartFunction.h  
│       &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── src/  
│           &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;└── uartFunction.c  
├── main/  
│   &nbsp;&nbsp;&nbsp;&nbsp;├── CMakeLists.txt  
│   &nbsp;&nbsp;&nbsp;&nbsp;└── main.c  
├── CMakeLists.txt  
└── README.md&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This is the file you are currently reading

## Hardware Setup and Usage
Hardware setup:  
    &nbsp;&nbsp;&nbsp;&nbsp;1.Connect GPIO16 of the first ESP32 to GPIO17 of the second ESP32.    
    &nbsp;&nbsp;&nbsp;&nbsp;2.Connect GPIO17 of the first ESP32 to GPIO16 of the second ESP32.    
    &nbsp;&nbsp;&nbsp;&nbsp;3.Connect GND of both ESP32 devices together.  
Usage:  
    &nbsp;&nbsp;&nbsp;&nbsp;1.Run the program on both ESP32 devices.  
**If you want to use uartFunction in your project:  
    &nbsp;&nbsp;&nbsp;&nbsp;1. Copy the folder "uartFunction" into your "components" folder  
    &nbsp;&nbsp;&nbsp;&nbsp;2. Ensure your CMakeLists.txt in the main directory contains "REQUIRES uartFunction"**
## Expected Output
Upon running the program, you should see the following output:

    data try data try sign try !@#$^ &*()
    85
    y
    1
    1
    1
    
This output confirms that the data transmission and reception between the two ESP32 devices are functioning as expected.


