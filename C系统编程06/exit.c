#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void getStrFromSTDIN(char caBuf[32])
{
	read(STDIN_FILENO, caBuf, 32);
	char *p = strrchr(caBuf, '\n');
	if (NULL != p) 
	{
		*p = '\0';
	}
	else
	{
		while ('\n' != getchar())
		{}
	}
}

void *run(void *arg)
{
	char caBuf[32] = {'\0'};
	int i = 0;
	while (1)
	{
		getStrFromSTDIN(caBuf);
		if (0 == strcmp(caBuf, "exit"))
		{
			pthread_exit((void*)666);
		}
		i++;
		if (5 == i)
		{
			break;
		}
	}
	return (void *)888;
}

int main(void)
{	
	pthread_t thr;
	pthread_create(&thr, NULL, run, NULL);

	void *retval = NULL;
	pthread_join(thr, &retval);
	if ((void*)666 == retval)
	{
		printf("线程是输入exit结束的\n");
	}
	else if ((void *)888 == retval)
	{
		printf("线程是正常结束的\n");
	}

	return 0;
}









