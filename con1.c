// Concurrency 1
// CS444 Spring2018 
// Name: Zachary Thomas
// Email: thomasza@oregonstate.edu
// Date: 4/09/2018
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>

void* consumer_thread();
void* producer_thread();

struct item_in_buffer {
	int consumption_num;
	int wait_period;
};

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Create a mutex lock.

int main(int argc, char **argv)
{

	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;

	//create a producer and consumer thread.	
	printf("I am making threads\n");
	pthread_t eat_t;
	pthread_create( &eat_t, NULL, producer_thread, NULL); 

	// Join thread.
 	pthread_join(eat_t, NULL);
	 		
	// Destroy mutex lock.	
	pthread_mutex_destroy(&lock);


	char vendor[13];
	
	eax = 0x01;

	__asm__ __volatile__(
	                     "cpuid;"
	                     : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
	                     : "a"(eax)
	                     );
	
	if(ecx & 0x40000000){
		rand(time(0)); //use rdrand
	} else {
		//use mt19937
	}
	
	return 0;
}

void* consumer_thread()
{
	printf("I am a consumer_thread");
}

void* producer_thread()
{
	printf("I am a producer_thread");
}
