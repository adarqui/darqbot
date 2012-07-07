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

int sig_stoi(char *string)
{
/* man psignal , man strsignal */

	int signum = 0;

	if (!string)
		return 0;

	if (!strcasecmp(string, "SIGSEGV") || !strcasecmp(string, "SEGV")) {
		signum = SIGSEGV;
	} else if (!strcasecmp(string, "SIGHUP") || !strcasecmp(string, "HUP")) {
		signum = SIGHUP;
	} else if (!strcasecmp(string, "SIGQUIT")
		   || !strcasecmp(string, "QUIT")) {
		signum = SIGQUIT;
	} else if (!strcasecmp(string, "SIGILL") || !strcasecmp(string, "ILL")) {
		signum = SIGILL;
	} else if (!strcasecmp(string, "SIGTRAP")
		   || !strcasecmp(string, "TRAP")) {
		signum = SIGTRAP;
	} else if (!strcasecmp(string, "SIGABRT")
		   || !strcasecmp(string, "ABRT")) {
		signum = SIGABRT;
	} else if (!strcasecmp(string, "SIGIOT") || !strcasecmp(string, "IOT")) {
		signum = SIGIOT;
	} else if (!strcasecmp(string, "SIGBUS") || !strcasecmp(string, "BUS")) {
		signum = SIGBUS;
	} else if (!strcasecmp(string, "SIGFPE") || !strcasecmp(string, "FPE")) {
		signum = SIGFPE;
	} else if (!strcasecmp(string, "SIGUSR1")
		   || !strcasecmp(string, "USR1")) {
		signum = SIGUSR1;
	} else if (!strcasecmp(string, "SIGALRM")
		   || !strcasecmp(string, "ALRM")) {
		signum = SIGALRM;
	} else if (!strcasecmp(string, "SIGUSR2")
		   || !strcasecmp(string, "USR2")) {
		signum = SIGUSR2;
	} else if (!strcasecmp(string, "SIGPIPE")
		   || !strcasecmp(string, "PIPE")) {
		signum = SIGPIPE;
	} else if (!strcasecmp(string, "SIGTERM")
		   || !strcasecmp(string, "TERM")) {
		signum = SIGTERM;
	} else if (!strcasecmp(string, "SIGSTKFLT")
		   || !strcasecmp(string, "STKFLT")) {
		signum = SIGSTKFLT;
	} else if (!strcasecmp(string, "SIGCLD") || !strcasecmp(string, "CLD")) {
		signum = SIGCLD;
	} else if (!strcasecmp(string, "SIGCHLD")
		   || !strcasecmp(string, "CHLD")) {
		signum = SIGCHLD;
	} else if (!strcasecmp(string, "SIGCONT")
		   || !strcasecmp(string, "CONT")) {
		signum = SIGCONT;
	} else if (!strcasecmp(string, "SIGSTOP")
		   || !strcasecmp(string, "STOP")) {
		signum = SIGSTOP;
	} else if (!strcasecmp(string, "SIGTSTP")
		   || !strcasecmp(string, "TSTP")) {
		signum = SIGTSTP;
	} else if (!strcasecmp(string, "SIGTTIN")
		   || !strcasecmp(string, "TTIN")) {
		signum = SIGTTIN;
	} else if (!strcasecmp(string, "SIGTTOU")
		   || !strcasecmp(string, "TTOU")) {
		signum = SIGTTOU;
	} else if (!strcasecmp(string, "SIGURG") || !strcasecmp(string, "URG")) {
		signum = SIGURG;
	} else if (!strcasecmp(string, "SIGXCPU")
		   || !strcasecmp(string, "XCPU")) {
		signum = SIGXCPU;
	} else if (!strcasecmp(string, "SIGXFSZ")
		   || !strcasecmp(string, "XFSZ")) {
		signum = SIGXFSZ;
	} else if (!strcasecmp(string, "SIGVTALRM")
		   || !strcasecmp(string, "VTALRM")) {
		signum = SIGVTALRM;
	} else if (!strcasecmp(string, "SIGPROF")
		   || !strcasecmp(string, "PROF")) {
		signum = SIGPROF;
	} else if (!strcasecmp(string, "SIGWINCH")
		   || !strcasecmp(string, "WINCH")) {
		signum = SIGWINCH;
	} else if (!strcasecmp(string, "SIGPOLL")
		   || !strcasecmp(string, "POLL")) {
		signum = SIGPOLL;
	} else if (!strcasecmp(string, "SIGIO") || !strcasecmp(string, "IO")) {
		signum = SIGIO;
	} else if (!strcasecmp(string, "SIGPWR") || !strcasecmp(string, "PWR")) {
		signum = SIGPWR;
	} else if (!strcasecmp(string, "SIGSYS") || !strcasecmp(string, "SYS")) {
		signum = SIGSYS;
	}

	return signum;
}

char *sig_itos(int signum)
{
	char *str = NULL;

	if (signum <= 0 || signum > _NSIG)
		return 0;

	switch (signum) {
	case SIGHUP:
		str = "Hangup (POSIX).";
		break;
	case SIGINT:
		str = "Interrupt (ANSI).";
		break;
	case SIGQUIT:
		str = "Quit (POSIX).";
		break;
	case SIGILL:
		str = "Illegal instruction (ANSI).";
		break;
	case SIGTRAP:
		str = "Trace trap (POSIX).";
		break;
	case SIGABRT:
		str = "Abort (ANSI). IOT trap (4.2 BSD).";
		break;
	case SIGBUS:
		str = "BUS error (4.2 BSD).";
		break;
	case SIGFPE:
		str = "Floating-point exception (ANSI).";
		break;
	case SIGKILL:
		str = "Kill, unblockable (POSIX).";
		break;
	case SIGUSR1:
		str = "User-defined signal 1 (POSIX).";
		break;
	case SIGSEGV:
		str = "Segmentation violation (ANSI).";
		break;
	case SIGUSR2:
		str = "User-defined signal 2 (POSIX).";
		break;
	case SIGPIPE:
		str = "Broken pipe (POSIX).";
		break;
	case SIGALRM:
		str = "Alarm clock (POSIX).";
		break;
	case SIGTERM:
		str = "Termination (ANSI).";
		break;
	case SIGSTKFLT:
		str = "Stack fault.";
		break;
	case SIGCHLD:
		str = "Child status has changed (POSIX).";
		break;
	case SIGCONT:
		str = "Continue (POSIX).";
		break;
	case SIGSTOP:
		str = "Stop, unblockable (POSIX).";
		break;
	case SIGTSTP:
		str = "Keyboard stop (POSIX).";
		break;
	case SIGTTIN:
		str = "Background read from tty (POSIX).";
		break;
	case SIGTTOU:
		str = "Background write to tty (POSIX).";
		break;
	case SIGURG:
		str = "Urgent condition on socket (4.2 BSD).";
		break;
	case SIGXCPU:
		str = "CPU limit exceeded (4.2 BSD).";
		break;
	case SIGXFSZ:
		str = "File size limit exceeded (4.2 BSD).";
		break;
	case SIGVTALRM:
		str = "Virtual alarm clock (4.2 BSD).";
		break;
	case SIGPROF:
		str = "Profiling alarm clock (4.2 BSD).";
		break;
	case SIGWINCH:
		str = "Window size change (4.3 BSD, Sun).";
		break;
	case SIGIO:
		str =
		    "Pollable event occurred (System V). I/O now possible (4.2 BSD).";
		break;
	case SIGPWR:
		str = "Power failure restart (System V).";
		break;
	case SIGSYS:
		str = "Bad system call.";
		break;
	default:
		str = "Unknown signal.";
		break;
	}

	return str;
}

int close_io_descriptors(char *to_dup)
{
	int fd;

	if (to_dup == NULL)
		return (-1);

	if ((fd = open(to_dup, O_RDWR, 0)) != -1) {
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		if (fd > STDERR_FILENO)
			close(fd);
	}

	return 0;
}

int xdaemon(nochdir, noclose)
int nochdir, noclose;
{
	pid_t pid;

	switch ((pid = fork())) {
	case -1:
		return (-1);
	case 0:
		break;
	default:
		exit(0);
	}

	if (setsid() == -1)
		return (-1);

	if (!nochdir)
		(void)chdir("/");

	if (!noclose)
		close_io_descriptors(_PATH_DEVNULL);

	return (pid);
}

void xsetproctitle(char *new_name)
{
/* uses simple method of overwriting argv[0] chars, strlen max */
	if (!new_name)
		return;

	if (!gi->argv)
		return;

	if (!gi->argv[0])
		return;

	memset(gi->argv[0], 0, strlen(gi->argv[0]));
	strlcpy(gi->argv[0], new_name, strlen(gi->argv[0]) - 1);
	return;
}

char *random_data(int len)
{
	int r, i;
	char *data = NULL;

	if (len <= 0)
		return NULL;

	data = (char *)calloc(1, len);
	if (!data)
		goto cleanup;

	for (i = 0; i < len; i++) {
		r = rand();
		data[i] = (char)r;
	}

 cleanup:

	return data;
}

void clean_environ(void)
{
	int x;

	for (x = 0; environ[x] != (NULL); x++) {
		memset(environ[x], 0, strlen(environ[x]));
		environ[x] = NULL;
	}

	environ[0] = strdup("ENVIRON=CLEANED");

	return;
}
