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
#include "mod_smf.h"

void
__smf_init__ (void)
{

strlcpy_buf(mod_smf_info.name, "mod_smf");
strlcpy_buf(mod_smf_info.trigger, "^smf");

  mod_smf_info.init = smf_init;
  mod_smf_info.fini = smf_fini;
  mod_smf_info.help = smf_help;
  mod_smf_info.run = smf_run;


  mod_smf_info.output = NULL;
  mod_smf_info.input = NULL;


  mod_smf_info.max_nesting = 2;

  debug (NULL, "__smf_init__: Loaded mod_smf\n");

  return;
}



bot_t *
smf_init (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "smf_init: Entered\n");


  return NULL;
}

bot_t *
smf_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "smf_fini: Entered\n");
  return NULL;
}

bot_t *
smf_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "smf_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^smf";

  return NULL;
}

bot_t *
smf_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr, *new_str = NULL;
  int opt;

  char *subforum, *topic, *ptr;

  char **array;
  int array_size, i;


  debug (bot, "smf_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);


  debug (bot,
	     "smf_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;


  subforum = "the-hole";
  topic = "Random-Thoughts-From-IRC";

  if (bot->var_nesting_cur > mod_smf_info.max_nesting)
    return NULL;

  opt = 0;
  new_str = NULL;

  MOD_OPTIONS_TOP_HALF;

  ptr = strtok (dl_options_ptr, ",");
  if (ptr)
    {
      subforum = ptr;

      ptr = strtok (NULL, "");
      if (ptr)
	{
	  topic = ptr;
	  smf_fix_url (topic);
	}

    }

  MOD_OPTIONS_BOTTOM_HALF;


  debug (bot, "smf_run: subforum=%s, topic=%s\n", subforum, topic);


/* mongo db commands for adding smf servers */
  array = NULL;
  array_size = 0;
  if (!strcasecmp (subforum, "add"))
    {
/* ^smf(add) tag baseurl defaultforum user pass */
      array_size = 5;
      array =
	tokenize_array (bot, eat_whitespace (dl_module_arg_after_options),
			    TOKENIZE_NORMAL, " ", &array_size);
      for (i = 0; array[i] != NULL; i++)
	{
	  printf ("OMG %s\n", array[i]);
	}

      return NULL;
    }
  else if (!strcasecmp (subforum, "del"))
    {
      return NULL;
    }
  else if (!strcasecmp (subforum, "get"))
    {
      return NULL;
    }


  MOD_PARSE_TOP_HALF_NODL;

  if (!bot_fork_clean (bot))
    {
      l_new_str = smf_change_string (bot, subforum, topic, l_str_ptr, opt);
    }

  MOD_PARSE_BOTTOM_HALF_NODL;


  return bot;
}







smf_info_t *
smf_initx (bot_t * bot, char *url, char *user, char *pass)
{
  smf_info_t *si;

  if (!bot || !url || !user || !pass)
    return NULL;

  si = (smf_info_t *) calloc (1, sizeof (smf_info_t));
  if (!si)
    return NULL;

  si->curl = curl_easy_init ();
  if (!si->curl)
    return NULL;

  si->smf_url = strdup (url);
  si->smf_user = strdup (user);
  si->smf_pass = strdup (pass);
  si->level = SMF_LEVEL_NOTHING;
  si->smf_cookie_file = strdup (tmpnam (NULL));

  return si;
}


smf_info_t *
smf_login (bot_t * bot, smf_info_t * si)
{
  char *postfields;

  if (!bot || !si)
    return NULL;

  if (si->curl)
    {
      curl_easy_cleanup (si->curl);
      si->curl = curl_easy_init ();
    }

  curl_easy_setopt (si->curl, CURLOPT_VERBOSE, 1);


/*
  postfields =
    "user=1turks3&passwrd=1Turks3terd&autologin=1&cookieneverexp=on";
*/

  postfields =
    str_unite ("user=%s&passwrd=%s&autologin=1&cookieneverexp=on",
	       si->smf_user, si->smf_pass);

  curl_easy_setopt (si->curl, CURLOPT_URL,
		    str_unite ("%s/login2", si->smf_url));
  curl_easy_setopt (si->curl, CURLOPT_POSTFIELDS, postfields);
  curl_easy_setopt (si->curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt (si->curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt (si->curl, CURLOPT_AUTOREFERER, 1);
  curl_easy_setopt (si->curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt (si->curl, CURLOPT_POST, 1);
  curl_easy_setopt (si->curl, CURLOPT_COOKIEJAR, si->smf_cookie_file);
  curl_easy_setopt (si->curl, CURLOPT_COOKIEFILE, si->smf_cookie_file);
  curl_easy_setopt (si->curl, CURLOPT_HEADER, 1);
  curl_easy_setopt (si->curl, CURLOPT_WRITEFUNCTION, smf_curl_write);
  curl_easy_setopt (si->curl, CURLOPT_WRITEDATA, si);
  curl_easy_setopt (si->curl, CURLOPT_READFUNCTION, smf_curl_read);


  si->curl_res = curl_easy_perform (si->curl);

  curl_easy_cleanup (si->curl);
  si->curl = NULL;


  smf_parse_cookies (bot, si);


  if (!si->smf_cookie || !si->smf_phpsessid)
    {
      goto cleanup;
    }

  si->level = SMF_LEVEL_LOGGED_IN;

cleanup:
  if (si->curl)
    curl_easy_cleanup (si->curl);

if(postfields)
free(postfields);

  return si;
}



smf_info_t *
smf_post (bot_t * bot, smf_info_t * si, char *subforum, char *topic,
	  char *body)
{
  module_t *mod = NULL;
  char *(*fn_identify_return_user) (dlist_t *, bot_t *);
  char *postfields, *url;
  char *new_topic, *new_body, *new_subforum;
  char *identified_user;
  char *eff_url;

  if (!bot || !si || !subforum || !topic || !body)
    {
      return NULL;
    }

  mod = module_find_by_name ("mod_identify");
  if (!mod)
    return NULL;
  if (!mod->dl_handle)
    return NULL;

  fn_identify_return_user = dlsym (mod->dl_handle, "identify_return_user");
  if (fn_identify_return_user)
    identified_user = identify_return_user (NULL, bot);

  if (!identified_user)
    identified_user = strdup ("unknown");

  body = str_unite ("darqbot message from %s:\n\n%s", identified_user, body);

  free (identified_user);

  debug (bot, "body=%s\n", body);

  new_topic = curl_easy_escape (si->curl, topic, strlen (topic));
  if (!topic)
    {
      free (body);
      return NULL;
    }
  new_body = curl_easy_escape (si->curl, body, strlen (body));
  if (!new_body)
    {
      free (body);
      free (new_topic);
      return NULL;
    }
  new_subforum = curl_easy_escape (si->curl, subforum, strlen (subforum));
  if (!new_subforum)
    {
      free (body);
      free (new_topic);
      free (new_body);
      return NULL;
    }

  if (si->curl)
    {
      curl_easy_cleanup (si->curl);
    }

/* see if topic already exists for that subforum */

  si->level = SMF_LEVEL_CHECKING_TOPIC;
  si->curl = curl_easy_init ();

  url = str_unite_static ("%s/%s/%s", si->smf_url, new_subforum, new_topic);

  curl_easy_setopt (si->curl, CURLOPT_URL, url);
  curl_easy_setopt (si->curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt (si->curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt (si->curl, CURLOPT_AUTOREFERER, 1);
  curl_easy_setopt (si->curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt (si->curl, CURLOPT_COOKIEJAR, si->smf_cookie_file);
  curl_easy_setopt (si->curl, CURLOPT_COOKIEFILE, si->smf_cookie_file);
  curl_easy_setopt (si->curl, CURLOPT_HEADER, 1);
  curl_easy_setopt (si->curl, CURLOPT_WRITEFUNCTION, smf_curl_write);
  curl_easy_setopt (si->curl, CURLOPT_WRITEDATA, si);
  curl_easy_setopt (si->curl, CURLOPT_READFUNCTION, smf_curl_read);
  curl_easy_setopt (si->curl, CURLOPT_VERBOSE, 1);

  si->curl_res = curl_easy_perform (si->curl);

  curl_easy_cleanup (si->curl);

  si->curl = NULL;
  smf_parse_cookies (bot, si);

  if (!si->smf_post_acttopic)
    {
    }

  si->curl = curl_easy_init ();
  si->level = SMF_LEVEL_CREATING_TOPIC;

  url = str_unite_static ("%s/%s/?action=post", si->smf_url, new_subforum);

  curl_easy_setopt (si->curl, CURLOPT_URL, url);
  curl_easy_setopt (si->curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt (si->curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt (si->curl, CURLOPT_AUTOREFERER, 1);
  curl_easy_setopt (si->curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt (si->curl, CURLOPT_COOKIE,
		    str_unite ("SMFCookie60=%s", si->smf_cookie));
  curl_easy_setopt (si->curl, CURLOPT_COOKIEJAR, si->smf_cookie_file);
  curl_easy_setopt (si->curl, CURLOPT_COOKIEFILE, si->smf_cookie_file);
  curl_easy_setopt (si->curl, CURLOPT_HEADER, 1);
  curl_easy_setopt (si->curl, CURLOPT_WRITEFUNCTION, smf_curl_write);
  curl_easy_setopt (si->curl, CURLOPT_WRITEDATA, si);
  curl_easy_setopt (si->curl, CURLOPT_READFUNCTION, smf_curl_read);
  curl_easy_setopt (si->curl, CURLOPT_VERBOSE, 1);

  si->curl_res = curl_easy_perform (si->curl);

  curl_easy_cleanup (si->curl);

  si->curl = NULL;
  smf_parse_cookies (bot, si);


  if (!si->smf_post_sc || !si->smf_post_seqnum)
    {
      return NULL;
    }


  printf ("sc=[%s], seqnum=[%s]\n", si->smf_post_sc, si->smf_post_seqnum);

  si->level = SMF_LEVEL_CREATING_FORM;
  si->curl = curl_easy_init ();

  postfields = NULL;
  url = str_unite_static ("%s/%s/?action=post2;start=0", si->smf_url, new_subforum);

  curl_easy_setopt (si->curl, CURLOPT_URL, url);
  curl_easy_setopt (si->curl, CURLOPT_NOPROGRESS, 1);
  curl_easy_setopt (si->curl, CURLOPT_NOSIGNAL, 1);
  curl_easy_setopt (si->curl, CURLOPT_AUTOREFERER, 1);
  curl_easy_setopt (si->curl, CURLOPT_FOLLOWLOCATION, 1);
  curl_easy_setopt (si->curl, CURLOPT_POST, 1);
  curl_easy_setopt (si->curl, CURLOPT_COOKIE,
		    str_unite ("SMFCookie60=%s; PHPSESSID=%s;",
			       si->smf_cookie, si->smf_phpsessid));
  curl_easy_setopt (si->curl, CURLOPT_HEADER, 1);
  curl_easy_setopt (si->curl, CURLOPT_WRITEFUNCTION, smf_curl_write);
  curl_easy_setopt (si->curl, CURLOPT_WRITEDATA, si);

  postfields =
    str_unite
    ("postmodify&topic=%s&subject=%s&icon=xx&num_replies=9999999&message=%s&notify=0&goback=1&post=Post&sc=%s&seqnum=%s",
     si->smf_post_acttopic == NULL ? "0" : si->smf_post_acttopic, new_topic,
     new_body, si->smf_post_sc, si->smf_post_seqnum);

  printf ("sending postfields=%s\n", postfields);
  curl_easy_setopt (si->curl, CURLOPT_POSTFIELDS, postfields);


  sleep (6);


  curl_easy_setopt (si->curl, CURLOPT_VERBOSE, 1);
  si->curl_res = curl_easy_perform (si->curl);

  curl_easy_getinfo (si->curl, CURLINFO_EFFECTIVE_URL, &eff_url);
  curl_easy_cleanup (si->curl);

  si->curl = NULL;
  smf_parse_cookies (bot, si);

  si->smf_posted_url =
    str_unite ("%s/%s/%s", si->smf_url, new_subforum, new_topic);

if(postfields)
free(postfields);

  return si;
}





smf_info_t *
smf_parse_cookies (bot_t * bot, smf_info_t * si)
{
  struct curl_slist *nc;
  int i;

  FILE *fp;
  char *s;
  char buf[1024];

  if (!bot || !si)
    return NULL;

  printf ("Cookies, curl knows:\n");

  if (si->curl == NULL)
    {
      fp = fopen (si->smf_cookie_file, "r");
      if (!fp)
	return NULL;

      while (1)
	{
	  memset (buf, 0, sizeof (buf));
	  if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	    break;


	  s = strstr (buf, "PHPSESSID");
	  if (s)
	    {
	      s = strtok (s, "\t");
	      if (!s)
		continue;
	      s = strtok (NULL, "\t");
	      if (!s)
		continue;
	      si->smf_phpsessid = strdup (s);
	      strstrip_nl (si->smf_phpsessid);
	      printf ("Cookie PHPSESSID=%s\n", si->smf_phpsessid);
	    }

	  s = strstr (buf, "SMFCookie60");
	  if (s)
	    {
	      s = strtok (s, "\t");
	      if (!s)
		continue;
	      s = strtok (NULL, "\t");
	      if (!s)
		continue;
	      si->smf_cookie = strdup (s);
	      strstrip_nl (si->smf_cookie);
	      printf ("Cookie SMFCookie60=%s\n", si->smf_cookie);
	    }
	}

goto cleanup;
    }

  si->curl_res =
    curl_easy_getinfo (si->curl, CURLINFO_COOKIELIST, &si->cookies);
  if (si->curl_res != CURLE_OK)
    {
      fprintf (stderr, "Curl curl_easy_getinfo failed: %s\n",
	       curl_easy_strerror (si->curl_res));
si = NULL;
goto cleanup;
    }
  nc = si->cookies, i = 1;

  if (!nc)
    {
si = NULL;
goto cleanup;
    }
  while (nc)
    {
      printf ("[%d]: %s\n", i, nc->data);

      s = strstr (nc->data, "PHPSESSID");
      if (s)
	{
	  s = strtok (s, "\t");
	  if (!s)
	    goto continue_loop;
	  s = strtok (NULL, "\t");
	  if (!s)
	    goto continue_loop;
	  si->smf_phpsessid = strdup (s);
	  goto continue_loop;
	}

      s = strstr (nc->data, "SMFCookie60");
      if (s)
	{
	  s = strtok (s, "\t");
	  if (!s)
	    goto continue_loop;
	  s = strtok (NULL, "\t");
	  if (!s)
	    goto continue_loop;
	  si->smf_cookie = strdup (s);
	  goto continue_loop;
	}

    continue_loop:
      nc = nc->next;
      i++;
    }
  if (i == 1)
    {
      printf ("(none)\n");
    }
//                    curl_slist_free_all(cookies);

cleanup:
if(fp)
fclose(fp);

  return si;
}




char *
smf_change_string (bot_t * bot, char *subforum, char *topic, char *string,
		   int opt)
{
  smf_info_t *si;
  char *str = NULL;
  char buf[MAX_BUF_SZ];

  char *sep_ptr;

  char *user;
  char *pass;
  char *body;

  char *postfields;
  char *url;

  if (!string || !bot)
    return NULL;

  sep_ptr = str_find_sep (string);
  if (sep_ptr)
    string = sep_ptr;

bz(buf);

  user = pass = body = postfields = url = NULL;
  body = string;

  user = "darqbot";
  pass = "darqbot999";

  printf ("user=%s, pass=%s, subforum=%s, topic=%s, body=%s\n", user, pass,
	  subforum, topic, body);

  si = smf_initx (bot, "http://www.adarq.org/forum", user, pass);
  if (!si)
    {
      bot_fork_clean_exit (bot);
    }

  if (!smf_login (bot, si))
    {
      bot_fork_clean_exit(bot);
    }

  if (!smf_post (bot, si, subforum, topic, body))
    {
bot_fork_clean_exit(bot);
    }

  if (si->smf_posted_url)
    {
      str = strdup (si->smf_posted_url);
    }


  smf_finix (bot, si);

  return str;
}





















size_t
smf_curl_write (void *buf, size_t size, size_t nmemb, void *udata)
{
  smf_info_t *si;

  si = (smf_info_t *) udata;

  if (!si)
    return size * nmemb;

  if (si->level == SMF_LEVEL_CREATING_TOPIC
      || si->level == SMF_LEVEL_CHECKING_TOPIC)
    {
      curl_sc_seqnum_acttopic (si, buf);
    }



  return size * nmemb;
}


size_t
smf_curl_read (void *buf, size_t size, size_t nmemb, void *udata)
{


  return size * nmemb;
}



int
curl_sc_seqnum_acttopic (smf_info_t * si, char *buf)
{
  char *str, *op, *val;

/* <input type="hidden" name="acttopic" value="4795" /> */

  if (!si || !buf)
    return 0;

  str = buf;
  while (1)
    {
      str = strstr (str, "<input type=\"hidden\" name=\"");
      if (!str)
	return 0;

      printf ("FOUND %.*s\n", 50, str);
      op = &str[strlen ("<input type=\"hidden\" name=\"")];

      if (!strncasecmp (op, "sc\"", strlen ("sc\"")))
	{
	  val = strstr (op, "value=\"");
	  val = &val[strlen ("value=\"")];
	  si->smf_post_sc = strdup (val);
	  val = strchr (si->smf_post_sc, '\"');
	  if (val)
	    *val = '\0';
	}
      else if (!strncasecmp (op, "seqnum\"", strlen ("seqnum\"")))
	{
	  val = strstr (op, "value=\"");
	  val = &val[strlen ("value=\"")];
	  si->smf_post_seqnum = strdup (val);
	  val = strchr (si->smf_post_seqnum, '\"');
	  if (val)
	    *val = '\0';
	}
      else if (!strncasecmp (op, "acttopic\"", strlen ("acttopic\"")))
	{
	  val = strstr (op, "value=\"");
	  val = &val[strlen ("value=\"")];
	  si->smf_post_acttopic = strdup (val);
	  val = strchr (si->smf_post_acttopic, '\"');
	  if (val)
	    *val = '\0';
	}


      str++;
    }

  return 0;
}







smf_info_t *
smf_finix (bot_t * bot, smf_info_t * si)
{



  if (!si)
    return NULL;

  if (si->smf_url)
    free (si->smf_url);

  if (si->smf_user)
    free (si->smf_user);

  if (si->smf_pass)
    free (si->smf_pass);

  if (si->smf_cookie_file)
    free (si->smf_cookie_file);

  if (si->smf_cookie)
    free (si->smf_cookie);

  if (si->smf_phpsessid)
    free (si->smf_phpsessid);

  if (si->smf_post_sc)
    free (si->smf_post_sc);

  if (si->smf_post_seqnum)
    free (si->smf_post_seqnum);

  if (si->smf_post_acttopic)
    free (si->smf_post_acttopic);

  if (si->smf_posted_url)
    free (si->smf_posted_url);


//  struct curl_slist *cookies;

  memset (si, 0, sizeof (smf_info_t));
  free (si);

  return NULL;
}
















char *
smf_fix_url (char *s)
{
  char *b = s;
  if (!s)
    return NULL;

  while (*s)
    {
      if (*s == ' ')
	*s = '-';
      s++;
    }

  return b;
}
