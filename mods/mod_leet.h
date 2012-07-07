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
/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#ifndef MOD_LEET_H
#define MOD_LEET_H

#include "bot.h"

module_t mod_leet_info;

bot_t *leet_init(dlist_t *, bot_t *);
bot_t *leet_fini(dlist_t *, bot_t *);
bot_t *leet_help(dlist_t *, bot_t *);
bot_t *leet_run(dlist_t *, bot_t *);

typedef struct lchar {
	int c;
	char *n[10];
	char *sound[5];
} lchar_t;

lchar_t leet_chars[46] = {
	{'a', {"4", "@", "/\\", "/-\\"}, {"aye", "eh"}},
	{'b', {"6", "|3", "]3", "[3", "|o"}, {"bee"}},
	{'c', {"c", "<"}, {"sea", "see"}},
	{'d', {"d", "o|", "|>", "|)", "]>"}, {"dee"}},
	{'e', {"3"}},
	{'f', {"f"}, {"eff", "eph"}},
	{'g', {"g", "9"}, {"jee"}},
	{'h', {"h", "|-|", "}{", "}-{", "]-["}, {"aych"}},
	{'i', {"1", "!"}, {"eye"}},
	{'j', {"j"}, {"gay"}},
	{'k', {"k", "|<", "|{"}, {"kay"}},
	{'l', {"1", "|_"}, {"el"}},
	{'m', {"m", "/\\/\\", "|\\/|", "(\\/)"}, {"em"}},
	{'n', {"n", "/\\/", "|/|"}, {"en"}},
	{'o', {"0", "()"}, {"oh"}},
	{'p', {"p", "|'"}, {"pea", "pee"}},
	{'q', {"q"}, {"cue"}},
	{'r', {"r", "|2", "/2", "|z", "I2"}, {"are"}},
	{'s', {"5", "$"}, {"es"}},
	{'t', {"7", "+"}, {"tee", "tea"}},
	{'u', {"u", "|_|", "(_)"}, {"yew", "you", "yoo"}},
	{'v', {"v", "\\/"}, {"vee"}},
	{'w', {"w", "\\/\\/", "\\\\'", "\\x/"}, {"dubyew"}},
	{'x', {"x", "><"}, {"eckz"}},
	{'y', {"y", "`/", "`("}, {"why"}},
	{'z', {"z"}, {"zee"}},
	{'0', {NULL}, {"zero", "oh"}},
	{'1', {NULL}, {"wun", "won"}},
	{'2', {NULL}, {"tew", "too"}},
	{'3', {NULL}, {"chree"}},
	{'4', {NULL}, {"fo"}},
	{'5', {NULL}, {"five"}},
	{'6', {NULL}, {"sicks"}},
	{'7', {NULL}, {"seven"}},
	{'8', {NULL}, {"ate"}},
	{'9', {NULL}, {"nein"}},
	{'+', {NULL}, {"plus"}},
	{'.', {NULL}, {"dot"}},
	{'/', {NULL}, {"slash"}},
	{'\\', {NULL}, {"leanback"}},
	{'$', {NULL}, {"dollar"}},
	{'*', {NULL}, {"star"}},
	{'@', {NULL}, {"at"}},
	{'&', {NULL}, {"and"}},
	{'#', {NULL}, {"pound"}},
	{0, {NULL}, {NULL}}

};

char *leet_change_string(char *, int, int);
char *x_caps(char *);

void __leet_init__(void) __attribute__ ((constructor));

#endif
