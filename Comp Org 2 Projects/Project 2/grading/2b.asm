.data
	.word 1,15,3
.text
	lw $1,0($0)
	lw $2,4($0)
	lw $3,8($0)
	add $3,$2,$1
	lw $4,12($0)
	sub $3,$2,$1
	add $5,$4,$3
	add $6,$5,$5
	halt $0,$0,$0 
