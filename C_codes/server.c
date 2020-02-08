/*****************************************************************************
名称: 服务器Socket通信代码
时间: 20200205
作者: voyager
功能: 服务器接受客户机数据
      通过system()调起处理程序(Python处理程序或者C处理程序)
	  将处理结果返回给客户机
注意:
*****************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>	//strlen
#include<sys/socket.h>  //socket
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//close


int socket_receive(int new_socket, char buf[100][1000]);  //接收处理结果
void save_data(FILE *write_file, char buf[100][1000], int lines);  //保存文本
int read_data(FILE *read_file, char buf[100][1000]);  //读取文本
void socket_send(int new_socket, char buf[100][1000], int lines);  //发送原始数据


int main(){
	int socket_desc, new_socket, c;
	struct sockaddr_in server, client;
	FILE *read_file, *write_file;
	char buf[100][1000];  //注意传输数据少于100行1000列
	int lines;
	
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
	lines = socket_receive(new_socket, buf);
	puts("\nSaving origin data......");
	save_data(write_file, buf, lines);
	// //调用Python服务器处理程序处理数据
	// puts("\n\n\nProcessing......\n\n\n");
	// system("python server_process.py");
	//调用C服务器处理程序处理数据
	puts("\n\n\nProcessing......\n\n\n");
	system("./server_process");
	
	puts("\nReading result data......");
	lines = read_data(read_file, buf);
	puts("Sending result to the client......");
	socket_send(new_socket, buf, lines);
	
	//关闭socket连接
	close(socket_desc);
	return 0;
}


int socket_receive(int new_socket, char buf[100][1000]){
	int lines = 0;
	char complete_msg[] = "complete";
	char response_msg[] = "Server received client's data";
	while(1){
		//接收原始数据
		if(recv(new_socket, buf[lines], 1000, 0) < 0){
			puts("Recv failed");
			continue;
		}
		puts("\nReceived:");
		puts(buf[lines]);
		if(strcmp(buf[lines], complete_msg) == 0){
			break;
		}
		lines++;
		//发送回复信息
		if(send(new_socket, response_msg, strlen(response_msg), 0) < 0){
			puts("Send failed!");
		}
		puts("Sent:");
		puts(response_msg);
	}
	return lines;
}


void save_data(FILE *write_file, char buf[100][1000], int lines){
	int count = 0;
	write_file = fopen("server_data.csv", "w+");
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


int read_data(FILE *read_file, char buf[100][1000]){
	int count = 0;
	read_file = fopen("server_result.csv", "r");
	if(!read_file){
		puts("Cannot read");
	}
	while(fgets(buf[count], 1000, read_file) != NULL){
		printf("Read: %s", buf[count]);
		count++;
	}
	fclose(read_file);
	return count;
}


void socket_send(int new_socket, char buf[100][1000], int lines){
	int i;
	char msg[100];
	char complete_msg[] = "complete";
	//发送处理结果
	for(i=0; i<lines; i++){
		if(send(new_socket, buf[i], strlen(buf[i]), 0) < 0){
			puts("Send failed!");
		}
		puts("\nSent:");
		puts(buf[i]);
		if(recv(new_socket, msg, 1000, 0) < 0){
			puts("Recv failed!");
		}
		puts("Received:");
		puts(msg);
	}
	//发送结束标志
	if(send(new_socket, complete_msg, strlen(complete_msg), 0) < 0){
		puts("Send failed!");
	}
	else{
		puts("\nSend complete!");
	}
}
