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
#include "mod_relinkd.h"

void
__relinkd_init__ (void)
{

  strlcpy_buf (mod_relinkd_info.name, "mod_relinkd");
  strlcpy_buf (mod_relinkd_info.trigger, "^relinkd");

  mod_relinkd_info.init = relinkd_init;
  mod_relinkd_info.fini = relinkd_fini;
  mod_relinkd_info.help = relinkd_help;
  mod_relinkd_info.run = relinkd_run;


  mod_relinkd_info.output = NULL;
  mod_relinkd_info.input = NULL;


  debug (NULL, "__relinkd_init__: Loaded mod_relinkd\n");

  return;
}



bot_t *
relinkd_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "relinkd_init: Entered\n");
  return NULL;
}

bot_t *
relinkd_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "relinkd_fini: Entered\n");
  return NULL;
}

bot_t *
relinkd_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "relinkd_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^relinkd";

  return NULL;
}

bot_t *
relinkd_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt, opt_val = 0;

  debug (bot, "relinkd_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "relinkd_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  opt = 0;

  MOD_OPTIONS_TOP_HALF;

  if (!strcasecmp (dl_options_ptr, "links"))
    {
      opt = RELINKD_OPT_QUERY_LINKS;
    }
  else if (!strcasecmp (dl_options_ptr, "subs"))
    {
      opt = RELINKD_OPT_QUERY_SUBS;
    }
  else if (!strcasecmp (dl_options_ptr, "reload"))
    {
      opt = RELINKD_OPT_QUERY_RELOAD;
    }
  else if (!strncasecmp_len (dl_options_ptr, "destroy_link"))
    {
      opt = RELINKD_OPT_DESTROY_LINK;
      opt_val = atoi (&dl_options_ptr[strlen ("destroy_link=")]);
    }
  else if (!strncasecmp_len (dl_options_ptr, "destroy_sub"))
    {
      opt = RELINKD_OPT_DESTROY_SUB;
      opt_val = atoi (&dl_options_ptr[strlen ("destroy_sub=")]);
    }
  else if (!strcasecmp (dl_options_ptr, "destroy_all"))
    {
      opt = RELINKD_OPT_DESTROY_ALL;
    }

  MOD_OPTIONS_BOTTOM_HALF;


  if (!bot_fork_clean (bot))
    {
      MOD_PARSE_TOP_HALF;
      signal (SIGPIPE, SIG_IGN);
      signal (SIGALRM, bot_sigalrm_handler);
      l_new_str = relinkd_change_string (bot, l_str_ptr, opt, opt_val);
      MOD_PARSE_BOTTOM_HALF;
    }

  return bot;
}



char *
relinkd_change_string (bot_t * bot, char *string, int opt, int opt_val)
{
  char *str = NULL;
  char *sep_ptr;

  int fd;

  char *host = NULL, *port = NULL;

  if (!string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;


  host = strtok (string, ":");
  if (!host)
    return NULL;
  port = strtok (NULL, "");
  if (!port)
    return NULL;

  fd = bot_network_raw_connect (host, atoi (port), SOCK_STREAM, 10);
  if (fd < 0)
    {
      return NULL;
    }

  switch (opt)
    {
    case RELINKD_OPT_QUERY_LINKS:
      {
	str = relinkd_op_query_links (bot, fd);
	break;
      }
    case RELINKD_OPT_QUERY_SUBS:
      {
	str = relinkd_op_query_subs (bot, fd);
	break;
      }
    case RELINKD_OPT_QUERY_RELOAD:
      {
	str = relinkd_op_reload (bot, fd);
	break;
      }
    case RELINKD_OPT_DESTROY_LINK:
      {
	str = relinkd_op_destroy_link (bot, fd, opt_val);
	break;
      }
    case RELINKD_OPT_DESTROY_SUB:
      {
	str = relinkd_op_destroy_sub (bot, fd, opt_val);
	break;
      }
    case RELINKD_OPT_DESTROY_ALL:
      {
	str = relinkd_op_destroy_all (bot, fd);
	break;
      }
    default:
      {
	break;
      }
    }

  shutdown (fd, SHUT_RDWR);
  safe_close (fd);
  printf ("CLOSED %i\n", fd);

  return str;
}





char *
relinkd_op_query_links (bot_t * bot, int fd)
{
  dlist_t *dl = NULL, *dl_rlpkt, *dptr;
  relink_control_pkt_t *rlpkt = NULL;
  char buf[MAX_BUF_SZ], *buf_ptr;
  char *str = NULL;
  int n;

  char *data_str = NULL;
  int data_len = 0;

  debug (NULL, "relinkd_op_query_links: Entered\n");

  if (!bot || fd <= 0)
    return NULL;


  memset (buf, 0, sizeof (buf));

  rlpkt = (relink_control_pkt_t *) buf;
  n = relink_req_pack_buf_query_links (buf, 0);
  relink_packet_hton ((relink_control_pkt_t *) rlpkt);

  printf ("sending query_link: n=%i\n", n);
  n = send (fd, rlpkt, n, 0);
  if (n <= 0)
    return NULL;
  puts ("sent");

  sleep (1);

  alarm (2);
  while (1)
    {
      memset (buf, 0, sizeof (buf));
      n = recv (fd, buf, sizeof (buf) - 1, 0);
      if (n <= 0)
	break;

      printf ("QUERY_LINK, n=%i\n", n);

      dl_rlpkt = relink_buf_to_dlist (buf, n);

      if (!dl_rlpkt)
	{
	  puts ("dl_rlpkt = NULL");
	}
      else
	{
	  printf ("dl_rlpkt = %i\n", dlist_size (dl_rlpkt));

	  dlist_fornext (dl_rlpkt, dptr)
	  {
	    buf_ptr = (char *) dlist_data (dptr);
	    rlpkt = (relink_control_pkt_t *) buf_ptr;

	    n = relink_packet_len (buf_ptr);

	    relink_find_buf_result_string (buf_ptr, n, &data_str, &data_len);
	    if (data_len <= 0 || !data_str)
	      continue;

	    printf ("data_str=%s\n", data_str);
	    dlist_Dinsert_after (&dl, strdup (data_str));
	  }

	}

    }

  alarm (0);

  if (dl)
    {
      puts ("SETTING STRING");
      str = dlist_to_str (dl);
      dlist_fini (&dl, free);
    }

  return str;
}







char *
relinkd_op_query_subs (bot_t * bot, int fd)
{
  dlist_t *dl = NULL, *dl_rlpkt, *dptr;
  relink_control_pkt_t *rlpkt = NULL;
  char buf[MAX_BUF_SZ], *buf_ptr;
  char *str = NULL;
  int n;

  char *data_str = NULL;
  int data_len = 0;

  debug (NULL, "relinkd_op_query_subs: Entered\n");

  if (!bot || fd <= 0)
    return NULL;


  memset (buf, 0, sizeof (buf));

  rlpkt = (relink_control_pkt_t *) buf;
  n = relink_req_pack_buf_query_subs (buf, 0);
  relink_packet_hton ((relink_control_pkt_t *) rlpkt);

  printf ("sending query_link: n=%i\n", n);
  n = send (fd, rlpkt, n, 0);
  if (n <= 0)
    return NULL;
  puts ("sent");

  sleep (1);

  alarm (10);
  while (1)
    {
      memset (buf, 0, sizeof (buf));
      n = recv (fd, buf, sizeof (buf) - 1, 0);
      if (n <= 0)
	break;

      printf ("QUERY_LINK, n=%i\n", n);

      dl_rlpkt = relink_buf_to_dlist (buf, n);

      if (!dl_rlpkt)
	{
	  puts ("dl_rlpkt = NULL");
	}
      else
	{
	  printf ("dl_rlpkt = %i\n", dlist_size (dl_rlpkt));

	  dlist_fornext (dl_rlpkt, dptr)
	  {
	    buf_ptr = (char *) dlist_data (dptr);
	    rlpkt = (relink_control_pkt_t *) buf_ptr;

	    n = relink_packet_len (buf_ptr);

	    relink_find_buf_result_string (buf_ptr, n, &data_str, &data_len);
	    puts ("WTF0c");
	    if (data_len <= 0 || !data_str)
	      continue;

	    printf ("data_str=%s\n", data_str);
	    dlist_Dinsert_after (&dl, strdup (data_str));
	  }

	}

    }

  alarm (0);

  if (dl)
    {
      puts ("SETTING STRING");
      str = dlist_to_str (dl);
      dlist_fini (&dl, free);
    }

  if (dl_rlpkt)
    {
      puts ("DESTROYING DL_RLPKT");
      dlist_fini (&dl, free);
    }

  return str;
}











char *
relinkd_op_destroy_link (bot_t * bot, int fd, int link_id)
{
  relink_control_pkt_t *rlpkt = NULL;
  char buf[MAX_BUF_SZ];
  char *str = NULL;
  int n;

  debug (NULL, "relinkd_op_destroy_link: Entered\n");

  if (!bot || fd <= 0)
    return NULL;


  memset (buf, 0, sizeof (buf));

  rlpkt = (relink_control_pkt_t *) buf;
  n = relink_req_pack_buf_destroy_link (buf, 0, link_id);
  relink_packet_hton ((relink_control_pkt_t *) rlpkt);

  printf ("sending query_link: n=%i\n", n);
  n = send (fd, rlpkt, n, 0);
  if (n <= 0)
    return NULL;
  puts ("sent");

  return str;
}







char *
relinkd_op_destroy_sub (bot_t * bot, int fd, int sub_id)
{
  relink_control_pkt_t *rlpkt = NULL;
  char buf[MAX_BUF_SZ];
  char *str = NULL;
  int n;

  debug (NULL, "relinkd_op_destroy_sub: Entered\n");

  if (!bot || fd <= 0)
    return NULL;


  memset (buf, 0, sizeof (buf));

  rlpkt = (relink_control_pkt_t *) buf;
  n = relink_req_pack_buf_destroy_sub (buf, 0, sub_id);
  relink_packet_hton ((relink_control_pkt_t *) rlpkt);

  printf ("sending query_link: n=%i\n", n);
  n = send (fd, rlpkt, n, 0);
  if (n <= 0)
    return NULL;
  puts ("sent");

  return str;
}





char *
relinkd_op_destroy_all (bot_t * bot, int fd)
{
  relink_control_pkt_t *rlpkt = NULL;
  char buf[MAX_BUF_SZ];
  char *str = NULL;
  int n;

  debug (NULL, "relinkd_op_destroy_all: Entered\n");

  if (!bot || fd <= 0)
    return NULL;


  memset (buf, 0, sizeof (buf));

  rlpkt = (relink_control_pkt_t *) buf;
  n = relink_req_pack_buf_destroy_all (buf, 0);
  relink_packet_hton ((relink_control_pkt_t *) rlpkt);

  printf ("sending query_link: n=%i\n", n);
  n = send (fd, rlpkt, n, 0);
  if (n <= 0)
    return NULL;
  puts ("sent");

  return str;
}








char *
relinkd_op_reload (bot_t * bot, int fd)
{
  relink_control_pkt_t *rlpkt = NULL;
  char buf[MAX_BUF_SZ];
  char *str = NULL;
  int n;

  debug (NULL, "relinkd_op_reload: Entered\n");

  if (!bot || fd <= 0)
    return NULL;


  memset (buf, 0, sizeof (buf));

  rlpkt = (relink_control_pkt_t *) buf;
  n = relink_req_pack_buf_reload (buf);
  relink_packet_hton ((relink_control_pkt_t *) rlpkt);

  printf ("sending reload: n=%i\n", n);
  n = send (fd, rlpkt, n, 0);
  if (n <= 0)
    return NULL;
  puts ("sent");

  return str;
}
