#include <winsock2.h>
#include <ws2bth.h>
#include <vector>


struct ORDER
{
	int order_num;//Ψһָ����
	char *order_info;//ָ������
	char *order_sta;//��ǰָ���״̬��Ϣ
	int is_send = 0;//0 δ���� 1 �ѷ��� 2���ڷ��� -1������
};
struct USER
{
	char user_num[65];//Ψһ�û����
	std::vector<ORDER> user_order;//ʹ�ö��д洢ÿ���û���ָ��
	bool isfirt = true;
	int index = -1;//��client�е��±�
};

struct ClientParam
{
	sockaddr clntAddr;
	SOCKET	 ClientSocket;
	LPVOID	 pThis;
};