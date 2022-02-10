#ifndef __C_SOCKET_H__
#define __C_SOCKET_H__

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


#ifndef CLIENT_MAX_NUM
    #define CLIENT_MAX_NUM (10)
#endif

#ifndef READ_BUFFER_MAX_SIZE
    #define READ_BUFFER_MAX_SIZE (256)
#endif


extern int InitSocket(const char *serverIp,const int serverPort);
extern int AcceptSocket(int serverFd);
extern int CloseSocket(int fd);
extern int SendCmd(int clientFd,const char *pCmd,int cmdLen);


#endif

