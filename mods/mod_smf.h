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
#ifndef MOD_SMF_H
#define MOD_SMF_H

#include "bot.h"
#include <curl/curl.h>

module_t mod_smf_info;

bot_t *smf_init(dlist_t *, bot_t *);
bot_t *smf_fini(dlist_t *, bot_t *);
bot_t *smf_help(dlist_t *, bot_t *);
bot_t *smf_run(dlist_t *, bot_t *);

char *smf_change_string(bot_t *, char *, char *, char *, int);

size_t smf_curl_write(void *, size_t, size_t, void *);
char *smf_fix_url(char *);

extern char *identify_return_user(dlist_t *, bot_t *);

enum levels {
	SMF_LEVEL_NOTHING = 0,
	SMF_LEVEL_LOGGED_IN = 1,
	SMF_LEVEL_CHECKING_TOPIC = 2,
	SMF_LEVEL_CREATING_TOPIC = 3,
	SMF_LEVEL_CREATING_FORM = 4,
};

typedef struct smf_info {
	CURL *curl;
	CURLcode curl_res;
	char *smf_url;
	char *smf_user;
	char *smf_pass;

	char *smf_cookie_file;
	char *smf_cookie;
	char *smf_phpsessid;

	char *smf_post_sc;
	char *smf_post_seqnum;
	char *smf_post_acttopic;

	struct curl_slist *cookies;

	int level;

	char *smf_posted_url;

} smf_info_t;

size_t smf_curl_write(void *, size_t, size_t, void *);
size_t smf_curl_read(void *, size_t, size_t, void *);

smf_info_t *smf_initx(bot_t *, char *, char *, char *);
smf_info_t *smf_login(bot_t *, smf_info_t *);
smf_info_t *smf_post(bot_t *, smf_info_t *, char *, char *, char *);
smf_info_t *smf_parse_cookies(bot_t *, smf_info_t *);
smf_info_t *smf_finix(bot_t *, smf_info_t *);

int curl_sc_seqnum_acttopic(smf_info_t *, char *);

void __smf_init__(void) __attribute__ ((constructor));

#endif
