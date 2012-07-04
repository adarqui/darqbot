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
#include "gmod_gssl.h"


void __gssl_init__ (void) __attribute__ ((constructor));

void
__gssl_init__ (void)
{

/*
  gmod_gssl_info.name = "gmod_gssl";
  gmod_gssl_info.trigger = strdup ("^ssl");
*/
strlcpy_buf(gmod_gssl_info.name, "gmod_gssl");
strlcpy_buf(gmod_gssl_info.trigger, "^gssl");

  gmod_gssl_info.init = gssl_init;
  gmod_gssl_info.fini = gssl_fini;
  gmod_gssl_info.help = gssl_help;
  gmod_gssl_info.run = gssl_run;

  gmod_gssl_info.off = gssl_off;
  gmod_gssl_info.destroy_up = gssl_destroy_up;
  gmod_gssl_info.destroy_down = gssl_destroy_down;

  gmod_gssl_info.control_up = gssl_control_up;
  gmod_gssl_info.control_down = gssl_control_down;

  gmod_gssl_info.output = gssl_output;
  gmod_gssl_info.input = gssl_input;

  gmod_gssl_info.self = &gmod_gssl_info;

  debug (NULL, "__gssl_init__: Loaded gmod_gssl\n");

  return;
}



bot_t *
gssl_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gssl_init: Entered\n");
  return NULL;
}

bot_t *
gssl_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gssl_fini: Entered\n");
  return NULL;
}


bot_t *
gssl_off (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gssl_t *gssl = NULL;

  debug (bot, "gssl_off: Entered\n");


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

  gssl = (gssl_t *) gmod->data;
  if (!gssl)
    {
      return NULL;
    }

  free (gssl);

  return NULL;
}





bot_t *
gssl_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "gssl_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^gssl";

  return NULL;
}


bot_t *
gssl_run (dlist_t * dlist_node, bot_t * bot)
{
  dlist_t *dptr_gmod = NULL;
  bot_gmod_elm_t *gmod = NULL;
  gssl_t *gssl = NULL;

  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "gssl_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "gssl_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

  printf ("dl_module_arg=%s\n", bot->dl_module_arg);


  dptr_gmod =
    (dlist_t *) gmodule_find_gmod_dptr (bot, gmod_gssl_info.trigger,
					gmod_gssl_info.trigger_ext);
  if (!dptr_gmod)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dptr_gmod);
  if (!gmod)
    {
      puts ("GSSL GMOD=NULL");
      return NULL;
    }

  if (!gmod->data)
    {
      puts ("GIRCD GMOD->DATA = NULL, ALLOCATING");
      gssl_gmod_init (bot, gmod, dptr_gmod);
      gssl = gmod->data;
    }

  MOD_OPTIONS_TOP_HALF;

  gssl_process_options (gssl, dl_options_ptr);

  MOD_OPTIONS_BOTTOM_HALF;

  gmodule_set_can_pass_up (gmod);
  gmodule_set_can_pass_down (gmod);

  gssl_input (dptr_gmod, bot);

  gmodule_set_has_run (gmod);

  return bot;
}



char *
gssl_process_options (gssl_t * gssl, char *string)
{

  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;

  dlist_t *dl = NULL, *dptr;

  debug (NULL, "gssl_process_options: Entered\n");


  if (!gssl || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));


  dl =
    tokenize (NULL, string,
		  TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES, "...");
  if (!dl)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    gssl_process_options_parse (gssl, (char *) dlist_data (dptr));
  }


  tokenize_destroy (NULL, &dl);

  return str;
}






void
gssl_process_options_parse (gssl_t * gssl, char *string)
{

  if (!gssl || !string)
    return;

  debug (NULL, "gssl_process_options_parse: Entered\n");

  if (!strcasecmp (string, "blah"))
    {
    }

  gmodule_parse_common_options (gssl->bot, gssl->gmod, string);

  return;
}




void
gssl_process_options_parse_blah (gssl_t * gssl, char *string)
{

  if (!gssl || !string)
    return;

  return;
}



char *
gssl_change_string (bot_t * bot, char *string, int opt)
{
  char *str = NULL;
  char buf[MAX_BUF_SZ];


  char *sep_ptr;


  if (!bot || !string)
    return NULL;


  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

  memset (buf, 0, sizeof (buf));

  return str;
}






bot_t *
gssl_output (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gssl_t *gssl = NULL;
  int n;
  char buf[MAX_BUF_SZ + 1];

  debug (bot, "gssl_output: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  gssl = (gssl_t *) gmod->data;
  if (!gssl)
    {
      puts ("GSSL_OUTPUT: gssl=NULL");
      return NULL;
    }

  memset (buf, 0, sizeof (buf));

  debug (NULL, "gssl_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	     bot->txt_data_out);

  if (bot->txt_data_out_sz > 0)
    {
      n = SSL_write (gssl->ssl, bot->txt_data_out, bot->txt_data_out_sz);

      if (n > 0)
	{
	  n =
	    BIO_read (gssl->obio, bot->txt_data_out,
		      sizeof (bot->txt_data_out) - 1);

	  if (n > 0)
	    {
	      bot->txt_data_out_sz = n;
	    }

	}


    }

  if (SSL_want_write (gssl->ssl))
    {
      puts ("SSL_want_write");
    }

  gmodule_down (dlist_node, bot);

  return bot;
}


bot_t *
gssl_input (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gssl_t *gssl = NULL;
  int n, ssl_errno;
  char buf[MAX_BUF_SZ];

  debug (bot, "gssl_input: Entered\n");

  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  if (!gmod)
    return NULL;

  gssl = (gssl_t *) gmod->data;
  if (!gssl)
    {
      puts ("GSSL_INPUT: gssl=NULL");
      return NULL;
    }

  memset (buf, 0, sizeof (buf));

  if (bot->txt_data_in_sz > 0)
    {
      n = BIO_write (gssl->ibio, bot->txt_data_in, bot->txt_data_in_sz);

      while (1)
	{
	  gssl_check_status (gssl);

	  n = SSL_read (gssl->ssl, buf, sizeof (buf) - 1);

	  if (n < 0 && gssl->initialized > 0)
	    {
	      ssl_errno = SSL_get_error (gssl->ssl, n);
	      if (ssl_errno == 2)
		break;
	    }

	  if (n < 0 && gssl->initialized > 0)
	    {
	      gssl_destroy_down (dlist_node, bot);
	      return NULL;
	    }

	  if (n < 0)
	    {
	      if (SSL_want_read (gssl->ssl))
		{
		  puts ("SSL_want_read");
		  gssl_handshake (gssl);
		}
	      return NULL;
	    }

	  memcpy (bot->txt_data_in, buf, n);
	  bot->txt_data_in_sz = n;

	  gmodule_up (dlist_node, bot);

	  if (SSL_want_read (gssl->ssl))
	    {
	      puts ("SSL_want_read 2");
	    }
	}
    }

  debug (NULL, "gssl_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	     bot->txt_data_out);

  return bot;
}




bot_t *
gssl_destroy_up_gssl (gssl_t * gssl)
{
  bot_t *ret = NULL;

  debug (NULL, "gssl_destroy_up_gssl: Entered\n");

  if (gssl)
    {
      ret = gssl_destroy_up (gssl->dptr_gmod, gssl->bot);
    }

  return ret;
}


bot_t *
gssl_destroy_down_gssl (gssl_t * gssl)
{
  bot_t *ret = NULL;

  debug (NULL, "gssl_destroy_down_gssl: Entered\n");

  if (gssl)
    {
      ret = gssl_destroy_down (gssl->dptr_gmod, gssl->bot);
    }

  return ret;
}




bot_t *
gssl_destroy_up (dlist_t * dlist_node, bot_t * bot)
{
  bot_gmod_elm_t *gmod = NULL;
  gssl_t *gssl = NULL;

  debug (NULL, "gssl_destroy_up: Entered\n");

  gmodule_destroy_up (dlist_node, bot);

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  gssl = gmod->data;
	  free (gssl);
	}
    }

  return bot;
}

bot_t *
gssl_destroy_down (dlist_t * dlist_node, bot_t * bot)
{

  bot_gmod_elm_t *gmod = NULL;
  gssl_t *gssl = NULL;

  debug (NULL, "gssl_destroy_up: Entered\n");

  if (dlist_node)
    {
      gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
      if (gmod)
	{
	  gssl = gmod->data;
	  free (gssl);
	}
    }

  gmodule_destroy_down (dlist_node, bot);

  return bot;
}






void
gssl_gmod_init (bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
  gssl_t *gssl = NULL;
  debug (NULL, "gssl_gmod_init: Entered\n");

  if (!gmod || !bot || !dptr_gmod)
    return;

  if (gmod->data)
    return;

  gssl = (gssl_t *) calloc (1, sizeof (gssl_t));

  if (!gssl)
    return;

  gmod->data = gssl;

  gssl->bot = bot;
  gssl->dptr_gmod = dptr_gmod;
  gssl->gmod = gmod;


  gssl->ctx = SSL_CTX_new (SSLv23_client_method ());
  if (!gssl->ctx)
    {
      debug (NULL, "gssl_gmod_init: SSL_CTX_new: Failed\n");
      ERR_print_errors_fp (stderr);
      goto cleanup_error;
    }

  gssl->ssl = SSL_new (gssl->ctx);
  if (!gssl->ssl)
    {
      debug (NULL, "gssl_gmod_init: SSL_new: Failed\n");
      ERR_print_errors_fp (stderr);
      goto cleanup_error;
    }

  gssl->ibio = BIO_new (BIO_s_mem ());
  gssl->obio = BIO_new (BIO_s_mem ());

  SSL_set_bio (gssl->ssl, gssl->ibio, gssl->obio);


  SSL_set_connect_state (gssl->ssl);


  return;


cleanup_error:
  if (gssl->ctx)
    {
    }

  if (gssl->ssl)
    {
    }

  if (gssl)
    free (gssl);


  return;
}





int
gssl_handshake (gssl_t * gssl)
{
  int n = 0;
  char buf[MAX_BUF_SZ];

  debug (NULL, "gssl_handshake: Entered\n");

  if (!gssl)
    return -1;

  memset (buf, 0, sizeof (buf));

  n =
    BIO_read (gssl->obio, gssl->bot->txt_data_out,
	      sizeof (gssl->bot->txt_data_out) - 1);
  if (n > 0)
    {
      gssl->bot->txt_data_out_sz = n;
      gmodule_down (gssl->dptr_gmod, gssl->bot);
      return 0;
    }
  else
    {
      n = SSL_write (gssl->ssl, "hello\n", 6);
    }

  if (n <= 0)
    {
      ERR_print_errors_fp (stderr);
    }

  memset (buf, 0, sizeof (buf));

  n =
    BIO_read (gssl->obio, gssl->bot->txt_data_out,
	      sizeof (gssl->bot->txt_data_out) - 1);

  if (n > 0)
    {
      gssl->bot->txt_data_out_sz = n;

      gmodule_down_force (gssl->dptr_gmod, gssl->bot);
    }


  return 0;
}




bot_t *
gssl_control_up (dlist_t * dlist_node, bot_t * bot)
{
  gssl_t *gssl = NULL;
  bot_gmod_elm_t *gmod = NULL;

  debug (NULL, "gssl_control_up: Entered\n");

  if (!dlist_node || !bot)
    {
      return NULL;
    }


  gmod = (bot_gmod_elm_t *) dlist_data (dlist_node);
  gssl = (gssl_t *) gmod->data;
  if (!gssl)
    {


      gssl_gmod_init (bot, gmod, dlist_node);
      gssl = gmod->data;

/* initiate SSL packets */
      gssl_handshake (gssl);

    }


  gmodule_control_up (dlist_node, bot);

  return bot;
}




bot_t *
gssl_control_down (dlist_t * dlist_node, bot_t * bot)
{
  debug (NULL, "gssl_control_down: Entered\n");


  if (!dlist_node || !bot)
    return NULL;

  gmodule_control_down (dlist_node, bot);

  return bot;
}



void
gssl_free (void *arg)
{
  gssl_t *gssl = (gssl_t *) arg;

  if (!gssl)
    return;

/* ... */

  if (!gssl->gmod)
    return;

  gssl->gmod->data = NULL;
  free (gssl);

  return;
}




void
gssl_check_status (gssl_t * gssl)
{
  if (!gssl)
    return;

  if (gssl->initialized)
    return;

  if (SSL_is_init_finished (gssl->ssl))
    {
      gssl->initialized = 1;
    }

  return;
}
