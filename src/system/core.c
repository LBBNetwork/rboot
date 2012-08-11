/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    core.c

Abstract:

    Core functions for finding subroutines.
    
--*/

#include "core.h"

static CHAR Push = {0xB5};
static CHAR PushR7Lr[] = {0x80, 0xB5};
static CHAR PushR4R7Lr[] = {0x90, 0xB5};
static CHAR PushR4ToR7Lr[] = {0xF0, 0xB5};
static CHAR PushR4R5R7Lr[] = {0xB0, 0xB5};

VOID (*_free)(PVOID ptr) = NULL;
PVOID (*_malloc)(ULONG size) = NULL;
INT (*_printf)(const PCHAR fmt, ...) = NULL;

static PCHAR Functions[][3] = {
    { "aes_crypto_cmd", "aes_crypto_cmd", PushR4R7Lr },
    { "free", "heap_panic",  PushR4ToR7Lr },
    { "fs_mount", "fs_mount", PushR4ToR7Lr },
    { "cmd_ramdisk", "Ramdisk too large", PushR4R5R7Lr },
    { "cmd_go", "jumping into image", PushR7Lr },
    { "image_load", "image validation failed but untrusted images are permitted", PushR4ToR7Lr },
    { "fsboot", "root filesystem mount failed", PushR4ToR7Lr },
    { "kernel_load", "rd=md0", PushR4ToR7Lr },
    { "task_yield", "task_yield", PushR4R5R7Lr },
    { "default_block_write", "no reasonable default block write routine", PushR7Lr },
    { "populate_images", "image %p: bdev %p type %c%c%c%c offset 0x%x", PushR4R5R7Lr },
    { "uart_read", "uart_read", PushR4ToR7Lr },
    { "uart_write", "uart_write", PushR4ToR7Lr },
    { "task_create", "task_create", PushR4ToR7Lr },
    { "task_exit", "task_exit", PushR4ToR7Lr },
    { NULL, NULL, NULL }
};

ULONG
FwPatchFindReference(
    PUCHAR Data,
    ULONG Base,
    ULONG Size,
    PCHAR Signature
    )
/*++

Routine Description:

    Find a reference to a signature.
    
Arguments:

    Data        - Buffer to scan.
    Base        - Base address.
    Size        - Size of the buffer.
    Signaure    - Signature of the reference.
    
Return Value:

    Address of found reference.
    
--*/
{
    ULONG i, Address, Reference, Reference2 = 0;

    //
    // Find the string
    //

    for(i = 0; i < Size; i++) {
        if(!memcmp(&Data[i], Signature, strlen(Signature))) {
            Address = Base | i;
            break;
        }
    } 
    if(!Address)
        return 0;

    //
    // Find xref.
    //

    for(i = 0; i < Size; i++) {
        if(!memcmp(&Data[i], &Address, sizeof(ULONG))) {
            Reference = Base | i;
            break;
        }
    }
    if(!Reference)
        return 0;

    Reference -= 8;

    for(i = 0; i < Size; i++) {
        if(!memcmp(&Data[i], &Reference, sizeof(ULONG))) {
            Reference2 = Base | i;
            break;
        }
    }
    if(!Reference2)
        return 0;

    return Reference2;        
}

ULONG
FwPatchFindTop(
    PUCHAR Data,
    ULONG Base,
    ULONG Size,
    ULONG Address
    )
/*++

Routine Description:

    Find the top of a function.
    
Arguments:

    Data        - Buffer to scan.
    Base        - Base address.
    Size        - Size of the buffer.
    Address     - Address to find.
    
Return Value:

    Address of the top of the function
    
--*/
{
    INT i = 0;
    ULONG Function = 0;

    while(i > 0) {
        i--;
        if(Data[i] == Push) {
            Function = Base | i;
            break;
        }
    }

    if(!Function)
        return 0;

    return Function;
}

ULONG
FwPatchFindOffset(
    PUCHAR Data,
    ULONG Base,
    ULONG Size,
    PUCHAR* Function
    )
/*++

Routine Description:

    Find the offset of a specified function.
    
Arguments:

    Data        - Buffer to scan.
    Base        - Base address.
    Size        - Size of the buffer.
    Function    - Function to find.
    
Return Value:

    Address of the function
    
--*/
{
    ULONG i = 0, pBase = (ULONG)Data;
    PUCHAR Signature = Function[1];
    ULONG Address, Reference = 0, Func = 0;

    //
    // Look for the string.
    //

    for(i = 0; i < Size; i++) {
        if(!memcmp(&Data[i], Signature, strlen((const PCHAR)Signature))) {
            Address = Base | i;
            break;
        }
    }

    if(!Address)
        return 0;

    //
    // Find xref.
    //

    for(i = 0; i < Size; i++) {
        if(!memcmp(&Data[i], &Address, sizeof(ULONG))) {
            Reference = Base | i;
            break;
        }
    }
    
    if(!Reference)
        return 0;

    while(i > 0) {
        i--;
        if(Data[i] == Push) {
            Func = pBase | i;
            break;
        }
    }
    
    if(!Func)
        return 0;

    return Func;
}

ULONG
FwPatchFindString(
    PUCHAR Data,
    ULONG Base,
    ULONG Size,
    const PUCHAR Name
    )
/*++

Routine Description:

    Find a string.
    
Arguments:

    Data        - Buffer to scan.
    Base        - Base address.
    Size        - Size of the buffer.
    Name        - String to find.

Return Value:

    Address of the string.
    
--*/
{
    INT i = 0;
    ULONG Address = 0;

    for(i = 0; i < Size; i++) {
        if(!memcmp(&Data[i], Name, strlen((const PCHAR)Name))) {
            Address = (ULONG)&Data[i];
            break;
        }
    }

    return Address;
}

PVOID
FwPatchFindFunction(
    const PUCHAR Name,
    PUCHAR Target,
    PUCHAR Base
    )
/*++

Routine Description:

    Find the specified function by name.
    
Arguments:

    Name    - Function name
    Target    - Function target
    Base    - Function base.
    
Return Value:

    Address of the function
    
--*/
{
    INT i;
    ULONG Found = 0;

    for(i = 0; i < sizeof(Functions); i++) {
        if(!strcmp((PCHAR)Functions[i][0], (PCHAR)Name)) {
            Found = FwPatchFindOffset(Target, (ULONG)Base, 0x40000, (PUCHAR*)Functions[i]);
            if(Found < 0)
                return NULL;
            break;
        }
    }

    return (PVOID)Found;
}

PUCHAR
FwPatchFind(
    PUCHAR Start,
    INT Length,
    CHAR *Find,
    INT Size
    )
/*++

Routine Description:

    Find abstract data.
    
Arguments:

    Start        - Start of the buffer to scan.
    Length        - Length of the buffer
    Find        - Data to find.
    Size        - Length of the data to find.
    
Return Value:

    Address of the data
    
--*/
{
    INT i;
    for(i = 0; i < Length; i++) {
        if(!memcmp(&Start[i], Find, Size)) {
            return &Start[i];
        }
    }

    return 0;
}

PUCHAR
FwPatchReverseFind(
    PUCHAR Start,
    INT Length,
    CHAR *Find,
    INT Size
    )
/*++

Routine Description:

    Find abstract data in reverse searching order.
    
Arguments:

    Start        - Start of the buffer to scan.
    Length        - Length of the buffer
    Find        - Data to find.
    Size        - Length of the data to find.
    
Return Value:

    Address of the data
    
--*/
{
    INT i;
    for(i = Length; i < Length; i--) {
        if(!memcmp(&Start[i], Find, Size)) {
            return &Start[i];
        }
    }

    return 0;
}


PVOID
FwpFindPrintf(
    VOID
    )
/*++

Routine Description:

    Find _printf in iBoot.
    
Arguments:

    None.
    
Return Value:

    Address of the function.
    
--*/
{
    int i = 0;
    int j = 0;
    unsigned int sp;
    unsigned int* stack = &sp;
    void(*default_block_write)(void) = FwPatchFindFunction((PUCHAR)"default_block_write", (PUCHAR)TARGET_BASEADDR, (PUCHAR)TARGET_BASEADDR);
    default_block_write();
    for(i = 0; i < 0x100; i += 4) {
        unsigned int value = *(stack - i);
        if((value & TARGET_BASEADDR) == TARGET_BASEADDR) {
            for(j = 0; j < 0x100; j++) {
                unsigned short* instruction = (unsigned short*)(value + j);
                if(*instruction == 0xB40F) {
                    return (void*) value + (j+1);
                }
            }
        }
    }
    return 0;    
}

PVOID
FwpFindFree(
    VOID
    )
/*++

Routine Description:

    Find _free in iBoot.
    
Arguments:

    None.
    
Return Value:

    Address of the function.
    
--*/
{
    return FwPatchFindFunction((PUCHAR)"free", (PUCHAR)TARGET_BASEADDR, (PUCHAR)TARGET_BASEADDR);
}

PVOID
FwpFindMalloc(
    VOID
    )
/*++

Routine Description:

    Find _malloc in iBoot.
    
Arguments:

    None.
    
Return Value:

    Address of the function.
    
--*/
{
    PVOID Bytes = FwPatchFind((PUCHAR)TARGET_BASEADDR, 0x40000, "\x80\xB5\x00\xAF\x01\x21\x00\x22", 8);

    if(!Bytes)
        return 0;

    return Bytes+1;
}


ULONG
FwBaseInitialize(
    VOID
    )
/*++

Routine Description:

    Initialize base routines.
    
Arguments:

    None.
8    
Return Value:

    None.
    
--*/
{
    _printf = FwpFindPrintf();
    _malloc = FwpFindMalloc();
    _free = FwpFindFree();

    DebugTrace("Initialized base subsystem.\n");

    return 0;
}
