/******************************************************************
* UART_Util.h
*
* Copyright (C) 1999-2011 by  ZiLOG, Inc.
* All Rights Reserved.
*
* Permission to use this code is granted on a royalty-free
* basis. However users are cautioned to authenticate the
* code contained herein.
*
* Zilog DOES NOT GUARANTEE THE VERACITY OF THE SOFTWARE.
******************************************************************/
#ifndef UART_UTIL_H
#define UART_UTIL_H

#define RX_BUFFER_SIZE 8

// UART Configuration definitions
//#define U0_BAUD_HIGH    0x00						    // UART 0 BRG high setting for 9600 Baud with IPO
//#define U0_BAUD_LOW     0x24						    // UART 0 BRG low setting for 9600 Baud with IPO
#define U0_BAUD_HIGH    0x00						    // UART 0 BRG high setting for 115200 Baud with IPO
#define U0_BAUD_LOW     0x03						    // UART 0 BRG low setting for 115200 Baud with IPO
#define U0CTL0_VAL      0xC0						    // UART 0 Control Register 0 value
#define U0CTL1_VAL      0x00						    // UART 0 Control Register 1 value

// Serial Interface Mode Inactivity timeout
#if DEBUG                                               // When debugging, give a longer timeout, for terminal use
#define SER_IDLE_TIMEOUT DELAY_5SEC
#else
#define SER_IDLE_TIMEOUT DELAY_2SEC
#endif

// Serial Interface Macro definitions

#define ACK_CHAR        0x06
#define NACK_CHAR       0x15
#define AWAKE_CHAR       0x16
#define SER_ACK TxDirect(ACK_CHAR)
#define SER_NACK TxDirect(NACK_CHAR)
#define SER_AWAKE TxDirect(AWAKE_CHAR)

void interrupt isrRX0(void);						    // UART0 RX interrupt
void TxDirect(unsigned char);

#endif
