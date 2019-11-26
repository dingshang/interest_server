#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

int main()
{

	// create socket
	printf("create socket ...\n");

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("create socket fail\n");
		return sockfd;
	}	

	// bind socket
	printf("bind socket ...\n");
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr("192.168.122.1");

	int ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
	if (ret == -1)
	{
		printf("bind socket fail\n");
		return ret;
	}

	// listen socket
	printf("listen socket ...\n");
	int backlog = 50;
	ret = listen(sockfd, backlog);
	if (ret == -1)
	{
		printf("listen socket fail\n");
		return ret;
	}

	// accept
	int client_fd = 0;
	struct sockaddr_in client_addr;
	socklen_t addr_len;
	client_fd = 0;
	while (client_fd != -1)
	{
		printf("accept socket ...\n");
		client_fd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_len);
		printf("client fd:%i;", client_fd);
		printf(" ip:%s;", inet_ntoa(client_addr.sin_addr));
		printf(" port:%i\n", client_addr.sin_port);
		#define BUFF_SIZE 1024
		char buff[BUFF_SIZE];
		ret = read(client_fd, buff, BUFF_SIZE);
		if (ret >= 0)
		{
			printf("read msg, size[%i]:[%s]\n", ret, buff);
		}
	}

	return 0;
}

