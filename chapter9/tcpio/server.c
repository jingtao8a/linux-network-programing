/*************************************************************************
	> File Name: server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月23日 星期三 11时26分05秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
extern void sig_proccess(int signo);
#define PORT 8888
#define BACKLOG 2
static int sc;
void sig_proccess(int signo) 
{
	printf("%d catch a exit signal\n", getpid());
	close(sc);
	exit(0);
}

void sig_pipe(int sign)
{
	printf("%d Catch a SIGPIPE signal\n", getpid());
	close(sc);
	exit(0);
}

void process_conn_server(int s)
{
	ssize_t size = 0;
	char buffer[1024];
	while (1) {
		size = recv(s, buffer, 1024, 0);
		if (size == 0) {
			return;
		}
		sprintf(buffer, "%ld bytes altogether\n", size);
		send(s, buffer, strlen(buffer) + 1, 0);
	}
}

int main()
{
	int ss;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int err;
	pid_t pid;
	signal(SIGINT, sig_proccess);
	signal(SIGPIPE, sig_pipe);

	ss = socket(AF_INET, SOCK_STREAM, 0);
	if (ss < 0) {
		printf("socket error\n");
		return -1;
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	err = bind(ss, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (err < 0) {
		printf("bind error\n");
		return -1;
	}

	err = listen(ss, BACKLOG);
	if (err < 0) {
		printf("listen error\n");
		return -1;
	}
	while (1) {
		int addrlen = sizeof(struct sockaddr);
		sc = accept(ss, (struct sockaddr*)&client_addr, &addrlen);
		if (sc < 0) {
			continue;
		}

		pid = fork();
		if (pid == 0) {
			close(ss);
			process_conn_server(sc);
		} else {
			close(sc);
		}
	}
	return 0;
}
