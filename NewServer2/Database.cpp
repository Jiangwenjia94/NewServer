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