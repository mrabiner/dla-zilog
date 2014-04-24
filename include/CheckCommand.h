/******************************************************************
* CheckCommand.h
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
#ifndef CHECK_COMMAND_H
#define CHECK_COMMAND_H

typedef struct CmdEntry{
    unsigned char cCmd;
    void (*callback)(void);
}CmdEntry;

// Serial Command list
#define CMD_MD_STATUS_READ              'a'
#define CMD_RAM_READ                    'b'
#define CMD_RAM_WRITE                   'B'
#define CMD_EXT_SENSE_READ              'e'
#define CMD_EXT_SENSE_WRITE             'E'
#define CMD_FREQ_RESP_READ              'f'
#define CMD_FREQ_RESP_WRITE             'F'
#define CMD_VERSION_READ                'i'
#define CMD_NO_MOTION_CHECK_TIME_READ   'j'
#define CMD_NO_MOTION_CHECK_TIME_WRITE  'J'
#define CMD_REAL_TIME_MD_READ           'm'
#define CMD_REAL_TIME_MD_WRITE          'M'
#define CMD_NOISE_SENS_READ             'n'
#define CMD_NOISE_SENS_WRITE            'N'
#define CMD_RANGE_CONTROL_READ          'r'
#define CMD_RANGE_CONTROL_WRITE         'R'
#define CMD_SENS_READ                   's'
#define CMD_SENS_WRITE                  'S'
#define CMD_TRANS_SENS_READ             't'
#define CMD_TRANS_SENS_WRITE            'T'
#define CMD_STABILITY_FLAG_READ         'u'
#define CMD_RESET_REQUEST               'X'
#define CMD_NOISE_FLAGS_READ            'y'
#define CMD_FLASH_LOAD_REQUEST          'Z'


// Serial command states
#define ZDOT_IDLE                       0
#define ZDOT_SENS_SET                   1
#define ZDOT_MOTION_OFF_SET             3
#define ZDOT_FREQ_SET                   4
#define ZDOT_RAM_WRITE                  6
#define ZDOT_RAM_READ                   7
#define ZDOT_RESET_SET                  9
#define ZDOT_FLASH_LOAD_SET             10
#define ZDOT_REAL_TIME_MD_SET           11
#define ZDOT_EXT_SENSE_SET              12
#define ZDOT_NOISE_SENS_SET             13
#define ZDOT_TRANS_SENS_SET             14
#define ZDOT_RANGE_CONTROL_SET          17


void CheckCommand(unsigned char);			// Check the newly received command
void commandStatusRead(void);
void commandRealTimeMDWrite(void);
void commandRealTimeMDRead(void);
void commandNoiseSensWrite(void);
void commandNoiseSensRead(void);
void commandTransientSensWrite(void);
void commandTransientSensRead(void);
void commandNoMotionCheckTimeWrite(void);
void commandNoMotionCheckTimeRead(void);
void commandFreqRespWrite(void);
void commandFreqRespRead(void);
void commandRangeWrite(void);
void commandRangeRead(void);
void commandStabilityFlagRead(void);
void commandExtSenseWrite(void);
void commandExtSenseRead(void);
void commandVersionRead(void);
void commandSensitivityWrite(void);
void commandSensitivityRead(void);
void commandNoiseFlagsRead(void);
void commandRAMWrite(void);
void commandRAMRead(void);
void commandMCUReset(void);
void commandFlashLoader(void);

#endif
