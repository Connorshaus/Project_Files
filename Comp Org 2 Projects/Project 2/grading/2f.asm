.data
	.word 1,15,3
.text
	lw $1,0($0)
	lw $2,4($0)
	noop $0,$0,$0
	noop $0,$0,$0
	beq $1,$2,-2
	add $3,$2,$1
	sub $4,$2,$1
	noop $0,$0,$0
	beq $3,$4,12
	noop $0,$0,$0
	noop $0,$0,$0
	noop $0,$0,$0
	halt $0,$0,$0 
