/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    core.c

Abstract:

    Core functions for finding subroutines.
    
--*/

#include "core.h"

BOOLEAN FramebufferInitialized = FALSE;
BOOLEAN FramebufferCanHasText = FALSE;

ULONG CursorX, CursorY;

extern UCHAR FixedFont[256*16], FwHwBootFont[256*16];

static ULONG Twidth, Theight;
static PFONT pFont;

ULONG BackColor;
ULONG FrontColor;

//
// Hacky bit reverse function to prepare the font.
//

unsigned char __reverse_bits(unsigned char v) {
    unsigned char r = v;
    int s = sizeof(v) * 8 - 1;
    for(v >>= 1; v; v >>= 1) {
        r <<= 1;
        r |= v & 1;
        s--;
    }
    r <<= s;
    
    return r;
}

VOID
FwPrepareFont(
    VOID
    )
/*++

Routine Description:

    Flip the font.

Arguments:

    None.

Return Value:

    None.
    
--*/
{
    INT i;
    
    for(i = 0; i < sizeof(FixedFont); i++) {
        FixedFont[i] = __reverse_bits(FwHwBootFont[i]);
    }
    
    Font.Data = FixedFont;
}

inline
INT
FwpFontGetPixel(
    FONT *Font,
    INT ch,
    INT x,
    INT y
    )
/*++

Routine Description:

    Determine whether pixel in font is transparent or opaque.

Arguments:

    Font        - Font definition
    ch          - Character
    x           - X location in character.
    y           - Y location in character.

Return Value:

    Pixel status.
    
--*/
{
    register int index = ((Font->Width * Font->Height) * ch) + (Font->Width * y) + x;
    return (Font->Data[index / 8] >> (index % 8)) & 0x1;
}

volatile
unsigned int*
FwpFramebufferGetPixel(
    register unsigned int x,
    register unsigned int y
    )
/*++

Routine Description:

    Find pixel in framebuffer memory.
    
Arguments:

    x            - X location.
    y            - Y location.

Return Value:

    Pixel address.
    
--*/
{
    return (((unsigned int*)BoardInformation.FramebufferBase) + (y * BoardInformation.FramebufferWidth) + x);
}

static
VOID
FwpFramebufferScroll(
    VOID
    )
/*++

Routine Description:

    Scroll the screen.
    
Arguments:

    None.

Return Value:

    None.

--*/
{
    register volatile unsigned int* newFirstLine = FwpFramebufferGetPixel(0, pFont->Height);
    register volatile unsigned int* oldFirstLine = FwpFramebufferGetPixel(0, 0);
    register volatile unsigned int* end = oldFirstLine + (BoardInformation.FramebufferWidth * BoardInformation.FramebufferHeight);
    while(newFirstLine < end) {
        *(oldFirstLine++) = *(newFirstLine++);
    }
    while(oldFirstLine < end) {
        *(oldFirstLine++) = BackColor;
    }
    CursorY--;
}

VOID
FwFramebufferClearScreen(
    VOID
    )
/*++

Routine Description:

    Clear the screen.
    
Arguments:

    None.

Return Value:

    None.

--*/
{
    unsigned int *p = 0;
    for(p = (unsigned int*)BoardInformation.FramebufferBase; p < (unsigned int*)(BoardInformation.FramebufferBase + (BoardInformation.FramebufferWidth * BoardInformation.FramebufferHeight * 4)); p++) {
        *p = BackColor;
    }
}

ULONG
FwFramebufferGetX(
    VOID
    )
/*++

Routine Description:

    Get X coordinate of the screen cursor.
    
Arguments:

    None.

Return Value:

    Coordinate.

--*/
{
    return CursorX;
}

ULONG
FwFramebufferGetY(
    VOID
    )
/*++

Routine Description:

    Get Y coordinate of the screen cursor.
    
Arguments:

    None.

Return Value:

    Coordinate.

--*/
{
    return CursorY;
}

ULONG
FwFramebufferPutChar(
    INT c
    )
/*++

Routine Description:

    Print a character to the framebuffer.

Arguments:

    c            - Character.

Return Value:

    None.
    
--*/
{
    if(c == '\r') {
        CursorX = 0;

    } else if(c == '\n') {
        CursorX  = 0;
        CursorY++;

    } else if(c == '\t') {
        CursorX = (CursorX + 8) & ~(8-1);
    } else if(c == '\b' && CursorX) {
        CursorX--;
    } else {
        register unsigned int sx;
        register unsigned int sy;

        for(sy = 0; sy < pFont->Height; sy++) {
            for(sx = 0; sx < pFont->Width; sx++) {
                if(FwpFontGetPixel(pFont, c, sx, sy)) {
                    *(FwpFramebufferGetPixel(sx + (pFont->Width * CursorX), sy + (pFont->Height * CursorY))) = FrontColor;
                } else {
                    *(FwpFramebufferGetPixel(sx + (pFont->Width * CursorX), sy + (pFont->Height * CursorY))) = BackColor;
                }
            }
        }

        CursorX++;
    }

    if(CursorX == Twidth) {
        CursorX = 0;
        CursorY++;
    }

    if(CursorY == Theight) {
        FwpFramebufferScroll();
    }

    return 0;
}

VOID
FwPuts(
    const PCHAR String
    )
/*++

Routine Description:

    Print a string to the framebuffer.
    
Arguments:

    String            - String to display.

Return Value:

    None.
    
--*/
{
    ULONG Length = strlen(String);
    INT i;

    for(i = 0; i < Length; i++) {
        FwFramebufferPutChar(String[i]);
    }
}

VOID
FwPrintf(
    const PCHAR Format,
    ...
    )
/*++

Routine Description:

    Print a formatted string to the framebuffer.

Arguments:

    Format            - Format string.

Return Value:

    None.
    
--*/
{
    CHAR Buffer[2048];
    va_list ap;

    va_start(ap, Format);
    _vsnprintf(Buffer, 2048, Format, ap);
    va_end(ap);
    if(FramebufferInitialized)
        FwPuts(Buffer);
    _printf(Buffer);

    return;
}

INT
FwFramebufferInitialize(
    VOID
    )
/*++

Routine Description:

    Initalize the framebuffer.
    
Arguments:

    None.

Return Value:

    None.
    
--*/
{
    FwPrepareFont();

    if(FramebufferInitialized)
        return 0;

    pFont = &Font;
    BackColor = BLACK;
    FrontColor = WHITE;
    Twidth = (BoardInformation.FramebufferWidth / pFont->Width);
    Theight = (BoardInformation.FramebufferHeight / pFont->Height);
    CursorX = 0;
    CursorY = 0;
    
    FwFramebufferClearScreen();
    FramebufferInitialized = 1;
    FwPrintf("Console initialized (height: %d cells, width: %d cells)\n", Theight, Twidth);
    
    return 0;
}

