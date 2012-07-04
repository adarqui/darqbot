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
#include "gmod_grelinkc.h"


void __grelinkc_init__ (void) __attribute__ ((constructor));

void
__grelinkc_init__ (void)
{

  strlcpy_buf (gmod_grelinkc_info.name, "gmod_grelinkc");
  strlcpy_buf (gmod_grelinkc_info.trigger, "^grelinkc");

  gmod_grelinkc_info.init = grelinkc_init;
  gmod_grelinkc_info.fini = grelinkc_fini;
  gmod_grelinkc_info.help = grelinkc_help;
  gmod_grelinkc_info.run = grelinkc_run;

  gmod_grelinkc_info.off = grelinkc_off;
  gmod_grelinkc_info.destroy_up = grelinkc_destroy_up;
  gmod_grelinkc_info.destroy_down = grelinkc_destroy_down;

  gmod_grelinkc_info.control_up = grelinkc_control_up;
  gmod_grelinkc_info.control_down = grelinkc_control_down;

  gmod_grelinkc_info.output = grelinkc_output;
  gmod_grelinkc_info.input = grelinkc_input;

  gmod_grelinkc_info.self = &gmod_grelinkc_info;

  debug (NULL, "__grelinkc_init__: Loaded gmod_grelinkc\n");

  return;
}



bot_t *
grelinkc_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "grelinkc_init: Entered\n");
  return NULL;
}

bot_t *
grelinkc_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "grelinkc_fini: Entered\n");
  return NULL;
}


bot_t *
grelinkc_off (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  grelinkc_t *grelinkc = NULL;

  debug (bot, "grelinkc_off: Entered\n");


  if (!dlist_node || !bot)
    {
      puts ("WTF 1");
      return NULL;
    }

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    {
      return NULL;
    }

  grelinkc = (grelinkc_t *) gmod->data;
  if (!grelinkc)
    {
      return NULL;
    }

  free (grelinkc);

  return NULL;
}





bot_t *
grelinkc_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "grelinkc_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^grelinkc";

  return NULL;
}


bot_t *
grelinkc_run (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;
  grelinkc_t *grelinkc = NULL;

  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "grelinkc_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "grelinkc_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  printf ("dl_module_arg=%s\n", bot->dl_module_arg);


  dptr_gmod =
    (dlist_t *) gmodule_find_gmod_dptr (bot, gmod_grelinkc_info.trigger,
					gmod_grelinkc_info.trigger_ext);
  if (!dptr_gmod)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    {
      puts ("GRELINKC GMOD=NULL");
      return NULL;
    }

  if (!gmod->data)
    {
      puts ("GIRCD GMOD->DATA = NULL, ALLOCATING");
      grelinkc_gmod_init (bot, gmod, dptr_gmod);
      grelinkc = gmod->data;
    }

  MOD_OPTIONS_TOP_HALF;

  grelinkc_process_options (grelinkc, dl_options_ptr);

  MOD_OPTIONS_BOTTOM_HALF;

  gmodule_set_can_pass_down (gmod);

  grelinkc_input (dptr_gmod, bot);

  gmodule_set_has_run (gmod);

  return bot;
}



char *
grelinkc_process_options (grelinkc_t * grelinkc, char *string)
{

  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  dlist_t *dl = NULL, *dptr;

  debug (NULL, "grelinkc_process_options: Entered\n");


  if (!grelinkc || !sNULL (string))
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));


  dl = tokenize (NULL, string, TOKENIZE_NORMAL, "...");
  if (!dl)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    grelinkc_process_options_parse (grelinkc, (char *) dlist_data (dptr));
  }


  tokenize_destroy (NULL, &dl);

  return str;
}






void
grelinkc_process_options_parse (grelinkc_t * grelinkc, char *string)
{

  if (!grelinkc || !sNULL (string))
    return;

  debug (NULL, "grelinkc_process_options_parse: Entered\n");

  if (!strncasecmp_len (string, "id"))
    {
      grelinkc_process_options_parse_id (grelinkc, &string[strlen ("id=")]);
    }
  else if (!strncasecmp_len (string, "line="))
    {
      grelinkc_process_options_parse_line (grelinkc,
					   &string[strlen ("line=")]);
    }

  gmodule_parse_common_options (grelinkc->bot, grelinkc->gmod, string);

  return;
}


void
grelinkc_process_options_parse_id (grelinkc_t * grelinkc, char *string)
{

  if (!grelinkc || !sNULL (string))
    return;

  grelinkc->id = atoi (string);

  return;
}



void
grelinkc_process_options_parse_line (grelinkc_t * grelinkc, char *string)
{

  if (!grelinkc || !sNULL (string))
    return;

  if (grelinkc->line)
    free (grelinkc->line);

  grelinkc->line = strdup (string);

  return;
}




char *
grelinkc_change_string (bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;


  if (!bot || !sNULL (string))
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  return str;
}






bot_t *
grelinkc_output (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  grelinkc_t *grelinkc = NULL;

  debug (bot, "grelinkc_output: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  grelinkc = (grelinkc_t *) gmod->data;
  if (!grelinkc)
    {
      puts ("GRELINKC_OUTPUT: grelinkc=NULL");
      return NULL;
    }

  debug (NULL, "grelinkc_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	 bot->txt_data_out);

  grelinkc_op_data_notify (grelinkc);

  gmodule_down (dlist_node, bot);


  return bot;
}


bot_t *
grelinkc_input (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dl_rlpkt = NULL, *dptr = NULL;
  relink_control_pkt_t *rlpkt = NULL;
  char *buf_ptr = NULL;
  bot_gmod_elm_t *gmod = NULL;
  grelinkc_t *grelinkc = NULL;
  int buf_len;

  debug (bot, "grelinkc_input: Entered\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  grelinkc = (grelinkc_t *) gmod->data;
  if (!grelinkc)
    {
      puts ("GRELINKC_INPUT: grelinkc=NULL");
      return NULL;
    }


  dl_rlpkt = relink_buf_to_dlist (bot->txt_data_in, bot->txt_data_in_sz);
  if (!dl_rlpkt)
    {
      return NULL;
    }

  dlist_fornext (dl_rlpkt, dptr)
  {
    buf_ptr = (char *) dlist_data (dptr);

    rlpkt = (relink_control_pkt_t *) buf_ptr;

    switch (rlpkt->op)
      {
      case RELINK_CONTROL_OP_NOP:
	{
	  buf_len = grelinkc_op_nop (grelinkc, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_RELINK:
	{
	  buf_len = grelinkc_op_relink (grelinkc, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_DESTROY:
	{
	  break;
	}
      case RELINK_CONTROL_OP_QUERY:
	{
	  break;
	}
      case RELINK_CONTROL_OP_HELLO:
	{
	  buf_len = grelinkc_op_hello (grelinkc, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_RESULT:
	{
	  buf_len = grelinkc_op_result (grelinkc, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_DATA:
	{
	  buf_len = grelinkc_op_data (grelinkc, rlpkt);
	  break;
	}
      default:
	buf_len += sizeof (relink_control_pkt_t);
	break;
      }

/* end while */
  }

  tokenize_destroy (bot, &dl_rlpkt);

  debug (NULL, "grelinkc_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	 bot->txt_data_out);

  return bot;
}







int
grelinkc_op_hello (grelinkc_t * grelinkc, relink_control_pkt_t * rlpkt)
{

  int n = sizeof (relink_control_pkt_t);

  debug (NULL, "grelinkc_op_hello: Entered\n");

  if (!grelinkc || !rlpkt)
    return -1;

  if (rlpkt->op != RELINK_CONTROL_OP_HELLO)
    return -1;

  n =
    relink_req_pack_buf_relink_new (grelinkc->bot->txt_data_out, grelinkc->id,
				    grelinkc->line);

  relink_packet_print ("relinkc:\n",
		       (relink_control_pkt_t *) grelinkc->bot->txt_data_out);
  relink_packet_hton ((relink_control_pkt_t *) grelinkc->bot->txt_data_out);
  grelinkc->bot->txt_data_out_sz = n;

  return n;
}






int
grelinkc_op_data_notify (grelinkc_t * grelinkc)
{
  int n;
  char buf[MAX_BUF_SZ + 1];
  int buf_len;

  debug (grelinkc->bot, "grelinkc_op_data_notify: Entered\n");

  if (!grelinkc)
    return -1;

  memset (buf, 0, sizeof (buf));
  buf_len = grelinkc->bot->txt_data_out_sz;
  memcpy (buf, grelinkc->bot->txt_data_out, buf_len);

  grelinkc->bot->txt_data_out_sz =
    relink_pack_buf_data (grelinkc->bot->txt_data_out, grelinkc->id, buf,
			  buf_len);
  relink_packet_hton ((relink_control_pkt_t *) grelinkc->bot->txt_data_out);

  n = grelinkc->bot->txt_data_out_sz;

  return n;
}





int
grelinkc_op_data (grelinkc_t * grelinkc, relink_control_pkt_t * rlpkt)
{
  int n = 0;
  char *data_ptr;
  int data_len;

  debug (NULL, "grelinkc_op_data: Entered\n");

  if (!grelinkc || !rlpkt)
    return -1;

  if (rlpkt->op != RELINK_CONTROL_OP_DATA)
    return -1;


  relink_find_buf_data ((char *) rlpkt,
			sizeof (grelinkc->bot->txt_data_in) - 1, &data_ptr,
			&data_len);

  if (data_len <= 0 || data_ptr == NULL)
    return -1;

  memcpy (grelinkc->bot->txt_data_in, data_ptr, data_len);
  grelinkc->bot->txt_data_in_sz = data_len;
  n = sizeof (relink_control_pkt_t) + data_len;

  if (data_len > 0)
    {
      gmodule_up (grelinkc->dptr_gmod, grelinkc->bot);

      if (grelinkc->bot->txt_data_out_sz)
	{
	  grelinkc->bot->txt_data_out_sz =
	    relink_pack_buf_data (grelinkc->bot->txt_data_in, grelinkc->id,
				  grelinkc->bot->txt_data_out,
				  grelinkc->bot->txt_data_out_sz);
	  memcpy (grelinkc->bot->txt_data_out, grelinkc->bot->txt_data_in,
		  grelinkc->bot->txt_data_out_sz);
	  gmodule_down (grelinkc->dptr_gmod, grelinkc->bot);
	}

    }


  return n;
}








int
grelinkc_op_result (grelinkc_t * grelinkc, relink_control_pkt_t * rlpkt)
{
  int n = 0;

  debug (NULL, "grelinkc_op_result: Entered\n");

  if (!grelinkc || !rlpkt)
    return -1;

  if (rlpkt->op != RELINK_CONTROL_OP_RESULT)
    return -1;

  puts ("RELINKC: RESULT RECV'D");
  if (rlpkt->type == RELINK_CONTROL_RESULT_CONNECTION_ERROR)
    {
/* unset hooks */
      grelinkc->initialized = 0;
      return -1;
    }

  gmodule_set_can_pass_up (grelinkc->gmod);

  if (rlpkt->type == RELINK_CONTROL_RESULT_CONNECTION_NEW)
    {
      puts ("NEW");
    }
  else if (rlpkt->type == RELINK_CONTROL_RESULT_CONNECTION_OLD)
    {
      puts ("OLD");
    }

  return n;
}





bot_t *
grelinkc_destroy_up_grelinkc (grelinkc_t * grelinkc)
{
  bot_t *ret = NULL;


  debug (NULL, "grelinkc_destroy_up_grelinkc: Entered\n");

  if (grelinkc)
    {
      ret = grelinkc_destroy_up (grelinkc->dptr_gmod, grelinkc->bot);
    }

  return ret;
}


bot_t *
grelinkc_destroy_down_grelinkc (grelinkc_t * grelinkc)
{
  bot_t *ret = NULL;

  debug (NULL, "grelinkc_destroy_down_grelinkc: Entered\n");

  if (grelinkc)
    {
      ret = grelinkc_destroy_down (grelinkc->dptr_gmod, grelinkc->bot);
    }

  return ret;
}




bot_t *
grelinkc_destroy_up (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  grelinkc_t *grelinkc = NULL;

  debug (NULL, "grelinkc_destroy_up: Entered\n");

  gmodule_destroy_up (dlist_node, bot);

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  grelinkc = gmod->data;
	  free (grelinkc);
	}
    }

  return bot;
}

bot_t *
grelinkc_destroy_down (dlist_t * dlist_node, bot_t * bot)
{

  bot_gmod_elm_t *gmod = NULL;
  grelinkc_t *grelinkc = NULL;

  debug (NULL, "grelinkc_destroy_up: Entered\n");

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  grelinkc = gmod->data;
	  free (grelinkc);
	}
    }

  gmodule_destroy_down (dlist_node, bot);

  return bot;
}






void
grelinkc_gmod_init (bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
  grelinkc_t *grelinkc = NULL;
  debug (NULL, "grelinkc_gmod_init: Entered\n");

  if (!gmod || !bot || !dptr_gmod)
    return;

  if (gmod->data)
    return;

  grelinkc = (grelinkc_t *) calloc (1, sizeof (grelinkc_t));

  if (!grelinkc)
    return;

  gmod->data = grelinkc;

  grelinkc->bot = bot;
  grelinkc->dptr_gmod = dptr_gmod;
  grelinkc->gmod = gmod;

  return;
}








bot_t *
grelinkc_control_up (dlist_t * dlist_node, bot_t * bot)
{
  grelinkc_t *grelinkc = NULL;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "grelinkc_control_up: Entered\n");

  if (!dlist_node || !bot)
    {
      return NULL;
    }


  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  grelinkc = (grelinkc_t *) gmod->data;

  gmodule_control_up (dlist_node, bot);

  return bot;
}




bot_t *
grelinkc_control_down (dlist_t * dlist_node, bot_t * bot)
{
  debug (NULL, "grelinkc_control_down: Entered\n");


  if (!dlist_node || !bot)
    return NULL;

  gmodule_control_down (dlist_node, bot);

  return bot;
}



void
grelinkc_free (void *arg)
{
  grelinkc_t *grelinkc = (grelinkc_t *) arg;

  if (!grelinkc)
    return;

/* ... */

  if (!grelinkc->gmod)
    return;

  grelinkc->gmod->data = NULL;
  free (grelinkc);

  return;
}
