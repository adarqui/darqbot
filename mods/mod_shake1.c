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
#include "mod_shake1.h"

void
__shake1_init__ (void)
{

strlcpy_buf(mod_shake1_info.name, "mod_shake1");
strlcpy_buf(mod_shake1_info.trigger, "^shake1");

  mod_shake1_info.init = shake1_init;
  mod_shake1_info.fini = shake1_fini;
  mod_shake1_info.help = shake1_help;
  mod_shake1_info.run = shake1_run;


  mod_shake1_info.output = NULL;
  mod_shake1_info.input = NULL;

  debug (NULL, "__shake1_init__: Loaded mod_shake1\n");

  memset (&shake1_info, 0, sizeof (shake1_info));

  return;
}



bot_t *
shake1_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "shake1_init: Entered\n");

  if (shake1_info.initialized)
    return NULL;

  shake1_fill_toc (bot);

  return NULL;
}

bot_t *
shake1_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "shake1_fini: Entered\n");

  shake1_info.initialized = 0;

  if (shake1_info.fp_1)
    fclose (shake1_info.fp_1);
  if (shake1_info.fp_1_offsets)
    free (shake1_info.fp_1_offsets);

  if (shake1_info.fp_2)
    fclose (shake1_info.fp_2);
  if (shake1_info.fp_2_offsets)
    free (shake1_info.fp_2_offsets);

  if (shake1_info.fp_3)
    fclose (shake1_info.fp_3);
  if (shake1_info.fp_3_offsets)
    free (shake1_info.fp_3_offsets);

  memset (&shake1_info, 0, sizeof (shake1_info));

  return NULL;
}

bot_t *
shake1_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "shake1_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^shake1";

  return NULL;
}

bot_t *
shake1_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "shake1_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);


  debug (bot,
	     "shake1_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);



  if (bot_shouldreturn (bot))
    return NULL;



  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = shake1_change_string (l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}


char *
shake1_get_sym (FILE * fp, off_t * offsets, int num)
{
  off_t off;
  static char buf[1024];

  if (!fp || !offsets || num < 0)
    return NULL;

bz(buf);

  off = offsets[num];

  fseek (fp, off, SEEK_SET);
  fgets (buf, sizeof (buf) - 1, fp);
  strstrip_nl (buf);
  charcat_safe (buf, ' ', sizeof (buf) - 1);

  return buf;
}


char *
shake1_change_string (char *string, int opt)
{
  int i, j, k;
  char *str, buf[MAX_BUF_SZ];

  char *sep_ptr;

  str = NULL;

  if (!string)
    return str;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  i = (rand () % shake1_info.fp_1_lines);
  j = (rand () % shake1_info.fp_2_lines);
  k = (rand () % shake1_info.fp_3_lines);

  str = shake1_get_sym (shake1_info.fp_1, shake1_info.fp_1_offsets, i);
  strlcat_bot (buf, str);

  str = shake1_get_sym (shake1_info.fp_2, shake1_info.fp_2_offsets, j);
  strlcat_bot (buf, str);


  str = shake1_get_sym (shake1_info.fp_3, shake1_info.fp_3_offsets, k);
  strlcat_bot (buf, str);

  if (sNULL(buf)!=NULL)
    str = strdup (buf);

  return str;
}



void
shake1_print_info (bot_t * bot)
{

  debug (bot, "shake1_print_info: 1_lines=%i, 2_lines=%i, 3_lines=%i\n",
	     shake1_info.fp_1_lines, shake1_info.fp_2_lines,
	     shake1_info.fp_3_lines);

  return;
}


int
shake1_fill_toc_info (bot_t * bot, FILE * fp, int *lines, off_t ** offsets)
{
  off_t off;
  off_t *offp, *offp_p;
  char buf[1024];
  int n;

  if (!fp || !lines || !offsets)
    return -1;


  n = 0;
  while (1)
    {
      if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	break;
      n++;
    }


  *lines = n;

  fseek (fp, 0, SEEK_SET);

  offp = (off_t *) malloc (sizeof (off_t) * (n + 4));

  offp_p = offp;
  n = 0;
  while (1)
    {
      off = ftell (fp);
      offp[n] = off;
      if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	break;

      n++;
    }

  offp_p = offp;
  *offsets = offp;

  return 0;
}


void
shake1_fill_toc (bot_t * bot)
{
  char *str;

  if (shake1_info.initialized)
    return;

  if (shake1_info.fp_1)
    fclose (shake1_info.fp_1);
  if (shake1_info.fp_2)
    fclose (shake1_info.fp_2);
  if (shake1_info.fp_3)
    fclose (shake1_info.fp_3);

  shake1_info.fp_1 = shake1_info.fp_2 = shake1_info.fp_3 = NULL;

  str = str_unite_static ("%s/mods/mod_shake1_files/1", gi->confdir);

  if (!str)
    goto cleanup;

  shake1_info.fp_1 = fopen (str, "r");


  if (!shake1_info.fp_1)
    goto cleanup;

  shake1_fill_toc_info (bot, shake1_info.fp_1, &shake1_info.fp_1_lines,
			&shake1_info.fp_1_offsets);

  str = str_unite_static ("%s/mods/mod_shake1_files/2", gi->confdir);
  if (!str)
    goto cleanup;

  shake1_info.fp_2 = fopen (str, "r");
  if (!shake1_info.fp_2)
    goto cleanup;

  shake1_fill_toc_info (bot, shake1_info.fp_2, &shake1_info.fp_2_lines,
			&shake1_info.fp_2_offsets);


  str = str_unite_static ("%s/mods/mod_shake1_files/3", gi->confdir);
  if (!str)
    goto cleanup;

  shake1_info.fp_3 = fopen (str, "r");
  if (!shake1_info.fp_3)
    goto cleanup;


  shake1_fill_toc_info (bot, shake1_info.fp_3, &shake1_info.fp_3_lines,
			&shake1_info.fp_3_offsets);

cleanup:

  return;
}
