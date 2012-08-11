/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    start.c

Abstract:

    Main firmware initialization functions.
    
--*/

#include "core.h"

extern void _start();
extern void _end();
int initialized = 0;
extern ULONG CommandCount;
extern PCOMMAND_INFO* CommandList;

BOARD_INFORMATION BoardInformation;
extern ULONG BackColor, FrontColor;

INT
FwpSelfTest(
    INT argc,
    PARGUMENT argv
    )
{
    ULONG i = 0, j, k = 0xdb;
    BackColor = 0;
    FrontColor = 0;
    
    for(j = 0;j < 0xFFFFFF; j++) {
        BackColor = 0;
        FrontColor = j;
        for(i = 0; i <= 80; i++)
            FwFramebufferPutChar(k);
    }
    
    return 0;
}

INT
FwMain(
    INT argc,
    PARGUMENT argv
    )
/*++

Routine Description:

    Main firmware function.
    
Arguments:

    argc        - Argument count.
    argv        - Arguments.
    
Return Value:

    Error status..
    
--*/
{
    if(!initialized) {
        FwBaseInitialize();
        FwHardwareInitialize();
        FwCmdlineInitialize();
        FwFindNvramList();

        if(!strcmp(FwNvramGetVariable("config_board"), "k66ap")) {
            BoardInformation.ChipId = 0x8930;
            BoardInformation.FramebufferBase = 0x4F700000;
            BoardInformation.FramebufferWidth = 720;
            BoardInformation.FramebufferHeight = 1280;
        } else if(!strcmp(FwNvramGetVariable("config_board"), "n92ap") ||
                  !strcmp(FwNvramGetVariable("config_board"), "n90ap") ||
                  !strcmp(FwNvramGetVariable("config_board"), "n81ap")) {
            BoardInformation.ChipId = 0x8930;
            BoardInformation.FramebufferBase = 0x5F700000;
            BoardInformation.FramebufferWidth = 640;
            BoardInformation.FramebufferHeight = 960;
       } else { 
            BoardInformation.ChipId = 0x8920;
            BoardInformation.FramebufferBase = 0x4FD00000;
            BoardInformation.FramebufferWidth = 320;
            BoardInformation.FramebufferHeight = 480;
        }

        FwFramebufferInitialize();
        initialized = 1;

        FwCmdAdd("boot_loader", (COMMAND_FUNCTION)&FwBootLoader, "boot freeldr");
        FwCmdAdd("printenv", (COMMAND_FUNCTION)&FwDisplayNvramList, "display all environment variables");
        FwCmdAdd("clear", (COMMAND_FUNCTION)&FwClearScreen, "clear screen");
        FwCmdAdd("freeldr", (COMMAND_FUNCTION)&FwCopyLoader, "copy boot loader from load address to destination");
        FwCmdAdd("selftest", (COMMAND_FUNCTION)&FwpSelfTest, "stress test hardware");
        FwCmdAdd("ramdisk", (COMMAND_FUNCTION)&FwCopyRamdisk, "create a ramdisk for freeloader");
    
        FwPrintf("Core bootloader subsystems initialized.\n\n");
        
        FwPrintf("ReactOS ARM LLB for %s\n"
               "=========================\n"
               "::\n"
               ":: Configuration:\n"
               "::\n"
               "::   Framebuffer: Base at 0x%08x, Resolution: %d x %d\n"
               "::\n"
               "::   Base Address: %p (LLB end at %p)\n"
               "::\n"
               "::   iBoot Information: %s (%s-%s)\n"
               "::\n"
               "::   iBoot Base: 0x%08x\n"
               "::\n"
               "=========================\n\n",
               FwNvramGetVariable("config_board"),
               BoardInformation.FramebufferBase,
               BoardInformation.FramebufferHeight,
               BoardInformation.FramebufferWidth,
               _start, _end, (char*)TARGET_BASEADDR+0x200, (char*)TARGET_BASEADDR+0x280,(char*)TARGET_BASEADDR+0x240, TARGET_BASEADDR);
            
        FwBuildLoaderBlock();
        FwPrintf("Bootloader initialization complete.\n");
        return 0;
    }

    if(argc > 1) {
        int i = 0;
        PCHAR Command = argv[1].String;
        for(i = 0; i < CommandCount; i++) {
            if(!strcmp(CommandList[i]->Name, Command)) {
                int Return = CommandList[i]->Handler(argc-1, &argv[1]);
                return Return;
            }
        }
        return -1;
    }

    return 0;
}
