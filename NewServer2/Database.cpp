#include "stdafx.h"
#include "Database.h"


Database::Database()
{
}


Database::~Database()
{
}

bool Database::Connect()
{
	const char user[] = "root";
	const char pswd[] = "123";
	const char host[] = "localhost";
	const char db[] = "test";
	unsigned int port = 3306;
	mysql_init(&mysql);
	if (!(mysql_real_connect(&mysql, host, user, pswd, db, 3306, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
	{
		printf("Error connecting to database:%s\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		printf("Connected...\n");
		return true;
	}
}

void Database::FreeConnect()
{

}

#include <iostream>

bool Database::GetDataRC(char * SQL, int *RC,char * Msg)
{
	MYSQL_RES *m_res;
	char sql[2048];
	sprintf_s(sql, SQL);
	if (mysql_query(&mysql, sql) != 0)
	{
		Msg = "select ps_info Error";
		return 0;
	}
	m_res = mysql_store_result(&mysql);

	if (m_res == NULL)
	{
		Msg = "select username Error";
		return 0;
	}
	RC[0] = mysql_num_rows(m_res);
	RC[1] = mysql_num_fields(m_res);

	return true;
}

bool Database::SelectData(char * SQL,string *Data, char * Msg)
{
	MYSQL_ROW m_row;
	MYSQL_RES *m_res;
	char sql[2048];
	sprintf_s(sql, SQL);
	int rnum = 0;
	//char rg = 0x06;//行隔开
	//char cg = { 0x05 };//字段隔开
	char rg = ' ';//行隔开
	char cg = ',';//字段隔开

	if (mysql_query(&mysql, sql) != 0)
	{
		Msg = "select ps_info Error";
		return 0;
	}
	m_res = mysql_store_result(&mysql);

	if (m_res == NULL)
	{
		Msg = "select username Error";
		return 0;
	}
	string str("");
	int row_num, col_num;
	row_num = mysql_num_rows(m_res);
	col_num = mysql_num_fields(m_res);
	//MYSQL_FIELD *fd;
	//while (fd = mysql_fetch_field(m_res)) {
	//	std::cout << fd->name << "\t";
	//
	//}

	MYSQL_ROW sql_row;
	int j = 0;
	while (sql_row = mysql_fetch_row(m_res)) {
		for (int i = 0; i < col_num; i++) {
			if (sql_row[i] != NULL) 
			{
				Data[j] = sql_row[i];
				j++;
			}
		}
		
	}
	/*while (m_row = mysql_fetch_row(m_res))
	{
		for (int i = 0; i < Cnum; i++)
		{
			str += m_row[i];
			str += rg;
		}
		str += rg;
		rnum++;
	}*/

	mysql_free_result(m_res);

	return true;
}

bool Database::InsertData(char * SQL, char * Msg)
{
	char sql[2048];
	sprintf_s(sql, SQL);
	if (mysql_query(&mysql, sql) != 0)
	{
		Msg = "Insert Data Error";
		return false;
	}
	return true;
}

int Database::InsertIntoUser(int id, int isfirst, int index, char *usernum)
{
	//////
	MYSQL_RES *m_res;
	char *epresql = "select * from user where user_number='";
	char isexist[1024];
	memset(isexist, 0, sizeof(isexist));
	sprintf_s(isexist, "%s%s%s", epresql,usernum,"'");
	if (mysql_query(&mysql, isexist)!=0)
		return 1;
	m_res = mysql_store_result(&mysql);
	if (mysql_num_rows(m_res))
		return 2;
	mysql_free_result(m_res);
	//////
	char *upresql = "insert into user(id,m_isfirst,m_index,user_number) values(";
	char sql[1024];
	memset(sql,0,sizeof(sql));
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%d%s%s%s%s", upresql, id, ",", isfirst, ",", index, ",", "'", usernum, "')");
	char* Msg = "";
	if (!InsertData(sql, Msg))
		return 1;
	return 0;
}
bool Database::InsertIntoOrder(char *usernum, int ordernum, char *orderinfo, char *ordersta, int issend)
{
	char *opresql = "insert into recvorder(order_number,m_issend,user_number,order_info,order_state) values(";
	char sql[1024];
	memset(sql, 0, sizeof(sql));
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%s%s%s%s%s%s", opresql, ordernum, ",", issend, ",'", usernum, "','", orderinfo, "','", ordersta, "')");
	
	char* Msg = "";
	if (!InsertData(sql, Msg))
		return false;
	return true;
}

bool Database::SelectImage(char *name,string &data)
{
	char *ipresql = "SELECT  m_url FROM image WHERE m_name = '";
	char sql[1024];
	sprintf_s(sql, sizeof(sql), "%s%s%s", ipresql, name, "'");
	char* Msg = "";
	if(!SelectData(sql,&data,Msg))
		return false;
	
	return true;
}

bool Database::UpdateOnecl(char *table, char *column, char *value,char *condition, char *some_column, string &data)
{
	char *ipresql = "UPDATE ";
	char sql[1024];
	sprintf_s(sql, sizeof(sql), "%s%s%s%s%s%s%s%s%s%s%s", ipresql, table, " SET ", column, "=", value, " WHERE ", some_column, "='", condition, "'");
	char* Msg = "";
	if(mysql_query(&mysql, sql)!=0)
		return false;
	return true;
	/*if (!SelectData(sql, &data, Msg))
		return false;
		
	return true;*/
}

bool Database::UpdateUserIsfirst(char *value, char *user_num)
{
	string a;
	if (UpdateOnecl("user", "m_isfirst", value, user_num, "user_number", a))
		return true;
	return false;
}

bool Database::UpdateOrderIssend(char *value, int order_num)
{
	string a;
	char number[20];
	itoa(order_num,number,10);
	if (UpdateOnecl("recvorder", "m_issend", value, number, "order_number", a))
		return true;
	return false;
}

bool Database::GetUserIsfirst(char *usernum, string &data)
{
	char *ipresql = "SELECT  m_isfirst FROM user WHERE user_number = '";
	char sql[1024];
	sprintf_s(sql, sizeof(sql), "%s%s%s", ipresql, usernum, "'");
	char* Msg = "";
	if (!SelectData(sql, &data, Msg))
		return false;

	return true;
}

bool Database::GetReadyOrder(char *usernum, string *(&data))
{
	char *ipresql = "SELECT order_number,order_info FROM recvorder WHERE m_issend = 0";
	char* Msg = "";
	int RC[2] = { 0 };
	GetDataRC(ipresql,RC,Msg);
	int count = RC[0] * RC[1];
	if (count == 0)
		return false;
	string *temp = new string[count];
	if (!SelectData(ipresql, temp, Msg))
		return false;
	//for (int i = 0; i < count; i++)
	//	cout <<"GetReadyOrder:"<< temp[i] << endl;
	data = temp;

	return true;
}

bool Database::KeepOrderSyn(char *order_info, char *issend)
{
	string a;
	if (UpdateOnecl("recvorder", "m_issend", issend, order_info, "order_info", a))
		return true;
	return false;
}

///////////////////////////////终端入库/////////////////////////////////

bool Database::InsertGeometry(int id, float *coor)
{
	//MYSQL_RES *m_res;
	////////
	//char *upresql = "insert into testtable (id) values(";
	//char sql[1024];
	//memset(sql, 0, sizeof(sql));
	//sprintf_s(sql, sizeof(sql), "%s%d%s", upresql,id, ")");
	//char* Msg = "";
	//if (!InsertData(sql, Msg))
	//	return 1;

	float co[8] = { 123.11, 12.22, 124.22, 34.33, 125.33, 28.44, 123.11, 12.22 };
	int num = 4;
	char SQL[1024];
	memset(SQL, 0, sizeof(SQL));
	//	MakePoly(num,987,co,SQL);

	cout << SQL << endl;

	return 0;
}

//终端入库

void Database::MakePoly(int num, float *coor, char *SQL)
{
	char cSQL[1024];
	memset(cSQL, 0, sizeof(cSQL));
	char cPolygon[1024];
	memset(cPolygon, 0, sizeof(cPolygon));
	char *presql = "INSERT INTO testtable (id,coor) VALUES (";
	char *midsql = "(ST_PolygonFromText(";
	char *prepoly = "'POLYGON((";

	for (int i = 0; i < num * 2; i++)
	{
		if (i == 0)
		{
			sprintf_s(cPolygon, "%s%.3f", prepoly, coor[i]);
		}
		else if (i == num * 2 - 1)					//结尾
		{
			int size = strlen(cPolygon) + 1;
			char *temp = (char*)malloc(sizeof(char) * size);
			strcpy_s(temp, size, cPolygon);
			sprintf_s(cPolygon, "%s%s%.3f%s%.3f%s%.3f%s", temp, " ", coor[i], ",", coor[0], " ", coor[1], "))'");
			delete temp;
		}
		else if (i != 0 && i % 2 == 0)		//添加逗号
		{
			int size = strlen(cPolygon) + 1;
			char *temp = (char*)malloc(sizeof(char) * size);
			strcpy_s(temp, size, cPolygon);
			sprintf_s(cPolygon, "%s%s%.3f", temp, ",", coor[i]);
			delete temp;


		}
		else
		{
			int size = strlen(cPolygon) + 1;
			char *temp = (char*)malloc(sizeof(char) * size);
			strcpy_s(temp, size, cPolygon);
			sprintf_s(cPolygon, "%s%s%.3f", temp, " ", coor[i]);
			delete temp;
		}
	}
	sprintf_s(cSQL, "%s%s%s", midsql, cPolygon, "))");
	int allsize = strlen(cSQL) + 1;
	strcpy_s(SQL, allsize, cSQL);
}

bool Database::InsertIntoClientQueComd(DataQuery_0 DQ)
{
	char polysql[1024];
	memset(polysql, 0, sizeof(polysql));
	MakePoly(DQ.coor->num, DQ.coor->xy, polysql);
	char *opresql = "INSERT INTO client_querycommand (id,data_size,query_term,satellite_name,time_begin1,time_begin2,time_begin_years,time_end1,time_end2,time_end_years,coor_num,coordinate,max_listnum) VALUES (";
	char sql[1024];
	int id = 9;
	memset(sql, 0, sizeof(sql));
	char *satellite;
	size_t len = wcslen(DQ.satellite) + 1;
	size_t converted = 0;
	satellite = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, satellite, len, DQ.satellite, _TRUNCATE);
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%d%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%d%s", opresql, id, ",", DQ.header_0.length, ",", DQ.requirement, ",'", satellite, "',", DQ.time_begin[0], ",", DQ.time_begin[1], ",", DQ.time_begin_years, ",", DQ.time_end[0], ",", DQ.time_end[1], ",", DQ.time_end_years, ",", DQ.coor->num, ",", polysql, ",", DQ.list_max, ")");
	delete satellite;
	cout << sql << endl;
	char* Msg = "";
	if (!InsertData(sql, Msg))
		return false;
	return true;
}

bool Database::InsertIntoClientQueResp(DataQuery_0_Response DQR)
{
	int id = 99;
	int messageid = 12345;
	int *dataid;
	int listnum = DQR.list_number;
	dataid = new int[listnum];
	char polysql[1024];
	char *presql = "INSERT INTO client_queryresponse (id,message_id,data_size,max_listnum,data_id) VALUES (";
	char sql[1024];
	cout << "main: " << endl;
	for (int i = 0; i < listnum; i++)
	{
		dataid[i] = i;
		id++;
		memset(sql, 0, sizeof(sql));
		sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%d%s%d%s%d%s", presql, id, ",", messageid, ",", DQR.header_0.length, ",", DQR.list_number, ",", dataid[i], ")");
		cout << sql << endl;
		cout << endl;
		char* Msg = "";
		if (!InsertData(sql, Msg))
		{
			cout << "insert error!" << endl;
			return false;
		}
	}
	char *datapresql = "INSERT INTO client_queryresponse_data (id,message_id,satellite_name,time_begin1,time_begin2,time_begin_years,time_end1,time_end2,time_end_years,coor_num,coordinate,data_size,tm_num,tm_size,frame_length,framehead_length,task_num,record_num) VALUES (";
	char datasql[1024];
	memset(datasql, 0, sizeof(datasql));
	cout << "data: " << endl;
	for (int i = 0; i < listnum; i++)
	{
		memset(sql, 0, sizeof(sql));
		memset(polysql, 0, sizeof(polysql));
		MakePoly(DQR.qdata[i].coor->num, DQR.qdata[i].coor->xy, polysql);
		//	dataid[i] = i;
		char *satellite;
		size_t len = wcslen(DQR.qdata[i].name) + 1;
		size_t converted = 0;
		satellite = (char*)malloc(len*sizeof(char));
		wcstombs_s(&converted, satellite, len, DQR.qdata[i].name, _TRUNCATE);
		sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s%.3f%s%d%s%d%s%d%s%d%s%d%s%d%s",
			datapresql, dataid[i], ",", messageid, ",'", satellite, "',", DQR.qdata[i].time_begin[0], ",", DQR.qdata[i].time_begin[1],
			",", DQR.qdata[i].time_begin_year, ",", DQR.qdata[i].time_end[0], ",", DQR.qdata[i].time_end[1],
			",", DQR.qdata[i].time_end_year, ",", DQR.qdata[i].coor->num, ",", polysql, ",", DQR.qdata[i].data_size,
			",", DQR.qdata[i].tm_number, ",", DQR.qdata[i].tm_size, ",", DQR.qdata[i].frame_length, ",", DQR.qdata[i].frame_header_length,
			",", DQR.qdata[i].task_number, ",", DQR.qdata[i].record, ")");
		delete satellite;
		cout << sql << endl;
		cout << endl;
		char* Msg = "";
		if (!InsertData(sql, Msg))
		{
			cout << "insert error!" << endl;
			return false;
		}
	}
	return true;
}

bool Database::InsertIntoClientRecvComd(DataRecv DR,int id)
{
	char polysql[1024];
	memset(polysql, 0, sizeof(polysql));
	MakePoly(DR.coor->num, DR.coor->xy, polysql);
	char *opresql = "INSERT INTO client_datarecvcommand (id,data_size,request_code,recv_term,satellite_name,time_begin1,time_begin2,time_begin_years,time_end1,time_end2,time_end_years,coor_num,coordinate) VALUES (";
	char sql[1024];
	memset(sql, 0, sizeof(sql));
	char *satellite;
	size_t len = wcslen(DR.satellite) + 1;
	size_t converted = 0;
	satellite = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, satellite, len, DR.satellite, _TRUNCATE);
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%d%s%d%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%s%s", opresql, id, ",", DR.header_0.length, ",", DR.request_id, ",", DR.requirement, ",'", satellite, "',", DR.time_begin[0], ",", DR.time_begin[1], ",", DR.time_begin_years, ",", DR.time_end[0], ",", DR.time_end[1], ",", DR.time_end_years, ",", DR.coor->num, ",", polysql, ")");
	delete satellite;
	cout << sql << endl;
	char* Msg = "";
	if (!InsertData(sql, Msg))
	{
		cout << "insert error!" << endl;
		return false;
	}
	return true;
}

bool Database::InsertIntoClientPostData2(DataRecv_RT_Response DRTR,int id)
{
	char *opresql = "INSERT INTO client_post_data_2 (id,data_size,satellite_name,ready) VALUES (";
	char sql[1024];
	memset(sql, 0, sizeof(sql));
	char *satellite;
	size_t len = wcslen(DRTR.satellite) + 1;
	size_t converted = 0;
	satellite = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, satellite, len, DRTR.satellite, _TRUNCATE);
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%s%s%d%s", opresql, id, ",", DRTR.header_0.length, ",'", satellite, "',", DRTR.state, ")");
	delete satellite;
	cout << sql << endl;
	char* Msg = "";
	if (!InsertData(sql, Msg))
	{
		cout << "insert error!" << endl;
		return false;
	}
	return true;
}

bool Database::InsertIntoClientSynData1235(DataRecv_RT_Response DRTR,int id)
{
	char *opresql = "INSERT INTO client_syn_data_1235 (id,data_size,satellite_name,ready) VALUES (";
	char sql[1024];
	memset(sql, 0, sizeof(sql));
	char *satellite;
	size_t len = wcslen(DRTR.satellite) + 1;
	size_t converted = 0;
	satellite = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, satellite, len, DRTR.satellite, _TRUNCATE);
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%s%s%d%s", opresql, id, ",", DRTR.header_0.length, ",'", satellite, "',", DRTR.state, ")");
	delete satellite;
	cout << sql << endl;
	char* Msg = "";
	if (!InsertData(sql, Msg))
	{
		cout << "insert error!" << endl;
		return false;
	}
                                       	return true;
}

bool Database::InsertIntoClientSynData4(DataRecv_RT_Data DRTD)
{
	char *opresql = "INSERT INTO client_syn_data_4 (id,data_size,satellite_name,framehead_length,frame_length,time_length,tm_state_length,tm_length,tm_num,tm_path) VALUES (";
	char sql[1024];
	memset(sql, 0, sizeof(sql));
	char *satellite;
	size_t len = wcslen(DRTD.satellite) + 1;
	size_t converted = 0;
	int id = DRTD.path;
	satellite = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, satellite, len, DRTD.satellite, _TRUNCATE);
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s", opresql, id, ",", DRTD.header_0.length, ",'", satellite, "',", DRTD.frame_header_length, ",", DRTD.frame_lenght, ",", DRTD.timeblock_length, ",", DRTD.tm_stateblock_length, ",", DRTD.tm_length, ",", DRTD.tm_number, ",'", DRTD.path, "')");
	delete satellite;
	cout << sql << endl;
	char* Msg = "";
	if (!InsertData(sql, Msg))
	{
		cout << "insert error!" << endl;
		return false;
	}
	return true;
}

bool Database::InsertIntoClientPostData1(DataRecv_RT_Data DRTD)
{
	char *opresql = "INSERT INTO client_post_data_1 (id,data_size,satellite_name,framehead_length,frame_length,time_length,tm_state_length,tm_length,tm_num,tm_path) VALUES (";
	char sql[1024];
	memset(sql, 0, sizeof(sql));
	char *satellite;
	size_t len = wcslen(DRTD.satellite) + 1;
	size_t converted = 0;
	int id = DRTD.path;
	satellite = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, satellite, len, DRTD.satellite, _TRUNCATE);
	sprintf_s(sql, sizeof(sql), "%s%d%s%d%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s", opresql, id, ",", DRTD.header_0.length, ",'", satellite, "',", DRTD.frame_header_length, ",", DRTD.frame_lenght, ",", DRTD.timeblock_length, ",", DRTD.tm_stateblock_length, ",", DRTD.tm_length, ",", DRTD.tm_number, ",'", DRTD.path, "')");
	delete satellite;
	cout << sql << endl;
	char* Msg = "";
	if (!InsertData(sql, Msg))
	{
		cout << "insert error!" << endl;
		return false;
	}
	return true;
}