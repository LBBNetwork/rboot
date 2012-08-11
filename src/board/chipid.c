/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    chipid.c

Abstract:

    Armv7 ChipId support
    
--*/

#include "core.h"

ULONG
FwHwGetChipId(
    VOID
    )
{
    return READ_REGISTER_ULONG(0xBF500000);
}

