/*************************************************************************
	> File Name: tcp1.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月21日 星期一 15时34分21秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h" 


int main()
{
	int sfd;
	struct sockaddr_in addr;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd == -1) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons((unsigned short)8888);
	addr.sin_addr.s_addr = my_inet_addr("192.168.223.128");
	
	if (bind(sfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(sfd, 5) == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in client_addr;
	int addr_length = sizeof(struct sockaddr_in);
	int client_fd = accept(sfd, (struct sockaddr *)&client_addr, &addr_length);

	if (client_fd == -1) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	close(client_fd);

	close(sfd);
	return 0;
}
