/*************************************************************************
	> File Name: sem.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 11时19分02秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef int sem_t;
union semun{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

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
	struct sembuf sops = {0, +1, IPC_NOWAIT};
	return (semop(semid, &sops, 1));
}

int Sem_V(sem_t semid)
{
	struct sembuf sops = {0, -1, IPC_NOWAIT};
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

int main()
{
	key_t key;
	int semid;
	char i;
	int value = 0;
	
	key = ftok("./ipc/sem", 'b');
	if (key == -1) fprintf(stderr, "ftok error\n");

	semid = CreateSem(key, 100);
	if (semid == -1) fprintf(stderr, "CreateSem error\n");
	
	for (i = 0; i < 3; i++) {
		Sem_P(semid);
		Sem_V(semid);
	}

	value = GetvalueSem(semid);
	printf("sem value: %d \n", value);
	DestroySem(semid);
	return 0;
}
