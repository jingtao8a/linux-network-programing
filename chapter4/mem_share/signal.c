/*************************************************************************
	> File Name: signal.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 13时07分07秒
 ************************************************************************/

#include "signal.h"
union semun arg;
sem_t CreateSem(key_t key, int value)
{
	union semun sem;
	sem_t semid;
	sem.val = value;

	semid = semget(key, 1, IPC_CREAT);
	if (semid == -1) {
		perror("segmet()");	
		return -1;
	}

	semctl(semid, 0, SETVAL, sem);
	return semid;
}

int Sem_P(sem_t semid)
{
	struct sembuf sops = {0, -1, SEM_UNDO};
	return (semop(semid, &sops, 1));
}

int Sem_V(sem_t semid)
{
	struct sembuf sops = {0, +1, SEM_UNDO};
	return (semop(semid, &sops, 1));
}

void SetvalueSem(sem_t semid, int value)
{
	union semun sem;
	sem.val = value;
	semctl(semid, 0, SETVAL, sem);
}

int GetvalueSem(sem_t semid)
{
	union semun sem;
	return semctl(semid, 0, GETVAL, sem);
}

void DestroySem(sem_t semid)
{
	union semun sem;
	sem.val = 0;
	semctl(semid, 0, IPC_RMID, sem);
}
