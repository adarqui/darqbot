/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include <stdio.h>


void __cyg_profile_func_enter (void *this_fn, void *call_site)
  __attribute__ ((no_instrument_function));

void __cyg_profile_func_exit (void *this_fn, void *call_site)
  __attribute__ ((no_instrument_function));

void
__cyg_profile_func_enter (void *this_fn, void *call_site)
{

  printf ("enter: addr=%p\n", this_fn);
  return;
}

void
__cyg_profile_func_exit (void *this_fn, void *call_site)
{
  printf ("exit: addr=%p\n", this_fn);
  return;
}


void
foo (void)
{

  puts ("foo called");

  return;
}

int
main (int argc, char **argv[])
{

  puts ("main called");

  foo ();
}
