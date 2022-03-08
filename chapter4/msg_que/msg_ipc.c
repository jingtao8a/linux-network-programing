/*************************************************************************
	> File Name: msg_ipc.c
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年02月19日 星期六 10时02分28秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

void msg_show_attr(int msg_id, struct msqid_ds msg_info)
{
	int ret = -1;
	sleep(1);
	ret = msgctl(msg_id, IPC_STAT, &msg_info);
	if (ret == -1) {
		fprintf(stderr, "get msg_info error\n");
		return;
	}
	printf("\n");
	printf("current queue bytes: %ld\n", msg_info.msg_cbytes);
	printf("the number of msg: %d\n", (int)msg_info.msg_qnum);
	printf("queue max bytes: %d\n", (int)msg_info.msg_qbytes);
	printf("last sending proc: %d\n", msg_info.msg_lspid);
	printf("last receiving proc: %d\n", msg_info.msg_lrpid);
	printf("last sending time: %s\n", ctime(&(msg_info.msg_stime)));
	printf("last receiving time: %s\n", ctime(&(msg_info.msg_rtime)));
	printf("last change time: %s\n", ctime(&(msg_info.msg_ctime)));

	printf("MSG UID IS: %d\n", msg_info.msg_perm.uid);
	printf("MSG GID IS: %d\n", msg_info.msg_perm.gid);
	return;
}

int main()
{
	int ret = -1;
	int msg_flags, msg_id;
	key_t key;
	struct msgbuf{
		long mtype;
		char mtext[10];
	};
	struct msqid_ds msg_info;
	struct msgbuf msg_buf, buffer;

	int msg_sflags, msg_rflags;

	char *msgpath = "./ipc/msg";
	key = ftok(msgpath, 'a');
	if (key != -1) {
		printf("create key successfully\n");
	} else {
		fprintf(stderr, "ftok error\n");
	}

	msg_flags = IPC_CREAT|IPC_EXCL;
	msg_id = msgget(key, msg_flags|0x0666);
	if (msg_id <= 0) {
		fprintf(stderr, "msgget error\n");
		return 0;
	}

	msg_show_attr(msg_id, msg_info);

	msg_sflags = IPC_NOWAIT;
	msg_buf.mtype = 1;
	memcpy(msg_buf.mtext, "test", sizeof("test"));
	ret = msgsnd(msg_id, &msg_buf, sizeof("test"), msg_sflags);
	if (ret == -1) {
		fprintf(stderr, "msgsend error\n");
	}

	msg_show_attr(msg_id, msg_info);

	msg_rflags = IPC_NOWAIT|MSG_NOERROR;
	buffer.mtype = 1;
	ret = msgrcv(msg_id, &buffer, sizeof(buffer.mtext), (long)0, msg_rflags);
	if (ret == -1) {
		fprintf(stderr, "msgrcv error\n");
	} else {
		printf("receive data: %d byte\n", ret);
	}
	msg_show_attr(msg_id, msg_info);

	msg_info.msg_perm.uid = 8;
	msg_info.msg_perm.gid = 8;
	msg_info.msg_qbytes = 12345;
	ret = msgctl(msg_id, IPC_SET, &msg_info);
	if (ret == -1) {
		fprintf(stderr, "msgctl error\n");
		return 0;
	}
	msg_show_attr(msg_id, msg_info);

	ret = msgctl(msg_id, IPC_RMID, NULL);
	if (ret == -1) {
		fprintf(stderr, "delete queue fail\n");
		return 0;
	}
	return 0;
}
