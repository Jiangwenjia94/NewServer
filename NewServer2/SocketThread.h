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

	void StartClientOrderSock(ClientParam* pClientSockParam);

	void StartClientDataSock(ClientParam* pClientSockParam);

	vector<USER> client;//�洢ÿһ���û�
	ClientParam Clientparam;
	LogEvent Logevent;
//	HANDLE ThreadEvent[20];
};

