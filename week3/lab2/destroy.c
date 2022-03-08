/*************************************************************************
	> File Name: destroy.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月01日 星期二 11时01分06秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

int main(int argc, char **argv)
{
	if (argc != 2) {
		return -1;
	}
	msgctl(atoi(argv[1]), IPC_RMID, NULL);
	return 0;
}
