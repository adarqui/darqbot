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
#include "mod_cmph.h"

void
__cmph_init__ (void)
{

  strlcpy_buf (mod_cmph_info.name, "mod_cmph");
  strlcpy_buf (mod_cmph_info.trigger, "^cmph");

  mod_cmph_info.init = cmph_init;
  mod_cmph_info.fini = cmph_fini;
  mod_cmph_info.help = cmph_help;
  mod_cmph_info.run = cmph_run;


  mod_cmph_info.output = NULL;
  mod_cmph_info.input = NULL;


  debug (NULL, "__cmph_init__: Loaded mod_cmph\n");

  return;
}



bot_t *
cmph_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "cmph_init: Entered\n");

  swap_inmem_get_assign_and_remove ("dl_mod_cmph", 0,
				    (void **) &dl_mod_cmph_unique);

  return NULL;
}

bot_t *
cmph_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "cmph_fini: Entered\n");

  swap_inmem_create_and_add ("dl_mod_cmph", 0, dl_mod_cmph_unique);

  return NULL;
}

bot_t *
cmph_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "cmph_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help =
    "^cmph || ^cmph(build:find:clear:size:list) ...";

  return NULL;
}

bot_t *
cmph_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "cmph_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "cmph_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;


  MOD_OPTIONS_TOP_HALF;

  if (!strncasecmp_len (dl_options_ptr, "clear"))
    {
      opt = MOD_CMPH_CLEAR;
    }
  else if (!strncasecmp_len (dl_options_ptr, "size"))
    {
      opt = MOD_CMPH_SIZE;
    }
  else if (!strncasecmp_len (dl_options_ptr, "find"))
    {
      opt = MOD_CMPH_FIND;
    }
  else if (!strncasecmp_len (dl_options_ptr, "build"))
    {
      opt = MOD_CMPH_BUILD;
    }
  else if (!strncasecmp_len (dl_options_ptr, "list"))
    {
      opt = MOD_CMPH_LIST;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = cmph_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
cmph_change_string (bot_t * bot, char *string, int opt)
{
  unique_t *bu = NULL;
cmphx_t ** cmphx=NULL;
  char *str = NULL;
  char *sep_ptr;

  if (!string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  bu =
    unique_create (bot, &dl_mod_cmph_unique, UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
  if (!bu)
    return NULL;

  cmphx = (cmphx_t **) &bu->data;

switch(opt) {
case MOD_CMPH_CLEAR:
str = cmph_op_clear(bot, cmphx, NULL);
break;
case MOD_CMPH_SIZE:
str = cmph_op_size(bot, cmphx, NULL);
break;
case MOD_CMPH_BUILD:
str = cmph_op_build(bot, cmphx, string);
break;
case MOD_CMPH_LIST:
break;
case MOD_CMPH_FIND:
str = cmph_op_find(bot, cmphx, string);
break;
default:
break;
}

  return str;
}







char * cmph_op_clear(bot_t * bot, cmphx_t ** cmphx, char * string) {
cmphx_t * cmphx_ptr=NULL;
char * str=NULL;

if(!bot || !cmphx)
return NULL;

cmphx_ptr = *cmphx;
/*
if(!cmphx_ptr) return NULL;

if(!cmphx_ptr->hash) return NULL;

if(cmphx_ptr->config) 
cmph_config_destroy(cmphx_ptr->config);

if(cmphx_ptr->source)
cmph_io_vector_adapter_destroy(cmphx_ptr->source);

if(cmphx_ptr->fp)
fclose(cmphx_ptr->fp);


cmph_destroy(cmphx_ptr->hash);
*/

memset(cmphx_ptr,0,sizeof(cmphx_t));
//free(cmphx_ptr);
//*cmphx = NULL;

puts("GRRRRRRRRRR");
sleep(5);
return str;
}


char * cmph_op_size(bot_t * bot, cmphx_t ** cmphx, char * string) {
cmphx_t * cmphx_ptr=NULL;
int sz=0;
char * str = NULL;

if(!bot || !cmphx ) return NULL;

cmphx_ptr = *cmphx;
if(!cmphx_ptr) return NULL;

if(!cmphx_ptr->hash) return NULL;

/*
brz_data = (__brz_data_t *)cmphx_ptr->hash->data;
/if(!brz_data) return NULL;
SCREW THIS LIBRARY :D
cmph_t's data is private
*/

sz = cmph_size(cmphx_ptr->hash);
str = str_unite("%i", sz);

return str;
}



char * cmph_op_list(bot_t * bot, cmphx_t ** cmphx, char * string) {
char * str=NULL;

return str;
}


char * cmph_op_build(bot_t * bot, cmphx_t ** cmphx, char * string) {
cmphx_t *cmphx_ptr=NULL;
char * str=NULL;

char ** keys=NULL;
int nkeys=0,i=0;

debug(NULL, "cmph_op_build: Entered: %p %p %p\n", bot, cmphx, string);

if(!bot || !cmphx || !sNULL(string)) 
return NULL;

cmphx_ptr = *cmphx;

if(!cmphx_ptr) {
cmphx_ptr = (cmphx_t *) calloc(1,sizeof(cmphx_t));
if(!cmphx_ptr) return NULL;
*cmphx = cmphx_ptr;
}

if(cmphx_ptr->hash) {
cmph_op_clear(bot, cmphx, string);
}

keys = tokenize_array(NULL, string, TOKENIZE_NORMAL | TOKENIZE_EATWHITESPACE, " ", &nkeys);
if(!keys) goto cleanup;

tokenize_sort_strings(keys, &nkeys, TOKENIZE_SORT_STRINGS_FORWARD | TOKENIZE_SORT_STRINGS_UNIQ);

cmphx_ptr->fp = fopen("/tmp/cmph.mph", "w");
if(!cmphx_ptr->fp) goto cleanup;

cmphx_ptr->source  = cmph_io_vector_adapter((char **) keys, nkeys);
if(!cmphx_ptr->source) goto cleanup;

cmphx_ptr->config = cmph_config_new(cmphx_ptr->source);
if(!cmphx_ptr->config) goto cleanup;

cmph_config_set_algo(cmphx_ptr->config, CMPH_BRZ);
cmph_config_set_tmp_dir(cmphx_ptr->config, (cmph_uint8 *)"/tmp/");
cmph_config_set_mphf_fd(cmphx_ptr->config, cmphx_ptr->fp);
cmphx_ptr->hash = cmph_new(cmphx_ptr->config);
if(!cmphx_ptr->hash) goto cleanup;

cmph_config_destroy(cmphx_ptr->config);
cmphx_ptr->config = NULL;

cmph_dump(cmphx_ptr->hash, cmphx_ptr->fp);

cmph_destroy(cmphx_ptr->hash);

fclose(cmphx_ptr->fp);

cmphx_ptr->fp = fopen("/tmp/cmph.mph", "r");

tokenize_destroy_array(NULL, keys);

debug(NULL, "cmph_op_build: Success\n");

cmphx_ptr->hash = cmph_load(cmphx_ptr->fp);

return str;

/* error */
cleanup:
cmph_op_clear(bot, cmphx, string);

debug(NULL, "cmph_op_build: Failure\n");

return str;
}



char * cmph_op_find(bot_t * bot, cmphx_t ** cmphx, char * string) {
cmphx_t * cmphx_ptr=NULL;
unsigned int key_id=0;
char * str=NULL;


debug(NULL, "cmph_op_find: Entered\n");

if(!bot || !cmphx || !sNULL(string)) 
return NULL;

cmphx_ptr = *cmphx;
if(!cmphx_ptr) 
return NULL;

if(!cmphx_ptr->hash) return NULL;

key_id = cmph_search(cmphx_ptr->hash, string, strlen(string));
if(key_id < 0) return NULL;

str = str_unite("key [%s] found at slot %i", string, key_id);


return str;
}




