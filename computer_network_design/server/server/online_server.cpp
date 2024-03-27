
#include <iostream>  
#include "windows.h" //һ��Ҫ������ͷ�ļ�
#include "process.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;
#pragma comment(lib, "WS2_32.lib")  //��ʾ���� ws2_32.dll	ws2_32.dll��������socket�汾

int g_curPlayerNum = 0; //��ǰ������
const char* g_PlayerName[] =		//�ٶ�������������
{
	"aaaa",
	"bbbb",
	"cccc",
	"dddd",
};

struct PlayerInfo	//���ýṹ�洢���ӵĿͻ���
{
	SOCKET sock;
	string name;
};

vector<PlayerInfo>g_clientSockList;		//����vector��ȡ�����ӵĿͻ���

void process(void* param)
{
	int index = *(int*)param;	//��ǰ���̱߳��
	while (1)
	{
		//������������Ϣ
		//int index = *(int*)param;
		char buf[2048] = { 0 };  //���ջ�����
		int bytes;
		if ((bytes = recv(g_clientSockList[index].sock, buf, sizeof(buf), 0)) == SOCKET_ERROR)
		{
			cout << "��������������ʧ�ܣ�" << endl;
		}
		//������ת�������߼�����
		if (buf[0] == '@')
		{
			//˽��
			string Buf(buf);
			string recvPlayerName = Buf.substr(1, 4);	//���������������
			copy(g_clientSockList[index].name.begin(), g_clientSockList[index].name.end(), &buf[1]);

			for (vector<PlayerInfo>::iterator it = g_clientSockList.begin(); it != g_clientSockList.end(); it++)
			{
				if (it->name == recvPlayerName)
				{
					if (send(it->sock, buf, strlen(buf), 0) == SOCKET_ERROR)
					{
						cout << "��������ʧ�� to" << it->name << endl;
					}
					break;
				}
			}
		}
		else
			//Ⱥ��
			cout << g_clientSockList[index].name << "��" << "������˵��" << buf << endl;
	}
}

//�����׽��֣������׽��ֵ�IP��ַ�Ͷ˿�
//������listen()�����������ȴ��ͻ�������
//����������ܣ������µĶ�Ӧ�˴����ӵ��׽���
//�����÷��ص��׽�����ͻ���ͨ��
//���أ��ȴ���һ���ͻ��˵���������
//�����Ҫ�ˣ��Ϳ��Թر��׽����ˡ�
int main()
{
	cout << "-----------�����ҷ�����-----------" << endl;

	//�׽��ֳ�ʼ��
	WSADATA wsaData;	//����ṹ�������洢��WSAStartup�������ú󷵻ص� Windows Sockets ���ݡ�
	WORD sockVersion = MAKEWORD(2, 2);	//windows�����̿�İ汾����Ϣ
	if (WSAStartup(sockVersion, &wsaData) != 0) //WSAStartup�������ڳ����г�ʼ��������Windows����
	{
		cout << "�׽��ֳ�ʼ��ʧ��!" << endl;
		return 0;
	}

	//�����������׽���
	SOCKET SeverSocket;
	if ((SeverSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		cout << "�׽��ִ���ʧ�ܣ�" << endl;
		return 0;
	}
	struct sockaddr_in SeverAddress;		//һ���󶨵�ַ:��IP��ַ,�ж˿ں�,��Э����
	memset(&SeverAddress, 0, sizeof(sockaddr_in)); //��ʼ���ṹ��
	SeverAddress.sin_family = AF_INET;
	SeverAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//���뱾��IP��ַ 
	SeverAddress.sin_port = htons(60000);//�趨�˿ں�

	//���׽���     ָ���󶨵�IP��ַ�Ͷ˿ں�
	if (bind(SeverSocket, (sockaddr*)&SeverAddress, sizeof(SeverAddress)) == SOCKET_ERROR)
	{
		cout << "�׽��ְ�ʧ�ܣ�" << endl;
		return 0;
	}

	//����������	,�ȴ���������
	if (listen(SeverSocket, SOMAXCONN) == SOCKET_ERROR) //�����ĵڶ�����������:�ܴ�Ŷ��ٸ��ͻ�������
	{
		cout << "����ʧ�ܣ�" << endl;
		return 0;
	}
	else
		cout << "�������ȴ�����......" << endl;


	while (1)
	{
		//������������������
		sockaddr_in revClientAddress;	//�׽��ֵĵ�ַ���˿�
		SOCKET revClientSocket = INVALID_SOCKET;		//�������տͻ�������
		//memset(&revClientAddress, 0, sizeof(revClientAddress));
		int addlen = sizeof(revClientAddress);
		// �������󣬽����������󣬷���һ���µĶ�Ӧ�ڴ˴����ӵ��׽���
		if ((revClientSocket = accept(SeverSocket, (sockaddr*)&revClientAddress, &addlen)) == INVALID_SOCKET)
		{
			cout << "���ܿͻ�������ʧ�ܣ�" << endl;
			return 0;
		}

		PlayerInfo stPlayerInfo;
		stPlayerInfo.sock = revClientSocket;
		stPlayerInfo.name = g_PlayerName[g_curPlayerNum];
		g_clientSockList.push_back(stPlayerInfo);
		int temp = g_curPlayerNum;
		_beginthread(process, 0, &temp);		//�������߳����շ�����
		g_curPlayerNum++;
		cout << stPlayerInfo.name << "��������" << endl;
	}
	return 0;
}

