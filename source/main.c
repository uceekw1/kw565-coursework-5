/*
Assignment 5 for Embedded System for IoT (4B25)
A UCL & Cambridge student
Date: 8/1/2022

This project is to implement a game system, including a motion controller and
a OLED display. The motion controller controls direction of arrow to shot falling
heart from the top of screen. A mark for player is supposed to be shown on the
top-right corner of screen, however, it is not implemented at the end due to
limited memory space.
 */

///////////////////////////////////////////////////////////////////////////////
//  Includes
///////////////////////////////////////////////////////////////////////////////
// Standard C Included Files
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "board.h"
#include "fsl_spi_master_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_port_hal.h"
#include "fsl_gpio_driver.h"
#include <string.h>
#include "accel.h"
#include "gpio_pins.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define SPI_MASTER_INSTANCE         BOARD_SPI_INSTANCE 
#define TRANSFER_BAUDRATE           (10000000U)          

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t	inBuffer[32];
volatile uint8_t	payloadBytes[32];
uint32_t commandnum = 0;
volatile bool isButtonPress = false;
uint8_t shot = 0;
uint8_t x_arrow = 47;
uint8_t y_arrow = 63;

/*******************************************************************************
 * SPI Pin Assignment
 ******************************************************************************/
enum
{
	kSSD1331PinMOSI		= GPIO_MAKE_PIN(PORTA_IDX, 8), //orginal is 7
	kSSD1331PinSCK		= GPIO_MAKE_PIN(PORTA_IDX, 9),
	kSSD1331PinDC		= GPIO_MAKE_PIN(PORTA_IDX, 12),//new
	kSSD1331PinCSn          = GPIO_MAKE_PIN(PORTB_IDX, 10),
	kSSD1331PinRST		= GPIO_MAKE_PIN(PORTB_IDX, 0),//new
};

/*******************************************************************************
 * OLED Command Table
 ******************************************************************************/
typedef enum
{
	kSSD1331CommandDRAWLINE		= 0x21,
	kSSD1331CommandDRAWRECT		= 0x22,
	kSSD1331CommandCLEAR		= 0x25,
	kSSD1331CommandFILL		    = 0x26,
	kSSD1331CommandSETCOLUMN	= 0x15,
	kSSD1331CommandSETROW		= 0x75,
	kSSD1331CommandCONTRASTA	= 0x81,
	kSSD1331CommandCONTRASTB	= 0x82,
	kSSD1331CommandCONTRASTC	= 0x83,
	kSSD1331CommandMASTERCURRENT	= 0x87,
	kSSD1331CommandSETREMAP		= 0xA0,
	kSSD1331CommandSTARTLINE	= 0xA1,
	kSSD1331CommandDISPLAYOFFSET	= 0xA2,
	kSSD1331CommandNORMALDISPLAY	= 0xA4,
	kSSD1331CommandDISPLAYALLON	= 0xA5,
	kSSD1331CommandDISPLAYALLOFF	= 0xA6,
	kSSD1331CommandINVERTDISPLAY	= 0xA7,
	kSSD1331CommandSETMULTIPLEX	= 0xA8,
	kSSD1331CommandSETMASTER	= 0xAD,
	kSSD1331CommandDISPLAYOFF	= 0xAE,
	kSSD1331CommandDISPLAYON	= 0xAF,
	kSSD1331CommandPOWERMODE	= 0xB0,
	kSSD1331CommandPRECHARGE	= 0xB1,
	kSSD1331CommandCLOCKDIV		= 0xB3,
	kSSD1331CommandPRECHARGEA	= 0x8A,
	kSSD1331CommandPRECHARGEB	= 0x8B,
	kSSD1331CommandPRECHARGEC	= 0x8C,
	kSSD1331CommandPRECHARGELEVEL	= 0xBB,
	kSSD1331CommandVCOMH		= 0xBE,
} SSD1331Commands;

//uint8_t commandBytes[64] = {0};
/*******************************************************************************
 * writeCommand is to write one byte of command to OLED.
 ******************************************************************************/
void
writeCommand(uint8_t commandByte)
{
	GPIO_DRV_ClearPinOutput(kSSD1331PinCSn);

	GPIO_DRV_ClearPinOutput(kSSD1331PinDC);
	payloadBytes[0] = commandByte;

    SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE,
                                            NULL,
                                            (const uint8_t * restrict)&payloadBytes[0],
                                            (uint8_t * restrict)&inBuffer[0],
                                            1,
                                            1000);

	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);
}

/*******************************************************************************
 * printHeart function draws four lines for printing red heart.
 ******************************************************************************/
void
printHeart(uint8_t x, uint8_t y)
{
    uint8_t commands[32]= {0};

    commands[0]=kSSD1331CommandDRAWLINE;
    commands[1]=x;
    commands[2]=y+1;
    commands[3]=x+4;
    commands[4]=y+1;
    commands[5]=0xFF;
    commands[6]=0x00;
    commands[7]=0x00;
    commands[8]=kSSD1331CommandDRAWLINE;
    commands[9]=x+1;
    commands[10]=y;
    commands[11]=x+1;
    commands[12]=y+2;
    commands[13]=0xFF;
    commands[14]=0x00;
    commands[15]=0x00;
    commands[16]=kSSD1331CommandDRAWLINE;
    commands[17]=x+2;
    commands[18]=y+1;
    commands[19]=x+2;
    commands[20]=y+3;
    commands[21]=0xFF;
    commands[22]=0x00;
    commands[23]=0x00;
    commands[24]=kSSD1331CommandDRAWLINE;
    commands[25]=x+3;
    commands[26]=y;
    commands[27]=x+3;
    commands[28]=y+2;
    commands[29]=0xFF;
    commands[30]=0x00;
    commands[31]=0x00;

	GPIO_DRV_ClearPinOutput(kSSD1331PinCSn);
	
	GPIO_DRV_ClearPinOutput(kSSD1331PinDC);
    SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE,
                                            NULL,
                                            (const uint8_t * restrict)&commands[0],
                                            (uint8_t * restrict)&inBuffer[0],
                                            32,
                                            1000);
	
	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);
}

/*******************************************************************************
 * heartFall function implement red hearting falling. When blue arrow is close
 * to red heart, red heart disappears.
 ******************************************************************************/
void
heartFall(uint8_t x, uint8_t y, uint8_t steps)
{
    uint8_t commands[12]= {0};
    uint8_t commandslen = 12;

    if ((x_arrow>=x-2) && (x_arrow<=(x+7)) && (y_arrow>=y-2) && (y_arrow<=(y+5)))
    {   //heart disappear when arrow is close.
        commands[0]=0x25;
        commands[1]=x;
        commands[2]=y;
        commands[3]=x+4;
        commands[4]=y+3;
        commandslen = 5;
    }
    else //heart falling.
    {
        commands[0]=0x23;
        commands[1]=x;
        commands[2]=y;
        commands[3]=x+4;
        commands[4]=y+3;
        commands[5]=x;
        commands[6]=y+steps;

        commands[7]=0x25;
        commands[8]=x;
        commands[9]=y;
        commands[10]=x+4;
        commands[11]=y+steps-1;
    }
 
	OSA_TimeDelay(100);
	GPIO_DRV_ClearPinOutput(kSSD1331PinCSn);
	
	GPIO_DRV_ClearPinOutput(kSSD1331PinDC);
    SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE,
                                            NULL,
                                            (const uint8_t * restrict)&commands[0],
                                            (uint8_t * restrict)&inBuffer[0],
                                            commandslen,
                                            1000);
	
	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);
}

/*******************************************************************************
 * printArrow prints blue arrow when Flay = 0 based on position (x,y).
 * printArrow moves blue arrow when Flay = 1 based on position (x,y).
 * deltaX and deltaY determins arrow direction.
 ******************************************************************************/
void
printArrow(uint8_t x, uint8_t y, int8_t deltaX, int8_t deltaY, uint8_t Fly)
{
    uint8_t commands[13]= {0};
    uint8_t len = 13;

    commands[0]=0x25;
    if(Fly == 0)
    {
        commands[1]=x-8;
        commands[2]=y-8;
        commands[3]=x+8;
        commands[4]=y;
    }
    else if (deltaX<0)
    {
        commands[1]=x;
        commands[2]=y;
        commands[3]=x-deltaX;
        commands[4]=y-deltaY;
    }
    else
    {
        commands[1]=x-deltaX;
        commands[2]=y;
        commands[3]=x;
        commands[4]=y-deltaY;
    }

    commands[5]=kSSD1331CommandDRAWLINE;
    commands[6]=x;
    commands[7]=y;
    commands[8]=x+deltaX;
    commands[9]=y+deltaY;
    commands[10]=0x00;
    commands[11]=0x00;
    commands[12]=0xFF;
    
	GPIO_DRV_ClearPinOutput(kSSD1331PinCSn);
	
	GPIO_DRV_ClearPinOutput(kSSD1331PinDC);
    SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE,
                                            NULL,
                                            (const uint8_t * restrict)&commands[0],
                                            (uint8_t * restrict)&inBuffer[0],
                                            len,
                                            1000);
	
	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);
    
}

/*******************************************************************************
 * clearArrow function clear the blue arrow based on position (x,y).
 * In this project, it will clear arrow when arrow is close to edge of OLED
 * window.
 ******************************************************************************/
void
clearArrow(uint8_t x, uint8_t y, int8_t deltaX, int8_t deltaY)
{
    uint8_t commands[5]= {0};

    commands[0]=0x25;
    
    if (deltaX<0)
    {
        commands[1]=x;
        commands[2]=y;
        commands[3]=x-deltaX;
        commands[4]=y-deltaY;
    }
    else
    {
        commands[1]=x-deltaX;
        commands[2]=y;
        commands[3]=x;
        commands[4]=y-deltaY;
    }

	GPIO_DRV_ClearPinOutput(kSSD1331PinCSn);
	
	GPIO_DRV_ClearPinOutput(kSSD1331PinDC);
    SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE,
                                            NULL,
                                            (const uint8_t * restrict)&commands[0],
                                            (uint8_t * restrict)&inBuffer[0],
                                            5,
                                            1000);
	
	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);
    
}

/*******************************************************************************
 * devSSD1331init is to initialise OLED
 ******************************************************************************/
void
devSSD1331init(void)
{
    /*uint8_t initCommands[42] = {kSSD1331CommandDISPLAYOFF, kSSD1331CommandSETREMAP, 0x72, kSSD1331CommandSTARTLINE,0x0,kSSD1331CommandDISPLAYOFFSET,0x0,
                              kSSD1331CommandNORMALDISPLAY,kSSD1331CommandSETMULTIPLEX,0x3F,kSSD1331CommandSETMASTER,0x8E,kSSD1331CommandPOWERMODE,
                              0x0B,kSSD1331CommandPRECHARGE,0x31,kSSD1331CommandCLOCKDIV,0xF0,kSSD1331CommandPRECHARGEA,0x64,kSSD1331CommandPRECHARGEB,
                              0x78,kSSD1331CommandPRECHARGEC,0x64,kSSD1331CommandPRECHARGELEVEL,0x3A,kSSD1331CommandVCOMH,0x3E,kSSD1331CommandMASTERCURRENT,
                              0x06,kSSD1331CommandCONTRASTA,0x91,kSSD1331CommandCONTRASTB,0x50,kSSD1331CommandCONTRASTC,0x7D,
                              kSSD1331CommandCLEAR,0x00,0x00,0x5F,0x3F,kSSD1331CommandDISPLAYON};*/

	GPIO_DRV_ClearPinOutput(kSSD1331PinRST);
	OSA_TimeDelay(100);
	GPIO_DRV_SetPinOutput(kSSD1331PinRST);
	//OSA_TimeDelay(100);

    //GPIO_DRV_SetPinOutput(kSSD1331PinCSn);
	//OSA_TimeDelay(100);
	/*GPIO_DRV_ClearPinOutput(kSSD1331PinCSn);
	
	GPIO_DRV_ClearPinOutput(kSSD1331PinDC);

    SPI_DRV_MasterTransferBlocking(SPI_MASTER_INSTANCE,
                                            NULL,
                                            (const uint8_t * restrict)&initCommands[0],
                                            (uint8_t * restrict)&inBuffer[0],
                                            42,
                                            1000);
    
	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);*/
	/*
	 *	Initialization sequence, borrowed from https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino
	 */
	writeCommand(kSSD1331CommandDISPLAYOFF);	// 0xAE
	writeCommand(kSSD1331CommandSETREMAP);		// 0xA0
	writeCommand(0x72);				// RGB Color
	writeCommand(kSSD1331CommandSTARTLINE);		// 0xA1
	writeCommand(0x0);
	writeCommand(kSSD1331CommandDISPLAYOFFSET);	// 0xA2
	writeCommand(0x0);
	writeCommand(kSSD1331CommandNORMALDISPLAY);	// 0xA4
	writeCommand(kSSD1331CommandSETMULTIPLEX);	// 0xA8
	writeCommand(0x3F);				// 0x3F 1/64 duty
	writeCommand(kSSD1331CommandSETMASTER);		// 0xAD
	writeCommand(0x8E);
	writeCommand(kSSD1331CommandPOWERMODE);		// 0xB0
	writeCommand(0x0B);
	writeCommand(kSSD1331CommandPRECHARGE);		// 0xB1
	writeCommand(0x31);
	writeCommand(kSSD1331CommandCLOCKDIV);		// 0xB3
	writeCommand(0xF0);				// 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
	writeCommand(kSSD1331CommandPRECHARGEA);	// 0x8A
	writeCommand(0x64);
	writeCommand(kSSD1331CommandPRECHARGEB);	// 0x8B
	writeCommand(0x78);
	writeCommand(kSSD1331CommandPRECHARGEA);	// 0x8C
	writeCommand(0x64);
	writeCommand(kSSD1331CommandPRECHARGELEVEL);	// 0xBB
	writeCommand(0x3A);
	writeCommand(kSSD1331CommandVCOMH);		// 0xBE
	writeCommand(0x3E);
	writeCommand(kSSD1331CommandMASTERCURRENT);	// 0x87
	writeCommand(0x06);
	writeCommand(kSSD1331CommandCONTRASTA);		// 0x81
    writeCommand(0x91);
	writeCommand(kSSD1331CommandCONTRASTB);		// 0x82
	writeCommand(0x50);
	writeCommand(kSSD1331CommandCONTRASTC);		// 0x83
	writeCommand(0x7D);

	/*
	 *	Clear Screen
	 */
	writeCommand(kSSD1331CommandCLEAR);
	writeCommand(0x00);
	writeCommand(0x00);
	writeCommand(0x5F);
	writeCommand(0x3F);

    writeCommand(kSSD1331CommandDISPLAYON);		// Turn on oled panel
	
}

/*******************************************************************************
 * angleTable is used to generate deltaX and deltaY for moving blue arrow.
 * It has 25 pairs of deltaX and deltaY, corresponding to 0-180 degree for
 * the arrow dirrection in game. Each pair repersents 7.5 degree. 
 ******************************************************************************/
int8_t angleTable[25][2] = {  
    {-8, 0},   // arrow towards left
    {-8, -1},   
    {-8, -2},   
    {-8, -3},
    {-7, -4},
    {-7, -5},
    {-6, -6},
    {-5, -7},
    {-4, -7},
    {-3, -8},
    {-2, -8},
    {-1, -8},
    {0, -8},    // arrow towards up
    {1, -8},
    {2, -8},
    {3, -8},
    {4, -7},
    {5, -7},
    {6, -6},
    {7, -5},
    {7, -4},
    {8, -3},
    {8, -2},
    {8, -1},
    {8, 0}      //arrow towards right
};

/*******************************************************************************
 * Code
 ******************************************************************************/
int main (void)
{
    /*Variables declaration for SPI, I2C and the other device parameters.*/
    uint32_t calculatedBaudRate;
    spi_master_state_t spiMasterState;
    spi_master_user_config_t userConfig =
    {
        #if FSL_FEATURE_SPI_16BIT_TRANSFERS
            .bitCount       = kSpi8BitMode,
        #endif
        .polarity       = kSpiClockPolarity_ActiveHigh,
        .phase          = kSpiClockPhase_FirstEdge,
        .direction      = kSpiMsbFirst,
        .bitsPerSec     = TRANSFER_BAUDRATE
    };
    accel_dev_t accDev;
    accel_dev_interface_t accDevice;
    accel_sensor_data_t accelData;
    accel_i2c_interface_t i2cInterface;

    
    
    i2cInterface.i2c_init       =  I2C_DRV_MasterInit;
    i2cInterface.i2c_read       =  I2C_DRV_MasterReceiveDataBlocking;
    i2cInterface.i2c_write      =  I2C_DRV_MasterSendDataBlocking;

    accDev.i2c      = &i2cInterface;
    accDev.accel    = &accDevice;

    accDev.slave.baudRate_kbps  = BOARD_ACCEL_BAUDRATE;
    accDev.slave.address        = BOARD_ACCEL_ADDR;
    accDev.bus                  = BOARD_ACCEL_I2C_INSTANCE;
    
    /* Intializes random number generator for heart position generation later*/
    srand(time(0));

    /* Position variables for red heart and blue arrow printing. The second red heart
       was commented out due to limited memory space. */
    uint8_t x;
    uint8_t y = 0;
    //uint8_t x2;
    //uint8_t y2 = 0;
    int8_t deltaX;
    int8_t deltaX1 = 0; //when button sw3 is pressed, deltaX1 is updated with value of deltaX.
    int8_t deltaY;
    int8_t deltaY1 = 0; //when button sw3 is pressed, deltaY1 is updated with value of deltaY.
    int16_t yData;  //store data read from accelerometer.
    int8_t yAngle;  //calculated arrow angle based on data read from accelerometer.
        
    uint8_t steps = 2;  //red heart falling speed variable.
    //uint8_t steps2 = 3;
    uint32_t i = 0;
    uint8_t j = 20; //Number of round of red heart falling.
   
    // Init hardware
    hardware_init();

    // Init GPIO
    // Define gpio input pin config structure.
    gpio_input_pin_user_config_t inputPin[] = {
        {
            .pinName                       = kGpioSW3,
            .config.isPullEnable           = true,
            .config.interrupt              = kPortIntFallingEdge,
        },
        {
            .pinName = GPIO_PINS_OUT_OF_RANGE,
        }
    };

    // Define gpio output pin config structure.
    gpio_output_pin_user_config_t outputPin[] = {
        {
            .pinName              = kSSD1331PinDC,
            .config.outputLogic   = 1,
        },
        {
			.pinName = kSSD1331PinCSn,
			.config.outputLogic = 1,
		},
        {
			.pinName = kSSD1331PinRST,
			.config.outputLogic = 1,
		},
        {
           .pinName = GPIO_PINS_OUT_OF_RANGE,
        }
    };

    GPIO_DRV_Init(inputPin, outputPin);

    PORT_HAL_SetMuxMode(PORTA,12u,kPortMuxAsGpio);  //DC
    PORT_HAL_SetMuxMode(PORTB,10u,kPortMuxAsGpio);  //CSn
    PORT_HAL_SetMuxMode(PORTB,0u,kPortMuxAsGpio);   //RST
    PORT_HAL_SetMuxMode(PORTA,8u,kPortMuxAlt3);     //MOSI
    PORT_HAL_SetMuxMode(PORTA,9u,kPortMuxAlt3);     //SCK
    
    // Init OSA layer, used in DSPI_DRV_MasterTransferBlocking.
    OSA_Init();

    // Initialize the Accel.
    accel_init(&accDev);

    // Init and setup SPI for OLED
    SPI_DRV_MasterInit(SPI_MASTER_INSTANCE, &spiMasterState);
    SPI_DRV_MasterConfigureBus(SPI_MASTER_INSTANCE,
                                &userConfig,
                                &calculatedBaudRate);
    // Init OLED display
    devSSD1331init();

    /* j rounds of red heart falling game.
     * Code for the second read heart falling simutaneously was commentted out */
    while (j!=0)
    {
        x   = (uint8_t)(rand()%92); // Randomly generate red heart position
        y = 0;
        //x2 = (uint8_t)(rand()%92);
        //y2 = 0;
        //while ((x2>=x) && (x2<=(x+4)))
        //{
        //  x2  = (uint8_t)(rand()%92);
        //}
        
        // Print red heart
        printHeart(x,y);
        //printHeart(x2,y2);


        /* While a red heart is not falling down to the bottom, repeat this "while loop".*/
        //while (!(((y+3)>=64) && ((y2+3) >= 64))) // this while loop is for the second heart.
        while (y<61)
        {
            heartFall(x, y, steps);
            y = y + steps;
               
            //if (y2<61)
            //{
            //    heartFall(x2, y2, steps2);
            //    y2 = y2 + steps2;
            //}
               
            accDev.accel->accel_read_sensor_data(&accDev,&accelData);   // Read data from accelerometer.

            yData = (int16_t)((accelData.data.accelYMSB << 8) | accelData.data.accelYLSB);  // Format data

            yAngle = (int8_t)(((yData/100)+90)/7);  // yData is between -9000 and 9000. Divide by 100 to
                                                    // convert to degree -90 to 90. Plus 90 to change range
                                                    // to 0 to 180. Then divide by 8 (which is close to 7.5),
                                                    // to generate an index for looking up in angleTable.

            deltaX = angleTable[yAngle][0];         // Use angleTable to determine arrow direction.
            deltaY = angleTable[yAngle][1];         //
            printArrow(47, 63, deltaX, deltaY, 0);  // Print arrow on screen with updated direction.
                
            if(isButtonPress)                       // Detect if button sw3 is pressed.
            {
                // Reset state of button.
                isButtonPress=false;

                deltaX1 = deltaX;                   // Record deltaX in deltaX1 when button is pressed.
                deltaY1 = deltaY;                   // deltaX1 and deltaY1 will be used to shot arrow.
                x_arrow = 47+deltaX1;               // Update arrow position variables.
                y_arrow = 63+deltaY1;                   
            }

            if (shot == 1)                          // Shot is set to 1 in interrupt service routine
            {                                       // when button is pressed.
                printArrow(x_arrow, y_arrow, deltaX1, deltaY1, 1);  // Shot arrow!!!
                x_arrow = x_arrow + deltaX1;        // Update arrow position variables.
                y_arrow = y_arrow + deltaY1;
                if ((x_arrow <8)||(y_arrow <8)||(x_arrow >86))  // Clear arrow when it is at the edge of screen.
                {
                    shot = 0;                       // Clear shot flag.
                    clearArrow(x_arrow, y_arrow, deltaX1, deltaY1);                    
                }
            }                                   
            i = i+1;
        }
        j = j - 1;
    }
    return 0;
}

/*******************************************************************************
 * Interrupt service routine. Enter when SW3 is pressed.
 ******************************************************************************/
void BOARD_SW_IRQ_HANDLER(void)
{
    // Clear external interrupt flag.
    GPIO_DRV_ClearPinIntFlag(kGpioSW3);
    // Set shot flag to shot arrow.
    shot = 1;
    // Change state of button.
    isButtonPress = true;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

