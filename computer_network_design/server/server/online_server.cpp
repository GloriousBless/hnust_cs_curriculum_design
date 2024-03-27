
#include <iostream>  
#include "windows.h" //一定要包含该头文件
#include "process.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
#pragma comment(lib, "WS2_32.lib")  //显示加载 ws2_32.dll	ws2_32.dll就是最新socket版本

int g_curPlayerNum = 0; //当前连接数
const char* g_PlayerName[] =		//假定的聊天者名字
{
	"aaaa",
	"bbbb",
	"cccc",
	"dddd",
};

struct PlayerInfo	//利用结构存储连接的客户端
{
	SOCKET sock;
	string name;
};

vector<PlayerInfo>g_clientSockList;		//利用vector存取已连接的客户端

void process(void* param)
{
	int index = *(int*)param;	//当前子线程编号
	while (1)
	{
		//服务器接收信息
		//int index = *(int*)param;
		char buf[2048] = { 0 };  //接收缓冲区
		int bytes;
		if ((bytes = recv(g_clientSockList[index].sock, buf, sizeof(buf), 0)) == SOCKET_ERROR)
		{
			cout << "服务器接收数据失败！" << endl;
		}
		//服务器转发（含逻辑处理）
		if (buf[0] == '@')
		{
			//私聊
			string Buf(buf);
			string recvPlayerName = Buf.substr(1, 4);	//分离出接收者名字
			copy(g_clientSockList[index].name.begin(), g_clientSockList[index].name.end(), &buf[1]);

			for (vector<PlayerInfo>::iterator it = g_clientSockList.begin(); it != g_clientSockList.end(); it++)
			{
				if (it->name == recvPlayerName)
				{
					if (send(it->sock, buf, strlen(buf), 0) == SOCKET_ERROR)
					{
						cout << "发送数据失败 to" << it->name << endl;
					}
					break;
				}
			}
		}
		else
			//群聊
			cout << g_clientSockList[index].name << "对" << "所有人说：" << buf << endl;
	}
}

//创建套接字，并绑定套接字到IP地址和端口
//服务器listen()函数监听，等待客户端连接
//有请求则接受，返回新的对应此次连接的套接字
//并利用返回的套接字与客户端通信
//返回，等待下一个客户端的连接请求
//最后不需要了，就可以关闭套接字了。
int main()
{
	cout << "-----------聊天室服务器-----------" << endl;

	//套接字初始化
	WSADATA wsaData;	//这个结构被用来存储被WSAStartup函数调用后返回的 Windows Sockets 数据。
	WORD sockVersion = MAKEWORD(2, 2);	//windows网络编程库的版本号信息
	if (WSAStartup(sockVersion, &wsaData) != 0) //WSAStartup函数是在程序中初始化并加载Windows网络
	{
		cout << "套接字初始化失败!" << endl;
		return 0;
	}

	//创建服务器套接字
	SOCKET SeverSocket;
	if ((SeverSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "套接字创建失败！" << endl;
		return 0;
	}
	struct sockaddr_in SeverAddress;		//一个绑定地址:有IP地址,有端口号,有协议族
	memset(&SeverAddress, 0, sizeof(sockaddr_in)); //初始化结构体
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//填入本机IP地址 
	SeverAddress.sin_port = htons(60000);//设定端口号

	//绑定套接字     指定绑定的IP地址和端口号
	if (bind(SeverSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "套接字绑定失败！" << endl;
		return 0;
	}

	//服务器监听	,等待连接请求
	if (listen(SeverSocket, SOMAXCONN) == SOCKET_ERROR) //监听的第二个参数就是:能存放多少个客户端请求
	{
		cout << "监听失败！" << endl;
		return 0;
	}
	else
		cout << "服务器等待连接......" << endl;


	while (1)
	{
		//服务器接受连接请求
		sockaddr_in revClientAddress;	//套接字的地址，端口
		SOCKET revClientSocket = INVALID_SOCKET;		//用来接收客户端连接
		//memset(&revClientAddress, 0, sizeof(revClientAddress));
		int addlen = sizeof(revClientAddress);
		// 请求到来后，接受连接请求，返回一个新的对应于此次连接的套接字
		if ((revClientSocket = accept(SeverSocket, (sockaddr*)&revClientAddress, &addlen)) == INVALID_SOCKET)
		{
			cout << "接受客户端连接失败！" << endl;
			return 0;
		}

		PlayerInfo stPlayerInfo;
		stPlayerInfo.sock = revClientSocket;
		stPlayerInfo.name = g_PlayerName[g_curPlayerNum];
		g_clientSockList.push_back(stPlayerInfo);
		int temp = g_curPlayerNum;
		_beginthread(process, 0, &temp);		//创建子线程来收发数据
		g_curPlayerNum++;
		cout << stPlayerInfo.name << "上线啦！" << endl;
	}
	return 0;
}

