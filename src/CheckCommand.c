/************************************************************************************************
** File:        CheckCommand.c
** Description: ZMOTION Serial (M2M) Interface Command Checker source file
**
** Copyright 2014 Zilog Inc. ALL RIGHTS RESERVED.
*
*************************************************************************************************
* The source code in this file was written by an authorized Zilog employee or a licensed
* consultant. The source code has been verified to the fullest extent possible.
*
* Permission to use this code is granted on a royalty-free basis. However, users are cautioned to
* authenticate the code contained herein.
*
* ZILOG DOES NOT GUARANTEE THE VERACITY OF THIS SOFTWARE; ANY SOFTWARE CONTAINED HEREIN IS
* PROVIDED "AS IS." NO WARRANTIES ARE GIVEN, WHETHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
* IMPLIED WARRANTIES OF FITNESS FOR PARTICULAR PURPOSE OR MERCHANTABILITY. IN NO EVENT WILL ZILOG
* BE LIABLE FOR ANY SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES OR ANY LIABILITY IN TORT,
* NEGLIGENCE, OR OTHER LIABILITY INCURRED AS A RESULT OF THE USE OF THE SOFTWARE, EVEN IF ZILOG
* HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. ZILOG ALSO DOES NOT WARRANT THAT THE USE
* OF THE SOFTWARE, OR OF ANY INFORMATION CONTAINED THEREIN WILL NOT INFRINGE ANY PATENT,
* COPYRIGHT, OR TRADEMARK OF ANY THIRD PERSON OR ENTITY.

* THE SOFTWARE IS NOT FAULT-TOLERANT AND IS NOT DESIGNED, MANUFACTURED OR INTENDED FOR USE IN
* CONJUNCTION WITH ON-LINE CONTROL EQUIPMENT, IN HAZARDOUS ENVIRONMENTS, IN APPLICATIONS
* REQUIRING FAIL-SAFE PERFORMANCE, OR WHERE THE FAILURE OF THE SOFTWARE COULD LEAD DIRECTLY TO
* DEATH, PERSONAL INJURY OR SEVERE PHYSICAL OR ENVIRONMENTAL DAMAGE (ALL OF THE FOREGOING,
* "HIGH RISK ACTIVITIES"). ZILOG SPECIFICALLY DISCLAIMS ANY EXPRESS OR IMPLIED WARRANTY TO HIGH
* RISK ACTIVITIES.
*
************************************************************************************************/
/******************************************************************
// CheckCommand
//  Parameters: Received Character
//  Routine for checking the newly received command and sets up the new command state
//  Called when new character is received while in ZDOT_IDLE state
//  Read commands are performed here and return in the ZDOT_IDLE state
//  Write commands change to the new state and set up any necessary parameters
//  for main() to execute the command
******************************************************************/

#include <eZ8.h>                                        // Standard Z8 library header (pulls in other standard files)
#include "ePIR_API.h"                                   // ZMotion API header
#include "main.h"                                       // Project header
#include "UART_Util.h"
#include "CheckCommand.h"

extern unsigned char    MDOnTime;                       // Default /MD on Time
extern unsigned char    MotionOffTime;                  // Default time to not look for motion when motion is detected (min time between events)
extern unsigned char    cModuleStatus1;                 // Module Status flags group 1
extern unsigned char    cModuleStatus2;                 // Module Status flags group 2
extern unsigned char    cCmdState;                      // Serial interface state register
extern unsigned char    cCmdResponse;                   // Response to command
extern unsigned char    cMDDelay;                       // /MD Output delay timer
extern unsigned char    cStateVar1;                     // Variable for use within the serial state machine
extern rom unsigned int AppVersionH;                    // Application version

////////////////////////////////////////////////////////
// Command Lookup Table
// First entry is the command character
// Second entry is the function address
////////////////////////////////////////////////////////
rom CmdEntry CommandList[] = {\
{CMD_MD_STATUS_READ, commandStatusRead},\
{CMD_RAM_READ, commandRAMRead},\
{CMD_RAM_WRITE, commandRAMWrite},\
{CMD_EXT_SENSE_READ, commandExtSenseRead},\
{CMD_EXT_SENSE_WRITE, commandExtSenseWrite},\
{CMD_FREQ_RESP_READ, commandFreqRespRead},\
{CMD_FREQ_RESP_WRITE, commandFreqRespWrite},\
{CMD_VERSION_READ, commandVersionRead},\
{CMD_NO_MOTION_CHECK_TIME_READ, commandNoMotionCheckTimeRead},\
{CMD_NO_MOTION_CHECK_TIME_WRITE, commandNoMotionCheckTimeWrite},\
{CMD_REAL_TIME_MD_READ, commandRealTimeMDRead},\
{CMD_REAL_TIME_MD_WRITE, commandRealTimeMDWrite},\
{CMD_NOISE_SENS_READ, commandNoiseSensRead},\
{CMD_NOISE_SENS_WRITE, commandNoiseSensWrite},\
{CMD_RANGE_CONTROL_READ, commandRangeRead},\
{CMD_RANGE_CONTROL_WRITE, commandRangeWrite},\
{CMD_SENS_READ, commandSensitivityRead},\
{CMD_SENS_WRITE, commandSensitivityWrite},\
{CMD_TRANS_SENS_READ, commandTransientSensRead},\
{CMD_TRANS_SENS_WRITE, commandTransientSensWrite},\
{CMD_STABILITY_FLAG_READ, commandStabilityFlagRead},\
{CMD_RESET_REQUEST, commandMCUReset},\
{CMD_NOISE_FLAGS_READ, commandNoiseFlagsRead},\
{CMD_FLASH_LOAD_REQUEST, commandFlashLoader},\
};


void CheckCommand(unsigned char CommandChar)
{
    unsigned char cTemp;
    for (cTemp = 0; ;)
    {
        if (CommandList[cTemp].cCmd == CommandChar)     // If the command matches the table entry...
        {
            (*CommandList[cTemp].callback)();
            break;                                      // Terminate the loop
        }
        if (++cTemp >= (unsigned char)(sizeof(CommandList)/sizeof(CommandList[0]))) // If we reached the ed of the list with no match
        {
            cCmdResponse = NACK_CHAR;                   // Load to send ACK
            break;                                      // Terminate the loop
        }
    }

}

// Requesting Motion Detected Status Read Mode
void commandStatusRead(void)
    {
        if (cModuleStatus1 & MOD_PIR_STABLE)            // If the sensor has stabilized after powerup...
        {
            if (cModuleStatus1 & SER_MOTION_DETECTED)   // If motion has been detected...
            {
                cCmdResponse = 'Y';                     // Load to send Yes
            }
            else                                        // Otherwise...
            {
                cCmdResponse = 'N';                     // Load to send No
            }
        }
        else                                            // Otherwise, we are not stable yet...
        {
            cCmdResponse = 'U';                         // Load to send U
        }
        cModuleStatus1 &= ~SER_MOTION_DETECTED;         // Clear the motion flag
    }




// Requesting Real-Time Motion Detected Write Mode
void commandRealTimeMDWrite(void)
    {
        cCmdState = ZDOT_REAL_TIME_MD_SET;              // Set for the Real-Time MD Setting state
    }

// Requesting Real-Time Motion Detected Read Mode
void commandRealTimeMDRead(void)
    {
        if (cModuleStatus1 & SER_REAL_TIME_MD)          // If Real-time MD status is enabled...
        {
            cCmdResponse = 'Y';                         // Load to send Yes
        }
        else                                            // Otherwise...
        {
            cCmdResponse = 'N';                         // Load to send No
        }
    }

// Requesting Noise Sens Write Mode
void commandNoiseSensWrite(void)
    {
        cCmdState = ZDOT_NOISE_SENS_SET;                // Set for the delay setting state
    }

// Requesting Noise Sens Read Mode
void commandNoiseSensRead(void)
    {
        TxDirect(ePIR_Noise_Sense);                     // Send the Noise sense value
    }

// Requesting Transient Sens Write Mode
void commandTransientSensWrite(void)
    {
        cCmdState = ZDOT_TRANS_SENS_SET;                // Set for the delay setting state
    }

// Requesting Transient Sens Read Mode
void commandTransientSensRead(void)
    {
        TxDirect(ePIR_Transient_Sense);                 // Send the Noise sense value
    }

// Requesting No Motion Check Time Write Mode
void commandNoMotionCheckTimeWrite(void)
    {
        cCmdState = ZDOT_MOTION_OFF_SET;                // Set for the Motion Off setting state
    }

// Requesting No Motion Check Time READ Mode
void commandNoMotionCheckTimeRead(void)
    {
        TxDirect(MotionOffTime);                        // Send the Motion Off Time value
    }

// Requesting Freq. Response Status Write Mode
void commandFreqRespWrite(void)
    {
        cCmdState = ZDOT_FREQ_SET;                      // Set for the Freq. Response Setting state
    }

// Requesting Freq. Response Status Read Mode
void commandFreqRespRead(void)
    {
        TxDirect((ePIR_SC1 & SC1_FREQUENCY_RESPONSE) >> 3); // Send the Frequency Response Value
    }

// Requesting Range Control Write Mode
void commandRangeWrite(void)
    {
        cCmdState = ZDOT_RANGE_CONTROL_SET;             // Set for the Range Control Setting state
    }

// Requesting Range Control Read Mode
void commandRangeRead(void)
    {
        TxDirect(ePIR_SC2 & SC2_RANGE_CONTROL);         // Send the Range Value
    }

// Requesting Read Stability flag
void commandStabilityFlagRead(void)
    {
        if(cModuleStatus1 & MOD_PIR_STABLE)             // If we have a stable signal...
        {
            cCmdResponse = 'Y';                         // Load to send Yes
        }
        else                                            // Otherwise...
        {
            cCmdResponse = 'N';                         // Load to send No
        }
    }

// Requesting Extended Detection Write Mode
void commandExtSenseWrite(void)
    {
        cCmdState = ZDOT_EXT_SENSE_SET;                 // Set for the Extended Sens Setting state
    }

// Requesting Extended Detection Read Mode
void commandExtSenseRead(void)
    {
        TxDirect((ePIR_SC0 & SC0_EXTENDED) >> 6);       // Send the Extended Level value
    }

// Requesting Version Read Mode
void commandVersionRead(void)
    {
        TxDirect(AppVersionH);                          // Send the App Version
        TxDirect(ePIR_Version);                         // Send the ZMOTION Engine Version
    }

// Requesting Sensitivity Write Mode
void commandSensitivityWrite(void)
    {
        cCmdState = ZDOT_SENS_SET;                      // Set for the sensitivity setting state
    }

// Requesting Sensitivity Read Mode
void commandSensitivityRead(void)
    {
        TxDirect(ePIR_Sensitivity);                     // Send the Sensitivity value
    }

// Requesting Noise/Transient Flags Read Mode
void commandNoiseFlagsRead(void)
    {
        DI();                                           // Ensure that the following section is atomic
        TxDirect((ePIR_ASC0 & (ASC0_TRANSIENT_DETECTED | ASC0_NOISE_DETECTED))); // Send the Noise/Transient Fags
        ePIR_ASC0 &= ~(ASC0_TRANSIENT_DETECTED | ASC0_NOISE_DETECTED); // Clear the Noise/Transient Flags
        EI();                                           // Re-enable interrupts
    }


// Handle the RAM Write state
void commandRAMWrite(void)
    {
        cCmdState = ZDOT_RAM_WRITE;                     // Set for RAM Write handler
        cStateVar1 = 0;                                 // Set for MSB of address
    }

// Handle the RAM Read state
void commandRAMRead(void)
    {
        cCmdState = ZDOT_RAM_READ;                      // Load state for RAM Read handler
        cStateVar1 = 0;                                 // Set for MSB of address
    }

// Requesting ZMOTION MCU Reset
void commandMCUReset(void)
    {
        cCmdState = ZDOT_RESET_SET;                     // Set for the reset setting state
        cStateVar1 = '1';                               // Set for the first pattern byte
    }

// Requesting Flash Loader
void commandFlashLoader(void)
    {
        cCmdState = ZDOT_FLASH_LOAD_SET;                // Set for the reset setting state
        cStateVar1 = '1';                               // Set for the first pattern byte
    }


