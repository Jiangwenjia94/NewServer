#pragma once
#include <windows.h>
#include <mysql.h>
#include <string>
using namespace std;

class Database
{
public:
	Database();
	~Database();

	bool Connect();
	void FreeConnect();

	bool SelectData(char * SQL, string *Data,char * Msg);
	bool InsertData(char * SQL, char * Msg);
	bool GetDataRC(char * SQL, int *RC,char * Msg);
	int  InsertIntoUser(int id, int isfirst, int index, char *usernum);//return 0 插入成功 1 插入失败 2 已存在
	bool InsertIntoOrder(char *usernum, int ordernum, char *orderinfo, char *ordersta, int issend);
	bool SelectImage(char *name, string &data);
	bool UpdateOnecl(char *table, char *column, char *value, char *condition, char *some_column, string &data);
	bool UpdateUserIsfirst(char *value, char *user_num);
	bool UpdateOrderIssend(char *value, int order_num);
	bool GetUserIsfirst(char *usernum,string &data);
	bool GetReadyOrder(char *usernum, string *(&data));
	bool KeepOrderSyn(char *order_info,char *issend);
private:
	MYSQL mysql;
};

