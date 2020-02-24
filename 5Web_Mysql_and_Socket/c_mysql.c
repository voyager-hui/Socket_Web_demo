/*****************************************************************************
名称: C语言与Mysql的连接和基本操作
时间: 20200224
作者: voyager
功能: 向数据库插入数据；从数据库获取数据
注意:
*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>

#define SQL_ALL_DATA "select * from data;"
#define SQL_INSERT_DATA "insert into data (name, lon, lat) values ('pc', 87.585, -42.156);"
#define SQL_LASTEST_DATA "select * from data order by no desc limit 1;"

void get_insert(MYSQL *mysql, const char *query);  // 插入数据
void get_lastest(MYSQL *mysql, const char *query);  // 获取最新一条数据

int main()
{
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *query;
	char command[100]={0};
	
	mysql_init(&mysql);
	// connect to Mysql
	if(!mysql_real_connect(&mysql, "localhost", "root", "1234", "place", 0, NULL, 0)) {
		printf("Failed to connect to Mysql!\n");
		return 0;
	}else {
		printf("Connected to Mysql successfully!\n");
	}
	
	sprintf(command, "insert into data (name, lon, lat) values ('%s', %f, %f);", "ppppp", 58.624, -8.25);
	puts(command);
	// 插入一条新数据
	query = SQL_INSERT_DATA;
	get_insert(&mysql, query);
	//获取最新一条数据
	query = SQL_LASTEST_DATA;
	get_lastest(&mysql, query);
	
	// 打印数据库返回结果
	res = mysql_store_result(&mysql);
	while(row = mysql_fetch_row(res)) {
		for(int i=0; i<mysql_num_fields(res); i++)
		{
			printf("%s\t", row[i]);
		}
		printf("\n");
	}
	
	mysql_close(&mysql);
	return 0;
}


void get_insert(MYSQL *mysql, const char *query){
	if(mysql_real_query(mysql, query, (unsigned int)strlen(query))) {
		printf("Insert failed!\n");
	}else {
		printf("%s\n", query);
	}
}


void get_lastest(MYSQL *mysql, const char *query){
	if(mysql_real_query(mysql, query, (unsigned int)strlen(query))) {
		printf("Query failed!\n");
	}else {
		printf("%s\n", query);
	}
}
