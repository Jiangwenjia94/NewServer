#pragma once
#include <bitset>   

#define DATA_ERROR -1
#define Invalid_Ins_Length 20
#define MOD_ID 0
#define REQUEST_ID 1
#define END -1234567890

struct MsgHeader				//��Ϣͷ  12byte
{
	int begin = 1234567890;		//��Ϣ��ʼ
	int length;					//��Ϣ���ȣ�������Ϣͷ����Ϣβ��
	int id = 0;						//����ʶ
};


struct InvalidInstruction		//��Чָ����Ӧ	20byte
{
	MsgHeader header_0;			//��Ϣͷ
	int message = -1;			//��Чָ����Ϣ

	int end = -1234567890;		//��Ϣβ
};

struct Coor
{
	int num;//�յ���
	float *xy;	//xy����

	Coor(int n)
	{
		num = n;
		xy = new float[2 * n];
	}
};

struct QData			//һ�����ݵ���Ϣ ���ڽ���
{
	wchar_t name[20];
	int time_begin[2];
	int time_begin_year;
	int time_end[2];
	int time_end_year;
	Coor *coor;
	float data_size;			//һ�����ݵ�������
	int tm_number;				//һ�������е�TM������
	int tm_size;				//һ�������е�TM���С
	int frame_length;			//һ�������е�֡��
	int frame_header_length;	//һ�������е�֡ͷ��
	int task_number;			//һ�����ݵ������
	int record;					//һ�����ݵļ�¼�κ�
	int placeholder[7];	//����ֵ

	QData()
	{
		memset(placeholder, 0, sizeof(placeholder));
	}

};
//����ָ��
struct DataQuery_0				//3.5.1 �����㼶�����б��ѯָ��
{
	MsgHeader header_0;			//��Ϣͷ
	int id;						//ģ��ID
	int request_id;				//������
	int requirement;			//��ѯ������ͨ������bitλ�õ�int��
	wchar_t satellite[20];		//������
	int time_begin[2];			//���յ����ݵĿ�ʼʱ��
	int time_begin_years;		//���յ����ݵĿ�ʼ���
	int time_end[2];			//���յ����ݵĽ���ʱ��
	int time_end_years;			//���յ����ݵĽ������
	Coor *coor;
	int list_max;				//��ѯ������б�����(1-100)
	int placeholder[6];	//����ֵ
	int end = -1234567890;		//��Ϣβ

	DataQuery_0()
	{
		memset(placeholder, 0, sizeof(placeholder));
	}
};

struct DataQuery_0_Response		//3.5.2 �����㼶�����б��ѯ��Ӧ
{
	MsgHeader header_0;			//��Ϣͷ
	int id;						//ģ��ID
	int request_id;				//������
	int placeholder[3];			//����ֵ
	int list_number;			//��ѯ���б�����
	QData *qdata;				//�����������ݵ�����
	int end = -1234567890;		//��Ϣβ
};
//����ָ��

struct DataRecv					//3.6.1 ���ݽ���ָ�� length = 256
{
	MsgHeader header_0;			//��Ϣͷ
	int port;					//ͨ����
	int request_id;				//������ 4��ʵʱ���ݽ��� 3���º����ݽ���
	int requirement;			//��ѯ������ͨ������bitλ�õ�int��
	wchar_t satellite[20];		//������
	int time_begin[2];			//���յ����ݵĿ�ʼʱ��
	int time_begin_years;		//���յ����ݵĿ�ʼ���
	int time_end[2];			//���յ����ݵĽ���ʱ��
	int time_end_years;			//���յ����ݵĽ������
	Coor *coor;
	int placeholder[39];			//����ֵ
	int end = -1234567890;		//��Ϣβ
	char tableid[30];
	DataRecv()
	{
		memset(placeholder, 0, sizeof(placeholder));
		memset(tableid, 0, sizeof(tableid));
	}
};

struct DataRecvFaile_Response	//3.6.2 ����ʧ����Ӧ length = 24
{
	MsgHeader header_0;			//��Ϣͷ,���ȵ���24
	int id = 0xFFFFFFFF;		//��ʶ�� -1
	int error_code;				//������
	int end = -1234567890;		//��Ϣβ
};

struct DataRecv_RT_Response		//3.6.3 ʵʱ���ݽ�����Ӧ-1-2-3-5 �º�������Ӧ2 length = 80;
{
	MsgHeader header_0;			//��Ϣͷ,���ȵ���24
	int id = 0xEEEEEEE1;		//��ʶ��
	wchar_t satellite[20];		//������
	int state;				//����״̬
	int placeholder[4];			//����ֵ
	int end;			//��Ϣβ
	char tableid[30];
	DataRecv_RT_Response()
	{
		memset(placeholder, 0, sizeof(placeholder));
		memset(tableid, 0, sizeof(tableid));
	}
};


struct TMBlock					//Ĭ��Ϊԭʼ����
{
	char *frame_data;			//ͬ�����ݲŻ�ʹ�õ�
	char *data;
	int	lastdata;				//���<=8�ֽ�����+���ܴ��ڵ����������
	int tm_time[2];				//TM��ʱ��
	int tm_state;				//TM��״̬
	int placeholder;				//����ֵ
};


struct DataRecv_RT_Data		//3.6.6 ʵʱ���ݽ�����Ӧ-4  3.6.8�º�������Ӧ1
{
	MsgHeader header_0;			//��Ϣͷ
	int id = 0xEEEEEEE4;		//��ʶ��
	wchar_t satellite[20];		//������
	int frame_header_length;	//֡ͷ��
	int frame_lenght;			//֡��
	int timeblock_length;		//ʱ��������
	int tm_stateblock_length;	//TM��״̬������
	int tm_length;				//TM�鳤��
	int tm_number;				//TM����
	int tm_header_length = 0;	//TM�ļ�ͷ����(��֪��Ϊʲô����Ϊ0)
	int placeholder[43];
	TMBlock *TMB;
	int end;
	char tableid[30];
	char path[30];				//TBVBlock�ı���·��
	DataRecv_RT_Data()
	{
		memset(placeholder, 0, sizeof(placeholder));
		memset(tableid, 0, sizeof(tableid));
		memset(path, 0, sizeof(path));
	}
};


//int BitSet(int a, int b, int c)//�ֱ���0��8��16bitλ
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
