/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	osloader.h

Abstract:

	Core header for OS Loader.
	
--*/

#ifndef _OSLOADER_H_
#define _OSLOADER_H_
    
ULONG
FwBootLoaderCommand(
    int argc,
    PARGUMENT argv
    );


INT
FwCopyLoader(
    INT argc,
    PARGUMENT argv
    );
    
INT
FwCopyRamdisk(
    INT argc,
    PARGUMENT argv
    );

//
// OS Loader Main Routine
// 
typedef
VOID (*POSLOADER_INIT)(
    PVOID BoardInit
);

//
// Type of memory detected by LLB
//
typedef enum
{
    BiosMemoryUsable = 1,
    BiosMemoryBootLoader,
    BiosMemoryBootStrap,
    BiosMemoryReserved
} BIOS_MEMORY_TYPE;

//
// Firmware Memory Map
//
typedef struct
{
    LONGLONG BaseAddress;
    LONGLONG Length;
    ULONG Type;
    ULONG Reserved;
} BIOS_MEMORY_MAP, *PBIOS_MEMORY_MAP;

//
// Information sent from LLB to OS Loader
//
#define ARM_BOARD_CONFIGURATION_MAJOR_VERSION 1
#define ARM_BOARD_CONFIGURATION_MINOR_VERSION 4
typedef struct _ARM_BOARD_CONFIGURATION_BLOCK
{
    ULONG MajorVersion;
    ULONG MinorVersion;
    ULONG BoardType;
    ULONG ClockRate;
    ULONG TimerRegisterBase;
    ULONG UartRegisterBase;
    ULONG MemoryMapEntryCount;
    PBIOS_MEMORY_MAP MemoryMap;
    CHAR CommandLine[256];
    PVOID ConsPutChar;
    PVOID ConsKbHit;
    PVOID ConsGetCh;
    PVOID VideoClearScreen;
    PVOID VideoSetDisplayMode;
    PVOID VideoGetDisplaySize;
    PVOID VideoGetBufferSize;
    PVOID VideoSetTextCursorPosition;
    PVOID VideoHideShowTextCursor;
    PVOID VideoPutChar;
    PVOID VideoCopyOffScreenBufferToVRAM;
    PVOID VideoIsPaletteFixed;
    PVOID VideoSetPaletteColor;
    PVOID VideoGetPaletteColor;
    PVOID VideoSync;
    PVOID GetTime;
} ARM_BOARD_CONFIGURATION_BLOCK, *PARM_BOARD_CONFIGURATION_BLOCK;


//
// Firmware routines.
//

VOID
FwBuildLoaderBlock(
	VOID
	);

VOID
FwBootLoader(
	VOID
	);

#endif
