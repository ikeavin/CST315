#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//Created by Kevin Ahlstrom for CST 315
//All work within is my own work
//Sources: 
//learnc.org
//Effective C by Robert Seacord

void cd(char* dir, char** parseDir) {
	int i;
	for(i = 0; i < 100; i++) {
		parseDir[i] = strsep(&dir, " ");
		if(parseDir[i] == NULL) {
			break;
		}
		if(strlen(parseDir[i]) == 0) {
			i--;
		}
	}
	chdir(parseDir[1]);
}

void execute(char *input, int len) {
	printf("%c", input[0]);
	if(input[0] == 'c' && input[1] == 'd') {
		char* parseDir[100];
		cd(input, parseDir);
	} else {
		system(input);
	}
}

int run_cli(void) {
	int i = 0;
	while(1) {
		i++;
		char* in = NULL;
		size_t len = 0;
		ssize_t lineSize = 0;
		printf("kevOS: ");
		lineSize = getline(&in, &len, stdin);
		//Exit if 'Ctrl-X' is pressed
		if(in[0] == 24) {
			exit(0);
			return 0;
		} else if(in[0] == 'c' && in[1] == 'd') {
			char* parseDir[100];
			cd(in, parseDir);
			continue;
		}

		//Otherwise create a fork and wait for fork to complete
		pid_t pid;
		if((pid = fork()) == 0) {
			execute(in, len);
			_Exit(3);
		} else {
			wait(&pid);
		} 
	}
}

int main(void) {
	run_cli();
	return 0;
}
