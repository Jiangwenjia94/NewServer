#pragma once
#include <bitset>   

#define DATA_ERROR -1
#define Invalid_Ins_Length 20
#define MOD_ID 0
#define REQUEST_ID 1
#define END -1234567890

struct MsgHeader				//信息头  12byte
{
	int begin = 1234567890;		//消息开始
	int length;					//消息长度（包含信息头和信息尾）
	int id = 0;						//流标识
};


struct InvalidInstruction		//无效指令响应	20byte
{
	MsgHeader header_0;			//消息头
	int message = -1;			//无效指令信息

	int end = -1234567890;		//消息尾
};

struct Coor
{
	int num;//拐点数
	float *xy;	//xy坐标

	Coor(int n)
	{
		num = n;
		xy = new float[2 * n];
	}
};

struct QData			//一个数据的信息 用于接收
{
	wchar_t name[20];
	int time_begin[2];
	int time_begin_year;
	int time_end[2];
	int time_end_year;
	Coor *coor;
	float data_size;			//一个数据的数据量
	int tm_number;				//一个数据中的TM块数量
	int tm_size;				//一个数据中的TM块大小
	int frame_length;			//一个数据中的帧长
	int frame_header_length;	//一个数据中的帧头长
	int task_number;			//一个数据的任务号
	int record;					//一个数据的记录段号
	int placeholder[7];	//保留值

	QData()
	{
		memset(placeholder, 0, sizeof(placeholder));
	}

};
//控制指令
struct DataQuery_0				//3.5.1 本地零级数据列表查询指令
{
	MsgHeader header_0;			//消息头
	int id;						//模块ID
	int request_id;				//请求码
	int requirement;			//查询条件（通过设置bit位得到int）
	wchar_t satellite[20];		//卫星名
	int time_begin[2];			//接收的数据的开始时间
	int time_begin_years;		//接收的数据的开始年份
	int time_end[2];			//接收的数据的结束时间
	int time_end_years;			//接收的数据的结束年份
	Coor *coor;
	int list_max;				//查询的最大列表数量(1-100)
	int placeholder[6];	//保留值
	int end = -1234567890;		//消息尾

	DataQuery_0()
	{
		memset(placeholder, 0, sizeof(placeholder));
	}
};

struct DataQuery_0_Response		//3.5.2 本地零级数据列表查询响应
{
	MsgHeader header_0;			//消息头
	int id;						//模块ID
	int request_id;				//请求码
	int placeholder[3];			//保留值
	int list_number;			//查询到列表数量
	QData *qdata;				//保存所有数据的数组
	int end = -1234567890;		//消息尾
};
//控制指令

struct DataRecv					//3.6.1 数据接收指令 length = 256
{
	MsgHeader header_0;			//消息头
	int port;					//通道号
	int request_id;				//请求码 4：实时数据接收 3：事后数据接收
	int requirement;			//查询条件（通过设置bit位得到int）
	wchar_t satellite[20];		//卫星名
	int time_begin[2];			//接收的数据的开始时间
	int time_begin_years;		//接收的数据的开始年份
	int time_end[2];			//接收的数据的结束时间
	int time_end_years;			//接收的数据的结束年份
	Coor *coor;
	int placeholder[39];			//保留值
	int end = -1234567890;		//消息尾
	char tableid[30];
	DataRecv()
	{
		memset(placeholder, 0, sizeof(placeholder));
		memset(tableid, 0, sizeof(tableid));
	}
};

struct DataRecvFaile_Response	//3.6.2 接收失败响应 length = 24
{
	MsgHeader header_0;			//消息头,长度等于24
	int id = 0xFFFFFFFF;		//标识号 -1
	int error_code;				//错误码
	int end = -1234567890;		//消息尾
};

struct DataRecv_RT_Response		//3.6.3 实时数据接收响应-1-2-3-5 事后数据响应2 length = 80;
{
	MsgHeader header_0;			//消息头,长度等于24
	int id = 0xEEEEEEE1;		//标识号
	wchar_t satellite[20];		//卫星名
	int state;				//卫星状态
	int placeholder[4];			//保留值
	int end;			//消息尾
	char tableid[30];
	DataRecv_RT_Response()
	{
		memset(placeholder, 0, sizeof(placeholder));
		memset(tableid, 0, sizeof(tableid));
	}
};


struct TMBlock					//默认为原始数据
{
	char *frame_data;			//同步数据才会使用到
	char *data;
	int	lastdata;				//最后<=8字节数据+可能存在的零填充数据
	int tm_time[2];				//TM块时间
	int tm_state;				//TM块状态
	int placeholder;				//保留值
};


struct DataRecv_RT_Data		//3.6.6 实时数据接收响应-4  3.6.8事后数据响应1
{
	MsgHeader header_0;			//消息头
	int id = 0xEEEEEEE4;		//标识号
	wchar_t satellite[20];		//卫星名
	int frame_header_length;	//帧头长
	int frame_lenght;			//帧长
	int timeblock_length;		//时间区长度
	int tm_stateblock_length;	//TM块状态区长度
	int tm_length;				//TM块长度
	int tm_number;				//TM块数
	int tm_header_length = 0;	//TM文件头长度(不知道为什么定义为0)
	int placeholder[43];
	TMBlock *TMB;
	int end;
	char tableid[30];
	char path[30];				//TBVBlock的保存路径
	DataRecv_RT_Data()
	{
		memset(placeholder, 0, sizeof(placeholder));
		memset(tableid, 0, sizeof(tableid));
		memset(path, 0, sizeof(path));
	}
};


//int BitSet(int a, int b, int c)//分别是0，8，16bit位
//{
//	std::bitset<32> temp;
//	if (a == 1)
//		temp.set(0);
//	if (b == 1)
//		temp.set(8);
//	if (c == 1)
//		temp.set(16);
////	std::cout << temp.to_string() << std::endl;
//	int result = temp.to_ulong();
//
//	return result;
//}
