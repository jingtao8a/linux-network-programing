/*************************************************************************
	> File Name: stat.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月18日 星期五 17时05分55秒
 ************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main()
{
	struct stat st;
	if (stat("test.txt", &st)) {
		fprintf(stderr, "stat error\n");
		return -1;
	}

	printf("%ld\n", st.st_nlink);
	return 0;
}

