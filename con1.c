// Concurrency 1
// CS444 Spring2018
//
// Name: Zachary Thomas
// Email: thomasza@oregonstate.edu
// Date: 4/09/2018
//

#include "mt19937ar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
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
int random_range(int, int);

//create mutex lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	
	//seed random number generation
	init_genrand(time(NULL));

	//print some random numbers:
	int test = random_range(0, 0);
	printf("Random number: %d\n", test);
	test = random_range(1, 2);
	printf("Random number: %d\n", test);
	test = random_range(10, 20);
	printf("Random number: %d\n", test);
	test = random_range(30, 40);
	printf("Random number: %d\n", test);
	test = random_range(5, 6);
	printf("Random number: %d\n", test);
	test = random_range(5, 10);
	printf("Random number: %d\n", test);
	
	//create multiple producer and consumer threads	
	printf("Creating three producer and three consumer threads.\n\n");
	pthread_t pro_t1, pro_t2, pro_t3, con_t1, con_t2, con_t3;
	pthread_create( &pro_t1, NULL, producer_thread, NULL);
	pthread_create( &pro_t2, NULL, producer_thread, NULL);
	pthread_create( &pro_t3, NULL, producer_thread, NULL);
	pthread_create( &con_t1, NULL, consumer_thread, NULL); 
	pthread_create( &con_t2, NULL, consumer_thread, NULL); 
	pthread_create( &con_t3, NULL, consumer_thread, NULL); 
	
	//join threads (this should never finish)
 	pthread_join(pro_t1, NULL);
 	pthread_join(pro_t2, NULL);
 	pthread_join(pro_t3, NULL);
 	pthread_join(con_t1, NULL);
 	pthread_join(con_t2, NULL);
 	pthread_join(con_t3, NULL);
	
	//destroy mutex lock	
	pthread_mutex_destroy(&lock);
	
	return 0;
}

void* consumer_thread()
{
	while(true){
		if(items_in_buffer > 0){
			//we get the mutex lock
			pthread_mutex_lock(&lock);
			printf("Consumer has mutex lock.\n");
			//we take an item out of the buffer
			items_in_buffer--;
			printf("Consumer is working for %d seconds to consume.\n", buffer[items_in_buffer].wait_period);
			sleep(buffer[items_in_buffer].wait_period);
			printf("%d\n", buffer[items_in_buffer].consumption_num);
			printf("Consumer has removed an item.\n");
			//we release the mutex lock
			printf("Consumer has released mutex lock.\n\n");
			pthread_mutex_unlock(&lock);
		}
	}
}

void* producer_thread()
{
	while(true){
		if(items_in_buffer < MAX_ITEMS){
			//we get the mutex lock
			pthread_mutex_lock(&lock);
			printf("Producer has mutex lock.\n");
			//we put an item into the buffer
			buffer[items_in_buffer].consumption_num = 7;
			buffer[items_in_buffer].wait_period = 8;
			items_in_buffer++;
			printf("Producer has created an item.\n");
			//we release the mutex lock
			printf("Producer has released mutex lock.\n\n");
			pthread_mutex_unlock(&lock);
			sleep(5);
		}
	}
}

int random_range(int min_val, int max_val)
{
	unsigned int output;
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
		//use rdrand
	__asm__ __volatile__(
	                     "rdrand %0"
                             :"=r" (output)
	                     );
	} else {
		//use mt19937
		output = genrand_int32();
	}

	//get random number in the range requested 
	output = abs(output) % (max_val + 1 - min_val) + min_val;
	return output;
}
