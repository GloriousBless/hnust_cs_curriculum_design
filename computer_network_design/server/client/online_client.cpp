
#include "windows.h"
#include "iostream"
#include "process.h"
#include <string>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

//建立连接之后接收数据
void Receive(void* param)
{
	string msg;
	while (1)
	{
		//客户端接受来自服务器的数据
		SOCKET clientSocket = *(SOCKET*)(param);
		char  recvbuf[2048] = {};		//接收缓冲区
		if (recv(clientSocket, recvbuf, 2048, 0) == SOCKET_ERROR)
		{
			cout << "数据接受失败" << endl;
		}
		else
		{
			msg = recvbuf;
			char sendPlayerName[5] = { 0 };
			int len = strlen(recvbuf);		//消息长度
			copy(&recvbuf[1], &recvbuf[5], sendPlayerName);	//分离出名字
			msg = msg.substr(5, len - 5);
			cout << sendPlayerName << "对你说：" << msg << endl;
		}

	}
}

//建立连接之后发送数据
void Send(void* param)
{
	while (1)
	{
		//客户端发送数据给服务器
		SOCKET clientSocket = *(SOCKET*)(param); //加载和创建Socket
		char sendbuf[2048] = {};		//发送缓冲区
		cin.getline(sendbuf, 2048);
		if (send(clientSocket, sendbuf, strlen(sendbuf), 0) == SOCKET_ERROR)
		{
			cout << "发送消息失败！";
		}
		else
			cout << "发送消息成功" << endl;
	}
}

//同样也要创建套接字加载，并发出连接请求给监听的服务器并通信
int main()
{
	cout << "-----------个人客户端-----------" << endl;
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "套接字初始化失败!" << endl;
	}
	SOCKET clientSocket;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "套接字创建失败!" << endl;
	}
	Sleep(30);
	struct sockaddr_in ClientAddress;		//一个绑定地址:有IP地址,有端口号,有协议族
	memset(&ClientAddress, 0, sizeof(sockaddr_in)); //初始化结构体
	ClientAddress.sin_family = AF_INET;
	ClientAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//填入本机IP地址 
	//ClientAddress.sin_port = htons(60001);//设定端口号
	//绑定套接字     指定绑定的IP地址和端口号
	if (bind(clientSocket, (sockaddr*)&ClientAddress, sizeof(ClientAddress)) == SOCKET_ERROR)
	{
		cout << "套接字绑定失败！" << endl;
		return 0;
	}
	struct sockaddr_in SeverAddress;		//服务器地址 也就是即将要连接的目标地址
	memset(&SeverAddress, 0, sizeof(sockaddr_in));
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //127.0.0.1表示本机ip地址
	SeverAddress.sin_port = htons(60000);//设定端口号

	//向服务器发出连接请求
	if (connect(clientSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "客户端：和服务器连接失败！" << endl;
		return 0;
	}
	else
		cout << "与服务器连接成功！" << endl;

	//创建两个子线程
	_beginthread(Receive, 0, &clientSocket);
	_beginthread(Send, 0, &clientSocket);

	Sleep(INFINITE);	//这里采用另外一种技术避免主线程执行完退出——使其无限期休眠
	//	关闭socket
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}
	return 0;
}
