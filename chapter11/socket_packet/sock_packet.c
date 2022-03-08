/*************************************************************************
	> File Name: sock_packet.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月03日 星期四 20时47分45秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/if_ether.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>

int main()
{
	int fd = socket(AF_INET, SOCK_PACKET, htons(0x0003));

	return 0;
}
