// TCP_Server_Demo2Cpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <winsock2.h>
#include <stdio.h>

#pragma comment(lib,"ws2_32.lib")

void TcpServerDemo() {
	std::string serverIp = "192.168.1.246";
	int serverPort = 6000;

	std::cout << "本机作为服务端" << std::endl;
	std::cout << "本机IP：" << serverIp.c_str() << " 端口：" << serverPort << std::endl;
	//初始化DLL
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsdata;
	if (WSAStartup(sockVersion, &wsdata) != 0)
	{
		return;
	}

	//创建套接字
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "创建套接字错误" << std::endl;
		return;
	}

	//绑定套接字
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(serverPort);
	sockAddr.sin_addr.S_un.S_addr = inet_addr(serverIp.c_str());

	if (bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
		std::cout << "绑定错误，请检测IP是否正确，或端口是否被点用！" << std::endl;
		return;
	}

	//开始监听
	if (listen(serverSocket, 10) == SOCKET_ERROR) {
		std::cout << "监听错误" << std::endl;
		return;
	}

	SOCKET clientSocket;
	sockaddr_in client_sin;
	char msg[100];//存储传送的消息
	int len = sizeof(client_sin);

	std::cout << "启动侦听，等待客户端请求连接。。。" << std::endl;
	clientSocket = accept(serverSocket, (sockaddr*)&client_sin, &len);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "请求异常" << std::endl;
		return;
	}
	std::cout << "连接成功" << std::endl;
	std::cout << "客户端IP：" << inet_ntoa(client_sin.sin_addr) << " 端口为：" << htons(client_sin.sin_port) << std::endl;
	//========================
	// 以下为数据发送与接收
	//========================

	
	std::cout << "TCP-KP系列网络继电器功能测试：" << std::endl;
	const char * sendData;
	std::string data = "AT+STACH1=1\r\n";
	std::cout << "控制继电器通道1常开吸合发送：" << data.c_str();
	sendData = data.c_str();
	send(clientSocket, sendData, strlen(sendData), 0);

	int num = recv(clientSocket, msg, 100, 0);
	if (num > 0)
	{
		msg[num] = '\0';
		std::cout << "网络继电器应答：" << msg;
		std::cout << "指令执行成功" << std::endl;
	}
	else
	{
		std::cout << "连接异常" << std::endl;
		closesocket(clientSocket);
		return;
	}

	Sleep(300);

	data = "AT+STACH1=0\r\n";
	std::cout << "控制继电器通道1常开断开发送：" << data.c_str();
	sendData = data.c_str();
	send(clientSocket, sendData, strlen(sendData), 0);

	num = recv(clientSocket, msg, 100, 0);
	if (num > 0)
	{
		msg[num] = '\0';
		std::cout << "网络继电器应答：" << msg;
		std::cout << "指令执行成功" << std::endl;
	}
	else
	{
		std::cout << "连接异常" << std::endl;
		closesocket(clientSocket);
		return;
	}

	Sleep(300);

	data = "AT+STACH1=1,15\r\n";
	std::cout << "控制继电器通道1常开接口吸合15秒后断开发送：" << data.c_str();
	sendData = data.c_str();
	send(clientSocket, sendData, strlen(sendData), 0);

	num = recv(clientSocket, msg, 100, 0);
	if (num > 0)
	{
		msg[num] = '\0';
		std::cout << "网络继电器应答：" << msg;
		std::cout << "指令执行成功" << std::endl;
	}
	else
	{
		std::cout << "连接异常" << std::endl;
		closesocket(clientSocket);
		return;
	}

	data = "AT+OCCH1=?\r\n";
	std::cout << "获取开关量输入接口1的状态发送：" << data.c_str();
	sendData = data.c_str();
	send(clientSocket, sendData, strlen(sendData), 0);

	num = recv(clientSocket, msg, 100, 0);
	if (num > 0)
	{
		msg[num] = '\0';
		std::cout << "网络继电器应答：" << msg;
		std::cout << "指令执行成功" << std::endl;
	}
	else
	{
		std::cout << "连接异常" << std::endl;
		closesocket(clientSocket);
		return;
	}

	std::cout << "关闭客户端连接" << std::endl;
	closesocket(clientSocket);
	std::cout << "测试完成" << std::endl;
}


int main()
{
	TcpServerDemo();
	std::cout << "更多控制指令的详细解释请查阅《TCP-KP型网络继电器AT指令表.PDF》" << std::endl;
	std::cout << "本Demo仅用于开发参考，不能直接用于商业项目实施！" << std::endl;
}





// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
