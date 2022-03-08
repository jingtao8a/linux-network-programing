/*************************************************************************
	> File Name: server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月28日 星期一 16时16分38秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MCAST_PORT 8888
#define MCAST_ADDR "224.0.0.88"
#define MCAST_DATA "BROADCAST TEST DATA"
#define MCAST_INTERVAL 5

int main()
{
	int s;
	struct sockaddr_in mcast_addr;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == -1) {
		perror("socket()");
		return -1;
	}
	memset(&mcast_addr, 0, sizeof(struct sockaddr_in));
	mcast_addr.sin_family = AF_INET;
	mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	mcast_addr.sin_port = htons(MCAST_PORT);

	while (1) {
		int n = sendto(s, MCAST_DATA, strlen(MCAST_DATA), 0, (struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
		if (n < 0) {
			perror("sendto()");
			return -2;
		}
		sleep(MCAST_INTERVAL);
	}
	return 0;
}
