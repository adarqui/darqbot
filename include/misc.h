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
#ifndef MISC_H
#define MISC_H


#include "bot.h"

/* decls */

#define bz(a) memset((a),0,sizeof((a)))
#define bz2(a) memset(&(a), 0, sizeof((a)))

#define strlcpy_buf(a,b) strlcpy_safe((a), (b), (sizeof((a))/sizeof((a)[0])-1))
#define charcat_buf(a,b) charcat_safe((a), (b), (sizeof((a))/sizeof((a)[0])-1))
#define strlcat_buf(a,b) strlcat_safe((a), (b), (sizeof((a))/sizeof((a)[0])-1))
#define strlcatfmt_buf(a,b,...) strlcatfmt_safe((a), (sizeof((a))/sizeof((a)[0])-1), b, ##__VA_ARGS__)
#define snprintf_buf(a,b,...) snprintf_safe((a), (sizeof((a))/sizeof((a)[0])-1), b,##__VA_ARGS__)
#define vsnprintf_buf(a,b,c) vsnprintf_safe((a),(sizeof((a))/sizeof((a)[0])-1),(b),(c))

#define strzero_bot(a) strzero_bot_safe((a),(sizeof((a))))
#define memcpy_bot(a,b,c) memcpy_bot_safe((a), (sizeof((a))-1), (b), (c))
#define strlcat_bot(a,b) strlcat_bot_safe((a), (b), (sizeof((a))/sizeof((a)[0])-1))
#define strlcatfmt_bot(a,b,...) strlcatfmt_bot_safe((a), (sizeof((a))/sizeof((a)[0])-1), b, ##__VA_ARGS__)
#define charcat_bot(a,b) charcat_bot_safe((a), (b), (sizeof((a))/sizeof((a)[0])-1))
#define strprepend_bot(a,b) strprepend_bot_safe((a), (b), (sizeof((a))/sizeof((a)[0])-1))

/*
WORKS:
#define strlcpy_buf(a,b) strlcpy_safe((a), (b), sizeof((a))-1)
#define charcat_buf(a,b) charcat_safe((a), (b), sizeof((a))-1)
#define strlcat_buf(a,b) strlcat_safe((a), (b), sizeof((a))-1)
#define strlcatfmt_buf(a,b,...) strlcatfmt_safe((a), sizeof((a))-1, b, ##__VA_ARGS__)
#define snprintf_buf(a,b,...) snprintf_safe((a), sizeof((a))-1, b,##__VA_ARGS__)
#define vsnprintf_buf(a,b,c) vsnprintf_safe((a),sizeof((a))-1,(b),(c))

#define strzero_bot(a) strzero_bot_safe((a),sizeof((a)))
#define memcpy_bot(a,b,c) memcpy_bot_safe((a), sizeof((a))-1, (b), (c))
#define strlcat_bot(a,b) strlcat_bot_safe((a), (b), sizeof(a)-1)
#define strlcatfmt_bot(a,b,...) strlcatfmt_bot_safe((a), sizeof((a))-1, b, ##__VA_ARGS__)
#define charcat_bot(a,b) charcat_bot_safe((a), (b), sizeof(a)-1)
#define strprepend_bot(a,b) strprepend_bot_safe((a), (b), sizeof((a))-1)
*/

int true(void);
int false(void);

     size_t strlcpy_safe(char *, const char *, size_t);

     size_t strlcat_safe(char *, const char *, size_t);

char * sNULL(char *);

int vsnprintf_safe(char * , size_t , const char * , va_list ) ;

char *str_unite (const char *, ...);

char * str_unite_static(const char *, ...);

char *str_unite_foralloc (const char *, ...);

int strstrip_nl (char *);

char *eat_whitespace (char *);

int strstrip_chars (char *, char *);

char *strtokpos (char *, char);
char * (fn_strtokpos)(char *, int );

int strncasecmp_len (const char *, const char *);

int strlcatfmt_safe(char *, int , char *, ...);

char * strstr_len(char *, char *, int);

int strlcat_bot_safe(char *, char *, int);

int strlcatfmt_bot_safe(char *, int , char *, ...);

int strprepend_bot_safe(char *, char *, int);

char * strdup_char(char );

char * strdup_int(int);

char * strdup_len(char *, int);

int charcat_safe(char *, char, int);

int charcat_bot_safe(char *, char , int);

char* strtok_nonalpha(char *);

char * strtokdup_str(char *, char *);

char * xstrcasestr(char *, char *);

char * xstrcasestr_len(char *, char *, int);

char * xstrcasestr2(char *, char *);

char * xstrcasestr_unique(char *, char *);


/* ^: sep routines */
char * strzero_bot_safe(char * ,  size_t);

char * str_find_sep(char *);

int str_clean_sep(char *);

int str_clean_sep_shrink(char *, int);

int str_transform_pipes(char *);

int str_untransform_pipes(char *);

 char * strtokbionic(char *, const char *, int *);

char * strtokbionic_nonalpha (char *, int * );

int str_clean_nl(char *, int );

int snprintf_safe(char *, size_t, const char *, ...);

int capsup(char *);

int capsdown(char *);

int str_isclean(char *, int (*fn)(int));

int str_apply(char * , int (*fn)(int)) ;

int str_apply_is(char *, int (*fn)(int)) ;

int iskeychars(int);

int ctoi(int);

int itoc(int);

char * itocstr(int);


void * memcopy(char *, char *, size_t, size_t);

int str_shrink_quotes(char *);


int isbinary(int);

int memcpy_bot_safe(char *, int, char *, int);

char * strrchr_len(const char *, int, int);

int str_last_char_replace(char *, int, int);

int str_has_newline(char *);

int str_shrink_chars(char *, char *);

int
str_shrink_spaces_to_one (char *);

int str_shrink_chars_and_replace(char *, char *, char);

int str_sum(char *);

int MAX_FMT(char * , ...) ;

char * strchr_str(const char *, const char *, int *);

char * str_replace(char *, char *, char *);

int istrig(int) ;

int isprog(int) ;

char * ret_trig(char * ) ;

int
qsort_compare_forward (const void *, const void *);
int qsort_compare_backward(const void *, const void *);
int qsort_compare_randward(const void *, const void *);

int
sort_compare_forward (const char *, const char *);
int sort_compare_backward(const char *, const char *);
int sort_compare_randward(const char *, const char *);

int dl_str_unite(dlist_t **, const char *, ...);

int dl_str_dup(dlist_t **, const char *);

void dl_str_debug(dlist_t ** ) ;

void dl_str_print(dlist_t ** ) ;

void dl_str_destroy(dlist_t **);

char * str_create_spaces(int ) ;

enum str_match_flags {
STR_MATCH_STRCASECMP=0x01,
STR_MATCH_STRNCASECMP=0x02,
STR_MATCH_STRCMP=0x04,
STR_MATCH_STRNCMP=0x08,
STR_MATCH_STRSTR=0x10,
STR_MATCH_STRCASESTR=0x20,
};

char * str_match(const char *, int, int, ...) ;


enum strstrip_char_flags {
STRSTRIP_CHARS_KEEP=0x01,
STRSTRIP_CHARS_DONTKEEP=0x02,
STRSTRIP_CHARS_SHRINK=0x04,
};

int strstrip_chars_fmtv2(char *, int, ...);
int strstrip_chars_fmt(char *, int, ...);

enum seg_result_types {
SEG_UNKNOWN=0,
SEG_DATA,
SEG_TEXT,
};

int seg(void *);



typedef struct memdup_struct {
char * data;
int len;
} memdup_t;

memdup_t * memdup(char *, int) ;

void memdup_free(void *);




#endif


