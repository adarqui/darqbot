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

int
bit_extract_bit_char (unsigned char byte, int pos)
{
  if (pos < 0)
    return 0;
  return (byte >> pos) & 0x01;
}



int
bit_pack_bit_char (unsigned char *byte, int pos, int val)
{
  int b;

  if (!byte)
    return 0;

  if (pos < 0)
    return 0;

  b = *byte;

  if (val)
    b = b | (0x01 << pos);
  else
    b = b & ~(0x01 << pos);

  *byte = b;

  return *byte;
}



int
bit_pack_bit_char_set (unsigned char *byte, int pos)
{
  return bit_pack_bit_char (byte, pos, 1);
}



int
bit_pack_bit_char_clr (unsigned char *byte, int pos)
{
  return bit_pack_bit_char (byte, pos, 0);
}




bit_vector_t *
bit_vec_init (int size)
{
  bit_vector_t *bv;
  if (size <= 0)
    return NULL;
  bv = (bit_vector_t *) calloc (1, sizeof (bit_vector_t));
  if (!bv)
    return NULL;

/* FIX THIS 132 */
  bv->vec = (unsigned char *) calloc (1, size + 132);
  if (!bv->vec)
    {
      free (bv);
      return NULL;
    }

  bv->sz = size;

  return bv;
}



void
bit_vec_fini (bit_vector_t ** arg)
{
  bit_vector_t **bv, *bv_ptr;

  bv = (bit_vector_t **) arg;
  if (!bv)
    return;

  bv_ptr = (bit_vector_t *) * bv;
  if (!bv_ptr)
    return;

  if (bv_ptr->vec)
    free (bv_ptr->vec);

  memset (bv_ptr, 0, sizeof (bit_vector_t));
  free (bv_ptr);

  *bv = NULL;

  return;
}



int
bit_vec_get_and_set (bit_vector_t * bv)
{
  int bit;

  if (!bv)
    return -1;

  bit = bit_vec_scan (bv, 0);
  if (bit < 0)
    return -1;

  bit_vec_set (bv, bit);

  return bit;
}



void
bit_vec_clr (bit_vector_t * bv, int pos)
{
  int vec_pos, vec_mod;
  unsigned char byte;
  int found;

  if (!bv || pos < 0)
    return;

  if ((bv->sz * 8) < pos)
    return;

  vec_mod = vec_pos = found = 0;
  vec_pos = pos / 8;
  vec_mod = pos % 8;

  byte = bv->vec[vec_pos];
  bit_pack_bit_char_clr (&byte, vec_mod);
  bv->vec[vec_pos] = byte;

  return;
}



void
bit_vec_zero (bit_vector_t * bv)
{
  if (!bv)
    return;
  memset (bv->vec, 0, bv->sz * 8);
  return;
}



void
bit_vec_set (bit_vector_t * bv, int pos)
{

  int vec_pos, vec_mod, found;
  unsigned char byte;

  if (!bv || pos < 0)
    return;

  if ((bv->sz * 8) < pos)
    return;

  vec_mod = vec_pos = found = 0;
  vec_pos = pos / 8;
  vec_mod = pos % 8;

  byte = bv->vec[vec_pos];
  bit_pack_bit_char_set (&byte, vec_mod);
  bv->vec[vec_pos] = byte;

  return;
}





int
bit_vec_get (bit_vector_t * bv, int pos)
{

  int vec_pos, vec_mod, found, bit;
  unsigned char byte;

  if (!bv || pos < 0)
    return -1;

  if ((bv->sz * 8) < pos)
    return -1;

  vec_mod = vec_pos = found = 0;
  vec_pos = pos / 8;
  vec_mod = pos % 8;

  byte = bv->vec[vec_pos];
  bit = bit_extract_bit_char (byte, vec_mod);

  return bit;
}




int
bit_vec_scan (bit_vector_t * bv, int val)
{
  int bit, byte, i, j, k;

  if (!bv || val < 0 || val > 1)
    return -1;

  k = 0;
  for (i = 0; i < bv->sz; i++)
    {
      byte = bv->vec[i];
      for (j = 0; j < 8; j++, k++)
	{
	  bit = bit_extract_bit_char (byte, j);
	  if (bit == val)
	    return k;
	}
    }

  return -1;
}




char *
bit_vec_to_str (bit_vector_t * bv)
{
  int i, j, bit;
  char *str = NULL, buf[MAX_BUF_SZ + 1];
  unsigned char byte;

  if (!bv)
    return NULL;

  bz (buf);

  for (i = 0; i < bv->sz; i++)
    {
      byte = bv->vec[i];
      for (j = 0; j < 8; j++)
	{
	  bit = bit_extract_bit_char (byte, j);
	  strlcatfmt_buf (buf, "%i", bit);
	}
    }

  if (sNULL (buf) != NULL)
    str = strdup (buf);

  return str;
}



void
bit_vec_from_str (bit_vector_t * bv, char *string)
{

/* KEEP THIS IN HERE
   DO NOT DELETE
   incorrect code

unsigned long place_val, place_pow, place_pow_next;
unsigned long base_val, base_pow, base_pow_next;
unsigned long diff;
int i,j,k, len, bit_to_set;

debug(NULL, "bit_vec_from_str: Entred\n");

if(!bv || !string) return;

if(!str_isclean(string, isdigit)) return;

bit_vec_zero(bv);

len = strlen(string);
for(i=0,j=len-1;i<len;i++,j--) {

place_val = ctoi(string[i]);

place_pow = pow(10, j);
place_pow_next = pow(10, j-1);

place_val = place_val * place_pow;

for(;;) {

base_pow = bit_vec_base_next(place_val);

diff = place_val - base_pow;

printf("place_val=%li place_pow=%li place_pow_next=%li base_pow=%li base_pow_next=%li diff=%li\n", 
place_val, place_pow, place_pow_next, base_pow, base_pow_next, diff);

bit_to_set=log2(base_pow);
printf("SETTING %i, base_pow=%i\n", bit_to_set, base_pow);

bit_vec_set(bv, bit_to_set);
if(diff == 0) break;

place_val = diff;
}
}
*/

  unsigned long base_pow;
  unsigned long diff, num;
  int bit_to_set;

  debug (NULL, "bit_vec_from_str: Entred\n");

  if (!bv || !string)
    return;

  if (!str_isclean (string, isdigit))
    return;

  bit_vec_zero (bv);

  num = strtoul (string, NULL, 10);
  while (1)
    {
      base_pow = bit_vec_base_next (num);
      diff = num - base_pow;
      bit_to_set = log2 (base_pow);
      bit_vec_set (bv, bit_to_set);
      if (diff == 0)
	break;

      num = diff;
    }

  return;
}




void
bit_vec_from_num (bit_vector_t * bv, unsigned long num)
{

  if (!bv)
    return;


  return;
}





unsigned long
bit_vec_base_next (unsigned long x)
{
  unsigned long z;
  int y;

  x = abs (x);
  for (y = 0;; y++)
    {
      z = pow (2, y);

      if (z > x)
	{
	  return pow (2, y - 1);
	}

    }

  return z;
}




void
bit_vec_and (bit_vector_t * bv1, bit_vector_t * bv2, bit_vector_t * bvres)
{
  int i, bit_1, bit_2;

  debug (NULL, "bit_vec_and: Entered\n");

  if (!bv1 || !bv2 || !bvres)
    return;

  if (!bit_vec_3safe (bv1, bv2, bvres))
    return;

  bit_vec_zero (bvres);

  for (i = 0; i < bvres->sz * 8; i++)
    {
      bit_1 = bit_vec_get (bv1, i);
      bit_2 = bit_vec_get (bv2, i);

      if (bit_1 < 0 || bit_2 < 0)
	continue;

      if (bit_1)
	{
	  if (bit_1 == bit_2)
	    {
	      bit_vec_set (bvres, i);
	    }
	}
    }

  return;
}



void
bit_vec_or (bit_vector_t * bv1, bit_vector_t * bv2, bit_vector_t * bvres)
{
  int i, bit_1, bit_2;

  debug (NULL, "bit_vec_or: Entered\n");

  if (!bv1 || !bv2 || !bvres)
    return;

  if (!bit_vec_3safe (bv1, bv2, bvres))
    return;

  bit_vec_zero (bvres);

  for (i = 0; i < bvres->sz * 8; i++)
    {
      bit_1 = bit_vec_get (bv1, i);
      bit_2 = bit_vec_get (bv2, i);

      if (bit_1 < 0 || bit_2 < 0)
	continue;

      if (bit_1 || bit_2)
	{
	  bit_vec_set (bvres, i);
	}
    }

  return;
}






void
bit_vec_xor (bit_vector_t * bv1, bit_vector_t * bv2, bit_vector_t * bvres)
{
  int i, bit_1, bit_2;

  debug (NULL, "bit_vec_xor: Entered\n");

  if (!bv1 || !bv2 || !bvres)
    return;

  if (!bit_vec_3safe (bv1, bv2, bvres))
    return;

  bit_vec_zero (bvres);

  for (i = 0; i < bvres->sz * 8; i++)
    {
      bit_1 = bit_vec_get (bv1, i);
      bit_2 = bit_vec_get (bv2, i);

      if (bit_1 < 0 || bit_2 < 0)
	continue;

      if (bit_1 || bit_2)
	{
	  if (bit_1 != bit_2)
	    {
	      bit_vec_set (bvres, i);
	    }
	}
    }

  return;
}






void
bit_vec_not (bit_vector_t * bv)
{
  int i, bit;

  debug (NULL, "bit_vec_not: Entered\n");

  if (!bv)
    return;

  for (i = 0; i < bv->sz * 8; i++)
    {
      bit = bit_vec_get (bv, i);

      if (bit < 0)
	continue;

      bit = ~bit & 0x01;

      if (bit)
	{
	  bit_vec_set (bv, i);
	}
      else
	{
	  bit_vec_clr (bv, i);
	}
    }



  return;
}






int
bit_vec_3safe (bit_vector_t * bv1, bit_vector_t * bv2, bit_vector_t * bvres)
{
/* returns 0 if unsafe */
  int max_sz;

  if (!bv1 || !bv2 || !bvres)
    return 0;


  max_sz = MAX_FMT ("3", bv1->sz, bv2->sz, bvres->sz);

  if (bv1->sz != max_sz || bv2->sz != max_sz || bvres->sz != max_sz)
    return 0;

  return 1;
}
