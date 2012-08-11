/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	vectors.S

Abstract:

	Stub ARM vectors for the boot loader.
	
--*/

.code 32
.global FwUndefinedVector
FwUndefinedVector:
	b	FwStallProcessor

.global FwSyscallVector
FwSyscallVector:
	b	FwStallProcessor

.global FwPrefetchAbortVector
FwPrefetchAbortVector:
	b	FwStallProcessor

.global FwDataAbortVector
FwDataAbortVector:
	b	FwStallProcessor

.global FwReservedVector
FwReservedVector:
	b	FwStallProcessor

.global FwIrqVector
FwIrqVector:
	b	FwStallProcessor

.global FwFiqVector
FwFiqVector:
	b	FwStallProcessor

