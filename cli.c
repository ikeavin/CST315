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


//Memory management based on:
//c-sharpcorner.com/article/writing-our-own-memory-manager
typedef struct __page_frame {
	struct __page_frame *next;
	size_t size;
	void *memory_address;	
 } _PAGE_FRAME;

#define FRAME_SIZE sizeof(_PAGE_FRAME)

_PAGE_FRAME *allocateMemory(size_t size) {
	_PAGE_FRAME *frame = (_PAGE_FRAME*)sbrk(0);
	void *new_memory_address = (void*)sbrk(0);
	void *allocate_mem = (void*)sbrk(FRAME_SIZE + size);

	if(allocate_mem == (void*)-1) {
		return NULL;
	} else {
		frame->next = NULL;
		frame->size = size;
		frame->memory_address = new_memory_address + FRAME_SIZE;
		return frame;
	}
}

void allocateNextMemoryBlock(size_t size, _PAGE_FRAME  **head) {
	_PAGE_FRAME *current = *head;
	void *allocate_mem = NULL;
	void *memadr = (void*)sbrk(0);

	if(current == NULL) {
		*head = allocateMemory(size);
	} else {
		while(current->next != NULL) {
			current = current->next;
		}
		_PAGE_FRAME *newFrame = sbrk(0);
		
		allocate_mem = (void*)sbrk(FRAME_SIZE + size);
		if(allocate_mem != (void*) - 1) {
			newFrame->next = NULL;
			newFrame->size = size;
			newFrame->memory_address = memadr + FRAME_SIZE;
			current->next = newFrame;
		}
	}
}

void freeMemoryBlock(_PAGE_FRAME **head) {
	if(*head != NULL) {
		(*head)->size = -1;
	}
}
 
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
