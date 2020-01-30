#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>  //close

//文件相关
FILE *r_file, *w_file;
char buf[50][100];
int i=0, j=0;

void read_data();
void save_data();


int main(int argc, char *argv[])
{
	//Socket相关
	int socket_desc;
	struct sockaddr_in server;
	char *message, server_reply[2000];
	int k;

	//读取原始数据
	read_data();

	//Create socket
	socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	server.sin_addr.s_addr = inet_addr("49.232.163.31");
	server.sin_family = AF_INET;
	server.sin_port = htons(3333);

	//Connect to remote server
	if(connect(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	puts("Connected\n");
	
	//Send data
	printf("\nSending data to the server");
	for(k=0; k<i; k++){
		if(send(socket_desc, buf[k], strlen(buf[k]), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}
		puts("Sent:");
		puts(buf[k]);
		
		if(recv(socket_desc, server_reply, 2000, 0) < 0)
		{
			puts("recv failed");
		}
		puts("Received:");
		puts(server_reply);
		
		sleep(2);
	}
	
	//Receive result
	printf("\nReceiving data from the server");
	while(1){
		if(recv(socket_desc, msg, 2000, 0) < 0)
		{
			puts("recv failed");
		}
		puts("Received:");
		puts(msg);
		if(msg == 'q'){
			break;
		}
		
	}
	

	//Close socket
	close(socket_desc);

	//保存结果
	save_data();

	return 0;
}


void read_data()
{
	printf("Read......\n");
	r_file =fopen("data.csv", "r");
	if(!r_file){
		printf("Cannot read\n");
		return 1;
	}
	while(fgets(buf[i], 100, r_file) != NULL){
		printf("%s", buf[i]);
		i++;
	}
	fclose(r_file);
}


void save_data()
{
	printf("Writing......\n");
	w_file =fopen("result.csv", "w+");
	if(!w_file){
		printf("Cannot write\n");
		return 1;
	}
	while(j < i){
		printf("%s", buf[j]);
		fprintf(w_file, buf[j]);
		j++;
	}
	fclose(w_file);
}