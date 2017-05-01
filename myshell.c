#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

pid_t child_pid;

void kill_process(int sign)
{
	kill(child_pid, sign);
}

void main()
{
	pid_t pid;
	int status;
	pid_t child_pid;
	char *words[100];
	char str[100];
	char sep[] = " ";

	while(1) {
		printf("> ");		
		gets(str);
		
		int mode = 1;
		int i = 0;
		char *ptr = strtok(str, sep);

   		while (ptr != NULL)
		{
			words[i++] = ptr;
			ptr = strtok (NULL, sep);
		}
		
		if (i > 1 && strcmp(words[i-1], "$") == 0) {
			mode = 0;
			words[i-1] = NULL;
		} else words[i] = NULL;
		
		if (strcmp(words[0], "exit") == 0)
			exit(0);
			
		if (strcmp(words[0], "cd") == 0) {
			chdir(words[1]);
			continue;
		}
		
		pid = fork();
		switch(pid)
		{
			case -1:
				perror("fork failed");
				exit(1);
			case 0:
				(void) signal(SIGINT, SIG_DFL);
				execvp(words[0], words);
			default:
				if (mode) {
					child_pid = pid;
					(void) signal(SIGINT, SIG_IGN);
//					(void) signal(SIGINT, kill_process);
					child_pid = wait(&status);
					if (WIFEXITED(status))
						printf("------ process finished ------\n");
					else printf("------ process interrupted -----\n");
				} else {
					(void) signal(SIGINT, SIG_DFL);
				}
		}
	}
}
