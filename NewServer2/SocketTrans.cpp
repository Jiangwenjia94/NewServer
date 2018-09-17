#include "stdafx.h"
#include "SocketTrans.h"

#define PRINT_DEBUG 2

SocketTrans::SocketTrans()
{
	m_param = NULL;
}

SocketTrans::~SocketTrans()
{
}

void SocketTrans::Set(ClientParam *param)
{
	m_param = param;
}

int SocketTrans::Recv(char *buf, int len)
{
	int rec = recv(m_param->ClientSocket, buf, len, 0);
	int i = 0;
	return rec;
}

int SocketTrans::Recv(int &order)
{
	int rec = recv(m_param->ClientSocket, (char *)&order, sizeof(int), 0);

	return rec;
}

int SocketTrans::Send(int order)
{
	int sen = send(m_param->ClientSocket, (char *)&order, sizeof(int), 0);

	return sen;
}

int SocketTrans::Send( char *buf, int len)
{
	int sen = send(m_param->ClientSocket, buf, len, 0);

	return sen;
}

//float SocketTrans::Send(float order)
//{
//
//}

UINT SocketTrans::sendCurrentPic(CString path)
{
	SOCKET serverSocket = m_param->ClientSocket;
	std::string strError;
	CFile file;
	int error = 0;
	LPCTSTR Path = path;
	if (!file.Open(Path, CFile::modeRead | CFile::typeBinary))
	{
		//	AfxMessageBox("���ļ�ʧ��");
#ifdef PRINT_DEBUG
		printf("���ļ�ʧ��\n");
#endif
		Logevent.WriteLog("���ļ�ʧ��");
		return 1;
	}
	char dataType = '1';
	int lFileSize = file.GetLength();
	char temp[1010];							//������������ļ���С���������������ļ�������
	memset(temp, 0, sizeof(temp));

	int ret1 = send(serverSocket, (char *)&lFileSize, sizeof(int), 0);
	//	int ret1 = send(serverSocket, c, str.GetLength(), 0);	//�����ļ������ƺʹ�С
	if (ret1 == SOCKET_ERROR)
	{
#ifdef PRINT_DEBUG
		printf("Send File: [%d]\n",WSAGetLastError());
#endif
		Logevent.WriteLog("Send File: " + WSAGetLastError());
		return 1;
	}

	Sleep(100);//���̫�콫������һ�η��͵ģ����ڻ�����δ���������send�ķ�һ��
	//��������recv����˵��»��ٽ���һ��
	/////////////////////////�����ļ�///////////////////////////////////////////
	int iEnd = 0;
	int ilen = 0;
	long iTmp = 0;
	int p = 0;
	while (1)
	{
		memset(temp, 0, sizeof(temp));
		ilen = file.Read(temp, 1010);
		if (ilen == 0)
		{
			break;
		}
		/*if(p == 105)
		{
		cout<<"wait for 5 sec"<<endl;
		Sleep(5000);
		}*/
		iEnd = send(serverSocket, temp, ilen, 0);
		if (iEnd == SOCKET_ERROR)
		{
#ifdef PRINT_DEBUG
			printf("Send File: [%d]\n", WSAGetLastError());
#endif
			Logevent.WriteLog("Send File: " + WSAGetLastError());
			break;
		}
		iTmp += iEnd;
		p++;
//		cout << iTmp << " : " << iEnd << " : " << p << endl;
#ifdef PRINT_DEBUG
		printf("[%d] : [%d] : [%d]\n", iTmp,iEnd,p);
#endif
		char temp2[2];
		memset(temp2, 0, sizeof(temp2));
		int t2 = recv(serverSocket, temp2, 1, 0);
//		cout << temp2 << endl;
#ifdef PRINT_DEBUG
		printf("[%d]\n", temp2);
#endif
	}
	file.Close();

	/////////////////�жϷ����Ƿ�ɹ�����������////////////////////////////////////
	//	closesocket(LocalSocket);
	if (iTmp == lFileSize)
	{
#ifdef PRINT_DEBUG
		printf("�ļ����ͳɹ�\n");
#endif
	}
	else
	{
#ifdef PRINT_DEBUG
		printf("�ļ�����ʧ��\n");
#endif
		Logevent.WriteLog("�ļ�����ʧ��");
		return 0;
	}

	//shutdown();
	//	ExitThread(0);
	return 0;
}