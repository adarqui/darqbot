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

int randchar_seed(int seed)
{
	if (seed != 0)
		srand(seed);
	return 0;
}

int randchar(int c, int char_orig)
{
	int x;

	if (c < RANDCHAR_BASE) {
		return c;
	}

	x = c;

	for (;;) {
		x = rand();
		x = x % 255;

		if (c == RANDCHAR_ASCII) {
			if (isascii(x))
				break;
		} else if (c == RANDCHAR_ALPHA) {
			if (isalpha(x))
				break;
		} else if (c == RANDCHAR_ALNUM) {
			if (isalnum(x))
				break;
		} else if (c == RANDCHAR_DIGIT) {
			if (isdigit(x))
				break;
		} else if (c == RANDCHAR_LOWER) {
			if (islower(x))
				break;
		} else if (c == RANDCHAR_UPPER) {
			if (isupper(x))
				break;
		} else if (c == RANDCHAR_PUNCT) {
			if (ispunct(x))
				break;
		} else if (c == RANDCHAR_BINARY) {
			if (isbinary(x))
				break;
		} else if (c == RANDCHAR_SAME) {
			return char_orig;
		} else if (c == RANDCHAR_SPACE) {
			if (isspace(x))
				return ' ';
		}

		else
			return c;

	}

	return x;
}

int randchar_str(char *str)
{
	int x;

	if (!str) {
		return 0;
	}

	if (str[0] == ' ')
		return 0;

	x = str[0];

/*
  if (!strstr (str, "RAND"))
    {
      return str[0];
    }
*/

	if (!strncasecmp(str, "RASCII", fn_strlen("RASCII"))) {
		x = RANDCHAR_ASCII;
	} else if (!strncasecmp(str, "RALPHA", fn_strlen("RALPHA"))) {
		x = RANDCHAR_ALPHA;
	} else if (!strncasecmp(str, "RALNUM", fn_strlen("RALNUM"))) {
		x = RANDCHAR_ALNUM;
	} else if (!strncasecmp(str, "RDIGIT", fn_strlen("RDIGIT"))) {
		x = RANDCHAR_DIGIT;
	} else if (!strncasecmp(str, "RLOWER", fn_strlen("RLOWER"))) {
		x = RANDCHAR_LOWER;
	} else if (!strncasecmp(str, "RUPPER", fn_strlen("RUPPER"))) {
		x = RANDCHAR_UPPER;
	} else if (!strncasecmp(str, "RPUNCT", fn_strlen("RPUNCT"))) {
		x = RANDCHAR_PUNCT;
	} else if (!strncasecmp(str, "RBINARY", fn_strlen("RBINARY"))) {
		x = RANDCHAR_BINARY;
	} else if (!strncasecmp(str, "RSPACE", fn_strlen("RSPACE"))) {
		x = RANDCHAR_SPACE;
	} else if (!strncasecmp(str, "RSAME", fn_strlen("RSAME"))) {
		x = RANDCHAR_SAME;
	}

	return x;
}

char *randchar_retstr(int c)
{
	char buf[2];

/*
  if (!strstr (str, "MATCH"))
    {
      return str[0];
    }
*/

	if (c == RANDCHAR_ASCII)
		return "RASCII";
	else if (c == RANDCHAR_ALPHA)
		return "RALPHA";
	else if (c == RANDCHAR_ALNUM)
		return "RALNUM";
	else if (c == RANDCHAR_DIGIT)
		return "RDIGIT";
	else if (c == RANDCHAR_LOWER)
		return "RLOWER";
	else if (c == RANDCHAR_UPPER)
		return "RUPPER";
	else if (c == RANDCHAR_PUNCT)
		return "RPUNCT";
	else if (c == RANDCHAR_BINARY)
		return "RBINARY";
	else if (c == RANDCHAR_SPACE)
		return "RSPACE";
	else if (c == RANDCHAR_SAME)
		return "RSAME";

// FREE THE STRDUP'd STRING
	buf[0] = c;
	buf[1] = 0;

	return strdup(buf);
}
