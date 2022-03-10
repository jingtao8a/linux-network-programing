/*************************************************************************
	> File Name: single_client_thread1.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月10日 星期四 11时18分50秒
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

#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5


static void* handle_request(void* arg) {
	int s_c = *(int *)arg;
	pthread_detach(pthread_self());
	char buff[BUFFLEN];
	memset(buff, 0, BUFFLEN);
	int n = recv(s_c, buff, BUFFLEN, 0);
	if (n > 0 && !strncmp(buff, "TIME", 4)) {
		time_t now = time(NULL);
		sprintf(buff, "%24s\r\n", ctime(&now));
		send(s_c, buff, strlen(buff), 0);
	}
	close(s_c);
	return NULL;
}

static void handle_connect(int s_s) {
	struct sockaddr_in from;
	socklen_t  len;
	pthread_t t;
	while (1) {
		int s_c = accept(s_s, (struct sockaddr*)&from, &len);
		if (s_c > 0)
			pthread_create(&t, NULL, handle_request, &s_c);
	}
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
	
	handle_connect(s_s);
	close(s_s);
	return 0;
}
