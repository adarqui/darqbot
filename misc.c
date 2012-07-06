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
#include "bot.h"
#include "misc.h"



size_t
strlcpy_safe (char *dst, const char *src, size_t sz)
{
  if (!dst || !src || sz <= 0)
    return 0;
  return strlcpy (dst, src, sz);
}

size_t
strlcat_safe (char *dst, const char *src, size_t sz)
{
  if (!dst || !src || sz <= 0)
    return 0;
  return strlcat (dst, src, sz);
}



char *
sNULL (char *s)
{
  if (!s)
    return NULL;
  if (s[0] == '\0')
    return NULL;
  return s;
}



char *
str_unite_static (const char *msg, ...)
{
/* str_unite: unite multiple strings/ints/whatever, return a strdup'd string */
  va_list ap;
  int n;
  static char buf[MAX_BUF_SZ + 1];

  if (!sNULL ((char *) msg))
    return NULL;

  bz (buf);

  va_start (ap, msg);
  n = vsnprintf_buf (buf, msg, ap);
  va_end (ap);

  if (n > 0)
    {
      buf[n] = '\0';
    }

  return buf;
}



char *
str_unite (const char *msg, ...)
{
/* str_unite: unite multiple strings/ints/whatever, return a strdup'd string */
  va_list ap;
  int n;
  char buf[MAX_BUF_SZ + 1];

  if (!sNULL ((char *) msg))
    return NULL;

  va_start (ap, msg);
  n = vsnprintf_buf (buf, msg, ap);
  va_end (ap);

  if (n > 0)
    {
      buf[n] = '\0';
    }

  return strdup (buf);
}



char *
str_unite_foralloc (const char *msg, ...)
{
/* str_unite: unite multiple strings/ints/whatever, return a strdup'd string */
  va_list ap;
  int n;
  char buf[MAX_BUF_SZ + 1];

  if (!sNULL ((char *) msg))
    return NULL;

  va_start (ap, msg);
  n = vsnprintf_buf (buf, msg, ap);
  va_end (ap);

  if (n > 0)
    {
      buf[n] = '\0';
    }

  return strdup (buf);
}



int
dl_str_unite (dlist_t ** dl, const char *msg, ...)
{
  char *str = NULL;
  va_list ap;
  int n;
  char buf[MAX_BUF_SZ + 1];

  if (!dl || !sNULL ((char *) msg))
    return -1;

  va_start (ap, msg);
  n = vsnprintf_buf (buf, msg, ap);
  if (n > sizeof (buf) - 1)
    n = sizeof (buf) - 1;
  else
    n = n;
  va_end (ap);

  buf[n] = '\0';

  str = strdup (buf);

  dlist_Dinsert_after (dl, str);

  return 1;
}


int
dl_str_dup (dlist_t ** dl, const char *string)
{
  char *str = NULL;

  if (!dl || !sNULL ((char *) string))
    return -1;

  str = strdup (string);
  if (!sNULL (str))
    return -1;

  return 1;
}

void
dl_str_debug (dlist_t ** dl)
{
  char *str = NULL;

  if (!dl)
    return;

/* SUPERFIX
*/
  str = dlist_to_str (*dl);
  if (!sNULL (str))
    return;

  debug (NULL, "dl_str_debug: [%s]\n", str);

  free (str);
  dl_str_destroy (dl);

  return;
}


void
dl_str_print (dlist_t ** dl)
{
  char *str = NULL;

  if (!dl)
    return;

// SUPERFIX 
  str = dlist_to_str (*dl);
  if (!sNULL (str))
    return;

  debug (NULL, "dl_str_print:\n[%s]\n", str);

  free (str);
  return;
}



void
dl_str_destroy (dlist_t ** dl)
{


  if (!dl)
    return;

  dlist_fini (dl, free);

  return;
}



int
strstrip_nl (char *str)
{
/* strip \n and \r from a string */
  int n;
  if (!sNULL (str))
    return 0;

  n = 0;
  while (*str)
    {
      if (*str == '\n' || *str == '\r')
	{
	  *str = '\0';
	  break;
	}
      str++;
      n++;
    }

  return n;
}




char *
eat_whitespace (char *str)
{
/* disregard whitespace, return ptr to start of string */
  if (!sNULL (str))
    return NULL;
  while (*str && (*str == ' ' || *str == '\t'))
    str++;

  return str;
}


int
strstrip_chars (char *str, char *chars)
{
/* strip supplied chars from a string, fill them with nulls */
  int i;

  if (!sNULL (str) || !sNULL (chars))
    return 0;

  while (*str != '\0')
    {
      for (i = 0; i < fn_strlen (chars); i++)
	{
	  if (*str == chars[i])
	    *str = '\0';
	}
      str++;
    }

  return fn_strlen (str);
}

char *
strtokpos (char *str, char c)
{
/* return a pointer to a string starting at char c, should just use strchr */

  if (!sNULL (str))
    return NULL;

  while (*str != '\0')
    {
      if (*str == c)
	return str;
      str++;
    }

  return NULL;
}


int
strncasecmp_len (const char *c1, const char *c2)
{
/* match string, case insensitive, based on c2 */

  if (!sNULL ((char *) c1) || !sNULL ((char *) c2))
    return -1;

  return strncasecmp (c1, c2, fn_strlen (c2));
}


char *
str_match (const char *s1, int flags, int len, ...)
{
  char *match = NULL;
  va_list ap;
  char *str;

  if (!sNULL ((char *) s1))
    return NULL;

  va_start (ap, len);
  for (;;)
    {
      str = va_arg (ap, char *);
      if (!sNULL (str))
	break;

      if (flags & STR_MATCH_STRCASECMP)
	{
	  if (!strcasecmp (s1, str))
	    {
	      match = str;
	      break;
	    }
	}
      else if (flags & STR_MATCH_STRCMP)
	{
	  if (!strcmp (s1, str))
	    {
	      match = str;
	      break;
	    }
	}
      else if (flags & STR_MATCH_STRNCASECMP)
	{
	  if (!strncasecmp (s1, str, len == 0 ? strlen (str) : len))
	    {
	      match = str;
	      break;
	    }
	}
      else if (flags & STR_MATCH_STRNCMP)
	{
	  if (!strncmp (s1, str, len == 0 ? strlen (str) : len))
	    {
	      match = str;
	      break;
	    }
	}
      else if (flags & STR_MATCH_STRSTR)
	{
	  match = strstr (s1, str);
	  if (match)
	    {
	      break;
	    }
	}
      else if (flags & STR_MATCH_STRCASESTR)
	{
	  match = strcasestr (s1, str);
	  if (match)
	    {
	      break;
	    }
	}
    }

  va_end (ap);

  return match;
}



char *
strstr_len (char *c1, char *c2, int len)
{
/* tries to find c2 inside c1, within len bytes of data */
  char *sub, *ret;

  sub = strdup (c1);
  sub[len] = '\0';

  ret = strstr (sub, c2);
  free (sub);
  if (ret)
    {
      return strstr (c1, c2);
    }

  return NULL;
}



int
strlcat_bot_safe (char *buf, char *str, int len)
{
/* concatenates str to buf, but doesnt go above len bytes of data */
  int n;

  char *sep_ptr;
  int sep_len;

  if (!buf || !sNULL (str) || len <= 0)
    return 0;

  sep_len = 0;
  sep_ptr = str_find_sep (buf);
  if (sep_ptr)
    {
      sep_len = sep_ptr - buf;
      buf = sep_ptr;
      len = len - sep_len;
    }
  else
    sep_ptr = buf;

/*
  len_buf = fn_strlen (buf);
  len_str = fn_strlen (str);

  new_len = len - len_buf;

  strlcat (buf, str, new_len);

  n = fn_strlen (buf);

printf("zzz: new_len=%i, n=%i, len=%i\n", new_len, n, len);
*/
  strlcat (buf, str, len);

  return n;
}





int
memcpy_bot_safe (char *buf, int len, char *str, int str_len)
{
/* concatenates str to buf, but doesnt go above len bytes of data */
  int n, len_str, len_buf, new_len;

  char *sep_ptr;
  int sep_len;

  if (!buf || !sNULL (str) || len <= 0 || str_len <= 0)
    return 0;

  sep_len = 0;
  sep_ptr = str_find_sep (buf);
  if (sep_ptr)
    {
      sep_len = sep_ptr - buf;
      buf = sep_ptr;
      len = len - sep_len;
    }
  else
    sep_ptr = buf;

  len_buf = fn_strlen (buf);
  len_str = str_len;

  new_len = len - len_buf;

  if (new_len > str_len)
    memcpy (buf, str, str_len);
  else
    memcpy (buf, str, new_len);

  n = str_len;
  return n;
}





int
strlcatfmt_bot_safe (char *buf, int len, char *fmt, ...)
{
  va_list ap;
  int n;
  char tmp_buf[MAX_BUF_SZ];

  if (!sNULL (fmt))
    return 0;

  va_start (ap, fmt);
  n = vsnprintf_buf (tmp_buf, fmt, ap);
  if (n > sizeof (buf) - 1)
    n = sizeof (buf) - 1;
  else
    n = n;
  va_end (ap);

  n = strlcat_bot_safe (buf, tmp_buf, len);
  return n;
}



int
strlcatfmt_safe (char *buf, int len, char *fmt, ...)
{
/*
int n;

  va_list ap;

  va_start (ap, fmt);
  n = vsnprintf_safe(buf, len, fmt, ap);
  va_end(ap);
*/
  va_list ap;
  int n;
  char tmp_buf[MAX_BUF_SZ];


  if (!sNULL (fmt))
    return 0;

  va_start (ap, fmt);
  n = vsnprintf_buf (tmp_buf, fmt, ap);
  if (n > sizeof (buf) - 1)
    n = sizeof (buf) - 1;
  else
    n = n;
  va_end (ap);

  n = strlcat_safe (buf, tmp_buf, len);
  return n;
}



int
charcat_safe (char *buf, char c, int len)
{
/* same as strlcat_buf_safe, appends a one byte char */
  int n, len_buf, new_len;

  if (!buf || len <= 0)
    return 0;

  len_buf = fn_strlen (buf);

  new_len = len - (len_buf + 1);
  if (new_len <= 0)
    return 0;

  buf[fn_strlen (buf) + 1] = '\0';
  buf[fn_strlen (buf)] = c;

  n = fn_strlen (buf);
  return n;
}



int
charcat_bot_safe (char *buf, char c, int len)
{
/* same as strlcat_buf_safe, appends a one byte char */
  int n, len_buf, new_len;


  char *sep_ptr;
  int sep_len;

  if (!buf || len <= 0)
    return 0;

  sep_len = 0;
  sep_ptr = str_find_sep (buf);
  if (sep_ptr)
    {
      sep_len = sep_ptr - buf;
      buf = sep_ptr;
      len = len - sep_len;
    }
  else
    sep_ptr = buf;

  len_buf = fn_strlen (buf);

  new_len = len - (len_buf + 1);
  if (new_len <= 0)
    return 0;

  buf[fn_strlen (buf) + 1] = '\0';
  buf[fn_strlen (buf)] = c;

  n = fn_strlen (buf);
  return n;
}



int
strprepend_bot_safe (char *buf, char *str, int len)
{
/* re-adjusts buf, prepends str to buf, shifts buf over, no more than len bytes */
  char *tmp_str;

  char *sep_ptr;
  int sep_len;

  if (!buf || !sNULL (str) || len <= 0)
    return 0;

  sep_len = 0;
  sep_ptr = str_find_sep (buf);
  if (sep_ptr)
    {
      sep_len = sep_ptr - buf;
      buf = sep_ptr;
      len = len - sep_len;
    }
  else
    sep_ptr = buf;

/*
  len_buf = fn_strlen (buf);
  len_str = fn_strlen (str);

  new_len = len - (len_buf + len_str);
  if (new_len <= 0)
    return 0;
*/

  tmp_str = strdup (buf);

  strlcpy_safe (buf, str, len);
  strlcat_safe (buf, tmp_str, len);

  free (tmp_str);

  return fn_strlen (buf);
}


char *
strdup_char (char c)
{
/* creates a string out of a 1 byte arg */
  char buf[2];

  if (!c)
    return NULL;

  buf[0] = c;
  buf[1] = '\0';
  return strdup (buf);
}

char *
strdup_int (int x)
{
  char buf[32];
  bz (buf);
  snprintf_buf (buf, "%i", x);
  return strdup (buf);
}


char *
strtok_nonalpha (char *s)
{
/* strtok's based on punct chars as field separators */
  return strtok (s, " ,./';!@#$%^&*()_+='\\1234567890-<>?\"");

}



char *
strtokbionic_nonalpha (char *s, int *c)
{
/* same as strtok_nonalpha, but uses strtokbionic (returns null byte location as arg (int *c) */
  return strtokbionic (s, " ,./';!@#$%^&*()_+='\\1234567890-<>?\"", c);

}



char *
strtokdup_str (char *str, char *tok)
{
/* returns a token that needs to be free'd */
  char *s, *base;

  s = NULL;
  base = str;
  if (!sNULL (str) || !sNULL (tok))
    {
      return NULL;
    }

  while (*str)
    {
      if (!strncmp (str, tok, fn_strlen (tok)))
	{
	  s = calloc (1, str - base);
	  if (!s)
	    continue;
	  memcpy (s, base, str - base);
	  break;
	}
      str++;
    }

  return s;
}






char *
xstrcasestr (char *haystack, char *needle)
{
/* strcasestr */
  char *p, *startn = 0, *np = 0;

  if (!sNULL (haystack) || !sNULL (needle))
    return NULL;

  for (p = haystack; *p; p++)
    {
      if (np)
	{
	  if (toupper (*p) == toupper (*np))
	    {
	      if (!*++np)
		{
		  return startn;
		}
	    }
	  else
	    np = 0;
	}
      else if (toupper (*p) == toupper (*needle))
	{
	  np = needle + 1;
	  startn = p;
	}
    }


  return 0;
}








char *
xstrcasestr_len (char *haystack, char *needle, int len)
{
/* strcasestr */
  char *p, *startn = 0, *np = 0;

  char *p_end;

  if (!sNULL (haystack) || !sNULL (needle))
    return NULL;

  p = haystack;
  p_end = p + len;

  for (p = haystack; *p && p <= p_end; p++)
    {
      if (np)
	{
	  if (toupper (*p) == toupper (*np))
	    {
	      if (!*++np)
		{
		  return startn;
		}
	    }
	  else
	    np = 0;
	}
      else if (toupper (*p) == toupper (*needle))
	{
	  np = needle + 1;
	  startn = p;
	}
    }


  return 0;
}









char *
xstrcasestr2 (char *haystack, char *needle)
{
/* i forget why i needed this, oh, has something to do with whitespace */
  char *p, *startn = 0, *np = 0;
// trailing whitespace 
  char *z, *y;
  int c;
  if (!sNULL (haystack) || !sNULL (needle))
    return NULL;
  y = z = (needle + (fn_strlen (needle) - 1));
  while (*z == ' ')
    {
      z--;
    }
  if (y != z)
    {
      z++;
      *z = '\0';
    }
  c = *z;

  for (p = haystack; *p; p++)
    {
      if (np)
	{
	  if (toupper (*p) == toupper (*np))
	    {
	      if (!*++np)
		{
		  *z = c;
		  return startn;
		}
	    }
	  else
	    np = 0;
	}
      else if (toupper (*p) == toupper (*needle))
	{
	  np = needle + 1;
	  startn = p;
	}
    }

  *z = c;

  return 0;
}






char *
xstrcasestr_unique (char *haystack, char *needle)
{
/* case insensitive search, needle as a unique word */
  char buf[132], buf_haystack[132], *b;

  char *z, *z_end;

  int x;


  if (!sNULL (haystack) || !sNULL (needle))
    return NULL;

  bz (buf);
  bz (buf_haystack);


  b = needle;
  while (*b == ' ')
    b++;
  snprintf_buf (buf, "%s", b);
  b = buf;
  while (*b)
    {
      if (!isalnum (*b))
	{
	  *b = '\0';
	  break;
	}
      b++;
    }


  b = buf;

  snprintf_buf (buf_haystack, "%s", haystack);


  z = buf_haystack;
  z_end = buf_haystack + fn_strlen (buf_haystack);
  while (1)
    {
      if (z >= z_end)
	break;

      for (x = 0; x < fn_strlen (z); x++)
	{
	  if (!isalnum (z[x]))
	    {
	      z[x] = '\0';
	      break;
	    }
	}


      if (!strcasecmp (z, b))
	{
	  return needle;
	}

      z = z + fn_strlen (z) + 1;
      while (*z)
	{
	  if (isalnum (*z))
	    break;
	  z++;
	}
    }


  return 0;
}






char *
strzero_bot_safe (char *s, size_t n)
{
/* zero's out a string, but only the trailing portion of it, if a ^: (0x01) exists */
  char *sep_ptr;


  if (!s || n <= 0)
    return s;


  sep_ptr = str_find_sep (s);
  if (sep_ptr)
    {
      n = n - (sep_ptr - s);
      s = sep_ptr;
    }
  memset (s, 0, n);
  return s;
}




char *
str_find_sep (char *str)
{
/* find the last separator in a string */
  int len;
  int i;

  if (!sNULL (str))
    return str;

  len = fn_strlen (str);
  for (i = len; i >= 0; i--)
    {
      if (str[i] == 0x01)
	{
	  return str + i + 1;
	}

    }

  return NULL;
}





int
str_clean_sep (char *str)
{
/* replace all separators with a ' ' char */
  int len;
  int i;

  if (!sNULL (str))
    return 0;

  len = fn_strlen (str);
  for (i = 0; i < len; i++)
    {
      if (str[i] == 0x01)
	str[i] = ' ';
    }

  return 0;
}



int
str_clean_sep_shrink (char *str, int len)
{
/* shrink a string, removing \xff\xff's (separators) etc */
  int i, j;

  debug (NULL, "str_clean_sep_shrink: Entered\n");

  if (!sNULL (str) || len <= 0)
    return 0;

  i = j = 0;
  for (; i <= len;)
    {

      if (str[i] != 0x01)
	{
	  str[j] = str[i];
	  j++;
	}

      i++;
    }

  return 0;
}











int
str_transform_pipes (char *str)
{
/* look for \|, turn it into \xFF\xFF */
  int i, j;

  debug (NULL, "str_transform_pipes: Entered\n");

  if (!sNULL (str))
    return 0;

  j = 0;
  for (i = 0; i < fn_strlen (str); i++)
    {

      if (str[i] == '\\' && str[i + 1] == '|')
	{
	  str[i] = '\xFF';
	  str[i + 1] = '\xFF';
	  j++;
	  i++;
	}

    }

  return j;
}



int
str_untransform_pipes (char *str)
{
  int i, j;
  if (!sNULL (str))
    return 0;

  debug (NULL, "str_untransform_pipes: Entered\n");

  j = 0;
  for (i = 0; i < fn_strlen (str);)
    {

      printf ("%.2x ", str[i]);

      if (str[i] == '\xFF' && str[i + 1] == '\xFF')
	{
	  str[j] = '|';
	  i++;
	}
      else
	str[j] = str[i];


      i++;
      j++;
    }

  str[j] = '\0';

  return j;
}














char *
strtokbionic (char *s1, const char *delimit, int *retchar)
{
/* returns char that will be nulled out, in retchar */
  static char *lastToken;	/* UNSAFE SHARED STATE! */
  char *tmp;

  /* Skip leading delimiters if new string. */
  if (s1 == NULL)
    {
      s1 = lastToken;
      if (s1 == NULL)		/* End of story? */
	return NULL;
    }
  else
    {
      s1 += strspn (s1, delimit);
    }

  /* Find end of segment */
  tmp = strpbrk (s1, delimit);
  if (tmp)
    {
      /* Found another delimiter, split string and save state. */
      if (retchar)
	*retchar = *tmp;
      *tmp = '\0';
      lastToken = tmp + 1;
    }
  else
    {
      /* Last segment, remember that. */
      lastToken = NULL;
    }

  return s1;
}









char *
strtok_apple (s, delim)
     register char *s;
     register const char *delim;
{
  register char *spanp;
  register int c, sc;
  char *tok;
  static char *last;


  if (s == NULL && (s = last) == NULL)
    return (NULL);

  /*
   * Skip (span) leading delimiters (s += strspn(s, delim), sort of).
   */
cont:
  c = *s++;
  for (spanp = (char *) delim; (sc = *spanp++) != 0;)
    {
      if (c == sc)
	goto cont;
    }

  if (c == 0)
    {				/* no non-delimiter characters */
      last = NULL;
      return (NULL);
    }
  tok = s - 1;

  /*
   * Scan token (scan for delimiters: s += strcspn(s, delim), sort of).
   * Note that delim must have one NUL; we stop if we see that, too.
   */
  for (;;)
    {
      c = *s++;
      spanp = (char *) delim;
      do
	{
	  if ((sc = *spanp++) == c)
	    {
	      if (c == 0)
		s = NULL;
	      else
		s[-1] = 0;
	      last = s;
	      return (tok);
	    }
	}
      while (sc != 0);
    }
  /* NOTREACHED */
}






int
str_clean_nl (char *buf, int len)
{
/* clean \r\n's, could have just used strstrip_chars, i forget what i code sometimes */
  int i;

  if (!buf)
    return 0;

  for (i = 0; i < len; i++)
    {
      if (buf[i] == '\r' || buf[i] == '\n')
	buf[i] = ' ';
    }

  return i;
}




int
snprintf_safe (char *buf, size_t len, const char *fmt, ...)
{
/* fills buf, but doesnt go above len bytes of data, respecting seperators */

  va_list ap;
  char str_buf[MAX_BUF_SZ + 1];
  int n;

  if (!sNULL ((char *) fmt))
    return 0;

  va_start (ap, fmt);
  n = vsnprintf_buf (str_buf, fmt, ap);
  if (n > sizeof (buf) - 1)
    n = sizeof (buf) - 1;
  else
    n = n;
  va_end (ap);

  n = strlcat_safe (buf, str_buf, len);

  return n;
}









int
capsup (char *str)
{
  int n;

  if (!sNULL (str))
    return 0;

  n = 0;
  while (*str)
    {
      *str = toupper (*str);
      str++;
      n++;
    }

  return n;
}




int
capdown (char *str)
{
  int n;

  if (!sNULL (str))
    return 0;

  n = 0;
  while (*str)
    {
      *str = tolower (*str);
      str++;
      n++;
    }

  return n;
}



int
str_isclean (char *str, int (*fn) (int))
{
/* use such as, str_isclean(buf, isalpha); .. returns 1 if clean isalpha chars, 0 if not.. replace isalpha with isXXX etc */

  if (!sNULL (str) || !fn)
    return 0;

  while (*str)
    {
      if (!fn (*str))
	return 0;
      str++;
    }

  return 1;
}


int
istrig (int c)
{
  if (c == '_' || isalnum (c))
    return 1;
  return 0;
}

int
isprog (int c)
{
  if (c == '.' || c == '-' || c == '_' || isalnum (c))
    return 1;
  return 0;
}



int
str_apply (char *str, int (*fn) (int))
{
  if (!sNULL (str) || !fn)
    return 0;

  while (*str)
    {
      *str = fn (*str);
      str++;
    }

  return 1;
}


int
str_apply_is (char *str, int (*fn) (int))
{
  int i = 0;
  if (!sNULL (str) || !fn)
    return 0;

  while (*str)
    {
      if (!fn (*str))
	{
	  *str = '\0';
	  break;
	}
      i++;
      str++;
    }

  return i;
}



int
iskeychars (int c)
{
  char *list = "./-_=+!@#%&*,<>";
  int i;

  if (isalnum (c))
    return 1;
  for (i = 0; i < fn_strlen (list); i++)
    {
      if (c == list[i])
	return 1;
    }

  return 0;
}








int
ctoi (int c)
{
  char buf[2];
  buf[0] = c;
  buf[1] = '\0';
  return atoi (buf);
}

int
itoc (int c)
{
  char buf[10];
  bz (buf);
  snprintf_buf (buf, "%i", c);
  return buf[0];
}


char *
itocstr (int c)
{
  static char buf[10];
  bz (buf);
  snprintf_buf (buf, "%i", c);
  return buf;
}


void *
memcopy (char *s1, char *s2, size_t s1_len, size_t s2_len)
{

  if (s2_len > s1_len)
    s2_len = s1_len;

  return memcpy (s1, s2, s2_len);
}





int
str_shrink_quotes (char *s)
{
  char *s2;
  int n;

  if (!sNULL (s))
    return 0;

  n = 0;
  s2 = s;
  while (*s)
    {

      if (*s != '\"')
	{
	  *s2 = *s;
	  s2++;
	}
      s++;
    }

  *s2 = '\0';


  return n;
}



int
isbinary (int c)
{
  if (c == '0' || c == '1')
    return 1;
  return 0;
}



char *
strdup_len (char *s, int l)
{
/* man strndup .. */
  char *ns;

  if (!sNULL (s))
    return NULL;

  ns = (char *) calloc (1, l + 1);
  if (!ns)
    return NULL;

  if (l > 1)
    {
      l = l - 1;
    }

  while (l >= 0)
    {
      ns[l] = s[l];
      l--;
    }

  return ns;
}






/*
 * C standard string function: find rightmost instance of a character
 * in a string.
 */
char *
strrchr_len (const char *s, int ch, int len)
{
  /* start one past the last character INCLUDING NULL TERMINATOR */

  if (!sNULL ((char *) s) || !ch || len <= 0)
    return NULL;

  /* go from right to left; stop at 0 */
  while (len > 0)
    {

      /* decrement first */
      len--;

      /* now check the character we're over */
      if (s[len] == ch)
	{
	  return (char *) (s + len);
	}
    }

  /* didn't find it */
  return NULL;
}






int
str_last_char_replace (char *buf, int len, int c)
{
  int cr = 0;

  if (!buf || len <= 0)
    return 0;

  cr = buf[len];
  buf[len] = c;

  return cr;
}




int
str_has_newline (char *s)
{
  int i;


  if (!sNULL (s))
    return 0;

  for (i = strlen (s); i >= 0; i--)
    {
      if (s[i] == '\n')
	return 1;
    }

  return 0;
}





int
str_shrink_chars (char *s, char *chars)
{
  char *s2;
  int n;

  if (!sNULL (s) || !sNULL (chars))
    return 0;

  n = strlen (s);
  s2 = s;
  while (*s && n >= 0)
    {

      if (memcmp (s, chars, strlen (chars)))
	{
	  *s2 = *s;
	  s2++;
	  s++;
	  n -= 1;
	}
      else
	{
	  s = s + strlen (chars);
	  n -= strlen (chars);
	}
    }

  *s2 = '\0';

  return n;
}






int
str_shrink_chars_and_replace (char *s, char *chars, char replace)
{
  char *s2;
  int n, chars_len;

  if (!sNULL (s) || !sNULL (chars))
    return 0;

  chars_len = strlen (chars);

  n = strlen (s);
  s2 = s;
  while (*s && n >= 0)
    {

      if (memcmp (s, chars, chars_len))
	{
	  *s2 = *s;
	  s2++;
	  s++;
	  n -= 1;
	}
      else
	{
	  memset (s2, replace, chars_len);
	  s2 += (chars_len + 1);
	  s = s + (chars_len + 1);
	  n -= (chars_len + 1);
	}
    }

  *s2 = '\0';

  return n;
}






int
str_shrink_spaces_to_one (char *s1)
{
  char *s2;

  if (!sNULL (s1))
    return 0;

  s2 = s1;
  while (*s1)
    {
      *s2 = *s1;

      if (*s1 == ' ' || *s1 == '\t')
	{
	  s2++;
	  while (*s1 == ' ' || *s1 == '\t')
	    {
	      s1++;
	    }
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
str_sum (char *s)
{
  int tot = 0;
  if (!sNULL (s))
    return 0;
  while (*s)
    {
      tot += (int) *s;
      s++;
    }

  return tot;
}




int
MAX_FMT (char *fmt, ...)
{
  va_list ap;
  int val, max_val, i, j;

  if (!sNULL (fmt))
    return 0;

  max_val = 0;
  j = abs (atoi (fmt));

  va_start (ap, fmt);
  for (i = 0; i < j; i++)
    {
      val = va_arg (ap, int);
      if (val > max_val)
	max_val = val;
    }
  va_end (ap);

  return max_val;
}







int
strstrip_chars_fmt (char *str, int opt, ...)
{
/* strip out, or leave in, chars based on isalpha/blah type of funcs */
  va_list ap;
  int i, j, ret = 0;
  void *ptr = NULL;
  int seg_ret = 0;
  char *ptr_str = NULL;
  int (*ptr_fn) (int) = NULL;
  char bytebuf[strlen (str) + 1];
  int str_orig_len;

  if (!sNULL (str) || opt < 0)
    return -1;

  bz (bytebuf);

  str_orig_len = strlen (str);

  va_start (ap, opt);
  while (1)
    {
      ret = 0;
      ptr = va_arg (ap, void *);
      if (!ptr)
	break;
      seg_ret = seg (ptr);
      for (i = 0; i < strlen (str); i++)
	{
	  ret = 0;
	  if ((int) ptr <= 255)
	    {
/* char */
	      if (str[i] == (int) ptr)
		{
		  ret = 1;
		}
	    }
	  else if (seg_ret == SEG_DATA)
	    {
	      ptr_str = (char *) ptr;
	      for (j = 0; j < strlen (ptr_str); j++)
		{
		  if (ptr_str[j] == str[i])
		    {
		      ret = 1;
		      break;
		    }
		}
	    }
	  else if (seg_ret == SEG_TEXT)
	    {
	      ptr_fn = ptr;
	      ret = ptr_fn (str[i]);
	    }


	  if (ret)
	    {
	      bytebuf[i] = 1;
	    }


	}
    }

  j = 0;
  str_orig_len = strlen (str);
  for (i = 0; i < str_orig_len; i++)
    {
      if (opt & STRSTRIP_CHARS_KEEP)
	{
	  if (bytebuf[i] == 0)
	    {
	      str[i] = '\0';
	    }
	}
      else if (opt & STRSTRIP_CHARS_DONTKEEP)
	{
	  if (bytebuf[i] == 1)
	    {
	      str[i] = '\0';
	    }
	}

      if (opt & STRSTRIP_CHARS_SHRINK)
	{
	  if (str[i] != '\0')
	    {
	      str[j] = str[i];
	      j++;
	    }
	}

    }

  if (opt & STRSTRIP_CHARS_SHRINK)
    {
      str[j] = '\0';
    }

  va_end (ap);

  return 0;
}







int
seg (void *ptr)
{
  Dl_info dli;
  int seg_val = SEG_UNKNOWN;
  int n;

  if (!ptr)
    return SEG_UNKNOWN;

  n = dladdr (ptr, &dli);

  if (!dli.dli_sname || !dli.dli_saddr)
    n = 0;

  if (!n)
    seg_val = SEG_DATA;
  else
    seg_val = SEG_TEXT;

  return seg_val;
}





char *
strchr_str (const char *s, const char *match, int *res_match_len)
{
  int i, j, match_len;
  char *s2 = NULL;

  if (!sNULL ((char *) s) || !sNULL ((char *) match) || !res_match_len)
    return NULL;

  match_len = strlen (match);

  *res_match_len = 0;

  while (*s)
    {
      for (i = 0; i < match_len; i++)
	{
	  if (*s == match[i])
	    {
	      s2 = (char *) s;
	      while (*s2)
		{
		  for (j = 0; j < match_len; j++)
		    {
		      if (*s2 != match[j])
			{
			  *res_match_len = s2 - s;
			  break;
			}
		    }
		  if (*res_match_len)
		    break;
		  s2++;
		}

	      *res_match_len = s2 - s;
	      return (char *) s;
	    }
	}
      s++;
    }

  return NULL;
}




char *
str_create_spaces (int num)
{
  static char spaces[132];
  int i;

  if (num > sizeof (spaces) - 2)
    num = (sizeof (spaces) - 2);

  for (i = 0; i < num; i++)
    {
      spaces[i] = ' ';
    }

  spaces[i] = '\0';

  return spaces;
}





char *
str_replace (char *string, char *s1, char *s2)
{
  dlist_t *dl = NULL, *dptr;
  char *str = NULL, *ptr = NULL, *ptr_2 = NULL;
  char buf[MAX_BUF_SZ];

  if (!sNULL (string) || !sNULL (s1) || !sNULL (s2))
    return NULL;

  bz (buf);

  dl =
    tokenize (NULL, string,
	      TOKENIZE_NORMAL | TOKENIZE_LEAVESEP | TOKENIZE_LEAVEQUOTES, s1);
  if (!dl)
    return NULL;

  dlist_fornext (dl, dptr)
  {
    ptr = (char *) dlist_data (dptr);
    if (!ptr)
      continue;

    if (strcasestr (ptr, s1))
      {
	ptr_2 = ptr;

	ptr = str_unite ("%s%s", s2, ptr + strlen (s1));
	dptr->data = ptr;
	free (ptr_2);
      }
  }

  str = dlist_to_str (dl);

  return str;
}












int
vsnprintf_safe (char *str, size_t size, const char *fmt, va_list ap)
{
  int n = 0;

  if (!str || !sNULL ((char *) fmt))
    return 0;

  n = vsnprintf (str, size, fmt, ap);

  if (n > size)
    n = size;
  else
    n = n;

  return n;
}





char *
ret_trig (char *s)
{
  static char trig[32];
  int i = 0;

  if (!sNULL (s))
    return NULL;

  bz (trig);

  while (*s && i < sizeof (trig))
    {
      if (!istrig (*s))
	break;
      trig[i] = *s;
      i++;
      s++;
    }


  return trig;
}








memdup_t *
memdup (char *data, int len)
{
  memdup_t *mem = NULL;

  if (!data || len <= 0)
    return NULL;

  mem = (memdup_t *) calloc (1, sizeof (memdup_t));
  if (!mem)
    return NULL;

  mem->data = (char *) calloc (1, len + 1);
  if (!mem->data)
    goto cleanup;
  mem->len = len;

  memcpy (mem->data, data, len);

  return mem;

cleanup:
  if (mem)
    free (mem);

  return NULL;
}


void
memdup_free (void *arg)
{
  memdup_t *mem = NULL;
  if (!arg)
    return;

  mem = (memdup_t *) arg;

  if (mem->data)
    free (mem->data);

  free (mem);
  return;
}





int
sort_compare_forward (const char *c1, const char *c2)
{
  return strcasecmp ( c1, c2);
}

int
sort_compare_backward (const char *c1, const char *c2)
{
  return strcasecmp ( c2,  c1);
}

int
sort_compare_randward (const char *c1, const char *c2)
{
int r = rand() % 2, s = rand() % 2;
if(s) s = s*(-1);
  return r*s;
}

int
qsort_compare_forward (const void *c1, const void *c2)
{
  return strcasecmp (*(char *const *) c1, *(char *const *) c2);
}

int
qsort_compare_backward (const void *c1, const void *c2)
{
  return strcasecmp (*(char *const *) c2, *(char *const *) c1);
}

int
qsort_compare_randward (const void *c1, const void *c2)
{
int r = rand() % 2, s = rand() % 2;
if(s) s = s*(-1);
  return r*s;
}
