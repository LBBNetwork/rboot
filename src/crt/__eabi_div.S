.code 32
.arm
.global __aeabi_uidivmod
__aeabi_uidivmod:
	stmfd	sp!, {r0-r1,ip,lr}
	bl	__aeabi_uidiv
	ldmfd	sp!, {r1,r2,ip,lr}
	mul	r3, r0, r2
	sub	r1, r1, r3
	mov	pc, lr

.code 32
.arm
.global __aeabi_idivmod
__aeabi_idivmod:
	stmfd	sp!, {r0-r1,ip,lr}
	bl	__aeabi_idiv
	ldmfd	sp!, {r1,r2,ip,lr}
	mul	r3, r0, r2
	sub	r1, r1, r3
	mov	pc, lr

