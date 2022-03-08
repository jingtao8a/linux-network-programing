/*************************************************************************
	> File Name: util.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月21日 星期一 15时42分56秒
 ************************************************************************/
#include <stddef.h>

const char* my_strchr(const char *str, int c)
{
	const char* pos = str;
	for (; *pos != 0; ++pos) {
		if (*pos == c) {
			return pos;
		}
	}
	return NULL;
}

unsigned int my_inet_addr(const char* str)
{
	unsigned char ch[4] = {0};
	const char *pstr[3], *pos;
	pstr[0] = my_strchr(str, '.');
	pstr[1] = my_strchr(pstr[0] + 1, '.');
	pstr[2] = my_strchr(pstr[1] + 1, '.');
	
	pos = str;
	while (pos != pstr[0]) {
		ch[0] = ch[0] * 10 + (*pos - '0');
		++pos;
	}
	++pos;
	while (pos != pstr[1]) {
		ch[1] = ch[1] * 10 + (*pos - '0');
		++pos;
	}
	++pos;
	while (pos != pstr[2]) {
		ch[2] = ch[2] * 10 + (*pos - '0');
		++pos;
	}
	++pos;
	while (*pos != '\0') {
		ch[3] = ch[3] * 10 + (*pos - '0');
		++pos;
	}
	
	unsigned int *tmp = (unsigned int *)ch;
	return *tmp;
}
