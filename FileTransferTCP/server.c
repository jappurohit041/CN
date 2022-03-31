/*
    Name               : Jap Purohit
    Enrollment Number  : AU1940109
	EmailID 		   : jap.p@ahduni.edu.in
    Purpose of Program : Create simple server side program. Which binds with any client and have file transfer.
*/
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define BUFLEN 500

// structure for sending file data
struct PDU {
		char type;
		int length;
		char data[BUFLEN];
		char message[BUFLEN];

} spdu;

int fileFunction(int connFD){
	char buff[MAX];
	int n;
	//continous connection between server and client
	for(;;){
		bzero(buff,MAX);
		read(connFD,buff,sizeof(buff));
		int i=0;
		// updating file name because last character is '\n' in array
		while(buff[i]!='\0'){
			if(buff[i]=='\n'){
				buff[i]='\0';
				break;
			}
			i++;
		}
		fprintf(stderr,"\t\tFrom client : %s",buff);
		// close connection if client sends "Bye" message
		if(strncmp(buff,"Bye",3)==0){
			break;
		}
		//fprintf(stderr,"\nLength of string : %ld",strlen(buff));
		FILE *fp=NULL;
		fp = fopen(buff,"r");
		//check if file is present of not
		// if not present then sending message file not found
		if(fp==NULL){
			fprintf(stderr,"\nServer : File not found\n");		
  			char message[] = "File not found";
			strcpy(spdu.message,message);
			write(connFD, (char *)&spdu, sizeof(spdu));
        	//write(connFD, message, sizeof(message));
			//fclose(fp);
		}
		// if file is present then start with process of sending file
		else{
			fprintf(stderr,"\nServer : File found\n");	
			char message[] = "OK";
			strcpy(spdu.message,message);
     	//  write(connFD, message, sizeof(message));
		//  to get the statistics for file and file size
			struct stat fstat;
			spdu.type = 'F';		
			stat(buff, &fstat);
			int bytes_to_read = fstat.st_size;
			// starting with file sending process
			while(bytes_to_read > 0) {
				spdu.length = fread(spdu.data, sizeof(char), BUFLEN, fp);
				bytes_to_read -= spdu.length;
				spdu.type = 'F';
				//	fprintf(stderr,"Data is : %s\n",spdu.data);
				write(connFD, (char *)&spdu, sizeof(spdu));
			}
			printf("Transfer of \"%s\" sucessful.\n", buff);
			fclose(fp);	
		}
	}
}
int main(){
    int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server accept failed...\n");
		exit(0);
	}
	else{
		//if connection is done then sending message to client
        printf("server accept the client...\n");
        char message[] = "Hello Client";
        write(connfd, message, sizeof(message));
    }
	fileFunction(connfd);
	close(connfd);	
    return 0;
}
