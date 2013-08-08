#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

static const int8_t SHEEP_BASEDATA = 1;
static const int8_t MSG_ACK = 100;

struct msghead{
	int8_t	msg;
	int16_t	nof_bytes;
};

void error(const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct msghead head;
    int8_t hanshake;
    char buffer[256];
    
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(EXIT_FAILURE);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    if ((server = gethostbyname(argv[1])) == NULL) {
        fprintf(stderr,"ERROR, no such host: %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    printf("Please enter the message: ");
    bzero(buffer,256);
    head.msg = SHEEP_BASEDATA;
    fgets(buffer,255,stdin);
	/* remove newline */
	buffer[strcspn(buffer, "\n")] = '\0';
    head.nof_bytes = strlen(buffer)+1;
    if ((n = write(sockfd,&head,sizeof(head))) < 0) 
         error("ERROR writing to socket");
    if ((n = read(sockfd,&hanshake,sizeof(hanshake))) < 0) 
         error("ERROR reading from socket");
    if (hanshake != MSG_ACK)
         error("ERROR recived no MSG_ACK");
    if ((n = write(sockfd,buffer,strlen(buffer))) < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    if ((read(sockfd,buffer,255)) < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    close(sockfd);
    return 0;
}
