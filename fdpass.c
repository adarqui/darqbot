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

int fdpass_req_get(fdpass_control_t * fc, int subtype)
{

	debug(NULL, "fdpass_req_get: Entered\n");

	if (!fc || subtype < 0)
		return -1;

	fc->op.type = FDPASS_CONTROL_TYPE_REQUEST;
	fc->op.subtype = FDPASS_CONTROL_SUBTYPE_GET;
	fc->op.subtype_op = subtype;

	fc->msg.msg_control = NULL;
	fc->msg.msg_controllen = 0;

	return 0;
}

int fdpass_req_get_raw(fdpass_control_t * fc)
{

	debug(NULL, "fdpass_req_get_raw: Entered\n");
	return fdpass_req_get(fc, FDPASS_CONTROL_GET_RAW);
}

int fdpass_resp_nop(fdpass_control_t * fc)
{

	debug(NULL, "fdpass_resp_nop: Entered\n");

	if (!fc)
		return -1;

	fc->op.type = FDPASS_CONTROL_TYPE_RESPONSE;
	fc->op.subtype = FDPASS_CONTROL_SUBTYPE_NOP;

	fc->msg.msg_control = NULL;
	fc->msg.msg_controllen = 0;

	return 0;
}

int fdpass_resp_get(fdpass_control_t * fc, int subtype, int fd)
{

	debug(NULL, "fdpass_resp_get: Entered\n");

	if (!fc || subtype < 0 || fd < 0)
		return -1;

	fc->op.type = FDPASS_CONTROL_TYPE_RESPONSE;
	fc->op.subtype = FDPASS_CONTROL_SUBTYPE_GET;
	fc->op.subtype_op = subtype;

	*(int *)((void *)fc->cmsg + sizeof(struct cmsghdr)) = fd;

	fc->msg.msg_control = fc->cmsg_buf;
	fc->msg.msg_controllen = fc->cmsg->cmsg_len = sizeof(fc->cmsg_buf);

	return 0;
}

int fdpass_resp_get_raw(fdpass_control_t * fc, int fd)
{

	debug(NULL, "fdpass_resp_get_raw: Entered\n");

	return fdpass_resp_get(fc, FDPASS_CONTROL_GET_RAW, fd);
}

void fdpass_print(struct msghdr *msg)
{
	fdpass_control_op_t *fcop = NULL;
	struct cmsghdr *cmsg = NULL;
	int fd = 0;

	if (!msg)
		return;

	if (msg->msg_iovlen <= 0)
		return;

	fcop = (fdpass_control_op_t *) msg->msg_iov[0].iov_base;
	if (!fcop)
		return;

	debug(NULL,
	      "fdpass_print: PRINTING\n"
	      "\tmsg:\n"
	      "\t\tcontrol=%p, controllen=%i\n"
	      "\tfc:\n"
	      "\t\ttype=%i, subtype=%i, subtype_op=%i\n"
	      "\t\ttag=[%s] , tag_ext=[%s]\n",
	      msg->msg_control, msg->msg_controllen,
	      fcop->type, fcop->subtype, fcop->subtype_op, fcop->tag,
	      fcop->tag_ext);

	if (msg->msg_control) {
		cmsg = (struct cmsghdr *)msg->msg_control;
		fd = *(int *)((void *)cmsg + sizeof(struct cmsghdr));
		debug(NULL,
		      "\n\tcmsg:\n"
		      "\t\tcmsg_level=%i, cmsg_type=%i, fd=%i\n",
		      cmsg->cmsg_level, cmsg->cmsg_type, fd);
	}

	return;
}

fdpass_control_t *fdpass_init(char *tag, char *tag_ext)
{
	fdpass_control_t *fc = NULL;

	if (!_sNULL(tag))
		return NULL;

	fc = (fdpass_control_t *) calloc(1, sizeof(fdpass_control_t));
	if (!fc)
		return NULL;

	strlcpy_buf(fc->op.tag, tag);
	if (_sNULL(tag_ext))
		strlcpy_buf(fc->op.tag_ext, tag_ext);

	fc->iov[0].iov_len = sizeof(fdpass_control_op_t);
	fc->iov[0].iov_base = &fc->op;

	fc->msg.msg_name = 0;
	fc->msg.msg_namelen = 0;

	fc->cmsg = (struct cmsghdr *)fc->cmsg_buf;
	fc->cmsg->cmsg_level = SOL_SOCKET;
	fc->cmsg->cmsg_type = SCM_RIGHTS;

	fc->msg.msg_control = fc->cmsg_buf;
	fc->msg.msg_controllen = sizeof(fc->cmsg_buf);
	fc->msg.msg_iov = (struct iovec *)&fc->iov;
	fc->msg.msg_iovlen = 1;
	fc->msg.msg_flags = 0;

	return fc;
}

void fdpass_fini(fdpass_control_t ** fc)
{

	if (!fc)
		return;

	if (!(*fc))
		return;

	_memset(*fc, 0, sizeof(fdpass_control_t));
	free(*fc);

	*fc = NULL;

	return;
}
