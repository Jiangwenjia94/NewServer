#include <winsock2.h>
#include <ws2bth.h>
#include <vector>


struct ORDER
{
	int order_num;//唯一指令编号
	char *order_info;//指令内容
	char *order_sta;//当前指令的状态信息
	int is_send = 0;//0 未发送 1 已发送 2正在发送 -1不存在
};
struct USER
{
	char user_num[65];//唯一用户编号
	std::vector<ORDER> user_order;//使用队列存储每个用户的指令
	bool isfirt = true;
	int index = -1;//在client中的下标
};

struct ClientParam
{
	sockaddr clntAddr;
	SOCKET	 ClientSocket;
	LPVOID	 pThis;
};