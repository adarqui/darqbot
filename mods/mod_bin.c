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
/* darqbot 1.0 (2012)
  multi purpose chat & knowledge bot
  -- adarqui ::: adarq.org
 */
#include "mod_bin.h"

void
__bin_init__ (void)
{

strlcpy_buf(mod_bin_info.name, "mod_bin");
strlcpy_buf(mod_bin_info.trigger, "^bin");

  mod_bin_info.init = bin_init;
  mod_bin_info.fini = bin_fini;
  mod_bin_info.help = bin_help;
  mod_bin_info.run = bin_run;


  mod_bin_info.output = NULL;
  mod_bin_info.input = NULL;


  debug (NULL, "__bin_init__: Loaded mod_bin\n");

  return;
}



bot_t *
bin_init (dlist_t * dlist_node, bot_t * bot)
{

  debug (bot, "bin_init: Entered\n");

  return NULL;
}

bot_t *
bin_fini (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "bin_fini: Entered\n");

  return NULL;
}

bot_t *
bin_help (dlist_t * dlist_node, bot_t * bot)
{
  debug (bot, "bin_help: Entered\n");


  if (!bot)
    return NULL;

  bot->dl_module_help = "^bin(program,args) <input>";

  return NULL;
}

bot_t *
bin_run (dlist_t * dlist_node, bot_t * bot)
{
  char *dl_module_arg_after_options, *dl_options_ptr;
char * opt_prog=NULL, *opt_options=NULL;

  debug (bot, "bin_run: Entered\n");

  if (!dlist_node || !bot)
    return NULL;

  stat_inc(bot,bot->trig_called);

  debug (bot,
	     "bin_run: Entered: initial output buf=[%s], input buf=[%s], mod_arg=[%s]\n",
	     bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);


  if (bot_shouldreturn (bot))
    return NULL;

MOD_OPTIONS_TOP_HALF;
opt_prog = strtok(dl_options_ptr, ",");
if(opt_prog) {
opt_options = strtok(NULL, "");
}

  MOD_OPTIONS_BOTTOM_HALF;

  MOD_PARSE_TOP_HALF_NODL;
  l_new_str = bin_change_string (bot, opt_prog, opt_options, l_str_ptr, dlist_node);
  MOD_PARSE_BOTTOM_HALF_NODL;

  return bot;
}



char *
bin_change_string (bot_t * bot, char *prog, char * options, char * input, dlist_t * dlist_node)
{
  char *str = NULL;
  char *sep_ptr;

  sep_ptr = str_find_sep (input);
  if (sep_ptr)
    input = sep_ptr;

if(!bot_fork_clean(bot)) {

alarm(5);
      //global_signal_exit_hooks ();
signal(SIGCHLD, bin_sig_handler);
signal(SIGPIPE, bin_sig_handler);
      signal (SIGALRM, bin_sig_handler);

str = bin_op_run(bot, prog, options, input,dlist_node);
}

  return str;
}



char * bin_op_run(bot_t * bot, char * prog, char * options, char * input, dlist_t * dlist_node) {
pid_t pid=0;
char buf[MAX_BUF_SZ+1];
char * path=NULL;
char * str=NULL,*ptr=NULL;

dlist_t * dl_data=NULL,*dptr_data=NULL;
memdup_t *mem=NULL;

int argc=0;
char **argv=NULL;

int pipefds1[2],pipefds2[2];

int n=0,i=0;

debug(NULL, "bin_op_run: Entered: %s %s %s\n",prog,options,input);

if(!bot || !sNULL(prog))
return NULL;

if(!str_apply_is(prog, isprog)) return NULL;

if(!sNULL(options)) options="";
path = str_unite_static("%s/mods/mod_bin_files/%s %s", gi->confdir,prog, options);
argv = tokenize_str2argv(path, &argc, 0);
if(!argv) return NULL;

for(i=0;i<argc;i++) {
debug(NULL, "bin_op_run: %i. %s\n", i, argv[i]);
}

clean_environ();

if(pipe(pipefds1)<0) goto cleanup;
if(pipe(pipefds2)<0) goto cleanup;

pid = bot_fork_clean(bot);
if(pid< 0) goto cleanup;

if(!pid) {
pid = getpid();

close(0);
close(1);
close(2);

close(pipefds2[0]);
dup2(pipefds2[1], 1);
dup2(pipefds2[1], 2);
close(pipefds1[1]);
close(0);
dup2(pipefds1[0],0);


execve(argv[0],argv,environ);
bot_fork_clean_exit(bot);
return 0;
}
else {
close(pipefds1[0]);
close(pipefds2[1]);
while(1) {
bz(buf);
n = read(pipefds2[0], buf, sizeof(buf)-1);
if(n<=0) break;

dlist_Dinsert_after(&dl_data, memdup(buf,n));
}
}


bz (bot->txt_data_in);

strzero_bot(bot->txt_data_in);
strlcat_bot(bot->txt_data_in, bot->txt_data_out);

bot->txt_data_in_sz = strlen(bot->txt_data_in);

dlist_fornext(dl_data, dptr_data){
if(!dptr_data) break;
mem = (memdup_t *) dlist_data(dptr_data);
if(!mem) continue;
//memcpy_bot(bot->txt_data_in, mem->data, mem->len);
strlcat_bot(bot->txt_data_in, mem->data);
bot->txt_data_in_sz += mem->len;
}


if(dlist_node) {

  dlist_fornext (dlist_next (dlist_node), dptr_data)
  {
    ptr = (char *) dlist_data (dptr_data);
    charcat_bot (bot->txt_data_in, '|');
    strlcat_bot (bot->txt_data_in, ptr);
bot->txt_data_in_sz += strlen(ptr)+1;
  }
}


bz(bot->trig_called);
strlcat_buf(bot->trig_called, "^parse2");
bot->dl_module_arg = bot->trig_called + strlen("^parse2");

bot->isforked=0;
bot->isforked_stop=0;

  pmodule_cur_run2 (bot);
 
      gmodule_down (bot->dl_gmodules_cur, bot);
bot->isforked=1;
bot->isforked_stop=1;

cleanup:

dlist_fini(&dl_data, memdup_free);

if(argv)
tokenize_destroy_array(NULL, argv);

bot_fork_clean_exit(bot);

return str;
}





void
bin_sig_handler (int signum)
{
debug(NULL, "bin_sig_handler: Entered\n");
return;
}
