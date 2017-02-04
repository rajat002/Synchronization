/*
Multiplexing : it allows multiple threads to run in the critical section at the same time, but it enforces an upper limit on the number of concurrent threads. In other words, no more than n threads can run in the critical section at the same time.
This pattern is called a multiplex. In real life, the multiplex problem occurs at busy nightclubs where there is a maximum number of people allowed in the building at a time, either to maintain fire safety or to create the illusion of exclusivity.
At such places a bouncer usually enforces the synchronization constraint by keeping track of the number of people inside and barring arrivals when the room is at capacity. Then, whenever one person leaves another is allowed to enter.
Enforcing this constraint with semaphores may sound difficult, but it is almost trivial.
 */


#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>

sem_t signal;
sem_t mutex;
pthread_t tid[20];
int active = 0;

void* foo(void *arg)
{
	int tid = *((int*)arg);
	printf("Thread %d entering\n", tid);
	sem_wait(&signal);
	++active;
	printf("Thread %d processing , active : %d\n", tid, active);
	sleep(1);
	--active;
	sem_post(&signal);
	printf("Thread %d exits\n", tid);
}


int main(void)
{
	int i = 0;
	int err;
	int x[20] = {0};
	sem_init(&mutex, 1, 1); 
	if(sem_init(&signal,1,3) < 0)
	{
		perror("semaphore initilization");
		exit(0);
	}

	while(i < 20)
	{
		x[i] = i;
		err = pthread_create(&(tid[i]), NULL, &foo, &x[i]);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
		i++;
	}

	for (i = 0; i < 20; ++i)
	{
		pthread_join(tid[i], NULL);
	}
	return 0;
}
