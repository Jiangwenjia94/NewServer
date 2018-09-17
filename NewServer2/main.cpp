// NewServer2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "NewServer2.h"
#include "BluetoohServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

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
