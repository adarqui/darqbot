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
#include "mod_uu.h"

void __uu_init__(void)
{

	strlcpy_buf(mod_uu_info.name, "mod_uu");
	strlcpy_buf(mod_uu_info.trigger, "^uu");

	mod_uu_info.init = uu_init;
	mod_uu_info.fini = uu_fini;
	mod_uu_info.help = uu_help;
	mod_uu_info.run = uu_run;

	mod_uu_info.output = NULL;
	mod_uu_info.input = NULL;

	debug(NULL, "__uu_init__: Loaded mod_uu\n");

	return;
}

bot_t *uu_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "uu_init: Entered\n");
	return NULL;
}

bot_t *uu_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "uu_fini: Entered\n");
	return NULL;
}

bot_t *uu_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "uu_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^uu(enc:dec)";

	return NULL;
}

bot_t *uu_run(dlist_t * dlist_node, bot_t * bot)
{
	char *l_str_ptr =
	    NULL, *dl_module_arg_after_options, *dl_options_ptr, *l_new_str =
	    NULL;
	int opt;

	int new_len, l_should_strzero = 0;

	debug(bot, "uu_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "uu_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (_bot_shouldreturn(bot))
		return NULL;

	opt = MOD_UU_ENC;

	MOD_OPTIONS_TOP_HALF;

	if (!strncasecmp_len(dl_options_ptr, "enc")) {
		opt = MOD_UU_ENC;
	} else if (!strncasecmp_len(dl_options_ptr, "dec")) {
		opt = MOD_UU_DEC;
		puts("DEC");
	}

	MOD_OPTIONS_BOTTOM_HALF;

	l_str_ptr = _eat_whitespace(dl_module_arg_after_options);
	if (!l_str_ptr || !strlen(l_str_ptr)) {
		l_str_ptr = bot->txt_data_out;
		l_should_strzero = 1;
	}
	if (!_sNULL(l_str_ptr))
		return bot;
	l_new_str = uu_change_string(l_str_ptr, opt, &new_len);
	if (l_new_str != NULL) {
		if (l_should_strzero) {
			strzero_bot(bot->txt_data_out);
		}
		memcpy_bot(bot->txt_data_out, l_new_str, new_len);
		free(l_new_str);
		l_new_str = NULL;
	}

	return bot;
}

char *uu_change_string(char *string, int opt, int *new_len)
{
	char *str = NULL;
	char buf[MAX_BUF_SZ];

	int len;
	int data_off = 0;

	char *sep_ptr;

	if (!string || !new_len || opt < 0)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	_memset(buf, 0, sizeof(buf));

	strlcpy_buf(buf, string);

	if (opt == MOD_UU_ENC) {
		str = xxencode(buf, strlen(buf));
		*new_len = strlen(str);
	} else if (opt == MOD_UU_DEC) {
		len = strlen(buf);
		if (xxdecode(buf, &len, &data_off) >= 0) {
			str = (char *)calloc(1, len + 1);
			memcpy(str, buf, len);
			*new_len = len;
		}
	}

	return str;
}

/* ENC is the basic 1 character encoding function to make a char printing */
#define ENC(c) ( set[ (c) & 077 ] )
#define DEC(c)  ( table[ (c) & 0177 ] )

static char set[] =
    "+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static char table[128];

void outdec_encode(char *, char *);
void outdec_decode(char *, int, char *, int *);

/*
 * copy from in to out, encoding as you go along.
 */
char *xxencode(char *in, int in_sz)
{
	char *out;
	int i, j, n, p;

	if (in_sz == 0)
		return (NULL);

	p = (in_sz * 4) + 10;

	out = malloc(p);

	_memset(out, 0, p);

	n = in_sz;

	for (i = 0, j = strlen(out); i < n; i += 3, j += 4) {
		outdec_encode(&in[i], out + j);
	}

	return (out);
}

/*
 * output one group of 3 bytes, pointed at by p, on file f.
 */
void outdec_encode(char *p, char *f)
{
	int c1, c2, c3, c4;

	c1 = *p >> 2;
	c2 = ((*p << 4) & 060) | ((p[1] >> 4) & 017);
	c3 = ((p[1] << 2) & 074) | ((p[2] >> 6) & 03);
	c4 = p[2] & 077;

	f[0] = ENC(c1);
	f[1] = ENC(c2);
	f[2] = ENC(c3);
	f[3] = ENC(c4);

	return;
}

/*
 * copy from in to out, decoding as you go along.
 */
int xxdecode(char *data, int *decoded_len, int *data_offset)
{
	char *bp;
	int n, new_n, dataoff = 0;

	if (!data || !decoded_len || !data_offset)
		return -1;

	if (*decoded_len < 0 || *data_offset < 0)
		return -1;

	/* make this 'data' */

	bp = table;		/* clear table */
	for (n = 128; n; --n) {
		*(bp++) = 0;
	};

	bp = set;		/* fill table */
	for (n = 64; n; --n) {
		table[*(bp++) & 0177] = 64 - n;
	};

	n = strlen(data);

	if (n <= 0)
		return -1;

	new_n = 0;

	*data_offset = dataoff;

	bp = &data[dataoff];
	while (n > 0) {
		outdec_decode(bp, n, data, &new_n);
		bp += 4;
		n -= 3;
	}

	/* took this out during SSH mods */
	data[new_n] = '\0';

	return 0;
}

/*
 * output a group of 3 bytes (4 input characters).
 * the input chars are pointed to by p, they are to
 * be output to file f.  n is used to tell us not to
 * output all of them at the end of the file.
 */
void outdec_decode(char *p, int n, char *f, int *cnt)
{
	int c1, c2, c3;

	c1 = DEC(*p) << 2 | DEC(p[1]) >> 4;
	c2 = DEC(p[1]) << 4 | DEC(p[2]) >> 2;
	c3 = DEC(p[2]) << 6 | DEC(p[3]);

	if (n >= 1) {
		f[*cnt] = c1;
		*cnt += 1;
	}
	if (n >= 2) {

		f[*cnt] = c2;
		*cnt += 1;
	}
	if (n >= 3) {
		f[*cnt] = c3;
		*cnt += 1;
	}

	return;
}
