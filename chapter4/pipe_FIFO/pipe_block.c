/*************************************************************************
	> File Name: pipe_block.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月18日 星期五 19时32分12秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#define K 1024
#define WRITELEN (128 * K)

int main()
{
	int result = -1;
	int fd[2];
	result = pipe(fd);
	if (result == -1) fprintf(stderr, "pipe error");
	int nbytes;
	char string[WRITELEN] = "hello, pipe";
	char readbuffer[10 * K];
	pid_t pid = fork();
	if (pid < 0) {
		fprintf(stderr, "fork error");
	} else if (pid == 0) {
		close(fd[0]);
		int writesize = WRITELEN;
		result = 0;
		result = write(fd[1], string, writesize);
		if (result > 0) {
			writesize -= result;
			printf("child have write %d byte, %d byte left\n", result, writesize);
		}
	} else {
		close(fd[1]);
		while (1) {
			nbytes = read(fd[0], readbuffer, sizeof(readbuffer));

			if (nbytes == 0) {
				printf("none data left\n");
				break;
			}
			printf("receive %d byte, %s\n", nbytes, readbuffer);
		}
	}
	return 0;
}

