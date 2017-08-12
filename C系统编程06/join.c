#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *run(void *arg)
{
	return (void *)888;
}

int main(void)
{	
	pthread_t thr;
	pthread_create(&thr, NULL, run, NULL);

	void *retval = NULL;
	pthread_join(thr, &retval);
	printf("retval=%d\n", (int)retval);

	return 0;
}









