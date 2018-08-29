#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd, new_fd, struct_len, numbytes,i;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char buff[BUFSIZ];
	char fileName[32] = "socketTest.txt";
	char filePath[128];
	memset(buff, 0, BUFSIZ);
	memset(filePath, 0, 128);
		
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero), 8);
	struct_len = sizeof(struct sockaddr_in);

	fd = socket(AF_INET, SOCK_STREAM, 0);
	while(bind(fd, (struct sockaddr *)&server_addr, struct_len) == -1)
	{
		//printf("Binding...\n");
	}
	printf("Bind Success!\n");
	while(listen(fd, 10) == -1);
	printf("Listening...\n");
	printf("Ready for Accept,Waitting...\n\n");

	new_fd = accept(fd, (struct sockaddr *)&client_addr, (socklen_t *)&struct_len);
	printf("Connect client success!\n");
	numbytes = send(new_fd,fileName,strlen(fileName),0);  //发送文件名到客户端
	strcat(filePath,"/data1/yangjianghe/work/learnSocket/server/");
	strcat(filePath, fileName);

	char buffer[BUFSIZ];
	memset(buffer, 0, BUFSIZ);

	while((numbytes = recv(new_fd, buff, BUFSIZ, 0)) > 0)
	{
		buff[numbytes] = '\0';
		printf("Client chosse:%s\n",buff);  //buff为客户端输入的选项
		
		if (!strcmp(buff, "yes"))
		{
			int fd_tmp = open(filePath,O_RDONLY);
			int size = read(fd_tmp,buffer,sizeof(buffer));
			printf("fileSize = %d\n",size); 
			if(send(new_fd,buffer,size,0)<0)
			{  
				perror("write");  													
				return 1;
			}  
		}
		else
		{
			char msgToClient[64] = "Please input yes!!!\n";
			send(new_fd,msgToClient,strlen(msgToClient),0); //告知客户端重传
			printf("Please input yes!!!\n");
		}
	}
	
	close(new_fd);
	return 0;
}

