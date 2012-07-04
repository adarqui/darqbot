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
#ifndef MATCHCHAR_H
#define MATCHCHAR_H

#include <ctype.h>

#define MATCHCHAR_DEF_SEED 41

enum
{
  MATCHCHAR_BASE = 55555,
  MATCHCHAR_ASCII,
  MATCHCHAR_ALPHA,
  MATCHCHAR_ALNUM,
  MATCHCHAR_DIGIT,
  MATCHCHAR_LOWER,
  MATCHCHAR_UPPER,
  MATCHCHAR_PUNCT,
  MATCHCHAR_BINARY,
  MATCHCHAR_SAME,
  MATCHCHAR_SPACE,
  MATCHCHAR_INVERT,
};

int matchchar (int, int);

int matchchar_str (char *);

int matchchar_seed (int);

char *matchchar_retstr (int);


#endif
