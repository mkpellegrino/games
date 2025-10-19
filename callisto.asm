 //  Variable Labels
.label ts5 = $C000
.label characterset = $C002
.label general8bit = $C004
.label playerSFXFlag = $C005
.label z = $C006
.label status = $C007
.label num = $C008
.label xpos = $C00A
.label ypos = $C00C
.label level = $C00D
.label LZ = $C00E
.label peakarray = $C00F
.label _peakarray_1 = $C010
.label _peakarray_2 = $C011
.label _peakarray_3 = $C012
.label _peakarray_4 = $C013
.label _peakarray_5 = $C014
.label _peakarray_6 = $C015
.label _peakarray_7 = $C016
.label _peakarray_8 = $C017
.label j = $C018
.label scoreclock = $C019
//.label PAKtext = $C01B
//.label gopaText0 = $C01D
//.label gopaText1 = $C01F
//.label gopaText2 = $C021
//.label pauseText0 = $C023
//.label zero = $C025
//.label row0Text = $C027
//.label row1Text = $C029
//.label emptyText = $C02B
//.label crashedText = $C02D
//.label landedText = $C02F
//.label tooFastText = $C031
//.label spaceText = $C033
.label FUEL = $C035
.label xvel = $C037
.label yvel = $C038
.label gravbyte = $C039
.label timer = $C03A
.label rtn8bit = $C03C
.label byte0 = $C03D
.label byte1 = $C03E
.label byte2 = $C03F
.label plotDigitBindex = $C040
.label plotDigitX = $C041
.label plotDigitY = $C042
.label plotDigitColourValue11 = $C043
.label plotDigitColourValue1001 = $C044
.label digits = $C045
.label plotDigitAddr = $C047
.label keepPlaying = $C049
.label LZp = $C04A
.label leftBound = $C04B
.label rightBound = $C04D
.label touchDown = $C04F
.label c = $C050
.label T = $C051
.label myX = $C053
.label myY = $C054
.label delayLength = $C055
.label del = $C057
.label RIGHT = $C059
.label rem = $C05B
.label plotDigitOffset = $C05C
.label plotDigitColor1 = $C05E
.label plotDigitColors2And3 = $C060
.label plotDigitPixels = $C062
.label plotDigitI = $C064
.label drawSurfaceARG0 = $C065
.label X1 = $C066
.label X2 = $C067
.label getKeyRet = $C068
.label generateLandscapeRET0 = $C069
.label titleScreen = $C06A
.label tsp = $C06C
.label ts0 = $C06E
.label ts1 = $C070
.label ts2 = $C072
.label ts3 = $C074
.label ts4 = $C076
.label sprite1 = $C078
.label rightBurnerSprite = $C07A
.label leftBurnerSprite = $C07C
.label bottomBurnerSprite = $C07E
.label si = $C080
* = $0801
BasicUpstart($080D)
* = $080D
	lda #<STRLBL0
	ldx #>STRLBL0
	sta ts5
	stx ts5 +1
	lda #<LBL1L0
	sta characterset
	lda #>LBL1L0
	sta characterset +1
	
	jsr saveregs

	lda #$03
	ora $DD02
	sta $DD02
	//lda $02
	//pha // ---
	lda #$01
	sta $02
	lda $DD00
	and #$FC
	ora $02
	sta $DD00
	//pla // ---
	//sta $02
	
	lda #$03
	pha 
	jsr setScreenMode

	lda #$18
	sta $D018
	sei 
	lda $01
	and #$F8
	ora #$06
	sta $01
	cli 
	jsr screenReset
	lda #$A0
	pha 
	lda #$84
	pha 
	lda characterset
	pha 
	lda characterset +1
	pha 
	jsr bmPrintInit
	
	lda #$00
	sta general8bit
	sta playerSFXFlag
	jsr resetGame
	jsr initSprites
	jsr drawTitleScreen
// must be done prior to sound initialisation
// seed the RNG by repeatedly calling rnd until player presses a key
	lda #$00
	sta general8bit
	jsr SIDRND // initialize random number generator
	//lda $02
	//pha 
	//lda $03
	//pha 
//LBL1L1:
LBL1L2: // Top of WHILE Loop
	lda general8bit
	// OPTIMIZE
	//cmp #$00
	bne LBL1L4 // jump to ELSE
//LBL1L3:
	lda $D41B
	sta z
	lda #$09
	pha 
	lda #$16
	pha 
	lda ts5
	pha 
	lda ts5 +1
	pha 
	lda z
	pha 
	lda #$22
	pha 
	jsr bmPrint
	jsr $FFE4
	sta general8bit
	jmp LBL1L2 // jump to top of WHILE loop
LBL1L4:
	lda #$00
	sta $D015
	jsr mySeed
	jsr myRand
	pla 
	sta general8bit

// sound settings
	lda #$08
	sta $D418
	lda #$80
	sta $D407
	lda #$04
	sta $D408
	lda #$42
	sta $D40C
	lda #$E1
	sta $D40D
	lda #<irqfunc1
	ldx #>irqfunc1
	pha 
	txa 
	pha

	
	sei 
	lda #$7F
	sta $DC0D
	sta $DD0D
	lda #$01
	sta $D01A
	lda #$00
	sta $D012
	lda $D011
	and #$7F
	sta $D011
	pla  // High Byte of Address of Routine
	sta $0315
	pla  // Low Byte of Address of Routine
	sta $0314
	cli
	
	lda #$00
	sta xvel
	sta yvel
	sta timer
	sta timer +1
	sta keepPlaying
	
	lda #$FF
	sta gravbyte
	
	lda #$14
	sta plotDigitY
	
	lda #$0E
	sta plotDigitColourValue11
	sta plotDigitColourValue1001
	
	lda #<LBL1L5
	sta digits
	sta plotDigitAddr
	lda #>LBL1L5
	sta digits +1
	sta plotDigitAddr +1
		


	
LBL1L7: // OUTER GAME LOOP
	lda keepPlaying
	cmp #$3E
	bne LBL1L8
	jmp LBL1L9 // if z==1 jump to ELSE (CANNOT OPTIMIZE)
LBL1L8:
	jsr screenReset
	jsr initSprites
LBL2L0: // Top of IF statement
	lda status
	
	clc 
	bmi !+
	cmp #$01
!:	bne LBL2L2 // jump to ELSE
	jsr generateLandscape
	pla 
	sta LZ
LBL2L2:
	lda LZ
	clc 
	adc #$01
	sta LZp
	ldx LZ
	lda peakarray,X
	ldx LZp
	sta peakarray,X
	lda $02
	pha 
	lda $03
	pha 
	lda $04
	pha 
	lda $05
	pha 
	lda LZ
	sta $02
	sta $04
	lda #$00
	sta $03
	sta $05
	asl $02
	rol $03
	asl $02
	rol $03
	asl $02
	rol $03
	asl $04
	rol $05
	clc 
	lda $02
	adc $04
	sta $02
	lda $03
	adc $05
	sta $03
	asl $02
	rol $03
	asl $02
	rol $03
	lda $03
	tax 
	ldy $02
	pla 
	sta $05
	pla 
	sta $04
	pla 
	sta $03
	pla 
	sta $02
	//tya 
	sty leftBound
	stx leftBound +1
	lda LZ
	
	clc 
	adc #$01
	sta _MUL16_FD
	lda #$00
	sta _MUL16_FE
	sta _MUL16_FC
	lda #$28
	sta _MUL16_FB
	jsr MUL16
	lda MUL16R
	ldx MUL16R +1
	sta rightBound
	stx rightBound +1
	lda LZ
	pha 
	jsr drawSurface
	
	lda #$64
	ldx #$00
	sta scoreclock
	stx scoreclock +1
	
	//lda #$00
	stx yvel
	stx xvel
	
	lda #$4B
	sta ypos
LBL2L4: // Top of IF statement
	lda LZ
	cmp #$03
	bcc LBL2L6 // if c==0 jump to ELSE
	beq LBL2L6 // if z==1 jump to ELSE
LBL2L5:
	lda #$18
	ldx #$00
	sta xpos
	stx xpos +1
	jmp LBL2L7
LBL2L6:
	lda #$28
	ldx #$01
	sta xpos
	stx xpos +1
LBL2L7:
	lda ypos
	sta $D001
	lda xpos // (this _wasn't_ working)
	sta $D000
	lda #$01
	bit xpos+1
	beq !+
	// lda #$01
	ora $D010
	jmp !++
!:	lda #$FE
	and $D010
!:	sta $D010
	lda #$01
	sta $D015
	lda #$00
	pha 
	lda #$00
	pha 
	lda #<STRLBL7
	pha 
	lda #>STRLBL7
	pha 
	lda #$05
	pha 
	lda #$28
	pha 
	jsr bmPrint
	lda #$00
	pha 
	lda #$01
	pha 
	lda #<STRLBL8
	pha 
	lda #>STRLBL8
	pha 
	lda #$05
	pha 
	lda #$28
	pha 
	jsr bmPrint
	lda #$05
	sta plotDigitColourValue11
	lda #$00
	sta plotDigitY
	lda #$08
	sta plotDigitX
	lda level
	sta num
	lda #$00
	sta num +1
	jsr plotNumber
	lda #$00
	sta touchDown
	sta c

	//lda $02
	//pha 
	//lda $03
	//pha 

LBL2L9: // TOP OF INNER GAME LOOP
	lda touchDown
	//cmp #$00
	beq LBL2L10
	jmp LBL2L11 // jump to ELSE
LBL2L10:
	lda $CB
	sta c
	lda #$FF
	and timer
	tay 
	lda #$00
	and timer +1
	tax 
	tya 
	sta T
	stx T +1
LBL3L0: // Top of IF statement
	lda T +1
	bne !+
	lda T
!:	bne LBL3L2 // jump to ELSE
LBL3L1:
	lda #$20
	sta $87F8
	jsr checkLeft
	jsr checkRight
	jsr checkUp
	jsr gravity
	jsr showTelemetry
	jsr checkStatus
	lda #$00
	pha 
	lda #$04
	pha 
	jsr delay
	lda scoreclock +1
	bne !+
	lda scoreclock
!:	beq LBL3L2 // if z==1 jump to ELSE

	sec 
	lda scoreclock
	sbc #$01
	tay 
	lda scoreclock +1
	sbc #$00
	tax 
	tya 
	sta scoreclock
	stx scoreclock +1
LBL3L2:
	clc 
	lda timer
	adc #$01
	sta timer
	lda timer +1
	adc #$00
	sta timer +1
	jmp LBL2L9 // jump to top of WHILE loop

LBL2L11:
	//pla 
	//sta $03
	//pla 
	//sta $02

	lda status
	pha 
	lda #$FE
	pha 
	jsr SIGNEDCMP
	plp 
	bne LBL2L14 // jump to ELSE

	lda #$09
	pha 
	lda #$0A
	pha 
	lda #<STRLBL12
	pha 
	lda #>STRLBL12
	pha 
	lda #$05
	pha 
	lda #$15
	pha 
	jsr bmPrint
LBL2L14:

	lda status
	pha 
	lda #$FF
	pha 
	jsr SIGNEDCMP
	plp 
	bne LBL2L18 // jump to ELSE

	lda #$0E
	pha 
	lda #$0A
	pha 
	lda #<STRLBL10
	pha 
	lda #>STRLBL10
	pha 
	lda #$05
	pha 
	lda #$0B
	pha 
	jsr bmPrint
LBL2L18:

	lda status
	clc 
	bmi !+
	cmp #$01
!:	bne LBL2L22 // jump to ELSE

	lda #$0B
	pha 
	lda #$0A
	pha 
	lda #<STRLBL11
	pha 
	lda #>STRLBL11
	pha 
	lda #$05
	pha 
	lda #$11
	pha 
	jsr bmPrint
	inc level

	lda FUEL +1
	cmp #$FF
	bne !+
	lda FUEL
	cmp #$FE
!:	bcs LBL2L22 // JMP to else

	lda scoreclock +1
	//cmp #$00
	bne !+
	lda scoreclock
	//cmp #$00
!:	beq LBL2L22 // if z==1 jump to ELSE

	clc 
	lda FUEL
	adc #$32
	sta FUEL
	lda FUEL +1
	adc #$00
	sta FUEL +1
LBL2L22:
	
	lda #$FF
	pha 
	lda #$7F
	pha 
	jsr delay

	lda FUEL +1
	//cmp #$00
	bne LBL2L26
	lda FUEL
	//cmp #$00
!:	bne LBL2L26 // jump to ELSE

	jsr gopaScreen
	jsr getKey
	pla 
	sta keepPlaying
	cmp #$3E
	beq LBL2L27 // if z==1 jump to ELSE

	jsr resetGame
	jmp LBL2L27
LBL2L26:
	jsr pauseScreen	
LBL2L27:
	jmp LBL1L7 // jump to top of OUTER GAME LOOP
	
LBL1L9:	
	lda #$00
	sta $D015
	jsr clearkb
	sei 
	lda #$37 // Default Value
	sta $01
	cli 
	lda #$03
	ora $DD02
	sta $DD02
	lda $02
	pha 
	lda #$03
	sta $02
	lda $DD00
	and #$FC
	ora $02
	sta $DD00
	pla 
	sta $02
// Restore $0314/$0315 IRQ Vector
	lda #<irqrestore
	ldx #>irqrestore
	pha 
	txa 
	pha 
	sei 
	lda #$7F
	sta $DC0D
	sta $DD0D
	lda #$01
	sta $D01A
	lda #$00
	sta $D012
	lda $D011
	and #$7F
	sta $D011
	pla  // High Byte of Address of Routine
	sta $0315
	pla  // Low Byte of Address of Routine
	sta $0314
	cli 
	jsr clearsid

// set volume to 0
	lda #$00
	sta $D418
	jsr restoreregs
	rts

checkStatus:
	jsr onFloor
	pla 
	cmp #$00
	bne LBL1L11
	jmp LBL1L12 // if z==1 jump to ELSE (CANT OPTIMIZE)
LBL1L11:
	lda #$01
	sta touchDown
	lda yvel
	sta myY
	
	lda yvel	
	clc 
	bmi !+
	cmp #$00
!:	bcs LBL2L30 // jump to ELSE
	lda myY
	eor #$FF
	
	clc 
	adc #$01
	sta myY
	//tay
	// does this get used? -- yes
	lda #$FF
	adc #$00
	tax 
LBL2L30:
	lda xvel
	sta myX

	clc 
	bmi !+
	cmp #$00
!:	bcs LBL2L34 // jump to ELSE
	lda myX
	eor #$FF
	
	clc 
	adc #$01
	sta myX
	// does this get used?
	lda #$FF
	adc #$00
	tax 
LBL2L34:
	lda xpos +1
	cmp rightBound +1
	bne !+
	lda xpos
	cmp rightBound
!:	bcs LBL2L39 // JMP to else

	lda xpos +1
	cmp leftBound +1
	bne !+
	lda xpos
	cmp leftBound
!:	bcc LBL2L39 // if c==0 jump to ELSE
	beq LBL2L39 // if z==1 jump to ELSE

	lda #$01
	sta status
	lda myX	
	clc 
	adc myY
	tay
	
	lda #$00
	adc #$00
	bne !+
	tya 
	cmp #$04
!:	bcc LBL3L15 // if c==0 jump to ELSE
	beq LBL3L15 // if z==1 jump to ELSE 
	lda #$FE
	sta status
LBL3L15:
	jmp LBL1L12
LBL2L39:
	lda #$FF
	sta status
LBL1L12:
	rts

showTelemetry:
	// DIVISION BY 3 CAN BE OPTIMIZED
	lda #$00
	sta plotDigitY
	lda scoreclock
	sta _DIV16_FB
	lda scoreclock +1
	sta _DIV16_FC
	lda #$03
	sta _DIV16_FD
	lda #$00
	sta _DIV16_FE
	jsr DIV16
	lda _DIV16_FB
	ldx _DIV16_FC
	sta num
	stx num +1
	lda #$17
	sta plotDigitX
	jsr plotNumber
	lda xvel
	sta general8bit

	lda xvel
	clc 
	bmi !+
	cmp #$00
!:	bcs LBL1L16 // jump to ELSE
	lda general8bit
	eor #$FF
	clc 
	adc #$01
	sta general8bit
LBL1L16:
	lda general8bit
	ldx #$00
	sta num
	stx num +1
	lda #$27
	sta plotDigitX
	jsr plotNumber

	// vv-------vv   I have no idea why this is needed!
	inc plotDigitY
	// ^^-------^^
	
	lda #$08
	sta plotDigitX
	lda #$01
	sta plotDigitY
	
	lda FUEL
	ldx FUEL +1
	sta num
	stx num +1
	jsr plotNumber
	
	lda FUEL +1
	//cmp #$00
	bne !+
	lda FUEL
	//cmp #$00
!:	bne LBL1L20 // jump to ELSE
	lda #$06
	pha 
	lda #$01
	pha 
	lda #<STRLBL9
	pha 
	lda #>STRLBL9
	pha 
	lda #$05
	pha 
	lda #$05
	pha 
	jsr bmPrint
	jmp LBL1L21
	
LBL1L20:
	lda #$05
	sta plotDigitColourValue11
	lda FUEL
	ldx FUEL +1
	sta num
	stx num +1
	lda #$08
	sta plotDigitX
	jsr plotNumber
LBL1L21:
	
	lda yvel
	sta general8bit

	lda yvel
	clc 
	bmi !+
	cmp #$00
!:	bcs LBL1L24 // jump to ELSE

	lda general8bit
	eor #$FF
	
	clc 
	adc #$01
	sta general8bit
LBL1L24:
	lda general8bit
	ldx #$00
	sta num
	stx num +1
	lda #$27
	sta plotDigitX
	jsr plotNumber
	rts
	
gravity:
	lda gravbyte
	cmp #$FF
	bne LBL1L28 // jump to ELSE
	lda $C038
	clc 
	adc #$01
	sta yvel
LBL1L28:

	lda yvel
	clc 
	bmi !+
	cmp #$09
!:	bcc LBL1L32 // if c==0 jump to ELSE
	beq LBL1L32 // if z==1 jump to ELSE
	lda #$09
	sta yvel
LBL1L32:
	lda #$FF
	eor gravbyte
	sta gravbyte
	lda xpos +1
	cmp #$01
	bne !+
	lda xpos
	cmp #$40
!:	bcc LBL1L36 // if c==0 jump to ELSE
	beq LBL1L36 // if z==1 jump to ELSE
	lda #$40
	ldx #$01
	sta xpos
	stx xpos +1
LBL1L36:
	lda xpos +1
	cmp #$00
	bne !+
	lda xpos
	cmp #$18
!:	bcs LBL1L40 // jump to ELSE
	lda #$18
	ldx #$00
	sta xpos
	stx xpos +1
LBL1L40:
	lda ypos
	cmp #$4C
	bcs LBL1L44 // jump to ELSE
	lda #$4C
	sta ypos
LBL1L44:
	lda ypos
	cmp #$DD
	bcc LBL1L48 // if c==1 jump to ELSE
	lda #$DC
	sta ypos
	lda #$00
	sta yvel
LBL1L48:
	jsr onFloor
	pla 
	cmp #$01
	bne LBL1L52 // jump to ELSE
	lda #$00
	sta yvel
LBL1L52:
	lda ypos
	clc 
	adc yvel
	sta ypos
	
	clc 
	ldx #$00
	lda xvel
	bpl !+
	dex 
!:	adc xpos
	sta xpos
	txa 
	adc xpos +1
	sta xpos +1

	lda ypos
	sta $D001
	lda xpos
	sta $D000
	lda #$01
	bit xpos+1
	beq !+
	ora $D010
	jmp !++
!:	lda #$FE
	and $D010
!:	sta $D010
	rts

delay:
	pla
	tax
	pla
	tay
	pla 
	sta delayLength +1
	pla 
	sta delayLength
	tya
	pha
	txa
	pha
	
	lda #$00
	sta del
	sta del +1
LBL1L55: // Top of FOR Loop
	lda del +1
	cmp delayLength +1
	bne !+
	lda del
	cmp delayLength
!:	bcs LBL1L58 // jump out of FOR
	clc 
	lda del
	adc #$01
	sta del
	lda del +1
	adc #$00
	sta del +1
	jmp LBL1L55
LBL1L58:
	rts 

checkLeft:
	lda c
	cmp #$1E
	bne LBL1L61 // jump to ELSE
	lda FUEL +1
	//cmp #$00
	bne !+
	lda FUEL
	//cmp #$00
!:	bcc LBL1L61 // if c==0 jump to ELSE
	beq LBL1L61 // if z==1 jump to ELSE
	lda #$0A
	sta playerSFXFlag
	lda #$21
	sta $87F8
	dec xvel
	lda xvel
	pha 
	lda #$FB
	pha 
	jsr SIGNEDCMP
	plp 
	bcs LBL3L19 // jump to ELSE
	lda #$FB
	sta xvel
LBL3L19:
	jsr loseFuel
LBL1L61:
	rts
	
checkRight:
	lda c
	cmp #$26
	bne LBL1L66 // jump to ELSE
	lda FUEL +1
	//cmp #$00
	bne !+
	lda FUEL
	//cmp #$00
!:	bcc LBL1L66 // if c==0 jump to ELSE
	beq LBL1L66 // if z==1 jump to ELSE
	lda #$0A
	sta playerSFXFlag
	lda #$22
	sta $87F8
	inc xvel
	lda xvel
	clc 
	bmi !+
	cmp #$05
!:	bcc LBL3L23 // if c==0 jump to ELSE
	beq LBL3L23 // if z==1 jump to ELSE
	lda #$05
	sta xvel
LBL3L23:
	jsr loseFuel
LBL1L66:
	rts

checkUp:
	lda c
	cmp #$21
	bne LBL1L70 // jump to ELSE
	lda FUEL +1
	//cmp #$00
	bne !+
	lda FUEL
	//cmp #$00
!:	bcc LBL1L70 // if c==0 jump to ELSE
	beq LBL1L70 // if z==1 jump to ELSE
	
	// play sound effect for 10 ticks
	lda #$0A
	sta playerSFXFlag

	lda #$23
	sta $87F8
	lda $C038
	sec 
	sbc #$02
	sta yvel
	//lda yvel
	pha 
	lda #$F7
	pha 
	jsr SIGNEDCMP
	plp 
	bcs LBL3L27 // jump to ELSE
	lda #$F7
	sta yvel
LBL3L27:
	jsr loseFuel
LBL1L70:
	rts

	
clearkb:
	lda #$00
	sta $C6
	jsr $FFE4
	rts 

loseFuel:
	sec 
	lda FUEL
	sbc #$01
	sta FUEL
	lda FUEL +1
	sbc #$00
	sta FUEL +1
	rts 

onFloor:
	sec 
	lda xpos
	sbc #$04
	pha
	lda xpos +1
	sbc #$00
	pha
	lda ypos
	sec 
	sbc #$1D
	pha 
	jsr getpixel
	pla
	
	tay 
	lax #$00
	cpy #$00
	beq !+ +1
	cpy #$01
	beq !+
	pla 
	tax 
!:	pla 
	sta RIGHT
	stx RIGHT +1
	sta !+
	stx !++
	.byte $AD // <-- LDA Absolute
!:	.byte $00
!:	.byte $00
	sta byte0
	
	sec 
	lda RIGHT
	sbc #$08
	sta !+
	lda RIGHT +1
	sbc #$00
	sta !++
	
	.byte $AD // <-- LDA Absolute
!:	.byte $00
!:	.byte $00
	
	sta byte1
	
	sec 
	lda RIGHT
	sbc #$10
	sta !+
	lda RIGHT +1
	sbc #$00
	sta !++
	.byte $AD // <-- LDA Absolute
!:	.byte $00
!:	.byte $00
	sta byte2
	lda byte1
	ora byte0
	ora byte2
	sta rtn8bit

	pla
	tax
	pla
	tay
	
	lda rtn8bit
	pha

	tya
	pha
	txa
	pha
	rts

	
plotNumber:
	lda num +1
	bne !+
	lda num
!:	bne LBL1L73 // jump to ELSE

	lda plotDigitX
	pha 
	lda plotDigitY
	pha 
	lda #<STRLBL6
	pha 
	lda #>STRLBL6
	pha 
	lda #$05
	pha 
	lda #$01
	pha 
	jsr bmPrint
	jmp LBL1L74
LBL1L73:
	lda num +1
	bne !+
	lda num
!:	beq LBL2L56 // if z==1 jump to ELSE


	// DIVISION BY 10 - OPTIMIZE
	lda num
	sta _DIV16_FB
	lda num +1
	sta _DIV16_FC
	lda #$0A
	sta _DIV16_FD
	lda #$00
	sta _DIV16_FE
	jsr DIV16
	lda _DIV16_FB
	ldx _DIV16_FC
	sta num
	stx num +1
	lda $02
	sta rem
	lda rem
	asl 
	asl 
	asl 
	sta plotDigitBindex
	jsr plotDigit
	dec plotDigitX
	jmp LBL1L73 // jump to top of WHILE loop
LBL2L56:
	lda plotDigitX
	pha 
	lda plotDigitY
	pha 
	lda #<STRLBL13
	pha 
	lda #>STRLBL13
	pha 
	lda #$02
	pha 
	lda #$05
	pha 
	jsr bmPrint
LBL1L74:
	rts

plotDigit:
	lda plotDigitY
	ldx #$00
	sta plotDigitOffset
	stx plotDigitOffset +1
	lda $02
	pha 
	lda $03
	pha 
	lda $04
	pha 
	lda $05
	pha 
	lda plotDigitOffset
	sta $02
	sta $04
	lda plotDigitOffset +1
	sta $03
	sta $05
	asl $02
	rol $03
	asl $02
	rol $03
	asl $02
	rol $03
	asl $04
	rol $05
	clc 
	lda $02
	adc $04
	sta $02
	lda $03
	adc $05
	sta $03
	asl $02
	rol $03
	asl $02
	rol $03
	lda $03
	tax 
	ldy $02
	pla 
	sta $05
	pla 
	sta $04
	pla 
	sta $03
	pla 
	sta $02
	tya 
	pha 
	txa 
	tax 
	pla 
	clc 
	adc plotDigitX
	sta plotDigitOffset
	txa 
	adc #$00
	sta plotDigitOffset +1

	clc 
	lda plotDigitOffset
	adc #$00
	sta plotDigitColor1
	sta !+
	lda plotDigitOffset +1
	adc #$D8
	sta plotDigitColor1 +1
	sta !++

	clc 
	lda plotDigitOffset
	adc #$00
	sta plotDigitColors2And3
	sta !+++
	lda plotDigitOffset +1
	adc #$84
	sta plotDigitColors2And3 +1
	sta !++++

	lda plotDigitColourValue11
	.byte $8D // STA absolute
!:	.byte $00
!:	.byte $00
	lda plotDigitColourValue1001
	.byte $8D // STA absolute
!:	.byte $00
!:	.byte $00

	lda plotDigitOffset
	asl 
	tay 
	lda plotDigitOffset+1
	rol 
	tax 
	tya 
	asl 
	tay 
	txa 
	rol 
	tax 
	tya 
	asl 
	tay 
	txa 
	rol 
	tax 
	tya 
	stx plotDigitOffset +1
	sta plotDigitOffset
	clc 
	//lda plotDigitOffset
	adc #$00
	sta plotDigitPixels
	lda plotDigitOffset +1
	adc #$A0
	sta plotDigitPixels +1

	lda $02
	pha 
	lda $03
	pha 

	lda #$00
	sta plotDigitI


LBL1L76: // Top of FOR Loop
	lda plotDigitI
	cmp #$08
	bcs LBL1L79 // jump out of FOR
	lda plotDigitAddr
	sta !+
	lda plotDigitAddr+1
	sta !++
	ldx plotDigitBindex
	.byte $BD // <-- LDA Absolute, X
!:	.byte $00
!:	.byte $00
	ldy plotDigitPixels
	sty !+
	ldy plotDigitPixels +1
	sty !++
	.byte $8D // <-- STA absolute
!:	.byte $00
!:	.byte $00
	
	clc 
	lda #$01
	adc plotDigitPixels
	sta plotDigitPixels
	lda #$00
	adc plotDigitPixels +1
	sta plotDigitPixels +1

	inc plotDigitBindex
	inc plotDigitI
	jmp LBL1L76
LBL1L79:
	pla 
	sta $03
	pla 
	sta $02
	rts


	
//!rx:	.byte $00
//!ry:	.byte $00
drawSurface:
	pla
	tax
	//sta !rx-
	pla
	tay
	//sta !ry-
	pla 
	sta drawSurfaceARG0
	//lda !ry-
	tya
	pha 
	txa
	//lda !rx-	
	pha

	lda #$00
	sta j
	
LBL1L81: // Top of FOR Loop
	lda j
	cmp #$08
	bcc LBL1L83 // if c==0 jump to BODY
	jmp LBL1L84 // jump out of FOR (CANT OPTIMIZE)
LBL1L83:
	lda j
	sta _MUL16_FB
	lda #$00
	sta _MUL16_FC
	sta _MUL16_FE
	lda #$14
	sta _MUL16_FD
	jsr MUL16
	lda MUL16R
	ldx MUL16R +1
	sta X1
	lda j
	clc 
	adc #$01
	sta _MUL16_FD
	lda #$00
	sta _MUL16_FE
	sta _MUL16_FC
	lda #$14
	sta _MUL16_FB
	jsr MUL16
	lda MUL16R
	ldx MUL16R+1
	sta X2

	lda j
	cmp #$07
	bne LBL2L59 // jump to ELSE
	dec X2
LBL2L59:

	lda j
	cmp drawSurfaceARG0
	beq LBL2L62
	jmp LBL2L63 // jump to ELSE (CANT OPTIMIZE)
LBL2L62:
	lda #$00
	sta general8bit


LBL3L30: // Top of FOR Loop
	lda general8bit
	cmp #$04
	bcs LBL3L33 // jump out of FOR
	lda X1
	pha 
	ldx j
	lda peakarray,X
	clc 
	adc general8bit
	pha 
	lda X2
	pha 
	lda j
	
	clc 
	adc #$01
	tax 
	lda peakarray,X
	clc 
	adc general8bit
	pha 
	lda #$02
	pha 
	jsr segment
	inc general8bit
	jmp LBL3L30
	
LBL3L33:
	lda X1
	pha 
	ldx j
	lda peakarray,X
	
	clc 
	adc #$03
	pha 
	lda X2
	pha 
	lda j
	
	clc 
	adc #$01
	tax 
	lda peakarray,X
	
	clc 
	adc #$09
	pha 
	lda #$03
	pha 
	jsr segment
	lda X1
	pha 
	ldx j
	lda peakarray,X
	
	clc 
	adc #$09
	pha 
	lda X2
	pha 
	lda j
	
	clc 
	adc #$01
	tax 
	lda peakarray,X
	
	clc 
	adc #$03
	pha 
	lda #$03
	pha 
	jsr segment
	jmp LBL2L64
LBL2L63:
	lda X1
	pha 
	ldx j
	lda peakarray,X
	pha 
	lda X2
	pha 
	lda j
	clc 
	adc #$01
	tax 
	lda peakarray,X
	pha 
	lda #$03
	pha 
	jsr segment
	lda X1
	pha 
	ldx j
	lda peakarray,X
	clc 
	adc #$01
	pha 
	lda X2
	pha 
	lda j
	clc 
	adc #$01
	tax 
	lda peakarray,X
	clc 
	adc #$09
	pha 
	lda #$03
	pha 
	jsr segment
	lda X1
	pha 
	ldx j
	lda peakarray,X
	clc 
	adc #$09
	pha 
	lda X2
	pha 
	lda j
	clc 
	adc #$01
	tax 
	lda peakarray,X
	clc 
	adc #$01
	pha 
	lda #$03
	pha 
	jsr segment
LBL2L64:
	inc j
	jmp LBL1L81
LBL1L84:
	rts

	
pauseScreen:
	lda #$00
	sta $D015
	lda #$07
	pha 
	lda #$0D
	pha 
	lda #<STRLBL5
	pha 
	lda #>STRLBL5
	pha 
	lda #$05
	pha 
	lda #$11
	pha 
	jsr bmPrint
	jsr pause
	rts

	
gopaScreen:
	lda #$00
	sta $D015
	lda #$0F
	pha 
	lda #$0C
	pha 
	lda #<STRLBL2
	pha 
	lda #>STRLBL2
	pha 
	lda #$05
	pha 
	lda #$09
	pha 
	jsr bmPrint
	lda #$0B
	pha 
	lda #$0E
	pha 
	lda #<STRLBL3
	pha 
	lda #>STRLBL3
	pha 
	lda #$05
	pha 
	lda #$11
	pha 
	jsr bmPrint
	lda #$06
	pha 
	lda #$10
	pha 
	lda #<STRLBL4
	pha 
	lda #>STRLBL4
	pha 
	lda #$05
	pha 
	lda #$1B
	pha 
	jsr bmPrint
	rts 

getKey:
	jsr clearkb
	lda $CB
	sta getKeyRet
	
LBL1L86: // Top of WHILE Loop
	lda getKeyRet
	cmp #$40
	bne LBL1L88 // jump to ELSE
	lda $CB
	sta getKeyRet
	jsr clearkb
	jmp LBL1L86 // jump to top of WHILE loop
	
LBL1L88:

	
	pla
	tax
	pla
	tay
	
	lda getKeyRet
	pha
	
	tya
	pha
	txa
	pha 
	rts
	
// the sound effect routine
irqfunc1:
	lda playerSFXFlag
	//cmp #$00 // is this needed?
	beq !++ // if z==1 jump to ELSE
	lda #$81
	sta $D40B
	lda playerSFXFlag
	cmp #$01
	bne !+ // jump to ELSE
	lda #$80
	sta $D40B
!:	dec playerSFXFlag
!:	asl $D019
	jmp $EA31


irqrestore:
	asl $D019
	jmp $EA31

generateLandscape:
	lda #$00
	sta j
LBL1L94: // Top of FOR Loop
	lda j
	cmp #$09
	bcs LBL1L97 // jump out of FOR
	jsr myRand
	pla 
	pha
	txa // ??
	tax // ?
	pla
	tay 
	txa 
	lsr 
	tax 
	tya 
	ror 
	pha 
	txa // ??
	tax // ?
	pla
	
	clc 
	adc #$39
	tay
	lda $C018
	tax 
	tya 
	sta $C00F,X
	inc j
	jmp LBL1L94
LBL1L97:
	jsr myRand
	pla
	lsr
	lsr
	lsr
	lsr
	lsr
	sta generateLandscapeRET0


	
	pla
	tax
	pla
	tay
	lda generateLandscapeRET0
	pha 
	tya
	pha
	txa
	pha 
	rts 

resetGame:
	ldx #$00
	stx yvel
	stx xvel
	stx playerSFXFlag
	stx FUEL +1
	inx
	stx level
	stx status
	ldx #$50
	stx FUEL
	rts
	
screenReset:
	lda #$00
	sta $D020
	sta $D021
	lda #$D8
	pha 
	lda #$05
	pha 
	lda #$04
	pha 
	jsr fillmem
	lda #$84
	pha 
	lda #$D7
	pha 
	lda #$12
	pha 
	jsr fillmem
	lda #$A0
	pha 
	lda #$20
	pha 
	jsr clearmem
	rts
	
drawTitleScreen:
	lda #<LBL1L98
	sta titleScreen
	lda #>LBL1L98
	sta titleScreen +1
	lda titleScreen
	ldx titleScreen +1
	sta tsp
	stx tsp +1

	//lda $02
	//pha 
	//lda $03
	//pha 

	lda #$00
	sta general8bit

LBL1L100: // Top of FOR Loop
	lda general8bit
	cmp #$1E
	bcs LBL1L103 // jump out of FOR
	
	lda tsp
	sta !+
	sta !+++
	sta !+++++
	sta !+++++++
	
	lda tsp+1
	sta !++
	sta !++++
	sta !++++++
	sta !++++++++

	ldx #$00
	.byte $BD // <-- LDA Absolute,X
!:	.byte $00
!:	.byte $00
	pha 
	inx
	.byte $BD // <-- LDA Absolute,X
!:	.byte $00
!:	.byte $00
	pha
	inx
	.byte $BD // <-- LDA Absolute,X
!:	.byte $00
!:	.byte $00
	pha 
	inx
	.byte $BD // <-- LDA Absolute,X
!:	.byte $00
!:	.byte $00
	pha 
	lda #$03
	pha 
	jsr segment

	clc 
	lda tsp
	adc #$04
	sta tsp
	lda tsp +1
	adc #$00
	sta tsp +1
	
	inc general8bit
	jmp LBL1L100
	
LBL1L103:
	lda #$04
	pha 
	lda #$18
	pha 
	lda #<STRLBL14
	pha 
	lda #>STRLBL14
	pha
	lda #$05
	pha 
	lda #$22
	pha 
	jsr bmPrint
	lda #$05
	pha 
	lda #$0E
	pha 
	lda #<STRLBL15
	pha 
	lda #>STRLBL15
	pha 
	lda #$05
	pha 
	lda #$1F
	pha 
	jsr bmPrint
	lda #$04
	pha 
	lda #$10
	pha 
	lda #<STRLBL16
	pha 
	lda #>STRLBL16
	pha 
	lda #$05
	pha 
	lda #$20
	pha 
	jsr bmPrint
	lda #$02
	pha 
	lda #$12
	pha 
	lda #<STRLBL17
	pha 
	lda #>STRLBL17
	pha 
	lda #$05
	pha 
	lda #$23
	pha 
	jsr bmPrint
	lda #$02
	pha 
	lda #$14
	pha 
	lda #<STRLBL18
	pha 
	lda #>STRLBL18
	pha 
	lda #$05
	pha 
	lda #$10
	pha 
	jsr bmPrint
	
	//lda $02
	//pha 
	//lda $03
	//pha 

	lda #$00
	sta j
LBL1L105: // Top of FOR Loop
	lda j
	cmp #$09
	bcs LBL1L108 // jump out of FOR
	jsr myRand
	pla
	ldx #$00
	sta _DIV16_FB
	stx _DIV16_FC
// this is division by 3... it can probably be optimized
	lda #$03
	sta _DIV16_FD
	lda #$00
	sta _DIV16_FE
	jsr DIV16
	lda _DIV16_FB
	ldx _DIV16_FC
	pha 
	txa // ?
	tax // ?
	pla 
	clc 
	adc #$28
	tay 
	txa 
	adc #$00
	tax 
	tya 
	lda $C018
	tax 
	tya 
	sta $C00F,X
	inc j
	jmp LBL1L105
	
LBL1L108:
	//pla 
	//sta $03
	//pla 
	//sta $02

	
	lda #$02
	pha 
	jsr drawSurface

	ldx #$00
	//asl 

	//tax 
	lda #$35
	sta $D000,X // set the x-coord
	inx 
	lda #$3B
	sta $D000,X // set the y-coord
	lda #$01
	ora $D010
	sta $D010
	lda #$01
	sta $D015
	rts
	
initSprites:
	lda #<LBL1L109
	sta sprite1
	lda #>LBL1L109
	sta sprite1 +1
	lda #<LBL1L110
	sta rightBurnerSprite
	lda #>LBL1L110
	sta rightBurnerSprite +1
	lda #<LBL1L111
	sta leftBurnerSprite
	lda #>LBL1L111
	sta leftBurnerSprite +1
	lda #<LBL1L112
	sta bottomBurnerSprite
	lda #>LBL1L112
	sta bottomBurnerSprite +1
	lda #$20
	sta $87F8
// copy the sprite data to $8800


	lda #$00
	sta si
LBL1L114: // Top of FOR Loop
	lda si
	cmp #$FF
	bcs LBL1L117 // jump out of FOR
	lda #$00
	ldx #$88
	clc 
	adc si
	bcc !+
	inx 
!:
	jsr PUSH
	txa 
	jsr PUSH
	clc 
	lda sprite1
	adc si
	sta !+
	lda #$00
	adc sprite1 +1
	sta !++
	.byte $AD // <-- LDA Absolute
!:	.byte $00
!:	.byte $00
	tay 
	jsr POP
	sta !++
	jsr POP
	sta !+
	.byte $8C // <-- STY absolute
!:	.byte $00
!:	.byte $00
	inc si
	jmp LBL1L114
	
LBL1L117:
	lda #$05
	sta $D027
	rts
	
SIGNEDCMP:  //  Signed Comparison
	pla 
	jsr PUSH
	pla 
	jsr PUSH
	lda $02
	jsr PUSH
	lda $03
	jsr PUSH
	pla 
	sta $02
	pla 
	sta $03
	eor $02
	rol 
	bcs !+
	lda $02
	pha 
	lda $03
	sta $02
	pla 
	sta $03
!:	lda $02
	cmp $03
	php 
	jsr POP
	sta $03
	jsr POP
	sta $02
	jsr POP
	pha 
	jsr POP
	pha 
	rts 
_DIV16_FD:
	.byte $00
_DIV16_FE:
	.byte $00
_DIV16_FB:
	.byte $00
_DIV16_FC:
	.byte $00
DIV16:
	lda #$00
	sta $02
	sta $03
	ldx #$10
!:
	asl _DIV16_FB
	rol _DIV16_FC
	rol $02
	rol $03
	lda $02
	sec 
	sbc _DIV16_FD
	tay 
	lda $03
	sbc _DIV16_FE
	bcc !+
	sta $03
	sty $02
	inc _DIV16_FB
!:
	dex 
	bne !--
	rts 
!:
_MUL16_FB:
	.byte $00
!:
_MUL16_FC:
	.byte $00
!:
_MUL16_FD:
	.byte $00
!:
_MUL16_FE:
	.byte $00
MUL16R:
	.byte $00, $00, $00, $00
MUL16:
	lda #$00
	sta MUL16R
	sta MUL16R +1
	sta MUL16R +2
	sta MUL16R +3
	ldx #$10
!:	lsr !--
	ror !---
	bcc !+
	tay 
	clc 
	lda !-----
	adc MUL16R +2
	sta MUL16R +2
	tya 
	adc !----
!:	ror 
	ror MUL16R +2
	ror MUL16R +1
	ror MUL16R
	dex 
	bne !--
	sta MUL16R+3
	rts 
SIDRND:
	pha 
	lda #$FF
	sta $D40E
	sta $D40F
	lda #$80
	sta $D412
	pla 
	rts 
!:	.byte $00
!:	.byte $00
PUSH:
	stx !-
	ldx !--
	inx 
	sta $CF00,X
	stx !--
	ldx !-
	rts 
POP:
	stx !-
	ldx !--
	lda $CF00,X
	dex 
	stx !--
	ldx !-
	rts 
 // ; $1bd5			"PRESS ANY KEY TO BEGIN"
STRLBL0:
	.byte  $50, $52, $45, $53, $53, $20, $41, $4E, $59, $20, $4B, $45, $59, $20, $54, $4F, $20, $42, $45, $47, $49, $4E, $00
 // ; $1bfa			"GAME OVER"
STRLBL2:
	.byte  $47, $41, $4D, $45, $20, $4F, $56, $45, $52, $00
 // ; $1c04			"PRESS 'Q' TO QUIT"
STRLBL3:
	.byte  $50, $52, $45, $53, $53, $20, $27, $51, $27, $20, $54, $4F, $20, $51, $55, $49, $54, $00
 // ; $1c16			"ANY OTHER KEY TO PLAY AGAIN"
STRLBL4:
	.byte  $41, $4E, $59, $20, $4F, $54, $48, $45, $52, $20, $4B, $45, $59, $20, $54, $4F, $20, $50, $4C, $41, $59, $20, $41, $47, $41, $49, $4E, $00
 // ; $1c32			"PRESS ANY KEY TO CONTINUE"
STRLBL5:
	.byte  $50, $52, $45, $53, $53, $20, $41, $4E, $59, $20, $4B, $45, $59, $20, $54, $4F, $20, $43, $4F, $4E, $54, $49, $4E, $55, $45, $00
 // ; $1c4c			"0"
STRLBL6:
	.byte  $30, $00
 // ; $1c4e			"LEVEL:         CLOCK:         X-SPEED:"
STRLBL7:
	.byte  $4C, $45, $56, $45, $4C, $3A, $20, $20, $20, $20, $20, $20, $20, $20, $20, $43, $4C, $4F, $43, $4B, $3A, $20, $20, $20, $20, $20, $20, $20, $20, $20, $58, $2D, $53, $50, $45, $45, $44, $3A, $00
 // ; $1c75			"FUEL:                         Y-SPEED:"
STRLBL8:
	.byte  $46, $55, $45, $4C, $3A, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $20, $59, $2D, $53, $50, $45, $45, $44, $3A, $00
 // ; $1c9c			"EMPTY"
STRLBL9:
	.byte  $45, $4D, $50, $54, $59, $00
 // ; $1ca2			"YOU CRASHED"
STRLBL10:
	.byte  $59, $4F, $55, $20, $43, $52, $41, $53, $48, $45, $44, $00
 // ; $1cae			"YOU SAFELY LANDED"
STRLBL11:
	.byte  $59, $4F, $55, $20, $53, $41, $46, $45, $4C, $59, $20, $4C, $41, $4E, $44, $45, $44, $00
 // ; $1cc0			"YOU CAME IN TOO FAST!"
STRLBL12:
	.byte  $59, $4F, $55, $20, $43, $41, $4D, $45, $20, $49, $4E, $20, $54, $4F, $4F, $20, $46, $41, $53, $54, $21, $00
 // ; $1cd6			" "
STRLBL13:
	.byte  $20, $00
 // ; $1cd8			"(C) 2025 - MICHAEL K. PELLEGRINO"
STRLBL14:
	.byte  $28, $43, $29, $20, $32, $30, $32, $35, $20, $2D, $20, $4D, $49, $43, $48, $41, $45, $4C, $20, $4B, $2E, $20, $50, $45, $4C, $4C, $45, $47, $52, $49, $4E, $4F, $00
 // ; $1cf9			"THRUSTERS: I-UP U-LEFT O-RIGHT"
STRLBL15:
	.byte  $54, $48, $52, $55, $53, $54, $45, $52, $53, $3A, $20, $49, $2D, $55, $50, $20, $55, $2D, $4C, $45, $46, $54, $20, $4F, $2D, $52, $49, $47, $48, $54, $00
 // ; $1d18			"LAND THE CRAFT ON THE LANDING PAD"
STRLBL16:
	.byte  $4C, $41, $4E, $44, $20, $54, $48, $45, $20, $43, $52, $41, $46, $54, $20, $4F, $4E, $20, $54, $48, $45, $20, $4C, $41, $4E, $44, $49, $4E, $47, $20, $50, $41, $44, $00
 // ; $1d3a			"BEFORE TIME EXPIRES TO GET MORE FUEL"
STRLBL17:
	.byte  $42, $45, $46, $4F, $52, $45, $20, $54, $49, $4D, $45, $20, $45, $58, $50, $49, $52, $45, $53, $20, $54, $4F, $20, $47, $45, $54, $20, $4D, $4F, $52, $45, $20, $46, $55, $45, $4C, $00
 // ; $1d5f			"FOR THE NEXT LEVEL"
STRLBL18:
	.byte  $46, $4F, $52, $20, $54, $48, $45, $20, $4E, $45, $58, $54, $20, $4C, $45, $56, $45, $4C, $00
LBL1L0: .byte $00, $00, $00, $00, $00, $00, $00, $00, $00, $30, $30, $30, $30, $00, $30, $30, $00, $CC, $CC, $00, $00, $00, $00, $00, $00, $3C, $30, $30, $FC, $30, $33, $FF, $00, $00, $00, $00, $00, $00, $00, $00, $00, $30, $F0, $F0, $30, $3C, $3C, $30, $00, $00, $00, $00, $00, $00, $00, $00, $0C, $0C, $0C, $00, $00, $00, $00, $00, $0C, $30, $30, $30, $30, $30, $30, $0C, $C0, $30, $30, $30, $30, $30, $30, $C0, $FF, $FF, $C0, $C0, $D5, $D5, $EA, $EA, $FF, $FF, $00, $00, $55, $55, $AA, $AA, $00, $00, $00, $00, $00, $00, $30, $C0, $00, $00, $00, $3C, $3C, $00, $00, $00, $00, $00, $00, $00, $00, $00, $C0, $C0, $03, $03, $0C, $0C, $30, $30, $C0, $C0, $00, $30, $CC, $CC, $CC, $CC, $CC, $30, $00, $30, $30, $30, $30, $30, $30, $30, $00, $30, $CC, $0C, $30, $C0, $C0, $FC, $00, $30, $CC, $0C, $30, $0C, $CC, $30, $00, $0C, $CC, $CC, $FC, $0C, $0C, $0C, $00, $FC, $C0, $F0, $0C, $0C, $CC, $30, $00, $30, $CC, $C0, $F0, $CC, $CC, $30, $00, $FC, $0C, $0C, $30, $30, $30, $30, $00, $30, $CC, $CC, $30, $CC, $CC, $30, $00, $FC, $CC, $CC, $FC, $0C, $0C, $0C, $00, $00, $30, $30, $00, $30, $30, $00, $00, $00, $30, $30, $00, $30, $C0, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $3C, $00, $3C, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $00, $30, $FC, $CC, $FC, $CC, $CC, $CC, $00, $F0, $CC, $CC, $F0, $CC, $CC, $F0, $00, $30, $CC, $C0, $C0, $C0, $CC, $30, $00, $F0, $CC, $CC, $CC, $CC, $CC, $F0, $00, $FC, $C0, $C0, $F0, $C0, $C0, $FC, $00, $FC, $C0, $C0, $F0, $C0, $C0, $C0, $00, $30, $CC, $C0, $FC, $CC, $CC, $30, $00, $CC, $CC, $CC, $FC, $CC, $CC, $CC, $00, $FC, $30, $30, $30, $30, $30, $FC, $00, $FC, $0C, $0C, $0C, $CC, $CC, $30, $00, $CC, $CC, $CC, $F0, $CC, $CC, $CC, $00, $C0, $C0, $C0, $C0, $C0, $C0, $FC, $00, $CC, $FC, $FC, $CC, $CC, $CC, $CC, $00, $00, $00, $C0, $FC, $CC, $CC, $CC, $00, $FC, $CC, $CC, $CC, $CC, $CC, $FC, $00, $F0, $CC, $CC, $F0, $C0, $C0, $C0, $00, $30, $CC, $CC, $CC, $CC, $F0, $3C, $00, $F0, $CC, $CC, $F0, $CC, $CC, $CC, $00, $30, $CC, $C0, $30, $0C, $CC, $30, $00, $FC, $30, $30, $30, $30, $30, $30, $00, $CC, $CC, $CC, $CC, $CC, $CC, $FC, $00, $CC, $CC, $CC, $CC, $30, $30, $30, $00, $CC, $CC, $CC, $FC, $FC, $FC, $CC, $00, $CC, $CC, $CC, $30, $CC, $CC, $CC, $00, $CC, $CC, $CC, $3C, $0C, $CC, $30, $00, $FC, $0C, $0C, $30, $C0, $C0, $FC, $00, $00, $00, $00, $00, $00, $0F, $0F, $00, $00, $00, $00, $00, $00, $00, $00, $C0, $C0, $C0, $C0, $C0, $C0, $FF, $FF, $03, $00, $03, $03, $03, $03, $FF, $FF, $00, $00, $00, $00, $00, $00, $F0, $F0, $00, $00, $00, $00, $FF, $00, $00, $00
LBL1L5: .byte $00, $34, $DD, $DD, $DD, $DD, $DD, $34, $00, $34, $34, $34, $34, $34, $34, $34, $00, $34, $DD, $0D, $34, $D0, $D0, $FD, $00, $FD, $0D, $34, $0D, $DD, $DD, $34, $00, $0D, $DD, $DD, $DD, $FD, $0D, $0D, $00, $FD, $D0, $F4, $0D, $0D, $DD, $34, $00, $34, $DD, $D0, $F4, $DD, $DD, $34, $00, $FD, $0D, $0D, $34, $34, $34, $34, $00, $34, $DD, $DD, $30, $DD, $DD, $34, $00, $34, $DD, $DD, $3D, $0D, $DD, $34, $00, $00, $00, $00, $00, $00, $00, $00
LBL1L98: .byte $00, $0A, $00, $1E, $00, $0A, $08, $00, $00, $1E, $08, $28, $08, $00, $10, $00, $08, $28, $10, $28, $10, $00, $18, $0A, $10, $28, $18, $1E, $20, $1E, $28, $0A, $24, $14, $2D, $14, $28, $0A, $30, $1E, $38, $0A, $38, $1E, $38, $1E, $40, $1E, $48, $0A, $48, $1E, $48, $1E, $50, $1E, $58, $0A, $58, $1F, $60, $0A, $60, $14, $60, $0A, $68, $0A, $60, $14, $68, $14, $60, $1E, $68, $1E, $68, $1F, $68, $14, $70, $0A, $80, $0A, $78, $0A, $78, $1F, $88, $0A, $88, $1E, $88, $0A, $90, $00, $88, $1E, $90, $28, $90, $00, $98, $00, $90, $28, $98, $28, $98, $00, $9F, $0A, $98, $28, $9F, $1E, $9F, $0A, $9F, $1E
LBL1L109: .byte $00, $7E, $00, $07, $81, $E0, $08, $7E, $10, $17, $81, $E8, $28, $00, $14, $28, $00, $14, $28, $00, $14, $28, $00, $14, $17, $81, $E8, $08, $7E, $10, $07, $FF, $E0, $02, $96, $40, $02, $96, $40, $02, $D3, $40, $02, $D3, $40, $07, $FF, $E0, $0C, $18, $30, $18, $18, $18, $30, $18, $0C, $B4, $99, $2D, $78, $7E, $1E, $00
LBL1L110: .byte $00, $7E, $00, $07, $81, $E0, $08, $7E, $10, $17, $81, $E8, $28, $00, $14, $28, $00, $14, $28, $00, $14, $28, $00, $14, $17, $81, $E8, $08, $7E, $10, $07, $FF, $E0, $02, $96, $44, $02, $96, $46, $02, $D3, $47, $02, $D3, $46, $07, $FF, $E4, $0C, $18, $30, $18, $18, $18, $30, $18, $0C, $B4, $99, $2D, $78, $7E, $1E, $00
LBL1L111: .byte $00, $7E, $00, $07, $81, $E0, $08, $7E, $10, $17, $81, $E8, $28, $00, $14, $28, $00, $14, $28, $00, $14, $28, $00, $14, $17, $81, $E8, $08, $7E, $10, $07, $FF, $E0, $12, $96, $40, $32, $96, $40, $72, $D3, $40, $32, $D3, $40, $17, $FF, $E0, $0C, $18, $30, $18, $18, $18, $30, $18, $0C, $B4, $99, $2D, $78, $7E, $1E, $00
LBL1L112: .byte $00, $7E, $00, $07, $81, $E0, $08, $7E, $10, $17, $81, $E8, $28, $00, $14, $28, $00, $14, $28, $00, $14, $28, $00, $14, $17, $81, $E8, $08, $7E, $10, $07, $FF, $E0, $02, $96, $40, $02, $96, $40, $02, $D3, $40, $02, $D3, $40, $07, $FF, $E0, $0C, $18, $30, $18, $18, $18, $30, $FF, $0C, $B4, $FF, $2D, $79, $FF, $9E, $00
!arg0:	.byte $00
!arg1:	.byte $00
!arg2:	.byte $00

	// clearmem( PG, # or loops );
clearmem:
	pla 
	tax 
	pla 
	tay

	pla
	sta !arg1-

	// arg0
	pla
	sta $03

	tya 
	pha 
	txa 
	pha 

	ldx !arg1-
	
	lda #$00
	sta $02
	tay
	//ldy #$00
!:
	sta ($02),Y
	iny
	bne !-
	inc $03
	dex
	bne !-

	rts


	// fillmem( PG, char, # or loops );
fillmem:
	pla 
	tax 
	pla 
	tay

	pla
	sta !arg2-

	pla
	sta !arg1-
	
	// arg0
	pla
	sta $03

	tya 
	pha 
	txa 
	pha 

	ldx !arg2-
	
	ldy #$00
	sty $02
	
	lda !arg1-
!:
	sta ($02),Y
	iny
	bne !-
	inc $03
	dex
	bne !-

	rts


clearsid:	
	lda #$D4
	sta $03	
	lda #$18
	sta $02	
	ldx #$18
	lda #$00
	tay
!:
	sta ($02),Y
	dex
	bne !-
	rts
// store the random values here
!mem0:  .byte $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00
	.byte $00, $00, $00, $00, $00, $00, $00, $00

!mem1:  .byte $00, $00 // ptr to rnd values
!mem2:  .byte $00 // flag to tell if the init process has already been run (rndinit)
!mem3:  .byte $00 // rvindex
!mem4:	.byte $00 // mySeedi
!mem5:	.byte $00 // my random return value
mySeed:	
	lda !mem2- // rndinit
	//cmp #$00
	bne !+++++

	// SID RNG
	lda #$FF
	sta $D40E
	sta $D40F
	lda #$80
	sta $D412
	
	lda #$00
	sta !mem3-
	sta !mem4-

	lda #<!mem0-
	sta !mem1-
	lda #>!mem0-
	sta !mem1- +1
	
	//lda #$00
	//sta !mem4-
!:			 // Top of FOR Loop
	lda !mem4-
	cmp #$40
	bcs !+++

	clc 
	lda !mem1-
	adc !mem4-
	sta !+
	lda #$00
	adc !mem1- +1
	sta !++
	lda $D41B
	tay 
	.byte $8C	 // <-- STY absolute
!:	.byte $00
!:	.byte $00
	jsr shortDelay
	inc !mem4-
	jmp !---
!:	lda #$01
	sta !mem2-
!:	rts

	
myRand:	clc 
	lda !mem1- // ptr (LB) to random values
	
	adc !mem3- // rvindex
	tay
	
	lda #$00
	adc !mem1- +1  // ptr (HB) to rnd values
	tax
		
	lda $02
	pha 
	lda $03
	pha 

	sty $02
	ldy #$00
	stx $03
	lda ($02),Y

	sta !mem5- // the return value
	
	pla 
	sta $03
	pla 
	sta $02

	inc !mem3- // bump the index up by 1

// top of if
	lda !mem3-  // rvindex
	cmp #$40
	bne !+
	lda #$00
	sta !mem3-
!:	pla 
	tax 
	pla 
	tay 
	lda !mem5-
	pha 
	//lda #$01
	//pha 
	tya 
	pha 
	txa 
	pha 
	rts 


!mem0:	.byte $00, $00
shortDelay:
	lda #$00
	sta !mem0-
	sta !mem0- +1
	
!:	lda !mem0- +1
	cmp #$01
	bne !+
	lda !mem0-
	cmp #$FF
!:	bcs !+

	//nop

	clc 
	lda !mem0-
	adc #$01
	sta !mem0-
	lda !mem0- +1
	adc #$00
	sta !mem0- +1
	jmp !--
!:	rts
	
saveregs:
	lda $D011
	sta !reg1+
	lda $D020
	sta !reg2+
	lda $D021
	sta !reg3+
	lda $D016
	sta !reg4+
	lda $D018
	sta !reg5+
	lda $0286
	sta !reg6+
	rts
	
!reg1:	.byte $00
!reg2:	.byte $00
!reg3:	.byte $00
!reg4:	.byte $00
!reg5:	.byte $00
!reg6:	.byte $00	

restoreregs:
	lda !reg1-
	sta $D011
	lda !reg2-
	sta $D020
	lda !reg3-
	sta $D021
	lda !reg4-
	sta $D016
	lda !reg5-
	sta $D018
	lda !reg6-
	sta $0286
	rts 
!mem:	.byte $00
pause:	
	lda #$00
	sta $C6
	jsr $FFE4
	sta !mem-
!:
	lda !mem- // this $00 will change
//	cmp #$00
	bne !+
	jsr $FFE4
	sta !mem-
	jmp !-
!:	rts 
 //  Variable Labels

!lv_colour:	.byte $00 // colour
!lv_y2:		.byte $00 // y2
!lv_x2:		.byte $00 // x2
!lv_y1:		.byte $00 // y1
!lv_x1:		.byte $00 // x1

!lv_dy:		.byte $00, $00// dy
!lv_dx:		.byte $00, $00 // dx

!lv_Z1:		.byte $00, $00 // Z1

!lv_Zy:		.byte $00, $00 // Zy
!lv_Zx:		.byte $00, $00 // Zx

!lv_CASE:	.byte $00 // CASE

!lv_X:		.byte $00 // X
!lv_Y:		.byte $00 // Y
!lv_i:		.byte $00 // i

segment:
	pla 
	tax 
	pla 
	tay 
	pla
	
	sta !lv_colour-
	pla 
	sta !lv_y2-
	pla 
	sta !lv_x2-
	pla 
	sta !lv_y1-
	pla 
	sta !lv_x1-
	
	tya 
	pha 
	txa 
	pha 
// ************************************
	lda #$04
	sta !lv_CASE-
	
// Top of IF statement
	lda !lv_y1-
	cmp !lv_y2-
	bcc !+
	beq !+
// &&
	lda !lv_x1-
	cmp !lv_x2-
	bcc !+ 

	lda #$01
	sta !lv_CASE-
!:
	
// Top of IF statement
	lda !lv_y1-
	cmp !lv_y2-
	bcc !+
	beq !+
	jmp !++
!:
	
// Top of IF statement
	lda !lv_x1-
	cmp !lv_x2-
	bcc !+ // if c==0 jump to ELSE
	beq !+ // if z==1 jump to ELSE ()
	
	lda #$02
	sta !lv_CASE-
!:
// &&
	lda !lv_y1-
	cmp !lv_y2-
	bcc !+
	beq !+

	lda !lv_x1-
	cmp !lv_x2-
	bcs !+ // jump to ELSE ()
	
	lda #$03
	sta !lv_CASE-
!:

	
// Top of IF statement
	lda !lv_CASE-
	cmp #$01
	beq !+
// ||
	lda !lv_CASE-
	cmp #$02
	bne !++ // jump to ELSE ()
!:
// swap (x1,y1) and (x2,y2)
	ldx !lv_x1- // 4 cy
	ldy !lv_x2- // 4 cy
	stx !lv_x2- // 4 cy
	sty !lv_x1- // 4 cy
	ldx !lv_y1- // 4 cy
	ldy !lv_y2- // 4 cy
	stx !lv_y2- // 4 cy
	sty !lv_y1- // 4 cy
!:

	

// Top of IF statement
	lda !lv_CASE-
	cmp #$01
	beq !+
// ||
	lda !lv_CASE-
	cmp #$04
	bne !++ // jump to ELSE ()
!:
	sec 
	lda !lv_x2-
	sbc !lv_x1-
	sta !lv_dx-
	lda #$00
	sta !lv_dx-+1
	
	// moved to here from below
	sta !lv_dy-+1
	
	sec 
	lda !lv_y2-
	sbc !lv_y1-
	sta !lv_dy-
	//lda #$00
	//sta !lv_dy-+1
!:


	
// Top of IF statement
	lda !lv_CASE-
	cmp #$02
	beq !+
// ||
	lda !lv_CASE-
	cmp #$03
	bne !++ // jump to ELSE ()
!:
	sec 
	lda !lv_x2-
	sbc !lv_x1-
	sta !lv_dx-
	lda #$00
	sta !lv_dx-+1
	// moved to here from below
	sta !lv_dy-+1
	sec 
	lda !lv_y1-
	sbc !lv_y2-
	sta !lv_dy-

	//lda #$00
	//sta !lv_dy-+1
	
	lda #$03
	sta !lv_CASE-
!:
	lda !lv_x1-
	sta !lv_X-

	lda !lv_y1-
	sta !lv_Y-

// Top of IF statement   if( x2 == x1 )
	lda !lv_x2-
	cmp !lv_x1-
	beq !+
	jmp !++++ // jump to ELSE (CANT OPTIMIZE)
!:

	// Plot a Vertical Line from (x1,y1) to (x1,y2)
	lda !lv_y1-
	sta !lv_i-
!:
// Top of FOR Loop
	lda !lv_i-
	cmp !lv_y2-
	
	beq !+ // jump out of FOR () and then to rts
	
	lda !lv_X-
#if SAFEMCPLOT	
	sei
#endif
	sta $FA	
	lda !lv_i-
	sta $FC
	lda !lv_colour-
	sta $FD
	jsr MCPLOT
#if SAFEMCPLOT	
	cli
#endif
	inc !lv_i-

	jmp !-
!:
	rts
!:


// Top of IF statement if( y2 == y1 )
	lda !lv_y2-
	cmp !lv_y1-
	beq !+
	jmp !++++ // jump to ELSE (CANNOT OPTIMIZE)
!:
	// Plot a Horiz Line from (x1,y1) to (x2,y1)
	lda !lv_x1-
	sta !lv_i-
!:			 // Top of FOR Loop
	lda !lv_i-
	cmp !lv_x2-
	
	beq !+ // jump out of FOR () and on to rts
	
	lda !lv_i-
#if SAFEMCPLOT	
	sei
#endif
	sta $FA

	lda !lv_Y-
	sta $FC

	lda !lv_colour-
	sta $FD

	jsr MCPLOT
#if SAFEMCPLOT	
	cli
#endif

	inc !lv_i-

	jmp !-
!:
	rts
!:

// Top of IF statement
	lda !lv_dx-+1
	cmp !lv_dy-+1
	bne !+
	lda !lv_dx-
	cmp !lv_dy-
!:
	bcc !_skip+
	jmp !+++++ // jump to ELSE (CANNOT OPTIMIZE)
!_skip:

// Top of IF statement (if CASE == 3)
	lda !lv_CASE-
	cmp #$03
	bne !+++ // jump to ELSE ()

	lda !lv_y1-
	sta !lv_i-
!:			 // Top of FOR Loop
	lda !lv_i-
	cmp !lv_y2-
	beq !+
	
	sec 
	lda !lv_y1-
	sbc !lv_i-
	
	//sta !lv_Z1-
	sta _MUL16_FB

	ldx #$00
	stx _MUL16_FC

	//sta _MUL16_FB
	lda !lv_dx-
	sta _MUL16_FD
	lda !lv_dx-+1
	sta _MUL16_FE
	jsr MUL16
	
	lda MUL16R
	ldx MUL16R+1
	sta !lv_Zx-
	sta _DIV16_FB
	
	stx _DIV16_FC
	lda !lv_dy-
	sta _DIV16_FD
	lda !lv_dy-+1
	sta _DIV16_FE
	jsr DIV16
	
	lda _DIV16_FB
	
	clc 
	adc !lv_x1-
#if SAFEMCPLOT	
	sei
#endif
	sta $FA
	lda !lv_i-
	sta $FC
	lda !lv_colour-
	sta $FD
	jsr MCPLOT
#if SAFEMCPLOT	
	cli
#endif

	
	dec !lv_i-
	jmp !-
!:
	rts
	
	// if (dx < dy )
	//  and CASE != 3
!:
	lda !lv_y1-
	sta !lv_i-
!:			 // Top of FOR Loop
	lda !lv_i-
	cmp !lv_y2-	
	beq !+ // jump out of FOR ()
	
	sec 
	lda !lv_i-
	sbc !lv_y1-
	sta !lv_Z1-
	lda #$00
	sta !lv_Z1-+1
	sta _MUL16_FC
	
	lda !lv_Z1-
	sta _MUL16_FB
	
	lda !lv_dx-
	sta _MUL16_FD
	lda !lv_dx-+1
	sta _MUL16_FE
	jsr MUL16
	
	lda MUL16R
	ldx MUL16R+1
	sta !lv_Zx-
	stx !lv_Zx- +1

	sta _DIV16_FB
	stx _DIV16_FC
	
	lda !lv_dy-
	sta _DIV16_FD
	lda !lv_dy- +1
	sta _DIV16_FE
	jsr DIV16
	
	lda _DIV16_FB
	//ldx _DIV16_FC
	
	sta !lv_Zx-
	clc 
	adc !lv_x1-
#if SAFEMCPLOT	
	sei
#endif
	sta $FA
	lda !lv_i-
	sta $FC
	lda !lv_colour-
	sta $FD
	jsr MCPLOT
#if SAFEMCPLOT	
	cli
#endif
	inc !lv_i-
	jmp !-
	
!:

// Top of IF statement
	lda !lv_CASE-
	cmp #$03
	bne !+++ // jump to ELSE 
	lda !lv_x2-
	sta !lv_i-

!:			 // Top of FOR Loop
	lda !lv_i-
	cmp !lv_x1-
	beq !+ // jump out of FOR ()
	sec 
	lda !lv_x2-
	sbc !lv_i-
	sta !lv_Z1-
	sta _MUL16_FB
	lda #$00
	sta _MUL16_FC
	lda !lv_dy-
	sta _MUL16_FD
	lda !lv_dy-+1
	sta _MUL16_FE
	jsr MUL16
	
	lda MUL16R
	ldx MUL16R+1
	//sta !lv_Zy-
	//stx !lv_Zy-+1

	sta _DIV16_FB
	stx _DIV16_FC
	lda !lv_dx-
	sta _DIV16_FD
	lda !lv_dx-+1
	sta _DIV16_FE
	jsr DIV16

	
	lda _DIV16_FB
	//ldx _DIV16_FC

	// not sure is THIS is needed!
	//sta !lv_Zy-
	//stx !lv_Zy- +1
	
	clc 
	//lda !lv_Zy-
	adc !lv_y2-
	//sta !lv_Y-
#if SAFEMCPLOT	
	sei
#endif
	sta $FC

	lda !lv_i-
	sta $FA
	//lda !lv_Y-
	//sta $FC
	lda !lv_colour-
	sta $FD
	jsr MCPLOT
#if SAFEMCPLOT	
	cli
#endif
	dec !lv_i-
	jmp !-
!:
	rts
	
!:


	lda !lv_x1-
	sta !lv_i-
!:			 // Top of FOR Loop
	lda !lv_i-
	cmp !lv_x2-
	beq !--- // jump out of FOR ()
	sec 
	lda !lv_i-
	sbc !lv_x1-
	sta !lv_Z1-
	sta _MUL16_FB

	lda #$00
	sta !lv_Z1-+1
	sta _MUL16_FC
	
	lda !lv_dy-
	sta _MUL16_FD
	lda !lv_dy-+1
	sta _MUL16_FE
	jsr MUL16
	
	lda MUL16R
	ldx MUL16R+1

	sta !lv_Zy-
	sta _DIV16_FB
	stx !lv_Zy-+1
	stx _DIV16_FC

	lda !lv_dx-
	sta _DIV16_FD
	lda !lv_dx-+1
	sta _DIV16_FE
	jsr DIV16
	
	lda _DIV16_FB
	clc 
	adc !lv_y1-

	// not sure this next line is needed
	//sta !lv_Y-
	
#if SAFEMCPLOT	
	sei
#endif
	sta $FC
	lda !lv_i-
	sta $FA
	
	lda !lv_colour-
	sta $FD
	jsr MCPLOT
#if SAFEMCPLOT	
	cli
#endif
	inc !lv_i-
	jmp !-
!:
	rts

 // x = $FA, y = $FC, colour = $FD
 // STORE is at $FE(l), $FF(h)
 // LOC is at $02(l), $03(h)
mcplot:
	pla
	tax
	pla
	tay
	pla
	sta $FD
	pla
	sta $FC
	pla
	sta $FA
	tya
	pha
	txa
	pha
MCPLOT:	
	lda $FA // xcoord
	and #$03
	sta $FE // store
	lda #$00
	sta $02 // loc
	sta $FF // store + 1
	lda $FD // colcode
	clc 
	ror 
!:
	ror 
	ror 
	dec $FE // store
	bpl !-
	sta $50 // mask
	lda $FA // xcoord
	and #$FC
	asl 
	rol $FF // store + 1
	sta $FE // store
	lda $FC // ycoord
	lsr 
	lsr 
	lsr 
	sta $03 // loc + 1
	lsr 
	ror $02 // loc
	lsr 
	ror $02 // loc
	clc 
	adc $03 // loc + 1
	sta $03 // loc + 1
	lda $FC // ycoord
	and #$07
	adc $02 // loc
	adc $FE // store 
	sta $02 // loc
	lda $03 // loc+1
	adc $FF // store + 1	
	clc
	
	adc #$A0

	sta $03 // loc + 1
	ldy #$00
	lda ($02),Y
	ora $50 // mask
	sta ($02),Y // what is @ $50?
	rts 
// needed if plotting a point
//indexToBit: .byte 128,64,32,16,8,4,2,1
cell_addr_hi:	.byte $A0,$A1,$A2,$A3,$A5,$A6,$A7,$A8,$Aa,$Ab,$Ac,$Ad,$Af,$B0,$B1,$B2,$B4,$B5,$B6,$B7,$B9,$Ba,$Bb,$Bc,$Be
cell_addr_lo:	.byte $00,$40,$80,$c0,$00,$40,$80,$c0,$00,$40,$80,$c0,$00,$40,$80,$c0,$00,$40,$80,$c0,$00,$40,$80,$c0,$00

!lv_arg0: .byte $00 // y
!lv_arg1: .byte $00, $00
!lv_addr_hi: .byte $00
!lv_addr_lo: .byte $00
	
getpixel:	
	pla
	tax 
	pla 
	tay
	
	pla 
	sta !lv_arg0-
	pla 
	sta !lv_arg1- +1
	pla 
	sta !lv_arg1-
	tya 
	pha
	txa 
	pha

	///

	lda !lv_arg0-
	lsr
	lsr
	lsr
	tax            //  x = Y/8	
	lda !lv_arg1-
	and #$f8       // x /8 * 8 = clear lower 3 bits to 0
	clc
	adc cell_addr_lo,x // (Y/8)*320
	sta !lv_addr_lo-     // so this is BASE+Y/8*320+((X/8)*8)
	lda cell_addr_hi,x
	adc !lv_arg1- +1 // xhi byte
	sta !lv_addr_hi-  // and the upper 8 bits
	//lda !lv_arg1-
	//and #$07
	//tax            // get the Bit to set from X
	lda !lv_arg0-
	and #$07         // get the Line we want to set
	adc !lv_addr_lo-
	sta !lv_addr_lo-
	//tay
	//lda (ADDRESS),y   ; read value
	//ora indexToBit,x ;or with x index to bit
	//sta (zpTemp),y   ; store it back


	pla 
	tax 
	pla 
	tay 

	lda !lv_addr_lo-
	pha
	lda !lv_addr_hi-
	pha
	lda #$02
	pha
	
	tya 
	pha 
	txa 
	pha 

	rts
// $0E3A
!ARG0:  .byte $00
!ARG1:  .byte $00
!ARG2:  .byte $00, $00
!ARG3:  .byte $00
!ARG4:  .byte $00
!MEM0:	.byte $00      // plotshapeColourValue11
!MEM1:	.byte $00      // plotshapeColourValue0110	
!MEM2:  .byte $00, $00 // plotshapeColor1
!MEM3:  .byte $00, $00 // plotshapeOffset
!MEM4:  .byte $00, $00 // newPrintMEM4: which character to print
!MEM5:  .byte $00, $00 // plotshapeColors2And3
!MEM6:  .byte $00      // plotshapeBindex
!MEM7:  .byte $00      // plotshapeI	
!MEM8:  .byte $00, $00 // plotshapePixels
//!MEM9:  .byte $00, $00 // pointer to character data
// bmPrintInit( BMhi, CLRhi, ptr to chardata )
bmPrintInit:
	pla 
	tax 
	pla 
	tay 

	pla
	sta !CHR++ +1
	pla
	sta !CHR+ +1
	pla
	sta !CLRDTA+ +1
	pla
	sta !BMPDATA+ +1
	
	tya
	pha
	txa
	pha
	rts
	
bmPrint:
	pla 
	tax 
	pla 
	tay 
	pla 
	sta !ARG4-
	pla 
	sta !ARG3-
	pla 
	sta !ARG2- +1
	pla 
	sta !ARG2- 
	pla 
	sta !ARG1-
	pla 
	sta !ARG0-
	tya 
	pha 
	txa 
	pha 
	lda !ARG3-
	sta !MEM0-
	lda !ARG4-
	sta !MEM1-
	ldy #$00
	lda !ARG2- 
	ldx !ARG2- +1
	sta !+
	stx !++
	.byte $AD
!:	.byte $00
!:	.byte $00
	ldx #$00
	sta !MEM4- 
	stx !MEM4- +1
	lda $02
	pha 
	lda $03
	pha 
	php
	
!START:			 // Top of WHILE Loop
	lda !MEM4- +1
	//cmp #$00
	bne !+
	lda !MEM4- 
	//cmp #$00
!:	bne !_skip+
	jmp !END+
!_skip:
	lda $02
	pha 
	lda $03
	pha 
// ----------------------
	sec 
	lda !MEM4- 
	sbc #$20
	sta $02
	lda !MEM4- +1
	sta $03
	asl $02
	rol $03
	asl $02
	rol $03
	asl $02		
	rol $03
	ldx $03
	ldy $02
// -------------------
	pla 
	sta $03
	pla 
	sta $02
	tya 	
	//----------------
	sta !MEM4- 
	stx !MEM4- +1

	clc 
!CHR:	lda #$00
	adc !MEM4- 
	sta !MEM4- 
!CHR:	lda #$00
	adc !MEM4- +1
	sta !MEM4- +1
	
	lda #$00
	sta !MEM6-

	lda $02
	pha 
	lda $03
	pha 
	lda $04
	pha 
	lda $05
	pha 
// ----------------------
	lda !ARG1-
	sta $02
	sta $04
	lda #$00
	sta $03
	sta $05
	asl $02
	rol $03
	asl $02
	rol $03
	asl $02
	rol $03
	asl $04
	rol $05
	clc 
	lda $02
	adc $04
	sta $02
	lda $03
	adc $05
	sta $03
	asl $02
	rol $03
	asl $02
	rol $03
	ldx $03
	ldy $02
// -------------------
	pla 
	sta $05
	pla 
	sta $04
	pla 
	sta $03
	pla 
	sta $02
	tya 
	//========================

	clc 
	adc !ARG0-
	sta !MEM3-
	sta !MEM2- 
	txa 
	adc #$00
	sta !MEM3- +1
	
	clc
	lda !MEM3- +1
	adc #$D8
	sta !MEM2- +1
	
	clc 
	lda !MEM3-
	sta !MEM5- 
	lda !MEM3- +1
!CLRDTA:
	adc #$84
	sta !MEM5- +1
	
	lda !MEM3-
	asl 
	tay 
	lda !MEM3- +1
	rol 
	tax 
	tya 
	asl 
	tay 
	txa 
	rol 
	tax 
	tya 
	asl 
	sta !MEM8- 
	txa 
	rol 
	
	clc

	// this can likely be optimised
!BMPDATA:
	adc #$A0
	sta !MEM8- +1
	
	lda !MEM2- 
	sta !+
	lda !MEM2- +1
	sta !++
	lda !ARG3-
	.byte $8D	 // STA absolute
!:	.byte $00
!:	.byte $00
	lda !MEM5- 
	sta !+
	lda !MEM5- +1
	sta !++
	lda !ARG4-
	.byte $8D	 // STA absolute
!:	.byte $00
!:	.byte $00

	lda #$00
	sta !MEM7- 

!:	lda !MEM7- 
	cmp #$08
	
	bcs !+++++ // jump out of FOR
	lda !MEM4- 
	sta !+
	lda !MEM4- +1
	sta !++
	ldx !MEM6- 
	.byte $BD // <-- LDA ABS,X
!:	.byte $00
!:	.byte $00
	ldx !MEM8- 
	stx !+
	ldx !MEM8- +1
	stx !++
	.byte $8D // <-- STA absolute
!:	.byte $00
!:	.byte $00
	
	clc 
	lda !MEM8- 
	adc #$01
	sta !MEM8- 
	lda !MEM8- +1
	adc #$00
	sta !MEM8- +1
	
	inc !MEM6- 
	inc !MEM7- 
	jmp !-----
	
!:
	inc !ARG0-
	
	clc 
	lda !ARG2- 
	adc #$01
	sta !ARG2-
	sta !+
	lda !ARG2- +1
	adc #$00
	sta !ARG2- +1
	sta !++

	ldy #$00
	.byte $AD
!:	.byte $00
!:	.byte $00
	sta !MEM4-
	
	lda #$00
	sta !MEM4- +1
	jmp !START- // jump to top of WHILE loop
	
!END:
	plp 
	pla 
	sta $03
	pla 
	sta $02
	
	rts

!mem0:   .byte $00
setScreenMode:
	pla 
	tax 
	pla
	tay
	
	pla 
	sta !mem0-
	
	tya 
	pha 
	txa 
	pha
	
	lda !mem0-
	//cmp #$00
	bne !+
	
	lda #$BF
	and $D011
	sta !mem0-
	lda #$20
	ora !mem0-
	sta $D011
	
	lda #$EF
	and $D016
	sta $D016
	rts
!:

	lda !mem0-
	cmp #$01
	bne !+
	
	lda #$9F
	and $D011
	sta $D011
	lda #$10
	ora $D016
	sta $D016
	rts
!:

	lda !mem0-
	cmp #$02
	bne !+
	
	lda #$BF
	and $D011
	sta !mem0-
	lda #$20
	ora !mem0-
	sta $D011
	lda #$EF
	and $D016
	sta $D016
	rts
!:

	// multicolour bitmode (3)
	lda !mem0-
	cmp #$03
	bne !+
	lda #$BF  // clear bit 6
	and $D011
	sta !mem0-
	lda #$20  // set bit 5
	ora !mem0-
	sta $D011
	
	lda #$10 // set bit 4
	ora $D016
	sta $D016
	rts
!:

	lda !mem0-
	cmp #$04
	bne !+
	
	lda #$DF
	and $D011
	sta !mem0-
	lda #$40
	ora !mem0-
	sta $D011
	lda #$EF
	and $D016
	sta $D016
!:
	rts
	// returns the offset in screen or colour ($D800) memory
	// that is where the x, y coordinate is
!lv_arg0: .byte $00, $00
!lv_arg1: .byte $00
getOffset:
	pla
	tax
	pla
	tay
	pla
	sta !lv_arg1-
	pla
	sta !lv_arg0- +1
	pla
	sta !lv_arg0-
	tya
	pha
	txa
	pha

	clc
	lda #$04
	adc !lv_arg0- 
	sta !lv_arg0-
	lda #$00
	adc !lv_arg0- +1
	sta !lv_arg0- +1
	
	lsr !lv_arg0- +1
	ror !lv_arg0-
	lsr !lv_arg0- +1
	ror !lv_arg0-
	lsr !lv_arg0- +1
	ror !lv_arg0-
	
	sec
	lda !lv_arg0-
	sbc #$02
	sta !lv_arg0-
	lda !lv_arg0- +1
	sbc #$00
	sta !lv_arg0- +1
	
	lsr !lv_arg1-
	lsr !lv_arg1-
	lsr !lv_arg1-
	
	sec
	lda !lv_arg1-
	sbc #$04

	sta !lv_arg1-
	//lda !lv_arg1-

	
	sta $02
	sta $04
	lda #$00
	sta $03
	sta $05
	asl $02
	rol $03
	asl $02
	rol $03
	asl $02
	rol $03
	asl $04
	rol $05
	
	clc
	lda $02
	adc $04
	sta $02
	lda $03
	adc $05
	sta $03
	asl $02
	rol $03
	asl $02
	rol $03
	lda $03


	clc
	lda $02
	adc !lv_arg0-
	sta $02
	lda $03
	adc !lv_arg0- +1
	sta $03
	
	pla
	tax
	pla
	tay
	lda $02
	pha
	lda $03
	pha
	lda #$02
	pha
	tya
	pha
	txa
	pha
	rts
