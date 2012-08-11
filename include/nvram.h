/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	nvram.h

Abstract:

	Core header for NVRAM definitions.
	
--*/

#ifndef _NVRAM_H_
#define _NVRAM_H_

typedef struct _NVRAM_ENTRY {
    struct _NVRAM_ENTRY *Previous;
    struct _NVRAM_ENTRY *Next;
    PUCHAR String;
    ULONG Integer;
    ULONG Save;
    CHAR Name[0x40];
} NVRAM_ENTRY, *PNVRAM_ENTRY;

extern PLIST_ENTRY NvramVariables;


PVOID
FwFindNvramList(
    VOID
    );

PNVRAM_ENTRY
FwNvramFindVariable(
    const PCHAR Name
    );

VOID
FwDisplayNvramList(
    VOID
    );
    
ULONG
FwNvramDeleteVariable(
    PCHAR Name
    );
    
PCHAR
FwNvramGetVariable(
    const PCHAR Name
    );

ULONG
FwNvramSetVariable(
    const PCHAR Name,
    const PCHAR Value
    );

#endif
