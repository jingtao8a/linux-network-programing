/*************************************************************************
	> File Name: shm.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 13时03分20秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "signal.h"
#include <unistd.h>
#include <sys/wait.h>

static char msg[] = "hello, shared memory\n";


int main()
{
	key_t key;
	int semid, shmid;
	char i, *shms, *shmc;
	int value = 0;
	char buffer[80];
	pid_t p;

	key = ftok("./ipc/sem", 1);
	shmid = shmget(key, 1024, IPC_CREAT);
	if (shmid == -1) {
		perror("shmget()");
		return 0;
	}
	semid = CreateSem(key, 0);
	
	p = fork();
	if (p > 0) {
		shms = (char *)shmat(shmid, 0, 0);
		memcpy(shms, msg, strlen(msg) + 1);
		sleep(4);
		Sem_V(semid);
		//sleep(4);
		shmdt(shms);
		DestroySem(semid);
		wait(NULL);
		printf("father over\n");
	} else if (p == 0){
		shmc = (char *)shmat(shmid, 0, 0);
		Sem_P(semid);
		printf("shared msg: %s", shmc);
		shmdt(shmc);
		printf("son over\n");
	} else {
		perror("fork()");
	}
	return 0;
}
