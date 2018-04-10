// Concurrency 1
// CS444 Spring2018 
// Name: Zachary Thomas
// Email: thomasza@oregonstate.edu
// Date: 4/09/2018
//

#include <stdio.h>
#include <string.h>
#include <pthread.h>

struct item_in_buffer {
	int consumption_num;
	int wait_period;
}

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Create a mutex lock.

int main(int argc, char **argv)
{

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
	} else {
		//use mt19937
	}
	
	return 0;
}
