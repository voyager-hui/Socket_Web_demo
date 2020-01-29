#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write

int main(int argc , char *argv[])
{
	int socket_desc, new_socket, numbytes, c;
	struct sockaddr_in server , client;
	char *message;
	char buff[BUFSIZ];
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 3333 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		return 1;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	puts("Connection accepted");
	while((numbytes = recv(new_socket, buff, BUFSIZ, 0)) > 0)
	{
		//Print message from client
		buff[numbytes] = '\0';
		puts("Received:");
		printf("%s\n",buff);
		//Reply to the client
		message = "Hello Client , I have received your connection. But I have to go now, bye\n";
		puts("Sent:");
		puts(message);
		write(new_socket , message , strlen(message));
	}
	
	if (new_socket<0)
	{
		perror("accept failed");
		return 1;
	}
		
	//Close socket
	close(socket_desc);
	
	return 0;
}