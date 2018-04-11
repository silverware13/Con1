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

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //create a mutex lock

int main(int argc, char **argv)
{
	//create a producer and consumer thread	
	pthread_t pro_t, con_t;
	pthread_create( &pro_t, NULL, producer_thread, NULL);
	sleep(1); 
	pthread_create( &con_t, NULL, consumer_thread, NULL); 
	printf("Made producer and consumer threads.\n");

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
	printf("I am a consumer_thread\n");
	
	//lets take some items out of the buffer.
	while(items_in_buffer > 0) {
		items_in_buffer--;
		printf("%d\n",buffer[items_in_buffer].consumption_num);
		printf("%d\n",buffer[items_in_buffer].wait_period);
	}
}

void* producer_thread()
{
	printf("I am a producer_thread\n");
	
	//lets put some items in the buffer.
	for(int i = 0; i < 5; i++) {
		if(items_in_buffer < MAX_ITEMS) {
			buffer[items_in_buffer].consumption_num = items_in_buffer;
			buffer[items_in_buffer].wait_period = items_in_buffer;
			items_in_buffer++;
		}
	}
}
