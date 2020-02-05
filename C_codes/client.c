/*****************************************************************************
名称: 客户端Socket通信代码
时间: 20200205
作者: voyager
功能: 将原始数据发送给服务器
      接收服务器处理结果
注意:
*****************************************************************************/

#include<stdio.h>
#include<string.h>  //strlen
#include<sys/socket.h>  //socket
#include<arpa/inet.h>  //inet_addr
#include<unistd.h>  //close


int read_data(FILE *read_file, char buf[100][1000]);  //读取文本
void socket_send(int socket_desc, char buf[100][1000], int lines);  //发送原始数据
int socket_receive(int socket_desc, char buf[100][1000]);  //接收处理结果
void save_data(FILE *write_file, char buf[100][1000], int lines);  //保存文本


int main(){
	int socket_desc;
	struct sockaddr_in server;
	FILE *read_file, *write_file;
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

	puts("Sending data to the server......");
	socket_send(socket_desc, buf, lines);
	puts("Receiving result from the server......");
	lines = socket_receive(socket_desc, buf);

	//关闭socket连接
	close(socket_desc);
	
	puts("\nSaving final result......");
	save_data(write_file, buf, lines);

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
	char msg[100];
	char complete_msg[] = "complete";
	//发送原始数据
	for(i=0; i<lines; i++){
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
	//发送结束标志
	if(send(socket_desc, complete_msg, strlen(complete_msg), 0) < 0){
		puts("Send failed!");
	}
	else{
		puts("\nSend complete!");
	}
}

int socket_receive(int socket_desc, char buf[100][1000]){
	int lines = 0;
	char complete_msg[] = "complete";
	char response_msg[] = "Client received server's data";
	while(1){
		//接收处理结果
		if(recv(socket_desc, buf[lines], 1000, 0) < 0){
			puts("recv failed");
			continue;
		}
		puts("\nReceived:");
		puts(buf[lines]);
		if(strcmp(buf[lines], complete_msg) == 0){
			break;
		}
		lines++;
		//发送回复信息
		if(send(socket_desc, response_msg, strlen(response_msg), 0) < 0){
			puts("Send failed!");
		}
		puts("Sent:");
		puts(response_msg);
	}
	return lines;
}


void save_data(FILE *write_file, char buf[100][1000], int lines){
	int count = 0;
	write_file = fopen("result.csv", "w+");
	if(!write_file){
		puts("Cannot write");
	}
	while(count < lines){
		printf("Write: %s", buf[count]);
		fprintf(write_file, buf[count]);
		count++;
	}
	fclose(write_file);
}
