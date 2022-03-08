/*************************************************************************
	> File Name: gethost.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月22日 星期二 18时39分43秒
 ************************************************************************/

#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
	char host[] = "www.baidu.com";
	struct hostent *ht = NULL;
	char str[30];
	ht = gethostbyname(host);
	if (ht) {
		int i = 0;
		printf("get the host: %s addr\n", host);
		printf("name: %s\n", ht->h_name);
		printf("type: %s\n", ht->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
		printf("length: %d\n", ht->h_length);
		for (i = 0; ; i++) {
			if (ht->h_addr_list[i] != NULL) {
				printf("IP:%s\n", inet_ntop(ht->h_addrtype, ht->h_addr_list[i], str, 30));
			} else {
				break;
			}
		}
		for (i = 0; ; i++) {
			if (ht->h_aliases[i] != NULL) {
				printf("aliases %d: %s\n", i + 1, ht->h_aliases[i]);
			} else {
				break;
			}
		}
	}
	return 0;
}
