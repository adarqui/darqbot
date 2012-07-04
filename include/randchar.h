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
#ifndef RANDCHAR_H
#define RANDCHAR_H

#include <ctype.h>

#define RANDCHAR_DEF_SEED 41

enum
{
  RANDCHAR_BASE = 55555,
  RANDCHAR_ASCII,
  RANDCHAR_ALPHA,
  RANDCHAR_ALNUM,
  RANDCHAR_DIGIT,
  RANDCHAR_LOWER,
  RANDCHAR_UPPER,
  RANDCHAR_PUNCT,
  RANDCHAR_BINARY,
  RANDCHAR_SAME,
  RANDCHAR_SPACE,
};

int randchar (int, int);

int randchar_str (char *);

int randchar_seed (int);

char *randchar_retstr (int);


#endif
