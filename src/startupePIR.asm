;--------------------------------------------------------------
; ePIR Startup Routine for the eZ8 Compiler
;
; Copyright (C) 1999-2010 by Zilog, Inc.
; All Rights Reserved
;--------------------------------------------------------------
;
		INCLUDE "ez8.inc"			; Defines valid device peripheral control registers
		INCLUDE "CONFIGS.INC"		; Conditional assembly definitions

;*****************************************
;
;	Start of code
;	Conditional assembly values are defined in configs.inc
;
;*****************************************
	XDEF	_c_startup
	XDEF	_errno
	XDEF 	__intrp
	XDEF	_cResetStatus
;
;	Vector table space allocation
;
; Remove vector include file when using a bootloader
;	INCLUDE "vect.inc"
;
;	Reset vector definition
;
; Remove reset vector when using a bootloader
;	VECTOR	reset = _c_startup

;
;	Declare seperate code segment for interrupt vectors. Fix to %0204 for botloader to jump into
;
	DEFINE	INTJUMPS, SPACE = ROM, ORG = %0204
	SEGMENT	INTJUMPS
	XREF	_isrTimer0
	XREF 	_isrRX0
	XREF 	_isrADC_EOC

	jp	_c_startup					; WDT Vector
	jp	_c_startup					; TRAP Vector
	jp	_c_startup					; TIMER1 Vector
	jp	_isrTimer0					; TIMER0 Vector
	jp	_isrRX0						; UART0_RX Vector
	jp	_c_startup					; UART0_TX Vector
	jp	_isrADC_EOC					; ADC Vector
	jp	_c_startup					; P7AD Vector
	jp	_c_startup					; P6AD Vector
	jp	_c_startup					; P5AD Vector
	jp	_c_startup					; P4AD Vector
	jp	_c_startup					; P3AD Vector
	jp	_c_startup					; P2AD Vector
	jp	_c_startup					; P1AD Vector
	jp	_c_startup					; P0AD Vector
	jp	_c_startup					; C3 Vector
	jp	_c_startup					; C2 Vector
	jp	_c_startup					; C1 Vector
	jp	_c_startup					; C0 Vector
	jp	_c_startup					; POTRAP Vector
	jp	_c_startup					; WOTRAP Vector


;
;	Declare seperate code segment for Startup. Fixed at 0x0243 when using the ZMOTION Bootloader
;
	define startup, space=rom, org = %0243
	segment startup

_c_startup:

	SRP		#%00			; Set register pointer, Working registers 00-0f
;
;	Initialize hardware stack
;	Due to Memory organization, the near stack must be set manually to avoid overlapping with the Advanced API
;
 IF	INITSP
	LDX     SPL,#%70
	LDX     SPH,#%00
 ENDIF
;
;	Clear internal ram area (c_nearbss)
;
; When using the bootloader, RSTSTAT was copied to %10
;	LDX		_cResetStatus,RSTSTAT	; Get a copy of the reset status
;	TM		_cResetStatus,#%40		; Is this an SMR reset?
	LD		_cResetStatus,%10		; Get a copy of the reset status
	TM		_cResetStatus,#%40		; Is this an SMR reset?
	JR		NZ,NORAMCLR				; If so then no RAM clear or init

 IF CLRRRAM
	XREF	_low_nearbss
	XREF	_len_nearbss

	LD		R0,#_low_nearbss
	LD		R2,#_len_nearbss
	CP		R2,#0
	JR		Z,lab4
lab3:
	CLR		@R0
	INC		R0
	DJNZ	R2,lab3
lab4:
 ENDIF

;
;	Clear extended ram area (c_farbss)
;
 IF CLRERAM
	XREF	_low_farbss
	XREF	_len_farbss

	LD		R2,#high(_low_farbss)
	LD		R3,#low(_low_farbss)
	LD		R0,#high(_len_farbss)
	LD		R1,#low(_len_farbss)

	LD		R4,R0
	OR		R4,R1
	JR		Z,lab6
	CLR		R4
lab5:
	LDX		@RR2,R4
	INCW	RR2
	DECW	RR0
	JR		NZ,lab5
lab6:
 ENDIF

;
;	Copy ROM data into internal RAM
;
 IF COPYRRAM
	XREF	_low_neardata
	XREF	_len_neardata
	XREF	_low_near_romdata

	LD		R0,#high(_low_near_romdata)
	LD		R1,#low(_low_near_romdata)
	LD		R3,#_len_neardata
	LD		R4,#_low_neardata
	OR		R3,R3
	JR		Z,lab10
lab9:
	LDCI	@R4,@RR0
	DJNZ	R3,lab9
lab10:
 ENDIF

;
;	Copy ROM data into extended RAM
;
 IF COPYERAM
	XREF	_low_fardata
	XREF	_len_fardata
	XREF	_low_far_romdata

	LD		R0,#high(_low_fardata)
	LD		R1,#low(_low_fardata)
	LD		R2,#high(_low_far_romdata)
	LD		R3,#low(_low_far_romdata)
	LD		R4,#high(_len_fardata)
	LD		R5,#low(_len_fardata)

	LD		R6,R4
	OR		R6,R5
	JR		Z,lab12

lab11:
	LDC		R6,@RR2
	LDX		@RR0,R6
	INCW	RR0
	INCW	RR2
	DECW	RR4
	JR		NZ,lab11

lab12:
 ENDIF

NORAMCLR:
	LD		__intrp,#%10


 IFDEF _ZSL_PORT_USED		; Defined when ZSL is seleced in Project Settings
;****************************************************
; Initialize the peripheral devices supported by ZSL
;****************************************************
	XREF	__open_periphdevice

	CALL	__open_periphdevice

 ENDIF
;
;	Call main
;
	XREF	_main:ROM

	XOR		R15,R15
	XOR		R14,R14
	CALL	_main

 IFDEF _ZSL_PORT_USED		; Defined when ZSL is seleced in Project Settings
;****************************************************
; Close the peripheral devices supported by ZSL
;****************************************************
	XREF	__close_periphdevice

	call	__close_periphdevice

 ENDIF

	XDEF	_exit
_exit:
	JR		_exit

;*****************************************
;
;		Data area
;
;*****************************************
; Do not use Large Memory Model with ZMotion Project!!
; Small Memory Model must be used.
; Ensure "Configs.inc" MODEL EQU SMALL
; Ensure [Project Settings] -> [C] -> [Code Generation] -> [Memory Model] = "Small"

	SEGMENT	NEAR_BSS

_errno			DS	2
__intrp			DS	1
_cResetStatus	DS	1

	END		_c_startup

; End of file
