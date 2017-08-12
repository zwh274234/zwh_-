#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *run(void *arg)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE
					       , NULL);
	int i = 0;
	while (1)
	{
		printf("this is thread run\n");
		sleep(1);
		i++;
		if (5 == i)
		{
			pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		}
	}
}

int main(void)
{	
	pthread_t thr;
	pthread_create(&thr, NULL, run, NULL);

	pthread_cancel(thr);
	
	pthread_join(thr, NULL);

	return 0;
}









