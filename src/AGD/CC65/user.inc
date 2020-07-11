	cmp #0
	bne chk1
	jmp noise
chk1:
	cmp #1
	bne chk2
	jmp play_intro
chk2:
	rts

; White noise

noise:
	ldy #2
reploop:
	ldx #0
noiseloop:
	lda noisebyte
	pha
	and #4
	sta $b002
	pla
	and #$48
	adc #$38
	asl a
	asl a
	rol noisebyte+2
	rol noisebyte+1
	rol noisebyte
	dex
	bne noiseloop
	dey
	bne reploop
	rts

noisebyte:	.byte 34,137,251

;================================================

play_intro:
	.include "sound.inc"
	.include "introtune.inc"
