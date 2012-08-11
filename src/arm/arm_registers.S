/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

        arm_registers.S

Abstract:

        ARM processor cache functions: special registers.

--*/

.global FwReadAuxiliaryRegister
.global FwWriteAuxiliaryRegister
.global FwReadControlRegister
.global FwWriteControlRegister
.global FwEnableLevel1Cache

.code 32

FwReadAuxiliaryRegister:
	mrc	p15, 0, r0, c1, c0, 1
	bx	lr

FwWriteAuxiliaryRegister:
	mcr	p15, 0, r0, c1, c0, 1
	bx	lr

FwReadControlRegister:
	mrc	p15, 0, r0, c1, c0, 1
	bx	lr

FwWriteControlRegister:
	mcr	p15, 0, r0, c1, c0, 1
	bx	lr

FwEnableLevel1Cache:
	mrc	p15, 0, r0, c1, c0, 1
	orr	r0, r0, #8
	mcr	p15, 0, r0, c1, c0, 1
	bx	lr
