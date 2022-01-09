# kw565-coursework-5
This is the assignment 5 of Cambridge Uni module 4B25 Embedded Systems for the IoT
CRSid: kw565
Name: Kui Wang
College: UCL

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Project description:
This project is to build a game system that mainly includes a hand motion controller, an OLED display. The hand motion controller is implemented by CortexM0+ MCU reading data from accellerometer to detect hand motion. The hand motion data is then used to direct a blue arrow shotting red heart in a simple game. The game is displayed on OLED screen.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Repository structure:
-- armgcc      folder containing scripts to build this project and binary file ready to be loaded on hardware.
-- lib         folder containing platform library in both "debug" and "release" sub-folders.
-- platform    folder containing commonly used algorithm and functions for MKL03Z board.
-- README.md   file explaining this project and repository structure.
-- source      folder containing main source files of this project.
-- tools       folder containing armgcc toolchain files.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Instructions to re-build this project:
1. Install armgcc
2. Install cmake
3. Download this repository from github to local computer
4. Set ARMGCC_DIR by executing "export ARMGCC_DIR=/Applications/ARM"; "/Applications/ARM" should be modified to your own armgcc folder.
5. Change current working folder to /kw565-coursework-5/armgcc; Execute "./build_all.sh" to rebuild all executable files. The generated binary files /kw565-coursework-5/armgcc/debug/kw565-coursework-5.bin or /kw565-coursework-5/armgcc/release/kw565-coursework-5.bin could be loaded onto hardware using JLinkExe later.
6. Connect MKL03Z board to PC through USB cable.
7. Open JLinkExe in another terminal window by executing "/Applications/SEGGER/JLink/JLinkExe -device MKL03Z32XXX4 -if SWD -speed 100000". The path to JLinkExe should be modified according to your local JLinkExe installation path.
8. Execute 'r' to reset MCU in JLinkExe.
9. Execute "loadfile /kw565-coursework-5/armgcc/debug/kw565-coursework-5.bin" to load binary into hardware. Again, the binary file path need to be modified according to your local path.
10. Execute 'r' to reset MCU again in JLinkExe.
11. Execute 'go' to run application. Now you should see red heart falling down from the top of OLED screen. Control the arrow on screen by moving FRDM-KL03Z board and shot arrow by pressing switch 3 on board. Have fun!
