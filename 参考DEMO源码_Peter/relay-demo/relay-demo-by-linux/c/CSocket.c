#include "CSocket.h"


int InitSocket(const char *serverIp,const int serverPort) {
    int serverFd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if(serverFd < 0)return -1;

    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(serverIp);
    serv_addr.sin_port = htons(serverPort);

    if(bind(serverFd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
        return -1;

    if(listen(serverFd,CLIENT_MAX_NUM) < 0)
        return -1;

    return serverFd;
}

int AcceptSocket(int serverFd) {
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int clientFd = accept(serverFd,(struct sockaddr*)&client_addr,&client_addr_size);
    if(clientFd < 0) return -1;
    return clientFd;
}

int CloseSocket(int fd) {
    if(fd < 0) return -1;
    return close(fd);
}

int SendData(int clientFd,const char *sendBuf,int bufLen) {
    return write(clientFd,sendBuf,bufLen);
}

int ReceviceData(int clientFd,char *readBuf) {
    memset(readBuf,0,READ_BUFFER_MAX_SIZE);
    return read(clientFd,readBuf,READ_BUFFER_MAX_SIZE);
}

int SendCmd(int clientFd,const char *pCmd,int cmdLen) {
    SendData(clientFd,pCmd,cmdLen);
    char readBuf[READ_BUFFER_MAX_SIZE];
    ReceviceData(clientFd,readBuf);
    printf("receive data:%s\r\n",readBuf);
}
