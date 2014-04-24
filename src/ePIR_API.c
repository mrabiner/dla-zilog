//  Copyright (C) 2011 by  ZiLOG, Inc.
//  All Rights Reserved.
//
//  File: ePIR_API.c
//  ePIR API Bit Definitions. Do Not Modify or Remove These Values
//
//*** Standard API Interface ***//

// Address of ePIR API Enable Register.
far unsigned char	ePIR_Enable			_At	0x100;

// Address of ePIR API Sensitivity Regsiter
far unsigned char	ePIR_Sensitivity	_At	0x101;

// Address of ePIR API Status and Control Register 0
far unsigned char	ePIR_SC0			_At	0x102;

// Address of ePIR API Status and Control Register 1
far unsigned char	ePIR_SC1			_At	0x103;

// Address of ePIR API Status and Control Register 2
far unsigned char	ePIR_SC2			_At	0x104;

// Address of ePIR API Status and Control Register 3
far unsigned char	ePIR_SC3			_At	0x105;

// Address of Reserved Standard API Registers
far unsigned char	ePIR_Reserved_106	_At	0x106;
far unsigned char	ePIR_Reserved_107	_At	0x107;
far unsigned char	ePIR_Reserved_108	_At	0x108;
far unsigned char	ePIR_Reserved_109	_At	0x109;

// Address of ePIR ADC result Register
far unsigned int	ePIR_ADC_Result		_At	0x10A;

// Address of ePIR Engine S/W Version Register
far unsigned char	ePIR_Version		_At	0x10C;

// Address of Reserved Standard API Registers
far unsigned char	ePIR_Reserved_10D	_At	0x10D;
far unsigned char	ePIR_Reserved_10E	_At 0x10E;
far unsigned char	ePIR_Reserved_10F	_At	0x10F;


//*** Advanced API Interface ***//

// Address of ePIR Advanced API Status\Control Register 0 (ePIR_ASC0)
near unsigned char	ePIR_ASC0				_At	0xF0;

// Address of Reserved Advanced API Status\Control Register 1 (ePIR_ASC1)
near unsigned char	ePIR_ASC1				_At	0xF1;

// Address of ePIR Advanced API Status\Control Register 2 (ePIR_ASC2)
near unsigned char	ePIR_ASC2				_At	0xF2;

// Address of ePIR Process Rate Register (ePIR_Process_Rate)
near unsigned int	ePIR_Process_Rate		_At	0xF3;

// Address of ePIR Sample Size Register (ePIR_Sample_Size)
near unsigned char	ePIR_Sample_Size		_At	0xF5;

// Address of ePIR Debounce Time Register (ePIR_Debounce)
near unsigned char	ePIR_Debounce			_At	0xF6;

// Address of ePIR Debounce Batch Size Register (ePIR_Debounce_Batch)
near unsigned char	ePIR_Debounce_Batch		_At	0xF7;

// Address of ePIR Transient Sensitivity Register (ePIR_Transient_Sense)
near unsigned char	ePIR_Transient_Sense	_At	0xF8;

// Address of ePIR Noise Sensitivity Register (ePIR_Noise_Sense)
near unsigned char	ePIR_Noise_Sense		_At	0xF9;

// Address of ePIR PIR Signal Register (ePIR_Signal)
near unsigned int	ePIR_Signal				_At	0xFA;

// Address of ePIR PIR Signal DC Level Register (ePIR_Signal_DC)
near unsigned int	ePIR_Signal_DC			_At	0xFC;

// Address of ePIR Extended Debounce Register (ePIR_Ext_Debounce) - Intrusion only
//near unsigned char ePIR_Ext_Debounce        _At 0xFE;

// Address of ePIR Extended Debounce Register (ePIR_Ext_Timeout) - Intrusion only
//near unsigned char ePIR_Ext_Timeout         _At 0xFF;

// End of file
