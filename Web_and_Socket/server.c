/*****************************************************************************
名称: Web网页显示数据与发送控制指令——服务器端代码
时间: 20200214
作者: voyager
功能: 服务器不断接受客户机数据
      处理成shell命令
	  向Web网页文件写入新数据
	  接收Web网页的控制命令
	  发送给客户机控制命令
注意:
*****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>	//strlen
#include<sys/socket.h>  //socket
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//close


void socket_receive(int new_socket);
void shell_send(char *data_msg);  //推送数据到Web网页
int command_receive(FILE *read_file, char *command_msg);  //接收Web网页的控制命令


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
	puts("Bind done");
	// Listen
	listen(socket_desc , 3);
	// Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
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
	FILE *read_file;
	char data_msg[1000] = {0};
	char command_msg[1000] = {0};
	int flag = 0;  // 新command的标志
	char complete_msg[] = "complete";  // 通信结束标志
	char response_msg[] = "Server received client's data";  //自动回复

	// 清空data.txt与command.txt
	read_file = fopen("/var/www/html/sensor/data.txt", "w");
	if(read_file == NULL) {
		perror("打开data.txt失败");
		return;
	}
	fclose(read_file);
	read_file = fopen("/var/www/html/sensor/command.txt", "w");
	if(read_file == NULL) {
		perror("打开command.txt失败");
		return;
	}
	fclose(read_file);
	while(1){
		memset(data_msg ,0, sizeof(char)*1000);
		memset(command_msg ,0, sizeof(char)*1000);
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
		shell_send(data_msg);  // 推送数据
		flag = command_receive(read_file, command_msg);  // 获取命令
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
}


void shell_send(char *data_msg){
	char shell_string[1000] = {0};
	// 去掉字符串的换行符
	if(strlen(data_msg) > 1){
		if(data_msg[strlen(data_msg)-2] == '\r' || data_msg[strlen(data_msg)-2] == '\n'){
			data_msg[strlen(data_msg)-2] = 0;
		}
		else if(data_msg[strlen(data_msg)-1] == '\n' || data_msg[strlen(data_msg)-1] == '\r'){
			data_msg[strlen(data_msg)-1] = 0;
		}
	}
	else if(strlen(data_msg) == 1){
		if(data_msg[strlen(data_msg)-1] == '\n' || data_msg[strlen(data_msg)-1] == '\r'){
			data_msg[strlen(data_msg)-1] = 0;
		}
	}
	// 拼装并执行shell命令
	sprintf(shell_string, "echo \"%s\" > /var/www/html/sensor/data.txt", data_msg);
	printf("shell:%s\n", shell_string);
	system(shell_string);
}


int command_receive(FILE *read_file, char *command_msg){
	read_file = fopen("/var/www/html/sensor/command.txt", "r");
	if(fgets(command_msg, 1000, read_file) != NULL) {
		fclose(read_file);
		read_file = fopen("/var/www/html/sensor/command.txt", "w");  //清空command.txt
		fclose(read_file);
		return 1;
	}
	else{
		fclose(read_file);
		return 0;
	}
}
