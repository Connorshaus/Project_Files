.data
	.word 1,2,3,4,2
.text
	lw $6,16($0)
	add $1,$6,$0  
	lw $5,0($0)   
	sub $6,$1,$5  
	beq $6,$5,-4
	add $3,$1,$0  
	beq $3,$3,2
	add $3,$1,$0  
	add $7,$6,$1  
	halt $0,$0,$0
