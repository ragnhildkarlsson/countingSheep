/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

static const int8_t SHEEP_BASEDATA = 1;
static const int8_t MSG_ACK = 100;

struct msghead{
	int8_t	msg;
	int16_t	nof_bytes;
};

void error(const char *msg){
    perror(msg);
	exit(EXIT_FAILURE);
};

void doprocessing (int sock, const char* yourip);

int main( int argc, char *argv[] ){
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n,pid;
    char yourip[INET_ADDRSTRLEN];

    signal(SIGCHLD,SIG_IGN);
    /* First call to socket() function */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        error("ERROR opening socket");
    /* Initialize socket structure */
    bzero((void *) &serv_addr, sizeof(serv_addr));
    portno = 4711;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* Now bind the host address using bind() call.*/
    int opt = 1;
    setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const char *)&opt,sizeof(int));
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
         error("ERROR on binding");
    /* Now start listening for the clients, here 
     * process will go in sleep mode and will wait 
     * for the incoming connection
     */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while (1) 
    {
        if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0)
            error("ERROR on accept");
        inet_ntop(AF_INET, &(cli_addr.sin_addr), yourip, INET_ADDRSTRLEN);
        /* Create child process */
        if ((pid = fork()) < 0)
            error("ERROR on fork");
        if (pid == 0){
            /* This is the client process */
            close(sockfd);
			doprocessing(newsockfd,yourip);
			return EXIT_SUCCESS;
        } else {
            close(newsockfd);
        }
    }
}

void doprocessing (int sock, const char* yourip){
    int n;
    struct msghead head;
    char *buf,*tofree, success[8] = "SUCCESS";
    head.msg = 0;
    head.nof_bytes = 0;

    if((n = read(sock, &head, sizeof(head))) < 0)
        error("ERROR reading from socket");
    if( (tofree = buf = (char*)malloc(head.nof_bytes)) == NULL )
        error("ERROR malloc for payload");
    if((n = write(sock,&MSG_ACK,sizeof(MSG_ACK))) < 0)
        error("ERROR writng to socket");
    bzero(buf,sizeof(buf));
    if((n = read(sock, buf, head.nof_bytes)) < 0)
        error("ERROR reading payload from socket");
    if((n = write(sock,success,sizeof(success))) < 0)
        error("ERROR writng to socket");
    fprintf(stdout, "Head:\n\t%d\n\t%d\nPayload:\n\t'%s'\n", head.msg,head.nof_bytes,buf);
    free(tofree);
}
