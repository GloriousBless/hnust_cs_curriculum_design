#include <cstdio>
#include <string>
#include <fstream>
#include <WinSock2.h>
#include <iostream>
using namespace std;
#define BUFFER_SIZE 1024
#define HOST "192.168.12.3" //连接的局域网的IP地址
#define PORT 8100
#define HEADER "\
HTTP/1.1 200 OK\r\n\
Content-Type: text/html; charset=UTF-8\r\n\
Server: ZYhttp_v1.0.1\r\n\
Content-Length: %ld\r\n\r\n\
"
//将URL路径省略。则这里192.168.12.3:8100就指明是主页，不过也可以使用192.168.12.3:8100//index.html访问
const string strPath = "index.html"; //html页面
#pragma comment(lib, "WS2_32")

long GetFileLength(string strPath);

//在这里创建一个服务器，等待客户端发送连接请求进行资源查找，并进行http响应。

//那么客户端通过URL访问进行请求，接收到服务器返回信息，在浏览器上进行显示查找的资源
int main(int argc, char** argv)
{
    // 定义并且初始化一个服务器套接字
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
    addrServer.sin_port = htons(PORT);
    // 初始化
    WSADATA wsaData;
    WORD socketVersion = MAKEWORD(2, 2);
    if (WSAStartup(socketVersion, &wsaData) != 0)
    {
        printf("初始化失败!");
        exit(1);
    }
    // 创建套接字
    SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer == SOCKET_ERROR)
    {
        printf("创建失败!");
        exit(1);
    }
    // 绑定服务器套接字
    if (bind(socketServer, (LPSOCKADDR)&addrServer, sizeof(addrServer)) == SOCKET_ERROR)
    {
        printf("绑定失败!");
        exit(1);
    }
    // 监听
    if (listen(socketServer, 10) == SOCKET_ERROR)
    {
        printf("监听失败!");
        exit(1);
    }
    while (true)
    {
        printf("Listening ... \n");
        sockaddr_in addrClient;
        int nClientAddrLen = sizeof(addrClient);
        //服务器端建立连接
        SOCKET socketClient = accept(socketServer, (sockaddr*)&addrClient, &nClientAddrLen);
        if (SOCKET_ERROR == socketClient)
        {
            printf("接收失败!");
            break;
        }
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        //接收数据
        if (recv(socketClient, buffer, BUFFER_SIZE, 0) < 0)
        {
            printf("接收数据失败!");
            break;
        }
        printf("接收到的数据 : \n%s", buffer);
        // response
        // send header
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, HEADER, GetFileLength(strPath));//把文件和头文件合并然后发送数据
        if (send(socketClient, buffer, strlen(buffer), 0) < 0)
        {
            printf("发送数据失败!");
            break;
        }
        //读取文本
        ifstream fin(strPath.c_str(), ios::in | ios::binary);
        if (fin.is_open())
        {
            memset(buffer, 0, BUFFER_SIZE);
            //每次读取一个字节
            while (fin.read(buffer, BUFFER_SIZE - 1))
            {
                if (send(socketClient, buffer, strlen(buffer), 0) < 0)
                {
                    printf("发送数据失败!");
                    break;
                }
                memset(buffer, 0, BUFFER_SIZE);
            }
            if (send(socketClient, buffer, strlen(buffer), 0) < 0)
            {
                printf("发送数据失败!");
                break;
            }
        }
        //printf("发送!");
        //关于网络连接与数据传递
        fin.close();
        closesocket(socketClient);
    }
    closesocket(socketServer);
    WSACleanup();
    return 0;
}

//这个函数是获取文件的大小的
long GetFileLength(string strPath)
{
    ifstream fin(strPath.c_str(), ios::in | ios::binary);
    fin.seekg(0, ios_base::end);
    streampos pos = fin.tellg();//返回当前文件位置
    long lSize = static_cast<long>(pos);
    fin.close();
    cout << "content：" << lSize << endl;
    return lSize;
}
