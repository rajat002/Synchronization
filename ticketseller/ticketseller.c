/*
 * A very simple example of a critical section that is protected by a
 * semaphore lock. There is a global variable numTickets which tracks the
 * number of tickets remaining to sell. We will create many threads that all
 * will attempt to sell tickets until they are all gone. However, we must
 * control access to this global variable lest we sell more tickets than
 * really exist. We have a semaphore lock that will only allow one seller
 * thread to access the numTickets variable at a time. Before attempting to
 * sell a ticket, the thread must acquire the lock by waiting on the semaphore
 * and then release the lock when through by signalling the semaphore.
*/

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<time.h>

#define NUM_TICKETS 35
#define NUM_SELLERS 4


sem_t ticketLock;
int ticket_count = 0;

void random_sleep();


void* sell_ticket(void *arg)
{
	static int seller;
	int s =	seller++;
	int ticketsSold = 0;
	while(1)
	{
		sem_wait(&ticketLock);
		if (++ticket_count > NUM_TICKETS)
		{
			sem_post(&ticketLock);
			break;
		}
		else {
			ticketsSold++;
			printf("seller %d => selling ticket %d\n", s, ticket_count);
		}
		sem_post(&ticketLock);
		random_sleep();
	}	
	printf("seller %d sold %d tickets\n", s,ticketsSold);
	return NULL;
}


void random_sleep()
{
	time_t t;
	struct timespec tim, rem;
	srand((unsigned) time(&t));
	tim.tv_sec = 0;
	tim.tv_nsec = rand() % 1000000000;
	nanosleep(&tim , &rem);

}
int main(void)
{
	pthread_t tid[NUM_SELLERS];
	int err;

	if(sem_init(&ticketLock, 1, 1/*initial value*/) < 0)
	{
		perror("semaphore initilization");
		exit(0);
	}

	for(int i = 0; i < NUM_SELLERS; ++i)
	{
		err = pthread_create(&(tid[i]), NULL, sell_ticket, NULL);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
	}

	for(int i = 0; i < NUM_SELLERS; ++i)
	{
		pthread_join(tid[i], NULL);
	}
	return 0;
}

