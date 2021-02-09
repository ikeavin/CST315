#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void execute(char *input, int len) {
	system(input);
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
