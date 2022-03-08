/*************************************************************************
	> File Name: client.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月23日 星期三 10时27分25秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>

extern void sig_proccess(int signo);
extern void sig_pipe(int signo);

static int s;
void sig_process(int signo)
{
	printf("Catch a exit signal\n");
	close(s);
	exit(0);
}
void sig_pipe(int sign)
{
	printf("Catch a SIGPIPE signal\n");
	close(s);
	exit(0);
}
#define PORT 8888

void process_conn_client(int s)
{
	ssize_t size = 0;
	char buffer[1024];
	
	while (1) {
		size = read(0, buffer, 1024);
		if (size > 0) {
			send(s, buffer, size, 0);
			size = recv(s, buffer, 1024, 0);
			write(1, buffer, size);
		}
	}
}
int main(int argc, char** argv)
{
	struct sockaddr_in server_addr;
	int err;
	if (argc == 1) {
		printf("PLS input server addr\n");
		return 0;
	}
	signal(SIGINT, sig_process);
	signal(SIGPIPE, sig_pipe);

	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
		printf("socket error\n");
		return -1;
	}
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	inet_pton(AF_INET, argv[1], &server_addr.sin_addr);
	connect(s, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));

	process_conn_client(s);
	close(s);
	return 0;
}
