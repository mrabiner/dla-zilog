/******************************************************************
* main.h
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
#ifndef MAIN_H
#define MAIN_H


///////////////////////////////////////////////////////////
// User configurable application settings
///////////////////////////////////////////////////////////
#define DEF_MD_ON_TIME      DELAY_500MSEC               // Time MD stays on when motion is detected
#define DEF_NO_MOTION_TIME  DELAY_500MSEC               // Default time to not look for motion after a motion event has been detected
                                                        // that will be considered part of the same motion event.

// Identification for user to uniquely identify each released version of the application
#define APP_VERSION_HIGH 0x01
#define APP_VERSION_LOW 0x00

#define FLASHLOADER 1                                   // If Application code is loaded via ZMOTION Flash Loader, set this to 1

///////////////////////////////////////////////////////////
// Peripheral configuration Macro definitions
///////////////////////////////////////////////////////////
#define OSCCTL_VAL      0xA0                            // Oscillator control
#define PWRCTL0_VAL     0x8A                            // Power control 0 register value

///////////////////////////////////////////////////////////
// Application Definitions
///////////////////////////////////////////////////////////
// Initial default values
#define MOD_STAT1_DEF       0x00                        // cModuleSatus1 register POR Default value

#define MOD_STAT2_DEF       0x00                        // cModuleSatus2 register POR Default value

// Flags defined in cModuleStatus1
#define SER_BYTE_RX         0x01                        // Serial Data received flag
#define SER_MOTION_DETECTED 0x02                        // Motion Detected copy for Serial Mode
#define SER_REAL_TIME_MD    0x04                        // Serial sends real-time Motion Detected Status
#define MS1_08_FREE         0x08                        // Free
#define MS1_10_FREE         0x10                        // Free
#define MOD_PIR_STABLE      0x20                        // PIR Sensor is stable after powerup - includes extra delay
#define PIR_PRE_STABLE      0x40                        // Engine reported PIR stable
#define SERIAL_TIMEOUT      0x80                        // Indicates that the serial timeout timer has expired

// Flags defined in cModuleStatus2
#define MS1_01_FREE         0x01                        // Free
#define MS1_02_FREE         0x02                        // Free
#define MS1_04_FREE         0x04                        // Free
#define MS1_08_FREE         0x08                        // Free
#define ASCII_MODE_ON       0x10                        // Serial Interface Command Mode Enabled
#define MS1_20_FREE         0x20                        // Free
#define MS1_40_FREE         0x40                        // Free
#define MOD_SLEEP_REQ       0x80                        // Sleep Mode has been requested

// Time delay values based on 50ms time base
#define DELAY_0SEC          0
#define DELAY_100MSEC       2
#define DELAY_200MSEC       4
#define DELAY_500MSEC       10
#define DELAY_1SEC          20
#define DELAY_2SEC          40
#define DELAY_5SEC          100
#define DELAY_10SEC         200
#define DELAY_12SEC         240
#define DELAY_15SEC         300
#define DELAY_20SEC         400
#define DELAY_25SEC         500
#define DELAY_30SEC         600
#define DELAY_60SEC         1200                        // 1 Minute delay
#define DELAY_90SEC         1800                        // 1.5 Minute delay
#define DELAY_120SEC        2400                        // 2 Minute delay
#define DELAY_150SEC        3000                        // 2.5 Minute delay
#define DELAY_180SEC        3600                        // 3 Minute delay
#define DELAY_210SEC        4200                        // 3.5 Minute delay
#define DELAY_240SEC        4800                        // 4 Minute delay
#define DELAY_270SEC        5400                        // 4.5 Minute delay
#define DELAY_300SEC        6000                        // 5 Minute delay
#define DELAY_450SEC        9000                        // 7.5 Minute delay
#define DELAY_600SEC        12000                       // 10 Minute delay
#define DELAY_900SEC        18000                       // 15 Minute delay
#define DELAY_1200SEC       24000                       // 20 Minute delay
#define DELAY_1500SEC       30000                       // 25 Minute delay
#define DELAY_1800SEC       36000                       // 30 Minute delay


///////////////////////////////////////////////////////////
// Application Macro definitions
///////////////////////////////////////////////////////////
// Motion Alarm output macro definitions
#define MD_PORT PAOUT                                   // Motion Signal Port
#define MD_BIT 0x04                                     // Motion Signal Activation mask (PA2)
#define MD_OFF  MD_PORT &= ~MD_BIT                      // Motion Signal (LED) Off
#define MD_ON   MD_PORT |= MD_BIT                       // Motion Signal (LED) On

///////////////////////////////////////////////////////////
// In-Line assembly macros
///////////////////////////////////////////////////////////
#define Z8_NOP      asm("NOP")                          // Do nothing
#define Z8_WDT      asm("WDT")                          // Refresh the watchdog
#define Z8_HALT     asm("HALT")                         // Enter "Halt" mode
#define Z8_STOP     asm("STOP")                         // Enter "Stop" mode
#define Z8_ATM      asm("ATM")                          // DI for the next 3 assembly instructions

///////////////////////////////////////////////////////////
// Function Prototypes
///////////////////////////////////////////////////////////
extern void c_startup(void);                            // Startup vector for unused vectors
void interrupt isrADC_EOC(void);                        // ADC EOC interrupt

///////////////////////////////////////////////////////////
// Reserved Interrupt Vector definitions
///////////////////////////////////////////////////////////
#ifndef TIMER2
#define TIMER2      3
#endif
#ifndef I2C
#define I2C         8
#endif
#ifndef SPI
#define SPI         9
#endif
#ifndef TIMER3
#define TIMER3      19
#endif
#ifndef UART1_RX
#define UART1_RX    20
#endif
#ifndef UART1_TX
#define UART1_TX    21
#endif
#ifndef DMA
#define DMA         22
#endif
#ifndef C3
#define C3          23
#endif
#ifndef C2
#define C2          24
#endif
#ifndef C1
#define C1          25
#endif
#ifndef C0
#define C0          26
#endif

#endif

// End of file

