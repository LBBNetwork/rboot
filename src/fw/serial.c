/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    serial.c

Abstract:

    Serial print functions.
    
--*/

#include "core.h"


VOID
FwSerialPutc(
    INT c
    )
/*

Routine Description:

    Print one character to console out.
    
Arguments:

    c                - Character to print.
    
Return Value:

    None.
    
--*/
{
    FwPrintf("%c", c);
}

