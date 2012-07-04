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
#include "mod_bitvec.h"

void
__bitvec_init__ (void)
{

  strlcpy_buf (mod_bitvec_info.name, "mod_bitvec");
  strlcpy_buf (mod_bitvec_info.trigger, "^bv");

  module_add_subtrigs (&mod_bitvec_info, "^bv");

  mod_bitvec_info.init = bitvec_init;
  mod_bitvec_info.fini = bitvec_fini;
  mod_bitvec_info.help = bitvec_help;
  mod_bitvec_info.run = bitvec_run;


  mod_bitvec_info.output = NULL;
  mod_bitvec_info.input = NULL;


  debug (NULL, "__bitvec_init__: Loaded mod_bitvec\n");

  return;
}



bot_t *
bitvec_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "bitvec_init: Entered\n");

  swap_inmem_get_assign_and_remove ("mod_bitvec_bv", 1,
				    (void **) &mod_bv_unique);

  return NULL;
}

bot_t *
bitvec_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "bitvec_fini: Entered\n");

  swap_inmem_create_and_add ("mod_bitvec_bv", 1, mod_bv_unique);

  return NULL;
}

bot_t *
bitvec_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "bitvec_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help =
    "^bitvec || ^bv(init,fini,set,clr,or,xor,and,not,eq,zero,copy) ...";

  return NULL;
}

bot_t *
bitvec_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;

  debug (bot, "bitvec_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "bitvec_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;

  if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "init", NULL))
    {
      opt = BITVEC_OPT_INIT;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "fini", NULL))
    {
      opt = BITVEC_OPT_FINI;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "set", NULL))
    {
      opt = BITVEC_OPT_SET;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "clr", NULL))
    {
      opt = BITVEC_OPT_CLR;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "or", NULL))
    {
      opt = BITVEC_OPT_OR;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "xor", NULL))
    {
      opt = BITVEC_OPT_XOR;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "and", NULL))
    {
      opt = BITVEC_OPT_AND;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "not", NULL))
    {
      opt = BITVEC_OPT_NOT;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "eq", NULL))
    {
      opt = BITVEC_OPT_EQ;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "zero", NULL))
    {
      opt = BITVEC_OPT_ZERO;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "copy", NULL))
    {
      opt = BITVEC_OPT_COPY;
    }
  else if (str_match (dl_options_ptr, STR_MATCH_STRCASECMP, 0, "info", NULL))
    {
      opt = BITVEC_OPT_INFO;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = bitvec_change_string (bot, l_str_ptr, opt);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
bitvec_change_string (bot_t * bot, char *string, int opt)
{
  unique_t *bu = NULL;
  mod_bv_data_t *mbvd = NULL;

  char *str = NULL;

  char *sep_ptr;

  debug (NULL, "bitvec_change_string: Entered, opt=%i\n", opt);

  if (!bot || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  bu = unique_create (bot, &mod_bv_unique, UNIQUE_ID_TAG | UNIQUE_ID_CHAN);
  if (!bu)
    return NULL;

  mbvd = (mod_bv_data_t *) bu->data;
  if (!mbvd)
    {
      mbvd = (mod_bv_data_t *) calloc (1, sizeof (mod_bv_data_t));
      bu->data = mbvd;
    }


  switch (opt)
    {
    case BITVEC_OPT_INIT:
      {
	str = bitvec_op_init (mbvd, string);
	break;
      }
    case BITVEC_OPT_FINI:
      {
	str = bitvec_op_fini (mbvd);
	break;
      }
    case BITVEC_OPT_INFO:
      {
	str = bitvec_op_info (mbvd);
	break;
      }
    case BITVEC_OPT_SET:
      {
	str = bitvec_op_set (mbvd, string);
	break;
      }
    case BITVEC_OPT_CLR:
      {
	str = bitvec_op_clr (mbvd, string);
	break;
      }
    case BITVEC_OPT_EQ:
      {
	str = bitvec_op_eq (mbvd, string);
	break;
      }
    case BITVEC_OPT_AND:
      {
	str = bitvec_op_and (mbvd, string);
	break;
      }
    case BITVEC_OPT_OR:
      {
	str = bitvec_op_or (mbvd, string);
	break;
      }
    case BITVEC_OPT_XOR:
      {
	str = bitvec_op_xor (mbvd, string);
	break;
      }
    case BITVEC_OPT_NOT:
      {
	str = bitvec_op_not (mbvd, string);
	break;
      }
    case BITVEC_OPT_ZERO:
      {
	str = bitvec_op_zero (mbvd, string);
	break;
      }
    case BITVEC_OPT_COPY:
      {
	str = bitvec_op_copy (mbvd, string);
	break;
      }
    default:
      break;
    }

  return str;
}




char *
bitvec_op_init (mod_bv_data_t * mbvd, char *string)
{
  int size = 0;
  char *str = NULL;

  debug (NULL, "bitvec_op_init: Entered\n");

  if (!mbvd || !string)
    return NULL;

  size = atoi (string);

  if (size > 8)
    size = 8;
  else if (size <= 0)
    size = 1;

  str = bitvec_op_fini (mbvd);

  mbvd->mod_bitvec_bv_1 = bit_vec_init (size);
  if (!mbvd->mod_bitvec_bv_1)
    goto cleanup;

  mbvd->mod_bitvec_bv_2 = bit_vec_init (size);
  if (!mbvd->mod_bitvec_bv_2)
    goto cleanup;

  mbvd->mod_bitvec_bv_res = bit_vec_init (size);
  if (!mbvd->mod_bitvec_bv_res)
    goto cleanup;

  return str;

cleanup:
  if (mbvd->mod_bitvec_bv_1)
    bit_vec_fini (&mbvd->mod_bitvec_bv_1);
  if (mbvd->mod_bitvec_bv_2)
    bit_vec_fini (&mbvd->mod_bitvec_bv_2);
  if (mbvd->mod_bitvec_bv_res)
    bit_vec_fini (&mbvd->mod_bitvec_bv_res);

  return str;
}


char *
bitvec_op_fini (mod_bv_data_t * mbvd)
{


  debug (NULL, "bitvec_op_fini: Entered\n");

  if (!mbvd)
    return NULL;

  bit_vec_fini (&mbvd->mod_bitvec_bv_1);
  bit_vec_fini (&mbvd->mod_bitvec_bv_2);
  bit_vec_fini (&mbvd->mod_bitvec_bv_res);

  mbvd->mod_bitvec_bv_1 = NULL;
  mbvd->mod_bitvec_bv_2 = NULL;
  mbvd->mod_bitvec_bv_res = NULL;

  return NULL;
}


char *
bitvec_op_info (mod_bv_data_t * mbvd)
{
  char *str = NULL, *str_1 = NULL, *str_2 = NULL, *str_3 = NULL;

  debug (NULL, "bitvec_op_info: Entered\n");

  if (!mbvd)
    return NULL;

  str_1 = bit_vec_to_str (mbvd->mod_bitvec_bv_1);
  if (!str_1)
    goto cleanup;

  str_2 = bit_vec_to_str (mbvd->mod_bitvec_bv_2);
  if (!str_2)
    goto cleanup;

  str_3 = bit_vec_to_str (mbvd->mod_bitvec_bv_res);
  if (!str_3)
    goto cleanup;

  str = str_unite ("x: %s\ny: %s\nz: %s\n", str_1, str_2, str_3);

cleanup:

  if (str_1)
    free (str_1);
  if (str_2)
    free (str_2);
  if (str_3)
    free (str_3);

  return str;
}


char *
bitvec_op_set (mod_bv_data_t * mbvd, char *string)
{
  bit_vector_t *bv_ptr;
  char *str = NULL, *tok_bv, *tok_pos;
  int pos;

  debug (NULL, "bitvec_op_set: Entered: %s\n", string);

  if (!mbvd || !string)
    return NULL;

  tok_bv = strtok (string, " ");
  if (!tok_bv)
    return NULL;

  tok_pos = strtok (NULL, "");
  if (!tok_pos)
    return NULL;

  bv_ptr = bitvec_which (mbvd, tok_bv);
  if (!bv_ptr)
    return NULL;

  pos = atoi (tok_pos);

  bit_vec_set (bv_ptr, pos);

  return str;
}


char *
bitvec_op_clr (mod_bv_data_t * mbvd, char *string)
{
  bit_vector_t *bv_ptr;
  char *str = NULL, *tok_bv, *tok_pos;
  int pos;

  debug (NULL, "bitvec_op_clr: Entered\n");

  if (!mbvd || !string)
    return NULL;

  tok_bv = strtok (string, " ");
  if (!tok_bv)
    return NULL;

  tok_pos = strtok (NULL, "");
  if (!tok_pos)
    return NULL;


  bv_ptr = bitvec_which (mbvd, tok_bv);
  if (!bv_ptr)
    return NULL;

  pos = atoi (tok_pos);

  bit_vec_clr (bv_ptr, pos);

  return str;
}



char *
bitvec_op_eq (mod_bv_data_t * mbvd, char *string)
{
  bit_vector_t *bv_ptr;
  char *str = NULL, *tok_bv, *tok_num;

  debug (NULL, "bitvec_op_eq: Entered\n");

  if (!mbvd || !string)
    return NULL;

  tok_bv = strtok (string, " ");
  if (!tok_bv)
    return NULL;

  tok_num = strtok (NULL, "");
  if (!tok_num)
    return NULL;

  bv_ptr = bitvec_which (mbvd, tok_bv);
  if (!bv_ptr)
    return NULL;

  strstrip_chars (tok_num, " \r\n");

  bit_vec_from_str (bv_ptr, tok_num);

  return str;
}




char *
bitvec_op_not (mod_bv_data_t * mbvd, char *string)
{
  bit_vector_t *bv_ptr;
  char *str = NULL, *tok_bv;

  debug (NULL, "bitvec_op_not: Entered\n");

  if (!mbvd || !string)
    return NULL;

  tok_bv = strtok (string, " ");
  if (!tok_bv)
    return NULL;

  bv_ptr = bitvec_which (mbvd, tok_bv);
  if (!bv_ptr)
    return NULL;

  bit_vec_not (bv_ptr);

  return str;
}





char *
bitvec_op_zero (mod_bv_data_t * mbvd, char *string)
{
  bit_vector_t *bv_ptr;
  char *str = NULL, *tok_bv;

  debug (NULL, "bitvec_op_zero: Entered\n");

  if (!mbvd || !string)
    return NULL;

  tok_bv = strtok (string, " ");
  if (!tok_bv)
    return NULL;

  bv_ptr = bitvec_which (mbvd, tok_bv);
  if (!bv_ptr)
    return NULL;

  bit_vec_zero (bv_ptr);

  return str;
}





char *
bitvec_op_copy (mod_bv_data_t * mbvd, char *string)
{
  bit_vector_t *bv_ptr_1, *bv_ptr_2;
  char *str = NULL, *tok_bv_1, *tok_bv_2;

  debug (NULL, "bitvec_op_copy: Entered\n");

  if (!mbvd || !string)
    return NULL;

  tok_bv_1 = strtok (string, " ");
  if (!tok_bv_1)
    return NULL;

  strstrip_chars (tok_bv_1, " \r\n");

  bv_ptr_1 = bitvec_which (mbvd, tok_bv_1);
  if (!bv_ptr_1)
    return NULL;

  tok_bv_2 = strtok (NULL, "");

  strstrip_chars (tok_bv_2, " \r\n");

  bv_ptr_2 = bitvec_which (mbvd, tok_bv_2);
  if (!bv_ptr_2)
    return NULL;

  memcpy (bv_ptr_1->vec, bv_ptr_2->vec, bv_ptr_1->sz);

  return str;
}








char *
bitvec_op_and (mod_bv_data_t * mbvd, char *string)
{

  debug (NULL, "bitvec_op_and: Entered\n");

  if (!mbvd)
    return NULL;
  bit_vec_and (mbvd->mod_bitvec_bv_1, mbvd->mod_bitvec_bv_2,
	       mbvd->mod_bitvec_bv_res);
  return NULL;
}


char *
bitvec_op_or (mod_bv_data_t * mbvd, char *string)
{

  debug (NULL, "bitvec_op_or: Entered\n");

  if (!mbvd)
    return NULL;

  bit_vec_or (mbvd->mod_bitvec_bv_1, mbvd->mod_bitvec_bv_2,
	      mbvd->mod_bitvec_bv_res);
  return NULL;
}

char *
bitvec_op_xor (mod_bv_data_t * mbvd, char *string)
{

  debug (NULL, "bitvec_op_xor: Entered\n");

  if (!mbvd)
    return NULL;

  bit_vec_xor (mbvd->mod_bitvec_bv_1, mbvd->mod_bitvec_bv_2,
	       mbvd->mod_bitvec_bv_res);
  return NULL;
}





bit_vector_t *
bitvec_which (mod_bv_data_t * mbvd, char *tok_bv)
{
  bit_vector_t *bv_ptr = NULL;

  if (!mbvd || !tok_bv)
    return NULL;

  if (str_match (tok_bv, STR_MATCH_STRCASECMP, 0, "1", "x", "a", NULL))
    {
      bv_ptr = mbvd->mod_bitvec_bv_1;
    }
  else if (str_match (tok_bv, STR_MATCH_STRCASECMP, 0, "2", "y", "b", NULL))
    {
      bv_ptr = mbvd->mod_bitvec_bv_2;
    }
  else if (str_match (tok_bv, STR_MATCH_STRCASECMP, 0, "3", "z", "c", NULL))
    {
      bv_ptr = mbvd->mod_bitvec_bv_res;
    }
  else
    return NULL;

  return bv_ptr;
}
