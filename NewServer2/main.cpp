// NewServer2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "NewServer2.h"
#include "BluetoohServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	BluetoohServer BS;
	printf("Server\n");
	BS.start(LAN);
	printf("test\n");
	return 0;
}
