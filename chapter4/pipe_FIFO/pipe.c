/*************************************************************************
	> File Name: pipe.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月18日 星期五 19时13分14秒
 ************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fd[2];
	int result = -1;
	result = pipe(fd);
	if (result == -1) {
		fprintf(stderr, "pipe error\n");
	}
	//fd[0] read
	//fd[1] write
	int nbytes;
	char string[] = "hello, pipe", readbuffer[80];

	pid_t pid = fork();
	if (pid == 0) {
		close(fd[0]);
		write(fd[1], string, strlen(string));
	} else {
		close(fd[1]);
		read(fd[0], readbuffer, sizeof(readbuffer));
		printf("%s\n", readbuffer);
	}
	return 0;
}
