#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<fcntl.h>
#define BUF_SIZE 1024

// %h -->host %E --> Path  %p --> pid   %s signal  
int main(int argc, char ** argv)
{
	pid_t pid;
	int sfd;
	struct sockaddr_in client_addr;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd == -1)
	{
		perror("Failed to create socket: ");
		exit(1);
	}
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(60014);
	client_addr.sin_addr.s_addr = inet_addr("192.168.1.139");
	//client_addr.sin_addr.s_addr = inet_addr("10.0.2.15");
	int num;
	int ret;
	ret = connect(sfd,(struct sockaddr *)&client_addr,sizeof(struct sockaddr));
	if(ret == -1)
	{
		perror("Client connect is failed: ");
		exit(1);
	}
	char hostinfo[100],buf[100];
	for(int i=0;i<4;i++) puts(argv[i]);
	sprintf(hostinfo,"%s_%s_%s\0",argv[1],argv[2],argv[3]);
	ret= send (sfd,hostinfo,25,0);
	if(ret ==-1 )
	{
		perror("send_info");
		exit(1);
	}
	while((ret = read(0,buf,100))>0)
	{
		if(ret ==-1)
		{
			perror("read");
			exit(1);
		}
		ret=send(sfd,buf,100,0);
		if(ret ==-1 )
		{
			perror("send_core");
			exit(1);
		}
	}
}
