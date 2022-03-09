/*************************************************************************
	> File Name: tcp_client.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月09日 星期三 19时19分34秒
 ************************************************************************/

#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFLEN 1024
#define SERVER_PORT 8888

int main(int argc, char** argv) {
	if (argc < 2) return -1;
	int s;
	struct sockaddr_in server;
	char buff[BUFFLEN];
	int n = 0;
	s = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(argv[1]);
	server.sin_port = htons(SERVER_PORT);
	
	connect(s, (struct sockaddr *)&server, sizeof(server));

	memset(buff, 0, BUFFLEN);
	strcpy(buff, "TIME");
	send(s, buff, strlen(buff), 0);

	memset(buff, 0, BUFFLEN);
	n = recv(s, buff, BUFFLEN, 0);
	if (n > 0) {
		printf("%s", buff);
	}
	close(s);
	return 0;
}
