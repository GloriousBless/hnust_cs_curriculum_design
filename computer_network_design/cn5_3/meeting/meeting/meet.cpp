#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#include<winsock2.h>
#include<ws2tcpip.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<process.h>

using namespace std;

#define MCASTADDR "224.0.0.1"                   // �ಥ���ַ
#define MCASTPORT 5150                          // ���ض˿ں�
#define BUFFERSIZE 1024                         // ��������С


void sendMsg(void* sock);
void recvMsg(void* sock);

//�ಥ��ʵ�����൱��һ�������ң��κγ�Ա���鲥Դ�����������У��鲥 IP��������Ϣ���鲥���ݣ��������������ĳ�Ա���鲥�����ߣ�������յ�����Ϣ��
//�ಥ��ʵҲ��������ͬ�ľ������У�ֻ�м���ಥ��ĳ�Ա���ܽ��յ����ݣ�����ָ���ض��ĳ�Ա�������ݡ�
// ��ͬ�ڹ㲥�����ӵ��������Ŀͻ��˲����벻�붼����յ��㲥���ݣ������Ҿ��öಥ��һ�㡣

//�������Ҵ����������̣߳�����ͬʱ���ܺͷ�������
int main(int argc, char** argv)
{

    WORD wVersionRequested;                                 // ˫�ֽ�
    WSADATA wsaData;                                        // WinSock ��汾�������Ϣ
    wVersionRequested = MAKEWORD(2, 2);                   // ʹ�õ� WinSock �İ汾

    // �����׽��ֿ�
    int err = WSAStartup(wVersionRequested, &wsaData);    // ���� WinSock �Ⲣȷ�� WinSock �汾��ϵͳ����������� wsaData ��
    if (err != 0)
    {
        printf("WSASTARTUP_ERROR: %d\n", err);
        return 0;
    }

    // ���� SOCK_DGRAM ���͵��׽���
    SOCKET socketClient;
    SOCKET socketClient1;
    socketClient = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF | WSA_FLAG_OVERLAPPED);

    // ���ض�
    sockaddr_in local;
    local.sin_family = AF_INET;                               // IPv4
    local.sin_port = htons(MCASTPORT);                        // 5150 �󶨵ı��ض˿ں�
    local.sin_addr.s_addr = INADDR_ANY;                       // ����������IP  inet_addr("192.168.157.172")  INADDR_ANY

    // ���׽��ֵ�һ�� IP ��ַ��һ���˿���
    bind(socketClient, (struct sockaddr*)&local, sizeof(local));

    // �ಥ��
    sockaddr_in remote;
    remote.sin_family = AF_INET;                              // IPv4
    remote.sin_port = htons(MCASTPORT);                       // 5150 �󶨵ı��ض˿ں�
    remote.sin_addr.s_addr = inet_addr(MCASTADDR);            // ����ʹ�õĶಥ��IP��ַ

    // ����������ಥ��,WSAJoinLeaf()�������ֱ�Ϊ���׽ӿڡ�Զ�����ơ����ȡ�ָ�롢��־λJL_BOTH����ʾ���ǽ�����Ҳ�Ƿ����ߣ�
    if ((socketClient1 = WSAJoinLeaf(socketClient, (SOCKADDR*)&remote, sizeof(remote), NULL, NULL, NULL, NULL, JL_BOTH)) == INVALID_SOCKET)
    {
        printf("WSAJoinLeaf() failed:%d\n", WSAGetLastError());
        closesocket(socketClient);
        WSACleanup();
        return 0;
    }

    cout << "���ڼ����ಥ�飺" << MCASTADDR << "\n" << endl;

    // ���߳�
    HANDLE hHandle[2];
    // _beginthread() �������ڴ���һ���̣߳����������ֱ�Ϊ���̺߳����ĵ�ַ�����̵߳Ķ�ջ��С����Ϊ0��ʾ�����߳�ʹ��һ���Ķ�ջ���������б�û�в���ʱΪNULL��
    hHandle[0] = (HANDLE)_beginthread(sendMsg, 0, (void*)socketClient1);
    hHandle[1] = (HANDLE)_beginthread(recvMsg, 0, (void*)socketClient);

    // �ȴ������߳�ִ�����
    WaitForSingleObject(hHandle[0], INFINITE);

    // �رռ��ص��׽��ֿ�
    WSACleanup();

    return 0;
}

// ������Ϣ����
void sendMsg(void* sock) {

    SOCKET server = (SOCKET)sock;
    char sendBuffer[BUFFERSIZE];

    // �ಥ��
    sockaddr_in remote;
    memset(&remote, 0, sizeof(remote));
    remote.sin_family = AF_INET;                              // IPv4
    remote.sin_port = htons(MCASTPORT);                       // 5150 �󶨵ı��ض˿ں�
    remote.sin_addr.s_addr = inet_addr(MCASTADDR);            // ʹ�õĶಥ��IP��ַ

    while (true)

    {
        printf("������Ϣ:");
        scanf("%[^\n]", sendBuffer);
        getchar();
        if (strncmp(sendBuffer, "quit", 4) == 0)                   // ����quit�˳�
        {
            printf("\n�������\n\n");
            return;
        }
        if (sendto(server, sendBuffer, strlen(sendBuffer), 0, (sockaddr*)(&remote), sizeof(remote)) == SOCKET_ERROR)
        {
            printf("��Ϣ����ʧ��!\n");
            break;
        }
        Sleep(200);     // �Ե�һ�ᣬ���ڽ���֮����Ⱥ�
    }

    // �ر��׽���
    closesocket(server);
}

// ������Ϣ����
void recvMsg(void* sock) {

    SOCKET server = (SOCKET)sock;

    // ��������
    sockaddr_in from;                       // �Զ� socket ��ַ
    int iFromLen = sizeof(from);           // ��ַ�ṹ��С

    char recvBuffer[BUFFERSIZE + 1];
    int iReadDataLen;                           // �������ݳ���

    while (true)
    {
        iReadDataLen = recvfrom(server, recvBuffer, BUFFERSIZE, 0, (sockaddr*)&from, &iFromLen);
        if (iReadDataLen > 0)                    // �����ݵ���
        {
            recvBuffer[iReadDataLen] = '\0';    // ���������ʾ�쳣������
            printf("IPΪ %s ���û�˵: %s\n", inet_ntoa(from.sin_addr), recvBuffer);
        }
        else if (iReadDataLen < 0)
        {
            cout << "recvMSG failed:" << WSAGetLastError() << endl;
            break;
        }
        memset(recvBuffer, 0, BUFFERSIZE + 1);
    }

    // �ر��׽���
    closesocket(server);
}
