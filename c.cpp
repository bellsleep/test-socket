
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>



#define PORT	5555
#define MAXDATASIZE	100


int main(int argc, char *argv[]) {
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in s_addr;
	int ret;

	if (argc != 2) {
		fprintf(stderr, "usage: client hostname");
		exit(1);
	}

	he = gethostbyname(argv[1]);
	if (he == NULL) {
		herror("gethostbyname");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(1);
	}

	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
	s_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(s_addr.sin_zero), 8);
	
	ret = connect(sockfd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr));
	if (ret == -1) {
		perror("connect");
		exit(1);
	}

	numbytes = recv(sockfd, buf, MAXDATASIZE, 0);
	if (numbytes == -1) {
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';
	printf("Received:%s\n", buf);
	close(sockfd);

	return 0;
}
