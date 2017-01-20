/*
3.3 Rendezvous
Puzzle: Generalize the signal pattern so that it works both ways. Thread A has to wait for Thread B and vice versa. In other words, given this code
Thread A
1 statement a1
2 statement a2

Thread B
1 statement b1 
2 statement b2

we want to guarantee that a1 happens before b2 and b1 happens before a2. In writing your solution, be sure to specify the names and initial values of your semaphores (little hint there).
Your solution should not enforce too many constraints. For example, we don’t care about the order of a1 and b1. In your solution, either order should be possible.
This synchronization problem has a name; it’s a rendezvous. The idea is that two threads rendezvous at a point of execution, and neither is allowed to proceed until both have arrived.
*/



#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>

sem_t signala1;
sem_t signalb1;
pthread_t tid[2];

void* foo(void *arg)
{
	printf("statement a1\n");
	sem_post(&signala1);
	sem_wait(&signalb1);
	printf("statement a2\n");
	return NULL;
}


void* bar(void *arg)
{
	printf("statement b1\n");
	sem_post(&signalb1);
	sem_wait(&signala1);
	printf("statement b2\n");
	return NULL;
}

int main(void)
{
	int i = 0;
	int err;
	void* (*p[2])(void*);
	p[0] = &foo;
	p[1] = &bar;

	sem_init(&signala1,1,0);
	sem_init(&signalb1,1,0);

	while(i < 2)
	{
		err = pthread_create(&(tid[i]), NULL, p[i], NULL);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
		i++;
	}
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	return 0;
}
