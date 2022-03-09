/*************************************************************************
	> File Name: tcp_server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月09日 星期三 19时04分03秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <time.h>
#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5

int main(int argc, char** argv) {
	int s_s, s_c;
	struct sockaddr_in local, from;

	time_t now;
	char buff[BUFFLEN];
	int n = 0;
	socklen_t len = sizeof(from);

	s_s = socket(AF_INET, SOCK_STREAM, 0);

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);

	bind(s_s, (struct sockaddr*)&local, sizeof(local));

	listen(s_s, BACKLOG);

	while (1) {
		s_c = accept(s_s, (struct sockaddr*)&from, &len);
		memset(buff, 0, BUFFLEN);
		n = recv(s_c, buff, BUFFLEN, 0);
		if (n > 0 && !strncmp(buff, "TIME", 4)) {
			memset(buff, 0, BUFFLEN);
			now = time(NULL);
			sprintf(buff, "%24s\r\n", ctime(&now));
			send(s_c, buff, strlen(buff), 0);
		}
		close(s_c);
	}
	return 0;
}
