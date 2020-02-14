/*****************************************************************************
名称: Web网页显示传感器数据——客户端代码
时间: 20200213
作者: voyager
功能: 不断将传感器数据发送给服务器
注意:
*****************************************************************************/

#include<stdio.h>
#include<string.h>  //strlen
#include<sys/socket.h>  //socket
#include<arpa/inet.h>  //inet_addr
#include<unistd.h>  //close


int read_data(FILE *read_file, char buf[100][1000]);  //读取文本
void socket_send(int socket_desc, char buf[100][1000], int lines);  //发送原始数据


int main(){
	int socket_desc;
	struct sockaddr_in server;
	FILE *read_file;
	char buf[100][1000];  //注意传输数据少于100行1000列
	int lines;

	puts("\nReading origin data......");
	lines = read_data(read_file, buf);
	
	//建立socket连接
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1){
		puts("\nCould not create socket");
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("49.232.163.31");
	server.sin_port = htons(3333);
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0){
		puts("\nConnect error");
		return 1;
	}
	puts("\nConnected to the server!");
	
	//不断向服务器发送数据
	puts("Sending data to the server......");
	socket_send(socket_desc, buf, lines);

	//关闭socket连接
	close(socket_desc);

	return 0;
}


int read_data(FILE *read_file, char buf[100][1000]){
	int count = 0;
	read_file = fopen("data.csv", "r");
	if(!read_file){
		puts("Cannot read");
	}
	while(fgets(buf[count], 100, read_file) != NULL){
		printf("Read: %s", buf[count]);
		count++;
	}
	fclose(read_file);
	return count;
}


void socket_send(int socket_desc, char buf[100][1000], int lines){
	int i;
	char msg[1000];
	char complete_msg[] = "complete";
	//不断发送原始数据
	for(i=0; ; i++, i%=lines){
		//清空接收数组
		memset(msg ,0, sizeof(char)*1000);
		if(send(socket_desc, buf[i], strlen(buf[i]), 0) < 0){
			puts("Send failed!");
		}
		puts("\nSent:");
		puts(buf[i]);
		if(recv(socket_desc, msg, 1000, 0) < 0){
			puts("Recv failed!");
		}
		puts("Received:");
		puts(msg);
	}
	//发送结束标志(不会执行到)
	if(send(socket_desc, complete_msg, strlen(complete_msg), 0) < 0){
		puts("Send failed!");
	}
	else{
		puts("\nSend complete!");
	}
}
