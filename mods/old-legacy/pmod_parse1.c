/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include "pmod_parse1.h"

void
__parse1_init__ (void)
{

strlcpy_buf(pmod_parse1_info.name, "pmod_parse1");
strlcpy_buf(pmod_parse1_info.trigger, "^parse1");

  pmod_parse1_info.init = parse1_init;
  pmod_parse1_info.fini = parse1_fini;
  pmod_parse1_info.help = parse1_help;
  pmod_parse1_info.run = parse1_run;


  pmod_parse1_info.output = NULL;
  pmod_parse1_info.input = NULL;


  debug (NULL, "__parse1_init__: Loaded mod_parse1\n");

  return;
}



bot_t *
parse1_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "parse1_init: Entered\n");
  return NULL;
}

bot_t *
parse1_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "parse1_fini: Entered\n");
  return NULL;
}

bot_t *
parse1_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "parse1_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^parse1";

  return NULL;
}

bot_t *
parse1_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;

  debug (bot, "parse1_run: Entered\n");

  pmodule_cur_set (parse1_run, dlist_node, bot);

  if (!dlist_node || !bot)
    return NULL;

  debug (bot,
	     "parse1_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

  stat_inc(bot,bot->trig_called);

  MOD_OPTIONS_TOP_HALF;
  MOD_OPTIONS_BOTTOM_HALF;

  parse1_handle_text (dlist_node, bot, NULL);

  return bot;
}




int
parse1_handle_text (dlist_t * dlist_node, bot_t * bot, void *arg_info)
{
  dlist_t *text_list;
  dlist_t *dptr, *dptr_mod;
  module_t *module;
  char *str_ptr, *trig_ptr, *trigfound_ptr = NULL;
  int trigfound, subtrig_found;

  int i, j, k;

  int trig_called_proper_len = 0;

  debug (bot, "parse1_handle_text: Entered\n");


  if (!bot)
    return -1;

/* 
 * ^trigger text1
 * ^trigger(options) text1
 * ^trigger text1 || ^trigger ...
 * ^trigger text1 || ^trigger(options) ...
 * text1 || ^trigger
 * text1 || ^trigger(options)
 * text1 || ^trigger text2 
 * text1 || ^trigger(options) text2
 *
 * stack/queue
 * {module, input_text, text} <- module operates on input_text, then text
 * ^trigger text1 = {^trigger, NULL, text1}
 * text1 || ^trigger = {NULL, NULL, text1} {^trigger, INPUT, NULL}
 * text1 || ^trigger text2 = {NULL, NULL, text1} {^trigger, INPUT, text2}
 * ^trigger1 text1 || ^trigger2 text2 = {^trigger1, NULL, text1} {^trigger2, INPUT, text2}
 */

  debug (bot, "parse1_handle_text: Entered\n");

  debug (bot,
	     "parse1_handle_text: nesting level is %i (current max nesting is %i)\n",
	     bot->var_nesting_cur, bot->var_nesting);
  if (bot->var_nesting_cur >= bot->var_nesting)
    {
      debug (bot, "parse1_handle_text: nesting limit reached!\n");
      return 0;
    }
  bot->var_nesting_cur++;


  text_list = dptr = NULL;

  str_ptr = bot->txt_data_in;

  debug (bot, "parse1_handle_text: to=%s, out=%s, in=%s\n", bot->txt_to,
	     bot->txt_data_out, bot->txt_data_in);



/* hooks: output module processing */
  dlist_fornext (gi->dl_module, dptr)
  {
    module = (module_t *) dlist_data (dptr);
    if (module->output)
      module->output (dptr, bot);
  }



  if (bot->isprivmsg)
    {
      i = str_transform_pipes (str_ptr);


      while (1)
	{
	  str_ptr = strtok (str_ptr, "|");
	  if (!str_ptr)
	    break;

	  j = strlen (str_ptr);
	  i = str_untransform_pipes (str_ptr);

	  dptr = dlist_Dinsert_after (&text_list, strdup (str_ptr));
	  str_ptr = NULL;
	}

      memset (bot->txt_data_in, 0, sizeof (bot->txt_data_in));

      if (text_list)
	{
	  dlist_fornext (text_list, dptr)
	  {


	    str_ptr = (char *) dlist_data (dptr);
	    debug (bot, "parse1_handle_text: text_list: node=%s\n",
		       str_ptr);

// FIX - change this to searching through trigger mods only
	    trigfound = 0;


	    dlist_fornext (gi->dl_module, dptr_mod)
	    {
// FIXED
	      subtrig_found = 0;
	      module = (module_t *) dlist_data (dptr_mod);
	      if (!module)
		continue;

	      if (!module->trigger || !module->run)
		continue;

	      trig_ptr = eat_whitespace (str_ptr);

	      memset (bot->trig_called, 0, sizeof (bot->trig_called));
	      strlcpy_buf (bot->trig_called, trig_ptr);

	      if (!strncmp (bot->trig_called, "^:", 2)
		  || !strncmp (bot->trig_called, "^!", 2)
		  || !strncmp (bot->trig_called, "^#", 2))
		{
		  bot->trig_called[2] = '\0';
		}
	      else
		{
		  for (i = 1; i < strlen (bot->trig_called); i++)
		    {
		      if (!isalnum (bot->trig_called[i]))
			{
			  bot->trig_called[i] = '\0';
			  break;
			}
		    }
		}

/*
	      if (!strcasecmp (bot->trig_called, module->trigger)
		  || !strcasecmp (bot->trig_called, module->subtrig))
*/

	      trigfound_ptr = NULL;

	      subtrig_found = 0;
	      if (!strcasecmp (bot->trig_called, module->trigger))
		{
		  trigfound_ptr = module->trigger;

		  trig_called_proper_len = fn_strlen (module->trigger);

		  subtrig_found = 1;
		}
	      else
		{
		  for (k = 0; module->subtrigs[k] != NULL; k++)
		    {
		      if (!strcasecmp (bot->trig_called, module->subtrigs[k]))
			{
			  trigfound_ptr = module->subtrigs[k];

			  trig_called_proper_len =
			    fn_strlen (module->subtrigs[k]);

			  subtrig_found = 1;
			  break;
			}

		    }
		}

	      if (subtrig_found)
		{
//subtrig_found=0;

		  debug (bot, "parse1_handle_text: trigger caught, %s\n",
			     bot->trig_called);
		  bot->dl_module_arg = trig_ptr + trig_called_proper_len;
// FIX
		  module->run (dptr, bot);


		  debug (bot,
			     "parse1_handle_text: bot->isforked=%i, bot->isforked_stop=%i\n",
			     bot->isforked, bot->isforked_stop);

		  if (bot->isforked_stop)
		    {
		      bot->isforked_stop = 0;
		      trigfound = 0;
// FIX?
		      bot->shouldsend = 0;
		      goto cleanup;
		    }

		  trigfound = 1;


		}

	    }

	    if (!trigfound)
	      {
		strlcat_bot (bot->txt_data_out, str_ptr);
	      }


	  }


	}



      else
	debug (bot, "parse1_handle_text: text_list=NULL\n");
    }


cleanup:
  dlist_fini (&text_list, free);

  return 0;
}
