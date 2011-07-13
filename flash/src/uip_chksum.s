	.section	.text
	.arm
	.global uip_chksum
/*
 * r0: data block (half words)
 * r1: block length in bytes
 */
icnt		.req	r4
jcnt		.req	r3
sum		.req	r5
dstart		.req	r2

uip_chksum:
	stmfd	sp!,	{r4-r12, lr}	@ Save registers into stack

	mov	dstart,	r0		@ data block start
	mov	jcnt,	#6		@ j counter
	mov	icnt,	r1		@ i counter
	mov	sum,	#0		@ sum register
loop:
	cmp	jcnt,	#6		@ if j >= 6 then
	ldmgeia	r2!,	{r6-r12}	@ load data block
	addge	dstart,	dstart,#58
	movge	jcnt,	#0		@ j <- 0

	mov	r1,	r6, LSR#16
	bic	r6,	r6, r1, LSL#16
	add	sum,	sum, r1		@ sum <- sum + r6
	add	sum,	sum, r6
	bl	shift_registers		
	add	jcnt,	jcnt, #1	@ j <- j + 1
	subs	icnt,	icnt, #4	@ i <- i - 4
	bne	loop

	mov	r0,	sum, LSR#16
	bic	sum,	sum, #0xFF000000
	bic	sum,	sum, #0xFF0000
	add	r0,	r0, sum
	rsb	r0,	r0, #0
	add	r0,	r0, #-1
	bic	r0,	r0, #0xFF000000
	bic	r0,	r0, #0xFF0000

	ldmfd	sp!,	{r4-r12, lr}	@Restore registers and return
	bx	lr

	/*
	 * j = 6
	 * i = block_length
	 * while i != 0 do:
	 *	if j > 7  then:
	 *		load data_block into r6-r12
	 *		j <- 0
	 *	endif
	 *	if j < 6 then:
	 *		r5 <- r5 + r6
	 *		shift_registers
	 *		r5 <- r5 + r6
	 *		shift_registers
	 *		j <- j + 2
	 *		i <- i - 2
	 *	endif
	 * done
	 * r0 <- r5(l) + r5(h)
	 */

/*
 * Performs a descendent shift of registers r6 to r12
 */
shift_registers:
	mov	r6,	r7
	mov	r7,	r8
	mov	r8,	r9
	mov	r9,	r10
	mov	r10,	r11
	mov	r11,	r12
	mov	pc,	lr

