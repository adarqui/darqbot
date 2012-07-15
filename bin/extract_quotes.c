/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <stdarg.h>


#define QUOTE_BUF_SZ 712


char *
str_unite (const char *msg, ...)
{
  va_list ap;
  int n;
  char buf[255 + 1];


  va_start (ap, msg);
  n = vsnprintf_buf (buf, sizeof (buf) - 1, msg, ap);
  va_end (ap);

  buf[n] = '\0';

  return strdup (buf);
}


char *
clean_quote (char *str)
{
  int i;
  char buf[QUOTE_BUF_SZ];

  i = 0;
  _memset (buf, 0, sizeof (buf));
  while (*str)
    {
      if (*str == ' ' || *str == '\t')
	{
	  buf[i] = *str;
	  i++;
	  while (*str == ' ' || *str == '\t')
	    str++;

//str++;
	  continue;
	}
      else
	buf[i] = *str;

      i++;
      str++;
    }

  return strdup (buf);
}


int
count_words (char *str)
{
  int cnt;
  if (!str)
    return 0;

  cnt = 0;
  while (*str)
    {
      if (*str == ' ')
	cnt++;
      str++;
    }

  return cnt;
}



void
extract_quotes (char *filename)
{
  FILE *fp, *fp_out;
  char *ptr;
  char buf[QUOTE_BUF_SZ], *buf_ptr, *buf_end;
  char quote[QUOTE_BUF_SZ], *quote_ptr;
  int quote_begin, quote_end, quote_index, quote_word_count;
  int i, buf_index, buf_len;

  if (!filename)
    return;

  if (strstr (filename, ".eq"))
    {
      printf ("extract_quotes: Already extracted %s\n", filename);
      return;
    }

  printf ("extract_quotes: Extracting quotes from %s\n", filename);

  fp = fopen (filename, "r");
  if (!fp)
    {
      perror ("fopen");
      return;
    }

  ptr = str_unite ("%s.eq", filename);


  fp_out = fopen (ptr, "w");
  if (!fp_out)
    {
      perror ("fopen(2) ");
      goto cleanup;
    }



  quote_begin = quote_end = quote_index = 0;

  while (1)
    {

    begin:
      if (fgets (buf, sizeof (buf) - 1, fp) == NULL)
	break;

      buf_ptr = buf;
//      buf_end = buf + sizeof (buf)-1;

      buf_len = strlen (buf_ptr);
      buf_end = buf_ptr + buf_len;

      if (buf_ptr > buf_end)
	{
	  continue;
	}

      while (*buf_ptr && buf_ptr < buf_end)
	{

	  if (quote_begin == 1)
	    {


// LEFTOFF
	      if (*buf_ptr == '\n')
		{
		  quote[quote_index] = ' ';
		  quote_index++;
		  buf_ptr++;
		  break;
		}

	      if (*buf_ptr != '\n')
		{
		  quote[quote_index] = *buf_ptr;
		  quote_index++;
		}


/*
if(*buf_ptr==' ' && *(buf_ptr+1)==' ') {
quote[quote_index] = ' ';
buf_ptr++; 
while(*buf_ptr!=' ') buf_ptr++;
}
*/

	      if (*buf_ptr == '\"' && !isalnum (*(buf_ptr + 1)))
		{
		  quote_ptr = clean_quote (quote);
		  quote_word_count = count_words (quote);
		  if (quote_word_count)
		    fprintf (fp_out, "%i %s %s\n", quote_word_count, filename,
			     quote_ptr);
//clean_
		  _memset (quote, 0, sizeof (quote));
		  quote_begin = 0;
		  quote_index = 0;
		  buf_ptr++;
		  continue;
		}

	      if (quote_index > sizeof (quote))
		{
		  _memset (quote, 0, sizeof (quote));
		  quote_begin = 0;
		  buf_ptr++;
		  continue;
		}

	    }
	  else if (quote_begin == 0)
	    {
	      if (*buf_ptr == '\"' && !isalnum (*(buf_ptr - 1)))
		{
		  puts ("FOUND QUOTE");
		  quote_begin = 1;
		  _memset (quote, 0, sizeof (quote));
		  quote_index = 0;
		  quote[quote_index] = *buf_ptr;
		  quote_index++;
		}

	    }


	  buf_ptr++;
	}


/*
      if (quote_begin == 1 && quote_end == 0)
	{

	  while (*buf_ptr)
	    {
	      printf ("%c\n", *buf_ptr);
	      if (*buf_ptr == '\n')
		{
		  buf_ptr++;
		  continue;
		}

	      quote[quote_index] = *buf_ptr;
	      quote_index++;

	      if (quote_index > sizeof (quote))
		{
		  quote_end = 1;
		  break;
		}

	      if (*buf_ptr == '\"')
		{
		  quote_end = 1;
		  fprintf (fp_out, "%s\n", quote);
		  break;
		}
	    }
	  buf_ptr++;
	}

      if (quote_begin == 0)
	{

	  while (*buf_ptr)
	    {
	      if (*buf_ptr == '\"')
		{
		  quote_begin = 1;
		  _memset (quote, 0, sizeof (quote));
		  buf_ptr++;
		  break;
		}
	      buf_ptr++;
	    }

	}


      if (quote_begin)
	goto loop;
*/


    }


cleanup:

  if (fp)
    fclose (fp);
  if (fp_out)
    fclose (fp_out);

  if (ptr)
    free (ptr);


  return;
}

int
main (int argc, char *argv[])
{
  DIR *dir;
  struct dirent *de;
  char buf[10024];

  if (argc < 2)
    return 0;

  dir = opendir (argv[1]);
  if (!dir)
    return 0;


  chdir (argv[1]);

  while (1)
    {
      de = readdir (dir);
      if (!de)
	break;

      if (de->d_name[0] == '.')
	continue;

      extract_quotes (de->d_name);

    }

  return 0;
}
