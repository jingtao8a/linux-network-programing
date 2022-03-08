/*************************************************************************
	> File Name: B.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月01日 星期二 16时22分00秒
 ************************************************************************/

#include "common.h"
#include <sys/signal.h>

int msgid;

void sigint(int sign)
{

	msgctl(msgid, IPC_RMID, NULL);
	exit(1);
}

int main()
{
	signal(SIGINT, sigint);
	int running = 1;
	msgid = createmsgid();
	char buff[1024];
	while (running) {
		printf("A say:");
		fflush(stdout);
		msgreceive(msgid, 1, buff);
		printf("%s", buff);
		if (strcmp(buff, "q\n") == 0) {
			break;
		}
		printf("B:");
		fflush(stdout);
		read(0, buff, 1024);
		msgsend(msgid, 10, buff);
	}
	return 0;
}

