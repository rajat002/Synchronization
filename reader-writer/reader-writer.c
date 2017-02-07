/*In this classic Reader-Writer problem, there are two threads exchanging information
through a fixed size buffer. The Writer thread fills the buffer with data whenever there's
room for more. The Reader thread reads data from the buffer and prints it. Both
threads have a situation where they should block. The writer blocks when the buffer is
full and the reader blocks when the buffer is empty. The problem is to get them to
cooperate nicely and block efficiently when necessary.
For this problem, we will use "generalized semaphores" where the value can be any nonnegative
number. Zero still means "locked" and any other value means "available". The
code cannot look at the value of a generalized semaphore explicitly, you can only call
SemaphoreWait and SemaphoreSignal which in turn depend on the value.
There is a shared, fixed-size buffer. The reader reads starting at readPt and the writer
writes at writePt. No locks are required to protect these integers because only one
thread concerns itself with either. The semaphores ensure that the writer only writes at
writePt when there is space available and similarly for the reader and readPt. This
program is written using no global variables, but instead declaring the variables in main
and passing their address to the new threads.
*/

/**
 * readerWriter.c
 * --------------
 * The canonical consumer-producer example. This version has just one reader
 * and just one writer (although it could be generalized to multiple readers/
 * writers) communicating information through a shared buffer. There are two
 * generalized semaphores used, one to track the num of empty buffers, another
 * to track full buffers. Each is used to count, as well as control access.
 */


#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<time.h>
#define BUF_SIZE 5

const char *random_str = "ZmB411V4roFIlYofiKHp2VhulDPGeYtgo2DTzbo8FJF9PXgReO5TtzSHooOG4frPkPIkRWenxYY5HanS0JDNo0tJ2smTqYtLMXnzjWovhnI2NFQnJkhcMfcDYqLkgt1pI0Xx5heTPYmRKnmRZtThX5Rf4zWLVi3h7jvBWAbxu7cK3QXF4qLUWIeTELF78EA2Xq9oK2eN";
int ri = 0;
volatile int done = 0;
sem_t fullBufLock, emptyBufLock;


void random_sleep(unsigned long seed)
{
        time_t t;
        struct timespec tim, rem;
        srand((unsigned) time(&t));
        tim.tv_sec = 0;
        tim.tv_nsec = rand() % seed;
        //tim.tv_nsec = rand() % 1000000;
        nanosleep(&tim , &rem);

}

void* producer(void *arg)
{
	char *buf = (char*)arg;
	int index = 0;
	int i = 0;
	while(i < strlen(random_str))
	{
		random_sleep(100000);
		sem_wait(&emptyBufLock);
		buf[index] = random_str[ri++];
		printf("produced => %c \n", buf[index]);
		sem_post(&fullBufLock);
		index = (index + 1) % BUF_SIZE;
		++i;
	}

}



void* consumer(void *arg)
{
	char *buf = (char*)arg;
	int index = 0;
	int i = 0;
	while(i < strlen(random_str))
	{
		random_sleep(1000000000);
		sem_wait(&fullBufLock);
                printf("\t\tconsumed => %c \n",buf[index]);
                sem_post(&emptyBufLock);
                index = (index + 1) % BUF_SIZE;
		++i;
	}		

}

int main(void)
{
        pthread_t tid[2];
        int err;
	char buf[BUF_SIZE] = {0};
        sem_init(&emptyBufLock, 1, BUF_SIZE/*initial value*/) ;
        sem_init(&fullBufLock, 1, 0/*initial value*/) ;

	err = pthread_create(&(tid[0]), NULL, producer, buf);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));

	err = pthread_create(&(tid[0]), NULL, consumer, buf);
	if (err != 0)
		printf("\ncan't create thread :[%s]", strerror(err));

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	return 0;
}

