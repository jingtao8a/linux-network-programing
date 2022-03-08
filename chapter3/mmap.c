/*************************************************************************
	> File Name: mmap.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月18日 星期五 17时09分57秒
 ************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#define FILELENGTH 80

int main()
{
	int fd = -1;
	char buf[] = "quick brown fox jumps over the lazy dog";
	
	char *ptr = NULL;
	fd = open("mmap.txt", O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);

	if (fd == -1) {
		return -1;
	}
	
	lseek(fd, FILELENGTH - 1, SEEK_SET);
	write(fd, "a", 1);
	ptr = (char *)mmap(NULL, FILELENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	if (ptr == (char*)-1) {
		fprintf(stderr, "mmap failure\n");
	}
	memcpy(ptr + 16, buf, strlen(buf));
	munmap(ptr, FILELENGTH);
	close(fd);
	return 0;
}
