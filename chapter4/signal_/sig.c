/*************************************************************************
	> File Name: sig.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 14时17分15秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

typedef void (*sighandler_t) (int);

void sig_handle(int signo)
{
	if (SIGSTOP == signo) {
		printf("receive SIGSTOP\n");
	} else if (SIGKILL == signo) {
		printf("receive SIGKILL\n");
	} else {
		printf("receive %d\n", signo);
	}
	return;
}

int main()
{
	printf("pid = %d\n", getpid());
	sighandler_t ret;
	ret = signal(SIGINT, (sighandler_t)sig_handle);
	if (ret == SIG_ERR) {
		perror("signal()");
		return -1;
	} else {
		printf("sigint hang success\n");
	}

	for(;;);
}

