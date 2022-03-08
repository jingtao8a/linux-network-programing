/*************************************************************************
	> File Name: server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月28日 星期一 14时41分48秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define MCAST_PORT 8888

void *HandleIPFound(void *arg)
{
	#define BUFFER_LEN 32
	int ret = -1;
	int sock = -1;
	struct sockaddr_in local_addr;
	struct sockaddr_in from_addr;
	int from_len;
	int count = -1;
	fd_set readfd;
	char buff[BUFFER_LEN];
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("socket()");
		return NULL;
	}
	memset((void *)&local_addr, 0, sizeof(struct sockaddr_in));

	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(MCAST_PORT);

	ret = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
	if (ret != 0) {
		perror("bind()");
		return NULL;
	}

	while (1) {
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);
		ret = select(sock + 1, &readfd, NULL, NULL, &timeout);
		switch(ret) {
			case -1:
				perror("select");
				exit(1);
			case 0:
				break;
			default:
				if (FD_ISSET(sock, &readfd)) {
					count = recvfrom(sock, buff, BUFFER_LEN, 0, (struct sockaddr*)&from_addr, &from_len);
					if (strstr(buff, IP_FOUND)) {
						memcpy(buff, IP_FOUND_ACK, strlen(IP_FOUND_ACK) + 1);
						count = sendto(sock, buff, BUFFER_LEN, 0, (struct sockaddr*)&from_addr, from_len);
					}
				}
				break;
		}
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, HandleIPFound, NULL);
	pthread_join(tid, NULL);
	return 0;
}
