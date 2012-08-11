/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	entry.S

Abstract:

	Main firmware initialization functions, entered from iBSS.
	
--*/

#include "offsets.h"

.arm
.ltorg
.code 32
.global _start

_start:

	//
	// CPU 0 will start execution here.
	//
	// This code is executed after control is passed to it from
	// iBSS/iBoot.
	//

	b	FwReset
	ldr	pc, =FwUndefinedVector
	ldr	pc, =FwSyscallVector
	ldr	pc, =FwPrefetchAbortVector
	ldr	pc, =FwDataAbortVector
	ldr	pc, =FwReservedVector
	ldr	pc, =FwIrqVector
	ldr	pc, =FwFiqVector

.pool
.set	FP_CMD_1,	0x47184b00	@ search: LDR R3, =0x41000000 @ BR R3
.set	FP_CMD_2,	0x41000000	@ search: 0x41000000
.set	PATCH_CMD_1,	0x47184b00	@ _patch: LDR R3, =0x42000000 @ BX R3
.set	PATCH_CMD_2,	0x42000000	@ _patch: 0x42000000

.set	FP_CMD2_1,	0x47184b00	@ search: LDR R3, =0x09000000 @ BR R3
.set	FP_CMD2_2,	0x09000000	@ search: 0x09000000
.set	PATCH_CMD2_1,	0x47184b00	@ _patch: LDR R3, =0x0A000000 @ BX R3
.set	PATCH_CMD2_2,	0x0A000000	@ _patch: 0x0A000000
.set	ArmNewLoadAddress,		0x41000000
.set	ArmNewPayloadDestination,	0x42000000
.set 	ArmOldLoadAddress,		0x09000000
.set	ArmOldPayloadDestination,	0x0A000000

//
// Arm Reset Code
//
.code 32
FwReset:
	push	{r0-r12, lr}
	mov	r5, pc
	lsr	r5, #24		// Shift to get first byte
	cmp	r5, #0x42	// Check if LoadAddr
	beq	_Relocated
	cmp	r5, #0x0A
	beq	_Relocated
	cmp 	r5, #0x41
	beq	_RelocateNew
	cmp	r5, #0x09
	beq	_RelocateOld

	bl	FwStallProcessor	// Unknown, spin

_RelocateNew:
	ldr	r0, =ArmNewLoadAddress
	ldr	r1, =ArmNewPayloadDestination
	b	_Relocate

_RelocateOld:
	ldr	r0, =ArmOldLoadAddress
	ldr	r1, =ArmOldPayloadDestination
	b	_Relocate

	//
	// We need to move iBoot.
	//

_Relocate:
	mov	r4, r1
	bl	_Copy
	bl	_Patch
	cmp	r5, #0x09
	bne	_FlushNewCaches
	bl	FwSweepIcache
	b	_JumpMain

_FlushNewCaches:
	bl	FwSweepIcache
	bl	FwFlushDcache

_JumpMain:
	blx	r4
	b	_Done

_Relocated:

	//
	// Load arguments
	//

	ldr	r0, [sp, #0]
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #12]
	bl	FwMain
	str	r0, [sp, #0]

_Done:
	pop	{r0-r12, pc}

.code 32
//
// Tight memory copy.
//
_Copy:
	mov	r2, #0x100000
_CopyLoop:
	ldr	r3, [r0], #4
	str	r3, [r1], #4
	subs	r2, r2, #4
	bne	_CopyLoop
	bx	lr

.code 32
//
// Get rid of signature checking
//
_Patch:
	push	{lr}
	mov	r0, pc
	lsr	r0, #24

_Patch1:
	ldr	r0, =FP_CMD_1
	ldr	r1, =TARGET_BASEADDR
	mov	r2, #0x2C000
	ldr	r3, =FP_CMD_2
	blx	_Find64
	cmp	r0, #0
	beq	_Patch2
	ldr	r1, =PATCH_CMD_1
	str	r1, [r0]
	ldr	r1, =PATCH_CMD_2
	str	r1, [r0, #4]
	b	_PatchComplete

_Patch2:
	ldr	r0, =FP_CMD2_1
	ldr	r1, =TARGET_BASEADDR
	mov	r2, #0x24000
	ldr	r3, =FP_CMD2_2
	blx	_Find64
	cmp	r0, #0
	beq	_PatchComplete	// No more to patch
	ldr	r1, =PATCH_CMD2_1
	str	r1, [r0]
	ldr	r1, =PATCH_CMD2_2
	str	r1, [r0, #4]
	b	_PatchComplete

_PatchComplete:
	pop	{pc}

.code 16
.thumb_func
_Find64:
	push	{r4, lr}
_FindLoop:
	ldr	r4, [r1]
	cmp	r4, r0
	bne	_FindContinue
	ldr	r4, [r1, #4]
	cmp	r4, r3
	beq	_FindReturn
_FindContinue:
	add	r1, #2
	sub	r2, #2
	cmp	r2, #0
	bne	_FindLoop
	mov	r1, #0
_FindReturn:
	mov	r0, r1
	pop	{r4,pc}

