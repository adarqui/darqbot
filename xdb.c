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

DB *
xdb_open (char *filename)
{

  DB *db = NULL;
  int ret;

  debug (NULL, "xdb_open: Entered: %s\n", filename);

  if (!sNULL (filename))
    return NULL;

  ret = db_create (&db, NULL, 0);

  if (!db || ret != 0)
    return NULL;

  ret = db->set_flags (db, DB_RECNUM);

  ret = db->open (db, NULL, filename, NULL, DB_BTREE, DB_CREATE, 0644);
  if (ret != 0)
    goto cleanup;

  return db;


cleanup:
  if (db)
    db->close (db, 0);

  return NULL;
}


int
xdb_write (DB * db, char *key, char *value)
{
  DBT db_key, db_value;
  int ret = 0;

  if (!db || !sNULL (key) || !sNULL (value))
    return -1;

  bz2 (db_key);
  bz2 (db_value);

  db_key.data = key;
  db_key.size = strlen (key);

  db_value.data = value;
  db_value.size = strlen (value);

  ret = db->put (db, NULL, &db_key, &db_value, DB_NOOVERWRITE);

  return ret;
}


xdb_pair_t *
xdb_get (DB * db, char *key)
{
  xdb_pair_t *pair = NULL;

  DBT db_key, db_value;

  int ret;

  if (!db || !sNULL (key))
    return NULL;


  bz2 (db_key);
  bz2 (db_value);

  db_key.data = key;
  db_key.size = strlen (key);

  ret = db->get (db, NULL, &db_key, &db_value, 0);
  if (ret != 0)
    return NULL;

  pair =
    xdb_pair_create (db_key.data, db_key.size, db_value.data, db_value.size);

  return pair;
}




xdb_pair_t *
xdb_get_recnum (DB * db, int recnum)
{
  xdb_pair_t *pair = NULL;
  DBC *dbc = NULL;
  int ret;
  int count;

  DBT db_key, db_value;

  if (!db || recnum < 0)
    return NULL;

  count = xdb_count (db);
  if (recnum > count || recnum == 0)
    {
      while (1)
	{
	  recnum = rand () % (count + 1);
	  if (recnum > 0)
	    break;
	}
    }

  ret = db->cursor (db, NULL, &dbc, 0);

  if (ret != 0)
    return NULL;

  bz2 (db_key);
  bz2 (db_value);

  db_key.data = &recnum;
  db_key.size = sizeof (recnum);

  ret = db->get (db, NULL, &db_key, &db_value, DB_SET_RECNO);
  if (ret != 0)
    goto cleanup;

  pair =
    xdb_pair_create (db_key.data, db_key.size, db_value.data, db_value.size);

cleanup:
  if (dbc)
    dbc->c_close (dbc);

  return pair;
}



int
xdb_count (DB * db)
{
  struct __db_bt_stat *bts;
  int count = 0;

  debug (NULL, "xdb_count: Entered\n");

  if (!db)
    return 0;

  memset (&bts, 0, sizeof (bts));

  db->stat (db, NULL, &bts, DB_FAST_STAT);

  debug (NULL, "xdb_count: %i records\n", bts->bt_nkeys);

  count = bts->bt_nkeys;

  free (bts);

  return count;
}


xdb_pair_t *
xdb_pair_create (char *s1, int s1_len, char *s2, int s2_len)
{

  xdb_pair_t *pair = NULL;

  if (!sNULL (s1) || s1_len <= 0 || !sNULL (s2) || s2_len <= 0)
    return NULL;

  pair = (xdb_pair_t *) calloc (1, sizeof (xdb_pair_t));
  if (!pair)
    return NULL;

  pair->key = strdup_len (s1, s1_len);
  pair->value = strdup_len (s2, s2_len);

  return pair;
}


void
xdb_pair_destroy (void *arg)
{
  xdb_pair_t *pair = (xdb_pair_t *) arg;
  if (!pair)
    return;

  if (pair->key)
    free (pair->key);
  if (pair->value)
    free (pair->value);

  free (pair);
  return;
}


void
xdb_fini (DB * db)
{

  debug (NULL, "xdb_fini: Entered\n");

  if (!db)
    return;

  db->close (db, 0);

  return;
}







xdb_pair_t *
xdb_pair_find_by_key (dlist_t * dl, char *key)
{

  dlist_t *dptr;

  xdb_pair_t *pair = NULL;

  if (!dl || !sNULL (key))
    return NULL;

  dlist_fornext (dl, dptr)
  {
    pair = (xdb_pair_t *) dlist_data (dptr);
    if (!pair)
      continue;
    if (!pair->key)
      continue;

    if (!strcasecmp (pair->key, key))
      return pair;

  }


  return NULL;
}
