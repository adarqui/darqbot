/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include <stdio.h>
#include <execinfo.h>

int global_nesting;

#define ENTER if(1) { \
 char ** strings; void *array[2]; int tab, size, i; \
  size = backtrace(array,1); \
  if(size > 0) { \
   strings = backtrace_symbols(array, size); \
   for(tab=0;tab<=global_nesting;tab++) printf(" "); \
    printf("%s: Entered\n", strings[0]); \
   global_nesting++; } }

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
