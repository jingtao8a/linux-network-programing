/*************************************************************************
	> File Name: unix.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月24日 星期四 10时27分59秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>

static void display_err(const char* on_what)
{
	perror(on_what);
	exit(1);
}

int main(int argc, char *argv[])
{
	int error;
	int sock_UNIX;
	struct sockaddr_un addr_UNIX;
	int len_UNIX;
	const char path[] = "/demon/path";
	
	sock_UNIX = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock_UNIX == -1)
		display_err("socket()");
	unlink(path);
	memset(&addr_UNIX, 0, sizeof(addr_UNIX));

	add_UNIX.sun_family = AF_LOCAL;
	strcpy(addr_UNIX.sun_path, path);
	len_UNIX = sizeof(struct sockaddr_un);

	error = bind(sock_UNIX, (struct sockaddr*)&addr_UNIX, len_UNIX);
	if (error == -1)
		display_err("bind()");

	close(sock_UNIX);
	unlink(path);
	return 0;
}


	
	

	return 0;
}
