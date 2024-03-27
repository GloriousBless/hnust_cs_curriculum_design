
#include "windows.h"
#include "iostream"
#include "process.h"
#include <string>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

//��������֮���������
void Receive(void* param)
{
	string msg;
	while (1)
	{
		//�ͻ��˽������Է�����������
		SOCKET clientSocket = *(SOCKET*)(param);
		char  recvbuf[2048] = {};		//���ջ�����
		if (recv(clientSocket, recvbuf, 2048, 0) == SOCKET_ERROR)
		{
			cout << "���ݽ���ʧ��" << endl;
		}
		else
		{
			msg = recvbuf;
			char sendPlayerName[5] = { 0 };
			int len = strlen(recvbuf);		//��Ϣ����
			copy(&recvbuf[1], &recvbuf[5], sendPlayerName);	//���������
			msg = msg.substr(5, len - 5);
			cout << sendPlayerName << "����˵��" << msg << endl;
		}

	}
}

//��������֮��������
void Send(void* param)
{
	while (1)
	{
		//�ͻ��˷������ݸ�������
		SOCKET clientSocket = *(SOCKET*)(param); //���غʹ���Socket
		char sendbuf[2048] = {};		//���ͻ�����
		cin.getline(sendbuf, 2048);
		if (send(clientSocket, sendbuf, strlen(sendbuf), 0) == SOCKET_ERROR)
		{
			cout << "������Ϣʧ�ܣ�";
		}
		else
			cout << "������Ϣ�ɹ�" << endl;
	}
}

//ͬ��ҲҪ�����׽��ּ��أ���������������������ķ�������ͨ��
int main()
{
	cout << "-----------���˿ͻ���-----------" << endl;
	WSADATA  wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "�׽��ֳ�ʼ��ʧ��!" << endl;
	}
	SOCKET clientSocket;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "�׽��ִ���ʧ��!" << endl;
	}
	Sleep(30);
	struct sockaddr_in ClientAddress;		//һ���󶨵�ַ:��IP��ַ,�ж˿ں�,��Э����
	memset(&ClientAddress, 0, sizeof(sockaddr_in)); //��ʼ���ṹ��
	ClientAddress.sin_family = AF_INET;
	ClientAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//���뱾��IP��ַ 
	//ClientAddress.sin_port = htons(60001);//�趨�˿ں�
	//���׽���     ָ���󶨵�IP��ַ�Ͷ˿ں�
	if (bind(clientSocket, (sockaddr*)&ClientAddress, sizeof(ClientAddress)) == SOCKET_ERROR)
	{
		cout << "�׽��ְ�ʧ�ܣ�" << endl;
		return 0;
	}
	struct sockaddr_in SeverAddress;		//��������ַ Ҳ���Ǽ���Ҫ���ӵ�Ŀ���ַ
	memset(&SeverAddress, 0, sizeof(sockaddr_in));
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //127.0.0.1��ʾ����ip��ַ
	SeverAddress.sin_port = htons(60000);//�趨�˿ں�

	//�������������������
	if (connect(clientSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "�ͻ��ˣ��ͷ���������ʧ�ܣ�" << endl;
		return 0;
	}
	else
		cout << "����������ӳɹ���" << endl;

	//�����������߳�
	_beginthread(Receive, 0, &clientSocket);
	_beginthread(Send, 0, &clientSocket);

	Sleep(INFINITE);	//�����������һ�ּ����������߳�ִ�����˳�����ʹ������������
	//	�ر�socket
	if (clientSocket != INVALID_SOCKET) {
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}
	return 0;
}
