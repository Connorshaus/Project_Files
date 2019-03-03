.data
	.word 1,2,3
.text
	lw $2,4($0)
	lw $2,4($0)
	lw $3,8($0)
	halt $0,$0,$0
