/*
A second common use for semaphores is to enforce mutual exclusion. We have al- ready seen one use for mutual exclusion, controlling concurrent access to shared variables. The mutex guarantees that only one thread accesses the shared vari- able at a time.
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>

sem_t mutex;
pthread_t tid[2];

int count = 0;
void* foo(void *arg)
{
	int i=0; 
	for(;i<1000000;++i)
	{

		sem_wait(&mutex);
		count++;
		sem_post(&mutex);
	}
	return NULL;
}


int main(void)
{
	int i = 0;
	int err;
	void* (*p[2])(void*);
	p[0] = &foo;
	p[1] = &foo;

	if(sem_init(&mutex,1, 1/*initial value*/) < 0)
	{
		perror("semaphore initilization");
		exit(0);
	}

	while(i < 2)
	{
		err = pthread_create(&(tid[i]), NULL, p[i], NULL);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
		i++;
	}
	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
        printf("count : %d\n", count);
	return 0;
}

