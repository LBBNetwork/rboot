/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	patch.h

Abstract:

	Core header for patch subsystem.
	
--*/

#ifndef _PATCH_H_
#define _PATCH_H_

ULONG
FwPatchFindReference(
	PUCHAR Data,
	ULONG Base,
	ULONG Size,
	PCHAR Signature
	);

ULONG
FwPatchFindTop(
	PUCHAR Data,
	ULONG Base,
	ULONG Size,
	ULONG Address
	);

ULONG
FwPatchFindString(
	PUCHAR Data,
	ULONG Base,
	ULONG Size,
	const PUCHAR Name
	);

PVOID
FwPatchFindFunction(
	const PUCHAR Name,
	PUCHAR Target,
	PUCHAR Base
	);

PUCHAR
FwPatchReverseFind(
	PUCHAR Start,
	INT Length,
	CHAR *Find,
	INT Size
	);

PUCHAR
FwPatchFind(
	PUCHAR Start,
	INT Length,
	CHAR *Find,
	INT Size
	);

PVOID
FwpFindPrintf(
	VOID
	);

PVOID
FwpFindFree(
	VOID
	);

ULONG
FwBaseInitialize(
	VOID
	);

#endif
