/*************************************************************************
	> File Name: thread.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 15时31分29秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int run = 1;
int retvalue;

void* start_routine(void *arg)
{
	int *running = arg;
	printf("arg: %d\n", *running);

	while (*running) {
		printf("pthread running\n");
		usleep(1);
	}
	printf("pthread exit\n");
	retvalue = 8;
	pthread_exit((void *)(&retvalue));
}

int main()
{
	pthread_t pt;
	int ret = -1;
	int times = 3;
	int i = 0;
	int *ret_join = NULL;
	ret = pthread_create(&pt, NULL, (void *)start_routine, &run);
	if (ret != 0) {
		perror("pthread_create()");
		return 1;
	}
	usleep(1);
	for (; i < times; i++) {
		printf("main thread\n");
		usleep(1);
	}
	run = 0;
	pthread_join(pt, (void*)&ret_join);
	printf("thread retvalue: %d\n", *ret_join);
	return 0;
}
