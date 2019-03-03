.data
	.word 1,2,3,4,2
.text
	lw $3,8($0)
	lw $2,4($0)
	lw $1,0($0)
	sub $3,$2,$1  
	beq $1,$3,1
	add $4,$3,$1
	sub $2,$2,$1  
	beq $2,$1,-2
	noop $0,$0,$0
	noop $0,$0,$0
	noop $0,$0,$0
	halt $0,$0,$0
