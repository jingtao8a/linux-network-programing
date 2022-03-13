/*************************************************************************
	> File Name: A.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月01日 星期二 16时07分35秒
 ************************************************************************/

#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
static int msgid;
static int running;

static void sigint(int sign)
{
	msgctl(msgid, IPC_RMID, NULL);
	exit(1);
}

static void* thread(void* arg) {
	pthread_detach(pthread_self());
	char buff[1024];
	while (running) {
		memset(buff, 0, 1024);
		msgreceive(msgid, 10, buff);
		printf("\n\nB say: %s\n", buff);
		fflush(stdout);
		if (!strncmp("quit", buff, 4)) {
			exit(0);
		}
		printf("please input:");
		fflush(stdout);
	}
}

int main()
{
	signal(SIGINT, sigint);
	running = 1;
	msgid = createmsgid();
	pthread_t t;
	pthread_create(&t, NULL, thread, NULL);
	char buff[1024];
	while (running) {
		printf("please input:");
		fflush(stdout);
		memset(buff, 0, 1024);
		read(0, buff, 1024);
		msgsend(msgid, 1, buff);
	}
	return 0;
}
