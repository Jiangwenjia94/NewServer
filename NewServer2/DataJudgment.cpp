#include <cstring>
#include "DataJudgment.h"

#include <iostream>		//需删掉
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

bool DataJudgment::Push(USER &user, vector<USER>&client)//编号已存在则将原本剩余消息复制，不存在则push
{
	int count = 0;
	for (int i = 0; i < client.size(); i++)
	{
		if (isSame(user.user_num, client[i].user_num))
		{
			user = client[i];//遍历到相同user_num的便复制 //第二次连进来的时候没有复制成功
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
		cout << "第一次连接" << endl;
		rec = Trans.Recv(r_order);
		if (rec <= 0 || r_order != C_LINK)
			return -9999;//未收到验证消息
		s_order = S_LINK;
		sen = Trans.Send(s_order);
		if (sen <= 0)
			return -9998;
		rec = Trans.Recv(user->user_num, 64);//接收64字节用户编号于user_num中
		if (rec != 64)
			return -9999;//用户编号不对
		sen = Trans.Send(user->user_num, 64);//回发用户编号
		int ret = db.InsertIntoUser(9, 0, 9, user->user_num);
		if (ret == 1) //存入错误
			return 1;
	//	else if (ret == 2) //数据库中已存在

	//	cout <<"insert: "<< db.InsertIntoUser(9, 0, 9, user->user_num) << endl;;
		Push(*user, *m_client);
//		cout << "index: " << user->index << endl;
		if (sen <= 0)
			return -9998;
		is_identify = true;
		cout << "验证通过" << endl;
	}

	if( r_order == C_LINK && is_identify) //验证通过
	{
		
		//用户验证与唯一性确定，连接成功
		cout << "进入心跳" << endl;
		//开始数据心跳与数据传输
//		int nNetTimeout = 5000;//5s 超时设置
//		setsockopt(param->ClientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
		(*m_client)[user->index].isfirt = false;
		if (db.UpdateUserIsfirst("1",user->user_num))
			cout << "successed" << endl;
		else
			cout << "false" << endl;
		rec = Trans.Recv(r_order);
		cout << r_order << endl;//r_order存储收到的心跳信息
		//超时rec返回-1
		if (rec <= -1)
			return REC_ERROR_OFFLINE;//主动关闭客户端、断网
		else if(rec == 0)
			return REC_ERROR_CLOSE;//使用CloseSocket关闭连接

		switch (r_order)					//break相当于继续心跳回应
		{
		case C_HEART_LIVE_NOTICE:							//普通心跳信息
			{
				s_order = staque();		//s_order应该由一个状态函数决定,现暂定为全部接受
				cout<<"心跳连接1"<<endl;
			//	if(s_order == S_HEART_LIVE_FEED)
				sen = Trans.Send(s_order);
				break;
			}
		 case C_HEART_IMAGE_ASK:							//图像请求
			{
				ORDER cur_order;
				if(bPictureRecv)
					s_order = S_HEART_ASK_ACCEPT;//或S_HEART_ASK_REFUSE
				else
					s_order = S_HEART_ASK_REFUSE;
				sen = Trans.Send(s_order);//是否接收
				if(sen <= 0)
					break; 
				
				if(s_order == S_HEART_ASK_ACCEPT)
				{
					

					s_order = CreatOrder();				//产生一个唯一的指令编号
					cur_order.order_num = s_order;
					cout << s_order << endl;
					//		s_order = MakeCode();//makecode函数需要考虑到serverstate函数的情况
					if(s_order == 0)		//服务器拒绝请求
					{
						sen = Trans.Send(s_order);
						if(sen <= 0)
							break;
					}
					sen = Trans.Send(s_order);//发送指令编号
					if(sen <= 0)
						break;
					rec = Trans.Recv(datalength);//接收指令长度
					if(rec <= 0)
						break;
					cur_order.order_info = new char[datalength];
					rec = Trans.Recv(cur_order.order_info,datalength);//接收指令信息（名字，经纬度等等）
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
					此处增加指令的状态信息

				*/
				cur_order.order_sta = "999";
				//添加到USER.user_order Vector尾部中
				user->user_order.push_back(cur_order);
				(*m_client)[user->index] = *user;//同步client中的user
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
			int curorder = false;//上一步产生的唯一编号 
			CString path;
			curorder = getOrderNum(); //先从内存中的容器查找
			if (curorder != 0)
			{
				path = getOrderInfo(curorder);
				romexist = true;
			}

			if (curorder == 0)// 内存容器内没有待发送图片，则从数据库中遍历查找未发送的图片
			{
				string *Info_Num;
				if (db.GetReadyOrder(user->user_num, Info_Num))	//可能会获取多组，只取第一组
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
				//Trans.sendCurrentPic(user->user_order.front().order_info); //从队列里取第一个指令，包括长度和图像数据
			//int temp = -9999;
			rec = Trans.Recv(r_order);//接收6666或-2 //wenti
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
		//	user->user_order.erase(user->user_order.begin());//删除Vector第一个元素
			(*m_client)[user->index] = *user;//不能对client使用front()，应当在user进入client时返回一个编号，通过编号访问
			/*
				此处更新指令的状态信息
			*/
			
			s_order = S_HEART_LIVE_FEED;
			break;
		}
		case C_HEART_IMAGE_REFUSE:							
		{
			s_order = S_HEART_IMAGE_READLY;//拒绝则retry
			break;
		} 
		case C_HEART_NOTIC_READLY:
			{
			//	sendCurrentMsg(param,orderinfo);////包括长度和状态信息
				if(!bNoticeReady)//正常不会触发
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
				rec = Trans.Recv(r_order);//接收4444或-6
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
			
				sen = Trans.Send(s_order);//发送是否接收
				if(sen <= 0)
					break;
				if(s_order == S_HEART_ORDER_RECEIVE)
				{
					int ordnum = 0;
					rec = Trans.Recv(ordnum);
					//
					//由ordnum指令状态查询得到stainfo
					char stainfo[100];
					strncpy(stainfo, infoStateInquiry(ordnum),sizeof(stainfo));
					//
				//	char *stainfo = "12345678";//由查询后得到
					int length = 100;
					sen = Trans.Send(length);
					if(sen <= 0)
						break;
					sen = Trans.Send(stainfo,length);
					if(sen <= 0)
						break;
					rec = Trans.Recv(r_order);//接收4444或-6
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
				int accept = 8;			//  8 / -8 是否接受查询轨道坐标数据
				int length = 10;
				int unit = -1;
				double temptime[8] = { 0.0 };
				double querytime[6] = { 0.0 };
				double pos[3] = { 0 };
				double *twodpos;
				char fn[] = "20180824.txt";		//方式待定  暂时写死

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
		//8888h 没有进入这里
	//	if((s_order == S_HEART_LIVE_FEED && r_order == C_HEART_LIVE_NOTICE) || (s_order == S_HEART_LIVE_FEED && r_order == C_HEART_IMAGE_SUCCESS))//恢复心跳回应
		if(s_order == S_HEART_LIVE_FEED && (r_order == C_HEART_LIVE_NOTICE || r_order == C_HEART_IMAGE_SUCCESS || r_order == C_HEART_IMAGE_ASK || r_order == C_HEART_NOTIC_SUCCESS))
		{			//完成一次switch后恢复心跳
			//rec = Trans.Recv(r_order);
			//if (rec <= -1)
			//	return REC_ERROR_OFFLINE;//主动关闭客户端、断网
			//else if (rec == 0)
			//	return REC_ERROR_CLOSE;//使用CloseSocket关闭连接

			//cout<<"心跳连接2"<<endl;
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
	//	AfxMessageBox("打开文件失败");
		cout<<"打开文件失败";
		return 1;
	}
	char dataType =  '1';
	int lFileSize = file.GetLength();
	char temp[1010];							//首先用来存放文件大小，后面用作发送文件缓冲区
	memset(temp, 0, sizeof(temp)); 

	int ret1 = send(serverSocket,(char *)&lFileSize,sizeof(int),0);
//	int ret1 = send(serverSocket, c, str.GetLength(), 0);	//发送文件的名称和大小
	if (ret1 == SOCKET_ERROR)
	{
		cout<<"Send File:"<< WSAGetLastError();
		return 1;
	}

	Sleep(100);//如果太快将导致上一次发送的（由于缓冲区未满）与后面send的放一起，
			   //被服务器recv，因此导致会少接收一次
	/////////////////////////发送文件///////////////////////////////////////////
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

		/////////////////判断发送是否成功、结束处理////////////////////////////////////
//	closesocket(LocalSocket);
	if (iTmp == lFileSize)
	{
		cout<<"文件发送成功！";
	}
	else
	{
		cout<<"文件发送失败";
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