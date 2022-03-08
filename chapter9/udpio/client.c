/*************************************************************************
	> File Name: client.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月23日 星期三 15时32分50秒
 ************************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT_SERV 8888
#define BUFF_LEN 256
#define NUM_DATA 100
#define LENGTH 1024

static char buff_send[LENGTH];

static void udpclient_echo(int s, struct sockaddr* to)
{
	char buff_init[BUFF_LEN] = "UDP TEST";

	struct sockaddr_in from;
	socklen_t len = sizeof(*to);

	int i;
	for (i = 0; i < NUM_DATA; ++i) {
		*((int*)&buff_send[0]) = htonl(i);
		memcpy(&buff_send[4], buff_init, sizeof(buff_init));
		sendto(s, buff_send, LENGTH, 0, to, len);
	}
}

int main(int argc, char** argv)
{
	int s;
	struct sockaddr_in addr_serv;
	s = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&addr_serv, 0, sizeof(addr_serv));
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_addr.s_addr = inet_addr("192.168.223.128");
	addr_serv.sin_port = htons(PORT_SERV);

	udpclient_echo(s, (struct sockaddr*)&addr_serv);

	close(s);
	return 0;
}
