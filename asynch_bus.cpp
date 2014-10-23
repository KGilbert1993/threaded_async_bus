#include <iostream>
#include <string.h>
#include <pthread.h>
#include <mutex>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

pthread_mutex_t BUS_LOCK;
signed int BUS;
unsigned int RUNNING = 1;

using namespace std;

void * publish(void *param) {
	while(RUNNING) {
		pthread_mutex_lock(&BUS_LOCK);
		if(rand()%2)
			BUS = 1;
		else 
			BUS = 2;
		pthread_mutex_unlock(&BUS_LOCK);
		while(BUS != 0xAA && BUS != 0xBB) { usleep(10); }
	}
	pthread_exit(NULL);
}

void * listen_1(void *param) {
	while(RUNNING) {
		while(BUS != 1 && RUNNING) {}
		pthread_mutex_lock(&BUS_LOCK);
		cout << "listen_1!\n";
		BUS = 0xAA;		// listen_1 AWK	
		pthread_mutex_unlock(&BUS_LOCK);
	}
	pthread_exit(NULL);
}

void * listen_2(void *param) {
	while(RUNNING) {
		while(BUS != 2 && RUNNING) {}
		pthread_mutex_lock(&BUS_LOCK);
		cout << "listen_2!\n";
		BUS = 0xBB;		// listen_2 AWK
		pthread_mutex_unlock(&BUS_LOCK);
	}
	pthread_exit(NULL);
}

#define NUM_NODES 3

enum {
	PUBLISH_NODE, LISTEN_1NODE, LISTEN_2NODE
};

int main(int argc, char** argv) {
	char in;
	pthread_barrier_t *latch;
	pthread_t nodes[NUM_NODES];
	void* (*functions[NUM_NODES]) (void *param) = {publish, listen_1, listen_2};

	for(int	i=0; i<NUM_NODES; i++) {
		if(pthread_create(&nodes[i], NULL, functions[i], NULL))
			cout << "ERROR creating thread...\n";
	}
	pthread_mutex_init(&BUS_LOCK, NULL);

	sleep(3);

	RUNNING = 0;
	for(int i=0; i<NUM_NODES; i++) {
		pthread_kill(nodes[i], NULL);
	}
	
	return 0;
}
