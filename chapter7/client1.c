/*************************************************************************
	> File Name: client.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月22日 星期二 16时58分25秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"
#define DEST_IP "192.168.223.128"
#define DEST_PORT 8888

int main()
{
	int ret = 0;
	int sockfd;
	struct sockaddr_in server;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(DEST_PORT);
	server.sin_addr.s_addr = my_inet_addr(DEST_IP);

	ret = connect(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr));
	if (ret == -1) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	close(sockfd);
	return 0;
}
