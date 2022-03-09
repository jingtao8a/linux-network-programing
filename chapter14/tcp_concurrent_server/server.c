/*************************************************************************
	> File Name: server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月09日 星期三 20时10分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <arpa/inet.h>

#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5
#define PIDNUMB 3

static void handle_connect(int s_s) {
	int s_c;
	struct sockaddr_in from; 
	socklen_t len;
	while (1) {
		s_c = accept(s_s, (struct sockaddr*)&from, &len);
		time_t now;
		char buff[BUFFLEN];
		memset(buff, 0, BUFFLEN);
		int n = recv(s_c, buff, BUFFLEN, 0);
		if (n > 0 && !strncmp(buff, "TIME", 4)) {
			memset(buff, 0, BUFFLEN);
			now = time(NULL);
			sprintf(buff, "%24s\r\n", ctime(&now));
			send(s_c, buff, strlen(buff), 0);
		}
		close(s_c);
	}
}

void sig_int(int num) {
	exit(1);
}

int main() {
	signal(SIGINT, sig_int);
	int s_s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);

	bind(s_s, (struct sockaddr*)&local, sizeof(local));
	listen(s_s, BACKLOG);
	
	pid_t pid[PIDNUMB];
	for (int i = 0; i < PIDNUMB; ++i) {
		pid[i] = fork();
		if (pid[i] == 0) {
			handle_connect(s_s);
		}
	}

	while(1);
	close(s_s);
	return 0;
}

