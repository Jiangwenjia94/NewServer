#pragma once

#include "SocketTrans.h"
#include "TerminalInterface.h"
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
//负责与终端通讯
class DataExPortable
{
public:
	DataExPortable();
	~DataExPortable();

	void SetZeroQuery(DataQuery_0 &DQ, MsgHeader mh, int requirement, wchar_t name[20], int time[6], int num, float *coordinate, int maxlistnum, int id = MOD_ID, int request_id = REQUEST_ID);
	void SetDataRecvOrder(DataRecv &DaR, MsgHeader mh, int port, int request_id, int requirement, wchar_t name[20], int time[6], int num, float *coordinate);
	int OrderRecv(bool isfir);
	int OrderSend(bool isfir);
	//指令通道  使用3001端口
	bool SendMsgHed(MsgHeader mh);
	bool RecvMsgHed(MsgHeader &mh);
	bool SendZeroQuery(DataQuery_0 DQ);//
	int	 RecvListData(int listnum, DataQuery_0_Response &DR);
	bool RecvErrorOrder(MsgHeader mh, InvalidInstruction &InvldIns);
	bool RecvZeroResponse(MsgHeader mh, DataQuery_0_Response &DR);
	//指令通道

	//数据通道  使用3075端口
	int DataRec(bool isfir);
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

