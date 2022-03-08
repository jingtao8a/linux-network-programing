/*************************************************************************
	> File Name: thread_signal.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 16时19分10秒
 ************************************************************************/

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

void *producer_f(void *arg);
void *consumer_f(void *arg);



sem_t sem;
int running = 1;
pthread_mutex_t mutex;

int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_t consumer_t, producer_t;
	sem_init(&sem, 0, 16);
	pthread_create(&producer_t, NULL, (void *)producer_f, NULL);
	pthread_create(&consumer_t, NULL, (void *)consumer_f, NULL);
	
	sleep(1);
	running = 0;
	
	pthread_join(producer_t, NULL);
	pthread_join(consumer_t, NULL);
	sem_destroy(&sem);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void *producer_f(void *arg)
{
	int semval = 0;
	while (running) {
		usleep(1);
		sem_post(&sem);
		pthread_mutex_lock(&mutex);
		sem_getvalue(&sem, &semval);
		printf("producer, num: %d\n", semval);
		pthread_mutex_unlock(&mutex);
	}
}

void *consumer_f(void *arg)
{
	int semval = 0;
	while (running) {
		usleep(1);
		sem_wait(&sem);
		pthread_mutex_lock(&mutex);
		sem_getvalue(&sem, &semval);
		printf("consumer, num: %d\n", semval);
		pthread_mutex_unlock(&mutex);
	}
}
