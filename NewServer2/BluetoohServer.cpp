#include "BluetoohServer.h"


void BluetoohServer::charTowchar(const char *ch, wchar_t *wch, int size)
{
	MultiByteToWideChar(CP_ACP, 0, ch,
		strlen(ch) + 1, wch, size / sizeof(wch[0]));
}

void BluetoohServer::wcharTochar(const wchar_t *wchar, char *chr, int length)
{
	WideCharToMultiByte(CP_ACP, 0, wchar, -1,
		chr, length, NULL, NULL);
}
DWORD WINAPI RunLANClientOrderMode(LPVOID P)//与第三方传输的Order主线程
{
	int ulRetCode = SER_SUCCESS;
	SOCKET clientSock = socket(PF_INET, SOCK_STREAM, 0);
	//初始化socket信息
	sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(SOCKADDR));
	//clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_family = PF_INET;
	clientAddr.sin_port = htons(3001);

	if (SOCKET_ERROR == connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR)))
	{
#ifdef PRINT_DEBUG
		printf("=CRITICAL= | connect() failed. WSAGetLastError = [%d]\n", WSAGetLastError());
#endif // PRINT_DEBUG

		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | socket() call failed. WSAGetLastError = " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}
	struct ClientParam *Con = new ClientParam;
	Con->ClientSocket = clientSock;
//	Con->clntAddr = (SOCKADDR)clientAddr;//有瑕疵
//	((BluetoohServer*)P)->Socketthread.StartClientSendOrderSock(Con);
	((BluetoohServer*)P)->Socketthread.StartClientRecvOrderSock(Con);
	return 0;
}

DWORD WINAPI BluetoohServer::RunLANClientDataMode(LPVOID P)//与第三方传输的Data主线程
{
	int ulRetCode = SER_SUCCESS;
	SOCKET clientSock = socket(PF_INET, SOCK_STREAM, 0);
	//初始化socket信息
	sockaddr_in clientAddr;
	memset(&clientAddr, 0, sizeof(SOCKADDR));
	//clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_family = PF_INET;
	clientAddr.sin_port = htons(3075);

	if (SOCKET_ERROR == connect(clientSock, (SOCKADDR*)&clientAddr, sizeof(SOCKADDR)))
	{
#ifdef PRINT_DEBUG
		printf("=CRITICAL= | connect() failed. WSAGetLastError = [%d]\n", WSAGetLastError());
#endif // PRINT_DEBUG

		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | socket() call failed. WSAGetLastError = " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}
	struct ClientParam *Con = new ClientParam;
	Con->ClientSocket = clientSock;
	//	Con->clntAddr = (SOCKADDR)clientAddr;//有瑕疵
	//	((BluetoohServer*)P)->Socketthread.StartClientSendOrderSock(Con);
	((BluetoohServer*)P)->Socketthread.StartClientRecvDataSock(Con);
	return ulRetCode;
}

DWORD WINAPI BluetoohServer::RunLANServerMode(LPVOID P)//LAN主线程
{
	int ulRetCode = SER_SUCCESS;
	sockaddr_in		servAddr;
	SOCKET			LocalSocket = INVALID_SOCKET;
	sockaddr_in		clntAddr;
	sockaddr		tcpaddr;
	SOCKET			ClientSocket = INVALID_SOCKET;
	int				size = 0;

	LocalSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (LocalSocket == INVALID_SOCKET)
	{

#ifdef PRINT_DEBUG
		printf("=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
#endif // PRINT_DEBUG

		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | socket() call failed. WSAGetLastError = " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}

	memset(&servAddr, 0, sizeof(SOCKADDR)); //初始化socket地址
	servAddr.sin_family = PF_INET; //设置使用的协议族
	servAddr.sin_port = htons(2017); //设置使用的端口
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //define s_addr = S_un.S_addr

	if (SOCKET_ERROR == bind(LocalSocket, (SOCKADDR *)&servAddr, sizeof(SOCKADDR)))
	{
#ifdef PRINT_DEBUG
		printf("=CRITICAL= | bind() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n",
			LocalSocket, WSAGetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | bind() call failed w/socket = [0x%X]. WSAGetLastError= " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}


	if (SOCKET_ERROR == listen(LocalSocket, SOMAXCONN))
	{

#ifdef PRINT_DEBUG
		printf("=CRITICAL= | listen() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | listen() call failed w/socket = [0x%X]. WSAGetLastError= " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}
#ifdef PRINT_DEBUG
	printf("等待连接...\n");
#endif
	size = sizeof(struct sockaddr);
	while (1)//开始循环接收Client
	{
		ClientSocket = accept(LocalSocket, (struct sockaddr *)&tcpaddr, (int *)&size);
		if (INVALID_SOCKET == ClientSocket)
		{
#ifdef PRINT_DEBUG
			printf("获取的SOCKET无效\n");
#endif
			((BluetoohServer*)(P))->Logevent.WriteLog("获取的SOCKET无效");
			break;
		}
		else if (INVALID_SOCKET == ClientSocket)
		{
#ifdef PRINT_DEBUG
			printf("不能获得用户连接的SOCK");
#endif
			((BluetoohServer*)(P))->Logevent.WriteLog("不能获得用户连接的SOCK");
			break;
		}
		else
		{
#ifdef PRINT_DEBUG
			printf("连接成功\n");
#endif
			
			struct ClientParam *Con = new ClientParam;
			Con->ClientSocket = ClientSocket;
			Con->clntAddr = tcpaddr;//有瑕疵
			((BluetoohServer*)(P))->Socketthread.StartServerSock(Con);//建立每个Client的子线程
		}
	}
	if (INVALID_SOCKET != LocalSocket)
		closesocket(LocalSocket);
	return 9999;//乱写的数字
}

DWORD WINAPI BluetoohServer::RunBTHServerMode(LPVOID P)//BTH主线程
{
	int ulRetCode = SER_SUCCESS;
	SOCKADDR_BTH	servAddr = {0};
	SOCKET			LocalSocket = INVALID_SOCKET;
	sockaddr_in		clntAddr = {0};
	sockaddr		tcpaddr = {0};
	char *			pszInstanceName = NULL;
	char			szThisComputerName[MAX_COMPUTER_NAME+512];
	DWORD			dwLenComputerName = MAX_COMPUTER_NAME+1;
	SOCKET			ClientSocket = INVALID_SOCKET;
	WSAQUERYSET		wsaQuerySet = { 0 };
	LPCSADDR_INFO	lpCSAddrInfo = NULL;
	int				iAddrLen = sizeof(SOCKADDR_BTH);
	int				size = 0;
	size_t			cbInstanceNameSize;
	HRESULT			res;

	lpCSAddrInfo = (LPCSADDR_INFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(CSADDR_INFO));

	if (NULL == lpCSAddrInfo)
	{
#ifdef PRINT_DEBUG
		printf("!ERROR! | Unable to allocate memory for CSADDR_INFO\n",
			LocalSocket, WSAGetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("!ERROR! | Unable to allocate memory for CSADDR_INFO");
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}

	wchar_t wchar[512];
	charTowchar(szThisComputerName, wchar, 512);
	if (!GetComputerName(wchar, &dwLenComputerName))
	{
#ifdef PRINT_DEBUG
		printf("=CRITICAL= | GetComputerName() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n",
			LocalSocket, WSAGetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | GetComputerName() call failed w/socket = [0x%X]. WSAGetLastError= " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}
	wcharTochar(wchar, szThisComputerName, MAX_COMPUTERNAME_LENGTH + 1);

	LocalSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (LocalSocket == INVALID_SOCKET)
	{

#ifdef PRINT_DEBUG
		printf("=CRITICAL= | socket() call failed. WSAGetLastError = [%d]\n", WSAGetLastError());
#endif // PRINT_DEBUG

		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | socket() call failed. WSAGetLastError = " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}
	servAddr.addressFamily = AF_BTH;
	servAddr.port = BT_PORT_ANY;

	if (SOCKET_ERROR == bind(LocalSocket, (SOCKADDR *)&servAddr, sizeof(SOCKADDR_BTH)))
	{
#ifdef PRINT_DEBUG
		printf("=CRITICAL= | bind() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n",
			LocalSocket, WSAGetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | bind() call failed w/socket = [0x%X]. WSAGetLastError= " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}

	ulRetCode = getsockname(LocalSocket, (SOCKADDR *)&servAddr,&iAddrLen);
	if (SOCKET_ERROR == ulRetCode)
	{
#ifdef PRINT_DEBUG
		printf("=CRITICAL= | getsockname() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n",
			LocalSocket, WSAGetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | bind() call failed w/socket = [0x%X]. WSAGetLastError= " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}

	lpCSAddrInfo[0].LocalAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	lpCSAddrInfo[0].LocalAddr.lpSockaddr = (LPSOCKADDR)&servAddr;
	lpCSAddrInfo[0].RemoteAddr.iSockaddrLength = sizeof(SOCKADDR_BTH);
	lpCSAddrInfo[0].RemoteAddr.lpSockaddr = (LPSOCKADDR)&servAddr;
	lpCSAddrInfo[0].iSocketType = SOCK_STREAM;
	lpCSAddrInfo[0].iProtocol = BTHPROTO_RFCOMM;

	ZeroMemory(&wsaQuerySet,sizeof(WSAQUERYSET));
	wsaQuerySet.dwSize = sizeof(WSAQUERYSET);
	wsaQuerySet.lpServiceClassId = (LPGUID)&g_guidServiceClass;

	
	charTowchar(szThisComputerName,wchar,512);
	res = StringCchLength(wchar, sizeof(szThisComputerName), &cbInstanceNameSize);

	if (FAILED(res))
	{
#ifdef PRINT_DEBUG
		printf("=FATAL= | ComputetName specified is too large\n");
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=FATAL= | ComputetName specified is too large");
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}

	cbInstanceNameSize += sizeof(INSTANCE_STRING) + 1;
	pszInstanceName = (LPSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,512);

	if (NULL == pszInstanceName)
	{
#ifdef PRINT_DEBUG
		printf("=FATAL= | HeapAlloc faile out of memory gle = [%d]\n",GetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=FATAL= | HeapAlloc faile out of memory gle = " + GetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}

	wchar_t wInstance[512];
	charTowchar(szThisComputerName,wInstance,512);

	wsaQuerySet.lpszServiceInstanceName = wInstance;
	wsaQuerySet.lpszComment = L"Example";
	wsaQuerySet.dwNameSpace = NS_BTH;
	wsaQuerySet.dwNumberOfCsAddrs = 1;
	wsaQuerySet.lpcsaBuffer = lpCSAddrInfo;

	if (SOCKET_ERROR == WSASetService(&wsaQuerySet, RNRSERVICE_REGISTER, 0))
	{
#ifdef PRINT_DEBUG
		printf("=CRITICAL= | WSASetService call failed. GetLastError = [%d]\n", GetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | WSASetService call failed. GetLastError = " + GetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}

	if (SOCKET_ERROR == listen(LocalSocket, SOMAXCONN))
	{

#ifdef PRINT_DEBUG
		printf("=CRITICAL= | listen() call failed w/socket = [0x%X]. WSAGetLastError=[%d]\n", LocalSocket, WSAGetLastError());
#endif
		((BluetoohServer*)(P))->Logevent.WriteLog("=CRITICAL= | listen() call failed w/socket = [0x%X]. WSAGetLastError= " + WSAGetLastError());
		ulRetCode = SER_ERROR;
		return ulRetCode;
	}
#ifdef PRINT_DEBUG
	printf("等待连接...\n");
#endif
	size = sizeof(struct sockaddr);
	while (1)
	{
		ClientSocket = accept(LocalSocket, (struct sockaddr *)&clntAddr, NULL);
		if (INVALID_SOCKET == ClientSocket)
		{
#ifdef PRINT_DEBUG
			printf("获取的SOCKET无效\n");
#endif
			((BluetoohServer*)(P))->Logevent.WriteLog("获取的SOCKET无效");
			break;
		}
		else if (INVALID_SOCKET == ClientSocket)
		{
#ifdef PRINT_DEBUG
			printf("不能获得用户连接的SOCK");
#endif
			((BluetoohServer*)(P))->Logevent.WriteLog("不能获得用户连接的SOCK");
			break;
		}
		else
		{
#ifdef PRINT_DEBUG
			printf("连接成功\n");
#endif
			struct ClientParam *Con = new ClientParam;
			Con->ClientSocket = ClientSocket;
			Con->clntAddr = tcpaddr;//有瑕疵
			((BluetoohServer*)(P))->Socketthread.StartServerSock(Con);//建立每个Client的子线程
		}
	}
	if (INVALID_SOCKET != LocalSocket)
		closesocket(LocalSocket);
	return 9999;//乱写的数字

}

BluetoohServer::BluetoohServer()
{
}


BluetoohServer::~BluetoohServer()
{
}

int BluetoohServer::start(int sw)
{
	int       ulRetCode = 0;
	WSADATA		WSAData = { 0 };

	ulRetCode = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if(ulRetCode != 0)
	{
#ifdef PRINT_DEBUG
		printf("-FATAL- | Unable to initialize Winsock version 2.2\n");
#endif // PRINT_DEBUG

		Logevent.WriteLog("-FATAL- | Unable to initialize Winsock version 2.2\n");

	}
//	ulRetCode = RunLANServerMode();
	DWORD dwThreadID[4] = { 0 };
	HANDLE handleServerThread;
	HANDLE handleClientThread_Order;
	HANDLE handleClientThread_Data;
	//RunLANServerMode()作为类成员函数则不需要WaitForSingleObject也会阻塞
	handleClientThread_Order = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunLANClientOrderMode, (LPVOID)this, 0, &dwThreadID[0]);
	handleClientThread_Data = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunLANClientDataMode(this), (LPVOID)this, 0, &dwThreadID[1]);
	if (sw == LAN)
		handleServerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunLANServerMode(this), (LPVOID)this, 0, &dwThreadID[2]);
	else if (sw == BTH)
		handleServerThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)RunBTHServerMode(this), (LPVOID)this, 0, &dwThreadID[3]);
	else
//	WaitForSingleObject(handleMainThread,INFINITE);
	//	CloseHandle(handleMainThread);
		return ulRetCode;

}

int BluetoohServer::stop()
{
	return 0;
}


