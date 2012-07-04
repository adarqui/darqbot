/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#ifndef PMOD_PARSE1_H
#define PMOD_PARSE1_H

#include "bot.h"


module_t pmod_parse1_info;

bot_t *parse1_init (dlist_t *, bot_t *);
bot_t *parse1_fini (dlist_t *, bot_t *);
bot_t *parse1_help (dlist_t *, bot_t *);
bot_t *parse1_run (dlist_t *, bot_t *);

int parse1_handle_text (dlist_t *, bot_t *, void *);

void __parse1_init__ (void) __attribute__ ((constructor));

#endif
