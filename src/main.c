/***********************************************/
/*                 CANSH SHELL                 */
/***********************************************/
/*   CREATED WITH PARTIAL HELP FROM MESYETI:   */
/*         https://github.com/MESYETI/         */
/*                                             */
/*    MEMORY LEAKS FIXED BY LORD OF TRIDENT:   */
/*      https://github.com/LordOfTrident/      */
/*                                             */
/*       https://github.com/apladi/cansh       */
/***********************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

char *args[] = {};
char rc[5000];
char d[500];
char username[256];

int cansh(void) {
	cuserid(username);
	
	strcpy(rc, "\e[32m");
	strcat(rc, username); //Username
	strcat(rc, "\e[97m:\e[94m");
	strcat(rc, getcwd(d, 100)); //Directory
	strcat(rc, "> \e[0m");
}

int launch(void) {
	pid_t pid;
	int   status;

	pid = fork();

	if (pid == 0) {
		if (execvp(args[0], args) == -1) {
			perror("cansh");
			exit(EXIT_FAILURE);
		} else if (pid < 0) {
			perror("cansh");
		}
	}

	if (pid > 0) {
		pid = wait(&status);
	}

	return 0;
}

int main(void) {
	signal(SIGINT, main);
	int counter;
	int checker;
	char *input;
 	char *token;

	while (1) {
		cansh();
		input = readline(rc);

		checker = 1;

		for (size_t i = 0; i < strlen(input); ++i) {
			if (input[i] != ' ') {
				checker = 0;
			}
		}

		if (checker)
			continue;

		counter = 0;
		args[counter] = strtok(input, " ");
		
		while(args[counter] != NULL) {
			++counter;
			args[counter] = strtok(NULL, " ");
		}

		if (strcmp("cd", args[0]) == 0) {
			chdir(args[1]);
		} else if (strcmp("cansh", args[0]) == 0) {
			printf("CANSH\nVersion: 1.0\n");
		} else if (strcmp("exit", args[0]) == 0) {
			exit(1);
		} else {
			launch();
		}

		free(input);
	}
}
