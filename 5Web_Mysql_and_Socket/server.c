/*****************************************************************************
名称: Web Mysql and socket demo——服务器端代码
时间: 20200224
作者: voyager
功能: 服务器不断接受客户机数据
	  将数据存入Mysql数据库
	  读取Mysql数据库的控制命令
	  发送给客户机控制命令
注意:
*****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <mysql/mysql.h>

void socket_receive(int new_socket);
void data_send(MYSQL *mysql, char *data_msg);  //存储数据到Mysql数据库
int command_receive(MYSQL *mysql, char *command_msg);  //从Mysql数据库取出命令


int main(){
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;

	// 建立socket连接
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1){
		printf("Could not create socket");
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 3333 );
	// Bind
	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		puts("Bind failed");
		return 1;
	}
	// Listen
	listen(socket_desc , 3);
	// Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);
	if(new_socket < 0){
		perror("Accept failed");
		return 1;
	}
	puts("Connection accepted");

	socket_receive(new_socket);

	// 关闭socket连接
	close(socket_desc);
	return 0;
}


void socket_receive(int new_socket){
	char complete_msg[] = "complete";  // 通信结束标志
	char response_msg[] = "Server received client's data";  // 自动回复
	char data_msg[1000] = {0};
	char command_msg[1000] = {0};
	int flag = 0;  // 新command的标志
	MYSQL mysql;

	// 连接Mysql数据库
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql, "localhost", "root", "1234", "place", 0, NULL, 0)) {
		printf("Failed to connect to Mysql!\n");
		return;
	}else {
		printf("Connected to Mysql successfully!\n");
	}

	while(1){
		memset(data_msg, 0, sizeof(char)*1000);
		memset(command_msg, 0, sizeof(char)*1000);
		flag = 0;
		// 接收原始数据
		if(recv(new_socket, data_msg, 1000, 0) < 0){
			puts("Recv failed");
			continue;
		}
		puts("\nReceived:");
		puts(data_msg);
		if(strcmp(data_msg, complete_msg) == 0){
			break;
		}
		// 服务器处理过程
		data_send(&mysql, data_msg);  // 推送数据
		flag = command_receive(&mysql, command_msg);  // 获取命令
		// 发送回复信息
		if(flag){
			if(send(new_socket, command_msg, strlen(command_msg), 0) < 0){
				puts("Send failed!");
			}
			puts("Sent:");
			puts(command_msg);
		}
		else{
			if(send(new_socket, response_msg, strlen(response_msg), 0) < 0){
				puts("Send failed!");
			}
			puts("Sent:");
			puts(response_msg);
		}
		sleep(10);  //控制传输速率
	}
	// 关闭数据库
	mysql_close(&mysql);
}


void data_send(MYSQL *mysql, char *data_msg){
	char data[1000] = {0};
	char lon[20] = {0};
	char lat[20] = {0};
	sscanf(data_msg,"%[^','],%[^','],", lon, lat);
	sprintf(data, "insert into data (name, lon, lat) values ('%s', %s, %s);", "client1", lon, lat);
	if(mysql_real_query(mysql, data, (unsigned int)strlen(data))) {
		printf("Insert failed!\n");
	}else {
		printf("%s\n", data);
	}
}


int command_receive(MYSQL *mysql, char *command_msg){
	MYSQL_RES *res;
	MYSQL_ROW row;
	char *sql1 = "select no from command order by no desc limit 1;";
	char sql2[100] = {0};
	static int current_no = 0; // 当前已处理的记录的no值
	int latest_no;  // 数据库中最新的记录的no值
	
	// 获取最新记录的no字段的值
	if(mysql_real_query(mysql, sql1, (unsigned int)strlen(sql1))) {
		printf("sql failed!\n");
	}
	res = mysql_store_result(mysql);
	row = mysql_fetch_row(res);
	latest_no = atoi(row[0]);
	// 判断是否有新指令
	if(current_no < latest_no){
		sprintf(sql2,"select name,action from command where no=%d;", ++current_no);
		if(mysql_real_query(mysql, sql2, (unsigned int)strlen(sql2))) {
			printf("sql failed!\n");
		}
		res = mysql_store_result(mysql);
		row = mysql_fetch_row(res);
		sprintf(command_msg, "name:%s,action:%s", row[0], row[1]);
		return 1;
	}
	
	return 0;
}
