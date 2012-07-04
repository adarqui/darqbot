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
#include "gmod_graw.h"


void __graw_init__ (void) __attribute__ ((constructor));

void
__graw_init__ (void)
{

  strlcpy_buf (gmod_graw_info.name, "gmod_graw");
  strlcpy_buf (gmod_graw_info.trigger, "^graw");
  gmod_graw_info.init = graw_init;
  gmod_graw_info.fini = graw_fini;
  gmod_graw_info.help = graw_help;
  gmod_graw_info.run = graw_run;
  gmod_graw_info.control = graw_control;

  gmod_graw_info.off = graw_off;
  gmod_graw_info.destroy_up = graw_destroy_up;
  gmod_graw_info.destroy_down = graw_destroy_down;

  gmod_graw_info.control_up = graw_control_up;
  gmod_graw_info.control_down = graw_control_down;

  gmod_graw_info.output = graw_output;
  gmod_graw_info.input = graw_input;

  gmod_graw_info.self = &gmod_graw_info;

  debug (NULL, "__graw_init__: Loaded gmod_graw\n");

  return;
}



bot_t *
graw_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "graw_init: Entered\n");
  return NULL;
}

bot_t *
graw_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "graw_fini: Entered\n");
  return NULL;
}


bot_t *
graw_off (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  graw_t *graw = NULL;

  debug (bot, "graw_off: Entered\n");


  if (!dlist_node || !bot)
    {
      return NULL;
    }

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    {
      return NULL;
    }


  graw = (graw_t *) gmod->data;
  if (!graw)
    {
      return NULL;
    }

  graw_unset_evhooks (graw);

  return NULL;
}





bot_t *
graw_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "graw_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^graw";

  return NULL;
}

bot_t *
graw_control (dlist_t * dlist_node, bot_t * bot, int what, va_list ap)
{
  graw_t *graw = NULL;
  bot_gmod_elm_t *gmod = NULL;
  int fd;

  debug (NULL, "graw_control: Entered: %p %p\n", dlist_node, bot);

  if (!dlist_node || !bot)
    return NULL;

  if (what == XMODULE_CONTROL_FDPASS)
    {

      fd = va_arg (ap, int);

      debug (NULL, "graw_control: XMODULE_CONTROL_FDPASS=%i\n", fd);

      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      graw = (graw_t *) gmod->data;

      graw->link.fd = fd;

      graw_set_evhooks_client (graw);

    }

  return NULL;
}

bot_t *
graw_run (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;
  graw_t *graw = NULL;

  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "graw_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc (bot, bot->trig_called);

  debug (bot,
	 "graw_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  debug (bot, "dl_module_arg=%s\n", bot->dl_module_arg);


//gmod = (bot_gmod_elm_t *) gmodule_find_gmod(bot, gmod_graw_info.trigger);
  dptr_gmod =
    (dlist_t *) gmodule_find_gmod_dptr (bot, gmod_graw_info.trigger,
					gmod_graw_info.trigger_ext);

  debug (bot, "dptr_gmod=%p\n", dptr_gmod);

  if (!dptr_gmod)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    {
      return NULL;
    }

  if (!gmod->data)
    {
      graw_gmod_init (bot, gmod, dptr_gmod);
      graw = gmod->data;
    }

  MOD_OPTIONS_TOP_HALF;

  graw_process_options (graw, dl_options_ptr);

  MOD_OPTIONS_BOTTOM_HALF;

  graw_input (dptr_gmod, bot);
  gmodule_set_has_run (gmod);

  return bot;
}



char *
graw_process_options (graw_t * graw, char *string)
{

  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  dlist_t *dl = NULL, *dptr;

  debug (NULL, "graw_process_options: Entered\n");


  if (!graw || !sNULL (string))
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
    graw_process_options_parse (graw, (char *) dlist_data (dptr));
  }


  tokenize_destroy (NULL, &dl);

  return str;
}






void
graw_process_options_parse (graw_t * graw, char *string)
{

  if (!graw || !sNULL (string))
    return;

  debug (NULL, "graw_process_options_parse: Entered\n");

  if (!strcasecmp (string, "client"))
    {
      graw->link_type = BOT_TYPE_CLIENT;
    }
  else if (!strcasecmp (string, "server"))
    {
      graw->link_type = BOT_TYPE_SERVER;
    }
  else if (!strncasecmp_len (string, "proto="))
    {
      graw_process_options_parse_proto (graw, &string[strlen ("proto=")]);
    }
  else if (!strncasecmp_len (string, "host="))
    {
      graw_process_options_parse_host (graw, &string[strlen ("host=")]);
    }
  else if (!strncasecmp_len (string, "path="))
    {
      graw_process_options_parse_path (graw, &string[strlen ("path=")]);
    }
  else if (!strncasecmp_len (string, "noqueue"))
    {
      graw->noqueue = 1;
    }
  else if (!strncasecmp_len (string, "destroy_down"))
    {
      graw->destroy_down = 1;
      graw->destroy_up = 0;
    }
  else if (!strncasecmp_len (string, "destroy_up"))
    {
/* default */
      graw->destroy_up = 1;
      graw->destroy_down = 0;
    }

  gmodule_parse_common_options (graw->bot, graw->gmod, string);

  return;
}



void
graw_process_options_parse_proto (graw_t * graw, char *string)
{

  if (!graw || !sNULL (string))
    return;

/*
XXX
  if (!strcasecmp (string, "tcp"))
    {
      bot->proto = BOT_PROTO_TCP;
    }
  else if (!strcasecmp (string, "udp"))
    {
      bot->proto = BOT_PROTO_UDP;
    }
  else if (!strcasecmp (string, "raw"))
    {
      bot->proto = BOT_PROTO_RAW;
    }
*/

  return;
}


void
graw_process_options_parse_host (graw_t * graw, char *string)
{
  server_t *server;
  char *tok_1, *tok_2;

  if (!graw || !sNULL (string))
    return;


  tok_1 = strtok (string, ":");
  if (!sNULL (tok_1))
    return;

  tok_2 = strtok (NULL, "");
  if (!sNULL (tok_2))
    return;

  server = (server_t *) calloc (1, sizeof (server_t));
  server->type = AF_INET;
  server->host = strdup (tok_1);
  server->port = atoi (tok_2);

  dlist_Dinsert_after (&graw->dl_server, server);

  return;
}



void
graw_process_options_parse_path (graw_t * graw, char *string)
{
  server_t *server;

  if (!graw || !sNULL (string))
    return;

  server = (server_t *) calloc (1, sizeof (server_t));
  server->type = AF_UNIX;
  strlcpy_buf (server->sun.sun_path, string);

  dlist_Dinsert_after (&graw->dl_server, server);

  return;
}



char *
graw_change_string (bot_t * bot, char *string, int opt)
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


int
graw_connect (graw_t * graw)
{
  dlist_t *dptr;
  fd_link_t *bfl = NULL;
  server_t *server;
  char *str_tmp;
  int link_fd;

  if (!graw)
    return -1;


  debug (NULL, "graw_connect: Entered\n");

  if (graw->dl_fd_link == NULL)
    {
/* setup the fd_link list */
      dlist_fornext (graw->dl_server, dptr)
      {
	server = (server_t *) dlist_data (dptr);
	if (!server)
	  continue;

	if (server->type == AF_INET)
	  {

	    str_tmp = n_gethostbyname (server->host);

	    if (str_tmp)
	      {
		server->sin.sin_addr.s_addr = inet_addr (str_tmp);
		server->sin.sin_port = htons (server->port);
		server->sin.sin_family = AF_INET;
		free (str_tmp);
	      }

	    bfl =
	      fd_link_add (&graw->dl_fd_link, 0, graw->link.ev, &server->sin,
			   NULL);
	  }
	else if (server->type == AF_UNIX)
	  {
	    server->sun.sun_family = AF_UNIX;
	    bfl =
	      fd_link_add (&graw->dl_fd_link, 0, graw->link.ev, NULL,
			   &server->sun);
	  }
	else
	  {
	    continue;
	  }

      }
    }

  while (1)
    {


      dlist_fornext (graw->dl_fd_link, dptr)
      {

	bfl = (fd_link_t *) dlist_data (dptr);

	if (bfl->type == AF_INET)
	  {
	    debug (NULL, "graw_connect: af_inet: %s, %i\n",
		   inet_ntoa (bfl->sin.sin_addr), ntohs (bfl->sin.sin_port));
	    link_fd =
	      graw_network_raw_connect (inet_ntoa (bfl->sin.sin_addr),
					ntohs (bfl->sin.sin_port));
	  }
	else if (bfl->type == AF_UNIX)
	  {
	    debug (NULL, "graw_connect: af_unix: %s\n", bfl->sun.sun_path);
	    link_fd = graw_network_unix_connect (bfl->sun.sun_path);
	  }

	if (link_fd < 0)
	  {
	    sleep (10);
	    continue;
	  }


	memcpy (&graw->link, bfl, sizeof (fd_link_t));

	graw->link.fd = link_fd;
	graw->dl_server_cur = dptr;

	debug (NULL,
	       "graw_connect: connected to %s on bot->fd_link=%i\n",
	       server->host, graw->link.fd);

	if (bot_unix_fd_send (graw->bot, graw->link.fd, graw->gmod) >= 0)
	  return 0;

	safe_close (graw->link.fd);
      }

      sleep (10);
    }

  return 0;
}






int
graw_send_link (graw_t * graw, char *buf, int len)
{
  int n;

  debug (NULL, "graw_send_link: Entered: %p %s %i\n", graw, buf, len);

  if (!graw || !sNULL (buf) || len <= 0)
    return -1;

/* need to clean separators */

  debug (NULL, "graw_send_link: fd=%i, len=%i, buf=[%s]\n", graw->link.fd,
	 len, buf);

  n = send (graw->link.fd, buf, len, 0);
  if (n <= 0)
    {
      perror ("graw_send_link: WTF ");
    }

  return n;
}






int
graw_recv_link (graw_t * graw, char *buf, int len)
{
  int n;


  debug (NULL, "graw_recv_link: Entered\n");

  if (!graw || !buf || len <= 0)
    return -1;

  debug (NULL, "graw_recv_link: fd=%i\n", graw->link.fd);


  n = recv (graw->link.fd, buf, len, 0);
  if (n <= 0)
    {
      debug (NULL,
	     "graw_recv_link: recv<=0, unsetting hooks, bot=%p, fd=%i, err=%s\n",
	     graw, graw->link.fd, strerror (errno));
    }

  debug (NULL, "graw_recv_link: n=%i, buf=[%s]\n", n, buf);
  return n;
}




int
graw_listen (graw_t * graw)
{
  server_t *server;
  int fd = 0;

  debug (NULL, "graw_listen: Entered\n");

  if (!graw)
    return -1;

  if (!graw->dl_server_cur)
    {
      return -1;
    }

  server = (server_t *) dlist_data (graw->dl_server_cur);
  if (!server)
    {
      return -1;
    }

  if (server->type == AF_INET)
    {
      graw->link.type = AF_INET;
      fd = graw_network_raw_listen (server->host, server->port);
    }
  else
    {
      graw->link.type = AF_UNIX;
      memcpy (&graw->link.sun, &server->sun, sizeof (server->sun));
      fd = graw_network_unix_listen (server->sun.sun_path);
    }

  debug (NULL,
	 "graw_listen: fd=%i, returned from graw_network_graw_listen\n", fd);

  if (fd < 0)
    return -1;

  return fd;
}







int
graw_network_raw_disconnect (int fd)
{
  safe_close (fd);
  return 0;
}



void
graw_disconnect (bot_t * bot)
{
  if (!bot)
    return;

/*
XXX
  graw_network_raw_disconnect (bot->fd_link);
*/
  return;
}




int
graw_accept (graw_t * graw)
{

  int nfd;
  struct sockaddr_in sin;
  struct sockaddr_un sun;
  struct sockaddr *s_ptr;
  socklen_t len;

  debug (NULL, "graw_accept: Entered\n");

  if (!graw)
    return -1;

  s_ptr = NULL;

  if (graw->link.type == AF_INET)
    {
      len = sizeof (sin);
      s_ptr = (struct sockaddr *) &sin;
    }
  else if (graw->link.type == AF_UNIX)
    {
      len = sizeof (sun);
      s_ptr = (struct sockaddr *) &sun;
    }

  nfd = accept (graw->link.fd, (struct sockaddr *) s_ptr, &len);

  debug (NULL, "graw_accept: nfd=%i\n", nfd);

  if (nfd > 0)
    {
      graw->link_connected = 1;
      graw->link_connecting = 0;
    }

/*
graw->link.type = AF_INET;
*/
  if (graw->link.type == AF_INET)
    {
      memcpy (&graw->link.sin, &sin, sizeof (sin));
    }
  else if (graw->link.type == AF_UNIX)
    {
    }

  return nfd;
}






pid_t
graw_spawner (graw_t * graw)
{

  debug (NULL, "graw_spawner: Entered\n");

  if (!graw)
    return -1;

  if (graw->link_type == BOT_TYPE_CLIENT)
    {
      return graw_connect_spawner (graw);
    }
  else if (graw->link_type == BOT_TYPE_SERVER)
    {
      return graw_accept_spawner (graw);
    }
  else
    {
      debug (NULL, "graw_spawner: WTF\n");
    }

  return 0;
}





pid_t
graw_accept_spawner (graw_t * graw)
{
  fd_link_t *bfl;
  dlist_t *dptr_server = NULL;
  int fd;

  int at_least_one = 0;


  debug (NULL, "graw_accept_spawner: Entered\n");

  if (!graw)
    return -1;

  if (graw->link_connecting)
    {
      debug (NULL, "graw_accept_spawner: already connecting, returning\n");
      return -1;
    }

  graw->link_connected = 0;
  graw->link_connecting = 1;

  graw->fd_link_flags = FD_LINK_LIST;

  fd_link_destroy (&graw->dl_fd_link, fd_link_cleanup);

  dlist_fornext (graw->dl_server, dptr_server)
  {
    memset (&graw->link, 0, sizeof (graw->link));

    graw->dl_server_cur = dptr_server;

    fd = graw_listen (graw);

    if (fd < 0)
      {
	gmodule_destroy_down (graw->dptr_gmod, graw->bot);
	return -1;
      }

    at_least_one = 1;

    graw->link.fd = fd;

    if (graw->link.type == AF_INET)
      {
	bfl =
	  fd_link_add (&graw->dl_fd_link, graw->link.fd, graw->link.ev,
		       &graw->link.sin, NULL);
      }
    else if (graw->link.type == AF_UNIX)
      {
	bfl =
	  fd_link_add (&graw->dl_fd_link, graw->link.fd, graw->link.ev, NULL,
		       &graw->link.sun);
      }
    else
      {
	continue;
      }

    debug
      (NULL,
       "graw_accept_spawner: graw->link.fd=%i, fd=%i, bfl->fd=%i, bfl->link.sin.sin_port=%i GOGO\n",
       graw->link.fd, fd, bfl->fd, bfl->sin.sin_port);

    graw_set_evhooks_accept (graw, bfl);
  }

  if (at_least_one == 0)
    {
/* destroy */
      gmodule_destroy_down (graw->dptr_gmod, graw->bot);
      return -1;
    }

/*
XXX
gmodule_set_can_pass_up(graw->gmod);
*/


  return 0;
}





pid_t
graw_connect_spawner (graw_t * graw)
{
/* initiates a child which tries to connect to a server, non stop */
  pid_t pid;

  if (!graw)
    return -1;

  debug (NULL, "graw_connect_spawner: Entered\n");

  if (graw->link_connecting)
    {
      debug (NULL, "graw_connect_spawner: already connecting, returning\n");
      return -1;
    }

  if (graw->bot->on == 0)
    {
      debug (NULL,
	     "graw_connect_spawner: bot is currently turned off, returning\n");
      return -1;
    }

  graw->link_connected = 0;
  graw->link_connecting = 1;

  pid = bot_fork_clean (graw->bot);
  if (!pid)
    {
      if (graw_connect (graw) < 0)
	{
	  debug (NULL, "main: graw_connect: Failed\n");
	  sleep (1);
	}

      exit (0);
    }
  else
    {
/* add this child to a reaper */
    }

  return pid;
}



int
graw_network_raw_connect (char *host, int port)
{
  struct sockaddr_in sin;
  char *str_tmp;
  int fd, n;
  if (!sNULL (host) || port <= 0)
    return -1;

  memset (&sin, 0, sizeof (sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons (port);

  str_tmp = n_gethostbyname (host);
  if (!sNULL (str_tmp))
    return -1;

  sin.sin_addr.s_addr = inet_addr (str_tmp);
  free (str_tmp);

  fd = socket (AF_INET, SOCK_STREAM, 0);
  if (fd < 0)
    return -1;

  n = connect (fd, (struct sockaddr *) &sin, sizeof (sin));
  if (n < 0)
    {
      safe_close (fd);
      return -1;
    }

  return fd;
}



int
graw_network_unix_connect (char *path)
{
  struct sockaddr_un sun;
  int fd, n;

  debug (NULL, "graw_network_unix_connect: Entered: %s\n", path);

  if (!sNULL (path))
    return -1;

  memset (&sun, 0, sizeof (sun));

  sun.sun_family = AF_UNIX;

  strlcpy_buf (sun.sun_path, path);

  fd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (fd < 0)
    {
      perror ("graw_network_unix_connect() ");
      return -1;
    }

  n = connect (fd, (struct sockaddr *) &sun, sizeof (sun));
  if (n < 0)
    {
      perror ("graw_network_unix_connect(connect) ");
      safe_close (fd);
      return -1;
    }

  return fd;
}





int
graw_network_raw_listen (char *host, int port)
{
  struct sockaddr_in sin;
  int fd;
  char *str_tmp = NULL;
  int on = 1;

  debug (NULL, "graw_network_raw_listen: Entered\n");

  if (!sNULL (host) || port <= 0)
    {
      return -1;
    }

  memset (&sin, 0, sizeof (sin));

  str_tmp = n_gethostbyname (host);
  if (!sNULL (str_tmp))
    {
      return -1;
    }

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = inet_addr (str_tmp);
  sin.sin_port = htons (port);

  free (str_tmp);

  fd = socket (AF_INET, SOCK_STREAM, 0);
  if (fd <= 0)
    {
      goto cleanup;
    }


  if (bind (fd, (struct sockaddr *) &sin, sizeof (sin)) < 0)
    {
      goto cleanup;
    }

  if (setsockopt (fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on)) < 0)
    {
      goto cleanup;
    }

  if (listen (fd, 10) < 0)
    {
      goto cleanup;
    }

  debug (NULL, "graw_network_raw_listen: SUCCESS %i\n", fd);

  return fd;


cleanup:

  if (fd > 0)
    safe_close (fd);

  return -1;
}







int
graw_network_unix_listen (char *path)
{
  struct sockaddr_un sun;
  int fd;

  debug (NULL, "graw_network_unix_listen: Entered\n");

  if (!sNULL (path))
    return -1;

  unlink (path);

  memset (&sun, 0, sizeof (sun));

  sun.sun_family = AF_UNIX;
  strlcpy_buf (sun.sun_path, path);

  fd = socket (AF_UNIX, SOCK_STREAM, 0);
  if (fd <= 0)
    {
      goto cleanup;
    }


  if (bind (fd, (struct sockaddr *) &sun, sizeof (sun)) < 0)
    {
      goto cleanup;
    }


  if (listen (fd, 10) < 0)
    {
      goto cleanup;
    }

  debug (NULL, "graw_network_unix_listen: SUCCESS %i\n", fd);

  return fd;


cleanup:
  if (fd > 0)
    safe_close (fd);

  return -1;
}









int
graw_set_evhooks_client (graw_t * graw)
{
  return graw_set_evhooks (graw);
}


int
graw_set_evhooks (graw_t * graw)
{

  if (!graw)
    return -1;


  safe_event_set (&graw->link.ev, graw->link.fd, EV_READ | EV_PERSIST,
		  graw_evhook_link, graw);
  safe_event_add (&graw->link.ev, NULL);


  return 0;
}





int
graw_set_evhooks_accept (graw_t * graw, fd_link_t * bfl)
{

  debug (NULL, "graw_set_evhooks_accept: Entered\n");

  if (!graw)
    return -1;


  safe_event_set (&bfl->ev, bfl->fd, EV_READ | EV_PERSIST,
		  graw_evhook_accept, graw);

  safe_event_del (&bfl->ev);
  safe_event_add (&bfl->ev, NULL);


  return 0;
}





int
graw_unset_evhooks (graw_t * graw)
{
  bot_gmod_elm_t *gmod;
  dlist_t *dptr_gmod;

  debug (NULL, "graw_unset_evhooks: Entered\n");

  if (!graw)
    return -1;

  gmod = graw->gmod;
  dptr_gmod = graw->dptr_gmod;

  if (graw->link.fd > 0)
    {
      debug (NULL, "graw_unset_evhooks: closing: %i\n", graw->link.fd);
      safe_event_del (&graw->link.ev);
      safe_close (graw->link.fd);
    }

  graw->link_connected = 0;
  graw->link_connecting = 0;

  fd_link_destroy (&graw->dl_fd_link, fd_link_cleanup);

  dlist_fini (&graw->dl_server, server_free);

  memset (graw, 0, sizeof (graw_t));

  free (graw);

  gmod->data = NULL;


  return 0;
}















void
graw_evhook_link (int fd, short event, void *arg)
{
  graw_t *graw = NULL;
  int n;
  char buf[MAX_BUF_SZ + 1];


  debug (NULL, "graw_evhook_link: Entered\n");

  graw = (graw_t *) arg;

  if (!graw)
    return;

  debug_set_cur (graw->bot, graw_evhook_link);

  waitpid (-1, &n, WNOHANG);

  bot_line_clear (graw->bot);

  n = graw_recv (graw);
  if (n <= 0)
    {
      debug (NULL, "graw_evhook_link: recv: %s\n", strerror (errno));

      if (graw->destroy_down)
	graw_destroy_down_graw (graw);
      else if (graw->destroy_up)
	graw_destroy_up_graw (graw);

      return;
    }



  memset (buf, 0, sizeof (buf));
  memcpy (buf, graw->bot->txt_data_in, n);

  debug (NULL, "=> graw_evhook_link: fd=%i, n=%i, buf=[%s]\n", fd, n, buf);

  gmodule_up (graw->dptr_gmod, graw->bot);
  if (graw->bot->txt_data_out_sz)
    graw_send (graw);

  if (!dlist_next (graw->dptr_gmod))
    {
/* no end node, yet we received data.. */
      gmodule_fix_data_in_to_out (graw->bot);
      gmodule_down (graw->dptr_gmod, graw->bot);
    }

  return;
}





int
graw_recv (graw_t * graw)
{
  int n;

  if (!graw)
    return -1;

  n =
    graw_recv_link (graw, graw->bot->txt_data_in,
		    sizeof (graw->bot->txt_data_in) - 1);
  graw->bot->txt_data_in_sz = n;

  return n;
}




int
graw_send (graw_t * graw)
{
  int n;

  if (!graw)
    return -1;

  n =
    graw_send_link (graw, graw->bot->txt_data_out,
		    graw->bot->txt_data_out_sz);
  return n;
}





void
graw_evhook_accept (int fd, short event, void *arg)
{
  bot_gmod_elm_t *gmod_sub = NULL;
  dlist_t *dptr_gmod_sub = NULL;
  graw_t *graw = NULL, *graw_sub = NULL;
  bot_t *bot_sub;
  int nfd = 0;

  debug (NULL, "graw_evhook_accept: Entered\n");

  graw = (graw_t *) arg;
  if (!graw)
    return;

  debug_set_cur (graw->bot, graw_evhook_accept);

  fd_link_set_cur (&graw->dl_fd_link, &graw->link, fd);

  nfd = graw_accept (graw);

  if (nfd <= 0)
    {
/* unset hooks etc */
      return;
    }


  bot_sub = bot_dup_sub (graw->bot);

  dptr_gmod_sub =
    (dlist_t *) gmodule_find_gmod_dptr (bot_sub, gmod_graw_info.trigger,
					gmod_graw_info.trigger_ext);
  if (!dptr_gmod_sub)
    return;

  gmod_sub = (bot_gmod_elm_t *) dlist_data (dptr_gmod_sub);
  if (!gmod_sub->data)
    {
      debug (NULL,
	     "graw_evhook_accept: GIRCD ACCEPT GMOD->DATA = NULL, ALLOCATING");
      graw_gmod_init (bot_sub, gmod_sub, dptr_gmod_sub);
      graw_sub = gmod_sub->data;
    }

  if (!graw_sub)
    return;

  graw_sub->initialized = 1;
  graw_sub->link_type = graw->link_type;
  graw_sub->link.fd = nfd;
  memcpy (&gmod_sub->link, &graw->link, sizeof (fd_link_t));

  bot_sub_add (graw->bot, bot_sub);

  graw_set_evhooks (graw_sub);

  debug (NULL, "graw_evhook_accept: ADDED SUB NFD: %i\n", graw_sub->link.fd);

  gmodule_set_can_pass_up (graw_sub->gmod);
  gmodule_set_can_pass_down (graw_sub->gmod);

  gmodule_up (dptr_gmod_sub, bot_sub);
  if (bot_sub->txt_data_out_sz)
    graw_send (graw_sub);

  return;
}












bot_t *
graw_output (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  graw_t *graw = NULL;
  int n;

  debug (bot, "graw_output: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  graw = (graw_t *) gmod->data;
  if (!graw)
    {
      debug (NULL, "graw_output: graw=NULL");
      return NULL;
    }

  graw_update_graw (graw, bot);

  n = graw_send (graw);
  if (n <= 0)
    {
    }

  debug (NULL, "(graw) FROM_OUTPUT\n");

  gmodule_down (dlist_node, bot);

  return bot;
}


bot_t *
graw_input (dlist_t * dlist_node, bot_t * bot)
{
  pid_t pid;
  bot_gmod_elm_t *gmod = NULL;
  graw_t *graw = NULL;

  debug (bot, "graw_input: Entered\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  graw = (graw_t *) gmod->data;
  if (!graw)
    {
      debug (NULL, "graw_input: graw=NULL\n");
      return NULL;
    }

  graw_update_graw (graw, bot);

  if (graw->link_connected == 0 && graw->link_connecting != 1)
    {
      debug (NULL, "graw_input: SPAWNING graw_spawner\n");

      pid = graw_spawner (graw);
    }

  if (graw->link_type == BOT_TYPE_CLIENT)
    {
      graw_input_client (graw);
    }

  gmodule_up (dlist_node, bot);

  return bot;
}



int
graw_input_client (graw_t * graw)
{
  bot_gmod_cache_elm_t *gmod_c;
  int n;

  debug (NULL, "graw_input_client: Entered\n");

  if (!graw)
    return -1;

  if (!graw->initialized && !graw->noqueue)
    {
      debug (NULL, "graw_input_client: ENQUEUEING..............\n");
      gmod_cache_enqueue (&graw->gmod->dl_cache, 0, graw->bot->txt_data_in,
			  graw->bot->txt_data_in_sz);
    }
  else
    {

      if (graw->gmod->dl_cache != NULL && !graw->noqueue)
	{
/* send cache'd data */

	  while (1)
	    {
	      gmod_c = gmod_cache_dequeue (&graw->gmod->dl_cache);
	      if (!gmod_c)
		break;
	      n = graw_send_link (graw, gmod_c->data, gmod_c->data_len);
	      gmod_cache_free (gmod_c);
	    }

	}


      if (graw->bot->txt_data_in_sz != 0)
	debug (NULL, "(graw) SENDING...............\n");
      n =
	graw_send_link (graw, graw->bot->txt_data_in,
			graw->bot->txt_data_in_sz);
/* ...
bot_line_clear(graw->bot);
*/
    }

  return 0;
}





bot_t *
graw_destroy_up_graw (graw_t * graw)
{
  bot_t *ret = NULL;

  debug (NULL, "graw_destroy_up_graw: Entered\n");

  if (graw)
    {
      ret = graw_destroy_up (graw->dptr_gmod, graw->bot);
    }

  return ret;
}


bot_t *
graw_destroy_down_graw (graw_t * graw)
{
  bot_t *ret = NULL;

  debug (NULL, "graw_destroy_down_graw: Entered\n");

  if (graw)
    {
      ret = graw_destroy_down (graw->dptr_gmod, graw->bot);
    }

  return ret;
}




bot_t *
graw_destroy_up (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  graw_t *graw = NULL;

  debug (NULL, "graw_destroy_up: Entered\n");

  gmodule_destroy_up (dlist_node, bot);

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  graw = gmod->data;
	  graw_unset_evhooks (graw);
	}
    }

  return bot;
}

bot_t *
graw_destroy_down (dlist_t * dlist_node, bot_t * bot)
{

  bot_gmod_elm_t *gmod = NULL;
  graw_t *graw = NULL;

  debug (NULL, "graw_destroy_up: Entered\n");

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  graw = gmod->data;
	  graw_unset_evhooks (graw);
	}
    }

  gmodule_destroy_down (dlist_node, bot);

  return bot;
}






void
graw_gmod_init (bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
  graw_t *graw = NULL;
  debug (NULL, "graw_gmod_init: Entered\n");

  if (!gmod || !bot || !dptr_gmod)
    return;

  if (gmod->data)
    return;

  graw = (graw_t *) calloc (1, sizeof (graw_t));

  if (!graw)
    return;

  gmod->data = graw;

  graw->bot = bot;
  graw->dptr_gmod = dptr_gmod;
  graw->gmod = gmod;

/* defaults */
  graw->destroy_down = 0;
  graw->destroy_up = 1;

  return;
}








bot_t *
graw_control_up (dlist_t * dlist_node, bot_t * bot)
{
  graw_t *graw = NULL;
  bot_gmod_elm_t *gmod = NULL;
  int fd;

  debug (NULL, "graw_control_up: Entered\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  graw = (graw_t *) gmod->data;
  if (graw)
    {

      fd = control_get_fdpass (&bot->dl_control);
      if (fd >= 0)
	{

	  graw->initialized = 1;
	  graw->link.fd = fd;

	  graw_set_evhooks_client (graw);

	  gmodule_set_can_pass_up (gmod);
	  gmodule_set_can_pass_down (gmod);

/* need this to flush the gmod->dl_cache */
	  graw_input_client (graw);

	  debug (NULL, "graw_control_up: GRAW_CONTROL_UP!!! fd found=%i\n",
		 fd);
	}
    }
  else
    {
      debug (NULL, "graw_control_up: GRAW = NULL\n");
    }

  gmodule_control_up (dlist_node, bot);

  return bot;
}




bot_t *
graw_control_down (dlist_t * dlist_node, bot_t * bot)
{
  debug (NULL, "graw_control_down: Entered\n");

  gmodule_control_down (dlist_node, bot);

  return bot;
}






void
graw_update_graw (graw_t * graw, bot_t * bot)
{

  debug (NULL, "graw_update_graw: Entered\n");

  if (!graw)
    return;

  if (bot)
    {
      graw->bot = bot;
    }

  return;
}
