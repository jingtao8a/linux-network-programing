/*************************************************************************
	> File Name: common.h
	> Author: yuxintao
	> Mail: 1921056015@qq.com 
	> Created Time: 2022年03月01日 星期二 16时08分14秒
 ************************************************************************/

#ifndef COMMON_H
#define COMMON_H

extern int createmsgid();

extern int msgsend(int msgid, long types, char* buffer);

extern int msgreceive(int msgid, long types, char *buffer);

#endif
