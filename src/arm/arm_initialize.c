/*

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

        arm_initialize.c

Abstract:

        ARM processor base initialization.

--*/

#include "core.h"

ULONG
FwArmInitialize(
    VOID
    )
{
    ULONG Register;
    ULONG Control;

    /* NOTE: DOING ANY OF THE CP15 FUNCTIONS CAUSES US TO JUMP BACK TO S-ROM */

    //
    // Flush processor caches.
    //

    //FwFlushDcache();
    //FwFlushIcache();

    //
    // Write auxiliary register.
    //

    //Register = FwReadAuxiliaryRegister();
    //Register |= 0x100;
    //FwWriteAuxiliaryRegister(Register);

    //
    // Enable L1 cache.
    //

    //FwEnableLevel1Cache();

    //
    // Set ARM control register.
    //

    //Control = FwReadControlRegister();
    //Control |= 0x1805;
    //Control &= ~2;
    //Control |= 0x400000;
    //FwWriteControlRegister(Control);

    //
    // Enable FPU.
    //
    //FwEnableFpu();

    return 0;
}


