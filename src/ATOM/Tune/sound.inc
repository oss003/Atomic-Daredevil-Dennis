; Define vars

L0050	= $50
L0051	= $51
L0052	= $52
L0053	= $53
L0054	= $54
L0092	= $92
L0093	= $93
LB002	= $b002
LFB83	= $fb83

; Define notes

F_1	= 0
G1	= 1
G_1	= 2
A1	= 3
A_1	= 4
B2	= 5
C2	= 6
C_2	= 7
D2	= 8
D_2	= 9
E2	= 10
F2	= 11
F_2	= 12
G2	= 13
G_2	= 14
A2	= 15
A_2	= 16
B3	= 17
C3	= 18
C_3	= 19
D3	= 20
D_3	= 21
E3	= 22
F3	= 23
F_3	= 24
G3	= 25
G_3	= 26
A3	= 27
A_3	= 28
B4	= 29
C4	= 30
C_4	= 31
D4	= 32
D_4	= 33
E4	= 34
F4	= 35

; Define note length

L1	= $04
L1_	= $06
L2	= $08
L2_	= $0c
L4	= $10
L4_	= $18
L8	= $20
L8_	= $30

play_music:
	lda #<tune		; Set tune address
	sta L0050
	lda #>tune
	sta L0051

	lda #0			; Reset tuneptr
	sta tun_ptr

music_loop:
	ldy tun_ptr
	lda (L0050),y		; Get frequency ptr
	pha
	tax
	lda freq_table,x	; Get frequency
	sta L0054

	iny
	lda (L0050),y		; Get length
	sta L0053
	pla
	beq play_wait		; Check for silence
	cmp #$ff
	beq music_end		; Check for end of tune

;=========================================================
; Parameters read:
; L0054 = frequency from frequency table
; L0053 = length from length table
;=========================================================

            ldy #$00               ; A0 00
            sty L0052              ; 84 52
            sty L0093              ; 84 93

            sec                    ; 38
LAB7A:      iny                    ; C8
            bne LAB7F              ; D0 02
            inc L0093              ; E6 93

LAB7F:      lda L0052              ; A5 52
            sbc L0054              ; E5 54
            sta L0052              ; 85 52

            lda L0053              ; A5 53
            sbc #$00               ; E9 00
            sta L0053              ; 85 53

            bcs LAB7A              ; B0 ED

            tya                    ; 98
            ldx #$03               ; A2 03
LAB90:      asl A                  ; 0A
            rol L0093              ; 26 93
            dex                    ; CA
            bne LAB90              ; D0 FA
            tay                    ; A8
            lda #$00               ; A9 00
            iny                    ; C8
            inc L0093              ; E6 93

; Play note loop

LAB9C:      eor #5              ; 45 92
            sta LB002              ; 8D 02 B0

; Waitloop for frequency

            ldx L0054              ; A6 54
LABA3:      dex                    ; CA
            nop                    ; EA
            nop                    ; EA
            nop                    ; EA
            bne LABA3              ; D0 FA

            dey                    ; 88
            bne LAB9C              ; D0 F0

            dec L0093              ; C6 93
            bne LAB9C              ; D0 EC
music_cont:
	clc
	lda L0050
	adc #2
	sta L0050
	bcc :+
	inc L0051
:
	jsr $fe71
	cpy #$ff
	bne music_end
	jmp music_loop

music_end:
	rts

play_wait:
	ldy L0053
wait_loop:
	ldx #1
	jsr LFB83
	dey
	bne wait_loop
	jmp music_cont

tun_ptr:
	.byte 0

freq_table:
	.byte $f6,$e7,$d9,$cd,$c2
	.byte $b7,$ad,$a3,$9a,$91,$89,$81,$79,$72,$6c,$66,$60
	.byte $5b,$56,$52,$4c,$48,$44,$40,$3c,$39,$36,$32,$2f
	.byte $2d,$2a,$27,$25,$23,$21,$1f

tune:
