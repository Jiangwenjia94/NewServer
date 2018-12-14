#include <cstring>
#include "DataJudgment.h"

#include <iostream>		//��ɾ��
using namespace std;

DataJudgment::DataJudgment(void)
{
	bPictureReady = true; //for S_HEART_IMAGE_READLY 3
	bNoticeReady = false;
	bPictureRecv = true;
	bNoticeRecv = true;
	is_identify = false;
	db.Connect();
//	out.open("out.txt", ios::app | ios::out);
}


DataJudgment::~DataJudgment(void)
{
}

bool DataJudgment::isSame(char *strA, char *strB)
{
	int count = 0;
	for (int i = 0; i < 64; i++)
	{
		if (strA[i] == strB[i])
			count++;
	}
	if (count == 64)
		return TRUE;
	else
		return FALSE;
}

bool DataJudgment::Push(USER &user, vector<USER>&client)//����Ѵ�����ԭ��ʣ����Ϣ���ƣ���������push
{
	int count = 0;
	for (int i = 0; i < client.size(); i++)
	{
		if (isSame(user.user_num, client[i].user_num))
		{
			user = client[i];//��������ͬuser_num�ı㸴�� //�ڶ�����������ʱ��û�и��Ƴɹ�
			user.index = i;
			return FALSE;
		}
		count++;
	}
	if (count == client.size())
	{
		client.push_back(user);
		user.index = count;
	}
	return TRUE;
}



int DataJudgment::staque()
{
	if(bPictureReady)
		return S_HEART_IMAGE_READLY;
	else if(bNoticeReady)
		return S_HEART_NOTIC_READLY;
	else
		return S_HEART_LIVE_FEED;
}

char* DataJudgment::infoStateInquiry(int num)
{
	char info[100];
	int size = 0;
	int now_num = 0;
	int next_num = 0;
	char now_info[20];
	char next_info[20];
	
	if (user->user_order.empty())
	{
		char *info_null = "the queue is NULL";
		return info_null;
	}
	else
	{
		size = user->user_order.size();
	}
	for (int i = 0; i < user->user_order.size(); i++)
	{
		if (num == user->user_order[i].order_num)
		{
			strncpy(now_info, user->user_order[i].order_info, sizeof(now_info));
			now_num = user->user_order[i].is_send;
			if (i + 1 < size)
			{
				strncpy(next_info, user->user_order[i + 1].order_info, sizeof(now_info));
				next_num = user->user_order[i + 1].order_num;
			}
			else
			{
				strncpy(next_info, "NULL", sizeof(now_info));
				next_num = 0;
			}
			sprintf(info, "%s%d%s%d%s%s%s%d%s%s", "vector size: ", size, " state: ", now_num, " info: ", now_info, " next: ", next_num, " info: ", next_info);
			break;
		}
	}
	return info;
}

int CreatOrder()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);
	int iOrder = st.wHour * 10000;
	iOrder += st.wMinute * 100;
	iOrder += st.wSecond;
	return iOrder;
}

int	DataJudgment::getOrderNum()
{
	int ordernum = 0;
	for (auto iter = user->user_order.cbegin(); iter != user->user_order.cend(); iter++)
	{
		if ((*iter).is_send == 0)
		{
			ordernum = (*iter).order_num;
			return ordernum;
		}
	}
	return ordernum;
}
CString DataJudgment::getOrderInfo(int ordernum)
{
	CString path = "NULL";
	for (auto iter = user->user_order.cbegin(); iter != user->user_order.cend(); iter++)
	{
		if ((*iter).order_num== ordernum)
		{
			path = (*iter).order_info;
			return path;
		}
	}
	return path;
}

bool DataJudgment::getOrderInfo(int ordernum, char *(&buffer))
{
	for (auto iter = user->user_order.cbegin(); iter != user->user_order.cend(); iter++)
	{
		if ((*iter).order_num == ordernum)
		{
			buffer = (*iter).order_info;
			return true;
		}
	}
	return true;
}

void DataJudgment::setOrdersta(int ordernum,int sta)
{
	for (int i = 0; i < user->user_order.size() ; i++)
	{
		if (user->user_order[i].order_num == ordernum)
		{
			user->user_order[i].is_send = sta;
		}
	}
}

int DataJudgment::recvCurHrt(ClientParam *param,bool fir)
{
	int r_order = C_LINK;
	int s_order = -9999;
	int datalength = 0;
	int sen = 0;
	int rec = 0;
	
	if (fir == true/*(*m_client)[user->index].isfirt*/)
	{
		memset(user->user_num, 0, sizeof(user->user_num));
		cout << "��һ������" << endl;
		rec = Trans.Recv(r_order);
		if (rec <= 0 || r_order != C_LINK)
			return -9999;//δ�յ���֤��Ϣ
		s_order = S_LINK;
		sen = Trans.Send(s_order);
		if (sen <= 0)
			return -9998;
		rec = Trans.Recv(user->user_num, 64);//����64�ֽ��û������user_num��
		if (rec != 64)
			return -9999;//�û���Ų���
		sen = Trans.Send(user->user_num, 64);//�ط��û����
		int ret = db.InsertIntoUser(9, 0, 9, user->user_num);
		if (ret == 1) //�������
			return 1;
	//	else if (ret == 2) //���ݿ����Ѵ���

	//	cout <<"insert: "<< db.InsertIntoUser(9, 0, 9, user->user_num) << endl;;
		Push(*user, *m_client);
//		cout << "index: " << user->index << endl;
		if (sen <= 0)
			return -9998;
		is_identify = true;
		cout << "��֤ͨ��" << endl;
	}

	if( r_order == C_LINK && is_identify) //��֤ͨ��
	{
		
		//�û���֤��Ψһ��ȷ�������ӳɹ�
		cout << "��������" << endl;
		//��ʼ�������������ݴ���
//		int nNetTimeout = 5000;//5s ��ʱ����
//		setsockopt(param->ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
		(*m_client)[user->index].isfirt = false;
		if (db.UpdateUserIsfirst("1",user->user_num))
			cout << "successed" << endl;
		else
			cout << "false" << endl;
		rec = Trans.Recv(r_order);
		cout << r_order << endl;//r_order�洢�յ���������Ϣ
		//��ʱrec����-1
		if (rec <= -1)
			return REC_ERROR_OFFLINE;//�����رտͻ��ˡ�����
		else if(rec == 0)
			return REC_ERROR_CLOSE;//ʹ��CloseSocket�ر�����

		switch (r_order)					//break�൱�ڼ���������Ӧ
		{
		case C_HEART_LIVE_NOTICE:							//��ͨ������Ϣ
			{
				s_order = staque();		//s_orderӦ����һ��״̬��������,���ݶ�Ϊȫ������
				cout<<"��������1"<<endl;
			//	if(s_order == S_HEART_LIVE_FEED)
				sen = Trans.Send(s_order);
				break;
			}
		 case C_HEART_IMAGE_ASK:							//ͼ������
			{
				ORDER cur_order;
				if(bPictureRecv)
					s_order = S_HEART_ASK_ACCEPT;//��S_HEART_ASK_REFUSE
				else
					s_order = S_HEART_ASK_REFUSE;
				sen = Trans.Send(s_order);//�Ƿ����
				if(sen <= 0)
					break; 
				
				if(s_order == S_HEART_ASK_ACCEPT)
				{
					

					s_order = CreatOrder();				//����һ��Ψһ��ָ����
					cur_order.order_num = s_order;
					cout << s_order << endl;
					//		s_order = MakeCode();//makecode������Ҫ���ǵ�serverstate���������
					if(s_order == 0)		//�������ܾ�����
					{
						sen = Trans.Send(s_order);
						if(sen <= 0)
							break;
					}
					sen = Trans.Send(s_order);//����ָ����
					if(sen <= 0)
						break;
					rec = Trans.Recv(datalength);//����ָ���
					if(rec <= 0)
						break;
					cur_order.order_info = new char[datalength];
					rec = Trans.Recv(cur_order.order_info,datalength);//����ָ����Ϣ�����֣���γ�ȵȵȣ�
					cout << cur_order.order_info << endl;
					if(rec == datalength)
					{
						s_order = S_HEART_ASK_SUCCESS;
						sen = Trans.Send(s_order);
						if(sen <= 0)
							break;
					}
					else
					{
						s_order = S_HEART_ASK_FAIL;
						sen = Trans.Send(s_order);
						break;
					}
				}
				/*
					�˴�����ָ���״̬��Ϣ

				*/
				cur_order.order_sta = "999";
				//��ӵ�USER.user_order Vectorβ����
				user->user_order.push_back(cur_order);
				(*m_client)[user->index] = *user;//ͬ��client�е�user
				if (db.InsertIntoOrder(user->user_num, cur_order.order_num, cur_order.order_info, cur_order.order_sta, cur_order.is_send))
					cout << "order insert successed" << endl;
				else
					cout << "order insert failed" << endl;
				s_order = S_HEART_LIVE_FEED;
				break;
			}
		case C_HEART_IMAGE_READLY:							
		{
			bool romexist = false;
			bool dbexist = false;
			int curorder = false;//��һ��������Ψһ��� 
			CString path;
			curorder = getOrderNum(); //�ȴ��ڴ��е���������
			if (curorder != 0)
			{
				path = getOrderInfo(curorder);
				romexist = true;
			}

			if (curorder == 0)// �ڴ�������û�д�����ͼƬ��������ݿ��б�������δ���͵�ͼƬ
			{
				string *Info_Num;
				if (db.GetReadyOrder(user->user_num, Info_Num))	//���ܻ��ȡ���飬ֻȡ��һ��
				{
					curorder = atoi(Info_Num[0].c_str());
					path = Info_Num[1].c_str();
					dbexist = true;
					delete[] Info_Num;
				}
				
			}

			s_order = curorder;
			if (s_order == 0)
				break;
			sen = Trans.Send(s_order);
			if(sen <= 0)
				break;
			if (romexist)
			{
				setOrdersta(curorder, 2);
			//	char *keepinfo = 
				USES_CONVERSION;
				db.KeepOrderSyn(T2A(path.GetBuffer()), "2");
				path.ReleaseBuffer();
			}
			else if (dbexist)
				db.UpdateOrderIssend("2", curorder);

			if (!user->user_order.empty() || dbexist)
				Trans.sendCurrentPic(path);
				//Trans.sendCurrentPic(user->user_order.front().order_info); //�Ӷ�����ȡ��һ��ָ��������Ⱥ�ͼ������
			//int temp = -9999;
			rec = Trans.Recv(r_order);//����6666��-2 //wenti
			if(rec <= 0)
				break;
			if(r_order == C_HEART_IMAGE_FAIL)
			{
				s_order = S_HEART_IMAGE_READLY;
				break;
			}
			if (romexist)
			{
				setOrdersta(curorder, 1);
				USES_CONVERSION;
				db.KeepOrderSyn(T2A(path.GetBuffer()), "1");
				path.ReleaseBuffer();
			}
			else if (dbexist)
				db.UpdateOrderIssend("1", curorder);
		//	user->user_order.erase(user->user_order.begin());//ɾ��Vector��һ��Ԫ��
			(*m_client)[user->index] = *user;//���ܶ�clientʹ��front()��Ӧ����user����clientʱ����һ����ţ�ͨ����ŷ���
			/*
				�˴�����ָ���״̬��Ϣ
			*/
			
			s_order = S_HEART_LIVE_FEED;
			break;
		}
		case C_HEART_IMAGE_REFUSE:							
		{
			s_order = S_HEART_IMAGE_READLY;//�ܾ���retry
			break;
		} 
		case C_HEART_NOTIC_READLY:
			{
			//	sendCurrentMsg(param,orderinfo);////�������Ⱥ�״̬��Ϣ
				if(!bNoticeReady)//�������ᴥ��
				{
					s_order = S_HEART_LIVE_FEED;
					break;
				}
				char *stainfo = "12345678";
				int length = 9;
				sen = Trans.Send(length);
				if(sen <= 0)
					break;
				sen = Trans.Send(stainfo,length);
				if(sen <= 0)
					break;
				rec = Trans.Recv(r_order);//����4444��-6
				if(rec <= 0)
					break;
				if(r_order == C_HEART_NOTIC_FAIL)
				{
					s_order = S_HEART_NOTIC_READLY;
					break;
				}
				s_order = S_HEART_LIVE_FEED;
				break;
			}
		case C_HEART_NOTIC_ASK:
			{
				if(bNoticeRecv)
					s_order = S_HEART_ORDER_RECEIVE;
				else
					s_order = S_HEART_ORDER_REFUSE;
			
				sen = Trans.Send(s_order);//�����Ƿ����
				if(sen <= 0)
					break;
				if(s_order == S_HEART_ORDER_RECEIVE)
				{
					int ordnum = 0;
					rec = Trans.Recv(ordnum);
					//
					//��ordnumָ��״̬��ѯ�õ�stainfo
					char stainfo[100];
					strncpy(stainfo, infoStateInquiry(ordnum),sizeof(stainfo));
					//
				//	char *stainfo = "12345678";//�ɲ�ѯ��õ�
					int length = 100;
					sen = Trans.Send(length);
					if(sen <= 0)
						break;
					sen = Trans.Send(stainfo,length);
					if(sen <= 0)
						break;
					rec = Trans.Recv(r_order);//����4444��-6
					if(rec <= 0)
						break;
					if(r_order == C_HEART_NOTIC_FAIL)
					{
						s_order = S_HEART_NOTIC_READLY;
						break;
					}
				}
				s_order = S_HEART_LIVE_FEED;
				break;

			}
		case C_HEART_NOTIC_REFUSE:
			{
				s_order = S_HEART_NOTIC_READLY;
				break;
			}
		case C_HEART_ORBIT_ASK:
			{
				int accept = 8;			//  8 / -8 �Ƿ���ܲ�ѯ�����������
				int length = 10;
				int unit = -1;
				double temptime[8] = { 0.0 };
				double querytime[6] = { 0.0 };
				double pos[3] = { 0 };
				double *twodpos;
				char fn[] = "20180824.txt";		//��ʽ����  ��ʱд��

				sen = Trans.Send(accept);
				if (sen <= 0)
					break;

				rec = Trans.Recv((char*)temptime, sizeof(temptime));
				if (rec <= 0)
					break;
				for (int i = 0; i < 6; i++)
				{
					querytime[i] = temptime[i];
				}
				unit	= temptime[6];
				length	= temptime[7];
				twodpos = new double[3 * length];
			//	double qtime[6] = {2018,8,24,0,0,0};
				LJOrbit.OribitQueryPerTime(fn, twodpos, querytime,unit,length);
				for (int i = 0; i < length; i++)
				{
					pos[3] = { 0 };
					pos[0] = twodpos[i*3];
					pos[1] = twodpos[i*3+1];
					pos[2] = twodpos[i*3+2];
					sen = Trans.Send((char*)pos, sizeof(pos));
					if (sen <= 0)
						break;
				}		

				rec = Trans.Recv(r_order);
				if (r_order == C_HEART_ORBIT_FAIL)
				{
					s_order = C_HEART_ORBIT_ASK;
					break;
				}
			}
		default:
			{
				s_order = S_HEART_ERROR;
				return 0;
			}
		}
		//8888h û�н�������
	//	if((s_order == S_HEART_LIVE_FEED && r_order == C_HEART_LIVE_NOTICE) || (s_order == S_HEART_LIVE_FEED && r_order == C_HEART_IMAGE_SUCCESS))//�ָ�������Ӧ
		if(s_order == S_HEART_LIVE_FEED && (r_order == C_HEART_LIVE_NOTICE || r_order == C_HEART_IMAGE_SUCCESS || r_order == C_HEART_IMAGE_ASK || r_order == C_HEART_NOTIC_SUCCESS))
		{			//���һ��switch��ָ�����
			//rec = Trans.Recv(r_order);
			//if (rec <= -1)
			//	return REC_ERROR_OFFLINE;//�����رտͻ��ˡ�����
			//else if (rec == 0)
			//	return REC_ERROR_CLOSE;//ʹ��CloseSocket�ر�����

			//cout<<"��������2"<<endl;
			//sen = Trans.Send(s_order);
 		//	if(sen <= 0)
			//	return SEN_ERROR;
			return 0;
		}
		else if(s_order == S_HEART_IMAGE_READLY || s_order == S_HEART_NOTIC_READLY)
		{
			
			return 0;
		}
		
	}
	return 0;
}

UINT DataJudgment::sendCurrentPic(ClientParam *param, CString path)
{
	SOCKET serverSocket = param->ClientSocket;
	string strError;
	CFile file;
	int error = 0;
	LPCTSTR Path = path;
	if (!file.Open(Path,CFile::modeRead | CFile::typeBinary))
	{
	//	AfxMessageBox("���ļ�ʧ��");
		cout<<"���ļ�ʧ��";
		return 1;
	}
	char dataType =  '1';
	int lFileSize = file.GetLength();
	char temp[1010];							//������������ļ���С���������������ļ�������
	memset(temp, 0, sizeof(temp)); 

	int ret1 = send(serverSocket,(char *)&lFileSize,sizeof(int),0);
//	int ret1 = send(serverSocket, c, str.GetLength(), 0);	//�����ļ������ƺʹ�С
	if (ret1 == SOCKET_ERROR)
	{
		cout<<"Send File:"<< WSAGetLastError();
		return 1;
	}

	Sleep(100);//���̫�콫������һ�η��͵ģ����ڻ�����δ���������send�ķ�һ��
			   //��������recv����˵��»��ٽ���һ��
	/////////////////////////�����ļ�///////////////////////////////////////////
	int iEnd = 0;
	int ilen = 0;
	long iTmp = 0;
	int p = 0;
	while (1)
	{
		memset(temp, 0, sizeof(temp)); 
		ilen = file.Read(temp, 1010);
		if (ilen == 0)
		{
			break;
		}
		/*if(p == 105)
		{
			cout<<"wait for 5 sec"<<endl;
			Sleep(5000);
		}*/
		iEnd = send(serverSocket, temp, ilen, 0);
		if (iEnd == SOCKET_ERROR)
		{
			cout<<"Send File:"<< WSAGetLastError();
		//	AfxMessageBox(strError);
			break;
		}
		iTmp += iEnd;
		p++;
		cout<<iTmp<<" : "<<iEnd<<" : "<<p<<endl;

		char temp2[2];
		memset(temp2,0,sizeof(temp2));
		int t2 = recv(serverSocket,temp2,1,0);
		cout<<temp2<<endl;
	}
	file.Close();

		/////////////////�жϷ����Ƿ�ɹ�����������////////////////////////////////////
//	closesocket(LocalSocket);
	if (iTmp == lFileSize)
	{
		cout<<"�ļ����ͳɹ���";
	}
	else
	{
		cout<<"�ļ�����ʧ��";
		return 0;
	}
	
	return 0;
}

int DataJudgment::recvHeart(bool fir)
{
//	sendCurrentMsg(param.serverSocket, data);
	int order = recvCurHrt(&param,fir);
	return order;
}