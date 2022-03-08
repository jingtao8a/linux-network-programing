/*************************************************************************
	> File Name: server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月23日 星期三 15时25分27秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT_SERV 8888
#define NUM_DATA 100
#define LENGTH 1024

static char buff[NUM_DATA][LENGTH];

static void udpserv_echo(int s, struct sockaddr* client)
{
	int n, i;
	char tmp_buff[LENGTH];
	socklen_t len = sizeof(struct sockaddr);
	for (i = 0; i < NUM_DATA; ++i) {
		len = sizeof(struct sockaddr);
		n = recvfrom(s, tmp_buff, sizeof(tmp_buff), 0, client, &len);
		memcpy(buff[ntohl(*(int*)tmp_buff)], tmp_buff, n);
	}
	for (i = 0; i < NUM_DATA; ++i) {
		printf("%d:%s\n", ntohl(*(int *)buff[i]), &buff[i][4]);
	}
}

int main(int argc, char *argv[])
{
	int s;
	struct sockaddr_in addr_serv, addr_clie;
	s = socket(AF_INET, SOCK_DGRAM, 0);
	
	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_serv.sin_port = htons(PORT_SERV);

	bind(s, (struct sockaddr*)&addr_serv, sizeof(addr_serv));

	udpserv_echo(s, (struct sockaddr*)&addr_clie);
	close(s);
	return 0;
}


