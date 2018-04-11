// Concurrency 1
// CS444 Spring2018
//
// Name: Zachary Thomas
// Email: thomasza@oregonstate.edu
// Date: 4/09/2018
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_ITEMS 32

//stuct for items that threads produce and consume
struct item {
	int consumption_num;
	int wait_period;
};

//global variable(s)
struct item buffer[MAX_ITEMS];
int items_in_buffer = 0;

//function prototype(s)
void* consumer_thread();
void* producer_thread();

//create mutex lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	//create a producer and consumer thread	
	pthread_t pro_t, con_t;
	pthread_create( &pro_t, NULL, producer_thread, NULL);
	sleep(1); 
	pthread_create( &con_t, NULL, consumer_thread, NULL); 

	//join threads
 	pthread_join(pro_t, NULL);
 	pthread_join(con_t, NULL);
	
	//destroy mutex lock	
	pthread_mutex_destroy(&lock);

	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	char vendor[13];
	
	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );
	
	if(ecx & 0x40000000){
	//	rand(time(0)); //use rdrand
	} else {
		//use mt19937
	}
	
	return 0;
}

void* consumer_thread()
{
	printf("Consumer thread created.\n");
	while(true) {
		if(items_in_buffer > 0) {
			//we get the mutex lock
			pthread_mutex_lock(&lock);
			printf("Consomer has mutex lock.\n");
			//we take an item out of the buffer
			items_in_buffer--;
			//sleep(buffer[items_in_buffer].wait_period);
			printf("%d\n",buffer[items_in_buffer].consumption_num);
			printf("Consumer has removed an item.\n");
			//we release the mutex lock
			pthread_mutex_unlock(&lock);
			printf("Consomer has released mutex lock.\n");
		}
	}
}

void* producer_thread()
{
	printf("Producer thread created.\n");
	while(true) {
		if(items_in_buffer < MAX_ITEMS) {
			//we get the mutex lock
			pthread_mutex_lock(&lock);
			printf("Producer has mutex lock.\n");
			//we put an item into the buffer
			buffer[items_in_buffer].consumption_num = items_in_buffer;
			buffer[items_in_buffer].wait_period = items_in_buffer;
			items_in_buffer++;
			printf("Producer has created an item.\n");
			//we release the mutex lock
			pthread_mutex_unlock(&lock);
			printf("Producer has released mutex lock.\n");
		}
	}
}
