/*****************************************************************************
名称: Web网页显示传感器数据——服务器端代码
时间: 20200213
作者: voyager
功能: 服务器不断接受客户机数据
      处理成shell命令
	  向Web网页文件写入新数据
注意:
*****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>	//strlen
#include<sys/socket.h>  //socket
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//close


void socket_receive(int new_socket);  //接收处理结果


int main(){
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	
	//建立socket连接
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_desc == -1){
		printf("Could not create socket");
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 3333 );
	//Bind
	if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		puts("Bind failed");
		return 1;
	}
	puts("Bind done");
	//Listen
	listen(socket_desc , 3);
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	if(new_socket < 0){
		perror("Accept failed");
		return 1;
	}
	puts("Connection accepted");
	
	puts("Receiving data from the client......");
	socket_receive(new_socket);

	//关闭socket连接
	close(socket_desc);
	return 0;
}

void shell_send(char *data_msg){
	char command[2000] = {0};
	//去掉字符串中的换行符
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
	//拼装shell命令
	sprintf(command, "echo \"%s\" > /var/www/html/sensor/data.txt", data_msg);
	//发送shell命令，间隔10秒
	printf("command:%s\n", command);
	system(command);
	sleep(10);
}

void socket_receive(int new_socket){
	char data_msg[1000] = {0};
	char complete_msg[] = "complete";
	char response_msg[] = "Server received client's data";
	while(1){
		//接收数组清零
		memset(data_msg ,0, sizeof(char)*1000);
		//接收原始数据
		if(recv(new_socket, data_msg, 1000, 0) < 0){
			puts("Recv failed");
			continue;
		}
		puts("\nReceived:");
		puts(data_msg);
		if(strcmp(data_msg, complete_msg) == 0){
			break;
		}
		//将新数据写入web的文件
		shell_send(data_msg);
		//发送回复信息
		if(send(new_socket, response_msg, strlen(response_msg), 0) < 0){
			puts("Send failed!");
		}
		puts("Sent:");
		puts(response_msg);
	}
}
