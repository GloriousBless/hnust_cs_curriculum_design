#include <cstdio>
#include <string>
#include <fstream>
#include <WinSock2.h>
#include <iostream>
using namespace std;
#define BUFFER_SIZE 1024
#define HOST "192.168.12.3" //���ӵľ�������IP��ַ
#define PORT 8100
#define HEADER "\
HTTP/1.1 200 OK\r\n\
Content-Type: text/html; charset=UTF-8\r\n\
Server: ZYhttp_v1.0.1\r\n\
Content-Length: %ld\r\n\r\n\
"
//��URL·��ʡ�ԡ�������192.168.12.3:8100��ָ������ҳ������Ҳ����ʹ��192.168.12.3:8100//index.html����
const string strPath = "index.html"; //htmlҳ��
#pragma comment(lib, "WS2_32")

long GetFileLength(string strPath);

//�����ﴴ��һ�����������ȴ��ͻ��˷����������������Դ���ң�������http��Ӧ��

//��ô�ͻ���ͨ��URL���ʽ������󣬽��յ�������������Ϣ����������Ͻ�����ʾ���ҵ���Դ
int main(int argc, char** argv)
{
    // ���岢�ҳ�ʼ��һ���������׽���
    sockaddr_in addrServer;
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.S_un.S_addr = INADDR_ANY;
    addrServer.sin_port = htons(PORT);
    // ��ʼ��
    WSADATA wsaData;
    WORD socketVersion = MAKEWORD(2, 2);
    if (WSAStartup(socketVersion, &wsaData) != 0)
    {
        printf("��ʼ��ʧ��!");
        exit(1);
    }
    // �����׽���
    SOCKET socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer == SOCKET_ERROR)
    {
        printf("����ʧ��!");
        exit(1);
    }
    // �󶨷������׽���
    if (bind(socketServer, (LPSOCKADDR)&addrServer, sizeof(addrServer)) == SOCKET_ERROR)
    {
        printf("��ʧ��!");
        exit(1);
    }
    // ����
    if (listen(socketServer, 10) == SOCKET_ERROR)
    {
        printf("����ʧ��!");
        exit(1);
    }
    while (true)
    {
        printf("Listening ... \n");
        sockaddr_in addrClient;
        int nClientAddrLen = sizeof(addrClient);
        //�������˽�������
        SOCKET socketClient = accept(socketServer, (sockaddr*)&addrClient, &nClientAddrLen);
        if (SOCKET_ERROR == socketClient)
        {
            printf("����ʧ��!");
            break;
        }
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        //��������
        if (recv(socketClient, buffer, BUFFER_SIZE, 0) < 0)
        {
            printf("��������ʧ��!");
            break;
        }
        printf("���յ������� : \n%s", buffer);
        // response
        // send header
        memset(buffer, 0, BUFFER_SIZE);
        sprintf(buffer, HEADER, GetFileLength(strPath));//���ļ���ͷ�ļ��ϲ�Ȼ��������
        if (send(socketClient, buffer, strlen(buffer), 0) < 0)
        {
            printf("��������ʧ��!");
            break;
        }
        //��ȡ�ı�
        ifstream fin(strPath.c_str(), ios::in | ios::binary);
        if (fin.is_open())
        {
            memset(buffer, 0, BUFFER_SIZE);
            //ÿ�ζ�ȡһ���ֽ�
            while (fin.read(buffer, BUFFER_SIZE - 1))
            {
                if (send(socketClient, buffer, strlen(buffer), 0) < 0)
                {
                    printf("��������ʧ��!");
                    break;
                }
                memset(buffer, 0, BUFFER_SIZE);
            }
            if (send(socketClient, buffer, strlen(buffer), 0) < 0)
            {
                printf("��������ʧ��!");
                break;
            }
        }
        //printf("����!");
        //�����������������ݴ���
        fin.close();
        closesocket(socketClient);
    }
    closesocket(socketServer);
    WSACleanup();
    return 0;
}

//��������ǻ�ȡ�ļ��Ĵ�С��
long GetFileLength(string strPath)
{
    ifstream fin(strPath.c_str(), ios::in | ios::binary);
    fin.seekg(0, ios_base::end);
    streampos pos = fin.tellg();//���ص�ǰ�ļ�λ��
    long lSize = static_cast<long>(pos);
    fin.close();
    cout << "content��" << lSize << endl;
    return lSize;
}
