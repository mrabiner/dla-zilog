//  Copyright (C) 2011 by  ZiLOG, Inc.
//  All Rights Reserved.
//
//  File: ePIR_API.h
//  ePIR API Bit Definitions. Do Not Modify or Remove These Values
//

#ifndef EPIR_API_H
#define EPIR_API_H

#undef EZ8_NVDS

//*** Standard API Interface ***//

// ePIR Enable Register (ePIR_Enable) Enable/Disable patterns
#define EPIR_DISABLE_PATTERN		0x00		// Pattern to disable Engine operations
#define EPIR_ENABLE_PATTERN			0x11		// Pattern to enable Engine operations

// Bit definitions of ePIR Sensitivity Register (ePIR_Sensitivity)
#define ePIR_SENSITIVITY			0xFF		// R/W bits

// Bit definitions of ePIR API Status\Control Register 0 (ePIR_SC0)
#define SC0_PIR_STABLE				0x01		// Read Only bit
#define SC0_MOTION_DETECTED			0x02		// R/W bit
#define SC0_MOTION_DIRECTION		0x04		// Read Only bit
#define SC0_DIRECTION_CONTROL		0x08		// R/W bit
#define SC0_MD_SUSPEND				0x10		// R/W bit
#define SC0_ENGINE_DISABLED			0x20		// Read Only bit
#define SC0_EXTENDED				0xC0		// R/W bit

// Bit definitions of ePIR API Status\Control Register 1 (ePIR_SC1)
#define SC1_DUAL_PYRO_ENABLE		0x01		// R/W bit
#define SC1_2_PULSE_ENABLE			0x02		// R/W bit
#define SC1_PIR_SCAN_RATE			0x04		// R/W bit
#define SC1_FREQUENCY_RESPONSE		0x78		// R/W bits
#define SC1_ENGINE_TIMER_TICK		0x80		// R/W bit

// Bit definitions of ePIR API Status\Control Register 2 (ePIR_SC2)
//#define SC2_WHITE_LIGHT_THRESHOLD	0xF8		// R/W bits - Intrusion only
#define SC2_RANGE_CONTROL			0x07		// R/W bits

// Bit definitions of ePIR API Status\Control Register 3 (ePIR_SC3)
#define SC3_ANA0_SCAN_REQUEST		0x01		// R/W bit
#define SC3_ANA1_SCAN_REQUEST		0x02		// R/W bit
#define SC3_BUFFERRED_MODE			0x04		// R/W bit
#define SC3_ANA3_SCAN_REQUEST		0x08		// R/W bit
#define SC3_ANA4_SCAN_REQUEST		0x10		// R/W bit
#define SC3_ANA5_SCAN_REQUEST		0x20		// R/W bit
#define SC3_ANA6_SCAN_REQUEST		0x40		// R/W bit
#define SC3_ANA7_SCAN_REQUEST		0x80		// R/W bit


//*** Advanced API Interface ***//

// Bit definitions of ePIR Advanced API Status\Control Register 0 (ePIR_ASC0)
#define ASC0_TRANSIENT_DETECTED		0x01		// R/W bit
#define ASC0_NOISE_DETECTED			0x02		// R/W bit
#define ASC0_MD_ORIGIN				0x04		// Read Only bit
#define ASC0_NEW_SAMPLE				0x08		// R/W bit
#define ASC0_BUFFER_REFRESH			0x10		// R/W bit
#define ASC0_WHITE_LIGHT_ANTI_JAM	0x20		// Reserved
#define ASC0_WHITE_LIGHT_DETECTED	0x40		// Reserved
#define ASC0_EXTERNAL_VREF			0x80		// Reserved

// Bit definitions of ePIR Advanced API Status\Control Register 1 (ePIR_ASC1)
#define ASC1_WHITE_LIGHT_SCAN_RATE	0x0F		// R/W bit
#define ASC1_WHITE_LIGHT_DEBOUNCE	0xF0		// R/W bit

// Bit definitions of ePIR Advanced API Status\Control Register 2 (ePIR_ASC2)
#define ASC2_WINDOW_UPDATE_RATE		0x07		// R/W bits
#define ASC2_WINDOW_SIZE			0x18		// R/W bits
#define ASC2_LOCK_LEVEL				0xE0		// R/W bits

// Bit definitions of ePIR Process Rate Register (ePIR_Process_Rate)
#define ePIR_PROCESS_RATE			0xFFFF		// Read Only

// Bit definitions of ePIR Sample Size Register (ePIR_Sample_Size)
#define ePIR_SAMPLE_SIZE			0xFF		// R/W bits

// Bit definitions of ePIR Debounce Time Register (ePIR_Debounce)
#define ePIR_DEBOUNCE_TIME			0xFF		// R/W bits

// Bit definitions of ePIR Debounce Batch Register (ePIR_Debounce_Batch)
#define ePIR_DEBOUNCE_BATCH			0xFF		// R/W bits

// Bit definitions of ePIR Transient Sensitivity Level Register (ePIR_Transient_Sense)
#define ePIR_TRANSIENT_SENSE				0x7F	// R/W bits
#define ePIR_TRANSIENT_SENSE_BIT7_RESERVED	0x80	// Reserved

// Bit definitions of ePIR Noise Sensitivity Register (ePIR_Noise_Sense)
#define ePIR_NOISE_SENSE					0x7F	// R/W bits
#define ePIR_NOISE_SENSE_BIT7_RESERVED		0x80	// Reserved

// Bit definitions of ePIR Signal Register (ePIR_Signal)
#define ePIR_SIGNAL					0xFFFF		// Read Only

// Bit definitions of ePIR DC Signal Register (ePIR_DC_Signal)
#define ePIR_DC_SIGNAL				0xFFFF		// Read Only

// Bit definitions of ePIR Extended Debounce Register (ePIR_Ext_Debounce) - Intrusion only
//#define	ePIR_DEBOUNCE_EXT			0xFF

// Bit definitions of ePIR Extended Timeout Register (ePIR_Ext_Timeout) - Intrusion only
//#define	ePIR_TIMEOUT_EXT			0xFF


//*** ePIR Engine Entry Macro Definitions ***//

#define EPIR_INIT asm("\tPUSHX RP\n\tLDX RP,#%E0\n\tCALL %1FFD\n\tPOPX RP")
#define EPIR_ADC_ISR asm("\tPUSHX RP\n\tLDX RP,#%E0\n\tCALL %1000\n\tPOPX RP")


//*** Standard API Memory Space External Definitions ***//
extern far unsigned char	ePIR_Enable				_At 0x100;
extern far unsigned char	ePIR_Sensitivity		_At 0x101;
extern far unsigned char	ePIR_SC0				_At 0x102;
extern far unsigned char	ePIR_SC1				_At 0x103;
extern far unsigned char	ePIR_SC2				_At 0x104;
extern far unsigned char	ePIR_SC3				_At 0x105;
extern far unsigned char	ePIR_Reserved_106		_At 0x106;
extern far unsigned char	ePIR_Reserved_107		_At 0x107;
extern far unsigned char	ePIR_Reserved_108		_At 0x108;
extern far unsigned char	ePIR_Reserved_109		_At 0x109;
extern far unsigned int		ePIR_ADC_Result			_At 0x10A;
extern far unsigned char	ePIR_Version			_At 0x10C;
extern far unsigned char	ePIR_Reserved_10D		_At 0x10D;
extern far unsigned char	ePIR_Reserved_10E		_At 0x10E;
extern far unsigned char	ePIR_Reserved_10F		_At 0x10F;

//*** Advanced API Memory Space External Definitions ***//
extern near unsigned char	ePIR_ASC0				_At 0xF0;
extern near unsigned char	ePIR_ASC1				_At 0xF1;
extern near unsigned char	ePIR_ASC2				_At 0xF2;
extern near unsigned int	ePIR_Process_Rate		_At 0xF3;
extern near unsigned char	ePIR_Sample_Size		_At 0xF5;
extern near unsigned char	ePIR_Debounce			_At 0xF6;
extern near unsigned char	ePIR_Debounce_Batch		_At 0xF7;
extern near unsigned char	ePIR_Transient_Sense	_At 0xF8;
extern near unsigned char	ePIR_Noise_Sense		_At 0xF9;
extern near unsigned int	ePIR_Signal				_At 0xFA;
extern near unsigned int	ePIR_Signal_DC			_At 0xFC;
//extern near unsigned char	ePIR_Ext_Debounce		_At 0xFE;	// Intrusion only
//extern near unsigned char	ePIR_Ext_Timeout		_At 0xFF;	// Intrusion only

#endif

// End of file
