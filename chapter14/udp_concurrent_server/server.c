/*************************************************************************
	> File Name: server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月09日 星期三 19时34分25秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#define BUFFLEN 1024 
#define SERVER_PORT 6666
#define PIDNUMB 2

static void handle_connect(int s) {
	struct sockaddr_in from;
	socklen_t len = sizeof(from);
	int n = 0;
	char buff[BUFFLEN];
	time_t now;

	while (1) {
		memset(buff, 0, BUFFLEN);
		n = recvfrom(s, buff, BUFFLEN, 0, (struct sockaddr*)&from, &len);
		if (n > 0 && !strncmp(buff, "TIME", 4)) {
			memset(buff, 0, BUFFLEN);
			now = time(NULL);
			sprintf(buff, "%24s\r\n", ctime(&now));
			sendto(s, buff, strlen(buff), 0, (struct sockaddr*)&from, len);
		}
	}
}

void sig_int(int num) {
	exit(1);
}

int main() {
	int s_s;
	struct sockaddr_in local;
	signal(SIGINT, sig_int);
	s_s = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);

	bind(s_s, (struct sockaddr*)&local, sizeof(struct sockaddr));

	pid_t pid[PIDNUMB];

	for (int i = 0; i < PIDNUMB; i++) {
		pid[i] = fork();
		if (pid[i] == 0) {
			handle_connect(s_s);
		}
	}
	while(1);
	return 0;
}
