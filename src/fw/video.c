/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

   video.c

Abstract:

    Video dispatch functions for rBoot.

--*/

#include "core.h"

typedef struct _COLOR_INFORMATION {
    ULONG ColorIndex;
    ULONG RgbValue;
    PCHAR Name;
} COLOR_INFORMATION, *PCOLOR_INFORMATION;

COLOR_INFORMATION Colors[] = {
    {0, 0x000000, "Black"},
    {1, 0x0000AA, "Blue"},
    {2, 0x00AA00, "Green"},
    {3, 0x00AAAA, "Cyan"},
    {4, 0xAA0000, "Red"},
    {5, 0xAA00AA, "Magenta"},
    {6, 0xAAAA00, "Brown"},
    {7, 0xAAAAAA, "Gray"},
    {8, 0x444444, "Dark Gray"},
    {9, 0x4444FF, "Light Blue"},
    {10, 0x44FF44, "Light Green"},
    {11, 0x44FFFF, "Light Cyan"},
    {12, 0xFF4444, "Light Red"},
    {13, 0xFF44FF, "Light Magenta"},
    {14, 0xFFFF44, "Light Yellow"},
    {15, 0xFFFFFF, "White"}
};



extern ULONG CursorX, CursorY;
extern ULONG BackColor, FrontColor;

VOID
FwVideoPutChar(
    INT c,
    UCHAR Attr,
    ULONG X,
    ULONG Y
    )
/*++

Routine Description:

    Display one character on the screen using X and Y coordinates.
    
Arguments:

    c            - Character to print.
    Attr         - Color attributes
    X            - X coordinate on screen.
    Y            - Y coordinate on screen.
    
Return Value:

    None.
    
--*/
{
    ULONG SaveX, SaveY, SaveBack, SaveFront;
    ULONG FrontColorT = (Attr & 0xF);
    ULONG BackColorT = ((Attr & 0xF0) >> 4);

    SaveY = CursorY;
    SaveX = CursorX;
    SaveBack = BackColor;
    SaveFront = FrontColor;

    CursorY = Y;
    CursorX = X;
    
    BackColor = Colors[BackColorT].RgbValue;
    FrontColor = Colors[FrontColorT].RgbValue;
    
    FwFramebufferPutChar(c);

    FrontColor = SaveFront;
    BackColor = SaveBack;
    CursorY = SaveY;
    CursorX = SaveX;
}

VOID
FwClearScreen(
    UCHAR Attr
    )
/*++

Routine Description:

    Clear the screen.

Arguments:

    Attr            - Color attributes for screen clear (unused)

Return Value:

    None.
    
--*/
{
    FwFramebufferClearScreen();
    CursorX = 0;
    CursorY = 0;
}

VOID
FwVideoGetDisplaySize(
    PULONG Width,
    PULONG Height,
    PULONG Depth
    )
/*++

Routine Description:

    Get display size in terms of cells and color depth (bpp).

Arguments:

    Width            - Pointer to buffer containing width
    Height           - Pointer to buffer containing height
    Depth            - Pointer to buffer containing bits per pixel.
    
Return Value:

    None.

--*/
{
    //
    // Query static settings
    //
    
    *Width = FRAMEBUFFER_WIDTH / 8;
    *Height = FRAMEBUFFER_HEIGHT / 16;
    
    //
    // Depth is always 32bpp
    //
    
    *Depth = 32;
}

ULONG
FwVideoSetDisplayMode(
    PCHAR DisplayModeName,
    BOOLEAN Init
    )
/*++

Routine Description:

    Set video mode for current display.
    
Arguments:

    DisplayModeName         - Display mode.
    Init                    - (Re)initialize display.

Return Value:

    Set display moade.

--*/
{

    //
    // Return text mode.
    //

    return 0;
}



