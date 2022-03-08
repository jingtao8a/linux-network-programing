/*************************************************************************
	> File Name: signal.h
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 13时07分11秒
 ************************************************************************/

#ifndef SIGNAL_H
#define SIGNAL_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef int sem_t;
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

sem_t CreateSem(key_t key, int value);

int Sem_P(sem_t semid);

int Sem_V(sem_t semid);

void SetvalueSem(sem_t semid, int value);

int GetvalueSem(sem_t semid);

int GetvalueSem(sem_t semid);

void DestroySem(sem_t semid);

#endif
