/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    platform.h

Abstract:

    iPhone3,1/8930 platform device definitions.
    
--*/

#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#ifdef _S5L8930X_

#define LOAD_ADDRESS        0x41000000
#define IBOOT_BASE_ADDRESS    0x5FF00000
#define IBEC_BASE_ADDRESS    0x5FF00000
#define LLB_BASE_ADDRESS    0x84000000
#define IBSS_BASE_ADDRESS    0x84000000
#define SYSTEM_KERNEL_PATH_XNU    "/System/Library/Caches/com.apple.kernelcaches/kernelcache"

//
// Hardware bases
//

#define GPIO_BASE                          (0xBFA00000)
#define FRAMEBUFFER_BASE                   0x5F700000

#if defined(_N90AP_) || defined(_N81AP_)
#define FRAMEBUFFER_HEIGHT    960
#define FRAMEBUFFER_WIDTH    640
#elif defined(_K48AP_)
#define FRAMEBUFFER_HEIGHT    768
#define FRAMEBUFFER_WIDTH    1024
#elif defined(_K66AP_)
#define FRAMEBUFFER_HEIGHT    720
#define FRAMEBUFFER_WIDTH    1280
#endif

#endif

#if defined(_S5L8920X_) || defined(_S5L8922X_)

#define LOAD_ADDRESS          0x41000000
#define IBOOT_BASE_ADDRESS    0x4FF00000
#define IBEC_BASE_ADDRESS     0x4FF00000
#define LLB_BASE_ADDRESS      0x84000000
#define IBSS_BASE_ADDRESS     0x84000000
#define SYSTEM_KERNEL_PATH_XNU    "/System/Library/Caches/com.apple.kernelcaches/kernelcache"

//
// Hardware bases
//

#define GPIO_BASE                          (0x83000000)
#define FRAMEBUFFER_BASE                   0x4FD00000
#define FRAMEBUFFER_HEIGHT    480
#define FRAMEBUFFER_WIDTH     320
#endif

#endif


