	.text
main:	la	$s0,nums
	lw	$t6,0($s0)
	addi	$t7,$t6,1	
	sw	$t7,0($s0)
	.data
nums:	.space	4
