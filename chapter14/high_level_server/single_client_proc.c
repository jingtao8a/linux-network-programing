/*************************************************************************
	> File Name: single_client_proc.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月10日 星期四 11时01分59秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <time.h>

#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5

static void handle_request(int s_c) {
	time_t now;
	char buff[BUFFLEN];
	int n = 0;
	memset(buff, 0, BUFFLEN);
	n = recv(s_c, buff, BUFFLEN, 0);
	if (n > 0 && !strncmp(buff, "TIME", 4)) {
		now = time(NULL);
		sprintf(buff, "%24s\r\n", ctime(&now));
		send(s_c, buff, strlen(buff), 0);
	}
	close(s_c);
}

static int handle_connect(int s_s) {
	int s_c;
	struct sockaddr_in from;
	socklen_t len;
	while (1) {
		s_c = accept(s_s, (struct sockaddr*)&from, &len);
		pid_t pid = fork();
		if (pid == 0) {
			handle_request(s_c);
			return 0;
		} else {
			close(s_c);
		}
	}
}

int main() {
	int s_s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);
	
	bind(s_s, (struct sockaddr*)&local, sizeof(struct sockaddr));
	listen(s_s, BACKLOG);
	
	handle_connect(s_s);
	close(s_s);
	return 0;
}
