/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	arm_flush_dcache.S

Abstract:

	ARM processor cache functions: flush dcache

--*/

.code 32
.global FwFlushDcache
FwFlushDcache:
	mrc	p15, 0, r0, c1, c0, 1
	bx	lr
