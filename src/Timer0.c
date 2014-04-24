/************************************************************************************************
** File:        Timer0.c
** Description: ZMOTION Timer 0 source file
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


#include <eZ8.h>                                // Standard Z8 library header (pulls in other standard files)
#include "ePIR_API.h"                           // ZMotion API header
#include "main.h"                               // Project header
#include "Timer0.h"                             // Timer 0 header
#include "UART_Util.h"                          // UART_Util header file

extern unsigned char    MDOnTime;               // Default /MD on Time
extern unsigned char    MotionOffTime;          // Default time to not look for motion when motion is detected (min time between events)
extern unsigned char    cModuleStatus1;         // Module Status flags group 1
extern unsigned char    cModuleStatus2;         // Module Status flags group 2
extern unsigned char    cCmdState;              // Serial interface state register
extern unsigned char    cMDDelay;               // /MD Output delay timer
extern unsigned char    cMDCheck;               // Counter used to control time between checking for motion
extern unsigned char    cSerialTimeout;         // Serial Interface inactivity timeout
extern unsigned char    c50mSecond;             // 50 mS counter to generate a 1 second tick for house keeping
extern unsigned char    c1Second;               // 1 Second counter to generate a 1 minute tick

///////////////////////////////////////////////////////////
// Interrupt routine for Timer 0 - runs every 50mS
//
//  Handle MDCheck timer - ignore motion timer
//  Handle MD output timer - Turns MD off when time expires
//  Handle Serial Timeout timer
//  Handle One-Second timer for Engine Timer Tick
//  Handle power-on pyro stabilization time
///////////////////////////////////////////////////////////
void interrupt isrTimer0(void)
{
// This is a little delay timer used to ignore motion events for set amount of time
    if(cMDCheck > 0)                            // When it hits 0, we check the MD bit in the main loop
    {
        cMDCheck--;                             // Decrement the MD check counter
        if(cMDCheck == 0)                       // When it hits 0, we check the MD bit in the main loop
        {
            ePIR_SC0 &= ~SC0_MOTION_DETECTED;   // Clear engine Motion detected flag
            ePIR_ASC0 |= ASC0_BUFFER_REFRESH;
        }
    }

// This is the MD Output timer
    if(cMDDelay > 0)                            // If MD is still activated
    {
        cMDDelay--;                             // Decrement the LED activation time
        if(cMDDelay == 0)                       // If the LED activation time has expired
        {
            MD_OFF;                             // Turn off the LED Output port
        }
    }


// Handle the serial time out timer
    if(cSerialTimeout > 0)                      // If we are handling serial timeout
    {
        cSerialTimeout--;                       // Decrement the Serial timeout counter--
        if(cSerialTimeout == 0)                 // If the received byte delay is greater than the timeout value
        {
            cModuleStatus1 |= SERIAL_TIMEOUT;   // Set the Serial Timeout Flag
        }
    }

// Handle the one second and one minute timers
    c50mSecond++;                                   // Increment the 50 ms counter
//1 second tick area
    if (c50mSecond >= DELAY_1SEC)                   // If we have reached one second...
    {
        c50mSecond = 0;                             // Reset the 50 ms counter
        ePIR_SC1 |= SC1_ENGINE_TIMER_TICK;          // Pass the one second tick to the ePIR engine

// 1 minute tick area
        c1Second++;                                 // Increment the one second counter
        if (c1Second >= 60)                         // If we have reached one minute...
        {
            c1Second = 0;                           // Clear the one second counter
        }
///////////////////////////////////////////////////////////
// Check for PIR sensor stability after power up
///////////////////////////////////////////////////////////
        if(!(cModuleStatus1 & MOD_PIR_STABLE) && cMDCheck == 0)
        {
            if(!(cModuleStatus1 & PIR_PRE_STABLE))          // If we have not seen engine stability after power up...
            {
                if (ePIR_SC0 & SC0_PIR_STABLE)              // If the PIR sensor has become stable...
                {
                    cModuleStatus1 |= PIR_PRE_STABLE;       // Indicate that the system has stabilized after powerup
                    cMDCheck = DELAY_5SEC;                  // Add some extra time to pyro stablization - just to be sure
                }
/*              else
                {
                    if(c1Second >= 30)                      // Limit stability time to 30 seconds
                    {
                        cModuleStatus1 |= (PIR_PRE_STABLE | MOD_PIR_STABLE);
                        MD_OFF;                             // Set /MD to inactive to show we are ready
                    }
                }
*/
            }
            else
            {
                cModuleStatus1 |= MOD_PIR_STABLE;           // Indicate that the system has stabilized after powerup
                MD_OFF;                                     // Set /MD to inactive to show we are ready
                cMDCheck = DELAY_500MSEC;
            }
        }
    }
}
