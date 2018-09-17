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
		//	AfxMessageBox("打开文件失败");
#ifdef PRINT_DEBUG
		printf("打开文件失败\n");
#endif
		Logevent.WriteLog("打开文件失败");
		return 1;
	}
	char dataType = '1';
	int lFileSize = file.GetLength();
	char temp[1010];							//首先用来存放文件大小，后面用作发送文件缓冲区
	memset(temp, 0, sizeof(temp));

	int ret1 = send(serverSocket, (char *)&lFileSize, sizeof(int), 0);
	//	int ret1 = send(serverSocket, c, str.GetLength(), 0);	//发送文件的名称和大小
	if (ret1 == SOCKET_ERROR)
	{
#ifdef PRINT_DEBUG
		printf("Send File: [%d]\n",WSAGetLastError());
#endif
		Logevent.WriteLog("Send File: " + WSAGetLastError());
		return 1;
	}

	Sleep(100);//如果太快将导致上一次发送的（由于缓冲区未满）与后面send的放一起，
	//被服务器recv，因此导致会少接收一次
	/////////////////////////发送文件///////////////////////////////////////////
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

	/////////////////判断发送是否成功、结束处理////////////////////////////////////
	//	closesocket(LocalSocket);
	if (iTmp == lFileSize)
	{
#ifdef PRINT_DEBUG
		printf("文件发送成功\n");
#endif
	}
	else
	{
#ifdef PRINT_DEBUG
		printf("文件发送失败\n");
#endif
		Logevent.WriteLog("文件发送失败");
		return 0;
	}

	//shutdown();
	//	ExitThread(0);
	return 0;
}