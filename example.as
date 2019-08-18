.extern W
.extern L3
.entry LIST
.define sz = 2
MAIN:	mov	r3, LIST[sz]
LOOP:	jmp	W
		prn #-5
		mov	STR[5], STR[2]
		bne W
L1:		inc L3
.entry LOOP
		bne LOOP
END:	stop
.define len = 4
STR:	.string "abcdef"
LIST:	.data 6, -9, len
