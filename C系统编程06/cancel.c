#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *run(void *arg)
{
	while (1)
	{
		printf("this is thread run\n");
		sleep(1);
	}
}

int main(void)
{	
	pthread_t thr;
	pthread_create(&thr, NULL, run, NULL);

	int i = 0;
	while (1)
	{
		printf("this is main while\n");
		sleep(1);
		i++;
		if (5 == i)
		{
			//向指定线程发送结束请求
			pthread_cancel(thr);
			break;
		}
	}

	pthread_join(thr, NULL);

	return 0;
}









