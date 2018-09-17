#pragma once

#include "SocketThread.h"
#include <stdio.h>
#include <cstring>
#include <initguid.h>
#include <winsock2.h>
#include <ws2bth.h>
#include <strsafe.h>

#define SER_SUCCESS                       0
#define SER_ERROR                         1
#define MAX_COMPUTER_NAME                 15
#define BTH								  2
#define LAN								  3
#define INSTANCE_STRING					  "BlueTooth"

DEFINE_GUID(g_guidServiceClass, 0xb62c4e8d, 0x62cc, 0x404b, 0xbb, 0xbf, 0xbf, 0x3e, 0x3b, 0xbb, 0x13, 0x74);

//#define PRINT_DEBUG						  2

class BluetoohServer
{
public:
	BluetoohServer();
	~BluetoohServer();

	int start(int sw);
	int stop();

	void charTowchar(const char *ch, wchar_t *wch, int size);
	void wcharTochar(const wchar_t *wchar, char *chr, int length);

	DWORD WINAPI RunLANServerMode(LPVOID P);		//局域网服务端

	DWORD WINAPI RunBTHServerMode(LPVOID P);		//蓝牙服务端

//	DWORD WINAPI RunLANClientOrderMode(LPVOID P);	//局域网客户端（Order）

	DWORD WINAPI RunLANClientDataMode(LPVOID P);	//局域网客户端（Data）

	LogEvent Logevent;
	SocketThread Socketthread;
};

