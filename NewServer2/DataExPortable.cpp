#include "stdafx.h"
#include "DataExPortable.h"


void PrintDQ(DataQuery_0 DQ)
{
	std::cout << DQ.header_0.begin << std::endl;
	std::cout << DQ.header_0.length << std::endl;
	std::cout << DQ.header_0.id << std::endl;
	std::cout << DQ.id << std::endl;
	std::cout << DQ.request_id << std::endl;
	std::cout << DQ.requirement << std::endl;
	std::wcout << DQ.satellite << std::endl;
	std::cout << DQ.time_begin[0] << " " << DQ.time_begin[1] << std::endl;
	std::cout << DQ.time_begin_years << std::endl;
	std::cout << DQ.time_end[0] << " " << DQ.time_end[1] << std::endl;
	std::cout << DQ.time_end_years << std::endl;

	std::cout << "坐标信息开始：" << std::endl;
	std::cout << DQ.coor->num << std::endl;
	for (int i = 0; i < DQ.coor->num * 2; i++)
	{
		std::cout << DQ.coor->xy[i] << std::endl;
	}
	std::cout << "坐标信息结束：" << std::endl;

	std::cout << DQ.list_max << std::endl;
	std::cout << DQ.end << std::endl;
}
void SetTime(char *ti, int firnum)			//时间，首位
{
	/*******************************************************
	发送数据接收指令 首位 : 1
	事后数据1	 首位 : 2
	事后数据2	 首位 : 3

	实时数据1	 首位 : 4
	实时数据2	 首位 : 5
	实时数据3	 首位 : 6
	实时数据4	 首位 : 7
	实时数据5	 首位 : 8
	*******************************************************/
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	int i = sizeof(ti);
	sprintf_s(ti, 30, "%d%d%d%d%d%d%d", firnum, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,st.wMilliseconds);
}


DataExPortable::DataExPortable()
{
}


DataExPortable::~DataExPortable()
{
}

bool DataExPortable::ConnectDB()
{
	return DB.Connect();
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
//	DaR.coor->xy = new float[num];
	for (int i = 0; i < num * 2; i++)
	{
 		DaR.coor->xy[i] = coordinate[i];
	}

	DaR.end = END;
}

int DataExPortable::SendMsgHed(MsgHeader mh)
{
	bool result = true;
	int send_length = 0;
	int sen = 0;

	sen = Trans.Send(mh.begin);
	if (sen < 0)
		return false;
	send_length += sen;

	sen = Trans.Send(mh.length);
	if (sen < 0)
		return false;
	send_length += sen;

	sen = Trans.Send(mh.id);
	if (sen < 0)
		return false;
	send_length += sen;

	return send_length;

}
bool DataExPortable::SendZeroQuery(DataQuery_0 DQ)
{
	bool result = true;
	int sen = 0;
	int send_length = 0;

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
		sen = Trans.Send((char*)&DQ.coor->xy[i],4);
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

	if ((send_length + 12) != DQ.header_0.length)	//此次发送的+消息头+消息尾
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

	std::cout << recv_length+12 << std::endl;

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

	if (recv_length + 12 != length)
		return false;
	return result;
}

int DataExPortable::RecvMsgHed(MsgHeader &mh)
{
	int result = 0;
	int rec = 0;
	int temp = -1;

	rec = Trans.Recv(temp);
	if (rec > 0)
		mh.begin = temp;
	else
		return -1;
	result += rec;

	rec = Trans.Recv(temp);
	if (rec > 0)
		mh.length = temp;
	else
		return -1;
	result += rec;

	rec = Trans.Recv(temp);
	if (rec > 0)
		mh.id = temp;
	else
		return -1;
	result += rec;

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

		int temp = 0;
		rec = Trans.Recv(temp);
		if (rec <= 0)
			return -1;
		DR.qdata[i].coor = new Coor(temp);
		recv_length += rec;
	//	cout << i << " " << recv_length+45 << endl;
		for (int j = 0; j < DR.qdata[i].coor->num * 2; j++) //每个数据的坐标范围
		{
			char tcoor[5] = { 0 };
			rec = Trans.Recv(tcoor,4);
			if (rec <= 0)
				return -1;
			recv_length += rec;
			DR.qdata[i].coor->xy[j] = *(float*)tcoor;
		//	cout << DR.qdata[i].coor->xy[j] << endl;
		}

		char tsize[5] = { 0 };
		rec = Trans.Recv(tsize, 4);
		if (rec <= 0)
			return -1;
		recv_length += rec;
		DR.qdata[i].data_size = *(float*)tsize;

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
		cout <<i<<" "<< recv_length+36 << endl;
	}
	cout <<"allsize: "<< recv_length+36 << endl;

	return recv_length;
}

//数据通道
int DataExPortable::DataRec(bool isfir)
{
	int rec = 0;
	if (isfir)
	{
		/*
		do some identify
		*/
	}
	//首先接收消息头

	MsgHeader msg;
	MsgHeader rec_msg;
	DataRecvFaile_Response DRF;
	DataRecv DR;

	int ttime[6] = { 0 };
	ttime[0] = 1;
	ttime[1] = 0;
	ttime[2] = 2018;
	ttime[3] = 10;
	ttime[4] = 0;
	ttime[5] = 2018;
	int num = 5;
//	float coordinate[10] = { 0.000 };
	float *coordinate = new float[num * 2];
	coordinate[0] = 1.111;
	coordinate[1] = 1.222;
	coordinate[2] = 2.333;
	coordinate[3] = 2.444;
	coordinate[4] = 3.555;
	coordinate[5] = 3.666;
	coordinate[6] = 4.777;
	coordinate[7] = 4.888;
	coordinate[8] = 5.999;
	coordinate[9] = 5.001;
	msg.length = 248 + (2 * num + 1) * 4;
//	msg.length = 0;
	SetDataRecvOrder(DR, msg, 1, 3, 5, L"abcd", ttime, 5, coordinate);

	bool isfin = false;
	int senlength = 0;
	int headlength = 0;
	int recvlength = 0;
	char tableid[30];
	memset(tableid, 0, sizeof(tableid));
//	while (1)
//	{
		headlength = SendMsgHed(msg);
		senlength = SendDataRecvOrder(msg, DR);		//发送数据接收指令
		if (senlength + headlength == msg.length)
		{
			SetTime(DR.tableid, 1);
			if (!DB.InsertIntoClientRecvComd(DR))
				cout << "insert ClientRecvComd error!" << endl;
		}
		else
		{
			return 0;
		}
		//存放此次数据
		char cpath[30];
		memset(cpath,0,sizeof(cpath));
		SetTime(cpath,0);
		CString cspath;
		cspath.Format(_T("%s"), CStringW(cpath));
		if (!PathIsDirectory(cspath))
		{
			CreateDirectory(cspath, NULL);
			cout << "创建成功" << endl;
		}
		else
		{
			cout << "fail" << endl;
		}
		//
		if (1/*rec > 0 && rec_msg.length != 24*/)		//数据可以发送则开始接收
		{
			if (DR.request_id == 3)					//事后数据接收
			{
				int count = 0;
				int headrec = 0;
				MsgHeader AHead;
				DataRecv_RT_Data ADRTD[20];
				DataRecv_RT_Response ADRTR;
				while (1)
				{
					headrec = RecvMsgHed(AHead);	//直到接收到数据接收响应2的消息头
					if (AHead.length == 80)
						break;
					if (headrec <= 0)
						break;
			//		sprintf_s(ADRTD[count].path, sizeof(ADRTD[count].path),"%d",count);
					SetTime(ADRTD[count].tableid, 2);
					sprintf_s(ADRTD[count].path, 30, "%s%s%s", cpath, "/", ADRTD[count].tableid);
					int ADRTR_length1 = RecvRTData(AHead, ADRTD[count],cpath);
					if (ADRTR_length1 + headrec != AHead.length)
						break;
					if (!DB.InsertIntoClientPostData1(ADRTD[count]))
						cout << "insert ClientPostData1 error!" << endl;
					count++;  
				}
				int ADRTR_length2 = RecvRTResponse(AHead, ADRTR);
				if (ADRTR_length2 + headrec != AHead.length)	//接收事后数据接收响应-2
					return -1;
				SetTime(ADRTR.tableid, 3);
				if(!DB.InsertIntoClientPostData2(ADRTR))
					cout << "insert ClientPostData2 error!" << endl;
			}
			else if (DR.request_id == 4)			//实时数据接收
			{
				DataRecv_RT_Response DRTR[4];		//实时数据接收响应-1-2-3-5
				MsgHeader DRRTHead[3];				//实时数据接收响应-1-2-3的消息头
				DataRecv_RT_Data DRTD[20];			//暂定有20个接收响应4
				MsgHeader Data4Head;				//接收响应4或5的消息头
				int headrec = 0;
				for (int i = 0; i < 3; i++)
				{
					headrec = RecvMsgHed(DRRTHead[i]);
					if (headrec <= 0)
						break;
					int DRTR_length123 = RecvRTResponse(DRRTHead[i], DRTR[i]);
					if (DRTR_length123 + headrec != DRRTHead[i].length)
						break;
					SetTime(DRTR[i].tableid, i + 4);
					if (!DB.InsertIntoClientSynData1235(DRTR[i]))
						cout << "insert ClientSynData1235 error!" << endl;

				}									//实时数据接收响应-1-2-3接收完成
				int count = 0;
				while (1)		
				{
					headrec = RecvMsgHed(Data4Head);	//直到接收到实时数据接收响应5的消息头
					if (Data4Head.length == 80)			//=80则是响应5的消息头
						break;
					if (headrec <= 0)
						break;
					SetTime(DRTD[count].tableid, 7);
					sprintf_s(DRTD[count].path, 30, "%s%s%s", cpath, "/", DRTD[count].tableid);
					int DRTR_length4 = RecvRTData(Data4Head, DRTD[count], cpath);
					if (DRTR_length4 + headrec != Data4Head.length)
						break;
					if (!DB.InsertIntoClientSynData4(DRTD[count]))
						cout << "insert ClientSynData4 error!" << endl;
					count++;
				}
				int DRTR_length5 = RecvRTResponse(Data4Head, DRTR[3]);
				if (DRTR_length5 + headrec != Data4Head.length)	//接收实时数据接收响应-5
					return -1;
				SetTime(DRTR[3].tableid, 8);
				if (!DB.InsertIntoClientSynData1235(DRTR[3]))
					cout << "insert ClientSynData1235 error!" << endl;
			}
		}
		else										//接收失败响应
		{
			int frec = RecvFailOrder(rec_msg,DRF);	
			if (frec <= 0)
			{
				cout << "recv failed" << endl;
				return -1;
			}
		}
         
//	}

	return rec;
}

int DataExPortable::SendDataRecvOrder(MsgHeader mh, DataRecv DaR)
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

	sen = Trans.Send(DaR.coor->num);
	if (sen <= 0)
		return false;
	send_length += sen;

	for (int i = 0; i < DaR.coor->num * 2; i++)
	{
		sen = Trans.Send((char*)&DaR.coor->xy[i],4);
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

//	if ((send_length + 12) != mh.length)	//此次发送的+消息头+消息尾
//		return false;

	return send_length;
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

int DataExPortable::RecvRTResponse(MsgHeader mh, DataRecv_RT_Response &RTR)
{
	int rec = 0;
	int recv_length = 0;

	RTR.header_0 = mh;
	
	rec = Trans.Recv(RTR.id);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv((char*)RTR.satellite, sizeof(RTR.satellite));
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTR.state);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv((char*)RTR.placeholder, sizeof(RTR.placeholder));
	if (rec <= 0)
		return false;
	recv_length += rec;

	rec = Trans.Recv(RTR.end);
	if (rec <= 0 || RTR.end != END)
		return -1;
	recv_length += rec;

	if ((recv_length + 12) != mh.length)
		return -1;

	return recv_length;
}

int DataExPortable::RecvRTData(MsgHeader mh, DataRecv_RT_Data &RTD, char *prepath)
{
	int tm_length;
	int rec = 0;
	int recv_length = 0;

	RTD.header_0 = mh;

	rec = Trans.Recv(RTD.id);
	if (rec <= 0 )
		return -1;
	recv_length += rec;

	rec = Trans.Recv((char*)RTD.satellite,sizeof(RTD.satellite));
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTD.frame_header_length);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTD.frame_lenght);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTD.timeblock_length);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_stateblock_length);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_length);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_number);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv(RTD.tm_header_length);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	rec = Trans.Recv((char*)RTD.placeholder,sizeof(RTD.placeholder));
	if (rec <= 0)
		return -1;
	recv_length += rec;

	bool datatype = 1;
	if (RTD.frame_header_length != 0)
		datatype = 0;
	recv_length += RecvTMData(RTD.tm_length,RTD,datatype,prepath);

	rec = Trans.Recv(RTD.end);
	if (rec <= 0)
		return -1;
	recv_length += rec;

	if ((recv_length + 12) != mh.length)
		return -1;

	return recv_length;
}

int DataExPortable::RecvTMData(int datasize, DataRecv_RT_Data &RTD, bool datatype, char *prepath)
{
	int recv_length = 0;
	int rec = 0;
	int tmsize = 0;
	
	int tm_datasize = 0;

	if (datatype == 0)			//同步数据
	{
		tm_datasize = RTD.tm_length * 8;		//每个tm块中数据的大小
	}
	else if (datatype == 1)		//原始数据
	{
		tm_datasize = 1024;
	}

	RTD.TMB = new TMBlock[RTD.tm_number];
	char temp[1024] = { 0 };
	CString path;
	path.Format(_T("%s%s%s"), CStringW(prepath), "/", CStringW(RTD.tableid));
	if (!PathIsDirectory(path))
	{
		CreateDirectory(path, NULL);
		cout << "创建成功" << endl;
	}
	else
	{
		cout << "fail" << endl;
	}
	CString num;
	char buf1[1024], buf2[1024], buf3[1024];
	for (int i = 0; i < RTD.tm_number; i++)
	{
		memset(buf1,0,sizeof(buf1));
		memset(buf2, 0, sizeof(buf2));
		memset(buf3, 0, sizeof(buf2));
		//创建文件
		num.Format(_T("%d"), i);
		CString strFileName = path + L"//" + num;
		strFileName += ".txt";
		LPCTSTR path = strFileName;
		CFile file(path, CFile::modeCreate | CFile::modeWrite);
		//创建文件
		bool isfin = false;
		if (datatype == 0)	//同步数据
		{
			RTD.TMB[i].frame_data = new char[RTD.frame_header_length];
			rec = Trans.Recv((char*)RTD.TMB[i].frame_data, RTD.frame_header_length);
			if (rec <= 0)
				return -1;
			sprintf(buf1, "%s", RTD.TMB[i].frame_data);
			file.Write(buf1, sizeof(buf1));
			recv_length += rec;

			RTD.TMB[i].data = new char[tm_datasize];
			int temp_rec = 0;
			while (temp_rec < tm_datasize)
			{
				rec = Trans.Recv((char*)temp,1024);
				if (rec <= 0)
					return -1;
				memcpy(RTD.TMB[i].data + temp_rec,temp,strlen(temp));
				temp_rec += rec;
				file.Write(temp, rec);
			}
			recv_length += temp_rec;

			rec = Trans.Recv((char*)RTD.TMB[i].tm_time, 8);
			if (rec <= 0)
				return -1;
			sprintf(buf2, "%d%d", RTD.TMB[i].tm_time[0], RTD.TMB[i].tm_time[1]);
			file.Write(buf2, sizeof(buf2));
			recv_length += rec;

			rec = Trans.Recv(RTD.TMB[i].tm_state);
			if (rec <= 0)
				return -1;
			sprintf(buf3, "%d", RTD.TMB[i].tm_state);
			file.Write(buf3, sizeof(buf3));
			recv_length += rec;

			rec = Trans.Recv(RTD.TMB[i].placeholder);
			if (rec <= 0)
				return -1;
			recv_length += rec;
		}
		else				//原始数据
		{
			RTD.TMB[i].data = new char[tm_datasize];
			int temp_rec = 0;
			while (temp_rec < tm_datasize)
			{
				rec = Trans.Recv((char*)temp, 1024);
				if (rec <= 0)
					return -1;
				memcpy(RTD.TMB[i].data + temp_rec, temp, strlen(temp));
				file.Write(temp, rec);
				temp_rec += rec;
			}
			recv_length += temp_rec;

			rec = Trans.Recv((char*)RTD.TMB[i].tm_time, 8);
			if (rec <= 0)
				return -1;
			sprintf(buf2, "%d%d", RTD.TMB[i].tm_time[0], RTD.TMB[i].tm_time[1]);
			file.Write(buf2, sizeof(buf2));
			recv_length += rec;

			rec = Trans.Recv(RTD.TMB[i].tm_state);
			if (rec <= 0)
				return -1;
			sprintf(buf3, "%d", RTD.TMB[i].tm_state);
			file.Write(buf3, sizeof(buf3));
			recv_length += rec;

			rec = Trans.Recv(RTD.TMB[i].placeholder);
			if (rec <= 0)
				return -1;
			recv_length += rec;

	//		cout << "**recv_length:" << RTD.tableid<<" : "<<recv_length << endl;
		}
		
		file.Close();
	}
	cout << "path: " << path << endl;

	return recv_length                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       ;
}

//数据通道

#include <iostream>

int DataExPortable::OrderSend(bool isfir)\
{
	int sen = 0;
	if (isfir)
	{
		/*
		do some identify
		*/
	}
	//首先设置指令
	cout << "in" << endl;
	MsgHeader mh;
	DataQuery_0 DQ0;

	int requirement = 1;
	wchar_t name[20];
	wcscpy_s(name, L"abcde");
	int ttime[6] = {0};
	ttime[0] = 1;
	ttime[1] = 2;
	ttime[2] = 1990;
	ttime[3] = 98;
	ttime[4] = 99;
	ttime[5] = 2018;
	int num = 5;	//表示坐标范围的拐点数
	float coordinate[10] = { 0.000 };
	coordinate[0] = 1.111;
	coordinate[1] = 1.222;
	coordinate[2] = 2.333;
	coordinate[3] = 2.444;
	coordinate[4] = 3.555;
	coordinate[5] = 3.666;
	coordinate[6] = 4.777;
	coordinate[7] = 4.888;
	coordinate[8] = 5.999;
	coordinate[9] = 5.001;
	int maxlistnum = 99;
	int OrderSize = 124 + 8 * num;
	
	mh.length = OrderSize;
	
	SetZeroQuery(DQ0, mh, requirement,name,ttime,num,coordinate,maxlistnum);
	PrintDQ(DQ0);
	//设置完成
	if (!SendMsgHed(mh))
		return 0;
	if (!SendZeroQuery(DQ0))
		return 0;
	
}

int DataExPortable::OrderRecv(bool isfir)
{
	int rec = 0;
	if (isfir)
	{
		/*
		do some identify
		*/
	}
	//首先接收消息头
	int num = 1000;
	
	MsgHeader msg;
	DataQuery_0_Response DQR;

	bool isfin = false;
	int senlength = 0;
	while (1)//剩余部分大于0
	{
		if ((RecvMsgHed(msg)))
		{
			int OrderSize = msg.length;
			RecvZeroResponse(msg, DQR);
		}
	}

	return rec;
}