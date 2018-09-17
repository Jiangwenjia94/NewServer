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

	vector<USER> client;//存储每一个用户
	ClientParam Clientparam;
	LogEvent Logevent;
//	HANDLE ThreadEvent[20];
};

