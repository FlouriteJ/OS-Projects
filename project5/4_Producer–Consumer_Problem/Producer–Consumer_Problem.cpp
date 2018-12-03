#include <stdlib.h> /* required for rand() */
#include <stdio.h>
#include <thread> 
#include <mutex> 
#include <Windows.h>
using namespace std;
#define random() ((double(rand())/RAND_MAX))

mutex mtx;

class counting_semaphores {
	int count;
	int max_count;
	mutex _mtx;
public:	
	counting_semaphores(int mc){
		max_count = mc;
		count = 0;
	}
	void wait(){
		while(true){
			_mtx.lock();
			if (count == max_count){
				Sleep(0.01);
				_mtx.unlock();
			}
			else{
				break;
			}
		}
		count++;
		_mtx.unlock();
	}
	void signal(){
		_mtx.lock();
		count--;
		_mtx.unlock();		
	}
}semaphores(1);


/* the buffer */
typedef int buffer_item;
#define BUFFER_SIZE 5
#define REAL_QUEUE_SIZE (BUFFER_SIZE+1)


buffer_item buffer[REAL_QUEUE_SIZE];
int head = 0;
int tail = 0;

int insert_item(buffer_item item) {
	if ((tail+1)%(REAL_QUEUE_SIZE) == head){
		return 1;
	}
	else{
		buffer[tail] = item;
		tail = (tail+1)%(REAL_QUEUE_SIZE);
		return 0;
	}	
}
int remove_item(buffer_item *item) {
	if(tail==head){
		//printf("the queue is empty. \n");
		return 1;
	}
	else{
		*item = buffer[head]; //*** rather than item = &buffer[head] ***
		head = (head+1)%(REAL_QUEUE_SIZE);
		return 0;	
	}
}

void *producer(void *param) {
	buffer_item item;
	while (true) {
		/* sleep for a random period of time */
		Sleep(random()*10);
		/* generate a random number */
		item = rand();
		
		//mutex lock version
		mtx.lock();
		int e = insert_item(item);
		mtx.unlock();
		
		//counting semaphores version
/* 		semaphores.wait();
		int e = insert_item(item);
		semaphores.signal(); */
		
		if (e)
			printf("report error condition (queue is full) \n");
		else
			printf("producer produced %d \n",item);
	}
}

void *consumer(void *param) {
	buffer_item item;
	while (true) {
		/* sleep for a random period of time */
		Sleep(random()*10);

		//mutex lock version		
		mtx.lock();
		int e = remove_item(&item);
		mtx.unlock();		
		//counting semaphores version
/* 		semaphores.wait();
		int e = remove_item(&item);
		semaphores.signal();	 */	
		if (e)
			printf("report error condition (queue is empty) \n");
		else
			printf("consumer consumed %d \n",item);
	}
}

int main(int argc, char *argv[]) {
	/* 1. Get command line arguments argv[1],argv[2],argv[3] */
	/* 2. Initialize buffer */
	/* 3. Create producer thread(s) */
	thread t_producer(producer,(void*)(NULL));
	/* 4. Create consumer thread(s) */
	thread t_consumer(consumer,(void*)(NULL));
	/* 5. Sleep */
	Sleep(300);
	/* 6. Exit */
}