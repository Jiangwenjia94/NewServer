#pragma once

#include "SocketTrans.h"
#include "LuojiaOrbit.h"
#include <vector>
#include <fstream>
#include "Database.h"
#define MAX_SIZE_LENGTH					  1000
#define CXN_SUCCESS                       0
#define CXN_ERROR                         1

#define SEN_ERROR						  -9998
#define REC_ERROR_OFFLINE				  -9997	//主动关闭 -1 被动掉线  heart
#define REC_ERROR_CLOSE				      -9996 //使用CloseSocket关闭连接 0
//接收
#define C_LINK							  12345
#define C_HEART_LIVE_NOTICE				  9999
#define C_HEART_IMAGE_ASK				  8888
#define C_HEART_IMAGE_READLY			  7777
#define C_HEART_IMAGE_REFUSE			  -1
#define C_HEART_IMAGE_SUCCESS			  6666
#define C_HEART_IMAGE_FAIL				  -2
#define C_HEART_NOTIC_READLY			  5555
#define C_HEART_NOTIC_REFUSE			  -5
#define C_HEART_NOTIC_SUCCESS			  4444
#define C_HEART_NOTIC_FAIL				  -6
#define C_HEART_NOTIC_ASK				  3333
#define C_HEART_ORBIT_ASK				  2222
#define C_HEART_ORBIT_SUCCESS			  1111
#define C_HEART_ORBIT_FAIL				  -7
#define C_HEART_ORBIT_READLY			  8
#define C_HEART_ORBIT_REFUSE			  -8
//发送
#define S_LINK							  12345
#define S_HEART_LIVE_FEED				  0
#define S_HEART_ASK_ACCEPT				  1
#define S_HEART_ASK_REFUSE				  -1
#define S_HEART_ASK_SUCCESS				  2
#define S_HEART_ASK_FAIL				  0
#define S_HEART_IMAGE_READLY			  3
#define S_HEART_NOTIC_READLY			  4
#define S_HEART_ORDER_RECEIVE			  5
#define S_HEART_ORDER_REFUSE			  -3
#define S_HEART_ERROR					  -9999



class DataJudgment
{
public:
	DataJudgment(void);
	~DataJudgment(void);

	int		recvHeart(bool fir);
	
	int		getOrderNum();//获取当前待发送的order num
	CString getOrderInfo(int ordernum);//获取当前待发送的order info
	bool	getOrderInfo(int ordernum,char *(&buffer));//获取当前待发送的order info
	void	setOrdersta(int ordernum,int sta);

	bool	isSame(char *strA,char *strB);
	bool	Push(USER &user, vector<USER>&client);
	int		staque();
	char	*infoStateInquiry(int num);

	vector<USER> *m_client;
	SocketTrans Trans;//负责发送和接收
	ClientParam param;
	LuojiaOrbit LJOrbit;	//轨道数据查询
//	ThreadParam param;
	USER *user;
	bool bPictureReady;
	bool bNoticeReady;
	bool bPictureRecv;
	bool bNoticeRecv;
	bool is_identify;

private:
	Database db;
	static UINT sendCurrentPic(ClientParam *param, CString Path); //unuse
	
	void  reset();
	int	  recvCurHrt(ClientParam *param,bool fir);

//	ofstream out;
};

