#pragma once

#include "SocketTrans.h"
#include "TerminalInterface.h"
#include "shlwapi.h"
#include "Database.h"
#pragma comment(lib,"shlwapi.lib")
//�������ն�ͨѶ
class DataExPortable
{
public:
	DataExPortable();
	~DataExPortable();

	bool ConnectDB();

	void SetZeroQuery(DataQuery_0 &DQ, MsgHeader mh, int requirement, wchar_t name[20], int time[6], int num, float *coordinate, int maxlistnum, int id = MOD_ID, int request_id = REQUEST_ID);
	void SetDataRecvOrder(DataRecv &DaR, MsgHeader mh, int port, int request_id, int requirement, wchar_t name[20], int time[6], int num, float *coordinate);
	int OrderRecv(bool isfir);
	int OrderSend(bool isfir);
	//ָ��ͨ��  ʹ��3001�˿�
	int SendMsgHed(MsgHeader mh);
	int RecvMsgHed(MsgHeader &mh);
	bool SendZeroQuery(DataQuery_0 DQ);//
	int	 RecvListData(int listnum, DataQuery_0_Response &DR);
	bool RecvErrorOrder(MsgHeader mh, InvalidInstruction &InvldIns);
	bool RecvZeroResponse(MsgHeader mh, DataQuery_0_Response &DR);
	//ָ��ͨ��

	//����ͨ��  ʹ��3075�˿�
	int DataRec(bool isfir);
	int SendDataRecvOrder(MsgHeader mh, DataRecv DaR);
	bool RecvFailOrder(MsgHeader mh, DataRecvFaile_Response &DataFailResp);
	int RecvRTResponse(MsgHeader mh, DataRecv_RT_Response &RTR);	//��Ӧ1235�ṹ��ͬ�������ĴΣ���ͬRT��
	int RecvRTData(MsgHeader mh, DataRecv_RT_Data &RTD,char *prepath);
	int	 RecvTMData(int datasize, DataRecv_RT_Data &RTD, bool datatype, char *prepath);
	bool RecvPFData();
	//����ͨ��

	int DataEx();

	SocketTrans Trans;//�����ͺͽ���
	ClientParam param;
	Database	DB;
};

