/*************************************************************************
	> File Name: server.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月10日 星期四 11时47分50秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5
#define CLIENTNUM 50

static int connect_host[CLIENTNUM];
static int connect_number = 0;

static void* handle_request(void* arg) {
	char buff[BUFFLEN];
	int max_fd;
	fd_set scanfd;
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	while (1) {
		max_fd = -1;
		FD_ZERO(&scanfd);
		for (int i = 0; i < CLIENTNUM; ++i) {
			if (connect_host[i] != -1) {
				FD_SET(connect_host[i], &scanfd);
				if (max_fd < connect_host[i])
					max_fd = connect_host[i];
			}
		}
		int err = select(max_fd + 1, &scanfd, NULL, NULL, &timeout);
		switch(err) {
			case 0://timeout
				//printf("select timeout\n");
				break;
			case -1://error
				printf("select error\n");
				break;
			default:
				if (connect_number <= 0)
					break;
				for (int i = 0; i < CLIENTNUM; ++i) {
					if (connect_host[i] != -1) {
						if (FD_ISSET(connect_host[i], &scanfd)) {
							memset(buff, 0, BUFFLEN);
							int n = recv(connect_host[i], buff, BUFFLEN, 0);
							if (n > 0 && !strncmp(buff, "TIME", 4)) {
								printf("receive success\n");
								memset(buff, 0, BUFFLEN);
								time_t now = time(NULL);
								sprintf(buff, "%24s\r\n", ctime(&now));
								send(connect_host[i], buff, strlen(buff), 0);
							}
							close(connect_host[i]);
							connect_host[i] = -1;
							connect_number--;
						}
					}
				}
				break;
		}
	}
	return NULL;
}

static void* handle_connect(void* arg) {
	int s_s = *(int*)arg;
	struct sockaddr_in from;
	socklen_t  len;
	while (1) {
		int s_c	= accept(s_s, (struct sockaddr*)&from, &len);
		printf("a client connect, form: %s\n", inet_ntoa(from.sin_addr));
		for (int i = 0; i < CLIENTNUM; ++i) {
			if (connect_host[i] == -1) {
				connect_host[i] = s_c;
				connect_number++;
				printf("add client\n");
				break;
			}
		}
	}
	return NULL;
}

int main() {
	int s_s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in local;
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(SERVER_PORT);

	bind(s_s, (struct sockaddr*)&local, sizeof(local));
	listen(s_s, BACKLOG);
	for (int i = 0; i < CLIENTNUM; ++i) {
		connect_host[i] = -1;
	}
	pthread_t t[2];
	pthread_create(t, NULL, handle_connect, &s_s);
	pthread_create(t + 1, NULL, handle_request, NULL);
	for (int i = 0; i < 2; ++i) {
		pthread_join(t[i], NULL);
	}
	close(s_s);
	return 0;
}
