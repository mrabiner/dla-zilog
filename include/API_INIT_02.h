/****************************************************************************/
//  Copyright (C) 1999-2010 by  Zilog, Inc.
//  All Rights Reserved.
//
//  File: API_INIT_02.h
//
//  Supported Lens:			Fresnel CM 0.77 GI V3
//  Supported PIR Sensors:	Nicera RE200B-P
//
//  This file contains the ePIR Engine configuration parameters for the lens
//  and PIR combination identified above. It should be included in the ZMOTION
//  application project along with ePIR_API.h and ePIR_API.c and loaded into
//  the API registers during initialization.
//
//  See sample project "ZMotion_Basic" for a full example of creating a
//  ZMOTION project. 
//
//  Revision: 1.0 - Initial Release
/****************************************************************************/

//  *** Do Not Modify or Remove the Following Values ***

#ifndef API_INIT_H
#define API_INIT_H

//////////////////////////////////////////////////////////////////////////
// API configuration for Normal Scan Rate Mode
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// ePIR_Sensitivity Register Default Setting for Normal Scan Rate
// Lower values provide greater sensitivity
//////////////////////////////////////////////////////////////////////////
#define EPIR_SENSITIVITY_DEF  12


//////////////////////////////////////////////////////////////////////////
// ePIR_SC0 Default Configuration for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// Bit 6-7 - Extended Detction - Level 1		(01)
// Bit 5 - Engine Disabled - Engine Controlled	(0)
// Bit 4 - MD Suspend - OFF						(0)
// Bit 3 - Motion Direction Control - OFF		(0)
// Bit 2 - Motion Direction - Engine Controlled	(0)
// Bit 1 - Motion Detected - Engine Controlled	(0)
// Bit 0 - PIR Stable - Engine Controlled		(0)
#define EPIR_SC0_DEF  0x00


//////////////////////////////////////////////////////////////////////////
// ePIR_SC1 Default Configuration for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// Bit 7 - Engine Timer Tick					(0)
// Bits 6-3 - Frequency Response				(1010)
// Bit 2 - PIR Scan Rate - Normal Mode			(0)
// Bit 1 - Reserved								(0)
// Bit 0 - Dual Pyro Enable - OFF				(0)
#define EPIR_SC1_DEF  0x50


//////////////////////////////////////////////////////////////////////////
// ePIR_SC2 Default Configuration for Normal Scan Rate
// Lower values provide greater range
//////////////////////////////////////////////////////////////////////////
// Bits 7-3 - Reserved							(00000)
// Bits 2-0 - Range								(000)
#define EPIR_SC2_DEF  0x01



//////////////////////////////////////////////////////////////////////////
// ePIR_SC3 Default Configuration for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// Bits 7-0 - ANAx Scan Request - None			(00000000)
// No ADC Scan requests made during Init
#define EPIR_SC3_DEF  0x00


//////////////////////////////////////////////////////////////////////////
// ePIR_ASC0 Default Configuration for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// Bits 7-5 - Reserved							(000)
// Bit 4 - Buffer Refresh - OFF					(0)
// Bit 3 - New Sample - Engine Controlled		(0)
// Bit 2 - MD Origin - Engine Controlled		(0)
// Bit 1 - EM Noise - Engine Controlled			(0)
// Bit 0 - EM Transient - Engine Controlled		(0)
#define EPIR_ASC0_DEF  0x00


//////////////////////////////////////////////////////////////////////////
// ePIR_ASC2 Default Configuration for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// Bits 7-5 - Lock Level - 2					(010)
// Bits 4-3 - Window Size - 3					(11)
// Bits 2-0 - Window Update Rate - 2			(010)
#define EPIR_ASC2_DEF  0x5A


//////////////////////////////////////////////////////////////////////////
// ePIR_Sample_Size Default Value for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// ePIR Sample Size - 32						(00100000)
#define EPIR_SAMPLE_SIZE_DEF  0x20


//////////////////////////////////////////////////////////////////////////
// ePIR_Debounce Time Default Value for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// ePIR Debounce Time - 120						(11001000)
#define EPIR_DEBOUNCE_DEF  0x78


//////////////////////////////////////////////////////////////////////////
// ePIR_Debounce_Batch Default Value for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// ePIR Debounce Batch Size - 255				(00111111)
#define EPIR_DEBOUNCE_BATCH_DEF  0x3F


//////////////////////////////////////////////////////////////////////////
// ePIR_Transient_Sense Default Value for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// ePIR Transient Sensitivity - Disabled		(00000000)
#define EPIR_TRANSIENT_SENSE_DEF  0x00


//////////////////////////////////////////////////////////////////////////
// ePIR_Noise_Sense Default Value for Normal Scan Rate
//////////////////////////////////////////////////////////////////////////
// ePIR Noise Sensitivity - Disabled			(00000000)
#define EPIR_NOISE_SENSE_DEF  0x00

#endif
