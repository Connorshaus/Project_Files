.data
	.word 1,2
.text
	lw $1,0($0)
	lw $2,4($0)
	sw $2,8($0)
	add $3,$1,$2
	sub $4,$3,$1
	add $5,$3,$4
	sw $5,12($0)
	halt $0,$0,$0
