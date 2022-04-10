#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

void *print(void *d)
{
	int i=*(int *)d;
	while (1)
		printf("%d%d%d%d%d%d%d %d\n", i,i,i,i,i,i,i,i);
}

int main()
{
	pthread_t tid[13];
	int i=-1;
	int id[13];
	id[0] = 0;
	id[1] = 1;
	id[2] = 2;
	while(++i < 13)
	{
		id[i] = i;
		pthread_create(&tid[i], NULL, print, &id[i]);
	}
	while (--i > -1)
		pthread_join(tid[i], NULL);
}


