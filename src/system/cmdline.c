/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    cmdline.c

Abstract:

    Currently broken.

--*/

#include "core.h"

#define MAX_COMMANDS 40

ULONG CommandCount = 0;
PVOID CommandListBegin, CommandListEnd;
PCOMMAND_INFO* CommandList = NULL;

PVOID
FwCmdFindList(
    VOID
    )
{
    ULONG Reference, i;
    
    Reference = FwPatchFindReference((PUCHAR)TARGET_BASEADDR, TARGET_BASEADDR, 0x40000, "save current environment to flash");
    
    if(!Reference) {
        FwPrintf("Unable to find saveenv reference!\n");
        return 0;
    }
    
    for(i = 0; i < 0x80; i+= 4) {
        PULONG Command = (PULONG)(Reference - i);
        
        if(*Command == 0) {
            return Command + 1;
        } else if((ULONG)Command < TARGET_BASEADDR || (ULONG)Command > TARGET_BASEADDR + 0x40000) {
            return Command+1;
        } else if(*Command < TARGET_BASEADDR || *Command >= TARGET_BASEADDR + 0x40000) {
            return Command+1;
        }
    }
    
    return 0;
}

PVOID
FwCmdFindListEnd(
    VOID
    )
{
    ULONG i;
    
    if(CommandListBegin) {
        for(i = 0; i < 0x20; i++) {
            PULONG Value = (PULONG)CommandListBegin+i;
            if(!strncmp((const PCHAR)Value, "auto-boot", 0x10) || ((PULONG)*Value == NULL)) {
                return Value;
            }
        }
    }
    
    return 0;
}

VOID
FwCmdAdd(
    PCHAR Name,
    COMMAND_FUNCTION Handler,
    PCHAR Description
    )
{
    PCOMMAND_INFO CommandInfo = NULL;
    
    if(CommandCount >= MAX_COMMANDS) {
        FwPrintf("Unable to add command: maximum commands reached\n");
        return;
    }
    
    CommandInfo = (PCOMMAND_INFO)(LOAD_ADDRESS+0x01800000)+(CommandCount * sizeof(COMMAND_INFO));
    CommandInfo->Name = Name;
    CommandInfo->Handler = Handler;
    CommandInfo->Description = Description;
    CommandList[CommandCount] = CommandInfo;
    CommandCount++;
    
    return;
}
ULONG
FwHelp(
    INT argc,
    PARGUMENT argv
    )
{
    int i;
    for(i=0;i<CommandCount;i++) {
        _printf("   %s\t", CommandList[i]->Name);
        if(strlen(CommandList[i]->Name) < 5)
            _printf(" \t");
        _printf("%s\n", CommandList[i]->Description);
    }
    _printf("\n");
}

ULONG
FwCmdlineInitialize(
    VOID
    )
{
    INT i;

    CommandCount = 0;
    CommandList = (PCOMMAND_INFO*)(LOAD_ADDRESS+0x01800000);
    CommandListBegin = FwCmdFindList();
    CommandListEnd = FwCmdFindListEnd();
    
    if(!CommandListBegin) {
        FwPrintf("Unable to find beginning of command list\n");
    } else {
        FwPrintf("Found command list beginning at %p\n", CommandListBegin);
    }
    
    if(!CommandListEnd) {
        FwPrintf("Unable to find end of command list\n");
    } else {
        FwPrintf("Found command list end at %p\n", CommandListEnd);
    }

    if(CommandListBegin && CommandListEnd) {
        PCOMMAND_INFO* Current = (PCOMMAND_INFO*)CommandListBegin;
        for(i = 0; &Current[i] < (PCOMMAND_INFO*)CommandListEnd; i++) {
            FwCmdAdd(Current[i]->Name, Current[i]->Handler, Current[i]->Description);
        }
    }
    
    FwCmdAdd("help", &FwHelp, "display all available commands");
    return 0;
}

