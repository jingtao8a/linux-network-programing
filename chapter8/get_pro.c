/*************************************************************************
	> File Name: get_pro.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月23日 星期三 09时59分11秒
 ************************************************************************/

#include <stdio.h>
#include <netdb.h>

void display_protocol(struct protoent* pt)
{
	int i = 0;
	if (pt) {
		printf("protocol name: %s,", pt->p_name);
		if (pt->p_aliases) {
			printf("alias name:");
			while (pt->p_aliases[i]) {
				printf("%s ", pt->p_aliases[i]);
				++i;
			}
		}
		printf(",value:%d\n", pt->p_proto);
	}
}

int main()
{
	int i = 0;
	char * protocol_name[] = {
		"ip",
		"icmp",
		"tcp",
		"udp",
		"ipv6",
		NULL
	};
	setprotoent(1);
	while (protocol_name[i] != NULL) {
		struct protoent* pt = getprotobyname((const char *)protocol_name[i]);
		if (pt) {
			display_protocol(pt);
		}
		++i;
	}
	endprotoent();
	return 0;
}
