/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    h3.c

Abstract:

    Samsung S5L Armv7 SoC Framebuffer support.
    
--*/

#include "core.h"

#define MIPI_DSIM 0x89500000
#define MIPI_DSIM_CLOCKGATE 0x11
#define STATUS          0x0     // status register
#define SWRST           0x4     // software reset register
#define CLKCTRL         0x8     // clock control register
#define TIMEOUT         0xC     // time out register
#define CONFIG          0x10    // configuration register
#define ESCMODE         0x14    // escape mode register
#define MDRESOL         0x18    // main display image resolution register
#define MVPORCH         0x1C    // main vertical porch register
#define MHPORCH         0x20    // main horizontal porch register
#define MSYNC           0x24    // main sync area register
#define SDRESOL         0x28    // sub display image resolution register
#define INTSRC          0x2C    // interrupt source register
#define INTMSK          0x30    // interrupt mask register
#define PKTHDR          0x34    // packet header fifo register
#define PAYLOAD         0x38    // payload fifo register
#define RXFIFO          0x3C    // read fifo register
#define FIFOTHLD        0x40    // fifo threshold level register
#define FIFOCTRL        0x44    // fifo control register
#define MEMACCHR        0x48    // fifo memory AC characteristic register
#define PLLCTRL         0x4C    // pll control register
#define PLLTMR          0x50    // pll timer register
#define PHYACCHR        0x54    // d-phy AC characteristic register
#define PHYACCHR1       0x58    // secondary d-phy AC characteristic register
#define VIDTCON_BACKPORCHSHIFT 16
#define VIDTCON_BACKPORCHMASK 0xFF
#define VIDTCON_FRONTPORCHSHIFT 8
#define VIDTCON_FRONTPORCHMASK 0xFF
#define VIDTCON_SYNCPULSEWIDTHSHIFT 0
#define VIDTCON_SYNCPULSEWIDTHMASK 0xFF
#define VIDTCON2_LINEVALMASK 0x3FF
#define VIDTCON2_LINEVALSHIFT 0
#define VIDTCON2_HOZVALMASK 0x3FF
#define VIDTCON2_HOZVALSHIFT 16
#define VIDCON1_IVCLKSHIFT 3
#define VIDCON1_IHSYNCSHIFT 2
#define VIDCON1_IVSYNCSHIFT 1
#define VIDCON1_IVDENSHIFT 0
#define PLLCON0_M_SHIFT		3
#define PLLCON0_M_MASK		0x3FF
#define PLLCON0_M(x)		(((x) >> PLLCON0_M_SHIFT) & PLLCON0_M_MASK)
#define PLLCON0_P_SHIFT		14
#define PLLCON0_P_MASK		0x3F
#define PLLCON0_P(x)		(((x) >> PLLCON0_P_SHIFT) & PLLCON0_P_MASK)
#define PLLCON0_S_SHIFT		0
#define PLLCON0_S_MASK		0x7
#define PLLCON0_S(x)		(((x) >> PLLCON0_S_SHIFT) & PLLCON0_S_MASK)
#define CLOCK_BASE_HZ		48000000

typedef struct _LCD_INFORMATION {
    PCHAR Name;
    ULONG Unknown1;
    ULONG DrivingClockFrequency;
    ULONG DotPitch;
    ULONG Width;
    ULONG HorizontalBackPorch;
    ULONG HorizontalFrontPorch;
    ULONG HorizontalSyncPulseWidth;
    ULONG Height;
    ULONG VerticalBackPorch;
    ULONG VerticalFrontPorch;
    ULONG VerticalSyncPulseWidth;
    ULONG IVClock;
    ULONG IHSync;
    ULONG IVSync;
    ULONG IVDen;
    ULONG BitsPerPixel;
    ULONG Unknown17;
    ULONG Unknown18;
} LCD_INFORMATION, *PLCD_INFORMATION;


static LCD_INFORMATION LCDInfoTable[] = {
// iPhone 4
	{"n90", 0xA, 0x30EC6A0, 0x146, 640, 0x47, 0x47, 0x49, 960, 0xC, 0xC, 0x10, 0, 0, 0, 0, 24, 3, 0x8391643},
// iPad
//	{"k48", 0xA, 0x413B380, 0x84, 1024, 0x85, 0x85, 0x87, 768, 0xA, 0xA, 0xC, 0, 0, 0, 0, 18, 3, 0x644},
// iPod Touch 4G
//	{"n81", 0xA, 0x30EC6A0, 0x146, 640, 0x47, 0x47, 0x49, 960, 0xC, 0xC, 0x10, 0, 0, 0, 0, 24, 3, 0x35731643},
	{"n81", 0xA, 0x30EC6A0, 0x146, 640, 0x47, 0x47, 0x49, 960, 0xC, 0xC, 0x10, 0, 0, 0, 0, 24, 3, 0x8391643},
// aTV 2G
//	{"720p", 0xB, 0, 0xF3, 1280, 0xDC, 0x6E, 0x28, 720, 0x14, 5, 5, 0, 0, 0, 0, 24, 5, 0},
};

static PLCD_INFORMATION CurrentLcdInformation;
static ULONG TimePerMillionFrames = 0;


typedef enum {
    Off = 0,
    On
} ON_OFF;

ULONG
HwClockCalculatePllFrequency(
    ULONG Pll
    )
{
    ULONG Configuration;
    ULONGLONG Ret;

    if(Pll >= 4)
        return 0;

    Configuration = READ_REGISTER_ULONG(0xBF100000 + (Pll * 0x04));
    if(!Configuration & (1 << 31))
        return 0;

    Ret = (PLLCON0_M(Configuration) * (ULONGLONG)CLOCK_BASE_HZ) / (((ULONGLONG)PLLCON0_P(Configuration)) << PLLCON0_S(Configuration));
    return Ret;
}

VOID
HwClockGateSwitch(
    ULONG Gate,
    UCHAR State
    )
{
    ULONG Register;
    if(Gate > 0x3F) {
        return;
    }

    Register = (0xBF100000 + 0x1010) + (Gate << 2);

    if(State) {
        WRITE_REGISTER_ULONG(Register, READ_REGISTER_ULONG(Register) | 0xF);
    } else {
        WRITE_REGISTER_ULONG(Register, READ_REGISTER_ULONG(Register) & ~0xF);
    }

    while((READ_REGISTER_ULONG(Register) & 0xF) != ((READ_REGISTER_ULONG(Register) >> 4) & 0xF));

    return;
}

#define DISPLAY_PIPE 0x89000000
#define CLCD			0x89200000
#define RGBOUT			0x89600000

ULONG
FwHwDisplayPipeInitialize(
    VOID
    )
{
    ULONG ClcdRegister = 0x180;

    DebugTrace("entered\n");

    //
    // Set current LCD information.
    //

    HwClockGateSwitch(0xB, On);

    if(!CurrentLcdInformation) 
        CurrentLcdInformation = &LCDInfoTable[0];

    if(CurrentLcdInformation->Unknown1 == 0xA) {
        HwClockGateSwitch(0x12, On);
        HwClockGateSwitch(0xF, On);
    } else {
        return -1;
    }

    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x104C, READ_REGISTER_ULONG(DISPLAY_PIPE+0x104C) | 0x10);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x104C, (READ_REGISTER_ULONG(DISPLAY_PIPE+0x104C) & 0xFFFFF8FF) | 0x100);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x104C, (READ_REGISTER_ULONG(DISPLAY_PIPE+0x104C) & 0xF800FFFF) | 0x4000000);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x1030, (CurrentLcdInformation->Width << 16) | (CurrentLcdInformation->Height));
    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x250C, (ClcdRegister << 16) | 0x1F0);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x2060, 0x90);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x105C, 0x13880801);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE+0x2064, 0xBFF00000);
    if(ClcdRegister == 0x180) {
        WRITE_REGISTER_ULONG(CLCD + 0x0, 0x100);
        while(READ_REGISTER_ULONG(CLCD+0x0) & 0x100);

        WRITE_REGISTER_ULONG(CLCD + 0x0, 4);
        WRITE_REGISTER_ULONG(CLCD + 0x4, 3);
        WRITE_REGISTER_ULONG(CLCD + 0x14, 0x80000001);
        WRITE_REGISTER_ULONG(CLCD + 0x18, 0x20408);
        if(CurrentLcdInformation->BitsPerPixel <= 18)
            WRITE_REGISTER_ULONG(CLCD + 0x14, READ_REGISTER_ULONG(CLCD+0x14) | 0x1110000);
        WRITE_REGISTER_ULONG(CLCD + 0x50, 0);
        WRITE_REGISTER_ULONG(CLCD + 0x54, (CurrentLcdInformation->IVClock << VIDCON1_IVCLKSHIFT) | (CurrentLcdInformation->IHSync << VIDCON1_IHSYNCSHIFT) | 
                                          (CurrentLcdInformation->IVSync << VIDCON1_IVSYNCSHIFT) | (CurrentLcdInformation->IVDen << VIDCON1_IVDENSHIFT));
        WRITE_REGISTER_ULONG(CLCD + 0x58, (((CurrentLcdInformation->VerticalBackPorch - 1) << VIDTCON_BACKPORCHSHIFT) |
                                          ((CurrentLcdInformation->VerticalFrontPorch - 1) << VIDTCON_FRONTPORCHSHIFT) |
                                          ((CurrentLcdInformation->VerticalSyncPulseWidth - 1) << VIDTCON_SYNCPULSEWIDTHSHIFT)));
        WRITE_REGISTER_ULONG(CLCD + 0x5C, (((CurrentLcdInformation->HorizontalBackPorch - 1) << VIDTCON_BACKPORCHSHIFT) |
                                          ((CurrentLcdInformation->HorizontalFrontPorch - 1) << VIDTCON_FRONTPORCHSHIFT) | 
                                          ((CurrentLcdInformation->HorizontalSyncPulseWidth - 1) << VIDTCON_SYNCPULSEWIDTHSHIFT)));
        WRITE_REGISTER_ULONG(CLCD + 0x60, ((CurrentLcdInformation->Width - 1) << VIDTCON2_HOZVALSHIFT | 
                                          (CurrentLcdInformation->Height - 1) << VIDTCON2_LINEVALSHIFT));
        TimePerMillionFrames = 1000000 * (CurrentLcdInformation->VerticalBackPorch + CurrentLcdInformation->VerticalFrontPorch + CurrentLcdInformation->VerticalSyncPulseWidth + CurrentLcdInformation->Height)
                                       * (CurrentLcdInformation->HorizontalBackPorch + CurrentLcdInformation->HorizontalFrontPorch + CurrentLcdInformation->HorizontalSyncPulseWidth + CurrentLcdInformation->Width)
                                       / (CurrentLcdInformation->DrivingClockFrequency);
    } else {
        /* NOT IMPLEMENTED YET */
    }

    //
    // Set framebuffer address
    //

    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x4040, (0 << 8) | 1);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x4044, 0x5F700000);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x4050, 0);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x4060, (CurrentLcdInformation->Width << 16) | (CurrentLcdInformation->Height));
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x2040, 0xFFFF0202);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x404C, 1);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x4074, 0x200060);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x4078, 32);
    WRITE_REGISTER_ULONG(DISPLAY_PIPE + 0x1038, READ_REGISTER_ULONG(DISPLAY_PIPE + 0x1038) | 0x100);

    /* TODO: Finish */
}
