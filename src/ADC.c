/************************************************************************************************
** File:        ADC.c
** Description: ZMOTION ADC source file
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


#include "ePIR_API.h"							// ZMotion API header
#include "ADC.h"								// ADC header file

///////////////////////////////////////////////////////////
//Interrupt routine for ADC EOC
///////////////////////////////////////////////////////////
void interrupt isrADC_EOC(void)
{
	EPIR_ADC_ISR;								// Call the engine ADC EOC ISR
}
