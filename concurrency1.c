/* Concurrency 1
 * CS444 Spring2018
 * ----------------
 * Name: Zachary Thomas
 * Email: thomasza@oregonstate.edu
 * Date: 4/09/2018
 * -------------------------------
 *  This program creates a number of producer and 
 *  a number of consumer threads. It also creates a buffer
 *  that holds up to thirty two items. Producers 
 *  put items into the buffer, consumers take items 
 *  out while also displaying a number that is randomly
 *  generated when the item is created. This program
 *  runs indefinitely and must be manually terminated.
 */

#define MAX_ITEMS 32
#include "mt19937ar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

//struct for items that threads produce and consume
struct item {
	int consumption_num;
	int wait_period;
};

//global variable(s)
struct item buffer[MAX_ITEMS];
int items_in_buffer = 0;

//function prototype(s)
void spawn_threads(int, int);
void* consumer_thread();
void* producer_thread();
int random_range(int, int);

//create mutex lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	//first make sure the user entered num of pro and con.
	if(argc < 2){
		printf("%s [number of producer threads] [number of consumer threads]\n", argv[0]);
		return 0;
	}	

	//make sure the user has entered digits.
	if(!isdigit(*argv[1]) || !isdigit(*argv[2])){
		printf("Please enter arguments as unsigned integers.\n");
		return 0;
	}	

	//convert arguments to int.
	int producers, consumers;
	producers = strtol(argv[1], NULL, 10);	
	consumers = strtol(argv[2], NULL, 10);

	//seed random number generation
	init_genrand(time(NULL));
	
	//create threads and wait for their completion
	spawn_threads(producers, consumers);
	
	//destroy mutex lock	
	pthread_mutex_destroy(&lock);
	
	return 0;
}

/* Function: spawn_threads
 * -----------------------
 *  Spawns producer and consumer threads, then waits for threads to
 *  finish execution and join. Since these threads will run forever, we expect
 *  to block here indefinitely.
 *
 *  producers: The number of producer threads to create.
 *  consumers: The number of consumer threads to create.
 */
void spawn_threads(int producers, int consumers)
{
	pthread_t thrd;

	printf("\nCreating %d producer and %d consumer threads.\n\n", producers, consumers);
	
	while(producers || consumers){
		if(producers){
			pthread_create(&thrd, NULL, producer_thread, NULL);
			producers--;
		}

		if(consumers){
			pthread_create(&thrd, NULL, consumer_thread, NULL);
			consumers--;
		}
	}

	//join thread (this should never finish)
 	pthread_join(thrd, NULL);
}

/* Function: consumer_thread
 * -------------------------
 *  This function is called by a new consumer thread when it is created.
 *  
 *  The consumer attempts to get the mutex lock before checking if the 
 *  buffer has at least one item. It then consumes the newest item in 
 *  the buffer which causes it to display a number. Consuming an item 
 *  takes a number of seconds specified by the item. Once this thread
 *  has consumed an item or there is not any items
 *  in the buffer it releases the mutex lock.
 */
void* consumer_thread()
{
	while(true){
		pthread_mutex_lock(&lock);
		printf("Consumer has mutex lock.\n");
		if(items_in_buffer > 0){
			items_in_buffer--;
			printf("Consumer is working for %d seconds to consume.\n", buffer[items_in_buffer].wait_period);
			sleep(buffer[items_in_buffer].wait_period);
			printf("%d\n", buffer[items_in_buffer].consumption_num);
			printf("Consumer has removed an item.\n");
		}	
		printf("Consumer has released mutex lock.\n\n");
		printf("Buffer is holding %d items.\n\n", items_in_buffer);
		pthread_mutex_unlock(&lock);
		sleep(random_range(1,2));
	}
}

/* Function: producer_thread
 * -------------------------
 *  This function is called by a new producer thread when it is created.
 *  
 *  The producer attempts to get the mutex lock before checking to see 
 *  if the buffer is full. It then produces an item in the buffer, 
 *  which takes three to seven seconds. Once this thread has produced
 *  an item or the buffer is full the producer releases the mutex lock.
 */
void* producer_thread()
{
	while(true){
		pthread_mutex_lock(&lock);
		printf("Producer has mutex lock.\n");
		if(items_in_buffer < MAX_ITEMS){
			int work_time = random_range(3, 7);
			printf("Producer is working for %d seconds to produce.\n", work_time);
			sleep(work_time);
			buffer[items_in_buffer].consumption_num = random_range(1, 50);
			buffer[items_in_buffer].wait_period = random_range(2, 9);
			items_in_buffer++;
			printf("Producer has created an item.\n");
		}
		printf("Producer has released mutex lock.\n\n");
		printf("Buffer is holding %d items.\n\n", items_in_buffer);
		pthread_mutex_unlock(&lock);
		sleep(random_range(1,2));
	}
}

/* Function: random_range
 * ----------------------
 *  This function finds a random number between a min and max value (inclusive).
 *  The random value is created using rdrand x86 ASM on systems that support it,
 *  and it uses Mersenne Twister on systems that do not support rdrand.
 *
 *  min_val: The lowest possible random number.
 *  max_val: The highest possible random number.
 *
 *  returns: A random number in the given range. In the case that min_val is
 *  	     greater than max_val this function returns -1.
 */
int random_range(int min_val, int max_val)
{
	if(min_val > max_val)
		return -1;

	int output;
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
                             : "=r"(output)
	                     );
	} else {
		//use mt19937
		output = genrand_int32();
	}

	//get random number in the range requested 
	output = (abs(output) % (max_val + 1 - min_val)) + min_val;
	return output;
}
