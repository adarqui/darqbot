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
#include "bot.h"

int
bot_unix_fd_send (bot_t * bot, int fd, bot_gmod_elm_t * gmod)
{
  char tag[132];
  char cmsg_buf[sizeof (struct cmsghdr) + sizeof (long)];
  struct sockaddr_un un;
  struct cmsghdr *cmsg;
  struct msghdr msg;
  struct iovec iov[2];
  int sock;

  int op;

  if (!bot)
    return -1;


  debug (NULL, "bot_unix_fd_send: Entered: %s\n", gi->fd_unix_path);

  if (!sNULL(global_info.fd_unix_path) || fd < 0)
    return -1;

  bz2(un);
  strlcpy_buf (un.sun_path, global_info.fd_unix_path);

  un.sun_family = AF_UNIX;

  sock = socket (AF_UNIX, SOCK_STREAM, 0);
  if (sock < 0)
    {
      perror ("socket ");
      return -1;
    }


  if (connect (sock, (struct sockaddr *) &un, sizeof (un)) < 0)
    {
      perror ("connect ");
      goto cleanup;
    }


  op = BOT_UNIX_OP_FDPASS;
  iov[0].iov_len = sizeof (int);
  iov[0].iov_base = &op;

  bz(tag);
  snprintf_buf (tag, "%s,%i", bot->tag, bot->ID);

  if (gmod)
    {
      if (sNULL(gmod->trigger_ext))
	{
	  strlcatfmt_buf (tag, ",%s", gmod->trigger_ext);
	}
    }

  iov[1].iov_len = sizeof (tag);
  iov[1].iov_base = tag;

  msg.msg_name = 0;
  msg.msg_namelen = 0;
  msg.msg_control = cmsg_buf;
  msg.msg_controllen = sizeof cmsg_buf;
  msg.msg_iov = (struct iovec *) &iov;
  msg.msg_iovlen = 2;
  msg.msg_flags = 0;

  cmsg = (struct cmsghdr *) cmsg_buf;
  cmsg->cmsg_level = SOL_SOCKET;
  cmsg->cmsg_type = SCM_RIGHTS;
  msg.msg_controllen = cmsg->cmsg_len =
    sizeof (struct cmsghdr) + sizeof (long);
  *(int *) ((void *) cmsg + sizeof (struct cmsghdr)) = fd;

  sleep (1);

  if (sendmsg (sock, &msg, 0) < 0)
    {
      perror ("sendmsg ");
      goto cleanup;
    }

  safe_close (sock);
  return 0;

cleanup:
  if (sock)
    safe_close (sock);

  return -1;
}

int
bot_unix_init (void)
{
  struct sockaddr_un *un;
  int s;

  struct stat st;

  debug (NULL, "bot_unix_init: Entered\n");

  if (global_info.fd_unix > 0)
    return 0;

if(!sNULL(global_info.fd_unix_path))
    strlcat_buf(global_info.fd_unix_path, str_unite_static ("%s/%s", gi->confdir, BOT_UNIX_SOCKPATH));

  debug (NULL, "bot_unix_init: fd_unix_path=%s\n", gi->fd_unix_path);

  un = (struct sockaddr_un *) &global_info.fd_unix_un;

  if (stat (global_info.fd_unix_path, &st) >= 0)
    {
      unlink (global_info.fd_unix_path);
    }


  memset (un, 0, sizeof (struct sockaddr_un));
  un->sun_family = AF_UNIX;
  strlcpy_buf (un->sun_path, global_info.fd_unix_path);

  s = socket (AF_UNIX, SOCK_STREAM, 0);
  if (s < 0)
    {
      perror ("socket ");
      return -1;
    }

  if (bind (s, (struct sockaddr *) un, sizeof (struct sockaddr_un)) < 0)
    {
      perror ("bind ");
      goto cleanup;
    }
  if (listen (s, 10) < 0)
    {
      perror ("listen ");
      goto cleanup;
    }


  global_info.fd_unix = s;

  safe_event_set (&global_info.ev_unix, global_info.fd_unix, EV_READ | EV_PERSIST,
	     bot_evhook_unix_accept, NULL);
  safe_event_add (&global_info.ev_unix, NULL);

  return 0;

cleanup:
  if (s)
    safe_close (s);

  return -1;
}


int
bot_unix_fini (void)
{

    bz (global_info.fd_unix_path);

  safe_event_del (&global_info.ev_unix);

  if (global_info.fd_unix)
    safe_close (global_info.fd_unix);

  global_info.fd_unix = 0;

  bz2(global_info.ev_unix);

  return 0;
}



void
bot_evhook_unix_accept (int fd, short event, void *arg)
{
  dlist_t *dptr;
  bot_unix_node_t *bun;
  struct sockaddr_un *un;
  int s;
  socklen_t unlen;

  debug (NULL, "bot_evhook_unix_accept: Entered\n");

  unlen = sizeof (struct sockaddr_un);

  un = (struct sockaddr_un *) &global_info.fd_unix_un;

  s = accept (fd, (struct sockaddr *) un, &unlen);

  if (s < 0)
    return;

  bun = (bot_unix_node_t *) calloc (1, sizeof (bot_unix_node_t));
  bun->ev_fd = s;

  dptr = dlist_Dinsert_after (&dl_bot_unix, bun);

  if (dptr)
    {
      safe_event_set (&bun->ev, bun->ev_fd, EV_READ | EV_PERSIST,
		 bot_evhook_unix_read, dptr);
      safe_event_add (&bun->ev, NULL);
    }
  else
    {
      safe_close (s);
    }

  return;
}


void
bot_evhook_unix_read (int fd, short event, void *arg)
{
  dlist_t *dptr;
  bot_unix_node_t *bun;
  struct cmsghdr *hdr;
  struct msghdr msg;
  char cmsg_buf[sizeof (struct cmsghdr) + sizeof (long)];
  struct iovec iov[2];

  int passed_fd;

  int op;
  char tag[132];


  if (!arg)
    {
      safe_close (fd);
      return;
    }

  dptr = (dlist_t *) arg;
  if (!dptr->data)
    {
      safe_close (fd);
      return;
    }

  bun = (bot_unix_node_t *) dlist_data (dptr);

  debug (NULL, "bot_evhook_unix_read: Entered\n");

  bz2(msg);

  iov[0].iov_base = &op;
  iov[0].iov_len = sizeof (op);
  iov[1].iov_base = &tag;
  iov[1].iov_len = sizeof (tag);

  msg.msg_iov = (struct iovec *) &iov;
  msg.msg_iovlen = 2;
  msg.msg_control = cmsg_buf;
  msg.msg_controllen = sizeof (cmsg_buf);

  errno = 0;

  if (recvmsg (fd, &msg, 0) < 0)
    {
      perror ("recvmsg: Entered ");
      bot_unix_node_destroy (dptr);
      return;
    }

  bz2(iov);

  if (msg.msg_iovlen > 0)
    {
      if (op == BOT_UNIX_OP_FDPASS || BOT_UNIX_OP_TAG)
	{
	  strlcpy_buf (bun->tag, tag);
	  bun->op = op;
	}
      else if (op == BOT_UNIX_OP_CLOSE)
	{

	}

    }


  if (op == BOT_UNIX_OP_FDPASS)
    {

      hdr = CMSG_FIRSTHDR (&msg);
      if (!hdr)
	{
	  goto fdpass_cleanup;
	}
      if (hdr->cmsg_level != SOL_SOCKET || hdr->cmsg_type != SCM_RIGHTS)
	{
	  goto fdpass_cleanup;
	}
      passed_fd = *(int *) CMSG_DATA (hdr);
      bun->passed_fd = passed_fd;

      bot_find_and_set_fd (bun->tag, bun->passed_fd);

    fdpass_cleanup:
      bot_unix_node_destroy (dptr);
    }

  sleep (1);
  return;
}







void
bot_unix_node_destroy (void *arg)
{
  dlist_t *dptr;
  bot_unix_node_t *bun;
  if (!arg)
    return;

  dptr = (dlist_t *) arg;
  bun = (bot_unix_node_t *) dlist_data (dptr);;

  safe_event_del (&bun->ev);
  safe_close (bun->ev_fd);

  free (bun);

  dlist_remove (&dl_bot_unix, dptr);

  free (dptr);

  return;
}
