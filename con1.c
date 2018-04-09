// Concurrency 1
// CS444 Spring2018 
// Name: Zachary Thomas
// Email: thomasza@oregonstate.edu
// Date: 4/09/2018
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>

struct item_in_buffer {
	int consumption_num;
	int wait_period;
}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Create a mutex lock.

int main (int argc, char **argvi) {
	//Buffer should hold up to 32 items. It must be implicitly shared between the threads. 
	//producer should wait 3-7 seconds before producing a new item.
	//use whatever synchronization construct you feel is appropriate.
	
}
