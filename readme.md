The code runs on a ATSAMV71.

## Tasks
### Port.c
- Initializing the PCB pins and sets a default state. 
- Defining the pins’ physical locations 
### Main.c
- Starting the project. 
- Initializing other components, including the console which is over UART, a shell that the console can be used for CLI, for I2C ( TWI), Ethernet, and the DAC 
- Calling the task scheduler since the code is running on a RTOS system.
### Dac.c
- Controlling the DAC. 
- Calculating the wanted load to be generated and sends it to the DAC. 
- Defining functions, structures, and variables for other c files to use.