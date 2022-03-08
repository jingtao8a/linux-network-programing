/*************************************************************************
	> File Name: common.h
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月01日 星期二 16时08分14秒
 ************************************************************************/

#ifndef COMMON_H
#define COMMON_H

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

int createmsgid();

int msgsend(int msgid, long types, const char* buffer);

int msgreceive(int msgid, long types, char *buffer);

#endif
