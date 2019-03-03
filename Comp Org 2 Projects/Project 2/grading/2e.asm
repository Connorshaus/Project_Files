.data
	.word 1,2
.text
	lw $1,0($0)
	add $1,$2,$3
	sw $1,12($0)
	lw $2,4($0)
	noop $0,$0,$0
	add $3,$1,$2
	sub $3,$1,$4
	sw $3,8($0)
	halt $0,$0,$0
