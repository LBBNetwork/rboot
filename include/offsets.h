/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

    offsets.h

Abstract:

    Offsets for iPhone 4 iBSS 8b117
    
--*/

#ifndef _BOOTSTRAP_FUNCTIONS_
#define _BOOTSTRAP_FUNCTIONS_

#define OSLOADER_BASE                      (0x420F1000)

#define RAMDISK_BASE                      0x41000000
#define RAMDISK_SIZE                      0x28000

#if defined(_N90AP_) || defined(_N81AP_) || defined(_K48AP_) || defined(_K66AP_)
#define _S5L8930X_ 1
#endif

#if defined(_N88AP_)
#define _S5L8920X_ 1
#endif

#if defined(_N18AP_)
#define _S5L8922X_ 1
#endif

#if defined(_S5L8930X_)

// Base address
#define TARGET_BASEADDR                   (0x84000000)

#if defined(_N90AP_)

// Standard offsets
#define TARGET_FREE                       (0x12220+1)
#define TARGET_MALLOC                     (0x1222C+1)
#define TARGET_JUMP_TO                    (0x15E18+1)
#define TARGET_PRINTF                     (0x1B684+1)
#define TARGET_VPRINTF                    (0x1AF54+1)

// Command offsets
#define TARGET_CMD_LIST_BEGIN             (0x24000)
#define TARGET_CMD_LIST_END               (0x2402C)

// Task functions
#define TARGET_TASK_YIELD                 (0x16714+1)
#define TARGET_TASK_RUNNING               (0x24880)
#define TARGET_TASK_LIST                  (0x24918)

// AES offsets
#define TARGET_AES_CRYPTO_CMD             (0x17B50+1)

// BDev offsets
#define TARGET_BDEV_LIST                  (0x28440)

// Image offsets
#define TARGET_IMAGE_LIST                 (0x24850)

// Filesystem offsets
//#define TARGET_FS_MOUNT                   (0x0)
//#define TARGET_FS_UNMOUNT                 (0x0)
//#define TARGET_FS_LOAD_FILE               (0x0)

// Kernel offsets
#define TARGET_KERNEL_LOAD                (0x12D70+1)
#define TARGET_KERNEL_PHYMEM              (0x28980)
#define TARGET_KERNEL_BOOTARGS            (0x20B04)

// NVRAM offsets
#define TARGET_NVRAM_LIST                 (0x24958)

#elif defined(_N81AP_) 

// Task functions
#define TARGET_TASK_RUNNING               (0x22738)
#define TARGET_TASK_LIST                  (0x227D0)

// BDev offsets
#define TARGET_BDEV_LIST                  (0x26280)

// Kernel offsets
#define TARGET_KERNEL_PHYMEM              (0x267C0)

#endif

#endif

#if defined(_S5L8920X_)
#ifdef _N88AP_

// Base address
#define TARGET_BASEADDR                   (0x84000000)

// Standard offsets
#define TARGET_FREE                       (0xA5CC+1)
#define TARGET_MALLOC                     (0xA5D8+1)
#define TARGET_JUMP_TO                    (0xDED8+1)
#define TARGET_PRINTF                     (0x134CC+1)
#define TARGET_VPRINTF                    (0x0+1)

// Command offsets
#define TARGET_CMD_LIST_BEGIN             (0x19000)
#define TARGET_CMD_LIST_END               (0x1902C)
#define TARGET_CMD_RAMDISK                (0xDEAD+1)

// Task functions
#define TARGET_TASK_YIELD                 (0xE760+1)
#define TARGET_TASK_RUNNING               (0x19C00)
#define TARGET_TASK_LIST                  (0x19C98)

// AES offsets
#define TARGET_AES_CRYPTO_CMD             (0xF9B8+1)

// BDev offsets
#define TARGET_BDEV_LIST                  (0x1C300)

// Image offsets
#define TARGET_IMAGE_LIST                 (0x19BD0)

// Filesystem offsets
//#define TARGET_FS_MOUNT                   (0x0)
//#define TARGET_FS_UNMOUNT                 (0x0)
//#define TARGET_FS_LOAD_FILE               (0x0)

// Kernel offsets
#define TARGET_KERNEL_LOAD                (0xB11C+1)
#define TARGET_KERNEL_PHYMEM              (0x15DE4)
#define TARGET_KERNEL_BOOTARGS            (0x1C3C0)

// NVRAM offsets
#define TARGET_NVRAM_LIST                 (0x19CD4)
#endif
#endif

#if defined(_S5L8922X_)
#ifdef _N18AP_

// Base address
#define TARGET_BASEADDR                   (0x84000000)

// Standard offsets
#define TARGET_FREE                       (0xE048+1)
#define TARGET_MALLOC                     (0xE2CC+1)
#define TARGET_JUMP_TO                    (0x121AC+1)
#define TARGET_PRINTF                     (0x17664+1)
#define TARGET_VPRINTF                    (0x16F34+1)

// Command offsets
#define TARGET_CMD_LIST_BEGIN             (0x1E000)
#define TARGET_CMD_LIST_END               (0x1E02C)

// Task functions
#define TARGET_TASK_YIELD                 (0x12AA8+1)
#define TARGET_TASK_RUNNING               (0x1E984)
#define TARGET_TASK_LIST                  (0x1EA1C)

// AES offsets
#define TARGET_AES_CRYPTO_CMD             (0x13CE8+1)

// BDev offsets
#define TARGET_BDEV_LIST                  (0x21680)

// Image offsets
#define TARGET_IMAGE_LIST                 (0x1E954)

// Filesystem offsets
//#define TARGET_FS_MOUNT                   (0x0)
//#define TARGET_FS_UNMOUNT                 (0x0)
//#define TARGET_FS_LOAD_FILE               (0x0)

// Kernel offsets
#define TARGET_KERNEL_LOAD                (0xF5E4+1)
#define TARGET_KERNEL_PHYMEM              (0x21BC0)
#define TARGET_KERNEL_BOOTARGS            (0x1BA04)

// NVRAM offsets
#define TARGET_NVRAM_LIST                 (0x1EA5C)
#endif
#endif

#endif

