/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    nvram.c

Abstract:

    iBoot NVRAM hooks
    
--*/


#include "core.h"

PLIST_ENTRY NvramVariables = NULL;

PVOID
FwFindNvramList(
    VOID
    )
{
    ULONG Reference1, *Reference2;
    
    DebugTrace("Looking for nvram list\n");

    Reference1 = (ULONG)FwPatchFindString((PUCHAR)TARGET_BASEADDR, TARGET_BASEADDR, 0x40000, (PUCHAR)"build-style");
    Reference2 = (PULONG)FwPatchFindString((PUCHAR)Reference1+4, Reference1+4, 0x40000, (PUCHAR)"build-style");
    
    Reference2 -= 5;
    
    FwPrintf("NVRAM list begin at 0x%08x\n", *Reference2);
    
    NvramVariables = (PLIST_ENTRY)*Reference2;
    
    return (PVOID)*Reference2;
}

VOID
FwDisplayNvramList(
    VOID
    )
{
    PNVRAM_ENTRY Variable = (PNVRAM_ENTRY)NvramVariables->Next;
    while(Variable != (PVOID)NvramVariables) {
        FwPrintf("<0x%08x> %s = %s\n", Variable, Variable->Name, Variable->String);
        Variable = Variable->Next;
    }
    FwPrintf("\n");
    return;
}

PNVRAM_ENTRY
FwNvramFindVariable(
    const PCHAR Name
    )
{
    PNVRAM_ENTRY Variable = (PNVRAM_ENTRY)NvramVariables->Next;
    
    while(Variable != (PVOID)NvramVariables) {
        if(!strcmp(Name, Variable->Name)) {
            return Variable;
        }
    }
    
    return NULL;
}

ULONG
FwNvramDeleteVariable(
    PCHAR Name
    )
{
    PNVRAM_ENTRY Variable = FwNvramFindVariable(Name);
    PNVRAM_ENTRY Next, Previous;
    
    if(!Variable)
        return -1;
        
    Next = Variable->Next;
    Previous = Variable->Previous;
    
    Next->Previous = Previous;
    Previous->Next = Next;
    
    Variable->Next = 0;
    Variable->Previous = 0;
    
    _free(Variable->String);
    _free(Variable);
    
    return 0;
}

PCHAR
FwNvramGetVariable(
    const PCHAR Name
    )
{
    PNVRAM_ENTRY Variable = (PNVRAM_ENTRY)NvramVariables->Next;
    while(Variable != (PVOID)NvramVariables) {
        if(!strcmp(Variable->Name, Name))
            return (PCHAR)Variable->String;
        Variable = Variable->Next;
    }
    return 0;
}

ULONG
FwNvramSetVariable(
    const PCHAR Name,
    const PCHAR Value
    )
{
    PNVRAM_ENTRY Entry, Next, Previous;
    
    Entry = FwNvramFindVariable(Name);
    if(Entry) {
        FwNvramDeleteVariable(Name);
        Entry = NULL;
    }
    
    Next = (PNVRAM_ENTRY)NvramVariables;
    Previous = (PNVRAM_ENTRY)NvramVariables->Previous;
    
    Entry = _malloc(sizeof(NVRAM_ENTRY));
    if(!Entry)
        return -1;
    
    strncpy(Entry->Name, Name, 0x40);
    Entry->String = _malloc(0x200);
    if(!Entry->String) {
        _free(Entry);
        return -1;
    }
    
    strncpy((PCHAR)Entry->String, Value, 0x200);
    Entry->Integer = strtoul((const PCHAR)Entry->String, NULL, 0);
    Entry->Save = 1;
    
    Entry->Next = Next;
    Entry->Previous = Previous;
    
    Next->Previous = Entry;
    Previous->Next = Entry;
    
    return 0;
}

