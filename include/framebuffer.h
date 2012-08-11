/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	framebuffer.h

Abstract:

	Core header for framebuffer.
	
--*/

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

//
// Framebuffer stuff.
//

#define WHITE	0xFFFFFF
#define	BLACK	0x0

#define Str2(x) #x
#define Stringify(x) Str2(x)

VOID
_snprintf(
    PCHAR Buffer,
    int Size,
    const PCHAR Format,
    ...
    );

typedef struct _FONT {
	ULONG Width;
	ULONG Height;
	PUCHAR Data;
} FONT, *PFONT;

extern FONT Font;

VOID
FwPrintf(
         const PCHAR Format,
         ...
         );


VOID
FwClearScreen(
              UCHAR Attr
              );

VOID
FwFramebufferClearScreen(
	VOID
	);

INT
FwFramebufferInitialize(
	VOID
	);

ULONG
FwFramebufferGetY(
	VOID
	);

ULONG
FwFramebufferGetX(
	VOID
	);

ULONG
FwFramebufferPutChar(
	INT c
	);

VOID
FwPuts(
	const PCHAR String
	);

VOID
FwSerialPutc(
	INT c
	);

#endif
