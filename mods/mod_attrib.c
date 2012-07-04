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
#include "mod_attrib.h"

void
__attrib_init__ (void)
{

strlcpy_buf(mod_attrib_info.name, "mod_attrib");
strlcpy_buf(mod_attrib_info.trigger, "^attrib");

module_add_subtrigs(&mod_attrib_info, "^bold");
module_add_subtrigs(&mod_attrib_info, "^underline");
module_add_subtrigs(&mod_attrib_info, "^multi");

  mod_attrib_info.init = attrib_init;
  mod_attrib_info.fini = attrib_fini;
  mod_attrib_info.help = attrib_help;
  mod_attrib_info.run = attrib_run;


  mod_attrib_info.output = NULL;
  mod_attrib_info.input = NULL;

  debug (NULL, "__attrib_init__: Loaded mod_attrib\n");

  return;
}



bot_t *
attrib_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "attrib_init: Entered\n");
  return NULL;
}

bot_t *
attrib_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "attrib_fini: Entered\n");
  return NULL;
}

bot_t *
attrib_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "attrib_help: Entered\n");

  if (!bot)
    return NULL;

  if (str_match (bot->trig_called, STR_MATCH_STRCASECMP, 0, "^multi", NULL))
{
bot->dl_module_help = "^multi ::: allows result data to be displayed on multiple lines";
}
else {
  bot->dl_module_help = "^attrib ::: ^bold || ^underline || ^multi";
}

  return NULL;
}

bot_t *
attrib_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt,sub;

  if (!dlist_node || !bot)
    return NULL;


  stat_inc(bot,bot->trig_called);

  debug (bot, "attrib_run: Entered\n");

  debug (bot,
	     "attrib_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

sub = MOD_ATTRIB;
if(!strcasecmp(bot->trig_called, "^bold"))
sub = MOD_ATTRIB_BOLD;
else if(!strcasecmp(bot->trig_called, "^underline")) 
sub = MOD_ATTRIB_UNDERLINE;
else if(!strcasecmp(bot->trig_called, "^multi"))
sub = MOD_ATTRIB_MULTI;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF;
/*
  l_new_str = attrib_change_string (l_str_ptr, opt);
*/
switch(sub) {
case MOD_ATTRIB:{
l_new_str = attrib_change_string(bot,l_str_ptr,opt);
break;
}
case MOD_ATTRIB_BOLD:
{
l_new_str = bold_change_string(bot,l_str_ptr,opt);
break;
}
case MOD_ATTRIB_UNDERLINE:
{
l_new_str = underline_change_string(bot,l_str_ptr,opt);
break;
}
case MOD_ATTRIB_MULTI: {
l_new_str = multi_change_string(bot);
break;
}
default:
break;
}

  MOD_PARSE_BOTTOM_HALF;

  return bot;
}


char * attrib_change_string(bot_t * bot, char *string, int opt) 
{
return NULL;
}


char *
bold_change_string (bot_t * bot, char *string, int opt)
{
  char *sep_ptr;
  char *str = NULL;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  str = str_unite("%s%s%s", IRC_COLOR_BOLD_STR, string,
		  IRC_COLOR_RESET_STR);

  return str;
}


char *
underline_change_string (bot_t * bot, char *string, int opt)
{

  char *sep_ptr;
  char *str = NULL;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

        str = str_unite( "%s%s%s\n",
                        IRC_COLOR_UNDERLINE_STR, string,
                        IRC_COLOR_RESET_STR);

  return str;
}



char * multi_change_string(bot_t *bot) {

if(!bot) return NULL;

bot->var_multi = 1;

return NULL;

}

