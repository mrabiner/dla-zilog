/************************************************************************************************
** File:        UART_Util.c
** Description: ZMOTION UART Utilities source file
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


#include <eZ8.h>                                        // Standard Z8 library header (pulls in other standard files)
#include "ePIR_API.h"                                   // ZMotion API header
#include "main.h"                                       // Project header
#include "UART_Util.h"


unsigned char cRXChar;                                  // Receive Data 'Buffer'
unsigned char cTXChar;                                  // Transmit Data 'Buffer'

// External variable declarations
extern unsigned char cModuleStatus1;                    // Module Status flags group 1
extern unsigned char cSerialTimeout;                    // Serial Interface inactivity timeout


///////////////////////////////////////////////////////////
// isrRX0 - UART Receive Interrupt
//  Receive serial data via UART0.
//  Data is stored in global variable cRXChar
//  Sets flag to indicate receive data is available
///////////////////////////////////////////////////////////
void interrupt isrRX0(void)
{
    cRXChar = U0RXD;                                    // Copy received byte
    cModuleStatus1 |= SER_BYTE_RX;                      // Indicate byte received
    cSerialTimeout = SER_IDLE_TIMEOUT;                  // Start the inactivity timeout
}

///////////////////////////////////////////////////////////
// Transmit byte via UART0
// Waits for transmitter to become available
// No WDT kick during wait because we shouldn't be here for more than ~100us
// H/W flow control is not used
///////////////////////////////////////////////////////////
void TxDirect(unsigned char txData)
{
    while ((U0STAT0 & 0x04)==0);        // Wait for the transmitter to be empty
    U0TXD = txData;                     // Send the character
}


// End of file
