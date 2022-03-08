/*************************************************************************
	> File Name: test.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月21日 星期一 16时10分42秒
 ************************************************************************/

#include <stdio.h>
#include "util.h"

int main()
{
	unsigned int addr = my_inet_addr("192.168.0.1");
	printf("%x\n", addr);
	return 0;
}
