/*
     Copyright (C) 2012 Andrew Darqui

     This file is part of darqbot.

     darqbot is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     darqbot is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with darqbot.  If not, see <http://www.gnu.org/licenses/>.

     Contact: [website: http://www.adarq.org]
      email: andrew.darqui@g m a i l . c o m
*/
#include "bot.h"

int matchchar_seed(int seed)
{
	if (seed != 0)
		srand(seed);
	return 0;
}

int matchchar(int c1, int c2)
{

	if (c1 == 0)
		return 1;

	if (c2 == ' ' && (c1 != MATCHCHAR_SPACE && c1 != MATCHCHAR_INVERT))
		return 0;

	if (c1 < MATCHCHAR_BASE) {
		return c1 == c2;
	}

	if (c1 == MATCHCHAR_ASCII) {
		if (isascii(c2))
			return 1;
	} else if (c1 == MATCHCHAR_ALPHA) {
		if (isalpha(c2))
			return 1;
	} else if (c1 == MATCHCHAR_ALNUM) {
		if (isalnum(c2))
			return 1;
	} else if (c1 == MATCHCHAR_DIGIT) {
		if (isdigit(c2))
			return 1;
	} else if (c1 == MATCHCHAR_LOWER) {
		if (islower(c2))
			return 1;
	} else if (c1 == MATCHCHAR_UPPER) {
		if (isupper(c2))
			return 1;
	} else if (c1 == MATCHCHAR_PUNCT) {
		if (ispunct(c2))
			return 1;
	} else if (c1 == MATCHCHAR_BINARY) {
		if (isbinary(c2))
			return 1;
	} else if (c1 == MATCHCHAR_SAME) {
		return 1;
	} else if (c1 == MATCHCHAR_SPACE) {
		if (c2 == ' ' || c2 == '\t')
			return 1;
	} else if (c1 == MATCHCHAR_INVERT) {
		return 1;
	} else
		return 0;

	return 0;
}

int matchchar_str(char *str)
{
	int x;

	if (!str) {
		return 0;
	}

	if (str[0] == ' ')
		return 0;

	x = str[0];

/*
  if (!strstr (str, "MATCH"))
    {
      return str[0];
    }
*/

	if (!strncasecmp(str, "MASCII", _strlen("MASCII"))) {
		x = MATCHCHAR_ASCII;
	} else if (!strncasecmp(str, "MALPHA", _strlen("MALPHA"))) {
		x = MATCHCHAR_ALPHA;
	} else if (!strncasecmp(str, "MALNUM", _strlen("MALNUM"))) {
		x = MATCHCHAR_ALNUM;
	} else if (!strncasecmp(str, "MDIGIT", _strlen("MDIGIT"))) {
		x = MATCHCHAR_DIGIT;
	} else if (!strncasecmp(str, "MLOWER", _strlen("MLOWER"))) {
		x = MATCHCHAR_LOWER;
	} else if (!strncasecmp(str, "MUPPER", _strlen("MUPPER"))) {
		x = MATCHCHAR_UPPER;
	} else if (!strncasecmp(str, "MPUNCT", _strlen("MPUNCT"))) {
		x = MATCHCHAR_PUNCT;
	} else if (!strncasecmp(str, "MBINARY", _strlen("MBINARY"))) {
		x = MATCHCHAR_BINARY;
	} else if (!strncasecmp(str, "MSPACE", _strlen("MSPACE"))) {
		x = MATCHCHAR_SPACE;
	} else if (!strncasecmp(str, "MSAME", _strlen("MSAME"))) {
		x = MATCHCHAR_SAME;
	} else if (!strncasecmp(str, "MINVERT", _strlen("MINVERT"))) {
		x = MATCHCHAR_INVERT;
	}

	return x;
}

char *matchchar_retstr(int c)
{
	char buf[2];

/*
  if (!strstr (str, "MATCH"))
    {
      return str[0];
    }
*/

	if (c == MATCHCHAR_ASCII)
		return "MASCII";
	else if (c == MATCHCHAR_ALPHA)
		return "MALPHA";
	else if (c == MATCHCHAR_ALNUM)
		return "MALNUM";
	else if (c == MATCHCHAR_DIGIT)
		return "MDIGIT";
	else if (c == MATCHCHAR_LOWER)
		return "MLOWER";
	else if (c == MATCHCHAR_UPPER)
		return "MUPPER";
	else if (c == MATCHCHAR_PUNCT)
		return "MPUNCT";
	else if (c == MATCHCHAR_BINARY)
		return "MBINARY";
	else if (c == MATCHCHAR_SPACE)
		return "MSPACE";
	else if (c == MATCHCHAR_SAME)
		return "MSAME";
	else if (c == MATCHCHAR_INVERT)
		return "MINVERT";

// FUNK FREE THE STRDUP'd STRING
	buf[0] = c;
	buf[1] = 0;

	return strdup(buf);
}
