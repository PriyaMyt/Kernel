#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include<string.h>
#include<ctype.h>
#include<fcntl.h>
#include<time.h>
#include<errno.h>
int main()
{
        int sfd;
        pid_t pid;
        sfd = socket(AF_INET,SOCK_STREAM,0);
        if(sfd == -1)
        {
                perror("error");
                exit(1);
        }
        struct sockaddr_in server_addr,client_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(60014);
        server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
        int ret,count;
        ret = bind(sfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr));
        if(ret == -1)
        {
                perror("bind : ");
                exit(1);
}
        ret = listen(sfd,10);
        if(ret == -1)
        {
                perror("listen: ");
                exit(1);
        }
        char buf[30];
        socklen_t client_len = sizeof(client_addr);
        int n_sfd = accept(sfd,(struct sockaddr *)&client_addr,&client_len);
        if(n_sfd == -1)
        {
                perror("accept: ");
                exit(1);
        }
        pid = fork();
        if(pid == 0)
        {
                char datetime[17];
                time_t t = time(NULL);
                struct tm *tm_info = localtime(&t);
                strftime(datetime, sizeof(datetime), "%d%m%Y_%H:%M", tm_info);
                printf("Formatted datetime: %s\n", datetime);
                char hostinfo[500];
                ret = recv(n_sfd,hostinfo,sizeof(hostinfo),0);
                printf("%s\n",hostinfo);
                if(ret == -1)
                {
                        perror("server receive: ");
                        exit(1);
                }
//char name[] = "core_remote_";
                int fd1,count1;
                char file_name[100];
                sprintf(file_name,"/tmp/core_remote_%s_%s",datetime,hostinfo);
                printf("%s\n",file_name);
                fd1 = open(file_name,O_CREAT|O_RDWR |O_EXCL,0664);
                if(fd1 == -1)
                {
                        perror("Error opening in created file: ");
                        printf("%d",errno);
                        exit(1);
                }
                while((count = recv(n_sfd,buf,sizeof(buf),0)) > 0)
                {
                        count1 = write(fd1,buf,count);
                        if(count != count1)
                        {
                                printf("Copy bytes are different\n");
                        }
                }
        }
        else if(pid == -1)
        {
                perror("error: ");
                exit(1);
        }
}
