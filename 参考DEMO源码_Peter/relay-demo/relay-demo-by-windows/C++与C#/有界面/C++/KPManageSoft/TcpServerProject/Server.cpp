
#define _SCL_SECURE_NO_WARNINGS 
#include "pch.h"
#include "framework.h"
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include "conio.h"
#include <windows.h>
#include <process.h>
#include <math.h>
#include "Server.h"

#define QLEN       5
#define    WSVERS    MAKEWORD(2, 0)
#define    BUFLEN    2000             // 缓冲区大小
#pragma comment(lib, "ws2_32.lib")  //winsock 2.2 library

SOCKET    msock, ssock;            /* master & slave sockets           */
SOCKET    sockets[100] = { NULL };

int cc;
char    *pts;                    /* pointer to time string           */
time_t    now;                    /* current time                       */
char buf[2000];                      /* buffer                          */
char *input;
HANDLE hThread1, hThread[100] = { NULL };
unsigned int threadID, ThreadID[100], number;

struct    sockaddr_in fsin;
struct    sockaddr_in Sin;

unsigned int __stdcall Chat(PVOID PM)
{
	char buf1[2000];
	char buf2[2000];
	char buf3[2000];
	char buf4[2000];
	(void)time(&now);
	pts = ctime(&now);
	sockets[number] = ssock;
	SOCKET    sock = ssock;
	ThreadID[number] = threadID;
	unsigned int threadid = threadID;
	sprintf(buf1, " 时间： %s  \t【我的线程号： %d 】\n", pts, threadid);
	(void)send(sock, buf1, sizeof(buf1), 0);
	sprintf(buf2, " 线程号 <%d> 客户<IP:%s 端口:%d>  enter  \n", threadid, inet_ntoa(fsin.sin_addr), fsin.sin_port);
	printf("%s ", buf2);
	printf("\t将自动把此数据发送给所有客户！ \n");
	for (int i = 0;i <= number;i++)
	{
		if (sockets[i] != NULL && sockets[i] != sock)
		{
			(void)send(sockets[i], buf2, sizeof(buf2), 0);
			printf(" 发送至线程号<%d>成功！\n", ThreadID[i]);
		}
	}
	printf(" \n");


flag1:cc = recv(sock, buf3, BUFLEN, 0);   //cc为接收的字符数
	if (cc == SOCKET_ERROR || cc == 0)
	{
		(void)time(&now);
		pts = ctime(&now);
		sprintf(buf3, " 线程号 <%d> 客户<IP:%s 端口:%d>  leave !  \n \t\t时间： %s", threadid, inet_ntoa(fsin.sin_addr), fsin.sin_port, pts);
		sock = NULL;
		sockets[number] = NULL;
		CloseHandle(hThread[number]);
		printf("%s ", buf3);
		printf("\t将自动把此数据发送给所有客户！ \n");
		for (int i = 0;i <= number;i++)
		{
			if (sockets[i] != NULL && sockets[i] != sock)
			{
				(void)send(sockets[i], buf3, sizeof(buf3), 0);
				printf(" 发送至线程号<%d>成功！\n", ThreadID[i]);
			}
		}
		printf(" \n");
	}

	else if (cc > 0)
	{
		(void)time(&now);
		pts = ctime(&now);
		sprintf(buf4, " 线程号 <%d> 客户<IP:%s 端口:%d>说 ：%s  \n \t\t时间 ： %s", threadid, inet_ntoa(fsin.sin_addr), fsin.sin_port, buf3, pts);

		printf("%s ", buf4);
		printf("\t将自动把此数据发送给所有客户！ \n");
		for (int i = 0;i <= number;i++)
		{
			if (sockets[i] != NULL && sockets[i] != sock)
			{
				(void)send(sockets[i], buf4, sizeof(buf4), 0);
				printf(" 发送至线程号<%d>成功！\n", ThreadID[i]);
			}
		}
		printf(" \n");

		goto flag1;
	}
	(void)closesocket(sock);

	return 0;
}

/*------------------------------------------------------------------------
* main - Iterative TCP server for DAYTIME service
 *------------------------------------------------------------------------
*/
void main(int argc, char *argv[])
/* argc: 命令行参数个数， 例如：C:\> TCPdaytimed 8080
				  argc=2 argv[0]="TCPdaytimed",argv[1]="8080" */
{
	int     alen;                    /* from-address length               */
	WSADATA wsadata;
	char    *service = "5050";
	WSAStartup(WSVERS, &wsadata);                         //加载 winsock 2.2 library
	msock = socket(PF_INET, SOCK_STREAM, 0);              //生成套接字。TCP协议号=6， UDP协议号=17
	memset(&Sin, 0, sizeof(Sin));
	Sin.sin_family = AF_INET;
	Sin.sin_addr.s_addr = INADDR_ANY; //INADDR_ANY                   //指定绑定接口的IP地址。INADDR_ANY表示绑定(监听)所有的接口。
	Sin.sin_port = htons((u_short)atoi(service));        //atoi--把ascii转化为int，htons - 主机序(host)转化为网络序(network), s(short) 
	bind(msock, (struct sockaddr *)&Sin, sizeof(Sin));   // 绑定端口号（和IP地址)
	listen(msock, 5);                                    //队列长度为5

	printf("\t\t\t\t Chat 多人聊天程序 \n");
	printf("\t\t\t\t       (Server) \n");
	(void)time(&now);
	pts = ctime(&now);
	printf("\t\t\t  时间 ：%s", pts);
	number = -1;
	while (1)                                    //检测是否有按键
	{
		alen = sizeof(struct sockaddr);
		ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
		number++;
		hThread[number] = (HANDLE)_beginthreadex(NULL, 0, Chat, NULL, 0, &threadID);
	}
	(void)closesocket(msock);
	WSACleanup();                         //卸载载 winsock 2.2 library
}


void Server::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	//m_bGreen = TRUE;
}
BEGIN_MESSAGE_MAP(Server, CDialog)
	ON_WM_NCHITTEST()
//	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()




//BOOL Server::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	return CDialog::OnEraseBkgnd(pDC);
//}
