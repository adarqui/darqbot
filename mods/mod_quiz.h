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
#ifndef MOD_QUIZ_H
#define MOD_QUIZ_H

/*
 *
 *
 */
#include "bot.h"

dlist_t *dl_quiz_pairs;
dlist_t *dl_quiz_active_unique;

enum quiz_opts {
	QUIZ_OPT_RUN = 0,
	QUIZ_OPT_GET = 1,
	QUIZ_OPT_DEL = 2,
	QUIZ_OPT_ADD = 3,
	QUIZ_OPT_GETMORE = 4,
	QUIZ_OPT_LIST = 5,
	QUIZ_OPT_ANSWER = 6,
	QUIZ_OPT_INFO = 7,
	QUIZ_OPT_SETUP = 8,
	QUIZ_OPT_OFF = 9,
	QUIZ_OPT_ON = 10,
	QUIZ_OPT_CLEAR = 11,
	QUIZ_OPT_TIMEOUT = 12,
	QUIZ_OPT_CLEAR_WINNERS = 13,
	QUIZ_OPT_SIZE = 14,
	QUIZ_OPT_WINNERS = 15,
	QUIZ_OPT_CHANNEL = 16,
	QUIZ_OPT_MULTI = 17,
	QUIZ_OPT_NEWLINES = 18,
	QUIZ_OPT_HIDEKEY = 19,
	QUIZ_OPT_FIXKEY = 20,
};

module_t mod_quiz_info;

bot_t *quiz_init(dlist_t *, bot_t *);
bot_t *quiz_fini(dlist_t *, bot_t *);
bot_t *quiz_help(dlist_t *, bot_t *);
bot_t *quiz_run(dlist_t *, bot_t *);
bot_t *quiz_timer(dlist_t *, bot_t *);
bot_t *quiz_off(dlist_t *, bot_t *);

void __quiz_init__(void) __attribute__ ((constructor));

typedef struct quiz_active {
	unsigned int ID;

/*
  char *irc_channel;
*/

	int incorrect;
	int correct;
	int count;

	time_t start;
	int timeout;

	size_t size;

	int multi;
	int newlines;
	int hidekey;
	int fixkey;

	dlist_t *dl_topics;
	dlist_t *dl_winners;

	xdb_pair_t *question;

} quiz_active_t;

typedef struct quiz_winners {
	char *nick;
	int count;
} quiz_winner_t;

char *quiz_change_string(bot_t *, char *, int);

char *quiz_op_get(bot_t *, char *, int);
char *quiz_op_del(bot_t *, char *);
char *quiz_op_list(bot_t *);
char *quiz_op_setup(bot_t *, quiz_active_t **, char *);
char *quiz_op_answer(bot_t *, quiz_active_t *, char *);
char *quiz_op_info(bot_t *, quiz_active_t *);
char *quiz_op_timeout(bot_t *, quiz_active_t *, int);
char *quiz_op_clear(bot_t *, quiz_active_t *);
char *quiz_op_clear_winners(bot_t *, quiz_active_t *);
char *quiz_op_size(bot_t *, char *);
char *quiz_op_winners(bot_t *, quiz_active_t *);
char *quiz_op_multi(bot_t *, quiz_active_t *, char *);
char *quiz_op_newlines(bot_t *, quiz_active_t *, char *);
char *quiz_op_hidekey(bot_t *, quiz_active_t *, char *);
char *quiz_op_fixkey(bot_t *, quiz_active_t *, char *);

void quiz_clear_db_from_files();
void quiz_add_files_to_db_file(char *);
void quiz_add_files_to_db(void);

char *quiz_change_string_fn_null(bot_t *, char *, int);

quiz_active_t *quiz_active_init(bot_t *, char *);
char *quiz_active_ask(bot_t *, quiz_active_t *);
quiz_active_t *quiz_active_find(bot_t *);
void quiz_active_set_question(quiz_active_t *, xdb_pair_t *);
char *quiz_active_answer(bot_t *, quiz_active_t *, char *);
char *quiz_active_answer_return(bot_t *, quiz_active_t *);
void quiz_active_winner(bot_t *, quiz_active_t *, char *);
void quiz_active_incorrect(bot_t *, quiz_active_t *, char *);
int quiz_active_size(quiz_active_t *, char *);
int quiz_pair_size(char *);
void quiz_active_defaults(quiz_active_t *);

void quiz_winner_free(void *);
void quiz_active_free(void *);

#endif
