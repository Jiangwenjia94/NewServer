#pragma once

#include "SocketTrans.h"
#include "TerminalInterface.h"
//�������ն�ͨѶ
class DataExPortable
{
public:
	DataExPortable();
	~DataExPortable();

	void SetZeroQuery(DataQuery_0 &DQ, MsgHeader mh, int requirement, wchar_t name[20], int time[6], int num, float *coordinate, int maxlistnum, int id = MOD_ID, int request_id = REQUEST_ID);
	void SetDataRecvOrder(DataRecv &DaR, MsgHeader mh, int port, int request_id, int requirement, wchar_t name[20], int time[6], int num, float *coordinate);
	int OrderEx(bool isfir);
	//ָ��ͨ��  ʹ��3001�˿�
	bool SendMsgHed(MsgHeader mh);
	bool RecvMsgHed(MsgHeader &mh);
	bool SendZeroQuery(MsgHeader mh, DataQuery_0 DQ);//��Ϣͷ�����������������յ�������ѯʱ��
	int	 RecvListData(int listnum, DataQuery_0_Response &DR);
	bool RecvErrorOrder(MsgHeader mh, InvalidInstruction &InvldIns);
	bool RecvZeroResponse(MsgHeader mh, DataQuery_0_Response &DR);
	//ָ��ͨ��

	//����ͨ��  ʹ��3075�˿�
	bool SendDataRecvOrder(MsgHeader mh, DataRecv DaR);
	bool RecvFailOrder(MsgHeader mh, DataRecvFaile_Response &DataFailResp);
	bool RecvRTResponse(MsgHeader mh, DataRecv_RT_Response &RTR);	//��Ӧ1235�ṹ��ͬ�������ĴΣ���ͬRT��
	bool RecvRTData(MsgHeader mh, DataRecv_RT_Data &RTD);
	int	 RecvTMData(int datasize, DataRecv_RT_Data &RTD, bool datatype);
	bool RecvPFData();
	//����ͨ��

	int DataEx();

	SocketTrans Trans;//�����ͺͽ���
	ClientParam param;
};

