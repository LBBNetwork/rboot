/*++

Copyright (C) 2012 The Little Beige Box, http://www.beige-box.com
Parts of this software are from the ReactOS project, copyright 1995-2012 ReactOS Foundation, and
the greenpois0n project, copyright 2010-2012 Chronic-Dev LLC.

This software is licensed under the GNU General Public License v3.

File name:

	crt.h

Abstract:

	CRT header.
	
--*/

#ifndef _CRT_H_
#define _CRT_H_

int _vsnprintf(char *buffer, size_t n, const char *format, va_list ap);

extern VOID (*_free)(PVOID ptr);
extern PVOID (*_malloc)(ULONG size);
extern INT (*_printf)(const PCHAR fmt, ...);

#endif
