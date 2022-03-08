/*************************************************************************
	> File Name: pselect.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月24日 星期四 09时18分56秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>

void sigint_sig_handler(int s)
{
	printf("SIGINT\n");
}

int main()
{
	fd_set rd;
	signal(SIGINT, sigint_sig_handler);
	FD_ZERO(&rd);
	FD_SET(0, &rd);
	
	sigset_t sigmask, origmask;
	sigemptyset(&sigmask);
	sigaddset(&sigmask, SIGINT);
	sigprocmask(SIG_BLOCK, &sigmask, &origmask);
	int ret = select(1, &rd, NULL, NULL, NULL);
	if (ret < -1) {
		perror("select");
		return -1;
	} else if (ret > 0) {
		printf("data come\n");
	} else {
		printf("no data\n");
	}
	return 0;
}
