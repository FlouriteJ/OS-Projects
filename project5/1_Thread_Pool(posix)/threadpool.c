/**
 * Implementation of thread pool.
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define REAL_QUEUE_SIZE (QUEUE_SIZE+1)
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool
typedef struct 
{
    void (*function)(void *p);
    void *data;
}
task;

// the stop token
int token = TRUE;

// the work queue
task works[REAL_QUEUE_SIZE];
int head = 0;
int tail = 0;

// the worker bee
pthread_t bees[NUMBER_OF_THREADS];

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
	if ((tail+1)%(REAL_QUEUE_SIZE) == head){
		printf("the work queue is full. \n");
		return 1;
	}
	else{
		works[tail] = t;
		tail = (tail+1)%(REAL_QUEUE_SIZE);
		return 0;
	}
}

// remove a task from the queue
// assume the calling is valid
task dequeue() 
{
	task worktodo;
	worktodo = works[head];
	head = (head+1)%(REAL_QUEUE_SIZE);
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    // execute the task
	task worktodo;
	while(token){
		if (head==tail){
			usleep(1); //usecond
		}
		else{
			worktodo = dequeue();
			execute(worktodo.function, worktodo.data);
		}
	}
    pthread_exit(0);
}

/**
 * Executes the task provided to the thread pool
 */
void execute(void (*somefunction)(void *p), void *p)
{
    (*somefunction)(p);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
	task worktodo;
    worktodo.function = somefunction;
    worktodo.data = p;
	enqueue(worktodo);
	return 0;
}

// initialize the thread pool
void pool_init(void)
{
	int i = 0;
	while(i<NUMBER_OF_THREADS){
		pthread_create(&(bees[i]),NULL,worker,NULL);
		i++;
	}
}

// shutdown the thread pool
void pool_shutdown(void)
{
	int i = 0;
	while(TRUE){
		if (head==tail){
			token = 0;
			break;
		}
		else{
			usleep(1); //usecond
		}
	}
	while(i<NUMBER_OF_THREADS){
		pthread_join(bees[i],NULL);
		i++;
	}
}
