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
#include "mod_fnsasmx86.h"

void
__fnsasmx86_init__ (void)
{

strlcpy_buf(mod_fnsasmx86_info.name, "mod_fnsasmx86");
strlcpy_buf(mod_fnsasmx86_info.trigger, "^fnsasmx86");

  mod_fnsasmx86_info.init = fnsasmx86_init;
  mod_fnsasmx86_info.fini = fnsasmx86_fini;
  mod_fnsasmx86_info.help = fnsasmx86_help;
  mod_fnsasmx86_info.run = fnsasmx86_run;


  mod_fnsasmx86_info.output = NULL;
  mod_fnsasmx86_info.input = NULL;


  debug (NULL, "__fnsasmx86_init__: Loaded mod_fnsasmx86\n");

  return;
}



bot_t *
fnsasmx86_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fnsasmx86_init: Entered\n");
  return NULL;
}

bot_t *
fnsasmx86_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fnsasmx86_fini: Entered\n");

  fnsasmx86_off (bot);

  return NULL;
}

bot_t *
fnsasmx86_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "fnsasmx86_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^fnsasmx86";

  return NULL;
}

bot_t *
fnsasmx86_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "fnsasmx86_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "fnsasmx86_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;
  opt = MOD_FNSASMX86_TEST;

  MOD_OPTIONS_TOP_HALF;

  if (!strcasecmp (dl_options_ptr, "test"))
    {
      opt = MOD_FNSASMX86_TEST;
    }
  else if (!strcasecmp (dl_options_ptr, "on"))
    {
      opt = MOD_FNSASMX86_ON;
    }
  else if (!strcasecmp (dl_options_ptr, "off"))
    {
      opt = MOD_FNSASMX86_OFF;
    }

  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = fnsasmx86_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
fnsasmx86_change_string (bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char *sep_ptr;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  if (opt == MOD_FNSASMX86_TEST)
    {
     str=  fnsasmx86_test (bot);
    }
  else if (opt == MOD_FNSASMX86_ON)
    {
      fnsasmx86_on (bot);
    }
  else if (opt == MOD_FNSASMX86_OFF)
    {
      fnsasmx86_off (bot);
    }

  return str;
}



void
fnsasmx86_on (bot_t * bot)
{

  debug (bot, "fnsasmx86_on: Entered\n");

  fn_true = asmx86_true;
  fn_false = asmx86_false;
  fn_val_int = asmx86_val_int;
  fn_val_string = asmx86_val_string;
  fn_strlen = asmx86_strlen;
  fn_memset = asmx86_memset;
  fn_bzero = asmx86_bzero;
  fn_strcmp = asmx86_strcmp;
  fn_strncmp = asmx86_strncmp;

  return;
}



void
fnsasmx86_off (bot_t * bot)
{

  debug (bot, "fnsasmx86_off: Entered\n");

  fns_load_defaults ();

  return;
}

char *
fnsasmx86_test (bot_t * bot)
{
dlist_t * dl_text=NULL;
  char buf[132], *str=NULL;

  strlcpy_buf (buf, "hey");

  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_true() = %i\n", asmx86_true ());
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_false() = %i\n",
	     asmx86_false ());
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_val_int(99) = %i\n",
	     asmx86_val_int (99));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_val_string(\"hello\") = %s\n",
	     asmx86_val_string ("hello"));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strlen(\"hello\") = %i\n",
	     asmx86_strlen ("hello"));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strlen(\"yo\") = %i\n",
	     asmx86_strlen ("yo"));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strlen(\"\") = %i\n",
	     asmx86_strlen (""));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: buf=%s\n", buf);
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_memset(buf,0,..) = %s\n",
	     asmx86_memset (buf, 0, sizeof (buf)));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_memset(buf,0x41,..) = %s\n",
	     asmx86_memset (buf, 0x41, sizeof (buf) - 1));
  asmx86_bzero (buf, sizeof (buf));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_bzero(buf) = %s\n", buf);
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strcmp(\"hi\", \"yo\") = %i\n",
	     asmx86_strcmp ("hi", "yo"));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strcmp(\"yo\", \"yo\") = %i\n",
	     asmx86_strcmp ("yo", "yo"));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strncmp(\"hi\", \"yo\") = %i\n",
	     asmx86_strncmp ("hi", "yo", 2));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strncmp(\"yo\", \"yo\") = %i\n",
	     asmx86_strncmp ("yo", "yo", 2));
  dl_str_unite (&dl_text, "=> fnsasmx86_test: asmx86_strncmp(\"hey\", \"yo\") = %i\n",
	     asmx86_strncmp ("hey", "yo", 3));
  dl_str_unite (&dl_text,
	     "=> fnsasmx86_test: asmx86_strncmp(\"yoyo\", \"yoyo\") = %i\n",
	     asmx86_strncmp ("yoyo", "yoyo", 3));

str = dlist_to_str(dl_text);
dl_str_destroy(&dl_text);

  return str;
}




int
asmx86_true (void)
{
  int ret;
__asm__ ("movl $1, %0\n":"=r" (ret));
  return ret;
}

int
asmx86_false (void)
{
  int ret;

__asm__ ("movl $0, %0\n":"=r" (ret));

  return ret;
}

int
asmx86_val_int (int val)
{
  int ret;

__asm__ ("movl %1, %0\n": "=r" (ret):"m" (val));

  return ret;
}

char *
asmx86_val_string (char *val)
{
  char *ret = NULL;

__asm__ ("movl %1, %0\n": "=r" (ret):"m" (val));

  return ret;
}


size_t
asmx86_strlen (const char *str)
{
  int ret;

__asm__ ("movl %1, %%edx\n" "movl $0, %0\n" "cmpb %0, (%%edx)\n" "je out\n" "redo:\n" "inc %%edx\n" "inc %0\n" "movb (%%edx), %%al\n" "cmpb $0, %%al\n" "jne redo\n" "out:\n": "=r" (ret): "m" (str):"%eax", "%edx",
	   "cc");

  return ret;
}


void *
asmx86_memset (void *ptr, int mem, size_t len)
{
  void *ret = ptr;

__asm__ ("leal %0, %%edx\n" "cmp $0, %%edx\n" "je end1\n" "redo1:\n" "movb %1, (%%edx)\n" "inc %%edx\n" "dec %2\n" "cmp $0, %2\n" "jne redo1\n" "end1:\n":
: "m" (ptr), "r" (mem), "r" (len):"%edx", "cc", "memory");

  return ret;
}


void
asmx86_bzero (void *ptr, size_t len)
{

__asm__ ("leal %0, %%edx\n" "cmp $0, %%edx\n" "je end2\n" "redo2:\n" "movb $0, (%%edx)\n" "inc %%edx\n" "dec %1\n" "cmp $0, %1\n" "jne redo2\n" "end2:\n":
: "m" (ptr), "r" (len):"%edx", "cc", "memory");

  return;
}


int
asmx86_strcmp (const char *s1, const char *s2)
{
  int ret;

__asm__ ("pushl %1\n" "call asmx86_strlen\n" "addl $4, %%esp\n" "movl %%eax, %%ecx\n" "leal %1, %%edi\n" "leal %2, %%esi\n" "movl $0, %0\n" "repe cmpsb\n" "cmovne %%ecx, %0\n": "=r" (ret): "m" (s1), "m" (s2):"%eax", "%ecx", "%edi", "%esi", "cc",
	   "memory");

  return ret;
}



int
asmx86_strncmp (const char *s1, const char *s2, size_t len)
{
  int ret;

__asm__ ("leal %1, %%edi\n" "leal %2, %%esi\n" "movl $0, %0\n" "movl %3, %%ecx\n" "repe cmpsb\n" "cmovne %%ecx, %0\n": "=r" (ret): "m" (s1), "m" (s2), "m" (len):"%edx", "%ecx", "%edi", "%esi", "cc",
	   "memory");

  return ret;
}
