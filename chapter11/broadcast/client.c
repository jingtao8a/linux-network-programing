/*************************************************************************
	> File Name: client.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月28日 星期一 15时05分44秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#define IP_FOUND "IP_FOUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"
#define MCAST_PORT 8888
#define IFNAME "ens33"

void* IPFound(void *arg)
{
	#define BUFFER_LEN 32
	int ret = -1;
	int sock = -1;
	int so_broad_cast = 1;
	struct ifreq ifr;
	struct sockaddr_in broadcast_addr;
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
	strncpy(ifr.ifr_name, IFNAME, strlen(IFNAME));
	if (ioctl(sock, SIOCGIFBRDADDR, &ifr) == -1) {
		perror("ioctl error");
		exit(1);
	}
	memcpy(&broadcast_addr, &ifr.ifr_broadaddr, sizeof(struct sockaddr_in));
	broadcast_addr.sin_port = htons(MCAST_PORT);
	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &so_broad_cast, sizeof(int));

	int times = 10;
	int i;
	for (i = 0; i < times; ++i) {
		ret = sendto(sock, IP_FOUND, strlen(IP_FOUND), 0, (struct sockaddr*)&broadcast_addr, sizeof(struct sockaddr));
		if (ret == -1) {
			continue;
		}
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);
		ret = select(sock + 1, &readfd, NULL, NULL, &timeout);
		switch(ret) {
			case -1:
				break;
			case 0:
				break;
			default:
				if (FD_ISSET(sock, &readfd)) {
					count = recvfrom(sock, buff, BUFFER_LEN, 0, (struct sockaddr*)&from_addr, &from_len);
					if (strstr(buff, IP_FOUND_ACK)) {
						printf("found server, IP is %s\n", inet_ntoa(from_addr.sin_addr));
					}
				}
				break;
		}
	}

}
int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, IPFound, NULL);
	pthread_join(tid, NULL);
	return 0;
}
