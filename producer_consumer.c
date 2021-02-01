#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef int buffer_item;
#define BUFFER_SIZE 10

#define RAND_DIVISOR 100000000
#define TRUE 1

pthread_mutex_t mutex;
sem_t full, empty;
buffer_item buffer[BUFFER_SIZE];

int counter;

pthread_t tid;
pthread_attr_t attr;

void *producer(void *param);
void *consumer(void *param);

int produce(buffer_item item) {
	if(counter < BUFFER_SIZE) {
		buffer[counter] = item;
		counter++;
		return 0;
	} else {
		return -1;
	}
}

int consume(buffer_item *item) {
	if(counter > 0) {
		*item = buffer[(counter-1)];
		counter--;
		return 0;
	} else {
		return -1;
	}
}

void initializeData() {
	pthread_mutex_init(&mutex, NULL);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, BUFFER_SIZE);
	pthread_attr_init(&attr);
	counter = 0;
}

void *producer(void *param) {
	buffer_item item;
	
	while(TRUE) {
		int randNum = rand();
		sleep(1);
		
		item = randNum % 100;
		sem_wait(&empty);
		pthread_mutex_lock(&mutex);

		if(produce(item)) {
			fprintf(stderr, "Producer error\n");
		}

		pthread_mutex_unlock(&mutex);
		sem_post(&full);
	}
}

void *consumer(void *param) {
	buffer_item item;
	int *consumerID = (int*)param;

	while(TRUE) {
		sleep(1);

		sem_wait(&full);
		pthread_mutex_lock(&mutex);
		if(consume(&item)) {
			fprintf(stderr, "Consumer failed");
		} else {
			printf("Consumer %d consumed %d\n", *consumerID, item );
		}
		pthread_mutex_unlock(&mutex);
		sem_post(&empty);
	}
}

int main(int argc, char *argv[]) {
	int i;
	int numberOfProducers = 5;
	int numberOfConsumers = 10;
	
	initializeData();

	for(i = 0; i < numberOfProducers; i++) {
		pthread_create(&tid, &attr, producer, NULL);
	}

	
	long taskID[] = {0,1,2,3,4,5,6,7,8,9};
	for(i = 0; i < numberOfConsumers; i++) {
		pthread_create(&tid, &attr, consumer, taskID + i);
	}

	sleep(10);
	printf("Exiting the program \n");
	exit(0);
}

