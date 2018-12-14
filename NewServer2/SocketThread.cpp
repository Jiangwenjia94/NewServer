#include "SocketThread.h"
//#

DWORD WINAPI ClientSockProc(LPVOID lpParameter)
{
	USER user;
	user.isfirt = true;
	ClientParam *t_client = (ClientParam*)lpParameter;
	DataJudgment DataJud;//ÿ���½��Ŀͻ��˶���һ��������DataJudgment
	DataJud.Trans.Set(t_client);
	SocketThread* st = (SocketThread*)t_client->pThis;
	DataJud.m_client = &st->client;//һ����client
//	DataTrans.param.ClientSocket = t_client->ClientSocket;
	DataJud.user = &user;
	bool fir = true;
	while (1)
	{
		int order = 0;
		order = DataJud.recvHeart(DataJud.user->isfirt);
		if (order == 0)
		{
			DataJud.user->isfirt = false;
		}
		else
		{
			break;
		}
	}
	return 0;
}
///////////////////////////////////////////////////////
SocketThread::SocketThread()
{
}


SocketThread::~SocketThread()
{
}

void SocketThread::StartServerSock(struct ClientParam *pClientSockParam)
{
	DWORD dwThreadID = 0;
	pClientSockParam->pThis = this;
	HANDLE handleClientSockThread = CreateThread(NULL, 0, ClientSockProc, (LPVOID)pClientSockParam, 0, &dwThreadID);//Ϊÿ���û�����һ���߳�
//	CloseHandle(handleClientSockThread);
	if (handleClientSockThread == NULL)
	{
		Logevent.WriteLog("���ܴ���ͬ�ͻ��������߳�");
#ifdef PRINT_DEBUG
		printf("���ܴ���ͬ�ͻ��������߳�");
#endif
	}
}

void SocketThread::StartClientSendOrderSock(struct ClientParam *pClientSockParam)
{
	DataExPortable DP;
	DP.Trans.Set(pClientSockParam);
	bool fir = true;
	while (1)
	{
		int order = 0;
		int key = 0;
		cout << "����1" << endl;
		cin >> key;
		if (key == 1)
		{
			order = DP.OrderSend(fir);
		}
		if (order >= 0)
		{
			fir = false;
		}
		else
		{
			break;
		}
	}
}

void SocketThread::StartClientRecvOrderSock(struct ClientParam *pClientSockParam)
{
	DataExPortable DP;
	DP.Trans.Set(pClientSockParam);
	bool fir = true;
	while (1)
	{
		int order = 0;
		order = DP.OrderRecv(fir);
		if (order >= 0)
		{
			fir = false;
		}
		else
		{
			break;
		}
	}
}

void SocketThread::StartClientRecvDataSock(struct ClientParam *pClientSockParam)
{
	DataExPortable DP;
	DP.Trans.Set(pClientSockParam);
	bool fir = true;
	while (1)
	{
		int order = 0;
		int key = 0;
		cout << "����1" << endl;
		cin >> key;
		if (key == 1)
		{
			order = DP.DataRec(fir);
		}
		if (order >= 0)
		{
			fir = false;
		}
		else
		{
			break;
		}
	}
}