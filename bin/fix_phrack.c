/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dlist.h"



void hexd(unsigned char *s) {
int i;

printf("HEX: ");
for(i=0;i<strlen((char *)s);i++) {
printf("%.2x(%c) ", s[i], s[i]);
}
puts("");


return;
}

int
_strstrip_chars (char *str, char *chars)
{
/* strip supplied chars from a string, fill them with nulls */
  int i;

  if (!str || !chars)
    return 0;

  while (*str != '\0')
    {
      for (i = 0; i < strlen (chars); i++)
        {
          if (*str == chars[i])
            *str = '\0';
        }
      str++;
    }

  return strlen (str);
}


int
_strstrip_nl (char *s)
{
  while (*s)
    {
      if (*s == '\r' || *s == '\n')
        *s = '\0';
      s++;
    }

  return 0;
}

int
str_allcaps (char *s)
{
  int x;


if(!s) return 0;

if(strlen(s)==0) return 0;

  x = 0;
  while (*s)
    {
if(islower(*s))
        x++;
      s++;
    }

if(x > 2)
x = 0;
else
x = 1;

  return x;
}


char *
_eat_whitespace (char *s)
{
  if (!s)
    return NULL;


  while (*s)
    {
      if (*s != ' ' && *s != '\t')
        break;
      s++;
    }

  return s;
}


int
shrink_spaces (char *s1)
{
  char *s2;
  int n = 0;


  if (!s1)
    return 0;

  s2 = s1;
top:
  while (*s1)
    {
      *s2 = *s1;

/*
if(*s1==' ') {
while(*s1 == ' ') if(*s1==' '){ s1++; } else goto top;
}
*/
      if (*s1 == ' ' || *s1 == '\t')
        {
          s2++;
          while (*s1 == ' ' || *s1 == '\t')
            s1++;
        }
      else
  {
          s1++;
          s2++;
        }
    }

  *s2 = '\0';

  return 0;
}


int
shrink_all_spaces (char *s1)
{
  char *s2;
  int n = 0;



  if (!s1)
    return 0;

  s2 = s1;
top:
  while (*s1)
    {
      *s2 = *s1;
/*
if(*s1==' ') {
while(*s1 == ' ') if(*s1==' '){ s1++; } else goto top;
}
*/
      if (*s1 == ' '||*s1 == '\t')
        {
          while (*s1 == ' '||*s1 == '\t')
            s1++;
        }
      else
        {
          s1++;
          s2++;
        }
    }

  *s2 = '\0';

  return 0;
}



int
fill_brackets (char *s)
{
  char *p1, *p2;


if(!s) return 0;

  p1 = p2 = NULL;

  while (*s)
    {

      if (*s == '(' || *s == '{' || *s == '[')
        {
          p1 = s;
          s++;
          while (*s)
            {
              if (*s == ')' || *s == '}' || *s == ']')
                {
                  p2 = s;
break;
                }
              s++;
            }

          if (p1 && p2)
            {
              while ((p1 != p2) && *p1 != '\0')
                {
                  *p1 = ' ';
                  p1++;
                }
if(*p2==')' || *p2 == '}' || *p2 == ']')
*p2 =
' ';
              p1 = p2 = NULL;
            }
        }

      s++;
    }

  return 0;
}


int leading_space_count(char *s) {
int i=0;

while(*s) {
if(*s == ' ' || *s == '\t') i++;
else break;
s++;
}

return i;
}
 

dlist_t * dl;


typedef struct article {
char * title;
char * author;
char * issue;
char * date;
} article_t;

char g_issue[1024];
char g_date[1024];

char * article_clean_str(char *s) ;
article_t * article_create(char * author, char * title) ;
void unhtml_buf(char *);

int main(int argc, char * argv[] ) {
dlist_t * dptr, *t;
  FILE *fp;
  char buf[10024];
  char *str_ptr;

char * field_1, *field_2;
char * tok;

article_t * a;

  if (argc < 2)
    {
      exit (0);
}


fp = fopen(argv[1], "r");
  if (!fp)
    exit (0);

  while (1)
    {
      _memset (buf, 0, sizeof (buf));
      if (fgets (buf, sizeof (buf) - 10, fp) == NULL)
        break;
      _strstrip_nl (buf);

if(buf[0]== '#') {
/* issue # & date */
phrack_parse_issue(buf);
continue;
}

unhtml_buf(buf);

field_1 = _eat_whitespace(buf);

field_2 = strtok(field_1, " ");
if(!field_2) continue;

field_2 = strtok(NULL, " ");
if(!field_2) continue;

field_2 = _eat_whitespace(field_2);


a = article_create(field_2, field_1);

dlist_Dinsert_after(&dl, a);

}


dlist_fornext(dl, dptr) {

a = (article_t *)dlist_data(dptr);
if(!a) continue;

//printf("author=%s, title=%s, issue=%s, date=%s\n", a->author, a->title, a->issue, a->date);
printf("%s:::(%s,%s) %s\n", a->author, a->issue, a->date, a->title);

}


return 0;
}




/*
Current issue : #27 | Release date : 20/05/1989

*/

void phrack_parse_issue(char * buf) {
char * str_1=NULL;
char * str_2 = NULL;
char * str_ptr=NULL;

if(!buf) return;

_memset(g_issue, 0, sizeof(g_issue));
_memset(g_date, 0, sizeof(g_date));

//printf("buf=%s\n", buf);

str_1 = strcasestr(buf, "Current issue : ");
if(!str_1) return;

str_2 = strcasestr(buf, "Release date : ");
if(!str_2) return;

str_1 = str_1 + strlen("Current issue : ");
str_ptr = _strchr(str_1, ' ');
if(!str_ptr) return;

snprintf_buf(g_issue, sizeof(g_issue)-1, "%s", str_1);

_strstrip_chars(g_issue, " \t\xc2");

//hexd(g_issue);


str_2 = str_2 + strlen("Release Date : ");
str_ptr = _strchr(str_2, ' ');
if(!str_ptr) return;

snprintf_buf(g_date, sizeof(g_date)-1, "%s",str_2);

_strstrip_chars(g_date, " \t\xc2");


return;
}





article_t * article_create(char * author, char * title) {
article_t * a=NULL;

if(!author || !title) return NULL;

a = (article_t *)calloc(1,sizeof(article_t));
if(!a) return NULL;

a->author = article_clean_str(author);
a->title = article_clean_str(title);
a->issue = strdup(g_issue);
a->date = strdup(_strrchr(g_date, '/')+1);

return a;
}




char * article_clean_str(char *s) {
char * n;
int i;


if(!s)
return NULL;

for(i=0;i<strlen(s);i++) {
if(s[i]=='_')
s[i] = ' ';
}

n = strdup(s);
return n;
}


void unhtml_buf(char *s) {
int i,j;
char  * ptr;

if(!s) return;

ptr = s;
while(1) {
ptr = strcasestr(ptr, "&amp;");
if(!ptr) break;
memcpy(ptr, ",____", strlen(",____"));
}

ptr = s;
while(1) {
ptr = strcasestr(ptr, "&quot;");
if(!ptr) break;
memcpy(ptr, "&_____", strlen("&_____"));
}

shrink_spaces(s);

return;
}

