/*************************************************************************
	> File Name: con_pro.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月01日 星期二 17时07分55秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <sys/signal.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int running = 1;
int fd;

void sigint(int sign)
{
	running = 0;
	close(fd);
}


sem_t pro;
sem_t con;
int food = 0;
pthread_mutex_t m;
void P(sem_t *t)
{
	sem_wait(t);
}

void V(sem_t *t)
{
	sem_post(t);
}

void* producer(void *arg)
{
	char buff[512];
	pthread_detach(pthread_self());
	while (running) {
		P(&pro);
		pthread_mutex_lock(&m);
		printf("produce, food = %d\n", ++food);
		sprintf(buff, "produce, food = %d\n", food);
		write(fd, buff, strlen(buff));
		pthread_mutex_unlock(&m);
		V(&con);
	}
	return NULL;
}

void* consumer(void *arg)
{
	char buff[512];
	pthread_detach(pthread_self());
	while (running) {
		P(&con);
		pthread_mutex_lock(&m);
		printf("consume, food = %d\n", --food);
		sprintf(buff, "consume, food = %d\n", food);
		write(fd, buff, strlen(buff));
		pthread_mutex_unlock(&m);
		V(&pro);
	}
	return NULL;
}

int main()
{
	fd = open("./test.txt", O_CREAT|O_RDWR);
	signal(SIGINT, sigint);
	sem_init(&pro, 0, 4);
	sem_init(&con, 0, 0);
	pthread_mutex_init(&m, NULL);
	pthread_t t1, t2;
	pthread_create(&t1, NULL, producer, NULL);
	pthread_create(&t2, NULL, consumer, NULL);
	pthread_exit(NULL);
}
