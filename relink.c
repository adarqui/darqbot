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

int relink_res_buf_result_string(char *buf, int id, char *msg)
{
	relink_control_pkt_t *rlpkt;

	int n, len;

	if (!buf || id < 0 || !msg)
		return -1;

	len = strlen(msg);

	rlpkt = (relink_control_pkt_t *) buf;
	rlpkt->magic = RELINK_MAGIC;
	rlpkt->id = id;
	rlpkt->op = RELINK_CONTROL_OP_RESULT;
	rlpkt->type = RELINK_CONTROL_RESULT_STRING;
	rlpkt->subtype = len;

	n = sizeof(relink_control_pkt_t);
	buf = buf + sizeof(relink_control_pkt_t);
	memcpy(buf, msg, len);
	n += len;

	return n;
}

int relink_find_buf_result_string(char *buf, int buf_sz, char **msg, int *len)
{
	relink_control_pkt_t *rlpkt;

	if (!buf || !msg || !len || buf_sz <= 0)
		return -1;

	*msg = NULL;
	*len = 0;

	rlpkt = (relink_control_pkt_t *) buf;
	if (rlpkt->op != RELINK_CONTROL_OP_RESULT
	    && rlpkt->type != RELINK_CONTROL_RESULT_STRING) {
		return -1;
	}

	*msg = buf + sizeof(relink_control_pkt_t);

	if (rlpkt->subtype > buf_sz)
		*len = buf_sz;
	else
		*len = rlpkt->subtype;

	return 0;
}

int relink_pack_buf_data(char *buf, int id, char *msg, int len)
{
	relink_control_pkt_t *rlpkt;
	int n;

	if (!buf || id < 0 || !msg || len <= 0)
		return -1;

	rlpkt = (relink_control_pkt_t *) buf;
	rlpkt->magic = RELINK_MAGIC;
	rlpkt->id = id;
	rlpkt->op = RELINK_CONTROL_OP_DATA;
	rlpkt->type = len;
	rlpkt->subtype = RELINK_CONTROL_SUBTYPE_NOP;

	n = sizeof(relink_control_pkt_t);
	buf = buf + sizeof(relink_control_pkt_t);
	memcpy(buf, msg, len);
	n += len;

	return n;
}

int relink_find_buf_data(char *buf, int buf_sz, char **msg, int *len)
{
	relink_control_pkt_t *rlpkt;

	if (!buf || !msg || !len || buf_sz <= 0)
		return -1;

	*msg = NULL;
	*len = 0;

	rlpkt = (relink_control_pkt_t *) buf;
	if (rlpkt->op != RELINK_CONTROL_OP_DATA) {
		return -1;
	}

	*msg = buf + sizeof(relink_control_pkt_t);

	if (rlpkt->type > buf_sz)
		*len = buf_sz;
	else
		*len = rlpkt->type;

	debug(NULL, "relink_find_buf_data: *len=%i, *msg=[%s]\n", *len, *msg);

	return 0;
}

int relink_res_pack_buf(char *buf, int id, int type, int subtype)
{
/* always RELINK_CONTROL_OP_RESULT */
	relink_control_pkt_t *rlpkt;
	if (!buf || id < 0 || type < 0 || subtype < 0)
		return -1;

	rlpkt = (relink_control_pkt_t *) buf;
	rlpkt->magic = RELINK_MAGIC;
	rlpkt->id = id;
	rlpkt->op = RELINK_CONTROL_OP_RESULT;
	rlpkt->type = type;
	rlpkt->subtype = subtype;

	return sizeof(relink_control_pkt_t);
}

int relink_res_pack_buf_new(char *buf, int id)
{
	return relink_res_pack_buf(buf, id,
				   RELINK_CONTROL_RESULT_CONNECTION_NEW,
				   RELINK_CONTROL_SUBTYPE_NOP);
}

int relink_res_pack_buf_old(char *buf, int id)
{
	return relink_res_pack_buf(buf, id,
				   RELINK_CONTROL_RESULT_CONNECTION_OLD,
				   RELINK_CONTROL_SUBTYPE_NOP);
}

int relink_res_pack_buf_error(char *buf, int id)
{
	return relink_res_pack_buf(buf, id,
				   RELINK_CONTROL_RESULT_CONNECTION_ERROR,
				   RELINK_CONTROL_SUBTYPE_NOP);
}

int relink_req_pack_buf(char *buf, int id, int op, int type, int subtype)
{
	relink_control_pkt_t *rlpkt;
	if (!buf || op < 0 || id < 0)
		return -1;
	rlpkt = (relink_control_pkt_t *) buf;
	rlpkt->magic = RELINK_MAGIC;
	rlpkt->id = id;
	rlpkt->op = op;
	rlpkt->type = type;
	rlpkt->subtype = subtype;
	return sizeof(relink_control_pkt_t);
}

int relink_req_pack_buf_destroy_link(char *buf, int id, int link_id)
{
	return relink_req_pack_buf(buf, id, RELINK_CONTROL_OP_DESTROY,
				   RELINK_CONTROL_DESTROY_LINK, link_id);
}

int relink_req_pack_buf_destroy_sub(char *buf, int id, int sub_id)
{
	return relink_req_pack_buf(buf, id, RELINK_CONTROL_OP_DESTROY,
				   RELINK_CONTROL_DESTROY_SUB, sub_id);
}

int relink_req_pack_buf_destroy_all(char *buf, int id)
{
	return relink_req_pack_buf(buf, id, RELINK_CONTROL_OP_DESTROY,
				   RELINK_CONTROL_DESTROY_ALL,
				   RELINK_CONTROL_SUBTYPE_NOP);
}

int relink_req_pack_buf_reload(char *buf)
{
	return relink_req_pack_buf(buf, 0, RELINK_CONTROL_OP_RELOAD,
				   RELINK_CONTROL_TYPE_NOP,
				   RELINK_CONTROL_SUBTYPE_NOP);
}

int relink_req_pack_buf_query(char *buf, int id, int type)
{

	return relink_req_pack_buf(buf, id, RELINK_CONTROL_OP_QUERY, type,
				   RELINK_CONTROL_SUBTYPE_NOP);
}

int relink_req_pack_buf_query_links(char *buf, int id)
{

	return relink_req_pack_buf_query(buf, id, RELINK_CONTROL_QUERY_LINKS);
}

int relink_req_pack_buf_query_subs(char *buf, int id)
{

	return relink_req_pack_buf_query(buf, id, RELINK_CONTROL_QUERY_SUBS);
}

int relink_req_pack_buf_query_reload(char *buf, int id)
{

	return relink_req_pack_buf_query(buf, id, RELINK_CONTROL_QUERY_RELOAD);
}

int relink_req_pack_buf_hello(char *buf, int id)
{
	return relink_req_pack_buf(buf, id, RELINK_CONTROL_OP_HELLO,
				   RELINK_CONTROL_TYPE_NOP,
				   RELINK_CONTROL_SUBTYPE_NOP);
}

int
relink_req_pack_buf_relink(char *buf, int id, char *host, int port, int type,
			   int subtype)
{
	relink_control_pkt_relink_t *rlpkt_relink;

	relink_req_pack_buf(buf, id, RELINK_CONTROL_OP_RELINK, type, subtype);

	rlpkt_relink =
	    (relink_control_pkt_relink_t *) (buf +
					     sizeof(relink_control_pkt_t));
	_memset(rlpkt_relink, 0, sizeof(relink_control_pkt_relink_t));
	strlcpy_buf(rlpkt_relink->host, host);
	rlpkt_relink->port = port;
	return sizeof(relink_control_pkt_t) +
	    sizeof(relink_control_pkt_relink_t);
}

int relink_req_pack_buf_relink_new(char *buf, int id, char *line)
{
	relink_control_pkt_relink_t *rlpkt_relink;

	relink_req_pack_buf(buf, id, RELINK_CONTROL_OP_RELINK,
			    RELINK_CONTROL_TYPE_NOP,
			    RELINK_CONTROL_SUBTYPE_NOP);

	rlpkt_relink =
	    (relink_control_pkt_relink_t *) (buf +
					     sizeof(relink_control_pkt_t));
	_memset(rlpkt_relink, 0, sizeof(relink_control_pkt_relink_t));
	strlcpy_buf(rlpkt_relink->host, line);
	rlpkt_relink->port = 0;
	return sizeof(relink_control_pkt_t) +
	    sizeof(relink_control_pkt_relink_t);
}

void relink_packet_print(char *msg, relink_control_pkt_t * rlpkt)
{
	char *ptr;
	relink_control_pkt_relink_t *rlpkt_relink = NULL;
	int new_len;

	if (!rlpkt)
		return;

	ptr = (char *)rlpkt;

	debug(NULL, "%s", msg);
	debug(NULL,
	      "relink control packet:\n\trlpkt->magic=%lx\n\trlpkt->id=%i\n\trlpkt->op=%i\n\trlpkt->type=%i\n\trlpkt->subtype=%i\n",
	      rlpkt->magic, rlpkt->id, rlpkt->op, rlpkt->type, rlpkt->subtype);

	if (rlpkt->op == RELINK_CONTROL_OP_RELINK) {
		rlpkt_relink =
		    (relink_control_pkt_relink_t *) (ptr +
						     sizeof
						     (relink_control_pkt_t));
		debug(NULL,
		      "\tRELINK_CONTROL_OP_RELINK PACKET:\n\t\trelink->host=%s\n\t\trelink->port=%i\n",
		      rlpkt_relink->host, rlpkt_relink->port);
		debug_hexdump_fp(stdout, 12, (unsigned char *)rlpkt,
				 sizeof(relink_control_pkt_t) +
				 sizeof(relink_control_pkt_relink_t));
	} else if (rlpkt->op == RELINK_CONTROL_OP_DATA) {
		if (rlpkt->type > RELINK_MAX_PACKET_SIZE)
			new_len = RELINK_MAX_PACKET_SIZE;
		else
			new_len = rlpkt->type;
		debug_hexdump_fp(stdout, 12, (unsigned char *)rlpkt,
				 sizeof(relink_control_pkt_t) + new_len);
	} else if (rlpkt->op == RELINK_CONTROL_OP_RESULT) {

		if (rlpkt->type == RELINK_CONTROL_RESULT_STRING) {
			if (rlpkt->subtype > RELINK_MAX_PACKET_SIZE) {
				new_len = RELINK_MAX_PACKET_SIZE;
			} else {
				new_len = rlpkt->subtype;
			}
			debug(NULL,
			      "\n\tRELINK_CONTROL_RESULT_STRING:\n\tstring=[%.*s]\n",
			      new_len, ptr + sizeof(relink_control_pkt_t));
		}

	} else {
		debug_hexdump_fp(stdout, 12, (unsigned char *)rlpkt,
				 sizeof(relink_control_pkt_t));
	}

	return;
}

void relink_packet_ntoh(relink_control_pkt_t * rlpkt)
{
	relink_control_pkt_relink_t *rlpkt_relink;
	char *ptr;
	if (!rlpkt)
		return;

	ptr = (char *)rlpkt;

	rlpkt->magic = ntohl(rlpkt->magic);
	rlpkt->id = ntohl(rlpkt->id);
	rlpkt->op = ntohl(rlpkt->op);
	rlpkt->type = ntohl(rlpkt->type);
	rlpkt->subtype = ntohl(rlpkt->subtype);

	if (rlpkt->op == RELINK_CONTROL_OP_RELINK) {
		rlpkt_relink =
		    (relink_control_pkt_relink_t *) (ptr +
						     sizeof
						     (relink_control_pkt_t));
		rlpkt_relink->port = ntohl(rlpkt_relink->port);
	}

	return;
}

void relink_packet_hton(relink_control_pkt_t * rlpkt)
{
	relink_control_pkt_relink_t *rlpkt_relink;
	char *ptr;
	if (!rlpkt)
		return;

	ptr = (char *)rlpkt;

	if (rlpkt->op == RELINK_CONTROL_OP_RELINK) {
		rlpkt_relink =
		    (relink_control_pkt_relink_t *) (ptr +
						     sizeof
						     (relink_control_pkt_t));
		rlpkt_relink->port = htonl(rlpkt_relink->port);
	}

	rlpkt->magic = htonl(rlpkt->magic);
	rlpkt->id = htonl(rlpkt->id);
	rlpkt->op = htonl(rlpkt->op);
	rlpkt->type = htonl(rlpkt->type);
	rlpkt->subtype = htonl(rlpkt->subtype);

	return;
}

int relink_ispacket(relink_control_pkt_t * rlpkt)
{

	if (!rlpkt)
		return 0;

	if (rlpkt->magic != RELINK_MAGIC)
		return 0;
	return 1;
}

int relink_packet_len(char *buf)
{
	relink_control_pkt_t *rlpkt;
	int len = 0;
	if (!buf)
		return 0;

	rlpkt = (relink_control_pkt_t *) buf;

	if (!relink_ispacket(rlpkt))
		return 0;

	len = sizeof(relink_control_pkt_t);

	if (rlpkt->op == RELINK_CONTROL_OP_RELINK) {
		len += sizeof(relink_control_pkt_relink_t);
	} else if (rlpkt->op == RELINK_CONTROL_OP_DATA) {
		len += rlpkt->type;
	} else if (rlpkt->op == RELINK_CONTROL_OP_RESULT) {
		if (rlpkt->type == RELINK_CONTROL_RESULT_STRING) {
			len += rlpkt->subtype;
		}
	}

	return len;
}

dlist_t *relink_buf_to_dlist(char *buf, int n)
{
	dlist_t *dl = NULL;

	char *buf_ptr = NULL;
	int buf_len = 0;

	char *mem;

	if (!buf || n <= 0)
		return NULL;

	buf_ptr = buf;
	while (1) {

		relink_packet_ntoh((relink_control_pkt_t *) buf_ptr);
		relink_packet_print("relink_buf_to_dlist:\n",
				    (relink_control_pkt_t *) buf_ptr);
		if (!relink_ispacket((relink_control_pkt_t *) buf_ptr))
			break;

		if ((buf_ptr - buf) > n) {
			break;
		}

		buf_len = relink_packet_len(buf_ptr);

		mem = (char *)calloc(1, buf_len + 1);
		memcpy(mem, buf_ptr, buf_len);
		dlist_Dinsert_after(&dl, mem);

		buf_ptr += buf_len;
	}

	return dl;
}
