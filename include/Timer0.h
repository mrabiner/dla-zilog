/******************************************************************
* Timer0.h
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
#ifndef TIMER0_H
#define TIMER0_H

// Timer 0 configuration parameters
#define T0RH_VAL		0x43						// Timer 0 reload value high for 50mS time out
#define T0RL_VAL		0x81						// Timer 0 reload value low  for 50mS time out
#define T0CTL0_VAL		0x00						// Timer 0 Control 0 Register value
#define T0CTL1_VAL		0xE1						// Timer 0 Control 1 Register value (Divide by 16)

void interrupt isrTimer0(void);

#endif
