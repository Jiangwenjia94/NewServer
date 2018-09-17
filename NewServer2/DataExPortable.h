#pragma once

#include "SocketTrans.h"
#include "TerminalInterface.h"
//负责与终端通讯
class DataExPortable
{
public:
	DataExPortable();
	~DataExPortable();

	void SetZeroQuery(DataQuery_0 &DQ, MsgHeader mh, int requirement, wchar_t name[20], int time[6], int num, float *coordinate, int maxlistnum, int id = MOD_ID, int request_id = REQUEST_ID);
	void SetDataRecvOrder(DataRecv &DaR, MsgHeader mh, int port, int request_id, int requirement, wchar_t name[20], int time[6], int num, float *coordinate);
	int OrderEx(bool isfir);
	//指令通道  使用3001端口
	bool SendMsgHed(MsgHeader mh);
	bool RecvMsgHed(MsgHeader &mh);
	bool SendZeroQuery(MsgHeader mh, DataQuery_0 DQ);//消息头，条件，卫星名，拐点数，查询时间
	int	 RecvListData(int listnum, DataQuery_0_Response &DR);
	bool RecvErrorOrder(MsgHeader mh, InvalidInstruction &InvldIns);
	bool RecvZeroResponse(MsgHeader mh, DataQuery_0_Response &DR);
	//指令通道

	//数据通道  使用3075端口
	bool SendDataRecvOrder(MsgHeader mh, DataRecv DaR);
	bool RecvFailOrder(MsgHeader mh, DataRecvFaile_Response &DataFailResp);
	bool RecvRTResponse(MsgHeader mh, DataRecv_RT_Response &RTR);	//响应1235结构相同，调用四次（不同RT）
	bool RecvRTData(MsgHeader mh, DataRecv_RT_Data &RTD);
	int	 RecvTMData(int datasize, DataRecv_RT_Data &RTD, bool datatype);
	bool RecvPFData();
	//数据通道

	int DataEx();

	SocketTrans Trans;//负责发送和接收
	ClientParam param;
};

