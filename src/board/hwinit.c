/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    hwinit.c

Abstract:

    Hardware initialization

--*/

#include "core.h"

VOID
FwHardwareInitialize(
    VOID
    )
{

    //
    // Print device ChipId
    //

    DebugTrace("device chip id is 0x%08x\n", FwHwGetChipId());

    return;
}
