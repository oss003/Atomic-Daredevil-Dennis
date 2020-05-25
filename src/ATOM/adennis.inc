;
; Code equates
;

LFFEE	= $FFEE
LFFF1	= $FFF1
LFFF4	= $FFF4	; Print character
LFE71	= $FE71	; Read key without waiting
LFB81	= $FB81	; Wait 0,5 sec
LFE66	= $FE66	; Wait 1/60 sec

green	= 0
yellow	= 1
blue	= 2
red	= 3

;-------------------------------------------------
; Workspace
;-------------------------------------------------

L0070	= $70
L0071	= $71
L0072	= $72
L0073	= $73
L0074	= $74
L0075	= $75
L0076	= $76
L0077	= $77

L0C00	= $0c00
L0C01	= $0c01
L0C02	= $0c02
L0C03	= $0c03
L0C04	= $0c04
L0C05	= $0c05

L0C09	= $0c09		; Break flag (only one break)
L0C0A	= $0c0a		; Dead flag

L0C10	= $0c10		; X-pos player
L0C11	= $0c11		; Y-pos player
L0C12	= $0c12		; Jump pointer
L0C13	= $0c13		; Acceleration 
L0C14	= $0c14	
L0C15	= $0c15		; Current take
L0C16	= $0c16	
L0C17	= $0c17	
L0C18	= $0c18	

L0C20	= $0c20	

;---------------------------------------------------
; Sound parameter block
;---------------------------------------------------

L0C30	= $0c30		; Channel
L0C31	= $0c31	
L0C32	= $0c32		; Amplitude
L0C33	= $0c33	
L0C34	= $0c34		; Pitch
L0C35	= $0c35	
L0C36	= $0c36		; Duration
L0C37	= $0c37	

;---------------------------------------------------

L0C50	= $0c50

;---------------------------------------------------
; Scroller intro screen
;---------------------------------------------------

L0E00:      .byte "DAREDEVIL DENNIS ..."
            .byte "... WRITTEN BY SIMON"
            .byte " PICK .... VISIONS P"
            .byte "RESENTS ......... "
            .byte $0D

;---------------------------------------------------
; Score
;---------------------------------------------------

L0E5A:      .byte "000000",$0D		; Highest wages

;---------------------------------------------------
; Intro screen
;---------------------------------------------------

L0E63:      .byte "HOW GOOD ARE YOU",$0D

            .byte "NOVICE",$0D
            .byte "EXPERIENCED",$0D
            .byte "EXPERT",$0D
            .byte "OSCAR NOMINEE",$0D
            .byte "OSCAR WINNER",$0D
            .byte "ACE",$0D

            .byte "TO START GAME PRESS A TO F ",$0D
            .byte "HIGHEST WAGES PAID TODAY:",$0D

;---------------------------------------------------
; Level starting at choosen letter
;---------------------------------------------------

;                  A   B   C   D   E   F
L0EF0:      .byte $01,$07,$0D,$13,$19,$1F

;---------------------------------------------
; Program entry
;---------------------------------------------

; Init

L0EF6:
	lda #12		; Text MODE
	jsr LFFF4

	lda #0		; Clear cursor
	sta $e1
	lda #$20
	jsr LFFF4

; Display start screen

	ldx #8		; X=8
	ldy #4		; Y=4
	jsr printat

	lda #0
	sta ptr
L0F2B:
	ldx ptr
	lda L0E63,x	; Print "How good are you"
	cmp #$0D
	beq L0F39
	jsr LFFF4
	inc ptr
	jmp L0F2B

L0F39:
	inc ptr		; Set pointer

	lda #$41	; Choice="A"
	sta L0070
	lda #6		; Y=6
	sta L0071

; Print experience levels

L0F42:
	ldx #8		; Cursor at 8,y
        ldy L0071
	jsr printat

	lda #$20	; Print SPACE
	jsr LFFF4	
	lda L0070	; Print choice letter
	jsr LFFF4	
	lda #$29	; Print ")"
	jsr LFFF4	
	lda #$20	; Print SPACE
	jsr LFFF4	
L0F79:
	ldx ptr
	lda L0E63,x	; Print experience level
	cmp #$0D
	beq L0F87
	jsr LFFF4
	inc ptr
	jmp L0F79
L0F87:
	inc ptr		; Next string
	inc L0071	; Y=Y+1
	inc L0070	; Next choice letter
	lda L0070
	cmp #'G'	; Repeat until choiche="G"
	bne L0F42

	ldx #3		; Cursor at 3,13
        ldy #13
	jsr printat

	lda #$02	; Set stringcounter
	sta L0071
L0FA7:
	ldx ptr
	lda L0E63,X	; Print "To start game press A to F"
	cmp #$0D	; Print "Highest wages paid today"
	beq L0FB5
	jsr LFFF4
	inc ptr
	jmp L0FA7
L0FB5:
	ldx #3		; Cursor at 3,1
        ldy #1
	jsr printat

	inc ptr		; Next string
	dec L0071	; Last string?
	bne L0FA7	; No, repeat

; Check for highest wages and print it

	ldx #$00
L0FBC:
	lda L0E5A,X	; Get highest wages digit
	cmp L2F3A,X	; Compare with current wages digit
	beq L0FC9
	bcc L0FD3	; Current is higher than highest so swap wages
	jmp L0FE0	; Print highest wages
L0FC9:
	inx
	cpx #$06	; All wages digits tested?
	bne L0FBC	; No, repeat
	jmp L0FE0	; Print highest wages

; Swap current wages with highest wages

	ldx #$00
L0FD3:
	ldx #$00
L0FD5:
	lda L2F3A,X	; Get current wages digit
	sta L0E5A,X	; Set highest wages digit
	inx
	cpx #$06
	bne L0FD5

; Print highest wages on screen

L0FE0:
	ldx #13		; Cursor at 13,2
        ldy #2
	jsr printat

	lda #'$'	; Print $
	jsr LFFF4

	ldx #$00
	lda #$01
	sta L0071	; Set leading zero flag
L0FF0:
	lda L0E5A,X	; Get highest wages digit
	cmp #$30	; Is it "0"?
	bne L1004	; No, jump

	lda L0071
	cmp #$00	; If leading zero flag set, skip digit
	beq L1004
	cpx #$05	; If leading zero flag not set, last digit?
	beq L1004	; Yes, print digit
	jmp L100E
L1004:
	lda #$00
	sta L0071	; Reset leading zero flag
	lda L0E5A,X	; Get highest wages digit
	jsr LFFF4	; Print digit
L100E:
	inx		; Next digit
	cpx #$06	; Last one?
	bne L0FF0	; No, repeat

; Read key

	lda #$04
	sta L0077	; Set scroll delay
L1019:
	jsr LFE71	; Read key
	tya
	sec
	sbc #$21
	sta L0070	; Save choice, A=0,B=1....

	dec L0077	; Ready to scroll?
	bne L106F	; No, skip

	lda #$47	; Reset scroll delay
	sta L0077

; Display scroller

	ldx #1		; Cursor at 0,23
        ldy #15
	jsr printat

	ldx #$00	; Reset scroller pointer
L104B:
	lda L0E00,X	; Print scroller string
	jsr LFFF4
	inx
	cpx #30
	bne L104B

	lda L0E00
	sta L0074	; Save first character

; Advance scroller 1 character

	ldx #$00	; Rest scroll pointer
L105D:
	inx
	lda L0E00,X	; Get character+1
	dex
	sta L0E00,X	; Store at character
	inx
	cpx #$4D	; End of scroller?
	bne L105D	; No, repeat

	lda L0074
	sta L0E00,X	; Store first character at the end

; Check key input

L106F:
	lda L0070	; Get choice
	cmp #$06	; Check A-F
	bcc L1078	; Legal choice, exit routine
	jmp L1019	; Repeat keyscan
L1078:
	ldx #9		; Textcursur at 9,choice
	clc
	lda L0070
	adc #6		; Start Y choice
	tay
	jsr printat

	lda L0070 	; Get choice
	clc
	adc #97		; Convert to inverted character
	jsr LFFF4	; Print character

; Choice made, start game

	ldx L0070	; Use choice as pointer
	lda L0EF0,X	; Get starting level
	sta L2FE3	; Store level

	jsr L192C	; Play intro tune

; Reset current wages

	ldx #$00
	lda #$30	; "0"
L10B0:
	sta L2F3A,X	; Score
	inx
	cpx #$06
	bne L10B0

; Reset variables

	lda #$01	; Reset
	sta L181C	; Bonus bar counter

	lda #$FF	; Set bonus bar
	sta L181B	; No bar falling

	lda #$4C	; Reset
	sta L174B	; Helicopter X-pos
	lda #$CA	; Reset	
	sta L174C	; Helicopter Y-pos

	lda #$80	; Set down
	sta L166F	; Spider movement

	lda #$01	; Reset
	sta L1670	; Spider movement counter

	lda #$01	; Set
	sta L0C09	; Break flag

	jsr L14B9	; Start game

	jmp L0EF6	; Restart game

ptr:	.byte 0

;----------------------------------------------
; Check to drop bonus bar
;----------------------------------------------

L1122:      lda L181B              ; Get bonus bar flag
            cmp #$FF               ; Check if bar falling?
            beq L112C              ; If no bonus bar falling, jump
            jmp L11B1              ; Bonus bar is already falling

L112C:      lda L174B              ; Get Helicopter X-pos
            ldx L181C              ; Get Bonus bar counter
            cmp L2EB6,X            ; Check if bonus bar must be dropped
            beq L1138              ; Yes, jump
            rts                    ; 

L1138:      lda L174C              ; Get Helicopter Y-pos
            sta L181A              ; Store bonusbar Y-pos
            sta L2FE1              ; Temp Y-pos

            clc                    ; 
            lda L174B              ; Helicopter X-pos
            adc #$04               ; Move 4 pixels to the right
            sta L1819              ; Store bonusbar X-pos 
            sta L2FE0              ; Temp X-pos

; Adjust Bonusbar Y-pos

            clc                    ; 
            lda L2FE1              ; Temp Y-pos
            adc #$20               ; 
            sta L2FE1              ; Temp Y-pos

; Adjust Bonusbar X-pos

            clc                    ; 
            lda L2FE0              ; Temp X-pos
            adc #$02               ; 
            sta L2FE0              ; Temp X-pos

            jsr L2642              ; Calculate screenaddress

            ldy L2FE2              ; Boundery counter
            lda (L0070),Y          ; Get screenbyte
            cmp #$00               ; Check for platform
            bne L119C              ; Platform reached, jump

            lda L174C              ; Get Helicopter Y-pos
            cmp #$5F               ; If platform 2
            beq L119C              ; No bonus bar drop

            lda L174C              ; Get Helicopter Y-pos
            sta L181A              ; Store bonusbar Y-pos
            sta L2FE1              ; Temp Y-pos

            clc                    ; 
            lda L174B              ; Helicopter X-pos
            adc #$06               ; Move 6 pixels to the right
            sta L2FE0              ; Temp X-pos

            clc                    ; 
            lda L174C              ; Helicopter Y-pos
            adc #$21               ; Move 33 pixels up
            sta L2FE1              ; Temp Y-pos

            jsr L2642              ; Calculate screenaddress

            ldy L2FE2              ; Boundery counter
L1193:
            lda (L0070),Y          ; Get screenbyte
            cmp #$00               ; Check for platform
            beq L119C              ; No platform, fall
            jmp L11A2              ; Jump

L119C:      lda #$FF               ; Set bonus bar falling
            sta L181B              ; No bar falling
            rts                    ; 

L11A2:      lda L174B              ; Get Helicopter X-pos
            clc                    ; 
            adc #$04               ; Move 4 pixels right
            sta L1819              ; Store bonusbar X-pos 

            lda L174C              ; Get Helicopter Y-pos
            sta L181A              ; Store bonusbar X-pos

L11B1:      lda L1819              ; Get bonusbar X-pos
            sta L2FE0              ; Temp X-pos

            lda L181A              ; Get bonusbar Y-pos
            sta L2FE1              ; Temp Y-pos

            jsr L2642              ; Calculate screenaddress

            lda #<L1790            ; Select sprite Bar
            sta L0072              ; 
            lda #>L1790            ; 
            sta L0073              ; 

            jsr L137D              ; Sprite plot routine

            inc L181A              ; Move bar 1 pixel down
            inc L181B              ; Set bonus bar falling flag

            lda L181B              ; Get bonus bar flag
            cmp #$11               ; Check if bar on floor
            bne L11EC              ; No, skip

            lda #$FF               ; Set bonus bar flag
            sta L181B              ; No bar falling

            inc L181C              ; Next Bonus bar

            lda L181C              ; Get Bonus bar counter
            cmp #$28               ; Max reached?
            bne L11EC              ; No, jump

            lda #$01               ; Reset
            sta L181C              ; Bonus bar counter

L11EC:      rts                    ; 

;----------------------------------------------------
; Sprite plot routine
;
;
; L0070/71 = screen address
; L0072/73 = Sprite data address
; L0075/76 = Temp sprite data address
; L2FDC    = Byte right to sprite
; L2FDD    = row counter
; L2FE2    = boundary counter
;----------------------------------------------------

L137D:
	lda L0072	; Save sprite data address
	sta L0075
	sta L1392+1
	sta L1393+1
	lda L0073
	sta L0076
	sta L1392+2
	sta L1393+2

; Plot top 8 rows sprite

	ldx #$00	; Reset row counter
L1392:
	lda L275A,x	; Plot 4 pixels
	ldy sprpos,x
	sta (L0070),y
	inx		; Increment row counter
	cpx #16		; 16 rows plotted?
	bne L1392	; No, repeat

; Plot bottom 8 rows sprite

	inc L0071
L1393:
	lda L275A,x	; Plot 4 pixels
	ldy sprpos,x
	sta (L0070),y
	inx		; Increment row counter
	cpx #32		; 16 rows plotted?
	bne L1393	; No, repeat

; End plotting sprite

L13F3:
	lda L0075	; Restore sprite data address
	sta L0072
	lda L0076
	sta L0073

	ldy sprpos,x	; Check for collision
	lda (L0070),y
	sta L2FDC	; Save byte
	rts

sprpos:	.byte $00,$01,$20,$21,$40,$41,$60,$61
	.byte $80,$81,$a0,$a1,$c0,$c1,$e0,$e1
	.byte $00,$01,$20,$21,$40,$41,$60,$61
	.byte $80,$81,$a0,$a1,$c0,$c1,$e0,$e1
	.byte $e2

;-------------------------------------------------------------
; Data 
;-------------------------------------------------------------

L14A5:      .byte $14			; Restore default colours
            .byte $1F,$03,$11		; Textcursor at 3,17
            .byte $11,$08		; Colour 8
            .byte "YOU ARE FIRED",$0d	; Text

;-------------------------------------------------
; Start game
;-------------------------------------------------

L14B9:
	lda #$01	; Reset take
	sta L0C15

	lda #$FF
	sta L0C50	; ???

	lda #152	; Reset
	sta L174C	; Helicopter Y-pos
	lda #30		; Reset
	sta L174B	; Helicopter X-pos

	lda #$01	; Reset
	sta L181C	; Bonus bar counter

	lda #$FF	; Reset bonus bar falling flag
	sta L181B	; No bar falling

	lda #$80	; Reset
	sta L166F	; Spider movement to down

	lda #$01	; Reset
	sta L1670	; Spider movement counter

	jsr L20A2	; Display take screen

L14E4:
	jsr L2392	; Build screen

; Plot empty sprite in front of player start position

	lda #$02
	sta L2FE0 	; Temp X-pos
	lda #48
	sta L2FE1	; Temp Y-pos

	lda #<L275A	; Select sprite 0 (empty)
	sta L0072
	lda #>L275A
	sta L0073
	jsr L2642	; Calculate screenaddress
	jsr L137D	; Sprite plot routine

; Clear platform 2 and make holes if level > 5

	jsr L1F4A	; Clear platform 2 + ver.sprite pos
	jsr L1FB8	; Make holes in platform
;	jsr L190D	; Reprogram cursor (BBC)

L1508:	jsr L226A	; Define colour 8
	jsr L181D	; Move helicopter
	jsr L1671	; Move spider
;~~
	jsr $ffe3
	inc L2FE3
	jmp L14B9
	rts
;~~
            jsr L1BE4              ; 20 E4 1B

            lda L2FDC              ; Get byte right of sprite
            cmp #$1F               ; Is it a bonus bar?
            bne L151E              ; No, skip

            jsr L16C9              ; Add bonus 80 points

L151E:      clc                    ; 
            lda L0C10              ; X-pos player
            adc #$04               ; 
            sta L2FE0              ; Temp X-pos
            sec                    ; 

            ldx L0C12              ; Set jump pointer
            lda L0C11              ; Y-pos player
            sbc L2EED,X            ; Subtract jump table value
            sta L2FE1              ; Temp Y-pos
            clc                    ; 
            lda L2FE1              ; Temp Y-pos
            adc #$08               ; Move 8 pixels down
            sta L2FE1              ; Temp Y-pos
            jsr L2642              ; Calculate screenaddress

            ldy L2FE2              ; Boundery counter
            lda (L0070),Y          ; Get screenaddress
            cmp #$00               ; Empty?
            bne L1550              ; No, skip
            jsr L1B62              ; Check if hit anything
            cmp #$00               ; Nothing hit?
            beq L1508              ; Yes, jump

L1550:      jsr L1A04              ; Hit routine

            lda #$CF               ; Reset
            sta L174C              ; Helicopter Y-pos
            lda #$4C               ; Reset
            sta L174B              ; Helicopter X-pos

            lda #$FF               ; Reset Bonus bar falling flag
            sta L181B              ; No bar falling

            lda L0C15              ; Get take
            cmp #$04               ; Game over?
            beq L1574              ; Yes, jump

            jsr L20A2              ; Display take screen

            lda #$01               ; Set
            sta L0C09              ; Break flag
            jmp L14E4              ; Restart level

;----------------------------------------------------
; Print "You are fired"
;----------------------------------------------------

L1574:      ldx #$00               ; 
L1576:      lda L14A5,X            ; Print "You are fired"
            jsr LFFEE              ; 
            inx                    ; 
            cpx #$13               ; 
            bne L1576              ; 

            lda #$00               ; Reset counter
            sta L0C30              ; 
L1586:      jsr L226A              ; Define colour 8
            jsr L1671              ; Move spider
            inc L0C30              ; Increment pointer

            lda L0C30              ; Ger pointer
            cmp #$3C               ; check for end
            bne L1586              ; No, repeat
            rts                    ; 

;----------------------------------------------------
;
;----------------------------------------------------

L1597:
	sta L0C30	; Save reg A

;	lda #$80	; A9 80
;	jsr LFFEE	; 20 EE FF

;	jsr L190D	; Reprogram cursor

	lda #green	; Textcolour red
	jsr LFFEE

	lda L2FE3	; Get level
	cmp #$01
	beq L160F	; Jump if 1

	lda L2FED	; Get theme
	cmp #$01	; First theme?
	bne L160F	; No, skip

	lda L0C50	; AD 50 0C
	cmp L2FE3	; Compare level
	beq L160F	; Jump if equal

	lda L2FE3	; Get level
	sta L0C50	; 8D 50 0C

	lda L0C15	; Get take
	cmp #$01	; First take?
	beq L160F	; Yes, jump

	dec L0C15	; Decrement lives

; Set sound parameter block, snap takeshot

;            lda #$12               ; Channel
;            sta L0C30              ; 
;            lda #$00               ; 
;            sta L0C31              ; 
;            lda #$03               ; Amplitude
;            sta L0C32              ; 
;            lda #$00               ; 
;            sta L0C33              ; 
;            lda #$64               ; Pitch
;            sta L0C34              ; 
;            lda #$00               ; 
;            sta L0C35              ; 
;            lda #$1E               ; Duration
;            sta L0C36              ; 
;            lda #$00               ; 
;            sta L0C37              ; 

;            ldx #<L0C30            ; Play sound at L0C30
;            ldy #>L0C30            ; 
;            lda #$07               ; 
;            jsr LFFF1              ; 

; 44/50 Sec wait loop

	lda #$00
	sta L0070
L1602:
	jsr LFE66	; Wait for vertical sync
	inc L0070
	lda L0070
	cmp #52
	bne L1602
L160F:
	lda #$01	; Reset
	sta L1670	; Spider movement counter

	lda #$80	; Down
	sta L166F	; Spider movement

	lda L0C30	; Restore reg A
	rts

;-----------------------------------------------------
; Sprite Spider
;-----------------------------------------------------

L1629:
        .byte 1,0,1,0,1,0,1,0
        .byte 1,0,1,0,15,192,63,240
        .byte 255,252,215,92,231,156,255,252
        .byte 63,240,34,32,136,136,136,136
;            .byte $00,$05,$00,$00,$00,$05,$00,$00
;            .byte $00,$05,$00,$00,$00,$05,$00,$00
;            .byte $00,$05,$00,$00,$00,$03,$02,$00
;            .byte $01,$03,$03,$00,$03,$03,$03,$02
            .byte $07,$0B,$0F,$02,$47,$A1,$DA,$02
            .byte $03,$03,$03,$02,$01,$03,$03,$00
            .byte $14,$14,$14,$00,$28,$28,$28,$28
            .byte $28,$28,$28,$28,$00,$00,$00,$00

;-----------------------------------------------------
; Spider movement
;-----------------------------------------------------

L166F:	.byte $80	; Direction movement $80=down, $00=up
L1670:	.byte $0E	; Spider movement counter

L1671:	lda L2FE3	; Get level
	cmp #$04	; Spider only in level 1-3
	bcc L1679
	rts

L1679:	lda L166F	; Check direction spider
	cmp #$80	; Is it down?
	bne L1692	; No, jump
	inc L1670	; Move spider down
	lda L1670
	cmp #$1A	; Bottom position reached?
	bne L16A1	; No, jump
	lda #$00	; Yes, change direction to up
	sta L166F
	jmp L16A1	; Jump

L1692:	dec L1670	; Move spider up
	lda L1670
	cmp #$01	; Top position reached?
	bne L16A1	; No, jump
	lda #$80	; Yes, change direction to down
	sta L166F

L16A1:	lda #<L1629	; Select sprite (spider)
	sta L0072
	lda #>L1629
	sta L0073

	lda #16	; Initial X-pos spider
	sta L2FE0
	lda #146	; Initial Y-pos spider
	sta L2FE1

	clc		; Add movement to Y-pos
	lda L2FE1
	adc L1670
	sta L2FE1

	jsr L2642	; Calculate screenaddress
	jsr L137D	; Sprite plot routine
	rts

;-----------------------------------------------------
; Add bonus 80 points and display 80-bonus sprite
;-----------------------------------------------------

L16C9:      lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos

            sec                    ; 
            lda L0C11              ; Y-pos player
            sbc #$20               ; Locate 32 pixels up
            sta L2FE1              ; Temp Y-pos

            lda #<L17D5            ; Select sprite (80-bonus)
            sta L0072              ; 
            lda #>L17D5            ; 
            sta L0073              ; 

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

; Add 80 to current wages

            lda #$08               ; Repeat 8 times
            sta L0070              ; 
L16EA:      jsr L221A              ; Add 10 to current wages
            dec L0070              ; 
            bne L16EA              ; 

; Erase sprite

            lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            inc L2FE0              ; Temp X-pos
            inc L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos
            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

; Set sound parameter block

            lda #$12               ; Channel
            sta L0C30              ; 
            lda #$00               ; 
            sta L0C31              ; 
            lda #$02               ; Amplitude
            sta L0C32              ; 
            lda #$00               ; 
            sta L0C33              ; 
            lda #$C8               ; Pitch
            sta L0C34              ; 
            lda #$00               ; 
            sta L0C35              ; 
            lda #$03               ; Duration
            sta L0C36              ; 
            lda #$00               ; 
            sta L0C37              ; 

            ldx #<L0C30            ; Play sound at L0C30
            ldy #>L0C30            ; 

; Beep
            lda #$07               ; 
            jsr LFFF1              ; 

            rts                    ; 

;------------------------------------------------
; Sprite Helicopter
;------------------------------------------------

L174B:      .byte 30	; Helicopter X-pos
L174C:      .byte 152	; Helicopter Y-pos

L174D:
	.byte 1,64,170,170,1,64,15,240
	.byte 20,240,18,176,74,178,74,191
	.byte 74,191,202,253,255,240,63,192
	.byte 8,128,32,132,85,80,0,0
;            .byte $00,$15,$2A,$00,$49,$CD,$F1,$AC
;            .byte $00,$15,$2A,$00,$00,$03,$03,$00
;            .byte $15,$3E,$03,$00,$15,$38,$21,$00
;            .byte $3E,$30,$21,$15,$3E,$30,$21,$03
            .byte $3E,$30,$21,$03,$16,$30,$03,$47
            .byte $03,$03,$03,$00,$01,$03,$02,$00
            .byte $00,$08,$08,$00,$04,$00,$08,$0A
            .byte $0F,$0F,$0F,$00,$00,$00,$00,$00

;------------------------------------------------
; Sprite Bar
;------------------------------------------------

L1790:
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$1F,$1F,$00
            .byte $00,$1F,$1F,$00,$00,$1F,$1F,$00

;------------------------------------------------
; Sprite 80
;------------------------------------------------

L17D5:
            .byte $00,$00,$00,$00,$C0,$80,$C0,$80
            .byte $80,$80,$80,$80,$80,$80,$80,$80
            .byte $80,$80,$80,$80,$C0,$80,$80,$80
            .byte $80,$80,$80,$80,$80,$80,$80,$80
            .byte $80,$80,$80,$80,$C0,$80,$C0,$80
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00

;------------------------------------------------

L1819:      .byte $18		; Bonus bar X-pos
L181A:      .byte $97		; Bonus bar Y-pos
L181B:      .byte $FF		; Bonus bar falling flag
L181C:      .byte $01		; Bonus bar counter

;------------------------------------------------
; Helicopter movement
;------------------------------------------------

L181D:	lda L2FE3	; Get level
	cmp #$04	; Helicopter of in level>3
	bcs L1825	; Yes, jump
	rts		; No, return

L1825:      lda L174B              ; Helicopter X-pos
            sta L2FE0              ; Set X-pos
            lda L174C              ; Get Helicopter Y-pos
            sta L2FE1              ; Set Y-pos

            jsr L2642              ; Calculate screenaddress

            lda L0070              ; Save old position
            pha                    ; 
            lda L0071              ; 
            pha                    ; 
            lda L2FE2              ; 
            pha                    ; 

            dec L174B              ; Move helicopter to the right
            lda L174B              ; 
            cmp #$00               ; Left limit reached?
            bne L1868              ; No, jump

            lda #30               ; Reset
            sta L174B              ; Helicopter X-pos

            sec                    ; 
            lda L174C              ; Get Helicopter Y-pos
            sbc #40               ; Move platform up
            sta L174C              ; 

            lda L174C              ; Get Helicopter Y-pos
            cmp #8               ; Go up from upper platform?
            bne L1868              ; No, jump
            lda #152               ; Reset Y-pos helicopter
            sta L174C              ; 

            lda L2FE3              ; Get level
            sta L181C              ; Store Bonus bar counter

L1868:      lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            pla                    ; Restore old position
            sta L2FE2              ; 
            pla                    ; 
            sta L0071              ; 
            pla                    ; 
            sta L0070              ; 

            jsr L137D              ; Erase sprite

            lda L174B              ; Get Helicopter X-pos
            sta L2FE0              ; Set new X-pos
            lda L174C              ; Get Helicopter Y-pos
            sta L2FE1              ; Set new Y-pos

            lda #<L174D            ; Select sprite (helicopter)
            sta L0072              ; 
            lda #>L174D            ; 
            sta L0073              ; 

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

            jmp L1122              ; Check to drop bonus bar

;-----------------------------------------------
; Program 6845 with VDU23 command
;-----------------------------------------------

L190D:
	rts
            lda #$17               ; Reprogram 6845
            jsr LFFEE              ; 

            lda #$00               ; 
            jsr LFFEE              ; 

            lda #$0A               ; R10 = 32
            jsr LFFEE              ; 
            lda #$20               ; Blink frequency 1/32
            jsr LFFEE              ; 

            ldy #$08               ; Complete VDU23 sequence
L1923:      lda #$00               ; 
            jsr LFFEE              ; 
            dey                    ; 
            bne L1923              ;
            rts                    ; 

;-----------------------------------------------
; Play intro tune
;-----------------------------------------------

L192C:
;~~~
	jmp LFB81
;~~~

	lda #$12               ; Set counter
            sta L2FE4              ; 

            lda #<L2F68            ; Set table address
            sta L0070              ; 
            lda #>L2F68            ; 
            sta L0071              ; 

L1939:      ldy #$02               ; Get byte2 from table
            lda (L0070),Y          ; 
            sta L0072              ; 

            clc                    ; byte2=byte2 x 3
            lda L0072              ; 
            adc L0072              ; 
            sta L0072              ; 
            clc                    ; 
            lda L0072              ; 
            ldy #$02               ; 
            adc (L0070),Y          ; 
            sta L0072              ; Store

            ldy #$01               ; Get byte1 from table
            lda (L0070),Y          ; 
            cmp #$00               ; Zero?
            bne L1969              ; No, play sound

; 45/50 Sec wait loop

            lda #$2D               ; 
            sta L2FE5              ; 

L195C:      lda #$13               ; Wait for vertical sync
            jsr LFFF4              ; 
            dec L2FE5              ; 
            bne L195C              ; 

            jmp L19CF              ; Next entry

; Play sound with parameter block

L1969:      lda #$01               ; Channel
            sta L0C30              ; 
            lda #$00               ; 
            sta L0C31              ; 

            lda #$F1               ; Amplitude
            sta L0C32              ; 
            lda #$FF               ; 
            sta L0C33              ; 

            ldy #$00               ; Pitch
            lda (L0070),Y          ; 
            sta L0C34              ; 
            lda #$00               ; 
            sta L0C35              ; 

            lda L0072              ; Duration
            sta L0C36              ; 
            lda #$00               ; 
            sta L0C37              ; 

            ldx #<L0C30            ; Play sound at L0C30
            ldy #>L0C30            ; 
            lda #$07               ; 
            jsr LFFF1              ; 

; Point to next entry in table (+3)

L19CF:      clc                    ; 
            lda L0070              ; 
            adc #$03               ; 
            sta L0070              ; 
            lda L0071              ; 
            adc #$00               ; 
            sta L0071              ; 

            dec L2FE4              ; Decrement counter
            beq L19E4              ; Last item?
            jmp L1939              ; No, repeat

; 1 Sec wait loop

L19E4:      lda #$32               ; 
            sta L2FE5              ; 
L19E9:      lda #$13               ; Wait for vertical sync
            jsr LFFF4              ; 
            dec L2FE5              ; 
            bne L19E9              ; 
            rts                    ; 

;-----------------------------------------------------------
; Hit routine
;-----------------------------------------------------------

; Play hit sound with parameter block

L1A04:
            lda #$11               ; Channel
            sta L0C30              ; 
            lda #$00               ; 
            sta L0C31              ; 
            lda #$01               ; Amplitude
            sta L0C32              ; 
            lda #$00               ; 
            sta L0C33              ; 
            lda #$64               ; Pitch
            sta L0C34              ; 
            lda #$00               ; 
            sta L0C35              ; 
            lda #$10               ; Duration
            sta L0C36              ; 
            lda #$00               ; 
            sta L0C37              ; 

            ldx #<L0C30            ; Play sound at L0C30
            ldy #>L0C30            ; 
            lda #$07               ; 
            jsr LFFF1              ; 

            lda #$9A               ; 
            sta L0C00              ; 8D 00 0C
            lda #$00               ; A9 00
            sta L0C01              ; 8D 01 0C

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

; Display fire

            ldx L0C12              ; Get jump pointer
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,X            ; 
            sta L2FE1              ; Temp Y-pos

            lda #<L279A            ; Select sprite 1 (fire)
            sta L0072              ; 
            lda #>L279A            ; 
            sta L0073              ; 

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

; Let dead man fly

            clc                    ; 
            lda L0C10              ; X-pos player
            adc #$04               ; 
            sta L0C10              ; X-pos player

            lda #$12               ; Reset
            sta L0C12              ; Jump pointer

L1A73:      jsr L226A              ; Define colour 8

            lda #$13               ; Wait for vertical sync
            jsr LFFF4              ; 

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            ldx L0C12              ; Get jump pointer
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,X            ; FD ED 2E
            sta L2FE1              ; Temp Y-pos

            lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            jsr L2642              ; Calculate screenaddress

            lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            jsr L137D              ; Sprite plot routine

            inc L0C10              ; X-pos player
            lda L0C10              ; X-pos player
            cmp #$4A               ; C9 4A
            bne L1AB8              ; D0 05
            lda #$49               ; A9 49
            sta L0C10              ; X-pos player
L1AB8:      inc L0C12              ; Increment jump pointer
            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos
            ldx L0C12              ; Get jump pointer
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,X            ; FD ED 2E
            sta L2FE1              ; Temp Y-pos

            lda L0C00              ; AD 00 0C
            sta L0072              ; 85 72
L1AD8:      lda L0C01              ; AD 01 0C
            sta L0073              ; 85 73

            jsr L2642              ; Calculate screenaddress

            lda #<L289A            ; Select sprite 5 (man dead)
            sta L0072              ; 
            lda #>L289A            ; 
            sta L0073              ; 

            jsr L137D              ; Sprite plot routine
            lda L0C12              ; Get jump pointer
            cmp #$20               ; Pointer at end?
            beq L1AF5              ; Yes, skip
            jmp L1A73              ; Repeat

; Display dead man wiggling 7 times

L1AF5:      lda #$07               ; Reset
            sta L2FE4              ; Counter

L1AFA:      lda #<L289A            ; Select sprite 5 (man dead)
            sta L0072              ; 
            lda #>L289A            ; 
            sta L0073              ; 

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos
            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

; 1/5 Sec wait loop

            lda #$0A               ; 
            sta L2FE6              ; 
L1B19:      lda #$13               ; Wait for vertical sync
            jsr LFFF4              ; 
            dec L2FE6              ; 
            bne L1B19              ;

; Display man dead1

            lda #<L285A            ; Select sprite 4 (man dead1)
            sta L0072              ; 
            lda #>L285A            ; 
            sta L0073              ; 

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos
            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

; 1/5 Sec wait loop

            lda #$0A               ; 
            sta L2FE6              ; 
L1B42:      lda #$13               ; Wait for vertical sync
            jsr LFFF4              ; 
            dec L2FE6              ; 
            bne L1B42              ; 

            dec L2FE4              ; Decrement counter
            bne L1AFA              ; If not zero, repeat

            inc L0C15              ; Increment take
            rts                    ; 

;-------------------------------------------------------------
; Check if hit anything
;-------------------------------------------------------------

L1B62:      lda #$00               ; Reset
            sta L0C0A              ; Dead flag

            lda L2FDC              ; Get byte right of sprite
            cmp #$00               ; Empty?
            beq L1B74              ; Yes, nothing hit, skip

            lda #$01               ; Set
            sta L0C0A              ; Dead flag
            rts                    ; 

L1B74:      lda L0C10              ; X-pos player
            clc                    ; ???
            adc #$01               ; ???
            clc                    ; ???
            adc #$01               ; ???

            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            ldx L0C12              ; Get jump pointer

            sec                    ; 
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,X            ; Subtract jump table value
            sta L2FE1              ; Temp Y-pos
            dec L2FE1              ; Get temp Y-pos
            jsr L2642              ; Calculate screenaddress

            ldy L2FE2              ; Boundery counter
            lda (L0070),Y          ; Get screenbyte
            cmp #$00               ; Hit anything?
            beq L1BA5              ; No, skip

            lda #$01               ; Hit, set
            sta L0C0A              ; Dead flag

            rts                    ; 

L1BA5:      lda L0C12              ; Get jump pointer
            cmp #$00               ; Jumping?
            bne L1BAD              ; Yes, jump
            rts                    ; 

L1BAD:      lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            ldx L0C12              ; Get jump pointer
            sec                    ; 
            lda L2FE1              ; Get temp Y-pos
            sbc L2EED,X            ; Subtract jump table value
            sta L2FE1              ; Temp Y-pos

            clc                    ; 
            lda L2FE1              ; Temp Y-pos
            adc #$10               ; Point 16 pixels down
            sta L2FE1              ; Temp Y-pos

            jsr L2642              ; Calculate screenaddress

            ldy L2FE2              ; Boundery counter
            lda (L0070),Y          ; Get screenbyte
            cmp #$00               ; Hit anything
            beq L1BE1              ; No, skip

            lda #$01               ; Hit, set
            sta L0C0A              ; Dead flag

            rts                    ; 

L1BE1:      rts                    ; 

;----------------------------------------------
;----------------------------------------------

L1BE4:      jsr L1DEC              ; 20 EC 1D

            clc                    ; 18
            lda L0C14              ; AD 14 0C
            adc L0C13              ; Add acceleration
            sta L0C14              ; 8D 14 0C

            lda L0C10              ; X-pos player
            adc #$00               ; 
            sta L0C10              ; X-pos player

            clc                    ; 18
            lda L0C14              ; AD 14 0C
            adc L0C13              ; Add acceleration
            sta L0C14              ; 8D 14 0C

            lda L0C10              ; X-pos player
            adc #$00               ; 
            sta L0C10              ; X-pos player

            clc                    ; 18
            lda L0C14              ; AD 14 0C
            adc L0C13              ; Add acceleration
            sta L0C14              ; 8D 14 0C

            lda L0C10              ; X-pos player
            adc #$00               ; 
            sta L0C10              ; X-pos player

; Play motor noise with parameter block

            lda #$11               ; Channel
            sta L0C30              ; 
            lda #$00               ; 
            sta L0C31              ; 
            lda #$04               ; Amplitude
            sta L0C32              ; 
            lda #$00               ; 
            sta L0C33              ; 
            lda L0C13              ; Pitch
            sta L0C34              ; 
            lsr L0C34              ; 
            lda #$00               ; 
            sta L0C35              ; 
            lda #$03               ; Duration
            sta L0C36              ; 
            lda #$03               ; 
            sta L0C37              ; 

            ldx #<L0C30            ; Play sound at L0C30
            ldy #>L0C30            ; 

; Beep
            lda #$07               ; 
            jsr LFFF1              ; 

; BBC code
; Sound channel

            lda #$03               ; A9 03
            sta L0C34              ; 8D 34 0C
            lda L0C01              ; AD 01 0C
            cmp #$2B               ; C9 2B
            beq L1C92              ; F0 34
            lda #$11               ; A9 11
            sta L0C30              ; 8D 30 0C
            lda #$00               ; A9 00
            sta L0C31              ; 8D 31 0C
            lda #$F1               ; A9 F1
            sta L0C32              ; 8D 32 0C
            lda #$FF               ; A9 FF
            sta L0C33              ; 8D 33 0C
            lda #$03               ; A9 03
            sta L0C34              ; 8D 34 0C
            lda #$00               ; A9 00
            sta L0C35              ; 8D 35 0C
            lda #$04               ; A9 04
            sta L0C36              ; 8D 36 0C
            lda #$00               ; A9 00
            sta L0C37              ; 8D 37 0C
            ldx #$30               ; A2 30
            ldy #$0C               ; A0 0C
            lda #$07               ; A9 07
            nop                    ; EA
            nop                    ; EA
            nop                    ; EA

            jmp L1CD1              ; Update player movement

; BBC code
; Sound channel

L1C92:      lda L0C12              ; Get jump pointer
            cmp #$00               ; C9 00
            bne L1CD1              ; D0 38

            lda L0C13              ; Get acceleration
            cmp #$00               ; C9 00
            beq L1CD1              ; F0 31

            lda #$10               ; A9 10
            sta L0C30              ; 8D 30 0C
            lda #$00               ; A9 00
            sta L0C31              ; 8D 31 0C
            lda #$F9               ; A9 F9
            sta L0C32              ; 8D 32 0C
            lda #$FF               ; A9 FF
            sta L0C33              ; 8D 33 0C
            lda #$07               ; A9 07
            sta L0C34              ; 8D 34 0C
            lda #$00               ; A9 00
            sta L0C35              ; 8D 35 0C
            lda #$02               ; A9 02
            sta L0C36              ; 8D 36 0C
            lda #$00               ; A9 00
            sta L0C37              ; 8D 37 0C
            ldx #$30               ; A2 30
            ldy #$0C               ; A0 0C
            lda #$07               ; A9 07
            nop                    ; EA
            nop                    ; EA
            nop                    ; EA

; Update player movements

L1CD1:      ldx #$FF               ; Read key
            ldy #$FF               ; 
            lda #$81               ; 
            jsr LFFF4              ; 
            cpx #$00               ; RETURN pressed?
            beq L1CF3              ; Skip if not

            inc L0C13              ; Increment acceleration
            inc L0C13              ; Increment acceleration
            inc L0C13              ; Increment acceleration
            lda L0C13              ; Get acceleration
            cmp #$FA               ; Check max
            bcc L1CF3              ; Not max, so skip
            lda #$FA               ; Set max
            sta L0C13              ; Acceleration

; Check if player reached right border

L1CF3:      lda L0C10              ; X-pos player
            cmp #$4D               ; Check right border+2
            beq L1D0B              ; Jump if reached

            lda L0C10              ; X-pos player
            cmp #$4C               ; Check right border+1
            beq L1D0B              ; Jump if reached

            lda L0C10              ; X-pos player
            cmp #$4B               ; Check right border
            beq L1D0B              ; Jump if reached

            jmp L1D46              ; Not reached right border, skip

; Right border reached

L1D0B:      ldy L2FE3              ; Get level

; Add level*10 to Wages if going to next platform

L1D0E:      jsr L221A              ; Add 10 to current wages
            dey                    ; 
            bne L1D0E              ; Repeat

            clc                    ; 
            lda L0C11              ; Y-pos player
            adc #$38               ; Platform down
            sta L0C11              ; Y-pos player

            lda #$00               ; Reset
            sta L0C10              ; X-pos player

            lda L0C11              ; Y-pos player
            cmp #$18               ; Last platform?
            bne L1D46              ; No, skip

            inc L2FE3              ; Increment level
            lda L2FE3              ; Get level
            cmp #$24               ; Last level?
            bne L1D38              ; No, skip

            lda #$21               ; Set 33
            sta L2FE3              ; Set Level

L1D38:      jsr L2392              ; Build screen
            jsr L1FB8              ; Make holes in platforms
            jsr L1F4A              ; Clear platform 2

            lda #$01               ; Set
            sta L0C09              ; Break flag

; Handle jump

L1D46:      ldx #$9D               ; Read key
            ldy #$FF               ; 
            lda #$81               ; 
            jsr LFFF4              ; 
            cpx #$00               ; SPACE pressed?
            beq L1D5F              ; Skip if not

            lda L0C12              ; Get jump pointer
            cmp #$00               ; Already jumping?
            bne L1D5F              ; Yes, skip

            lda #$01               ; Set
            sta L0C12              ; Jump pointer

L1D5F:      lda L0C12              ; Get jump pointer
            cmp #$00               ; Already jumping?
            beq L1D75              ; No, skip

            inc L0C12              ; Increment jump pointer
            lda L0C12              ; Get jump pointer
            cmp #$1E               ; Check max?
            bne L1D75              ; Not max, jump

            lda #$00               ; Max reached, reset
            sta L0C12              ; Jump pointer

;

L1D75:      jsr L1E19              ; 20 19 1E

            lda L0C12              ; Get jump pointer
            cmp #$00               ; Already jumping
            bne L1DBA              ; Yes, skip

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            clc                    ; Add 16 to Y-pos
            lda L2FE1              ; 
            adc #$10               ;
            sta L2FE1              ; 

            ldx L0C12              ; Subtract jump table value from Y-pos
            lda L2FE1              ; 
            sec                    ; 
            sbc L2EED,X            ; 
            sta L2FE1              ; 

            jsr L2642              ; Calculate screenaddress

; Check for hole in the platform

            ldy L2FE2              ; Boundery counter
            lda (L0070),Y          ; Get screenbyte
            cmp #$00               ; Empty?
            bne L1DBA              ; No, jump

            jsr L1E72              ; Plot player + balloon

            lda #$00               ; Reset
            sta L0C13              ; Acceleration
            lda #$00               ; Reset
            sta L0C12              ; Jump pointer

; Handle break, may be used once in screen

L1DBA:      ldx #$B6               ; Read key
            ldy #$FF               ; 
            lda #$81               ; 
            jsr LFFF4              ; 
            cpx #$00               ; RETURN pressed?
            beq L1DD8              ; Skip if not

            lda L0C09              ; Get break flag
            cmp #$01               ; Not used yet?
            bne L1DD8              ; If used, skip

            lda #$00               ; Reset
            sta L0C09              ; Break flag

            lda #$00               ; Reset
            sta L0C13              ; Acceleration

L1DD8:      rts                    ; 

;-------------------------------------------------------
;-------------------------------------------------------

L1DEC:      lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            ldx L0C12              ; Get jump pointer
            sec                    ; 
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,X            ; Subtract jump table value
            sta L2FE1              ; Temp Y-pos

            jsr L2642              ; Calculate screenaddress

            lda L0070              ; A5 70
            sta L0C16              ; 8D 16 0C
            lda L0071              ; A5 71
            sta L0C17              ; 8D 17 0C

            lda L2FE2              ; Boundery counter
            sta L0C18              ; 8D 18 0C

            rts                    ; 

;-------------------------------------------------------
;-------------------------------------------------------

L1E19:      lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            ldx L0C12              ; Get jump pointer 
            sec                    ; 
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,X            ; Subtract jump table value
            sta L2FE1              ; Temp Y-pos

            lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            lda #$13               ; Wait for vertical sync
            jsr LFFF4              ; 

            lda L0C16              ; AD 16 0C
            sta L0070              ; 85 70
            lda L0C17              ; AD 17 0C
            sta L0071              ; 85 71

            lda L0C18              ; AD 18 0C
            sta L2FE2              ; Boundery counter

            jsr L137D              ; Sprite plot routine

            jsr L2642              ; Calculate screenaddress

            lda L0C00              ; AD 00 0C
            sta L0072              ; 85 72
            lda L0C01              ; AD 01 0C
            sta L0073              ; 85 73

            jsr L137D              ; Sprite plot routine

            rts                    ; 

;-------------------------------------------------------
; Plot player + balloon when falling trough platform hole
;-------------------------------------------------------

L1E72:      cld                    ; ???
            .byte $2F,$C9,$00      ; rla L00C9 ???
            beq L1E79              ; F0 01
            rts                    ; 60

L1E79:      lda L2FDB              ; AD DB 2F
            cmp #$00               ; C9 00
            beq L1E81              ; F0 01
            rts                    ; 60

L1E81:      lda #$0F               ; Flush buffer
            jsr LFFF4              ; 

            lda #$38               ; A9 38
            sta L2FE4              ; 8D E4 2F

L1E8B:      lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            lda #$13               ; Wait for vertical sync
            jsr LFFF4              ; 

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

            inc L0C11              ; Y-pos player

            lda L0C00              ; AD 00 0C
            sta L0072              ; 85 72
            lda L0C01              ; AD 01 0C
            sta L0073              ; 85 73

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

            lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            sec                    ; 
            lda L2FE1              ; Temp Y-pos
            sbc #$12               ; Move 18 pixels up
            sta L2FE1              ; Temp Y-pos

            lda #$01               ; Read/write user flag
            jsr LFFF4              ; 

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            sec                    ; 
            lda L2FE1              ; Temp Y-pos
            sbc #$12               ; Move 18 pixels up
            sta L2FE1              ; Temp Y-pos
            inc L2FE1              ; Move 1 pixel down

            lda #<L27DA            ; Select sprite 2 (balloon)
            sta L0072              ; 
            lda #>L27DA            ; 
            sta L0073              ; 

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

            dec L2FE4              ; CE E4 2F
            beq L1F1F              ; F0 03
            jmp L1E8B              ; 4C 8B 1E

L1F1F:      lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            lda L0C10              ; X-pos player
            sta L2FE0              ; Temp X-pos
            lda L0C11              ; Y-pos player
            sta L2FE1              ; Temp Y-pos

            sec                    ; 38
            lda L2FE1              ; Temp Y-pos
            sbc #$12               ; Move 18 pixels up
            sta L2FE1              ; Temp Y-pos

            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

            rts                    ; 60

;----------------------------------------------------
; Clear platform 2 + ver.sprite pos
;----------------------------------------------------

; Clear platform 2

L1F4A:	lda #$00
	sta L2FE4	; X-pos = 0
	lda #88
	sta L2FE5	; Y-pos = 88

L1F54:
	lda #<L275A	; Select sprite 0 (empty)
	sta L0072
	lda #>L275A
	sta L0073

	lda L2FE4
	sta L2FE0	; Temp X-pos
	lda L2FE5
	sta L2FE1	; Temp Y-pos

	jsr L2642	; Calculate screenaddress
	jsr L137D	; Sprite plot routine

	clc	
	lda L2FE4	; Get X-pos
	adc #$02	; Add 2 to X-pos
	sta L2FE4
	lda L2FE4	; Get X-pos
	cmp #32		; Right limit?
	bne L1F54	; No, repeat

;	ldx #$00               ; Reset pointer????????

; Clear ver.sprite pos

	ldx #$00	; Reset pointer
	ldy L2FA6	; Get counter
	cpy #$00	; Counter=0?
	beq L1FAD	; If yes, end

L1F89:	lda L2FA7,X	; Get x value
	sta L2FE0	; Temp X-pos
	lda L2FA8,X	; Get y-value
	sta L2FE1	; Temp Y-pos

	stx L2FE4	; Save X
	sty L2FE5	; Save Y

	jsr L2642	; Calculate screenaddress
	jsr L137D	; Sprite plot routine

	ldx L2FE4	; Restore X
	ldy L2FE5	; Restore Y

	inx		; Point to next entry (+3)
	inx
	inx

	dey		; Decrement counter
	bne L1F89	; If not zero, repeat

L1FAD:	rts

;----------------------------------------------------
; Make holes in platform
; Holes start at level 5 and are incremented every
; 4 levels:
;
;     Levels        Holes/L2FE4
;  -  1, 2, 3, 4 -> 0 
;  -  5, 6, 7, 8 -> 1
;  -  9,10,11,12 -> 2
;  - 13,14,15,16 -> 3
;  - 17,18,19,20 -> 4
;  - 21,22,23,24 -> 5
;  - 25,26,27,28 -> 6
;  - 29,30,31,32 -> 7
;  - 33,34,35,36 -> 8
;----------------------------------------------------

L1FB8:	lda L2FE3	; Get level
	cmp #$05	; Greater than 5?
	bcs L1FC0	; Yes, jump
	rts 

; Determine nr of holes in L2FE4

L1FC0:	sec
	lda L2FE3	; Get level
	sbc #$01	; Subtract 1 (starts at 1)
	sta L2FE4	; Save pointer
	lsr L2FE4	; Divide by 4
	lsr L2FE4
;	lda L2FA6	; Get counter

;

	ldx L2FE4	; Get nr of holes
	ldy #$01	; Set index
L1FD7:	lda L2FE4,Y	; Get ??
	iny
	cmp #40		; Check ??
	bcc L1FD7	; If smaller repeat

; Set Y-pos hole

	lda #48		; Platform 1
	sta L2FE5	; Y-pos

	lda L2FE4,Y            ; B9 E4 2F
	sta L2FE6              ; 8D E6 2F
;	ldx L2FE4              ; Set counter

	ldy #$01               ; A0 01
L1FEF:	ldx L2FE6              ; AE E6 2F
	lda L2EB6,X            ; BD B6 2E
	sta L2FE7              ; 8D E7 2F
	inc L2FE6              ; EE E6 2F
	lda L2FE6              ; AD E6 2F
	cmp #40               ; C9 28
	bne L2007              ; D0 05

            lda #$01               ; A9 01
            sta L2FE6              ; 8D E6 2F

L2007:      ldy L2FA6              ; AC A6 2F
            ldx #$00               ; A2 00

            lda L2FA6              ; AD A6 2F
            cmp #$00               ; C9 00
            beq L202C              ; F0 19

; Check if hole at static enemy pos

L2013:      lda L2FA7,X            ; BD A7 2F
            cmp L2FE7              ; Compare Y-pos
            beq L1FEF              ; F0 D4

            lda L2FE7              ; AD E7 2F
            clc                    ; 18
            adc #$04               ; 69 04
            cmp L2FA7,X            ; Compare X-pos
            beq L1FEF              ; F0 C9

; Point to next item

            inx
            inx
            inx

            dey
            bne L2013

; Select empty sprite

L202C:	lda #<L275A 	; Select sprite 0 (empty)
	sta L0072
	lda #>L275A
	sta L0073

; Print 2 empty sprites as hole

	lda #$02	; Set counter
	sta L2FE8

; Plot sprite 1

L2039:	lda L2FE7
	sta L2FE0	; Temp X-pos
	lda L2FE5
	sta L2FE1	; Temp Y-pos
	jsr L2642	; Calculate screenaddress
	jsr L137D	; Sprite plot routine

; Plot sprite 2 below sprite 1

	clc		; Add 16 to Y-pos
	lda L2FE5
	adc #$10
	sta L2FE5

	lda L2FE7
	sta L2FE0	; Temp X-pos
	lda L2FE5
	sta L2FE1	; Temp Y-pos

	jsr L2642	; Calculate screenaddress
 	jsr L137D	; Sprite plot routine

	clc		; One sprite to the right
	lda L2FE7
	adc #$02
	sta L2FE7

	sec		; One sprite below
	lda L2FE5
	sbc #$10
	sta L2FE5

	dec L2FE8	; Decrement counter
	bne L2039	; If not zero, repeat

; Point down 1 platform

 	clc	
	lda L2FE5	; Get Y-pos
	adc #80		; Add 40 pixels
	sta L2FE5
	lda L2FE5
	cmp #$d0	; Check max Y-pos
	bne L2092	; If not max, jump
	lda #48		; Reset Y-pos to platform 1
	sta L2FE5

L2092:	dec L2FE4	; Decrement counter
	beq L209A	; If zero, end
	jmp L1FEF	; Repeat

L209A:	rts

;-----------------------------------------------------
; Display take screen
;-----------------------------------------------------

L20A2:
	lda #$d0	; Select CLEAR4 colour
	sta $b000
	lda #blue	; Clear screen in colour A
	jsr cls

	lda #yellow
	jsr setfgcol
	lda #blue
	jsr setbgcol

	lda #9		; Textcursor at 13,12
	sta dispx
	lda #12
	sta dispy

	lda #$54	; Print "TAKE "+ chr(5)
	jsr pchar
	lda #$41
	jsr pchar
	lda #$4B
	jsr pchar
	lda #$45
	jsr pchar
	lda #$20
	jsr pchar

; Calculate sprite address for take1/2/3 sprite

	lda #<L2A1A	; Select sprite 11 (skull)
	sta L0072
	lda #>L2A1A
	sta L0073

; Calculate right take-sprite address ( 1, 2 or 3)
; Address = ( address(skull) - take * 64)

	ldx #$00	; Reset counter
L20ED:
	sec
	lda L0072	; Get lb address
	sbc L0C15	; Subtract current take
	sta L0072
	lda L0073
	sbc #$00
	sta L0073

	inx		; Next byte
	cpx #$40	; All 64 done?
	bne L20ED	; No, repeat

; Plot take sprite

	lda #18		; Set X-pos
	sta L2FE0
	lda #88		; Set Y-pos
	sta L2FE1
	jsr L2642	; Calculate screenaddress
	jsr L137D	; Sprite plot routine

; 1 Sec wait loop

	lda #60		; Set timer = 1 sec
	sta L0070
L2114:
	jsr LFE66 	; Wait for vertical sync
	dec L0070
	bne L2114

; Plot take closed sprite

	lda #18
	sta L2FE0	; Temp X-pos
	lda #80
	sta L2FE1	; Temp Y-pos

	lda #<L291A            ; Select sprite 7 (take closed)
	sta L0072              ; 
	lda #>L291A            ; 
	sta L0073              ; 

	jsr L2642              ; Calculate screenaddress
	jsr L137D              ; Sprite plot routine

; Play CLACK sound with parameter block

            lda #$00               ; Channel
            sta L2FDA              ; 
            lda #$00               ; 
            sta L2FDB              ; 
            lda #$F1               ; Amplitude
            sta L2FDC              ; 
            lda #$FF               ; 
            sta L2FDD              ; 
            lda #$07               ; Pitch
            sta L2FDE              ; 
            lda #$00               ; 
            sta L2FDF              ; 
            lda #$01               ; Duration
            sta L2FE0              ; 
            lda #$00               ; 
            sta L2FE1              ; 

            ldx #<L2FDA            ; Play sound at L2FDA
            ldy #>L2FDA            ; 
            lda #$07               ; 
;            jsr LFFF1              ; 

; 1/2 Sec wait loop

	lda #30		; Set timer 0,5 sec
	sta L0070
L216A:
	jsr LFE66	; Wait for vertical sync
	dec L0070
	bne L216A

	lda #$d0	; Select CLEAR4 colour
	sta $b000
	lda #green	; Clear screen in colour A
	jsr cls

	lda #$00	; Reset
	sta L0C12	; Jump pointer

	lda #$00	; Reset
	sta L0C13	; Acceleration

	lda #$00	; Reset
	sta L0C14	; ???

	rts

;-------------------------------------------------
; Add 10 to current wages
;-------------------------------------------------

L221A:      ldx #$04               ; Point to ten digit
L221C:      inc L2F3A,X            ; Increment
            lda L2F3A,X            ; Get digit
            cmp #$3A               ; Digit > 9?
            bne L2230              ; No, skip

            lda #$30               ; Set '0'
            sta L2F3A,X            ; Reset digit
            dex                    ; Point to previous digit
            cmp #$FF               ; ??? BUG ??? -> cpx ???
            bne L221C              ; Check next digit

L2230:      lda #$1F               ; Textcursor at 7,1
            jsr LFFEE              ; 
            lda #$07               ; 
            jsr LFFEE              ; 
            lda #$01               ; 
            jsr LFFEE              ;

            lda #$11               ; Textcolour yellow foreground
            jsr LFFEE              ; 
            lda #$03               ; 
            jsr LFFEE              ; 

            lda #$11               ; Textcolour blue background
            jsr LFFEE              ; 
            lda #$84               ; 
            jsr LFFEE              ; 

; Print current wages

            ldx #$00               ; 
L2255:      lda L2F3A,X            ; 
            jsr LFFEE              ; 
            inx                    ; 
            cpx #$06               ; 
            bne L2255              ; 

            rts                    ; 

;-------------------------------------------------
; Define colour 8 
;-------------------------------------------------

L226A:
            ldx L0C20              ; Get counter

; Redefine colour 8

;            lda #$13               ; Redefine colour
;            jsr LFFEE              ; 
;            lda #$08               ; Colour 8
;            jsr LFFEE              ; 
;            lda L2EDE,X            ; Get colour from table
;            jsr LFFEE              ; Set colour
;            lda #$00               ; 
;            jsr LFFEE              ; 
;            lda #$00               ; 
;            jsr LFFEE              ; 
;            lda #$00               ; 
;            jsr LFFEE              ; 
;            inc L0C20              ; 
;            lda L0C20              ; 
;            cmp #$0E               ; End of colour table?
;            bcc L229B              ; No, jump
;            lda #$00               ; Reset colour pointer
;            sta L0C20              ; 

L229B:
            lda L2FA5              ; AD A5 2F
            cmp #$00               ; C9 00
            bne L22A3              ; D0 01
            rts                    ; 60

L22A3:
;            lda #$01               ; Read/write user flag
;            jsr LFFF4              ; 

            lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            lda L2FA4              ; AD A4 2F
            sta L2FE0              ; Temp X-pos
            lda L2FA5              ; AD A5 2F
            sta L2FE1              ; Temp Y-pos
            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

            inc L2FA4              ; EE A4 2F
            lda L2FA4              ; AD A4 2F
            cmp #$4B               ; C9 4B
            bne L22D1              ; D0 05
            lda #$00               ; A9 00
            sta L2FA4              ; 8D A4 2F

L22D1:      lda L0C04              ; Select sprite ???
            sta L0072              ; 
            lda L0C05              ; 
            sta L0073              ; 

            lda L2FA4              ; AD A4 2F
            sta L2FE0              ; Temp X-pos
            lda L2FA5              ; AD A5 2F
            sta L2FE1              ; Temp Y-pos
            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine

;            lda #$01               ; Read/write user flag
;            jsr LFFF4              ; 

            jsr L22FF              ; Move moving sprite
            rts                    ; 

;-------------------------------------------------
; Moving sprite
;-------------------------------------------------

L22FF:      ldx #$00               ; A2 00
            lda L2FA6              ; AD A6 2F
            sta L2FEA              ; 8D EA 2F

            lda L2FA6              ; AD A6 2F
            cmp #$00               ; C9 00
            bne L230F              ; D0 01
            rts                    ; 60

L230F:      lda L2FA7,X            ; Vert.X-pos
            sta L2FE0              ; Set X-pos
            lda L2FA8,X            ; Vert.Y-pos
            sta L2FE1              ; Set Y-pos

; Adjust Y-pos

            ldy L2FA9,X            ; Vertical counter
            sec
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,Y            ; Delta Y
            sta L2FE1              ; Temp Y-pos
            stx L2FE4              ; 8E E4 2F
            jsr L2642              ; Calculate screenaddress

            lda #<L275A            ; Select sprite 0 (empty)
            sta L0072              ; 
            lda #>L275A            ; 
            sta L0073              ; 

            jsr L137D              ; Sprite plot routine
            ldx L2FE4              ; AE E4 2F
            inc L2FA9,X            ; Vertical counter
            lda L2FA9,X            ; Vertical counter
            cmp #$20               ; C9 20
            bne L234B              ; D0 05
            lda #$01               ; A9 01
            sta L2FA9,X            ; Vertical counter

L234B:      lda L0C02              ; Select sprite 24 (moving sprite)
            sta L0072              ; 
            lda L0C03              ; 
            sta L0073              ; 

            lda L2FA7,X            ; BD A7 2F
            sta L2FE0              ; Temp X-pos
            lda L2FA8,X            ; BD A8 2F
            sta L2FE1              ; Temp Y-pos
            ldy L2FA9,X            ; Vertical counter
            sec                    ; 38
            lda L2FE1              ; Temp Y-pos
            sbc L2EED,Y            ; F9 ED 2E
            sta L2FE1              ; Temp Y-pos
            stx L2FE4              ; 8E E4 2F
            jsr L2642              ; Calculate screenaddress
            jsr L137D              ; Sprite plot routine
            ldx L2FE4              ; AE E4 2F
            inx                    ; E8
            inx                    ; E8
            inx                    ; E8
            cpx #$0F               ; E0 0F
            bne L2382              ; D0 01
            rts                    ; 60
L2382:      dec L2FEA              ; CE EA 2F
            bne L230F              ; D0 88
            rts                    ; 60

;-----------------------------------------------
; Build screen
;-----------------------------------------------

cnt:	.byte 0

L2392:
	lda #$d0	; Select CLEAR4 colour
	sta $b000
	lda #green	; Clear screen in colour A
	jsr cls

; Set vars

	lda #$00	; Reset
	sta L0C10	; X-pos player
	lda #$38	; Reset
	sta L0C11	; Y-pos player

	lda #$00	; Reset
	sta L0C12	; Jump pointer

	lda #$00	; Reset
	sta L0C13	; Acceleration

	lda #$00	; A9 00
	sta L0C14	; 8D 14 0C

	lda #blue	; Textcolour blue on blue
	jsr setfgcol
	lda #blue
	jsr setbgcol

; Print blue top banner

	lda #$00
	sta dispx
	lda #$00
	sta dispy
	lda #48
	sta cnt
	jsr printat
L23C3:
	lda #$20
	jsr pchar	; Print SPACE
	dec cnt
	bne L23C3

	lda #green	; Textcolour green
	jsr setfgcol

	lda #1		; Textcursor at 1,1
	sta dispx
	sta dispy

	lda #$57	; Print "WAGES "
	jsr pchar
	lda #$41
	jsr pchar
	lda #$47
	jsr pchar
	lda #$45
	jsr pchar
	lda #$53
	jsr pchar
	lda #$20
	jsr pchar

	lda #yellow	; Textcolour yellow
	jsr setfgcol

; Print current wages in 6 digits

	ldx #$00
	stx cnt
L241A:
	ldx cnt
	lda L2F3A,X	; Wages
	jsr pchar	; Print
	inc cnt
	lda cnt
	cmp #$06
	bne L241A

; Get theme colour

	lda #green	; Background colour
	jsr setbgcol

	lda L2FE3	; Get level
	sta L2FE4	; Store current level
L2430:
	lda L2FE4	; Get current level
	cmp #$01	; Level 1?
	beq L2454	; Yes, jump

	lda L2FE4	; Get current level
	cmp #$02	; Level 2?
	beq L2454	; Yes, jump

	lda L2FE4	; Get current level
	cmp #$03	; Level 3?
	beq L2454	; Yes, jump

	sec		; Theme repeats every 3 levels
	lda L2FE4
	sbc #$03
	sta L2FE4

	lda #$01	; Jump always
	cmp #$01
	beq L2430
L2454:
	lda L2FE4	; Get current level
	sta L2FED	; Store theme

	lda L2FE4	; Set pointer
	tax
	lda L2F37-1,X	; Set textcolour
	jsr setfgcol

	jsr L1597	; 20 97 15

; Print platforms

	ldx #$08	; Start Y-pos platform
	ldy #$04	; Nr of platforms
L246B:
	lda #$00	; Textcursor at 0,8
	sta dispx
	txa
	sta dispy

	lda #16	; Set width counter
	sta L2FE4
L247E:
	lda #$7F	; Print platform block
	jsr pchar
	dec L2FE4	; All platform blocks printed?
	bne L247E	; No, repeat

	clc		; One platform down
	txa
	adc #$05
	tax

	dey
	cpy #$00	; All platforms printed?
	bne L246B	; No, repeat

; Calculate round and theme
; There are 3 themes: summer, water, winter
; Every 3 themes is 1 round
;
; L2FE4 = round
; L2FE5 = theme

	lda #$00
	sta L2FE4	; Reset round counter
	lda L2FE3	; Get level
	sta L2FE5	; Save tmp-level

; Skip if level is 1, 2 or 3

L24A2:
	lda L2FE5	; Get tmp-level
	cmp #$01	; Level 1?
	beq L24C9	; Yes, skip

	lda L2FE5	; Get tmp-level
	cmp #$02	; Level 2?
	beq L24C9	; Yes, skip

	lda L2FE5	; Get tmp-level
	cmp #$03	; Level 3?
	beq L24C9	; Yes, skip

; Subtract 3 from level to determine round

	sec
	lda L2FE5	; Get tmp-level
	sbc #$03	; Subtract 3 from level
	sta L2FE5	; Store tmp-level
	inc L2FE4	; Increment round

	jmp L24A2	; Jump always

; Calculate pointer = round x 3

L24C9:
	ldx L2FE4	; Get round
	txa
	clc
	adc L2FE4	; Calculate 2x round
	tax
	txa
	clc
	adc L2FE4	; Add round
	tax
	stx L2FE4	; Store 3x round

	lda L2F40,X	; Get nr of sprites in level
	sta L2FE9	; Store nr of sprites counter

	lda L2FE3	; Get level
	sta L2FEC	; Store

	lda #$01
	sta L2FEB	; Counter
	lda #48
	sta L2FEA	; Y-pos

L24F6:
	lda L2FEA	; Get Y-pos
	sta L2FE1	; Temp Y-pos

	ldx L2FEC	; Set pointer
	lda L2EB6,X	; BD B6 2E

	inc L2FEC	; Increment pointer
	lda L2FEC	; Check pointer
	cmp #$28	; Is max?
	bne L2511	; No, skip
	lda #$00	; Reset
	sta L2FEC	; Pointer
L2511:
	lda L2EB6,X	; Get sprite X-pos
	sta L2FE0	; Temp X-pos

	lda #<L2F03	; Set sprite data table
	sta L0070
	lda #>L2F03
	sta L0071

	ldx L2FED	; Get theme

; Point into Sprite data table

L2522:
	clc                    ; 18
	lda L0070              ; A5 70
	adc #$0C               ; 69 0C
	sta L0070              ; 85 70
	lda L0071              ; A5 71
	adc #$00               ; 69 00
	sta L0071              ; 85 71

	dex                    ; CA
	bne L2522              ; D0 F0

	ldy #$00               ; A0 00
	lda (L0070),Y          ; B1 70
	sta L0C00              ; 8D 00 0C

	ldy #$01               ; A0 01
	lda (L0070),Y          ; B1 70
	sta L0C01              ; 8D 01 0C

	ldy #$0A               ; A0 0A
	lda (L0070),Y          ; B1 70
	sta L0C02              ; 8D 02 0C

	ldy #$0B               ; A0 0B
	lda (L0070),Y          ; B1 70
	sta L0C03              ; 8D 03 0C

	ldy #$08               ; A0 08
	lda (L0070),Y          ; B1 70
	sta L0C04              ; 8D 04 0C

	ldy #$09               ; A0 09
	lda (L0070),Y          ; B1 70
	sta L0C05              ; 8D 05 0C

; Set screen address

	clc                    ; 18
	lda L0070              ; A5 70
	adc L2FEB              ; Add 1
	sta L0070              ; 85 70
	lda L0071              ; A5 71
	adc #$00               ; 69 00
	sta L0071              ; 85 71

	clc                    ; 18
	lda L0070              ; A5 70
	adc L2FEB              ; Add 1
	sta L0070              ; 85 70
	lda L0071              ; A5 71
	adc #$00               ; 69 00
	sta L0071              ; 85 71

; Set sprite data address

	ldy #$00	; Select sprite
	lda (L0070),Y
	sta L0072
	ldy #$01
	lda (L0070),Y
	sta L0073

	jsr L2642	; Calculate screenaddress
	jsr L137D	; Sprite plot routine

	inc L2FEB              ; EE EB 2F
	lda L2FEB              ; AD EB 2F
	cmp #$04               ; C9 04
	bne L2599              ; D0 05

	lda #$01               ; A9 01
	sta L2FEB              ; 8D EB 2F

; Point to next platform

L2599:
	clc
	lda L2FEA              ; AD EA 2F
	adc #40               ; 69 38
	sta L2FEA              ; 8D EA 2F

	lda L2FEA
	cmp #$d0	; All platforms?
	bne L25AE	; If not, skip

	lda #48		; Reset
	sta L2FEA	; Y-pos 
L25AE:
	dec L2FE9	; Decrement nr of sprites counter
	beq L25B6	; Jump if all sprites plotted
	jmp L24F6	; Repeat

; All sprites plotted

L25B6:
            lda #$38*48/56               ; A9 38
            sta L2FE5              ; 8D E5 2F

            ldx L2FE4              ; Get current level
            inx                    ; 
            inx                    ; 
            lda L2F40,X            ; Get byte2 from table
            sta L2FA6              ; Store

            lda L2FA6              ; Check byte
            cmp #$00               ; 
            beq L261F              ; If zero,skip

            ldx #$00               ; A2 00
            lda L2FA6              ; AD A6 2F
            sta L2FE6              ; 8D E6 2F

L25D5:      ldy L2FEC              ; AC EC 2F
            lda L2EB6,Y            ; B9 B6 2E
            inc L2FEC              ; EE EC 2F
            sta L2FA7,X            ; 9D A7 2F
            inx                    ; E8

            lda L2FEC              ; AD EC 2F
            cmp #$28               ; C9 28
            bne L25EE              ; D0 05
            lda #$00               ; A9 00
            sta L2FEC              ; 8D EC 2F

L25EE:      lda L2FE5              ; AD E5 2F
            sta L2FA7,X            ; 9D A7 2F
            inx                    ; E8
            clc                    ; 18
            lda L2FE5              ; AD E5 2F
            adc #$38*40/56               ; 69 38
            sta L2FE5              ; 8D E5 2F
            lda L2FE5              ; AD E5 2F
            cmp #$18               ; C9 18
            bne L260A              ; D0 05
            lda #$38*48/56               ; A9 38
            sta L2FE5              ; 8D E5 2F
L260A:      ldy L2FEC              ; AC EC 2F
            lda L2EB6,Y            ; B9 B6 2E
            sta L2FA7,X            ; 9D A7 2F
            lsr L2FA7,X            ; 5E A7 2F
            lsr L2FA7,X            ; 5E A7 2F
            inx                    ; E8
            dec L2FE6              ; CE E6 2F
            bne L25D5              ; D0 B6

L261F:      ldx L2FE4              ; Get current level
            inx                    ; 
            lda L2F40,X            ; Get byte1 from table
            sta L2FA4              ; Store
            sta L2FA5              ; Store

            lda L2F40,X            ; Get byte1 from table
            cmp #$00               ; Zero?
            beq L263D              ; Yes, jump

; Location horizontal moving enemy

            lda #31               ; A9 4A
            sta L2FA4              ; 8D A4 2F
            lda #88             ; A9 70
            sta L2FA5              ; 8D A5 2F
L263D:      rts                    ; 60

;------------------------------------------------
; Calculate screenaddress in L0070/71
;------------------------------------------------

L2642:
	lda L2FE1	; Temp Y-pos
	sta L0070
	lda #0
	sta L0071

	asl L0070
	rol L0071
	asl L0070
	rol L0071
	asl L0070
	rol L0071
	asl L0070
	rol L0071
	asl L0070
	rol L0071

	lda L0071
	ora #$80
	sta L0071
	clc
	lda L0070
	adc L2FE0	; Temp X-pos
	sta L0070

	ldx L0076	
	lda #$00
	sta L2FE0	; Temp X-pos

	rts	

;------------------------------------------------
; Screen row addresses
;------------------------------------------------

L26A0:      .byte $00,$80,$00,$80,$00,$80,$00,$80
            .byte $00,$80,$00,$80,$00,$80,$00,$80
            .byte $00,$80,$00,$80,$00,$80,$00,$80
            .byte $00,$80,$00,$80,$00,$80,$00,$80

L26C0:      .byte $30,$32,$35,$37,$3A,$3C
L26C6:      .byte $3F,$41
            .byte $44,$46,$49,$4B,$4E,$50,$53,$55
            .byte $58,$5A,$5D,$5F,$62,$64,$67,$69
            .byte $6C,$6E,$71,$73,$76,$78,$7B,$7D

;-------------------------------------------------
; Sprites
;
; Byte 00-31 are sprites for left two X-positions
; Byte 32-63 are sprites for right two X-positions

;-------------------------------------------------

L275A:							;Sprite 0 Empty
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L279A:							;Sprite 1 Fire
            .byte $01,$01,$00,$00,$00,$01,$02,$02
            .byte $00,$07,$02,$02,$00,$07,$02,$00
            .byte $00,$07,$02,$00,$01,$1A,$0B,$02
            .byte $00,$07,$25,$02,$01,$1A,$0B,$02
            .byte $01,$1E,$0B,$02,$07,$34,$2D,$02
            .byte $07,$35,$2D,$03,$07,$35,$3A,$0B
            .byte $01,$1E,$2D,$03,$00,$07,$0B,$02
            .byte $00,$01,$03,$00,$00,$00,$00,$00
L27DA:							;Sprite 2 Balloon
            .byte $00,$0F,$0C,$00,$15,$0F,$0C,$08
            .byte $15,$0E,$0C,$0C,$2F,$0E,$0C,$0C
            .byte $2F,$0E,$0C,$0C,$2F,$0E,$0C,$0C
            .byte $2F,$0E,$0C,$0C,$2F,$0E,$0C,$0C
            .byte $2F,$0F,$0C,$0C,$15,$0F,$0F,$08
            .byte $15,$0F,$0F,$08,$00,$3F,$3F,$00
            .byte $00,$04,$08,$00,$00,$04,$08,$00
            .byte $00,$00,$2A,$00,$00,$00,$00,$00
L281A:							;Sprite 3 Empty
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L285A:							;Sprite 4 Man dead1
            .byte $00,$00,$00,$00,$00,$00,$03,$02
            .byte $03,$00,$0C,$00,$0C,$00,$0C,$00
            .byte $0C,$00,$0C,$00,$04,$08,$0C,$00
            .byte $00,$0F,$2D,$00,$11,$0C,$0C,$22
            .byte $0A,$0C,$0C,$05,$0F,$0C,$0C,$0F
            .byte $05,$0E,$0D,$0A,$00,$0C,$0C,$00
            .byte $00,$13,$33,$00,$00,$13,$33,$22
            .byte $00,$03,$36,$00,$00,$03,$03,$00
L289A:							;Sprite 5 Man dead2
            .byte $00,$00,$00,$00,$01,$03,$00,$00
            .byte $00,$0C,$00,$03,$00,$0C,$00,$0C
            .byte $00,$0C,$00,$0C,$00,$0C,$04,$08
            .byte $22,$0F,$2D,$11,$0A,$0C,$0C,$05
            .byte $0A,$0C,$0C,$05,$0F,$0C,$0C,$0F
            .byte $05,$0E,$0D,$0A,$00,$0C,$0C,$00
            .byte $00,$13,$33,$00,$00,$13,$33,$22
            .byte $00,$03,$32,$00,$00,$03,$03,$00
L28DA:							;Sprite 6 Raster
            .byte $00,$01,$02,$03,$04,$05,$06,$07
            .byte $08,$09,$0A,$0B,$0C,$0D,$0E,$0F
            .byte $10,$11,$12,$13,$14,$15,$16,$17
            .byte $18,$19,$1A,$1B,$1C,$1D,$1E,$1F
            .byte $20,$21,$22,$23,$24,$25,$26,$27
            .byte $28,$29,$2A,$2B,$2C,$2D,$2E,$2F
            .byte $30,$31,$32,$33,$34,$35,$36,$37
            .byte $38,$39,$3A,$3B,$3C,$3D,$3E,$3F
L291A:							;Sprite 7 Take closed
	.byte $aa,$aa,$aa,$aa,$aa,$aa,$aa,$aa
	.byte $aa,$aa,$aa,$aa,$aa,$aa,$aa,$aa
	.byte $aa,$aa,$aa,$aa,$aa,$aa,$55,$55
	.byte $55,$55,$55,$55,$aa,$aa,$55,$55
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $0F,$0F,$0F,$0F,$0F,$0F,$0F,$0F
            .byte $0F,$0F,$0F,$0F,$00,$00,$00,$00
L295A:							;Sprite 8 Take3 open
            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
            .byte $00,$00,$00,$00,$0F,$0F,$0F,$0F
            .byte $0F,$03,$03,$0F,$0B,$07,$0B,$07
            .byte $0F,$0F,$0B,$07,$0F,$0B,$03,$0F
            .byte $0F,$0F,$0B,$07,$0B,$07,$0B,$07
            .byte $0F,$03,$03,$0F,$0F,$0F,$0F,$0F  
L299A:							;Sprite 9 Take2 open
            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
            .byte $00,$00,$00,$00,$0F,$0F,$0F,$0F
            .byte $0F,$03,$03,$0F,$0B,$03,$03,$07
            .byte $0B,$07,$0B,$07,$0F,$0F,$03,$0F
            .byte $0F,$0B,$07,$0F,$0F,$03,$03,$07
            .byte $0B,$03,$03,$07,$0F,$0F,$0F,$0F
L29DA:							;Sprite 10 Take1 open
	.byte $aa,$a5,$aa,$a5,$aa,$a5,$aa,$a5
	.byte $aa,$a5,$aa,$a5,$aa,$aa,$55,$55
	.byte $57,$d5,$5f,$d5,$5f,$d5,$57,$d5
	.byte $57,$d5,$5f,$f5,$5f,$f5,$55,$55
;            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
;            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
;            .byte $00,$00,$00,$0F,$00,$00,$00,$0F
;            .byte $00,$00,$00,$00,$0F,$0F,$0F,$0F
            .byte $0F,$0B,$07,$0F,$0F,$03,$07,$0F
            .byte $0F,$03,$07,$0F,$0F,$0B,$07,$0F
            .byte $0F,$0B,$07,$0F,$0F,$03,$03,$0F
            .byte $0F,$03,$03,$0F,$0F,$0F,$0F,$0F
L2A1A:							;Sprite 11 Skull
	.byte 0,0,5,80,21,84,21,84
	.byte 85,85,109,121,121,109,93,117
	.byte 21,84,22,148,22,148,21,84
	.byte 55,116,29,220,21,84,5,80
;            .byte $00,$3F,$3F,$00,$15,$3F,$3F,$2A
;            .byte $15,$3F,$3F,$3F,$3F,$3F,$3F,$3F
;            .byte $6A,$95,$6A,$95,$6A,$95,$6A,$95
;            .byte $3F,$95,$6A,$3F,$15,$3F,$3F,$2A
            .byte $15,$2A,$15,$2A,$15,$2A,$15,$2A
            .byte $15,$3F,$3F,$2A,$01,$2B,$2B,$2A
            .byte $01,$03,$03,$02,$15,$17,$17,$02
            .byte $15,$3F,$3F,$2A,$00,$3F,$3F,$00
L2A5A:							;Sprite 12 Snow Ball
        .byte 0,0,0,0,0,0,0,0
        .byte 5,144,21,100,21,88,85,102
        .byte 85,89,85,102,85,89,85,102
        .byte 85,89,21,100,21,152,6,96
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$3C,$3C,$00,$15,$3F,$3C,$28
;            .byte $15,$3F,$3E,$28,$3F,$3F,$3E,$3C
            .byte $3F,$3F,$3E,$3C,$3F,$3F,$3E,$3C
            .byte $3F,$3F,$3E,$3C,$3F,$3F,$3E,$3C
            .byte $3F,$3F,$3E,$3C,$15,$3F,$3E,$28
            .byte $15,$3F,$3C,$28,$00,$3C,$3C,$00
L2A9A:							;Sprite 13 Snowman
        .byte 15,160,15,160,255,170,17,100
        .byte 17,100,21,84,7,208,21,84
        .byte 20,148,84,149,85,85,84,149
        .byte 84,149,21,84,21,84,21,84
;            .byte $00,$0F,$0C,$00,$00,$0F,$0C,$00
;            .byte $0F,$0F,$0C,$0C,$15,$35,$2A,$2A
;            .byte $15,$35,$2A,$2A,$15,$3F,$3F,$2A
;            .byte $00,$2B,$17,$00,$15,$3F,$3F,$2A
            .byte $15,$3A,$15,$2A,$3F,$3A,$15,$3F
            .byte $3F,$3F,$3F,$3F,$3F,$3A,$15,$3F
            .byte $3F,$3A,$15,$3F,$15,$3F,$3F,$2A
            .byte $15,$3F,$3F,$2A,$15,$3F,$3F,$2A
L2ADA:							;Sprite 14 Christmas tree
        .byte 2,64,2,64,2,128,6,96
        .byte 6,144,10,144,10,160,10,160
        .byte 26,164,27,164,43,232,43,232
        .byte 35,200,3,192,19,193,87,241
;            .byte $00,$04,$2A,$00,$00,$04,$2A,$00
;            .byte $00,$04,$08,$00,$00,$2E,$2E,$00
;            .byte $00,$2E,$1D,$00,$00,$0C,$1D,$00
;            .byte $00,$0C,$0C,$00,$00,$0C,$0C,$00
            .byte $15,$0C,$0C,$2A,$15,$09,$0C,$2A
            .byte $04,$09,$06,$08,$04,$09,$06,$08
            .byte $04,$01,$02,$08,$00,$01,$02,$00
            .byte $15,$01,$02,$15,$3F,$2B,$03,$15
L2B1A:							;Sprite 15 Snowed house
        .byte 1,64,1,192,7,208,15,208
        .byte 31,244,31,252,127,255,255,255
        .byte 85,85,255,255,85,85,235,171
        .byte 105,169,235,167,105,85,105,85
;            .byte $00,$15,$2A,$00,$00,$15,$02,$00
;            .byte $00,$2B,$17,$00,$00,$03,$17,$00
;            .byte $15,$03,$03,$2A,$15,$03,$03,$02
;            .byte $2B,$03,$03,$03,$03,$03,$03,$03
            .byte $0F,$0F,$0F,$0F,$03,$03,$03,$03
            .byte $0F,$0F,$0F,$0F,$02,$01,$00,$01
            .byte $0A,$05,$00,$05,$02,$01,$00,$2B
            .byte $0A,$05,$0F,$3F,$2A,$01,$3F,$3F
L2B5A:							;Sprite 16 Skier
            .byte $0C,$00,$00,$00,$1D,$2A,$00,$00
            .byte $15,$3F,$00,$00,$15,$3F,$2A,$00
            .byte $01,$03,$20,$00,$01,$13,$33,$00
            .byte $01,$13,$22,$00,$14,$3C,$28,$28
            .byte $14,$30,$28,$28,$1C,$30,$30,$0C
            .byte $0C,$0C,$0C,$0C,$1D,$3F,$3F,$2E
            .byte $2A,$00,$00,$15,$2A,$0F,$0F,$15
            .byte $2A,$00,$00,$15,$15,$3F,$3F,$2A
L2B9A:							;Sprite 17 Red ball
        .byte 0,0,0,0,0,0,0,0
        .byte 15,240,61,252,55,252,247,255
        .byte 247,255,255,255,255,255,255,255
        .byte 255,255,63,252,63,252,15,240
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$03,$03,$00,$01,$17,$03,$02
;            .byte $01,$2B,$03,$02,$03,$2B,$03,$03
            .byte $03,$2B,$03,$03,$03,$03,$03,$03
            .byte $03,$03,$03,$03,$03,$03,$03,$03
            .byte $03,$03,$03,$03,$01,$03,$03,$02
            .byte $01,$03,$03,$02,$00,$03,$03,$00
L2BDA:							;Sprite 18 Sailing boat
        .byte 0,0,8,192,34,192,0,192
        .byte 0,192,1,32,1,32,5,40
        .byte 5,40,21,42,21,42,0,192
        .byte 85,85,255,255,21,84,5,80
;            .byte $00,$00,$00,$00,$00,$08,$0A,$00
;            .byte $04,$04,$0A,$00,$00,$00,$0A,$00
;            .byte $00,$00,$1E,$00,$00,$15,$14,$00
;            .byte $00,$15,$14,$00,$00,$3F,$14,$28
            .byte $00,$3F,$14,$28,$15,$3F,$14,$3C
            .byte $15,$3F,$14,$3C,$00,$00,$0A,$00
            .byte $3F,$3F,$3F,$3F,$03,$03,$03,$03
            .byte $15,$3F,$3F,$2A,$00,$3F,$3F,$00
L2C1A:							;Sprite 19 Whale
        .byte 0,0,0,0,0,0,17,0
        .byte 85,64,68,64,4,0,4,0
        .byte 0,2,42,130,170,162,158,170
        .byte 150,170,170,170,2,170,170,168
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $15,$15,$00,$00,$2A,$2A,$2A,$00
;            .byte $00,$2A,$00,$00,$00,$2A,$00,$00
            .byte $00,$00,$00,$00,$14,$3C,$28,$14
            .byte $3C,$3C,$3C,$14,$3D,$34,$3C,$3C
            .byte $3D,$3E,$3C,$3C,$3C,$3C,$3C,$3C
            .byte $00,$14,$3C,$3C,$3C,$3C,$3C,$28
L2C5A:							;Sprite 20 Tower
        .byte 0,0,0,0,4,72,4,104
        .byte 5,104,5,104,5,104,5,104
        .byte 5,104,5,104,4,104,4,104
        .byte 4,108,59,184,238,238,187,187
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$2A,$2A,$28,$00,$2A,$2A,$28
;            .byte $00,$3F,$3E,$28,$00,$3F,$3E,$28
;            .byte $00,$3F,$3E,$28,$00,$3F,$3E,$28
            .byte $00,$3F,$3E,$28,$00,$3F,$3E,$28
            .byte $00,$2A,$3E,$28,$00,$2A,$3E,$28
            .byte $00,$2A,$3E,$08,$05,$0F,$0C,$08
            .byte $0F,$0E,$0C,$0C,$0F,$0C,$0C,$0C
L2C9A:							;Sprite 21 Pole
        .byte 5,80,7,208,7,208,7,208
        .byte 5,80,10,160,10,160,5,80
        .byte 5,80,10,160,10,160,5,88
        .byte 1,96,8,136,34,34,136,136
;            .byte $00,$3F,$0F,$00,$00,$6A,$85,$00
;            .byte $00,$6A,$85,$00,$00,$6A,$85,$00
;            .byte $00,$3F,$0F,$00,$00,$3C,$30,$00
;            .byte $00,$3C,$30,$00,$00,$3F,$0F,$00
            .byte $00,$3F,$0F,$00,$00,$3C,$30,$00
            .byte $00,$3C,$30,$00,$00,$3F,$0F,$08
            .byte $00,$1D,$0E,$08,$04,$0C,$0C,$08
            .byte $04,$0C,$0C,$0C,$0C,$0C,$0C,$0C
L2CDA:							;Sprite 22 Player Waterjet
            .byte $03,$03,$00,$00,$03,$12,$00,$00
            .byte $03,$33,$22,$00,$03,$33,$00,$00
            .byte $04,$0C,$00,$00,$04,$0F,$0F,$1B
            .byte $04,$0F,$0F,$1B,$04,$0C,$00,$14
            .byte $05,$0F,$00,$3F,$04,$0C,$09,$3F
            .byte $04,$0C,$3F,$3F,$03,$03,$03,$03
            .byte $3F,$3F,$3F,$3F,$35,$3D,$3F,$3F
            .byte $3C,$34,$3F,$2A,$34,$3C,$3C,$00
L2D1A:							;Sprite 23 Policeman
	.byte 1,160,2,160,2,240,5,240
	.byte 1,112,202,104,234,170,2,98
	.byte 2,162,2,98,2,163,2,32
	.byte 2,32,2,32,2,32,10,40
;            .byte $00,$15,$30,$00,$00,$10,$30,$00
;            .byte $00,$14,$0F,$00,$00,$33,$0F,$00
;            .byte $00,$11,$27,$00,$22,$30,$3A,$20
;            .byte $32,$30,$30,$30,$00,$10,$3A,$10
            .byte $00,$10,$30,$10,$00,$10,$3A,$10
            .byte $00,$10,$30,$11,$00,$10,$10,$00
            .byte $00,$10,$10,$00,$00,$10,$10,$00
            .byte $00,$10,$10,$00,$00,$30,$10,$20
L2D5A:							;Sprite 24 Ambulance
	.byte 10,0,10,0,85,64,85,64
	.byte 93,84,93,104,127,104,93,104
	.byte 93,104,85,104,85,84,85,84
	.byte 170,168,136,136,136,136,168,168

;            .byte $00,$C0,$00,$00,$00,$C0,$00,$00
;            .byte $3F,$3F,$2A,$00,$3F,$3F,$2A,$00
;            .byte $3F,$17,$3F,$2A,$3F,$17,$3E,$28
;            .byte $2B,$03,$3E,$28,$2B,$03,$3E,$28
            .byte $3F,$17,$3E,$28,$3F,$17,$3E,$28
            .byte $3F,$3F,$3F,$2F,$3F,$3F,$3F,$2F
            .byte $3C,$3C,$3C,$28,$3A,$2A,$3A,$2A
            .byte $3A,$2A,$3A,$2A,$3F,$2A,$3F,$2A
L2D9A:							;Sprite 25 Tree
	.byte 1,128,5,160,22,168,22,106
	.byte 150,170,90,166,102,234,117,234
	.byte 29,216,3,240,3,192,3,192
	.byte 3,192,3,192,3,192,15,240
;            .byte $00,$05,$08,$00,$00,$0F,$0C,$00
;            .byte $00,$0E,$0C,$08,$05,$0E,$0E,$0C
;            .byte $0D,$0E,$0C,$0C,$0F,$0C,$0C,$0E
;            .byte $0E,$0E,$06,$0C,$0B,$0F,$06,$0C
            .byte $05,$07,$07,$08,$00,$01,$03,$00
            .byte $00,$01,$02,$00,$00,$01,$02,$00
            .byte $00,$01,$02,$00,$00,$01,$02,$00
            .byte $00,$01,$02,$00,$00,$03,$03,$00
L2DDA:							;Sprite 26 Graveyard
        .byte 0,0,0,0,0,0,0,0
        .byte 0,0,0,0,0,0,0,0
        .byte 1,0,1,0,5,64,1,0
        .byte 1,0,1,0,10,160,170,170
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
;            .byte $00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$15,$00,$00,$00,$15,$00,$00
            .byte $00,$3F,$2A,$00,$00,$15,$00,$00
            .byte $00,$15,$00,$00,$00,$15,$00,$00
            .byte $00,$0C,$0C,$00,$0C,$0C,$0C,$0C
L2E1A:							;Sprite 27 House
	.byte 3,192,3,196,15,244,15,244
	.byte 63,252,63,252,255,255,255,255
	.byte 85,85,105,33,105,33,85,85
	.byte 105,33,105,33,109,85,105,85
;            .byte $00,$01,$02,$00,$00,$01,$02,$2A
;            .byte $00,$03,$03,$2A,$00,$03,$03,$2A
;            .byte $01,$03,$03,$02,$01,$03,$03,$02
;            .byte $03,$03,$03,$03,$03,$03,$03,$03
            .byte $3F,$3F,$3F,$3F,$2A,$15,$08,$1D
            .byte $2A,$15,$08,$1D,$3F,$3F,$3F,$3F
            .byte $3A,$35,$08,$1D,$3A,$35,$08,$1D
            .byte $3A,$3D,$3F,$3F,$3A,$35,$3F,$3F
L2E5A:							;Sprite 28 Player motor
	.byte 255,0,255,0,246,0,213,64
	.byte 213,0,63,0,63,40,63,244
	.byte 63,244,42,8,63,200,95,216
	.byte 87,246,139,242,136,34,168,42
;            .byte $03,$03,$00,$00,$03,$03,$00,$00
;            .byte $03,$32,$00,$00,$13,$33,$22,$00
;            .byte $13,$33,$00,$00,$04,$0C,$00,$00
;            .byte $04,$0C,$15,$2A,$04,$0F,$0F,$22
            .byte $04,$0F,$0F,$22,$04,$0C,$00,$2A
            .byte $04,$0C,$08,$2A,$3C,$0C,$1C,$2A
            .byte $3C,$2C,$03,$3D,$3A,$2B,$03,$35
            .byte $3A,$2A,$15,$35,$3F,$2A,$15,$3F

;--------------------------------------------------
; X-pos sprite on screen
; Atom X-pos = BBC X-pos/80 * 32
;--------------------------------------------------

L2EB6:

; BBC  X-pos
;	.byte $2C
;		
;	.byte $30,$18,$14,$0C,$10,$3C	; Choice A
;	.byte $10,$28,$34,$38,$08,$3C	; Choice B
;	.byte $38,$1C,$34,$18,$34,$2C	; Choice C
;	.byte $34,$08,$0C,$48,$18,$48	; Choice D
;	.byte $1C,$18,$0C,$1C,$08,$44	; Choice E
;	.byte $2C,$28,$14,$3C,$48,$0C	; Choice F
;
;	.byte $38,$38,$24

; Atom X-pos
	.byte $2C*32/80
		
	.byte $30*32/80,$18*32/80,$14*32/80,$0C*32/80,$10*32/80,$3C*32/80	; Choice A
	.byte $10*32/80,$28*32/80,$34*32/80,$38*32/80,$08*32/80+1,$3C*32/80	; Choice B
	.byte $38*32/80,$1C*32/80,$34*32/80,$18*32/80,$34*32/80,$2C*32/80	; Choice C
	.byte $34*32/80,$08*32/80,$0C*32/80,$48*32/80,$18*32/80,$48*32/80	; Choice D
	.byte $1C*32/80,$18*32/80,$0C*32/80,$1C*32/80,$08*32/80,$44*32/80	; Choice E
	.byte $2C*32/80,$28*32/80,$14*32/80,$3C*32/80,$48*32/80,$0C*32/80	; Choice F

	.byte $38*32/80,$38*32/80,$24*32/80

;--------------------------------------------------
; Define colour 8 table
;--------------------------------------------------

L2EDE:      .byte $10,$04,$05,$01,$06,$03,$07,$07
            .byte $07,$03,$06,$01,$05,$04

;--------------------------------------------------
; Jump table if man dead
;--------------------------------------------------

L2EED:      .byte $00,$01,$03,$06,$08
            .byte $0B,$0D,$0E
            .byte $10,$11,$12,$13,$13,$14,$14,$14
            .byte $14,$14,$14,$13,$13,$12
L2F03:      .byte $11,$10,$0E,$0D,$0B,$08,$06,$03,$01,$00,$00,$00

;--------------------------------------------------
; Sprite data table
;--------------------------------------------------

L2F0F:
; Scene 1
	.word L2E5A	; 00/01 Player motor
	.word L2E1A	; 02/03 House
	.word L2DDA	; 04/05 Graveyard
	.word L2D9A	; 06/07 Tree
	.word L2D5A	; 08/09 Ambulance
	.word L2D1A	; 0A/0B Policeman
; Scene 2
	.word L2CDA	; 00/01 Player waterjet
	.word L2C9A	; 02/03 Pole
	.word L2C5A	; 04/05 Tower
	.word L2C1A	; 06/07 Whale
	.word L2BDA	; 08/09 Sailing boat
	.word L2B9A	; 0A/0B Red ball
; Scene 3
	.word L2B5A	; 00/01 Skier
	.word L2B1A	; 02/03 Snowed house
	.word L2ADA	; 04/05 Christmas tree
	.word L2A9A	; 06/07 Snowman
	.word L2A5A	; 08/09 Snowball
	.word L2A1A	; 0A/0B Skull

 	.word L29DA	; 24/25 Take1 open
	.word L299A	; 26/27 Take2 open
            
;--------------------------------------------------
; Colour settings platforms per theme
;--------------------------------------------------

L2F37:      .byte red
            .byte blue
            .byte yellow

;--------------------------------------------------
; Score table
;--------------------------------------------------

L2F3A:      .byte "000790"		; Current wages

;--------------------------------------------------------
; Objects for every round (round = 3 themes)
; byte 0 = nr of static objects
; byte 1 = nr of hor.moving enemies
; byte 2 = nr of ver.moving enemies
;--------------------------------------------------------

L2F40:      .byte $03,$01,$00 ; Round 0
            .byte $04,$01,$00 ; Round 1
            .byte $05,$01,$01 ; Round 2
            .byte $06,$01,$02 ; Round 3
            .byte $07,$01,$02 ; Round 4
            .byte $08,$01,$03 ; Round 5
            .byte $04,$01,$04 ; Round 6
            .byte $04,$01,$03 ; Round 7
            .byte $0B,$01,$03 ; Round 8
            .byte $0C,$01,$06 ; Round 9
            .byte $0C,$01,$07 ; Round 10
            .byte $0C,$01,$08 ; Round 11

;--------------------------------------------------------
; Intro sound table
;--------------------------------------------------------

L2F68:		;  P1  P2 DUR
            .byte $59,$1D,$02
            .byte $4D,$4D,$01
            .byte $6D,$3D,$02
            .byte $00,$00,$01
            .byte $61,$21,$02
            .byte $51,$51,$01
            .byte $75,$45,$02
            .byte $00,$00,$01
            .byte $6D,$29,$01
            .byte $69,$69,$01
            .byte $61,$61,$01
            .byte $59,$45,$01
            .byte $89,$89,$01
            .byte $69,$69,$01
            .byte $6D,$3D,$01
            .byte $4D,$4D,$01
            .byte $59,$59,$01
            .byte $6D,$6D,$01
            .byte $00,$00,$00
            .byte $00,$00,$00

;--------------------------------------------------------

L2FA4:      .byte $0D
L2FA5:      .byte $70


;--------------------------------------------------------
; Vertical Enemy data table
;   byte 0 = X-pos
;   byte 1 = Y-pos
;   byte 2 = Vertical counter 

; BBC  56,112,168,224
; Atom 48, 88,128,168
;--------------------------------------------------------

L2FA6:      .byte $02		; Counter

L2FA7:      .byte $18*32/40
L2FA8:      .byte 48
L2FA9:      .byte $09
            .byte $34*32/40,88,$06
            .byte $10*32/40,128,$13
            .byte $3C*32/40,168,$08
            .byte $10*32/40,48,$0E
            .byte $28*32/40,88,$0D
            .byte $34*32/40,128,$0E
            .byte $38*32/40,168,$07
            .byte $04*32/40,48,$00
            .byte $00*32/40,88,$00
            .byte $00*32/40,128,$00
            .byte $01*32/40,168,$00
            .byte $03*32/40,48,$01
            .byte $06*32/40,88,$02
            .byte $08*32/40,128,$02
            .byte $0B*32/40,168,$03

            .byte $0D
L2FD8:      .byte $00
L2FD9:      .byte $00

;-----------------------------------------------------
; Sound parameter block
;-----------------------------------------------------

L2FDA:      .byte $00	; Channel
L2FDB:      .byte $00
L2FDC:      .byte $00	; Amplitude
L2FDD:      .byte $0C
L2FDE:      .byte $07	; Pitch
L2FDF:      .byte $00
L2FE0:      .byte $00	; Duration
L2FE1:      .byte $04

;-----------------------------------------------------

L2FE2:      .byte $03	; Boundery counter
L2FE3:      .byte $0A	; Level

L2FE4:      .byte $00	; Dummy
L2FE5:      .byte $01
L2FE6:      .byte $05
L2FE7:      .byte $14
L2FE8:      .byte $00

L2FE9:      .byte $00	; Nr of sprites
L2FEA:      .byte $02
L2FEB:      .byte $01
L2FEC:      .byte $12

L2FED:      .byte $01	; Theme (1-3)

;-----------------------------------------------------
; PRINTAT x,y
; Locate cursor at x,y
;-----------------------------------------------------

printat:
	txa
	clc
	adc scrcol,y	; Add X to lb screenlocation
	sta $de
	lda scrrow,y	; Get hb screenlocation
	sta $df
	lda #0
	sta $e0		; Reset x offset
	rts

scrcol:	.byte $00,$20,$40,$60,$80,$a0,$c0,$e0,$00,$20,$40,$60,$80,$a0,$c0,$e0
scrrow:	.byte $80,$80,$80,$80,$80,$80,$80,$80,$81,$81,$81,$81,$81,$81,$81,$81

;-----------------------------------------------------
; CLS A
; Clear screen in colour A
;-----------------------------------------------------

cls:
	tax
	lda codcol,x
	ldx #>$8000
	stx cls2+2
cls1:
	ldx #0
cls2:
	sta $8000,x
	inx
	bne cls2
	inc cls2+2
	ldx cls2+2
	cpx #>$9800
	bne cls1
	rts
