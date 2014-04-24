/******************************************************************
* This is compiled using ZDS-II - Encore! 5.0
*
* Copyright (C) 1999-2011 by  ZiLOG, Inc. ALL RIGHTS RESERVED.
*
* Zilog DOES NOT GUARANTEE THE VERACITY OF THE SOFTWARE.
******************************************************************/

///////////////////////////////////////////////////////////
// Header includes
///////////////////////////////////////////////////////////

// This file must match the lens and PIR sensor used in the application
#include "API_INIT_DL.h"                                // API configuration file for CM0.77 GI V3 Lens

#include <eZ8.h>                                        // Standard Z8 library header (pulls in other standard files)
#include "ePIR_API.h"                                   // ZMotion API header
#include "main.h"                                       // Project header
#include "UART_Util.h"                                  // UART_Util header file
#include "CheckCommand.h"                               // CheckCommand header file
#include "Timer0.h"                                     // Timer0 header file
#include "ADC.h"                                        // ADC header file

///////////////////////////////////////////////////////////
// Register (Rdata) Global memory assignments
// Global memory placement done for code saving and speed optimizations
///////////////////////////////////////////////////////////
unsigned char   MDOnTime = DEF_MD_ON_TIME;              // Default /MD on Time
unsigned char   MotionOffTime = DEF_NO_MOTION_TIME;     // Default time to not look for motion when motion is detected (min time between events)
unsigned char   cMDCheck = DELAY_12SEC;                 // Counter used to control time between checking for motion
unsigned char   cModuleStatus1 = MOD_STAT1_DEF;         // Module Status flags group 1
unsigned char   cModuleStatus2 = MOD_STAT2_DEF;         // Module Status flags group 2
unsigned char   cCmdState = ZDOT_IDLE;                  // Serial interface state register
unsigned char   cCmdResponse = 0;                       // Command Response character, sent when not zero and entering Idle state
unsigned char   cStateVar1;                             // Variable for use within the serial state machine
unsigned char   cStateVar2;                             // Variable for use within the serial state machine
unsigned char   cMDDelay = 0 ;                          // /MD Output delay timer
unsigned char   cSerialTimeout = 0;                     // Serial Interface inactivity timeout
unsigned char   c50mSecond = 0;                         // 50 mS counter to generate a 1 second tick for house keeping
unsigned char   c1Second = 0;                           // 1 Second counter to generate a 1 minute tick
unsigned char   ctemp;                                  // just some temporary scratchpad storage
unsigned long   WDTime;                                 // Temp var to calculate the watchdog timer value
unsigned int    wdt_1sec_val;                           // Calibrated 1 second WDT value from Flash Information Area
unsigned int    RAMAddress;                             // pointer to RAM address for RAM Read/Write command

// Pointer to WDT 1 second calibration value
rom unsigned int *wdt_ptr = (rom unsigned int*)(0xFE7E);

// External variable declarations
extern near unsigned char cResetStatus;                 // POR detection copy, from RSTSTAT (copied in startups.asm)
extern unsigned char cRXChar;                           // Receive Data 'Buffer'
extern unsigned char cTXChar;                           // Transmit Data 'Buffer'

///////////////////////////////////////////////////////////
// Flash option bits
///////////////////////////////////////////////////////////

#if !FLASHLOADER                                        // If using a Flash Loader, flash option bits cannot be set
#if DEBUG                                               // When debugging, flash protections can be disabled
FLASH_OPTION1 = 0xFF;
FLASH_OPTION2 = 0xFF;
#else
FLASH_OPTION1 = 0xFF;
FLASH_OPTION2 = 0xFF;
#endif
#endif

///////////////////////////////////////////////////////////
// Model identification for user to ID code
///////////////////////////////////////////////////////////
rom unsigned int AppChecksum _At 0x0200 = 0xA5C2;       // Application Checksum, load here after calculating, or if host will calculate, leave as 0x0000
rom unsigned char AppVersionH _At 0x0202 = APP_VERSION_HIGH;  // Application version High byte
rom unsigned char AppVersionL _At 0x0203 = APP_VERSION_LOW;  // Application version Low Byte

///////////////////////////////////////////////////////////
// main - Main application code file
///////////////////////////////////////////////////////////
void main(void)
{
///////////////////////////////////////////////////////////
// Interrupt Vector Initialization
///////////////////////////////////////////////////////////


#if !FLASHLOADER                                        // If using a bootloader, Interrupts are jumps instead, located in startupePIR.asm
    SET_VECTOR(WDT, c_startup);                         // Set unused vector to restart
    SET_VECTOR(TRAP, c_startup);                        // Set unused vector to restart
    SET_VECTOR(TIMER2, c_startup);                      // Set unused vector to restart
    SET_VECTOR(TIMER1, c_startup);                      // Set unused vector to restart
    SET_VECTOR(I2C, c_startup);                         // Set unused vector to restart
    SET_VECTOR(SPI, c_startup);                         // Set unused vector to restart
    SET_VECTOR(P7AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(P6AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(P5AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(P4AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(P3AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(P2AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(P1AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(P0AD, c_startup);                        // Set unused vector to restart
    SET_VECTOR(TIMER3, c_startup);                      // Set unused vector to restart
    SET_VECTOR(UART1_RX, c_startup);                    // Set unused vector to restart
    SET_VECTOR(UART1_TX, c_startup);                    // Set unused vector to restart
    SET_VECTOR(DMA, c_startup);                         // Set unused vector to restart
    SET_VECTOR(C3, c_startup);                          // Set unused vector to restart
    SET_VECTOR(C2, c_startup);                          // Set unused vector to restart
    SET_VECTOR(C1, c_startup);                          // Set unused vector to restart
    SET_VECTOR(C0, c_startup);                          // Set unused vector to restart
    SET_VECTOR(POTRAP, c_startup);                      // Set unused vector to restart
    SET_VECTOR(WOTRAP, c_startup);                      // Set unused vector to restart
    SET_VECTOR(UART0_TX, c_startup);                    // Set vector for byte TX done

    SET_VECTOR(TIMER0, isrTimer0);                      // Set vector for timer 0 end of count
    SET_VECTOR(ADC, isrADC_EOC);                        // Set vector for ADC End Of Count
    SET_VECTOR(UART0_RX, isrRX0);                       // Set vector for byte RX done
#endif

///////////////////////////////////////////////////////////
// Application initialization starts here
///////////////////////////////////////////////////////////
    DI();                                               // Disable all interrupts for setup
    OSCCTL = 0xE7;                                      // Unlock sequence for OSCTL write
    OSCCTL = 0x18;                                      // Unlock sequence for OSCTL write
    OSCCTL = OSCCTL_VAL;                                // Make sure we are on internal IPO, and enable the WDT oscillator

// Initialize non-engine IO and peripherals here
    PWRCTL0 = PWRCTL0_VAL;                              // For analog, only power up VBO and A/D converter

    // The calibration data at 0xFE7E/0xFE7F (WDTCALH/WDTCALL) gives a 1 second time-out for WDT at 30C and 3.3V
    FPS = 0x80;
    wdt_1sec_val = *wdt_ptr;                            // Save the 1 second WDT time-out value
    FPS = 0x00;


// We'll use a 1 second WDT timeout value
    WDTCTL = 0x55;                                      // Unlock watchdog timer
    WDTCTL = 0xAA;                                      // Unlock watchdog timer
    WDTU = 0x00;                                        // Load the watchjog timeout value Upper
    WDTH = wdt_1sec_val>>8;                             // Load the watchjog timeout value High
    WDTL = wdt_1sec_val;                                // Load the watchjog timeout value Lower
    Z8_WDT;                                             // Initial kick of the watchdog

    FFREQ = 0x159A;                                     // Set up Flash Frequency register - Value used by ePIR Engine

// Initialize engine API values and other Power-on items before calling EPIR_INIT
    if (!(cResetStatus & 0x40))                         // If this is not an SMR Reset...
    {

// Initialize the UART
        U0BRH = U0_BAUD_HIGH;                           // Serial interface baud rate set up
        U0BRL = U0_BAUD_LOW;                            // Serial interface baud rate set up
        U0CTL0 = U0CTL0_VAL;                            // Serial interface baud rate set up
        U0CTL1 = U0CTL1_VAL;                            // Serial interface baud rate set up

        IRQ0ENH = 0x21;                                 // Set UART0 RX, T0 and ADC interrupts and priority
        IRQ0ENL = 0x11;                                 // Set UART0 RX, T0 and ADC interrupts and priority

        SER_ACK;                                        // Send out an ACK to tell the world we're alive

// Initialize the ZMOTION Engine
        ePIR_Sensitivity = EPIR_SENSITIVITY_DEF;        // Default sensitivity level
        ePIR_SC0 = EPIR_SC0_DEF;                        // Init the status register 0
        ePIR_SC1 = EPIR_SC1_DEF;                        // Init the status register 1
        ePIR_SC2 = EPIR_SC2_DEF;                        // Init the status register 2
        ePIR_SC3 = EPIR_SC3_DEF;                        // Init the status register 3

        ePIR_ASC0 = EPIR_ASC0_DEF;                      // Init the advanced status register 0
        ePIR_ASC2 = EPIR_ASC2_DEF;                      // Init the advanced status register 2
        ePIR_Sample_Size = EPIR_SAMPLE_SIZE_DEF;
        ePIR_Debounce_Batch = EPIR_DEBOUNCE_BATCH_DEF;
        ePIR_Debounce = EPIR_DEBOUNCE_DEF;
        ePIR_Noise_Sense = EPIR_NOISE_SENSE_DEF;
        ePIR_Transient_Sense = EPIR_TRANSIENT_SENSE_DEF;
    }
    else
    {
        cMDCheck = 0;                                   // If we came from SMR, MDCheck may not be zero
    }

    cCmdState = ZDOT_IDLE;                              // Set for the Base Menu state
    cCmdResponse = 0;                                   // Load for nothing to send when entering Idle

// This assembly call must remain in the startup initialization, do not modify or delete
    ePIR_Enable = EPIR_ENABLE_PATTERN;                  // Safety pattern to enable engine
    EPIR_INIT;                                          // Initialization routine for ePIR engine

    ///////////////////////////////////////////////////////////
    // Initialize GPIO's for ZMOTION 8-Pin Occupancy MCU
    // This can be done after EPIR_INIT if you do not touch the
    // port configurations for PA3/ANA2.
    ///////////////////////////////////////////////////////////

/*
Port A
Pin     Desc.                   POR State
PA0 - Not used                  Output High
PA1 - VREF In                   Input VREF In
PA2 - MD Output                 Output Low
PA3 - Reserved                  Pyro Signal
PA4 - Receive Data              UART0 Rx
PA5 - Transmit Data             UART0 Tx
PA6 - Not Available             N/A
PA7 - Not Available             N/A
*/

/*   PAOUT = 0x01;                                       // Initialize GPIO's to a safe startup value

    PAADDR = 0x01;                                      // Port A Direction Register
    PACTL = 0x18;                                       // Port A Directions

    PAADDR = 0x07;									    // Port A Alt Functions 1
    PACTL &= ~0x30;									    // Rxd on PA4; Txd on PA5
    PACTL |= 0x02;									    // Enable Vref on PA1

    PAADDR = 0x08;									    // Port A Alt Functions 2
    PACTL &= ~0x30;									    // Enable Rxd on PA4; Txd on PA5
    PACTL |= 0x02;									    // Enable Vref on PA1

    PAADDR = 0x02;                                      // Port A Alt Functions Enable Register
    PACTL |= 0x32;                                      // Port A Alternate Functions
	PACTL &= ~0x04;									    // Set PA2 to GPIO

    PAADDR = 0x00;                                      // Port A Safe

    ADCCTL0 &= ~0x40;                                   // Select External Vref
    ADCCTL1 &= ~0x80;                                   // Select External Vref
	*/
//
	PAOUT = 0x35;									// Initialize GPIO's to a safe startup value
	MD_ON;											// Turn LED on until PIR is stable

	PAADDR = 0x01;									// Port A Direction - PA5=Out(Txd); PA4=In(Rxd)
	PACTL = 0x1B;									// PA3=In(PIR); PA2=Out(RST); PA1=In(Vref); PA0=In(DBG)
	
	PAADDR = 0x07;									// Port A Alt Functions 1
	PACTL &= ~0x30;									// Rxd on PA4; Txd on PA5
	PACTL |= 0x02;									// Enable Vref on PA1

	PAADDR = 0x08;									// Port A Alt Functions 2
	PACTL &= ~0x30;									// Enable Rxd on PA4; Txd on PA5
	PACTL |= 0x02;									// Enable Vref on PA1

	PAADDR = 0x02;									// Port A Alt Functions Enable
	PACTL |= 0x32;									// Rxd on PA4; Txd on PA5; Vref on PA1
	PACTL &= ~0x04;									// Set PA2 to GPIO

	PAADDR = 0x00;									// Port A Safe
	
	ADCCTL0 &= ~0x40;								// Select External Vref
	ADCCTL1 &= ~0x80;								// Select External Vref

// Initialize Timer 0, 50ms
    T0H = 0x00;                                         // Timer High
    T0L = 0x01;                                         // Timer Low
    T0RH = T0RH_VAL;                                    // Reload Compare High
    T0RL = T0RL_VAL;                                    // Reload Compare Low
    T0CTL0 = T0CTL0_VAL;                                // Load T0 Config0
    T0CTL1 = T0CTL1_VAL;                                // Load T0 Config1

    EI();                                               // Enable all interrupts

    if ((cResetStatus & 0x60) == 0x60)                  // If this was an SMR from WDT...
    {
        SER_AWAKE;                                      // We have returned from sleep via timeout, send the awake indicator
    }

///////////////////////////////////////////////////////////
// Main application loop starts here
// The major functions are:
//  Monitor for a motion event and handle the unsolicited mode tasks
//  Handle the serial input command
//  Halt - wait for interrupt from ADC, UART, T0
//  Start loop over
///////////////////////////////////////////////////////////
    while(1)
    {

// Check for motion detected
// If there is motion:
// Activate MD for MDOnTime as set by Write /MD Activation Time command: 'D'
// Save the event and direction so it can be reported via the Read Motion Status command: "a"
// Transmit the event if Motion status unsolicited mode is enabled: command 'M'
// Set the delay time before looking for motion again (MotionOffTime) set by Write Motion No Check Time: 'J'

    if((ePIR_SC0 & SC0_MOTION_DETECTED) && (cModuleStatus1 & MOD_PIR_STABLE) && (cMDCheck == 0))
    {
        cModuleStatus1 |= SER_MOTION_DETECTED;          // Set flag to indicate motion via serial command ('a')
        if(MDOnTime)
        {
            MD_ON;                                      // Turn on the MD Output port (turned off in T0 int)
            cMDDelay = MDOnTime;                        // Initiate MD ON time
        }
        if(MotionOffTime > 0)                           // if a motion off time is loaded...
        {
            cMDCheck = MotionOffTime;                   // Stop looking for motion for MotionOffTime time
        }
        else                                            // Otherwise...
        {
            cMDCheck = 0;
            ePIR_SC0 &= ~SC0_MOTION_DETECTED;           // Clear engine Motion detected flag
            ePIR_ASC0 |= ASC0_BUFFER_REFRESH;           // Initiale a buffer refresh to restore motion detection faster
        }

        if ((cModuleStatus1 & SER_REAL_TIME_MD) && (cCmdState == ZDOT_IDLE)) // If enabled to send real-time motion status...
        {
            cCmdResponse = 'M';                         // Load to send M char
        }
    }

///////////////////////////////////////////////////////////
// Serial Interface handler
// This handles all serial command input and processing
// Serial command state machine
///////////////////////////////////////////////////////////

        if (cCmdState == ZDOT_IDLE && cCmdResponse)     // If this is the idle state and the previous command wants to send a response...
        {
            TxDirect(cCmdResponse);                     // Send the character
            cCmdResponse = 0;                           // Clear the response so it only sends once
        }
        if (cCmdState == ZDOT_IDLE && cModuleStatus1 & SER_BYTE_RX) // If we are idle and have received a new byte...
        {
            cModuleStatus1 &= ~SER_BYTE_RX;             // Clear new byte received flag
            CheckCommand(cRXChar);                      // Check the command that was received
        }

        else if (cModuleStatus1 & SER_BYTE_RX)          // Otherwise, if we received a byte and are in the middle of a command...
        {
            cModuleStatus1 &= ~SER_BYTE_RX;             // Clear new byte received flag
            ctemp = cRXChar;                            // Get a copy of the received data
// Handle the Real-Time MD Status Change state
            if (cCmdState == ZDOT_REAL_TIME_MD_SET)
            {
                if (ctemp == 'Y')                       // If requesting Real-Time...
                {
                    cModuleStatus1 |= SER_REAL_TIME_MD; // Enable Real-Time MD Status output
                    cCmdResponse = ACK_CHAR;            // Load to send ACK
                }
                else if (ctemp == 'N')                  // If requesting Non-Real-Time...
                {
                    cModuleStatus1 &= ~SER_REAL_TIME_MD; // Disable Real-Time MD Status output
                    cCmdResponse = ACK_CHAR;            // Load to send ACK
                }
                else
                {
                    cCmdResponse = NACK_CHAR;           // Load to send ACK
                }
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the Sensitivity Change state

            else if (cCmdState == ZDOT_SENS_SET)
            {
                ePIR_Sensitivity = ctemp;
                cCmdResponse = ACK_CHAR;                // Load to send ACK
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the Noise Sens Change state
            else if (cCmdState == ZDOT_NOISE_SENS_SET)
            {
                ePIR_Noise_Sense = ctemp;
                cCmdResponse = ACK_CHAR;                // Load to send ACK
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the Transient Sens Change state
            else if (cCmdState == ZDOT_TRANS_SENS_SET)
            {
                ePIR_Transient_Sense = ctemp;
                cCmdResponse = ACK_CHAR;                // Load to send ACK
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the RAM Read and Write states
            else if (cCmdState == ZDOT_RAM_WRITE || cCmdState == ZDOT_RAM_READ)
            {
                // Get the address
                if (cStateVar1 == 0)                    // If this is the first byte...
                {
                    RAMAddress = ctemp << 8;            // Get the MSB of the address
                    cStateVar1++;                       // Increment to get the next byte
                }
                else if (cStateVar1 == 1)               // Otherwise, if this is the second byte of address...
                {
                    RAMAddress += ctemp;                // Get the LSB of the address
                    if ( cCmdState == ZDOT_RAM_WRITE)   // If this is the write command...
                    {
                        cStateVar1++;                   // Advance to write section
                    }
                    else                                // Otherwise, it is a read command...
                    {
                        TxDirect(*((far unsigned char*)RAMAddress));
                        cCmdState = ZDOT_IDLE;          // Set for the idle mode
                    }
                }
                else if (cStateVar1 == 2)               // If this is the read poisition...
                {
                    *((far unsigned char*)RAMAddress) = ctemp; // Write the data to the RAM address
                    ePIR_ASC0 |= ASC0_BUFFER_REFRESH;   // Request a buffer refresh in case buffer related API values were adjusted
                    cCmdResponse = ACK_CHAR;            // Load to send ACK
                    cCmdState = ZDOT_IDLE;              // Set for the idle mode
                }
            }

/* DSWIM Removed for code space
// Handle the White Light Settings Write state
            else if (cCmdState == ZDOT_WL_SET)
            {
                if (cStateVar1 == 0)                    // If this is the first byte...
                {
                    cStateVar2 = ctemp;                 // Copy the WL Threshold data
                    cStateVar1++;                       // Increment to get the next byte
                }
                else                                    // Otherwise, this is the second byte...
                {
                    if (cStateVar2 <= (SC2_WHITE_LIGHT_THRESHOLD >>3)) // If the value is not too large for the threshold bits...
                    {
                        cStateVar2 <<= 3;               // Shift by three
                        DI();                           // Ensure that the following section is atomic
                        ePIR_SC2 &= ~SC2_WHITE_LIGHT_THRESHOLD; // Clear off the current value
                        ePIR_SC2 |= cStateVar2;         // Write the WL Threshold value into register
                        ePIR_ASC1 = ctemp;              // Write the Scan Rate and Debounce into register
                        EI();                           // Re-enable interrupts
                        cCmdResponse = ACK_CHAR;        // Load to send ACK
                        cCmdState = ZDOT_IDLE;          // Set for the idle mode
                    }
                    else                                // Otherwise, data error...
                    {
                        cCmdResponse = NACK_CHAR;       // Load to send ACK
                        cCmdState = ZDOT_IDLE;          // Set for the idle mode
                    }
                }
            }
*/

// Handle the Motion Off Time change state
            else if (cCmdState == ZDOT_MOTION_OFF_SET)
            {
                MotionOffTime = ctemp;
                cCmdResponse = ACK_CHAR;                // Load to send ACK
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the Extended Detection Change state
            else if (cCmdState == ZDOT_EXT_SENSE_SET)
            {
                if(ctemp <= 3)                          // Make sure the value is valid (0-3)
                {
                    ctemp = (ctemp << 6) & SC0_EXTENDED; // Move it to the correct bit locations
                    Z8_ATM;
                    ePIR_SC0 &= ~SC0_EXTENDED;
                    ePIR_SC0 |= ctemp;
                    cCmdResponse = ACK_CHAR;            // Load to send ACK
                }
                else
                {
                    cCmdResponse = NACK_CHAR;           // Load to send ACK
                }
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the Freq. Response Change state
            else if (cCmdState == ZDOT_FREQ_SET)
            {
                if(ctemp <= SC1_FREQUENCY_RESPONSE >> 3)
                {
                    ctemp = (ctemp << 3) & SC1_FREQUENCY_RESPONSE;
                    Z8_ATM;
                    ePIR_SC1 &= ~SC1_FREQUENCY_RESPONSE;
                    ePIR_SC1 |= ctemp;
                    cCmdResponse = ACK_CHAR;            // Load to send ACK
                }
                else
                {
                    cCmdResponse = NACK_CHAR;           // Load to send ACK
                }
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the Range Control Change state
            else if (cCmdState == ZDOT_RANGE_CONTROL_SET)
            {
                if (ctemp <= SC2_RANGE_CONTROL)
                {
                    Z8_ATM;
                    ePIR_SC2 &= ~SC2_RANGE_CONTROL;
                    ePIR_SC2 |= ctemp;                  // Load the new Range value
                    cCmdResponse = ACK_CHAR;            // Load to send ACK
                }
                else
                {
                    cCmdResponse = NACK_CHAR;           // Load to send ACK
                }
                cCmdState = ZDOT_IDLE;                  // Set for the idle mode
            }

// Handle the Reset Request state or Flash Loader Request State
            else if (cCmdState == ZDOT_RESET_SET || cCmdState == ZDOT_FLASH_LOAD_SET)
            {
                if (ctemp == cStateVar1)                // If we get the next byte in...
                {
                    if (cStateVar1 == '4')              // If we have completed the sequence...
                    {
                        SER_ACK;                        // Send an ACK

                        DI();                           // Disable all interrupts
                        while(!(U0STAT0 & 0x02));       // Allow the ACK to finish transmitting
                        if (cCmdState == ZDOT_RESET_SET) // If this is the reset command...
                        {
                            while(1)                    // Wait until the Watchdog causes a reset
                            {
                                DI();                   // Disable interrupts to force a dead loop
                            }
                        }
                        else                            // Otherwise, must be Flash Loader
                        {
                            DI();                       // Disable interrupts to force a dead loop
                            asm("SRP #%E0");            // Set register pointer to E0 Group
                            asm("LD R1,#%A5");          // Load the pattern to indicate this is an app access to flash loader
                            asm("JP %003E");            // Jump to start of Flash Loader
                        }

                    }
                    else                                // Haven't completed the sequence yet
                    {
                        cStateVar1++;                   // Increment to the next sequence value
                    }
                }
                else                                    // We got the wrong character sequence
                {
                    cCmdResponse = NACK_CHAR;           // Load to send ACK
                    cCmdState = ZDOT_IDLE;              // Set for the idle mode
                }
            }

// In case the command state register is loaded with an invalid value...
            else                                        // Otherwise, command was not valid for some reason return to Idle
            {
                    cCmdState = ZDOT_IDLE;              // Set for the idle mode
            }
        }
        else if (cModuleStatus1 & SERIAL_TIMEOUT)       // Otherwise, if there was a serial data timeout...
        {
            cModuleStatus1 &= ~SERIAL_TIMEOUT;          // Clear the timeout flag
            if (cCmdState != ZDOT_IDLE)                 // If the state was not idle, meaning we were waiting for something from the serial...
            {
                cCmdResponse = NACK_CHAR;               // Load to send ACK
                cCmdState = ZDOT_IDLE;                  // Reset back to the idle state
            }
        }

        Z8_WDT;                                         // Kick the watchdog
        Z8_HALT;                                        // Halt to conserve power. Pulled out by T0, ADC and UART Rx
    }
}
