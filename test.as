.define sz = 2
MAIN:				   mov		    r3     ,    LIST[sz]     
LOOP:	jmp 	L1  
		prn		#-5
		mov		r1,	STR[sz]
	    inc	        			#sz
		sub		r1, r4
		cmp		r3, #sz
		bne		END
L1:		inc		K
		bne		LOOP
END:	stop
.define len = 4
STR:	.string "abcdef"
LIST:	.data 6, -9, len
K:		.data 22		
