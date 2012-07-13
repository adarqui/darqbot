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
#include "gmod_gsniff.h"

void __gsniff_init__(void) __attribute__ ((constructor));

void __gsniff_init__(void)
{

	strlcpy_buf(gmod_gsniff_info.name, "gmod_gsniff");
	strlcpy_buf(gmod_gsniff_info.trigger, "^gsniff");

	gmod_gsniff_info.init = gsniff_init;
	gmod_gsniff_info.fini = gsniff_fini;
	gmod_gsniff_info.help = gsniff_help;
	gmod_gsniff_info.run = gsniff_run;

	gmod_gsniff_info.off = gsniff_off;
	gmod_gsniff_info.destroy_up = gsniff_destroy_up;
	gmod_gsniff_info.destroy_down = gsniff_destroy_down;

	gmod_gsniff_info.control_up = gsniff_control_up;
	gmod_gsniff_info.control_down = gsniff_control_down;

	gmod_gsniff_info.output = gsniff_output;
	gmod_gsniff_info.input = gsniff_input;

	gmod_gsniff_info.self = &gmod_gsniff_info;

	debug(NULL, "__gsniff_init__: Loaded gmod_gsniff\n");

	return;
}

bot_t *gsniff_init(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gsniff_init: Entered\n");
	return NULL;
}

bot_t *gsniff_fini(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gsniff_fini: Entered\n");
	return NULL;
}

bot_t *gsniff_off(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gsniff_t *gsniff = NULL;

	debug(bot, "gsniff_off: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod) {
		return NULL;
	}

	gsniff = (gsniff_t *) gmod->data;
	if (!gsniff) {
		return NULL;
	}

	gsniff_free(gsniff);

	return NULL;
}

bot_t *gsniff_help(dlist_t * dlist_node, bot_t * bot)
{
	debug(bot, "gsniff_help: Entered\n");

	if (!bot)
		return NULL;

	bot->dl_module_help = "^gsniff";

	return NULL;
}

bot_t *gsniff_run(dlist_t * dlist_node, bot_t * bot)
{
	dlist_t *dptr_gmod = NULL;
	bot_gmod_elm_t *gmod = NULL;
	gsniff_t *gsniff = NULL;

	char *dl_module_arg_after_options, *dl_options_ptr;

	debug(bot, "gsniff_run: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	stat_inc(bot, bot->trig_called);

	debug(bot,
	      "gsniff_run: Entered: initial output buf=[%s], input buf=[%s], gmod_arg=[%s]\n",
	      bot->txt_data_out, bot->txt_data_in, bot->dl_module_arg);

	if (bot_shouldreturn(bot))
		return NULL;

	dptr_gmod =
	    (dlist_t *) gmodule_find_gmod_dptr(bot, gmod_gsniff_info.trigger,
					       gmod_gsniff_info.trigger_ext);
	if (!dptr_gmod)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dptr_gmod);
	if (!gmod) {
		debug(NULL, "gsniff_run: GNULL GMOD=NULL\n");
		return NULL;
	}

	if (!gmod->data) {
		debug(NULL,
		      "gsniff_run: GIRCD GMOD->DATA = NULL, ALLOCATING\n");
		gsniff_gmod_init(bot, gmod, dptr_gmod);
		gsniff = gmod->data;
	}

	MOD_OPTIONS_TOP_HALF;

	gsniff_process_options(gsniff, dl_options_ptr);

	MOD_OPTIONS_BOTTOM_HALF;

	gmodule_set_can_pass_up(gmod);
	gmodule_set_can_pass_down(gmod);

	gsniff_input(dptr_gmod, bot);

	gmodule_set_has_run(gmod);

	return bot;
}

char *gsniff_process_options(gsniff_t * gsniff, char *string)
{

	char *str = NULL;
	char buf[MAX_BUF_SZ];
	char *sep_ptr;

	dlist_t *dl = NULL, *dptr;

	debug(NULL, "gsniff_process_options: Entered\n");

	if (!gsniff || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	memset(buf, 0, sizeof(buf));

	dl = tokenize(NULL, string, TOKENIZE_NORMAL | TOKENIZE_LEAVEQUOTES,
		      "...");
	if (!dl)
		return NULL;

	dlist_fornext(dl, dptr) {
		gsniff_process_options_parse(gsniff, (char *)dlist_data(dptr));
	}

	tokenize_destroy(NULL, &dl);

	return str;
}

void gsniff_process_options_parse(gsniff_t * gsniff, char *string)
{

	if (!gsniff || !string)
		return;

	debug(NULL, "gsniff_process_options_parse: Entered\n");

	if (!strncasecmp_len(string, "logfile")) {
		gsniff_process_options_parse_logfile(gsniff,
						     &string[strlen
							     ("logfile=")]);
	}

	gmodule_parse_common_options(gsniff->bot, gsniff->gmod, string);

	return;
}

void gsniff_process_options_parse_logfile(gsniff_t * gsniff, char *string)
{

	if (!gsniff || !string)
		return;

	strlcpy_buf(gsniff->logfile, string);

	return;
}

char *gsniff_change_string(bot_t * bot, char *string, int opt)
{
	char *str = NULL;
	char *sep_ptr;

	if (!bot || !string)
		return NULL;

	sep_ptr = str_find_sep(string);
	if (sep_ptr)
		string = sep_ptr;

	return str;
}

bot_t *gsniff_output(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gsniff_t *gsniff = NULL;

	debug(bot, "gsniff_output: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gsniff = (gsniff_t *) gmod->data;
	if (!gsniff) {
		debug(NULL, "gsniff_output: GNULL_OUTPUT: gsniff=NULL\n");
		return NULL;
	}

	debug(NULL, "gsniff_output: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	gmodule_down(dlist_node, bot);

	return bot;
}

bot_t *gsniff_input(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gsniff_t *gsniff = NULL;

	debug(bot, "gsniff_input: Entered\n");

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	if (!gmod)
		return NULL;

	gsniff = (gsniff_t *) gmod->data;
	if (!gsniff) {
		debug(NULL, "GNULL_INPUT: gsniff=NULL\n");
		return NULL;
	}

/*
	gmodule_fix_data_down(bot);
	gmodule_up(dlist_node, bot);
*/
	if (!gsniff->initialized) {
		gsniff_input_initialize(gsniff);
	}

	if (!gsniff->initialized)
		return NULL;

	debug(NULL, "gsniff_input: in=[%s], out=[%s]\n", bot->txt_data_in,
	      bot->txt_data_out);

	return bot;
}

bot_t *gsniff_input_initialize(gsniff_t * gsniff)
{
	dlist_t *dptr_gmod_gfdpassc = NULL;
	bot_gmod_elm_t *gmod_gfdpassc = NULL;
	gfdpassc_t *gfdpassc = NULL;

	if (!gsniff)
		return NULL;

	dptr_gmod_gfdpassc =
	    gmodule_find_latest_gmod_dptr(gsniff->bot, "^gfdpassc", NULL);
	if (!dptr_gmod_gfdpassc) {
		return NULL;
	}

	gmod_gfdpassc = (bot_gmod_elm_t *) dlist_data(dptr_gmod_gfdpassc);
	if (!gmod_gfdpassc)
		return NULL;

	gfdpassc = (gfdpassc_t *) gmod_gfdpassc->data;
	if (!gfdpassc)
		return NULL;

	gsniff->initialized = 1;

	gsniff->fd = gfdpassc->fd;

	gsniff->ll.sll_protocol = htons(ETH_P_ALL);
	gsniff->ll_len = sizeof(struct sockaddr);

	gsniff_set_evhook(gsniff);

	return NULL;
}

void gsniff_set_evhook(gsniff_t * gsniff)
{

	debug(NULL, "gsniff_set_evhook: Entered\n");

	if (!gsniff)
		return;

	safe_event_set(&gsniff->ev, gsniff->fd, EV_READ | EV_PERSIST,
		       gsniff_evhook_sniff, gsniff);
	safe_event_add(&gsniff->ev, NULL);

	return;
}

void gsniff_evhook_sniff(int fd, short event, void *arg)
{

	dlist_t *dl_text = NULL;
	gsniff_t *gsniff = NULL;
	int n;
	char msg[4096], *str = NULL;
	struct ethhdr *eth = (struct ethhdr *)msg;

	//debug(NULL, "gsniff_evhook_sniff: Entered\n");

	gsniff = (gsniff_t *) arg;

	if (!gsniff)
		return;

	debug_set_cur(gsniff->bot, gsniff_evhook_sniff);

	waitpid(-1, &n, WNOHANG);

	n = recvfrom(gsniff->fd, msg, sizeof(msg), 0,
		     (struct sockaddr *)&gsniff->ll, &gsniff->ll_len);

	dl_str_unite(&dl_text,
		     "[src_mac=%2.2x.%2.2x.%2.2x.%2.2x.%2.2x.%2.2x] [dst_mac=%2.2x.%2.2x.%2.2x.%2.2x.%2.2x.%2.2x] ",
		     eth->h_source[0], eth->h_source[1], eth->h_source[2],
		     eth->h_source[3], eth->h_source[4], eth->h_source[5],
		     eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
		     eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
	dl_str_unite(&dl_text, "%s", gsniff_show_proto(eth->h_proto, msg));

/* payload */
	str = dlist_to_str(dl_text);
	dl_str_destroy(&dl_text);

	gsniff_log(gsniff, str);

	return;
}

bot_t *gsniff_destroy_up_gsniff(gsniff_t * gsniff)
{
	bot_t *ret = NULL;

	debug(NULL, "gsniff_destroy_up_gsniff: Entered\n");

	if (gsniff) {
		ret = gsniff_destroy_up(gsniff->dptr_gmod, gsniff->bot);
	}

	return ret;
}

bot_t *gsniff_destroy_down_gsniff(gsniff_t * gsniff)
{
	bot_t *ret = NULL;

	debug(NULL, "gsniff_destroy_down_gsniff: Entered\n");

	if (gsniff) {
		ret = gsniff_destroy_down(gsniff->dptr_gmod, gsniff->bot);
	}

	return ret;
}

bot_t *gsniff_destroy_up(dlist_t * dlist_node, bot_t * bot)
{
	bot_gmod_elm_t *gmod = NULL;
	gsniff_t *gsniff = NULL;

	debug(NULL, "gsniff_destroy_up: Entered\n");

	gmodule_destroy_up(dlist_node, bot);

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gsniff = gmod->data;
			free(gsniff);
		}
	}

	return bot;
}

bot_t *gsniff_destroy_down(dlist_t * dlist_node, bot_t * bot)
{

	bot_gmod_elm_t *gmod = NULL;
	gsniff_t *gsniff = NULL;

	debug(NULL, "gsniff_destroy_up: Entered\n");

	if (dlist_node) {
		gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
		if (gmod) {
			gsniff = gmod->data;
			free(gsniff);
		}
	}

	gmodule_destroy_down(dlist_node, bot);

	return bot;
}

void gsniff_gmod_init(bot_t * bot, bot_gmod_elm_t * gmod, dlist_t * dptr_gmod)
{
	gsniff_t *gsniff = NULL;
	debug(NULL, "gsniff_gmod_init: Entered\n");

	if (!gmod || !bot || !dptr_gmod)
		return;

	if (gmod->data)
		return;

	gsniff = (gsniff_t *) calloc(1, sizeof(gsniff_t));

	if (!gsniff)
		return;

	gmod->data = gsniff;

	gsniff->bot = bot;
	gsniff->dptr_gmod = dptr_gmod;
	gsniff->gmod = gmod;

	return;
}

bot_t *gsniff_control_up(dlist_t * dlist_node, bot_t * bot)
{
	gsniff_t *gsniff = NULL;
	bot_gmod_elm_t *gmod = NULL;

	debug(NULL, "gsniff_control_up: Entered\n");

	if (!dlist_node || !bot) {
		return NULL;
	}

	gmod = (bot_gmod_elm_t *) dlist_data(dlist_node);
	gsniff = (gsniff_t *) gmod->data;

	gmodule_control_up(dlist_node, bot);

	return bot;
}

bot_t *gsniff_control_down(dlist_t * dlist_node, bot_t * bot)
{
	debug(NULL, "gsniff_control_down: Entered\n");

	if (!dlist_node || !bot)
		return NULL;

	gmodule_control_down(dlist_node, bot);

	return bot;
}

void gsniff_free(void *arg)
{
	gsniff_t *gsniff = (gsniff_t *) arg;

	if (!gsniff)
		return;

/* ... */

	if (!gsniff->gmod)
		return;

	safe_event_del(&gsniff->ev);
	safe_close(gsniff->fd);
	gsniff->gmod->data = NULL;
	memset(gsniff, 0, sizeof(gsniff_t));
	free(gsniff);

	return;
}

	 /*
	  * Routine to classify
	  * protocols and
	  * display them
	  */
char *gsniff_show_proto(int i, char *msg)
{
	char *str = NULL;
	dlist_t *dl_text = NULL;
	int proto;
	struct ip *my_iphdr;
	struct tcphdr *tcph;
	struct udphdrx *my_udphdr;
	struct icmphdr *my_icmphdr;
	struct arphdr *arp_hdr;
	/*
	 *This will change the
	 *endianness
	 */
	proto = ((i >> 8) & 0xff) + ((i << 8) & 0xff00);
	if (i == 1544) {
		dl_str_unite(&dl_text, "(L2-proto=ARP[0x0%2.2X],", proto);
		arp_hdr = (struct arphdr *)(msg + sizeof(struct ethhdr));
		dl_str_unite(&dl_text, "HW-type=[%d]",
			     (((arp_hdr->ar_hrd) >> 8) & 0xff) +
			     (((arp_hdr->ar_hrd) << 8) & 0xff00));

		if (1 ==
		    (((arp_hdr->ar_hrd) >> 8) & 0xff) +
		    (((arp_hdr->ar_hrd) << 8) & 0xff00))
			dl_str_unite(&dl_text, "(ethernet)");

		dl_str_unite(&dl_text, ",");

		dl_str_unite(&dl_text, "proto-type=[%d]", arp_hdr->ar_pro);
		if (8 == arp_hdr->ar_pro)
			dl_str_unite(&dl_text, "(IP)");

		dl_str_unite(&dl_text, ",");

		dl_str_unite(&dl_text, "HW-length=[%d]", arp_hdr->ar_hln);
		dl_str_unite(&dl_text, "proto-length=[%d],", arp_hdr->ar_pln);
		dl_str_unite(&dl_text, "opcode=[0x0%2.2X],",
			     (((arp_hdr->ar_op) >> 8) & 0xff) +
			     (((arp_hdr->ar_op) << 8) & 0xff00));

		if (256 == arp_hdr->ar_op)
			dl_str_unite(&dl_text, "(request)");
		else if (512 == arp_hdr->ar_op)
			dl_str_unite(&dl_text, "(reply)");
		dl_str_unite(&dl_text, ") ... ");
	}

	else if (i == 36864)
		dl_str_unite(&dl_text, "(L2-proto=loopback[0x0%2.2X]) ... ",
			     proto);

	else if (i == 32923)
		dl_str_unite(&dl_text, "(L2-proto=AppleTalk[0x0%2.2X]) ... ",
			     proto);

	else if (i == 8) {
		dl_str_unite(&dl_text, "(L3-proto=IPv4[0x0%2.2X],", proto);
		my_iphdr = (struct ip *)(msg + sizeof(struct ethhdr));

		dl_str_unite(&dl_text, "src_addr=[%s],",
			     inet_ntoa(my_iphdr->ip_src));
		dl_str_unite(&dl_text, "dst_addr=[%s],",
			     inet_ntoa(my_iphdr->ip_dst));

		if (my_iphdr->ip_p == 1) {
			dl_str_unite(&dl_text, "(L3-proto=ICMP");
			my_icmphdr =
			    (struct icmphdr *)(msg + sizeof(struct ethhdr) +
					       sizeof(struct ip));

			if (my_icmphdr->type == ICMP_ECHO)
				dl_str_unite(&dl_text, "[ICMP_ECHO]");

			else if (my_icmphdr->type == ICMP_ECHOREPLY)
				dl_str_unite(&dl_text, "[ICMP_REPLY]");

			dl_str_unite(&dl_text, ") ... ");
		}

		else if (my_iphdr->ip_p == 6) {
			dl_str_unite(&dl_text, "(L4-proto=TCP,");
			tcph =
			    (struct tcphdr *)(msg + sizeof(struct ethhdr) +
					      sizeof(struct ip));
			dl_str_unite(&dl_text,
				     "src_port=[%i],dst_port=[%i]) ...",
				     ntohs(tcph->source), ntohs(tcph->dest));
		}

		else if (my_iphdr->ip_p == 17) {
			dl_str_unite(&dl_text, "(L4-protocol=UDP,");
			my_udphdr =
			    (struct udphdrx *)(msg + sizeof(struct ethhdr) +
					       sizeof(struct ip));
			dl_str_unite(&dl_text,
				     "src_port=[%i],dst_port=[%i]) ... ",
				     ntohs(my_udphdr->source),
				     ntohs(my_udphdr->dest));
		}

		else if (my_iphdr->ip_p == 112) {
			dl_str_unite(&dl_text, "(L3-proto=VRRP) ... ");
		}
	}

	else if (i == 56710)
		dl_str_unite(&dl_text, "(L3-proto=IPv6[0x0%2.2X]) ... ", proto);

	else
		dl_str_unite(&dl_text, "(Unknown-proto=[0x0%2.2X:%d]) ... ",
			     proto, proto);

	str = dlist_to_str(dl_text);
	dl_str_destroy(&dl_text);

	return str;
}

void gsniff_log(gsniff_t * gsniff, char *msg)
{
	FILE *fp = NULL;

	if (!gsniff || !sNULL(msg))
		return;

	if (sNULL(gsniff->logfile))
		fp = fopen(gsniff->logfile, "a+");
	else
		fp = stdout;

	if (!fp)
		return;

	fprintf(fp, "gsniff_log: %s\n", msg);
	fflush(fp);

	if (sNULL(gsniff->logfile))
		fclose(fp);

	return;
}
