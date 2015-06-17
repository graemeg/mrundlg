/*
	mrundlg - A application / program launcher for Motif/X
	Copyright (C) 2005   Matthew Holder (sixxgate@hotmail.com)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <string.h>
#include "stringhelp.h"

/**
 * Trims the whitespaces from the beginning and end of a string.
 *
 * @param (in, out) An allocated memory buffer that will hold the trimmed string
 * @param (in) The source string to be trimmed
 * @return A pointer to the trimmed string
 */
char *strtrim(char *dest, const char *src)
{
	const char *ret, *pos, *eos;	/* Return, Position, EndOfString */
	int ln, dln;			/* Length, Destination Length */
	
	/* Calculate the end of string location */
	ln = strlen(src);
	eos = ln + src;
	ret = src;
	dln = ln;
	
	for (pos = src; pos < eos; pos++)
	{
		if
		(
			(*pos == '\t') ||
			(*pos == ' ') ||
			(*pos == '\n') ||
			(*pos == '\f') ||
			(*pos == '\r')
	 	)
		{
			ret++;
		} else break;
	}
	for (pos = eos - 1; pos >= src; pos--)
	{
		if
		(
			(*pos == '\t') ||
			(*pos == ' ') ||
			(*pos == '\n') ||
			(*pos == '\f') ||
			(*pos == '\r')
	 	)
		{
			dln--;
		} else break;
	}
	memcpy(dest, ret, dln);
	*(dest + dln) = 0;
	return dest;
}
