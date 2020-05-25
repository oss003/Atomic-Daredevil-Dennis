;----------------------------------------------
;Daredevil Dennis intro tune
; Atom version by Kees van Oss  2020
;----------------------------------------------
	.DEFINE asm_code $3A00
	.DEFINE header   1		; Header Wouter Ras emulator
	.DEFINE filenaam "SOUND"

.org asm_code-22*header

.IF header
;********************************************************************
; ATM Header for Atom emulator Wouter Ras

name_start:
	.byte filenaam			; Filename
name_end:
	.repeat 16-name_end+name_start	; Fill with 0 till 16 chars
	  .byte $0
	.endrep

	.word asm_code			; 2 bytes startaddress
	.word exec			; 2 bytes linkaddress
	.word eind_asm-start_asm	; 2 bytes filelength

;********************************************************************
.ENDIF


exec  = start_asm
start_asm:
	.include "sound.inc"
;	.include "introtune.inc"
	.include "elktune2.inc"
eind_asm: