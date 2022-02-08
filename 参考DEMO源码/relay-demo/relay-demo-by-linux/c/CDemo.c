#include "CSocket.h"



int main() {

    printf("本机作为服务端");
    printf("本机IP：192.168.1.229 端口：6000");
    int serverFd = InitSocket("192.168.1.229",6000);
    if(serverFd < 0) {
        perror("初始化服务端失败，请检查端口是否被占用!\r\n");
    }

    printf("启动侦听，等待客户端请求连接。。。");
    int clientFd = AcceptSocket(serverFd);
    if(clientFd < 0) {
        perror("accept error!\r\n");
    }

    printf("TCP-KP系列网络继电器功能测试：\r\n");
    printf("控制继电器通道1常开吸合发送：AT+STACH1=1\\r\\n");
    SendCmd(clientFd,"AT+STACH1=1\r\n",strlen("AT+STACH1=1\r\n"));
    sleep(1);
    printf("读取继电器输出通道1状态");
    SendCmd(clientFd,"AT+STACH1=?\r\n",strlen("AT+STACH1=?\r\n"));

    sleep(1);
    printf("控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n");
    SendCmd(clientFd,"AT+STACH1=0\r\n",strlen("AT+STACH1=0\r\n"));
    sleep(1);
    printf("读取继电器输出通道1状态");
    SendCmd(clientFd,"AT+STACH1=?\r\n",strlen("AT+STACH1=?\r\n"));

    sleep(1);
    printf("控制继电器通道1常开断开发送：AT+STACH1=0\\r\\n");
    SendCmd(clientFd,"AT+STACH1=1,2\r\n",strlen("AT+STACH1=1,2\r\n"));    
    sleep(1);
    printf("读取继电器输出通道1状态");
    SendCmd(clientFd,"AT+STACH1=?\r\n",strlen("AT+STACH1=?\r\n"));

    sleep(1);
    printf("读取继电器输入通道1状态");
    SendCmd(clientFd,"AT+OCCH1=?\r\n",strlen("AT+OCCH1=?\r\n"));

    printf("更多控制指令的详细解释请查阅《TCP-KP型网络继电器AT指令表.PDF》\r\n");
    printf("本Demo仅用于开发参考，不能直接用于商业项目实施！\r\n");

    return 0;
}




