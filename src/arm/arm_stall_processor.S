/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	arm_stall_processor.S

Abstract:

	Stall the processor.
	
--*/

.code 32

.global FwStallProcessor
FwStallProcessor:
	b	FwStallProcessor

