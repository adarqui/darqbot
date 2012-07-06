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
#include "gmod_grelinkd.h"


void __grelinkd_init__ (void) __attribute__ ((constructor));

void
__grelinkd_init__ (void)
{

  strlcpy_buf (gmod_grelinkd_info.name, "gmod_grelinkd");
  strlcpy_buf (gmod_grelinkd_info.trigger, "^grelinkd");

  gmod_grelinkd_info.init = grelinkd_init;
  gmod_grelinkd_info.fini = grelinkd_fini;
  gmod_grelinkd_info.help = grelinkd_help;
  gmod_grelinkd_info.run = grelinkd_run;

  gmod_grelinkd_info.off = grelinkd_off;
  gmod_grelinkd_info.destroy_up = grelinkd_destroy_up;
  gmod_grelinkd_info.destroy_down = grelinkd_destroy_down;

  gmod_grelinkd_info.control_up = grelinkd_control_up;
  gmod_grelinkd_info.control_down = grelinkd_control_down;

  gmod_grelinkd_info.output = grelinkd_output;
  gmod_grelinkd_info.input = grelinkd_input;

  gmod_grelinkd_info.self = &gmod_grelinkd_info;

  debug (NULL, "__grelinkd_init__: Loaded gmod_grelinkd\n");

  return;
}



bot_t *
grelinkd_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "grelinkd_init: Entered\n");
  return NULL;
}

bot_t *
grelinkd_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "grelinkd_fini: Entered\n");
  return NULL;
}


bot_t *
grelinkd_off (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  grelinkd_t *grelinkd = NULL;

  debug (bot, "grelinkd_off: Entered\n");


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


  grelinkd = (grelinkd_t *) gmod->data;
  if (!grelinkd)
    {
      return NULL;
    }

  grelinkd_free (grelinkd);

  return NULL;
}





bot_t *
grelinkd_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "grelinkd_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^grelinkd";

  return NULL;
}


bot_t *
grelinkd_run (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;
  grelinkd_t *grelinkd = NULL;

  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "grelinkd_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "grelinkd_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  printf ("dl_module_arg=%s\n", bot->dl_module_arg);


  dptr_gmod =
    (dlist_t *) gmodule_find_gmod_dptr (bot, gmod_grelinkd_info.trigger,
					gmod_grelinkd_info.trigger_ext);
  if (!dptr_gmod)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    {
      puts ("GRELINKD GMOD=NULL");
      return NULL;
    }

  if (!gmod->data)
    {
      puts ("GIRCD GMOD->DATA = NULL, ALLOCATING");
      grelinkd_gmod_init (bot, gmod, dptr_gmod);
      grelinkd = gmod->data;
    }

  MOD_OPTIONS_TOP_HALF;

  grelinkd_process_options (grelinkd, dl_options_ptr);

  MOD_OPTIONS_BOTTOM_HALF;

  gmodule_set_can_pass_down (gmod);

  grelinkd_input (dptr_gmod, bot);

  gmodule_set_has_run (gmod);

  return bot;
}



char *
grelinkd_process_options (grelinkd_t * grelinkd, char *string)
{

  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  dlist_t *dl = NULL, *dptr;

  debug (NULL, "grelinkd_process_options: Entered\n");


  if (!grelinkd || !sNULL (string))
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));


  dl = tokenize (NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES, "...");
  if (!dl)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    grelinkd_process_options_parse (grelinkd, (char *) dlist_data (dptr));
  }


  tokenize_destroy (NULL, &dl);

  return str;
}






void
grelinkd_process_options_parse (grelinkd_t * grelinkd, char *string)
{

  if (!grelinkd || !sNULL (string))
    return;

  debug (NULL, "grelinkd_process_options_parse: Entered\n");

  if (!strcasecmp (string, "null"))
    {
      grelinkd_process_options_parse_null (grelinkd,
					   &string[strlen ("null=")]);
    }

  gmodule_parse_common_options (grelinkd->bot, grelinkd->gmod, string);

  return;
}




void
grelinkd_process_options_parse_null (grelinkd_t * grelinkd, char *string)
{

  if (!grelinkd || !sNULL (string))
    return;

  return;
}



char *
grelinkd_change_string (bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char *sep_ptr;

  if (!bot || !string)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  return str;
}






bot_t *
grelinkd_output (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  grelinkd_t *grelinkd = NULL;

  debug (bot, "grelinkd_output: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  grelinkd = (grelinkd_t *) gmod->data;
  if (!grelinkd)
    {
      puts ("GRELINKD_OUTPUT: grelinkd=NULL");
      return NULL;
    }

  debug (NULL, "grelinkd_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	 bot->txt_data_out);


  grelinkd_op_data_notify (grelinkd);

  return bot;
}


bot_t *
grelinkd_input (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dl_rlpkt = NULL, *dptr;
  relink_control_pkt_t *rlpkt;

  char *buf_ptr = NULL;
  bot_gmod_elm_t *gmod = NULL;
  grelinkd_t *grelinkd = NULL;
  int buf_len;

  debug (bot, "grelinkd_input: Entered\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  grelinkd = (grelinkd_t *) gmod->data;
  if (!grelinkd)
    {
      puts ("GRELINKD_INPUT: grelinkd=NULL");
      return NULL;
    }

  switch (grelinkd->initialized)
    {
    case 0:
      {
	grelinkd_input_uninitialized (grelinkd);
	return bot;
	break;
      }
    default:
      break;
    }



  dl_rlpkt =
    relink_buf_to_dlist (grelinkd->bot->txt_data_in,
			 grelinkd->bot->txt_data_in_sz);
  if (!dl_rlpkt)
    {
      return NULL;
    }

  debug (NULL, "grelinkd dl_rlpkt sz = %i\n", dlist_size (dl_rlpkt));

  dlist_fornext (dl_rlpkt, dptr)
  {
    buf_ptr = (char *) dlist_data (dptr);

    rlpkt = (relink_control_pkt_t *) buf_ptr;

    switch (rlpkt->op)
      {
      case RELINK_CONTROL_OP_NOP:
	{
	  buf_len = grelinkd_op_nop (grelinkd, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_RELINK:
	{
	  buf_len = grelinkd_op_relink (grelinkd, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_DESTROY:
	{
	  buf_len = grelinkd_op_destroy (grelinkd, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_QUERY:
	{
	  buf_len = grelinkd_op_query (grelinkd, rlpkt);
	  break;
	}

      case RELINK_CONTROL_OP_RESULT:
	{
	  buf_len = grelinkd_op_result (grelinkd, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_DATA:
	{
	  buf_len = grelinkd_op_data (grelinkd, rlpkt);
	  break;
	}
      case RELINK_CONTROL_OP_RELOAD:
	{
/* XXX
buf_len = grelinkd_op_reload(bot, grdm);
*/
	  break;
	}
      default:
	{
	  buf_len = sizeof (relink_control_pkt_t);
	  break;
	}
      }

  }


  tokenize_destroy (bot, &dl_rlpkt);


  gmodule_up (dlist_node, bot);

  debug (NULL, "grelinkd_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	 bot->txt_data_out);

  return bot;
}



bot_t *
grelinkd_input_uninitialized (grelinkd_t * grelinkd)
{
  int n;

  debug (NULL, "grelinkd_input_uninitialized: Entered\n");

  if (!grelinkd)
    return NULL;

  n = relink_req_pack_buf_hello ((char *) &grelinkd->bot->txt_data_out, 0);
  relink_packet_hton ((relink_control_pkt_t *) & grelinkd->bot->txt_data_out);
  grelinkd->bot->txt_data_out_sz = n;

  gmodule_fix_data_down (grelinkd->bot);

  gmodule_down (grelinkd->dptr_gmod, grelinkd->bot);

  debug (NULL, "grelinkd_input: in=[%s], out=[%s]\n",
	 grelinkd->bot->txt_data_in, grelinkd->bot->txt_data_out);

/* after hello packet, waiting for relinkc reply */
  grelinkd->initialized = 1;

  return grelinkd->bot;
}




int
grelinkd_op_data_notify (grelinkd_t * grelinkd)
{
  int n;
  char buf[MAX_BUF_SZ + 1];
  int buf_len;

  debug (grelinkd->bot, "grelinkd_op_data_notify: Entered\n");

  if (!grelinkd)
    return -1;

  memset (buf, 0, sizeof (buf));
  buf_len = grelinkd->bot->txt_data_out_sz;
  memcpy (buf, grelinkd->bot->txt_data_out, buf_len);

  grelinkd->bot->txt_data_out_sz =
    relink_pack_buf_data (grelinkd->bot->txt_data_out, grelinkd->id, buf,
			  buf_len);
  relink_packet_hton ((relink_control_pkt_t *) grelinkd->bot->txt_data_out);

  n = grelinkd->bot->txt_data_out_sz;

  gmodule_down (grelinkd->dptr_gmod, grelinkd->bot);

  return n;
}


int
grelinkd_op_data (grelinkd_t * grelinkd, relink_control_pkt_t * rlpkt)
{
  int n = sizeof (relink_control_pkt_t);
  char *data_ptr;
  int data_len;

  debug (grelinkd->bot, "grelinkd_op_data: Entered\n");

  if (!grelinkd || !rlpkt)
    return -1;

  if (rlpkt->op != RELINK_CONTROL_OP_DATA)
    return -1;

  if (grelinkd->initialized < 3)
    {
      return -1;
    }

/* data transfer is ok..*/
  relink_find_buf_data ((char *) rlpkt,
			sizeof (grelinkd->bot->txt_data_in) - 1, &data_ptr,
			&data_len);

  if (data_len)
    {

      if (data_len > sizeof (grelinkd->bot->txt_data_in))
	data_len = sizeof (grelinkd->bot->txt_data_in);

      memcpy (grelinkd->bot->txt_data_in, data_ptr, data_len);
      grelinkd->bot->txt_data_in_sz = data_len;
      gmodule_up (grelinkd->dptr_gmod, grelinkd->bot);
      gmodule_down (grelinkd->dptr_gmod, grelinkd->bot);

    }

  return n;
}





int
grelinkd_op_relink_notify (grelinkd_t * grelinkd)
{


  int n = sizeof (relink_control_pkt_t);

  debug (NULL, "grelinkd_op_relink_notify: Entered\n");

  if (!grelinkd)
    return -1;

  if (grelinkd->initialized != 2)
    return -1;

  switch (grelinkd->isrelinked)
    {
    case 0:
      {
/* new link, new connection */
	puts ("GRELINKD RESULT NEW");
	grelinkd->bot->txt_data_out_sz =
	  relink_res_pack_buf_new (grelinkd->bot->txt_data_out, grelinkd->id);
	relink_packet_hton ((relink_control_pkt_t *) grelinkd->
			    bot->txt_data_out);
	break;
      }
    case 1:
      {
/* old link, reconnect it */
	puts ("GRELINKD RESULT OLD");
	grelinkd->bot->txt_data_out_sz =
	  relink_res_pack_buf_old (grelinkd->bot->txt_data_out, grelinkd->id);
	relink_packet_hton ((relink_control_pkt_t *) grelinkd->
			    bot->txt_data_out);
	break;
      }
    case -1:
      {
/* FAILURE, notify relinkc */
	puts ("GRELINKD_LINK_INIT: RESULT: FAILED");
	grelinkd->bot->txt_data_out_sz =
	  relink_res_pack_buf_error (grelinkd->bot->txt_data_out,
				     grelinkd->id);
	relink_packet_hton ((relink_control_pkt_t *) grelinkd->
			    bot->txt_data_out);
	break;
      }
    default:
      return -1;
    }

  gmodule_down (grelinkd->dptr_gmod, grelinkd->bot);
  n = grelinkd->bot->txt_data_out_sz;

  return n;
}


int
grelinkd_op_relink (grelinkd_t * grelinkd, relink_control_pkt_t * rlpkt)
{
  bot_t *bot_saved = NULL;
  dlist_t *dptr_gmod_saved = NULL;
  relink_control_pkt_relink_t *rlpkt_relink = NULL;
  grelinkd_t *grelinkd_saved = NULL;
  char *ptr = NULL;
  int n = sizeof (relink_control_pkt_t);

  debug (NULL, "grelinkd_op_relink: Entered\n");

  if (!grelinkd || !rlpkt)
    return -1;

  if (rlpkt->op != RELINK_CONTROL_OP_RELINK)
    return -1;

  if (grelinkd->initialized != 1 && grelinkd->isrelinked != 0)
    return -1;

  ptr = (char *) rlpkt;
  rlpkt_relink = (char *) ptr + sizeof (relink_control_pkt_t);


  gmodule_set_can_pass_up (grelinkd->gmod);

  grelinkd->id = rlpkt->id;
  grelinkd->initialized = 2;
  grelinkd->bot_id = grelinkd->bot->ID;

  dptr_gmod_saved = grelinkd_find_id (grelinkd, &bot_saved);
  if (bot_saved && dptr_gmod_saved)
    {
      grelinkd_saved = (grelinkd_t *) dlist_data (dptr_gmod_saved);
      if (!grelinkd_saved)
	return -1;

      grelinkd_saved->bot = bot_saved;
      grelinkd_combine_dlist (grelinkd, grelinkd_saved);

      bot_fini (bot_saved);

      grelinkd->isrelinked = 1;
    }
  else
    {
      bot_parse_gmodules (grelinkd->bot, rlpkt_relink->host, 0, "+++");
    }

  gmodule_up (grelinkd->dptr_gmod, grelinkd->bot);

  grelinkd_op_relink_notify (grelinkd);

  grelinkd->initialized = 3;

  return n;
}



bot_t *
grelinkd_destroy_up_grelinkd (grelinkd_t * grelinkd)
{
  bot_t *ret = NULL;

  debug (NULL, "grelinkd_destroy_up_grelinkd: Entered\n");

/* RESPECT KEEPALIVE, ONLY DESTROY THE GMODS LEADING UP TO RELINKD, DO NOT FREE THE BOT */

  if (grelinkd)
    {
      ret = grelinkd_destroy_up (grelinkd->dptr_gmod, grelinkd->bot);
    }

  return ret;
}


bot_t *
grelinkd_destroy_down_grelinkd (grelinkd_t * grelinkd)
{
  bot_t *ret = NULL;

  debug (NULL, "grelinkd_destroy_down_grelinkd: Entered\n");

/* TAKE KEEPALIVE OFF, DESTROY EVERYTHING */

  if (grelinkd)
    {
      ret = grelinkd_destroy_down (grelinkd->dptr_gmod, grelinkd->bot);
    }

  return ret;
}




bot_t *
grelinkd_destroy_up (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  grelinkd_t *grelinkd = NULL;

  debug (NULL, "grelinkd_destroy_up: Entered\n");

  gmodule_destroy_up (dlist_node, bot);

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  grelinkd = gmod->data;
	  if (grelinkd)
	    {
	      grelinkd_free (grelinkd);
	    }
	}
    }

  return bot;
}

bot_t *
grelinkd_destroy_down (dlist_t * dlist_node, bot_t * bot)
{

  bot_gmod_elm_t *gmod = NULL;
  grelinkd_t *grelinkd = NULL;

  debug (NULL, "grelinkd_destroy_down: Entered\n");

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  grelinkd = (grelinkd_t *) gmod->data;
	  if (grelinkd)
	    {
	      grelinkd_free (grelinkd);
	    }
	}
    }

  gmodule_destroy_down (dlist_node, bot);

  return bot;
}






void
grelinkd_gmod_init (bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
  grelinkd_t *grelinkd = NULL;
  debug (NULL, "grelinkd_gmod_init: Entered\n");

  if (!gmod || !bot || !dptr_gmod)
    return;

  if (gmod->data)
    return;

  grelinkd = (grelinkd_t *) calloc (1, sizeof (grelinkd_t));

  if (!grelinkd)
    return;

  gmod->data = grelinkd;

  grelinkd->bot = bot;
  grelinkd->dptr_gmod = dptr_gmod;
  grelinkd->gmod = gmod;

  return;
}








bot_t *
grelinkd_control_up (dlist_t * dlist_node, bot_t * bot)
{
/* THIS WONT BE CALLED WHEN CONNECT FINISHES AND PASSES AN FD */
  grelinkd_t *grelinkd = NULL;
  bot_gmod_elm_t *gmod = NULL;
  int fd;

  debug (NULL, "grelinkd_control_up: Entered\n");

  if (!dlist_node || !bot)
    {
      return NULL;
    }


  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    goto cleanup;

  grelinkd = (grelinkd_t *) gmod->data;
  if (!grelinkd)
    goto cleanup;

  fd = control_get_fdpass_find (&bot->dl_control);
  if (fd >= 0)
    {

      grelinkd->initialized = 3;

      printf ("GRELINKD_CONTROL_UP!!! fd found=%i\n", fd);
    }
  else
    {
      puts ("GRELINKC = NULL WTF");
    }

  grelinkd_op_relink_notify (grelinkd);

cleanup:
  gmodule_control_up (dlist_node, bot);

  return bot;
}




bot_t *
grelinkd_control_down (dlist_t * dlist_node, bot_t * bot)
{
  debug (NULL, "grelinkd_control_down: Entered\n");


  if (!dlist_node || !bot)
    return NULL;

  gmodule_control_down (dlist_node, bot);

  return bot;
}



void
grelinkd_free (void *arg)
{
  grelinkd_t *grelinkd = (grelinkd_t *) arg;

  debug (NULL, "grelinkd_free: Entered\n");

  if (!grelinkd)
    return;

/* ... */

  debug (NULL, "grelinkd_free: grelinkd->id=%i, grelinkd->bot_id=%i\n",
	 grelinkd->id, grelinkd->bot_id);
  if (grelinkd->gmod)
    grelinkd->gmod->data = NULL;

  memset (grelinkd, 0, sizeof (grelinkd_t));
  free (grelinkd);

  return;
}







dlist_t *
grelinkd_find_id (grelinkd_t * grelinkd, bot_t ** bot)
{
/* go through all bots of 'tag', looking for relinkd id */
  grelinkd_t *grelinkd_t1;
  bot_t *bot_t1 = NULL;
  dlist_t *dptr_gmod = NULL, *dptr_bot;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "grelinkd_find_id: Entered\n");

  if (!grelinkd || !bot)
    {
      return NULL;
    }

  if (!(*bot))
    {
      bot_t1 = bot_find_tag (grelinkd->bot->tag);
      if (!bot_t1)
	return NULL;
    }
  else
    {
      bot_t1 = *bot;
    }

  dptr_gmod =
    gmodule_find_gmod_dptr (bot_t1, grelinkd->gmod->trigger,
			    grelinkd->gmod->trigger_ext);
  if (dptr_gmod)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
      if (gmod)
	{
	  grelinkd_t1 = (grelinkd_t *) gmod->data;
	  if (grelinkd_t1)
	    {

	      if (bot_t1->ID != grelinkd->bot_id
		  && grelinkd_t1->id == grelinkd->id)
		{
/* FOUND! */
		  return dptr_gmod;
		}
	    }
	}
    }

  dptr_gmod = NULL;

  dlist_fornext (bot_t1->dl_subs, dptr_bot)
  {
    bot_t1 = (bot_t *) dlist_data (dptr_bot);
    if (!bot_t1)
      continue;

    *bot = bot_t1;
    dptr_gmod = grelinkd_find_id (grelinkd, bot);
    if (dptr_gmod)
      return dptr_gmod;
  }

  return dptr_gmod;
}









void
grelinkd_combine_dlist (grelinkd_t * grelinkd, grelinkd_t * grelinkd_saved)
{
  dlist_t *dptr_gmod, *dptr_tmp, *dptr_gmod_removed;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "grelinkd_combine_dlist: Entered\n");

  if (!grelinkd || !grelinkd_saved)
    return;

  dlist_forprev_retarded (dlist_tail (grelinkd_saved->bot->dl_gmodules),
			  dptr_gmod, dptr_tmp)
  {
    if (!dptr_gmod)
      break;

    gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
    if (gmod)
      {
	debug (NULL, "combine: gmod->trigger=%s\n", gmod->trigger);
      }

    if (!strcasecmp (grelinkd->gmod->trigger, gmod->trigger))
      break;

    dptr_gmod_removed =
      dlist_remove (&grelinkd_saved->bot->dl_gmodules, dptr_gmod);

    dlist_insert_after (&grelinkd->bot->dl_gmodules, grelinkd->dptr_gmod,
			dptr_gmod_removed);
  }

  return;
}
