;----------------------------------------------
;Daredevil Dennis
; Atom version by Kees van Oss  2019
;----------------------------------------------
	.DEFINE asm_code $e00
	.DEFINE header   1		; Header Wouter Ras emulator
	.DEFINE filenaam "DDD"

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


exec  = L0EF6
start_asm:
	.include "adennis.inc"
	.include "OSWRCH.inc"
eind_asm: