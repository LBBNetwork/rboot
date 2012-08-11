/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    gpio.c

Abstract:

    Samsung S5L Armv7 SoC GPIO support.
    
--*/

#include "core.h"


typedef enum {
    BUTTON_HOME = 0,
    BUTTON_HOLD = 1,
    BUTTON_VOLUP = 2,
    BUTTON_VOLDOWN = 3,
    BUTTON_RINGER = 4
} HW_BUTTONS;

INT
FwHwGetGpioPinState(
    INT Port
    )
{
    UCHAR Pin = Port & 0x7;
#ifdef _S5L8930X_
    UCHAR BadPort = 0x2E;
#else
    UCHAR BadPort = 0x16;
#endif

    Port = Port >> 8;
    
    if(Port == BadPort) {
        return 0;
    } else {
        return (READ_REGISTER_ULONG(GPIO_BASE + (8 * Port + Pin) * sizeof(ULONG)) & 1);
    }
}


HW_BUTTONS PreviousButton = 0xFF;
BOOLEAN ButtonHit = FALSE;

BOOLEAN
FwKbHit(
    VOID
    )
/*++

Routine Description:

    Stub function.

Arguments:

    None.
    
Return Value:

    Something.

--*/
{
    HW_BUTTONS Button = BUTTON_HOME;

    while(Button <= BUTTON_RINGER) {
        if(!FwHwGetGpioPinState(Button)) {
            PreviousButton = Button;
            ButtonHit = 1;
            return 1;
        } else {
            Button++;
        }
    }
    return 0;
}

INT
FwGetCh(
    VOID
    )
/*++

Routine Description:

    Stub function.

Arguments:

    None.
    
Return Value:

    Something.

--*/
{    
    if(ButtonHit) {
        ButtonHit = 0;
        switch(PreviousButton) {
            case BUTTON_HOME:
                return 0x0D;
            case BUTTON_HOLD:
                return 0x1B;
            case BUTTON_VOLUP:
                return 0x48;
            case BUTTON_VOLDOWN:
                return 0x50;
            case BUTTON_RINGER:
                return 0;
            default:
                return 0;    
        }
    }
    return 0;
}


