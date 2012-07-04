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
#ifndef MODULE_H
#define MODULE_H

#include "bot.h"

/* module stuff */

module_t *module_load (char *);

module_t * module_unload(module_t *);

void module_timerlist(void);

void module_iolist(void);

void module_list(void);

char *module_path (char *);

void module_load_all (void);

void module_deactivate_dptr(void *);

void module_deactivate(void *);

void xmodule_free_destroy(void *);

bot_t * module_unload_all(void);

void module_add_subtrigs(module_t *, char *);

void module_update(module_t *, char *) ;

dlist_t * module_find_by_trig_dptr(char *);

module_t * module_find_by_trig(char *);

module_t * module_find_by_name(char *);

void module_lock(char *);

void module_unlock(char *);

void module_update_stats(void);

void module_sort(void);

void modules_off(void) ;

/* can process args or prior text */
#define MOD_PARSE_TOP_HALF \
  { \
  char * l_new_str=NULL; \
  char * l_str_ptr=NULL; \
  dlist_t * l_dl, *l_dptr; \
  int l_c, l_should_strzero=0; \
  l_str_ptr = eat_whitespace (dl_module_arg_after_options); \
  if (!l_str_ptr || !strlen (l_str_ptr)) { \
      l_str_ptr = bot->txt_data_out; \
      puts("STRZERO"); \
      l_should_strzero = 1; \
    } \
  if (!sNULL(l_str_ptr)) \
    return bot; \
  l_dl = tokenize (bot, l_str_ptr, TOKENIZE_NORMAL | TOKENIZE_LEAVESEP | TOKENIZE_LEAVEQUOTES, "\n"); \
  if (!l_dl) return NULL; \
  dlist_fornext (l_dl, l_dptr) { \
    l_str_ptr = (char *) dlist_data (l_dptr); \
    if (!l_str_ptr) continue; \
    l_c = l_str_ptr[strlen (l_str_ptr) - 1]; \
    if (l_c == '\n') { \
        l_str_ptr[strlen (l_str_ptr) - 1] = '\0'; } \

#define MOD_PARSE_BOTTOM_HALF \
    if (l_new_str) { \
        printf("NEW_STR=%s\n", l_new_str); \
        if (l_should_strzero) { \
            strzero_bot_safe (bot->txt_data_out, sizeof (bot->txt_data_out)); \
            l_should_strzero = 0; } \
        strlcat_bot_safe (bot->txt_data_out, l_new_str, sizeof(bot->txt_data_out)-1); \
        if (l_c == '\n') { \
            charcat_bot_safe (bot->txt_data_out, l_c, sizeof (bot->txt_data_out) - 1);  } \
        free (l_new_str); \
      } \
  } \
tokenize_destroy(bot, &l_dl); \
} \



/* no need for any args, ie, ^backtrace */
#define MOD_PARSE_TOP_HALF_VOID \
  { \
  char * l_new_str=NULL; \
  char * l_str_ptr=NULL; \

#define MOD_PARSE_BOTTOM_HALF_VOID \
    if (l_new_str) { \
        printf("NEW_STR=%s\n", l_new_str); \
        strlcat_bot_safe (bot->txt_data_out, l_new_str, sizeof(bot->txt_data_out)-1); \
        free (l_new_str); \
      } \
    }\


/* half_arg: process only an argument, ie, ^e hi, instead of hi |^e */
#define MOD_PARSE_TOP_HALF_ARG \
  { \
  char * l_new_str=NULL; \
  char * l_str_ptr=NULL; \
  if (!strlen ((l_str_ptr=eat_whitespace (dl_module_arg_after_options)))) { return NULL; } \

#define MOD_PARSE_BOTTOM_HALF_ARG \
    if (l_new_str) { \
        printf("NEW_STR=%s\n", l_new_str); \
	strzero_bot_safe(bot->txt_data_out, sizeof(bot->txt_data_out)); \
        strlcat_bot_safe (bot->txt_data_out, l_new_str, sizeof(bot->txt_data_out)-1); \
        free (l_new_str); \
      } \
    }\





/* can process args or prior text */
#define MOD_PARSE_TOP_HALF_NODL \
  { \
  char * l_new_str=NULL; \
  char * l_str_ptr=NULL; \
  int l_should_strzero=0; \
  l_str_ptr = eat_whitespace (dl_module_arg_after_options); \
  if (!l_str_ptr || !strlen (l_str_ptr)) { \
      l_str_ptr = bot->txt_data_out; \
      puts("STRZERO"); \
      l_should_strzero = 1; \
    } \

#define MOD_PARSE_BOTTOM_HALF_NODL \
    if (l_new_str) { \
        printf("NEW_STR=%s\n", l_new_str); \
        if (l_should_strzero) { \
            strzero_bot_safe (bot->txt_data_out, sizeof (bot->txt_data_out)); \
            l_should_strzero = 0; } \
        strlcat_bot_safe(bot->txt_data_out, l_new_str, sizeof(bot->txt_data_out)-1); \
      } \
} \





/* 
 * OPTION PROCESSING BLOCK 
 */
#define MOD_OPTIONS_TOP_HALF \
 { \
  dl_module_arg_after_options = bot->dl_module_arg; \
  dl_options_ptr = &bot->dl_module_arg[1]; \
  if (bot->dl_module_arg[0] == '(') \
    { \
      int len_options_area = 0; \
      dl_module_arg_after_options = tokenize_find_closing_bracket (bot->dl_module_arg, '('); \
      if (!dl_module_arg_after_options) return NULL; \
      *dl_module_arg_after_options = '\0'; \
      dl_module_arg_after_options++; \
      len_options_area = dl_module_arg_after_options - bot->dl_module_arg; \

#define MOD_OPTIONS_BOTTOM_HALF \
  } \
  else { \
  dl_options_ptr = ""; \
  } \
 } \

#endif
