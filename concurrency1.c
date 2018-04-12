// Concurrency 1
// CS444 Spring2018
//
// Name: Zachary Thomas
// Email: thomasza@oregonstate.edu
// Date: 4/09/2018

#include "mt19937ar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#define MAX_ITEMS 32

//struct for items that threads produce and consume
struct item {
	int consumption_num;
	int wait_period;
};

//global variable(s)
struct item buffer[MAX_ITEMS];
int items_in_buffer = 0;

//function prototype(s)
void spawn_threads();
void* consumer_thread();
void* producer_thread();
int random_range(int, int);

//create mutex lock
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv)
{
	//seed random number generation
	init_genrand(time(NULL));
	
	//create threads and wait for their completion
	spawn_threads();
	
	//destroy mutex lock	
	pthread_mutex_destroy(&lock);
	
	return 0;
}

/* Function: spawn_threads
 * -----------------------
 *  Spawns five producer and five consumer threads, then waits for threads to
 *  finish execution and join. Since these threads will run forever, we expect
 *  to block here indefinitely.
 */
void spawn_threads()
{
	//create five producer and five consumer threads	
	printf("\nCreating five producer and five consumer threads.\n\n");
	pthread_t pro_thrd1, pro_thrd2, pro_thrd3, pro_thrd4, pro_thrd5;
	pthread_t con_thrd1, con_thrd2, con_thrd3, con_thrd4, con_thrd5;
	pthread_create( &pro_thrd1, NULL, producer_thread, NULL);
	pthread_create( &pro_thrd2, NULL, producer_thread, NULL);
	pthread_create( &pro_thrd3, NULL, producer_thread, NULL);
	pthread_create( &pro_thrd4, NULL, producer_thread, NULL);
	pthread_create( &pro_thrd5, NULL, producer_thread, NULL);
	pthread_create( &con_thrd1, NULL, consumer_thread, NULL); 
	pthread_create( &con_thrd2, NULL, consumer_thread, NULL); 
	pthread_create( &con_thrd3, NULL, consumer_thread, NULL); 
	pthread_create( &con_thrd4, NULL, consumer_thread, NULL); 
	pthread_create( &con_thrd5, NULL, consumer_thread, NULL); 
	
	//join threads (this should never finish)
 	pthread_join(pro_thrd1, NULL);
 	pthread_join(pro_thrd2, NULL);
 	pthread_join(pro_thrd3, NULL);
 	pthread_join(pro_thrd4, NULL);
 	pthread_join(pro_thrd5, NULL);
 	pthread_join(con_thrd1, NULL);
 	pthread_join(con_thrd2, NULL);
 	pthread_join(con_thrd3, NULL);
 	pthread_join(con_thrd4, NULL);
 	pthread_join(con_thrd5, NULL);
}

/* Function: consumer_thread
 * -------------------------
 *  This function is called by a new consumer thread when it is created.
 *  
 *  The consumer waits for the buffer to contain at least one item and then 
 *  attempts to get the mutex lock before once more checking the buffer.
 *  It then consumes the newest item in the buffer which causes it to display 
 *  a number. Consuming an item takes a number of seconds specified by the item. 
 *  Once this thread has consumed an item or there is not any items
 *  in the buffer it releases the mutex lock.
 */
void* consumer_thread()
{
	while(true){
		if(items_in_buffer > 0){
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
			pthread_mutex_unlock(&lock);
		}
	}
}

/* Function: producer_thread
 * -------------------------
 *  This function is called by a new producer thread when it is created.
 *  
 *  The producer waits for the buffer to contain less than the max number of
 *  items that the buffer can hold and then attempts to get the mutex lock 
 *  before once more checking the buffer. It then produces an item in the
 *  buffer, after which the producer rests for three to seven seconds before
 *  attempting to produce another item. Once this thread has produced
 *  an item or the buffer is full the producer releases the mutex lock.
 */
void* producer_thread()
{
	int rest_time;
	while(true){
		if(items_in_buffer < MAX_ITEMS){
			pthread_mutex_lock(&lock);
			rest_time = 0;
			printf("Producer has mutex lock.\n");
			if(items_in_buffer < MAX_ITEMS){
				buffer[items_in_buffer].consumption_num = random_range(1, 50);
				buffer[items_in_buffer].wait_period = random_range(2, 9);
				items_in_buffer++;
				printf("Producer has created an item.\n");
				rest_time = random_range(3, 7);
				printf("Producer is about to rest for %d seconds.\n", rest_time);
			}
			printf("Producer has released mutex lock.\n\n");
			pthread_mutex_unlock(&lock);
			sleep(rest_time);
		}
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
