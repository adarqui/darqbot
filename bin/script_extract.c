/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>



int vflag;


enum
{
  SCRIPT_NOCONVO = 0,
  SCRIPT_INCONVO = 1,
};

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
//      if (!isupper (*s))
if(islower(*s))
	x++;
      s++;
    }

if(x)
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
      if (*s != ' ')
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
      if (*s1 == ' ')
	{
	  s2++;
	  while (*s1 == ' ')
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
      if (*s1 == ' ')
	{
	  while (*s1 == ' ')
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


int
main (int argc, char *argv[])
{
  FILE *fp;
  int space_count;
  int script_type;
  char buf[10024], convo[10024], convo_nick[124], convo_nick_prev[124];
  char *str_ptr;


//vflag = 1;

  if (argc < 2)
    {
      exit (-1);
    }

  fp = fopen (argv[1], "r");
  if (!fp)
    exit (-1);

  while (1)
    {
      _memset (buf, 0, sizeof (buf));
      if (fgets (buf, sizeof (buf) - 10, fp) == NULL)
	break;
      _strstrip_nl (buf);

if(vflag)
printf(":::rawbuf=[%s]\n",buf);

      for (space_count = 0; space_count < strlen (buf); space_count++)
	{
	  if (buf[space_count] != ' ')
	    break;
	}



      if (space_count > 7)

	{

	  if (script_type == SCRIPT_NOCONVO)
	    {

if(vflag) {
puts(":::type=NOCONVO, setting=INCONVO");
}

	      script_type = SCRIPT_INCONVO;
	      str_ptr = buf;
	      if (str_ptr)
		{
		  if (str_allcaps (_eat_whitespace(str_ptr)))
		    {
if(vflag)
puts(":::ALL_CAPS");
		      _memset (convo_nick, 0, sizeof (convo_nick));
		      strlcpy_buf (convo_nick, str_ptr, sizeof (convo_nick) - 1);
		    }

		}
	    }
	  else
	    {
	      strlcat_buf (convo, buf, sizeof (convo) - 1);
	    }


	}
      else
	{

	  if (script_type == SCRIPT_INCONVO)
	    {

/* write buf out */
	      if (convo[0])
		{
		  fill_brackets (convo);
		  fill_brackets (convo_nick);
		  shrink_spaces (convo);
		  shrink_all_spaces (convo_nick);

//puts("---");
if(!strcasecmp(convo_nick_prev, convo_nick) && str_allcaps(_eat_whitespace(convo_nick))) {
printf(" %s", _eat_whitespace(convo));
}
else {
_memset(convo_nick_prev,0,sizeof(convo_nick_prev));
strlcpy_buf(convo_nick_prev, convo_nick, sizeof(convo_nick_prev)-1);
if(str_allcaps(_eat_whitespace(convo_nick))){
		  printf ("\n%s:%s", _eat_whitespace (convo_nick),
			  _eat_whitespace (convo));
}
		}

}

	    }

if(vflag)
puts(":::type=INCONVO, setting=NOCONVO);");
 
	  script_type = SCRIPT_NOCONVO;
	  _memset (&convo, 0, sizeof (convo));
	  continue;
	}


    }


  return 0;
}
