/*
Possibly the simplest use for a semaphore is signaling, which means that one thread sends a signal to another thread to indicate that something has happened. Signaling makes it possible to guarantee that a section of code in one thread will run before a section of code in another thread; in other words, it solves the
serialization problem.
*/

//bar is guaranteed to print before foo

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>

sem_t signal;
pthread_t tid[2];

void* foo(void *arg)
{
    sem_wait(&signal);
    printf("foo\n");
    return NULL;
}


void* bar(void *arg)
{
    int x = 0;
    for(int i= 0; i <1000000;++i)
	x++;
    printf("bar\n");
    sem_post(&signal);
    return NULL;
}

int main(void)
{
    int i = 0;
    int err; 
    void* (*p[2])(void*);
    p[0] = &foo;
    p[1] = &bar;

    if(sem_init(&signal,1,0) < 0)
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
    return 0;
}
