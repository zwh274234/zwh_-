#include <stdio.h>

//void add(int a, int b, int sum)
void add(int a, int b, int *sum)
{
	*sum = a+b;
}

int main(void)
{
	int a = 90;
	int b = 90;
	int sum = 0;
	//add(a, b, sum);
	add(a, b, &sum);
	printf("sum=%d\n", sum);

	return 0;
}
