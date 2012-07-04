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


int
strstrip_nl (char *s)
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
eat_whitespace (char *s)
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

typedef struct convo {
char * nick;
char * text;
int spaces;
}
convo_t;

int main(int argc, char * argv[] ) {
dlist_t * dptr, *t;
  FILE *fp;
  char buf[10024];
  char *str_ptr;
convo_t * cptr, *cptr_cur;
int space_count, spaces_max;
struct stat st;



  if (argc < 2)
    {
      exit (0);
    }


if(stat(argv[1], &st) <0) 
{
exit(0);
}

if(!S_ISREG(st.st_mode)) {
exit(0);
} 

  fp = fopen (argv[1], "r");
  if (!fp)
    exit (0);

cptr_cur = NULL;
  while (1)
    {
      memset (buf, 0, sizeof (buf));
      if (fgets (buf, sizeof (buf) - 10, fp) == NULL)
        break;
      strstrip_nl (buf);


space_count = leading_space_count(buf);
//printf("%i buf=%s\n", space_count, buf);


str_ptr = buf + space_count;

if(str_allcaps(str_ptr)){ 
cptr = (convo_t *)calloc(1,sizeof(convo_t));
cptr->nick = strdup(eat_whitespace(str_ptr));
cptr->spaces = space_count;
dlist_Dinsert_after(&dl, cptr);
cptr_cur = cptr;
continue;
}


if(cptr_cur) {
float x, y;

x = cptr_cur->spaces;
y = x * .25;

//printf("yo %i %f %f\n", space_count, x, y);
if(space_count <= y) continue;

if(cptr_cur->text) {
 cptr_cur->text = realloc(cptr_cur->text, strlen(cptr_cur->text) + strlen(buf)+1);
} else {
cptr_cur->text = (char *)calloc(1,strlen(buf)+1);
}
strlcat_buf(cptr_cur->text, " ");
strlcat_buf(cptr_cur->text, eat_whitespace(buf)); 
}


}


spaces_max = 0;
dlist_fornext(dl, dptr) {
cptr = (convo_t *)dlist_data(dptr);
if(cptr->spaces > spaces_max) 
spaces_max = cptr->spaces;
}




dlist_fornext_safe(dl, dptr, t) {

cptr = (convo_t *)dlist_data(dptr);

if(cptr->spaces < 3 || cptr->text == NULL) { dlist_remove(&dl, dptr);

if(cptr->nick) free(cptr->nick);
if(cptr->text) free(cptr->text);
}
}




dlist_fornext_safe(dl, dptr, t){
float x , y;

cptr = (convo_t *)dlist_data(dptr);

x = spaces_max;
y = x * .25;

if(cptr->spaces <= y)  {

dlist_remove(&dl, dptr);

if(cptr->nick) free(cptr->nick);
if(cptr->text) free(cptr->text);
}
}




dlist_fornext(dl, dptr) {
cptr = (convo_t *)dlist_data(dptr);

fill_brackets(cptr->text);
shrink_spaces(cptr->text);

fill_brackets(cptr->nick);
shrink_all_spaces(cptr->nick);

//printf("%i. %s. [%s]\n", cptr->spaces, cptr->nick, cptr->text);
printf("%s:%s\n", cptr->nick, eat_whitespace(cptr->text));
}
 
return 0;
}

