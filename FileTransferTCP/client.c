/*
    Name               : Jap Purohit
    Enrollment Number  : AU1940109
	EmailID 		   : jap.p@ahduni.edu.in
    Purpose of Program : Create simple client side program. Which binds with any server binded and have file transfer.
*/
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define	BUFLEN 500
// structure for receiving file data
struct PDU {
		char type;
		int length;
		char data[BUFLEN];
		char message[BUFLEN];
} rpdu;

int fileFunction(int sockFD){
	char buff[MAX];
	char message[MAX];
	int n;
	//continous connection between server and client
	for(;;){
		bzero(buff,sizeof(buff));
		fprintf(stderr,"Enter the string : ");
		n = 0;
		while((buff[n++] = getchar())!= '\n');
		//if client writes bye then close the program
		if(strncmp(buff,"Bye",3)==0){
			write(sockFD,buff,sizeof(buff));	
			break;
		}
		//sending filename to server
		write(sockFD,buff,sizeof(buff));	
		//bzero(message,sizeof(message));
		//read(sockFD, message, sizeof(message));
		//read message from server
		read(sockFD, (char *)&rpdu, sizeof(rpdu));
		//copying message received from server to message array and then printing
		strcpy(message,rpdu.message);	
		fprintf(stderr,"\t\tFrom Server : %s\n", message);
		//condition to check if message is ok from server
		//then do process for file receiving from server
		if(strcmp(message,"OK")==0){
			// updating file name because last character is '\n' in array
			buff[n-1] = '\0';			
			if (rpdu.type == 'F') {
				FILE *fp = fopen(buff,"w");
				fwrite(rpdu.data, sizeof(char), rpdu.length, fp); // write data to file
			//	fprintf(stderr,"Data is : %s\n",rpdu.data);
				while (rpdu.length == BUFLEN) { // if there is more data to write
					read(sockFD, (char *)&rpdu, sizeof(rpdu));
					fwrite(rpdu.data, sizeof(char), rpdu.length, fp);
				//	fprintf(stderr,"Data is : %s\n",rpdu.data);
				}
				// close file
				fclose(fp);	
			}
		}
	}
	return 0;
}
int main(){
    int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
    char message[80];
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else{
		printf("connected to the server..\n");
        bzero(message, sizeof(message));
		//reading message from server when first time connection is done
		read(sockfd, message, sizeof(message));
		printf("From Server : %s\n", message);
    }	
	//calling function for communication
	fileFunction(sockfd);
	//closing connection
	close(sockfd);
    return 0;
}
