/*************************************************************************
	> File Name: thread_mutex.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 15时58分17秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

void *producer_f(void *arg);
void *consumer_f(void *arg);

int running = 1;
pthread_mutex_t mutex;
int buffer_has_item = 0;

int main()
{
	pthread_t consumer_t, producer_t;

	pthread_mutex_init(&mutex, NULL);
	pthread_create(&producer_t, NULL, (void *)producer_f, NULL);
	pthread_create(&consumer_t, NULL, (void *)consumer_f, NULL);

	sleep(10);
	running = 0;
	pthread_join(consumer_t, NULL);
	pthread_join(producer_t, NULL);
	pthread_mutex_destroy(&mutex);

	return 0;
}

void *producer_f(void *arg)
{
	while (running) {
		pthread_mutex_lock(&mutex);
		sleep(1);
		buffer_has_item++;
		printf("produce num: %d\n", buffer_has_item);
		pthread_mutex_unlock(&mutex);
	}
}

void *consumer_f(void *arg)
{
	while (running) {
		pthread_mutex_lock(&mutex);
		//sleep(1);
		buffer_has_item--;
		printf("consume num: %d\n", buffer_has_item);
		pthread_mutex_unlock(&mutex);
	}
}

