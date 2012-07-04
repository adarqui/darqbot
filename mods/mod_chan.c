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
#include "mod_chan.h"

void
__chan_init__ (void)
{

  strlcpy_buf (mod_chan_info.name, "mod_chan");
  strlcpy_buf (mod_chan_info.trigger, "^chan");

  module_add_subtrigs (&mod_chan_info, "^list");
  module_add_subtrigs (&mod_chan_info, "^users");
  module_add_subtrigs (&mod_chan_info, "^mode");
  module_add_subtrigs (&mod_chan_info, "^topic");
  module_add_subtrigs (&mod_chan_info, "^join");
  module_add_subtrigs (&mod_chan_info, "^part");
  module_add_subtrigs (&mod_chan_info, "^topic");
  module_add_subtrigs (&mod_chan_info, "^quit");
  module_add_subtrigs (&mod_chan_info, "^mode");
  module_add_subtrigs (&mod_chan_info, "^nick");

  mod_chan_info.init = mod_chan_init;
  mod_chan_info.fini = mod_chan_fini;
  mod_chan_info.help = mod_chan_help;
  mod_chan_info.run = mod_chan_run;
  mod_chan_info.output = mod_chan_output;
  mod_chan_info.input = mod_chan_input;



  debug (NULL, "__chan_init__: Loaded mod_chan\n");


  return;
}



bot_t *
mod_chan_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "mod_chan_init: Entered\n");
  return NULL;
}

bot_t *
mod_chan_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "mod_chan_fini: Entered\n");

  return NULL;
}

bot_t *
mod_chan_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "mod_chan_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^chan || ^chan({list,users,topic},#channel)";

  return NULL;
}

bot_t *
mod_chan_run (dlist_t * dlist_node, bot_t * bot)
{

  char *dl_module_arg_after_options, *dl_options_ptr;
  int opt;
  char *opt_str;

  if (!dlist_node || !bot)
    return NULL;

  debug (bot, "mod_chan_run: Entered\n");

  stat_inc (bot, bot->trig_called);


  debug (bot,
	 "mod_chan_run: Entered: initial hook buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	 bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);



  if (bot_shouldreturn (bot))
    return NULL;


  opt = 0;


  if (!strcasecmp (bot->trig_called, "^list"))
    {
      opt = MOD_CHAN_LIST;
    }
  else if (!strcasecmp (bot->trig_called, "^users"))
    {
      opt = MOD_CHAN_USERS;
    }
  else if (!strcasecmp (bot->trig_called, "^mode"))
    {
      opt = MOD_CHAN_MODE;
    }
  else if (!strcasecmp (bot->trig_called, "^join"))
    {
      opt = MOD_CHAN_JOIN;
    }
  else if (!strcasecmp (bot->trig_called, "^part"))
    {
      opt = MOD_CHAN_PART;
    }
  else if (!strcasecmp (bot->trig_called, "^topic"))
    {
      opt = MOD_CHAN_TOPIC;
    }
  else if (!strcasecmp (bot->trig_called, "^quit"))
    {
      opt = MOD_CHAN_QUIT;
    }
  else if (!strcasecmp (bot->trig_called, "^nick"))
    {
      opt = MOD_CHAN_NICK;
    }



  MOD_OPTIONS_TOP_HALF;

  if (opt != 0)
    {
      opt_str = dl_options_ptr;
    }
  else
    {

      opt_str = strtok (dl_options_ptr, ",");
      if (opt_str)
	{
	  opt_str = strtok (NULL, ")");
	}

      if (xstrcasestr_len (dl_options_ptr, "list", len_options_area))
	{
	  opt = MOD_CHAN_LIST;
	}
      else if (xstrcasestr_len (dl_options_ptr, "users", len_options_area))
	{
	  opt = MOD_CHAN_USERS;
	}
      else if (xstrcasestr_len (dl_options_ptr, "topic", len_options_area))
	{
	  opt = MOD_CHAN_TOPIC;
	}
      else if (xstrcasestr_len (dl_options_ptr, "mode", len_options_area))
	{
	  opt = MOD_CHAN_MODE;
	}
      else if (xstrcasestr_len (dl_options_ptr, "join", len_options_area))
	{
	  opt = MOD_CHAN_JOIN;
	}
      else if (xstrcasestr_len (dl_options_ptr, "part", len_options_area))
	{
	  opt = MOD_CHAN_PART;
	}
      else if (xstrcasestr_len (dl_options_ptr, "quit", len_options_area))
	{
	  opt = MOD_CHAN_QUIT;
	}
      else if (xstrcasestr_len (dl_options_ptr, "nick", len_options_area))
	{
	  opt = MOD_CHAN_NICK;
	}

    }
  MOD_OPTIONS_BOTTOM_HALF;

  printf ("opt=%i, opt_str=%s\n", opt, opt_str);

  if (!opt)
    return NULL;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = chan_change_string (bot, l_str_ptr, opt, opt_str);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}





char *
chan_change_string (bot_t * bot, char *string, int opt, char *opt_str)
{

  char **str_p_array;
  char *sep_ptr;
  char *str = NULL;
  char *tag = NULL;
  char *channel = NULL;

  bot_t *new_bot = NULL;
  str_p_array = NULL;

  debug (bot, "chan_change_string: Entered\n");

  if (!string)
    return 0;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  if (opt_str)
    {
      if (strchr (opt_str, ':'))
	{
	  tag = strtok (opt_str, ":");
	  if (tag)
	    {
	      channel = strtok (NULL, "");
	    }
	}
      else
	{
	  channel = opt_str;
	}
    }

  if (tag == NULL && channel)
    {
/* this is a tag */
      if (channel[0] != '#')
	{
	  tag = channel;
	  channel = NULL;
	}
    }

  printf ("chan_change_string: tag=%s, channel=%s\n", tag, channel);

  new_bot = chan_find_tag (tag);
  if (new_bot)
    {
      bot = new_bot;
    }

  if (opt == MOD_CHAN_LIST)
    {
/* return channel listing */
      str = chan_list (bot, tag);

    }
  else if (opt == MOD_CHAN_USERS)
    {
      str = chan_users (bot, channel);
    }
  else if (opt == MOD_CHAN_TOPIC)
    {
      str = chan_topic (bot, channel, string);
    }
  else if (opt == MOD_CHAN_JOIN)
    {
      str = chan_join (bot, string);
    }
  else if (opt == MOD_CHAN_PART)
    {
      str = chan_part (bot, string);
    }
  else if (opt == MOD_CHAN_QUIT)
    {
      str = chan_quit (bot, string);
    }
  else if (opt == MOD_CHAN_NICK)
    {
      str = chan_nick (bot, string);
    }
  else if (opt == MOD_CHAN_MODE)
    {
      str = chan_mode (bot, channel, string);
    }


  return str;
}



bot_t *
mod_chan_output (dlist_t * dlist_node, bot_t * bot)
{


  if (!dlist_node || !bot)
    return NULL;

  debug (bot, "mod_chan_output: Entered\n");

  return NULL;
}





bot_t *
mod_chan_input (dlist_t * dlist_node, bot_t * bot)
{
  channel_t *channel;
  nick_t *nick;
  dlist_t *dptr;
  char *tok_1;
  char *data_dup;
  char *nick_ptr;

  if (!dlist_node || !bot)
    return NULL;


/* XXX */
  return NULL;

  debug (bot, "mod_chan_input: Entered : %i, %s\n", bot->isline,
	 bot->irc_command);


  tok_1 = data_dup = NULL;

  if (!bot->isline && !bot->isprivmsg)
    {
/* parsed line */
      printf ("...mod_chan_input: PARSED LINE: txt_server_msg=%i\n",
	      bot->txt_server_msg);

      if (bot->txt_server_msg == 353)
	{

	  data_dup = strdup (bot->txt_data_in);

	  dptr = chan_find (bot, data_dup);
	  if (!dptr)
	    {
/* add the channel to the list */
//          goto cleanup;
	      channel_t *channel_new = NULL;
	      char *channel_new_sep, *channel_new_sep_2;

	      channel_new_sep = strchr (data_dup, '#');
	      if (!channel_new_sep)
		goto cleanup;

	      channel_new_sep_2 = strchr (channel_new_sep, ' ');
	      if (!channel_new_sep_2)
		goto cleanup;

	      channel_new = (channel_t *) calloc (1, sizeof (channel_t));
	      if (!channel_new)
		goto cleanup;

	      channel_new->name =
		strdup_len (channel_new_sep,
			    (channel_new_sep_2 - channel_new_sep));

	      printf ("... ADDING [%s]\n", channel_new->name);
/* XXX 
	      dptr = dlist_Dinsert_after (&bot->dl_channel, channel_new);
	      if (!dptr)
		{
		  free (channel_new->name);
		  free (channel_new);
		  goto cleanup;
		}
*/

	    }

	  channel = dlist_data (dptr);
/* channel users listing */
	  tok_1 = strtok (data_dup, ":");
	  dlist_fini (&channel->dl_users, free);

	  channel->present = 1;

	  if (tok_1)
	    {
	      while (1)
		{
		  tok_1 = strtok (NULL, " ");
		  if (!tok_1)
		    break;

		  while (*tok_1)
		    {
		      if (*tok_1 == '@' || *tok_1 == '+')
			tok_1++;
		      else
			break;
		    }

		  dlist_Dinsert_after (&channel->dl_users, strdup (tok_1));
		}

	    }

	}

      else if (bot->txt_server_msg == 332)
	{


/* topic */



	  data_dup = strdup (bot->txt_data_in);

	  tok_1 = strtok (data_dup, ":");
	  if (!tok_1)
	    goto cleanup;

	  tok_1 = strtok (NULL, "");
	  if (!tok_1)
	    goto cleanup;



	  dptr = chan_find (bot, data_dup);
	  if (!dptr)
	    goto cleanup;

	  channel = dlist_data (dptr);

/* channel users listing */

	  if (channel->topic)
	    free (channel->topic);

	  channel->topic = strdup (tok_1);


	}
    }

  else if (bot->isline == BOT_LINE_KICK)
    {

/* XXX 
      nick = dlist_data (bot->dl_nick_cur);
*/
      if (nick)
	{
	  nick_ptr = strchr (bot->txt_data_in, ' ');
	  if (nick_ptr)
	    *nick_ptr = '\0';
	  nick_ptr = bot->txt_data_in;
	  while (*nick_ptr)
	    {
	      if (*nick_ptr == '@' || *nick_ptr == '+')
		nick_ptr++;
	      else
		break;
	    }



	  dptr = chan_find (bot, bot->txt_to);
	  if (dptr)
	    {
	      channel = (channel_t *) dlist_data (dptr);

	      if (!strcasecmp (nick_ptr, nick->nick))
		{
		  channel->present = 0;
		}
	      else
		{
		  if (chan_del_user (bot, channel->dl_users, nick_ptr))
		    {
		    }
		}

	    }

	}

    }
  else if (bot->isline == BOT_LINE_PART)
    {

/* XXX
      nick = dlist_data (bot->dl_nick_cur);
*/
      if (nick)
	{
	  nick_ptr = strchr (bot->txt_nick, ' ');
	  if (nick_ptr)
	    *nick_ptr = '\0';
	  nick_ptr = bot->txt_nick;
	  while (*nick_ptr)
	    {
	      if (*nick_ptr == '@' || *nick_ptr == '+')
		nick_ptr++;
	      else
		break;
	    }



	  dptr = chan_find (bot, bot->txt_to);
	  if (dptr)
	    {
	      channel = (channel_t *) dlist_data (dptr);

	      if (!strcasecmp (nick_ptr, nick->nick))
		{
		  channel->present = 0;
		}
	      else
		{
		  if (chan_del_user (bot, channel->dl_users, nick_ptr))
		    {
		    }
		}

	    }

	}


    }
  else if (bot->isline == BOT_LINE_JOIN)
    {

      nick_ptr = bot->txt_nick;
      while (*nick_ptr)
	{
	  if (*nick_ptr == '@' || *nick_ptr == '+')
	    nick_ptr++;
	  else
	    break;
	}

      dptr = chan_find (bot, bot->txt_to);
      if (dptr)
	{
	  channel = (channel_t *) dlist_data (dptr);
	  nick_ptr = strdup (nick_ptr);
	  dptr = chan_add_user (bot, channel->dl_users, nick_ptr);
	  if (!dptr)
	    free (nick_ptr);
	}

    }
  else if (bot->isline == BOT_LINE_TOPIC)
    {

      dptr = chan_find (bot, bot->txt_to);
      if (!dptr)
	goto cleanup;

      channel = dlist_data (dptr);

/* channel users listing */
      if (channel->topic)
	free (channel->topic);

      channel->topic =
	strdup (bot->txt_data_in[0] ==
		':' ? bot->txt_data_in + 1 : bot->txt_data_in);

    }


  else if (bot->isline == BOT_LINE_QUIT)
    {

/* XXX
      nick = dlist_data (bot->dl_nick_cur);
*/
      if (nick)
	{
	  nick_ptr = strchr (bot->txt_nick, ' ');
	  if (nick_ptr)
	    *nick_ptr = '\0';
	  nick_ptr = bot->txt_nick;
	  while (*nick_ptr)
	    {
	      if (*nick_ptr == '@' || *nick_ptr == '+')
		nick_ptr++;
	      else
		break;
	    }


/* XXX
	  dlist_fornext (bot->dl_channel, dptr)
	  {
	    channel = (channel_t *) dlist_data (dptr);
	    dptr2 =
	      chan_find_user (bot, channel->dl_users, bot->txt_nick);
	    if (dptr2)
	      {
		dlist_remove (&channel->dl_users, dptr2);
	      }
	  }
*/

	}


    }


  else if (bot->isline == BOT_LINE_NICK)
    {

/* XXX
      nick = dlist_data (bot->dl_nick_cur);
*/
      if (nick)
	{
	  nick_ptr = bot->txt_to + 1;
	  while (*nick_ptr)
	    {
	      if (*nick_ptr == '@' || *nick_ptr == '+')
		nick_ptr++;
	      else
		break;
	    }


	  strstrip_nl (nick_ptr);

/* XXX
	  dlist_fornext (bot->dl_channel, dptr)
	  {
	    channel = (channel_t *) dlist_data (dptr);
	    dptr2 =
	      chan_find_user (bot, channel->dl_users, bot->txt_nick);
	    if (dptr2)
	      {
		dlist_remove (&channel->dl_users, dptr2);
		dlist_Dinsert_after (&channel->dl_users,
				     strdup (nick_ptr));
	      }
	  }
*/

	}

    }

cleanup:
  if (data_dup)
    free (data_dup);

  return NULL;
}








char *
chan_list (bot_t * bot, char *servertag)
{
  channel_t *channel;
  dlist_t *dptr;
  char buf[MAX_BUF_SZ + 1];
  char *str = NULL;

  if (!bot)
    return NULL;

/* XXX
  memset (buf, 0, sizeof (buf));

  dlist_fornext (bot->dl_channel, dptr)
  {
    channel = (channel_t *) dlist_data (dptr);
    if (!channel)
      return NULL;
    if (!channel->name)
      return NULL;
    strlcat_bot (buf, channel->name, sizeof (buf) - 1);
    charcat_safe (buf, ' ', sizeof (buf) - 1);
  }

  if (buf[0] != '\0')
    str = strdup (buf);
*/

  return str;

}









char *
chan_mode (bot_t * bot, char *channel, char *string)
{
  char *str = NULL;

  if (!bot || !channel || !string)
    return NULL;

  debug (bot, "chan_op: Entered\n");

  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
  strlcatfmt_bot (bot->txt_data_out, "MODE %s %s\n", channel, string);

  return str;
}







char *
chan_nick (bot_t * bot, char *nick)
{
  char *str = NULL;

  if (!bot || !nick)
    return NULL;

  nick = strtok (nick, "$!@#%&*()=+'][{}\\;'./>?<");

  debug (bot, "chan_nick: Entered, nick=%s\n", nick);

  nick_add (bot, nick, nick, nick, nick, nick);

  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
  strlcatfmt_bot (bot->txt_data_out, "NICK %s\n", nick);
  return str;
}






char *
chan_quit (bot_t * bot, char *msg)
{
  char *str = NULL;

  if (!bot || !msg)
    return NULL;

  debug (bot, "chan_quit: Entered, msg=%s\n", msg);

  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
  strlcatfmt_bot (bot->txt_data_out, "QUIT :%s\n", msg);
  return str;
}






char *
chan_join (bot_t * bot, char *channels)
{
  char *str = NULL;

  if (!bot || !channels)
    return NULL;

  debug (bot, "chan_join: Entered\n");

  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
  strlcatfmt_bot (bot->txt_data_out, "JOIN %s\n", channels);
  return str;
}



char *
chan_part (bot_t * bot, char *channels)
{
  char *str = NULL;

  if (!bot || !channels)
    return NULL;

  debug (bot, "chan_part: Entered\n");

  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
  strlcatfmt_bot (bot->txt_data_out, "PART %s\n", channels);

  return str;
}



char *
chan_users (bot_t * bot, char *channel_name)
{
/* ^chan(users,servertag:#channel */
/* if no servertag, return users for current channel */
  channel_t *channel;
  dlist_t *dptr;
  char buf[MAX_BUF_SZ + 1];
  char *str = NULL;


  if (!bot)
    return NULL;

  memset (buf, 0, sizeof (buf));

  if (!channel_name)
    channel_name = bot->txt_to;

  dptr = chan_find (bot, channel_name);
  if (dptr)
    {
      channel = (channel_t *) dlist_data (dptr);
      dlist_fornext (channel->dl_users, dptr)
      {
	strlcat_bot (buf, (char *) dlist_data (dptr));
	charcat_bot (buf, ' ');
      }
    }

  if (buf[0] != '\0')
    str = strdup (buf);

  return str;
}



char *
chan_topic (bot_t * bot, char *channel_name, char *new_topic)
{
/* ^chan(topic,servertag:#channel */
/* if no servertag, return topic for current channel */
  channel_t *channel;
  dlist_t *dptr;
  char buf[MAX_BUF_SZ + 1];
  char *str = NULL;


  if (!bot)
    return NULL;

  memset (buf, 0, sizeof (buf));

  if (!channel_name)
    channel_name = bot->txt_to;

  if (new_topic)
    {
      if (strlen (new_topic))
	{
	  memset (bot->txt_data_out, 0, sizeof (bot->txt_data_out));
	  strlcatfmt_bot (bot->txt_data_out, "TOPIC %s :%s\n", channel_name,
			  new_topic);
	  return NULL;
	}
    }

  dptr = chan_find (bot, channel_name);
  if (dptr)
    {
      channel = (channel_t *) dlist_data (dptr);
      strlcat_bot (buf, channel->topic);
    }

  if (buf[0] != '\0')
    str = strdup (buf);

  return str;
}
