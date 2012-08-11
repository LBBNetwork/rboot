/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	arm_sweep_icache.S

Abstract:

	ARM processor cache functions: sweep icache

--*/

.code 32
.global FwSweepIcache
FwSweepIcache:
	mov	r0, #0
	mcr	p15, 0, r0, c7, c5
	mcr	p15, 0, r0, c7, c5, 4	@ Flush Prefetch Buffer
	nop
	nop
	nop
	nop
	bx	lr

.global FwFlushIcache
FwFlushIcache:
        mov     r0, #0
        mcr     p15, 0, r0, c7, c5
        mcr     p15, 0, r0, c7, c5, 4   @ Flush Prefetch Buffer
        nop
        nop
        nop
        nop
        bx      lr
