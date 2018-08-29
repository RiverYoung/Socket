#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

int main(int argc,char *argv[])
{
    int sockfd,numbytes;
    char buf[BUFSIZ];
    struct sockaddr_in their_addr;

    while((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1);
    printf("We get the sockfd!\n");
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(8000);
    their_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    bzero(&(their_addr.sin_zero), 8);
	char filePath[128];
	memset(filePath, 0, 128);

	
    while(connect(sockfd,(struct sockaddr*)&their_addr,sizeof(struct sockaddr)) == -1);
    printf("Get the Server!\n");
    numbytes = recv(sockfd, buf, BUFSIZ,0);
    buf[numbytes]='\0';  
    printf("Server filename = %s\n",buf); //服务端传过来的文件名
    
    strcat(filePath,"/data1/yangjianghe/work/learnSocket/client/");
	strcat(filePath, buf);
	printf("filePath: %s\n",filePath);
    
    while(1)
    {
        printf("Entersome thing:");
        scanf("%s",buf);
        numbytes = send(sockfd, buf, sizeof(buf), 0);
        
		memset(buf,0,strlen(buf));
		numbytes=recv(sockfd,buf,BUFSIZ,0);  
        buf[numbytes]='\0'; 
		printf("fileContent: %s\n",buf); 
		printf("numbytes = %d\n",numbytes);

		int fp = open(filePath,O_CREAT|O_RDWR,0777);
		int res = 0;
		if ((res = write(fp, buf, numbytes)) != numbytes)
	    {
	        printf("Error writing to the file.\n");
	        exit(1);
	    }
   		printf("Wrote %d bytes to the file.\n", res);
	    close(fp); 
    }
	
    close(sockfd);
    return 0;
}
