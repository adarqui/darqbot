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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>

int kill(pid_t, int);

//extern char * environ[];

pid_t pid;

void sigint_handler(int signum)
{

	kill(pid, SIGKILL);
	exit(0);

	return;
}

int main()
{
	int status, exit_code;
	char *nargv[2];

	nargv[0] = "./bot_main_process";
	nargv[1] = NULL;

	signal(SIGINT, sigint_handler);

	while (1) {

		pid = fork();

		if (!pid) {
			pid = execve(nargv[0], nargv, NULL);
			exit(pid);
		} else {
			wait(&status);
			exit_code = WEXITSTATUS(status);
			printf("status=%i, exit_code=%i\n", status, exit_code);

			if (exit_code == 254) {
				kill(pid, SIGKILL);
				exit(0);
			}

			sleep(30);
		}

	}

	return 0;
}
