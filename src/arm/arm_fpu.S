/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

        arm_fpu.S

Abstract:

        ARM processor base initialization: FPU

--*/

.global FwEnableFpu
.code 32
FwEnableFpu:
//	mrc	p15, 0, r0, c1, c0, 2
//	mov	r1, #0xF
//	orr	r0, r0, r1, LSL#20
//	mcr	p15, 0, r0, c1, c0, 2	// Apparently we jump to bootrom? O.o
	fmrx	r2, fpexc
	orr	r2, r2, #0x40000000
	fmxr	fpexc, r2
	bx	lr
