# kw565-coursework-5
**This is the assignment 5 of Cambridge Uni module 4B25 Embedded Systems for the IoT <br />
CRSid: kw565 <br />
Name: Kui Wang <br />
College: UCL <br />**

## Project Description
This project is to build a game system that mainly includes a hand motion controller, an OLED display. The hand motion controller is implemented by CortexM0+ MCU reading data from accellerometer to detect hand motion. The hand motion data is then used to direct a blue arrow shotting red heart in a simple game. The game is displayed on OLED screen.<br />
Note. There are still some image processing bugs in the game. It needs more time to test for a perfect display. In additions, the memory space is nearly full, therefore, marks in the game is not implemented in this version. 

## Repository Structure
**kw565-coursework-5**
- **armgcc<br />** 
    folder containing scripts to build this project and binary file ready to be loaded on hardware.<br />
- **lib<br />** 
    folder containing platform library in both "debug" and "release" sub-folders.<br />
- **platform<br />** 
    folder containing commonly used algorithm and functions for MKL03Z board.<br />
- **README.md<br />** 
    file explaining this project and repository structure.<br />
- **source<br />** 
    folder containing main source files of this project.The main.c and hardareinit.c files are the major work. While the other c files are referenced from Freescale's spi_loop_back, I2C and GPIO example projects. <br />
- **tools<br />** 
    folder containing armgcc toolchain files.<br />
- **kw565-coursework-5.pdf<br />** 
    Final report for the project.<br />

## Instructions to Re-build Project:
1. Install armgcc. <br />
2. Install cmake. <br />
3. Download this repository from github to local computer. <br />
4. Set ARMGCC_DIR by executing ```export ARMGCC_DIR=/Applications/ARM```;"/Applications/ARM" should be modified to your own armgcc folder. <br />
5. Change current working folder to repository subfolder /kw565-coursework-5/armgcc; Execute ```./build_all.sh``` to rebuild all executable files. The generated binary files /kw565-coursework-5/armgcc/debug/kw565-coursework-5.bin or /kw565-coursework-5/armgcc/release/kw565-coursework-5.bin could be loaded onto hardware using JLinkExe later.
6. Connect MKL03Z board to PC through USB cable. <br />
7. Open JLinkExe in another terminal window by executing ```/Applications/SEGGER/JLink/JLinkExe -device MKL03Z32XXX4 -if SWD -speed 100000```. The path to JLinkExe should be modified according to your local JLinkExe installation path. <br />
8. Execute ```r``` to reset MCU in JLinkExe. <br />
9. Execute ```loadfile /kw565-coursework-5/armgcc/debug/kw565-coursework-5.bin``` to load binary into hardware. Again, the binary file path need to be modified according to your local path. **The binary file in debug folder is the suggested and tested version, other than the one in release folder** <br />
10. Execute ```r``` to reset MCU again in JLinkExe. <br />
11. Execute ```go``` to run application. Now you should see red heart falling down from the top of OLED screen. Control the arrow on screen by moving FRDM-KL03Z board and shot arrow by pressing switch 3 on board. Have fun!
