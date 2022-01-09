# kw565-coursework-5
This is the assignment 5 of Cambridge Uni module 4B25 Embedded Systems for the IoT <br />
CRSid: kw565 <br />
Name: Kui Wang <br />
College: UCL <br />

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////<br />
Project description:
This project is to build a game system that mainly includes a hand motion controller, an OLED display. The hand motion controller is implemented by <br />CortexM0+ MCU reading data from accellerometer to detect hand motion. The hand motion data is then used to direct a blue arrow shotting red heart in a simple <br />game. The game is displayed on OLED screen.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////<br />
Repository structure:<br />
-- armgcc      folder containing scripts to build this project and binary file ready to be loaded on hardware.<br />
-- lib         folder containing platform library in both "debug" and "release" sub-folders.<br />
-- platform    folder containing commonly used algorithm and functions for MKL03Z board.<br />
-- README.md   file explaining this project and repository structure.<br />
-- source      folder containing main source files of this project.<br />
-- tools       folder containing armgcc toolchain files.<br />
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////<br />
Instructions to re-build this project: <br />
1. Install armgcc. <br />
2. Install cmake. <br />
3. Download this repository from github to local computer. <br />
4. Set ARMGCC_DIR by executing "export ARMGCC_DIR=/Applications/ARM"; "/Applications/ARM" should be modified to your own armgcc folder. <br />
5. Change current working folder to /kw565-coursework-5/armgcc; Execute "./build_all.sh" to rebuild all executable files. The generated binary files <br />/kw565-coursework-5/armgcc/debug/kw565-coursework-5.bin or /kw565-coursework-5/armgcc/release/kw565-coursework-5.bin could be loaded onto hardware using <br />JLinkExe later.
6. Connect MKL03Z board to PC through USB cable. <br />
7. Open JLinkExe in another terminal window by executing "/Applications/SEGGER/JLink/JLinkExe -device MKL03Z32XXX4 -if SWD -speed 100000". The path to <br />JLinkExe should be modified according to your local JLinkExe installation path. <br />
8. Execute 'r' to reset MCU in JLinkExe. <br />
9. Execute "loadfile /kw565-coursework-5/armgcc/debug/kw565-coursework-5.bin" to load binary into hardware. Again, the binary file path need to be <br />modified according to your local path. <br />
10. Execute 'r' to reset MCU again in JLinkExe. <br />
11. Execute 'go' to run application. Now you should see red heart falling down from the top of OLED screen. Control the arrow on screen by moving <br />FRDM-KL03Z board and shot arrow by pressing switch 3 on board. Have fun!
