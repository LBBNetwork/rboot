/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    osloader.c

Abstract:

    Build the ARM memory map and loader block, prepare the system
    for FreeLoader.
    
--*/

#include "core.h"

ARM_BOARD_CONFIGURATION_BLOCK ArmBlock;
POSLOADER_INIT LoaderInit;
extern BIOS_MEMORY_MAP MemoryMap[32];
ULONG RamdiskBase = RAMDISK_BASE;
ULONG RamdiskSize = 0;
ULONG RamdiskOffset = 0;

ULONG
FwBootLoaderCommand(
    int argc,
    PARGUMENT argv
    )
{
    FwBootLoader();

    return 0;
}

VOID
FwBuildLoaderBlock(
    VOID
    )
/*++

Routine Description:

    Build the ARM loader block by creating the memory map
    and setting the proper function descriptors for FreeLoader.

Arguments:

    None.
    
Return Value:

    None.

--*/

{

    memset(&ArmBlock, 0, sizeof(ARM_BOARD_CONFIGURATION_BLOCK));

    FwPrintf("Initializing freeldr loader block.\n");
    
    DebugTrace("ArmBlock is located at 0x%08x\n", &ArmBlock);

    //
    // Version number
    //

    ArmBlock.MajorVersion = ARM_BOARD_CONFIGURATION_MAJOR_VERSION;
    ArmBlock.MinorVersion = ARM_BOARD_CONFIGURATION_MINOR_VERSION;

    //
    // The board doesn't really exist.
    //
    
    ArmBlock.BoardType = 387;
    ArmBlock.ClockRate = 0;
    ArmBlock.UartRegisterBase = 0;
    ArmBlock.TimerRegisterBase = 0;

    //
    // Firmware functions
    //

    ArmBlock.ConsPutChar = FwFramebufferPutChar;
    ArmBlock.ConsKbHit = FwKbHit;
    ArmBlock.ConsGetCh = FwGetCh;
    ArmBlock.VideoClearScreen = FwClearScreen;
    ArmBlock.VideoPutChar = FwVideoPutChar;
    ArmBlock.VideoSetDisplayMode = FwVideoSetDisplayMode;
    ArmBlock.VideoGetDisplaySize = FwVideoGetDisplaySize;
    ArmBlock.VideoPutChar = FwVideoPutChar;
    ArmBlock.GetTime = FwGetTime;
    
    //
    // Build the memory map.
    //
    
    FwPrintf("Building loader memory map.\n");
    
    FwBuildMemoryMap();
    
    //
    // Load it.
    //
    
    ArmBlock.MemoryMap = MemoryMap;    
    
}

INT
FwCopyLoader(
    INT argc,
    PARGUMENT argv
    )
{
    PCHAR SizeString = FwNvramGetVariable("filesize");
    INT Length = strtoul(SizeString, NULL, 0);
    PUCHAR Dest;
    
    if(!Length) {
        FwPrintf("Cannot copy a zero sized byte image. Perhaps it is invalid???\n");
        return -1;
    }
    
    if(argc != 2) {
        FwPuts("usage: loader <dest address> (dest address is also loader base)\n");
        return 0;
    }
    
    Dest = (PUCHAR)argv[1].Unknown1;
    memcpy(Dest, (PCHAR)strtoul(FwNvramGetVariable("loadaddr"), NULL, 0), Length);
    
    LoaderInit = (POSLOADER_INIT)Dest;
    FwPrintf("Loader init set to %p, copied %d bytes.\n", Dest, Length);
    
    return 0;
}

INT
FwCopyRamdisk(
    INT argc,
    PARGUMENT argv
    )
{
    
    if(argc != 4) {
        FwPuts("usage: ramdisk <rdbase> <rdoffset> <rdlen>\n");
        return 0;
    }
    
    RamdiskBase = argv[1].Unknown1;
    RamdiskOffset = argv[2].Unknown1;
    RamdiskSize = argv[3].Unknown1;
    
    FwPrintf("Ramdisk registered at 0x%08x, offset 0x%x, size 0x%x bytes\n", RamdiskBase, RamdiskOffset, RamdiskSize);
    
    return 0;
}


VOID
FwBootLoader(
    VOID
    )
/*++

Routine Description:

    Boot the OS loader by jumping to OSLOADER_BASE.
    
Arguments:

    None:
    
Return Value:

    None.
    
--*/
{
    FwPrintf("Bravely assuming loader is at %p, armblock at %p\n", LoaderInit, &ArmBlock);

    _snprintf(ArmBlock.CommandLine, 256, "rdbase=0x%x rdsize=0x%x rdoffset=%d", RamdiskBase, RamdiskSize, RamdiskOffset);

    FwPrintf("Done (cmdline: %s)... JUMP!\n\n\n", ArmBlock.CommandLine);
    
    LoaderInit(&ArmBlock);
    
    //
    // Should never reach here.
    //
    
    while(1);

}

