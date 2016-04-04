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



#define PORT	5555
#define BACKLOG	100

main()
{
	int sockfd,new_fd;
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;
	unsigned int sin_size;
	int ret;
	const char *buf;

	perror("start socket");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket error:%d", errno);
		perror("socket");
		exit(1);
	}

	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(PORT);
	s_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(s_addr.sin_zero), 8);

	perror("start bind");
	ret = bind(sockfd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr));
	if (ret == -1) {
		printf("bind err:%d", errno);
		perror("bing");
		exit(1);
	}

	perror("start listen");
	ret = listen(sockfd, BACKLOG);
	if (ret == -1) {
		printf("listen err:%d", errno);
		perror("listen");
		exit(1);
	}

	while(1) {
		printf("my pid=%d, ppid=%d\n", getpid(), getppid());
		sin_size = sizeof(struct sockaddr_in);
		perror("start accept");
		new_fd = accept(sockfd, (struct sockaddr *)&c_addr, &sin_size);
		if (new_fd == -1) {
			printf("accept err:%d", errno);
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", inet_ntoa(c_addr.sin_addr));
		if (!fork()) {
			printf("==>my pid=%d, ppid=%d\n", getpid(), getppid());
			perror("==>start send, sleep 5s");
			sleep(5);
			buf = "Hello world!!!\n";
			if (send(new_fd, buf, strlen(buf), 0) == -1) {
				printf("==>send err! %d", errno);
				perror("==>send");
			}
			close(new_fd);
			perror("==>exit child process, sleep 3S");
			sleep(3);
			perror("==>exit child process");
			exit(0);
		}

		close(new_fd);
		printf("wait child exit! my pid=%d, ppid=%d\n", getpid(), getppid());
		//waitpid(-1, NULL, WNOHANG);
		waitpid(-1, NULL, 0);
		perror("child process exit");
	}
}
