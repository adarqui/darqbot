/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include <stdio.h>

int global_nesting;

#define ENTER(a) if(1) { int tab; for(tab=0;tab<=global_nesting;tab++) printf(" "); printf("%s: Entered\n", (a)); global_nesting++; }
#define return_void if(1) { global_nesting--; return; }
#define return_val(a) if(1) { global_nesting--; return((a)); }

int foo_val (void);
void foo_void (void);
int main (void);

int
foo_val (void)
{

  ENTER ("foo_val");

  foo_void ();

  return_val (1);
}

void
foo_void (void)
{

  ENTER ("foo_void");

  return_void;
}

int
main (void)
{

  ENTER ("main");

  foo_val ();

  foo_void ();

  return_void;
}
