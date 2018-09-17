#pragma once
#include <afx.h>
#include <cstring>
#include <string>
#include "ClientParam.h"
#include "LogEvent.h"
class SocketTrans
{
public:
	SocketTrans();
	~SocketTrans();
	void Set(ClientParam *param);

	int Recv(char *buf, int len);
	int Recv(int &order);

	int Send(char *buf, int len);
	int Send(int order);
//	float Send(float order);

	UINT sendCurrentPic(CString path);

	LogEvent Logevent;
private:
	ClientParam *m_param;
};

