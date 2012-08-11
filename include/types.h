/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	types.h

Abstract:

	Sort of like ntdef.h :)
	
--*/

#ifndef _TYPES_
#define _TYPES_

#include "pstdint.h"

typedef uint32_t ULONG, UINT32, *PULONG, *PUINT32;
typedef uint16_t USHORT, *PUSHORT;
typedef uint8_t UCHAR, *PUCHAR;
typedef int32_t LONG, INT32, *PLONG, *PINT32;
typedef short SHORT, *PSHORT;
typedef char CCHAR, CHAR, *PCHAR;
typedef int INT, *PINT;
typedef void *PVOID;

#define VOID void
#define NULL 0

#define __in
#define __out
#define __inout
#define __optional
#define __inopt
#define __outopt

typedef enum {
	FALSE = 0,
	TRUE
} BOOLEAN;

//
// 64 bit types
//
typedef uint64_t __uint64, __uint64_t, ULONGLONG, *PULONGLONG;
typedef int64_t __int64, __int64_t, LONGLONG, *PLONGLONG;

typedef struct _LIST_ENTRY {
    PVOID Previous;
    PVOID Next;
} LIST_ENTRY, *PLIST_ENTRY;

typedef struct _TIMEINFO
{
    USHORT Year;
    USHORT Month;
    USHORT Day;
    USHORT Hour;
    USHORT Minute;
    USHORT Second;
} TIMEINFO, *PTIMEINFO;

#endif
