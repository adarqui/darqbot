#include "bot.h"

int main() {

bot_t bot;

bot.ID=0xdeadbeef;
bot.parent=NULL;
bot.dptr_self=NULL;
bot.dl_subs = NULL;
bot.node_type = 2;
bot.on = 3;
bot.tag[0]='A';
bot.master[0]='B';
bot.mute = 4;
bot.type = 5;
bot.debug = 6;
bot.trace = 7;
bot.brake = 8;
bot.error_count=9;
bot.confdir[0]='C';
bot.conffile[0]='D';
bot.logfile[0]='E';
bot.uptime=10;
bot.dl_module_arg=NULL;
bot.dl_module_help=NULL;
bot.dl_environ=NULL;
bot.trig_called[0]='F';
bot.irc_command[0] = 'G';
bot.isforked_stop=11;
bot.isforked=12;
bot.isline=13;
bot.isprivmsg=14;
bot.ispm=15;
bot.iscomment=16;
bot.var_multi=17;
bot.var_usleep=18;
bot.var_maxbufsz=19;
bot.var_bufsz=20;
bot.var_maxflood=21;
bot.var_maxnesting=22;
bot.var_nesting=23;
bot.var_nesting_cur=24;
bot.var_allowpm=25;
bot.txt_server[0] = 'H';
bot.txt_server_msg = 26;
bot.txt_nick[0] = 'I';
bot.txt_ident[0] = 'J';
bot.txt_host[0] = 'K';
bot.txt_to[0] = 'L';
bot.txt_data_in[0]='M';
bot.txt_data_in_sz = 27;
bot.txt_data_out[0] = 'N';
bot.txt_data_out_sz = 28;
bot.shouldsend = 29;
bot.console_opts = 30;
bot.dl_pmodules = NULL;
bot.dl_pmodules_cur = NULL;
bot.stack_cmds = NULL;
bot.dl_gmodules = NULL;
bot.dl_gmodules_cur = NULL;
bot.dl_gmodules_keepalive = 31;
bot.dl_gmodules_force = 32;
bot.dl_control = NULL;
bot.trig_prefix[0] = 'O';

puts("HELLO");

return 0;
}

