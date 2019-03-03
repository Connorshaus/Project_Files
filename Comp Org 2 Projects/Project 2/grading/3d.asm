.data
	.word 1,2
.text
	lw $1,0($0)
	lw $2,4($0)
	add $3,$2,$1
	beq $0,$3,2
	noop $0,$0,$0
	noop $0,$0,$0
	sub $4,$2,$1
	lw $3,8($0)
	beq $3,$4,2
	noop $0,$0,$0
	noop $0,$0,$0
	noop $0,$0,$0
	halt $0,$0,$0 
