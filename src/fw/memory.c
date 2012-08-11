/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    memory.c

Abstract:

    Build the ARM block memory BIOS map.
    
--*/

#include "core.h"

BIOS_MEMORY_MAP MemoryMap[32];
extern ARM_BOARD_CONFIGURATION_BLOCK ArmBlock;

BIOS_MEMORY_MAP FwHwS5L8930MemoryMap[] =
{
    {0x00000000, 0x0C000000, BiosMemoryReserved,   0}, // iBSS is mapped here.
    {0x41000000, 0x01000000, BiosMemoryReserved,   0}, // Load a ramdisk here.
    {0x42000000, 0x000F0000, BiosMemoryBootLoader, 0}, // relocated payload
    {0x420F0000, 0x00110000, BiosMemoryBootLoader, 0}, // FreeLoader
    {0x42200000, 0x01E00000, BiosMemoryUsable,     0}, // free RAM
    {0x84000000, 0x0C000000, BiosMemoryReserved,   0}, // iBSS space
    {0, 0, 0, 0}
};

VOID
FwAllocateMemoryEntry(
    BIOS_MEMORY_TYPE Type,
    ULONG BaseAddress,
    ULONG Length
    )
/*++

Routine Description:

    Allocate a memory entry in the memory map.

Arguments:

    Type            - Bios memory type.
    BaseAddress     - Base address of memory region.
    Length          - Memory length.
    
Return Value:

    None.
    
--*/
{
    PBIOS_MEMORY_MAP Entry;
    
    //
    // Get the next memory entry
    //
    
    Entry = MemoryMap;
    while (Entry->Length) Entry++;
    
    //
    // Fill it out
    //
    
    Entry->Length = Length;
    Entry->BaseAddress = BaseAddress;
    Entry->Type = Type;
    
    //
    // Block count
    // 
    
    ArmBlock.MemoryMapEntryCount++;
}

VOID
FwHwBuildMemoryMap(
    PBIOS_MEMORY_MAP MemoryMap
    )
/*++

Routine Description:

    Build the hardware memory map for the given hardware platform..

Arguments:

    MemoryMap            - BIOS memory map.
    
Return Value:

    None.
    
--*/
{
    PBIOS_MEMORY_MAP MapEntry;
    
    /* Parse hardware memory map */
    
    MapEntry = FwHwS5L8930MemoryMap;

    while (MapEntry->Length)
    {
        /* Add this entry */
        FwAllocateMemoryEntry(MapEntry->Type, MapEntry->BaseAddress, MapEntry->Length);

        /* Move to the next one */
        MapEntry++;
    }
}

VOID
FwBuildMemoryMap(
    VOID
    )
/*++

Routine Description:

    Portable function for building thwe memory map.

Arguments:

    None.

Return Value:

    None.

--*/
{
    /* Zero out the memory map */
    memset(MemoryMap, 0, sizeof(MemoryMap));

    DebugTrace("Building memory map\n");

    /* Call the hardware-specific function for hardware-defined regions */
    FwHwBuildMemoryMap(MemoryMap);
}


