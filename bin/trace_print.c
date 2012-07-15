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

int main(int argc, char *argv[]) {
FILE * fp = stdin;
char buf[10024];
int i;

time_t t;
int nest;
int type;
char * fn, *tok;

if(argc > 1) {
fp = fopen(argv[1], "r");
if(!fp) errx(0, "main: fopen ");
}

while(1) {
_memset(buf,0,sizeof(buf));
if(fgets(buf,sizeof(buf)-1,fp)==NULL)break;

_strstrip_nl(buf);

tok = strtok(buf, ":");
if(!tok) continue;

nest = atoi(tok);

tok = strtok(NULL, ":");
if(!tok) continue;

t = atol(tok);

tok = strtok(NULL, ":");
if(!tok) continue;

if(!strcasecmp(tok, "enter"))
type = 1;
else
type = 0;

tok = strtok(NULL, "");
if(!tok) continue;

for(i=0;i<nest;i++) {
putc(' ', stdout);
}

printf("%s:%s\n", tok, type == 1 ? "entered" : "returned");

}

return 0;
}

