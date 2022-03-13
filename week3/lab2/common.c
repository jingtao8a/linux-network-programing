/*************************************************************************
	> File Name: common.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月01日 星期二 16时08分18秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define TEXT_SIZE 1024

struct msgbuf{
	long mtype;
	char mtext[TEXT_SIZE];
};

static struct msgbuf msg;

int createmsgid()
{
	int msgid = msgget((key_t)1234, IPC_CREAT|0666);
	if (msgid < 0) {
		perror("msgget");
		return -1;
	}
	return msgid;
}

int msgsend(int msgid, long types, char* buffer)
{
	strcpy(msg.mtext, buffer);
	msg.mtype = types;
	int ret = msgsnd(msgid, (void *)&msg, TEXT_SIZE, 0);
	if (ret < 0) {
		perror("msgsnd");
		return -1;
	}
	return 0;
}

int msgreceive(int msgid, long types, char* buffer)
{
	msg.mtype = types;
	ssize_t size = msgrcv(msgid, (void *)&msg, TEXT_SIZE, msg.mtype, 0);
	if (size == 0) {
		perror("msgrcv");
		return -1;
	}
	strcpy(buffer, msg.mtext);
	return size;
}
