#include "stdafx.h"
#include "DataExPortable.h"


DataExPortable::DataExPortable()
{
}


DataExPortable::~DataExPortable()
{
}

void DataExPortable::SetZeroQuery(DataQuery_0 &DQ, MsgHeader mh, int requirement, wchar_t name[20], int time[6], int num, float *coordinate, int maxlistnum, int id, int request_id)
{
	DQ.header_0 = mh;
	DQ.id = id;
	DQ.request_id = request_id;
	DQ.requirement = requirement;
	wcscpy(DQ.satellite, name);
	DQ.time_begin[0] = time[0];
	DQ.time_begin[1] = time[1];
	DQ.time_begin_years = time[2];
	DQ.time_end[0] = time[3];
	DQ.time_end[1] = time[4];
	DQ.time_end_years = time[5];
	DQ.coor = new Coor(num);
	DQ.coor->xy = new float[num];
	for (int i = 0; i < num * 2; i++)
	{
		DQ.coor->xy[i] = coordinate[i];
	}
	/*for (int i = 0; i < num * 2; i++)
	{
		if (i % 2 == 0)
			DQ.coor->x[i / 2] = coordinate[i];
		else
			DQ.coor->y[( i - 1 ) / 2] = coordinate[i + 1];
	}*/
	DQ.list_max = maxlistnum;
	DQ.end = END;
}

void DataExPortable::SetDataRecvOrder(DataRecv &DaR, MsgHeader mh, int port, int request_id, int requirement, wchar_t name[20], int time[6], int num, float *coordinate)
{
	DaR.header_0 = mh;
	DaR.port = port;
	DaR.request_id = request_id;
	DaR.requirement = requirement;
	wcscpy(DaR.satellite, name);
	DaR.time_begin[0] = time[0];
	DaR.time_begin[1] = time[1];
	DaR.time_begin_years = time[2];
	DaR.time_end[0] = time[3];
	DaR.time_end[1] = time[4];
	DaR.time_end_years = time[5];
	DaR.coor = new Coor(num);
	DaR.coor->xy = new float[num];
	for (int i = 0; i < num * 2; i++)
	{
		DaR.coor->xy[i] = coordinate[i];
	}

	DaR.end = END;
}

bool DataExPortable::SendMsgHed(MsgHeader mh)
{
	bool result = true;
	int sen = 0;

	sen = Trans.Send(mh.begin);
	if (sen < 0)
		return false;
	sen = Trans.Send(mh.length);
	if (sen < 0)
		return false;

	sen = Trans.Send(mh.id);
	if (sen < 0)
		return false;

	return result;
}
bool DataExPortable::SendZeroQuery(MsgHeader mh, DataQuery_0 DQ)
{
	bool result = true;
	int sen = 0;
	int send_length = 0;

	int length = mh.length;

	sen = Trans.Send(DQ.id);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DQ.request_id);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DQ.requirement);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send((char*)DQ.satellite,40);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send((char*)DQ.time_begin,8);
	if (sen <= 0)
		return false;
	send_length += sen;
	
	sen = Trans.Send(DQ.time_begin_years);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send((char*)DQ.time_end, 8);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DQ.time_end_years);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DQ.coor->num);
	if (sen <= 0)
		return false;
	send_length += sen;

	for (int i = 0; i < DQ.coor->num * 2; i++)
	{
		sen = Trans.Send(DQ.coor->xy[i]);
		if (sen <= 0)
			return false;
		send_length += sen;
	}

	sen = Trans.Send(DQ.list_max);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send((char*)DQ.placeholder, sizeof(DQ.placeholder));
	if (sen <= 0)
		return false;
	send_length += sen;


	sen = Trans.Send(END);
	if (sen <= 0)
		return false;
	send_length += sen;

	if ((send_length + 16) != mh.length)	//此次发送的+消息头+消息尾
		return false;

	return true;
}
bool DataExPortable::RecvZeroResponse(MsgHeader mh, DataQuery_0_Response &DR)
{
	bool result = true;
	int length = mh.length;
	int recv_length;
	int rec = 0;

	rec = Trans.Recv(DR.id);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(DR.request_id);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv((char*)DR.placeholder, sizeof(DR.placeholder));
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(DR.list_number);
	if (rec <= 0)
		return false;
	recv_length += rec;

	DR.qdata = new QData[DR.list_number];
	int temp = RecvListData(DR.list_number, DR);
	if (temp > 0)
		recv_length += temp;
	else
		return false;

	rec = Trans.Recv(DR.end);
	if (rec <= 0 || DR.end != END)
		return false;
	recv_length += rec;

	return result;
}

bool DataExPortable::RecvMsgHed(MsgHeader &mh)
{
	bool result = true;
	int rec = 0;
	int temp = -1;

	rec = Trans.Recv(temp);
	if (temp != mh.begin)
		return false;
	rec = Trans.Recv(temp);
	if (rec > 0)
		mh.length = temp;
	else
		return false;
	rec = Trans.Recv(temp);
	if (temp != mh.id)
		return false;
	return result;
}

bool DataExPortable::RecvErrorOrder(MsgHeader mh, InvalidInstruction &InvldIns)
{
	int rec = 0;

	InvldIns.header_0 = mh;

	rec = Trans.Recv(InvldIns.message);
	if (rec <= 0)
		return false;
	rec = Trans.Recv(InvldIns.end);
	if (rec <= 0)
		return false;
	return true;
}

int DataExPortable::RecvListData(int listnum, DataQuery_0_Response &DR)
{
	int rec = 0;
	int recv_length = 0;
	for (int i = 0; i < listnum; i++) //接收所有的列表中的数据
	{
		rec = Trans.Recv((char*)DR.qdata[i].name, 40);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv((char*)DR.qdata[i].time_begin,8);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv(DR.qdata[i].time_begin_year);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv((char*)DR.qdata[i].time_end, 8);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv(DR.qdata[i].time_end_year);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv(DR.qdata[i].coor->num);
		if (rec <= 0)
			return -1;
		recv_length += rec;
		DR.qdata[i].coor = new Coor(DR.qdata[i].coor->num);
		for (int j = 0; j < DR.qdata[i].coor->num * 2; j++) //每个数据的坐标范围
		{
			int temp = 0;
			rec = Trans.Recv(temp);
			if (rec <= 0)
				return -1;
			recv_length += rec;
			DR.qdata[i].coor->xy[j] = (float)temp / 100;
		}

		int temp2 = 0;
		rec = Trans.Recv(temp2);
		if (rec <= 0)
			return -1;
		recv_length += rec;
		DR.qdata[i].data_size = (float)temp2 / 100;

		rec = Trans.Recv(DR.qdata[i].tm_number);
		if (rec <= 0)
			return -1;
		recv_length += rec;
		
		rec = Trans.Recv(DR.qdata[i].tm_size);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv(DR.qdata[i].frame_length);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv(DR.qdata[i].frame_header_length);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv(DR.qdata[i].task_number);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv(DR.qdata[i].record);
		if (rec <= 0)
			return -1;
		recv_length += rec;

		rec = Trans.Recv((char*)DR.qdata[i].placeholder, sizeof(DR.qdata[i].placeholder));
		if (rec <= 0)
			return -1;
		recv_length += rec;
	}
}

//数据通道
bool DataExPortable::SendDataRecvOrder(MsgHeader mh, DataRecv DaR)
{
	bool result = true;
	int sen = 0;
	int send_length = 0;

	int length = mh.length;

	sen = Trans.Send(DaR.port);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DaR.request_id);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DaR.requirement);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send((char*)DaR.satellite, sizeof(DaR.satellite));
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send((char*)DaR.time_begin, sizeof(DaR.time_begin));
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DaR.time_begin_years);
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send((char*)DaR.time_end, sizeof(DaR.time_end));
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(DaR.time_end_years);
	if (sen <= 0)
		return false;
	send_length += sen;

	//float *coor;
	//coor = new float[DaR.coor->num];
	//for (int i = 0; i < DaR.coor->num * 2; i++)
	//{
	//	coor[i] = DaR.coor->xy[i];
	//}

	sen = Trans.Send(DaR.coor->num);
	if (sen <= 0)
		return false;
	send_length += sen;

	for (int i = 0; i < DaR.coor->num * 2; i++)
	{
		sen = Trans.Send(DaR.coor->xy[i]);
		if (sen <= 0)
			return false;
		send_length += sen;
	}

	sen = Trans.Send((char*)DaR.placeholder, sizeof(DaR.placeholder));
	if (sen <= 0)
		return false;
	send_length += sen;

	sen = Trans.Send(END);
	if (sen <= 0)
		return false;
	send_length += sen;

	if ((send_length + 16) != mh.length)	//此次发送的+消息头+消息尾
		return false;

	return true;
}

bool DataExPortable::RecvFailOrder(MsgHeader mh, DataRecvFaile_Response &DataFailResp)
{
	int rec = 0;

	DataFailResp.header_0 = mh;

	rec = Trans.Recv(DataFailResp.id);
	if (rec <= 0)
		return false;

	rec = Trans.Recv(DataFailResp.error_code);
	if (rec <= 0)
		return false;

	rec = Trans.Recv(DataFailResp.end);
	if (rec <= 0)
		return false;

	return true;
}

bool DataExPortable::RecvRTResponse(MsgHeader mh, DataRecv_RT_Response &RTR)
{
	int rec = 0;
	int recv_length = 0;

	RTR.header_0 = mh;
	
	rec = Trans.Recv(RTR.id);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv((char*)RTR.satellite, sizeof(RTR.satellite));
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTR.state);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv((char*)RTR.placeholder, sizeof(RTR.placeholder));
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTR.end);
	if (rec <= 0 || RTR.end != END)
		return false;
	recv_length += rec;

	if ((recv_length + 16) != mh.length)
		return false;

	return true;
}

bool DataExPortable::RecvRTData(MsgHeader mh, DataRecv_RT_Data &RTD)
{
	int tm_length;
	int rec = 0;
	int recv_length;

	RTD.header_0 = mh;

	rec = Trans.Recv(RTD.id);
	if (rec <= 0 )
		return false;
	recv_length += rec;

	rec = Trans.Recv((char*)RTD.satellite,sizeof(RTD.satellite));
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTD.frame_header_length);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTD.frame_lenght);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTD.timeblock_length);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_stateblock_length);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_length);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_number);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_header_length);
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv((char*)RTD.placeholder,sizeof(RTD.placeholder));
	if (rec <= 0)
		return false;
	recv_length += rec;

	bool datatype = 1;
	if (RTD.frame_header_length != 0)
		datatype = 0;
	recv_length += RecvTMData(RTD.tm_length,RTD,datatype);

	rec = Trans.Recv(RTD.end);
	if (rec <= 0)
		return false;
	recv_length += rec;

	if ((recv_length + 16) != mh.length)
		return false;
}

int DataExPortable::RecvTMData(int datasize, DataRecv_RT_Data &RTD, bool datatype)
{
	//不清楚TM块数据格式，未完成
	int recv_length = 0;
	int rec = 0;
	RTD.TMB = new TMBlock[RTD.tm_number];
	for (int i = 0; i < RTD.tm_number; i++)
	{
		int temp[1024] = {0};
		int temp_recv = 0;
		int recsize = 1010;
		bool isfin = false;
		if (datatype == 0)	//同步数据
		{
			rec = Trans.Recv(RTD.TMB[i].frame_header);
			if (rec <= 0)
				break;
			recv_length += rec;

			RTD.TMB[i].data = new int[RTD.tm_length];
			while (1)
			{
				if (isfin)
					break;
				if (datasize < recsize)
				{
					recsize = datasize % recsize;
					isfin = true;
				}
				rec = Trans.Recv((char*)temp, recsize);
				if (rec <= 0)
					break;
				recv_length += rec;
				temp_recv += rec;
				datasize -= rec;
				RTD.TMB[i].data += rec;
			}
		}
	}

	return 0;
}

//数据通道

#include <iostream>
int DataExPortable::OrderEx(bool isfir)
{
	int sen = 0;
	int rec = 0;
	if (isfir)
	{
		/*
		do some identify
		*/
	}
	//首先接收消息头
	int num = 1000;
	char *a;
	a = new char[26];
	char *b;
	b = new char[num];
	memset(a,0,sizeof(a));
	memset(b, 0, sizeof(b));
	int size = num;
	int recc = 26;
	bool isfin = false;
	int senlength = 0;
	while (1)//剩余部分大于0
	{
		if (isfin)
			break;
		if (size < 26)
		{
			recc = size % 26;
			isfin = true;
		}
		int RecvSize = Trans.Recv((char*)a,recc);
		if (SOCKET_ERROR == RecvSize)
			break;
		for (int i = 0; i < recc; i++)
			b[senlength + i] = a[i];
		senlength += RecvSize;
		size = size - RecvSize;
		a += RecvSize;
	}
	for (int i = 0; i < num; i++)
	{
		cout << b[i] <<" ";
	}
	return rec;
}