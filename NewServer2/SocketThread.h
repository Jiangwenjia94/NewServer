#pragma once
#include "DataJudgment.h"
#include "DataExPortable.h"
#include <vector>
//#include "ClientParam.h"


class SocketThread
{
public:
	SocketThread();
	~SocketThread();

	void StartServerSock(ClientParam* pClientSockParam);

	void StartClientSendOrderSock(ClientParam* pClientSockParam);
	void StartClientRecvOrderSock(ClientParam* pClientSockParam);

	void StartClientSendDataSock(ClientParam* pClientSockParam);
	void StartClientRecvDataSock(ClientParam* pClientSockParam);

	vector<USER> client;//�洢ÿһ���û�
	ClientParam Clientparam;
	LogEvent Logevent;
//	HANDLE ThreadEvent[20];
};

