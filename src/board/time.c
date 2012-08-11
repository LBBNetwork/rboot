/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    time.c

Abstract:

    Time functions for S5L boards.
    
--*/


#include "core.h"

#define LEAPS_THRU_END_OF(y) ((y)/4 - (y)/100 + (y)/400)

UCHAR FwDaysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

BOOLEAN
FwIsLeapYear(
    ULONG Year
    )
{
    /* Every 4, 100, or 400 years */
    return (!(Year % 4) && (Year % 100)) || !(Year % 400);
}

ULONG
FwDayOfMonth(
    ULONG Month,
    ULONG Year
    )
{
    /* Check how many days a month has, accounting for leap yearS */
    return FwDaysInMonth[Month] + (FwIsLeapYear(Year) && Month == 1);
}

VOID
FwConvertRtcTime(
    ULONG RtcTime,
    TIMEINFO* TimeInfo
    )
{
    ULONG Month, Year, Days, DaysLeft;

    /* Count the days, keep the minutes */
    Days = RtcTime / 86400;
    RtcTime -= Days * 86400;

    /* Get the year, based on days since 1970 */
    Year = 1970 + Days / 365;
    
    /* Account for leap years which changed the number of days/year */
    Days -= (Year - 1970) * 365 + LEAPS_THRU_END_OF(Year - 1) - LEAPS_THRU_END_OF(1970 - 1);
    if (Days < 0)
    {
        /* We hit a leap year, so fixup the math */
        Year--;
        Days += 365 + FwIsLeapYear(Year);
    }

    /* Count months */
    for (Month = 0; Month < 11; Month++)
    {
        /* How many days in this month? */
        DaysLeft = Days - FwDayOfMonth(Month, Year);
        if (DaysLeft < 0) break;
        
        /* How many days left total? */             
        Days = DaysLeft;
    }

    /* Write the structure */
    TimeInfo->Year = Year;
    TimeInfo->Day = Days + 1;
    TimeInfo->Month = Month + 1;
    TimeInfo->Hour = RtcTime / 3600;
    RtcTime -= TimeInfo->Hour * 3600;
    TimeInfo->Minute = RtcTime / 60;
    TimeInfo->Second = RtcTime - TimeInfo->Minute * 60;
}

int time = 0;
TIMEINFO FwTime;

PTIMEINFO
FwGetTime(
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
    /* Call existing function */
    ULONG RtcTime;

    /* Read RTC time */
    //
    // HACK HACK HACK 
    // NO CLUE WHERE THE RTC IS RIGHT NOW
    // EVIL TIMING LOOP LOL
    //
    // ..but it works!
    //
    int i = 0x10000000;
    while(i--) {
        __asm__("mov r8,r8");
    }
    time++;

    RtcTime = time;

    /* Convert it */
    FwConvertRtcTime(RtcTime, &FwTime);
    return &FwTime;
}
