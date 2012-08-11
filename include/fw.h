/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    fw.h

Abstract:

    Firmware definitions.

--*/

#ifndef _FW_H_
#define _FW_H_

PTIMEINFO
FwGetTime(
    VOID
    );
    
BOOLEAN
FwKbHit(
    VOID
    );

INT
FwGetCh(
    VOID
    );

VOID
FwVideoGetDisplaySize(
    PULONG Width,
    PULONG Height,
    PULONG Depth
    );
    
ULONG
FwVideoSetDisplayMode(
    PCHAR DisplayModeName,
    BOOLEAN Init
    );

VOID
FwVideoPutChar(
    INT c,
    UCHAR Attr,
    ULONG X,
    ULONG Y
    );
    
#endif

