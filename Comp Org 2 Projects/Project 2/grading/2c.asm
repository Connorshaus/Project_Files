.data
	.word 1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8
.text
	lw $0,8($1)
	add $2,$0,$0
	sub $0,$2,$0
	add $3,$1,$0
	sub $0,$3,$2
	sw $0,16($1)
	halt $0,$0,$0
