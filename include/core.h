/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	core.h

Abstract:

	Core header.
	
--*/

#ifndef _CORE_
#define _CORE_

#include "types.h"
#include "offsets.h"
#include "platform.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define READ_REGISTER_UCHAR(Register)          (*(volatile UCHAR *)(Register))
#define READ_REGISTER_USHORT(Register)         (*(volatile USHORT *)(Register))
#define READ_REGISTER_ULONG(Register)          (*(volatile ULONG *)(Register))
#define WRITE_REGISTER_UCHAR(Register, Value)  (*(volatile UCHAR *)(Register) = (Value))
#define WRITE_REGISTER_USHORT(Register, Value) (*(volatile USHORT *)(Register) = (Value))
#define WRITE_REGISTER_ULONG(Register, Value)  (*(volatile ULONG *)(Register) = (Value))


typedef struct _BOARD_INFORMATION {
    ULONG FramebufferBase;
    ULONG FramebufferWidth;
    ULONG FramebufferHeight;
    ULONG ChipId;
} BOARD_INFORMATION, *PBOARD_INFORMATION;

extern BOARD_INFORMATION BoardInformation;

VOID FwFlushDcache(VOID);
VOID FwFlushIcache(VOID);
ULONG FwReadAuxiliaryRegister(VOID);
VOID FwWriteAuxiliaryRegister(ULONG Register);
ULONG FwReadControlRegister(VOID);
VOID FwWriteControlRegister(ULONG Register);
VOID FwEnableLevel1Cache(VOID);
VOID FwEnableFpu(VOID);
ULONG FwArmInitialize(VOID);
VOID FwHardwareInitialize(VOID);
ULONG FwHwGetChipId(VOID);

#include "crt.h"
#include "framebuffer.h"
#include "patch.h"
#include "command.h"
#include "nvram.h"
#include "osloader.h"
#include "fw.h"
	
#define DebugTrace if(_printf) _printf("%s(): ", __FUNCTION__), _printf

#endif 

