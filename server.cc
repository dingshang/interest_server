#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define BUFF_SIZE 1024

int service()
{

	char client_box[BUFF_SIZE];

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
		char buff[BUFF_SIZE];
		ret = read(client_fd, buff, BUFF_SIZE);
		if (ret >= 0)
		{
			printf("read msg, size[%i]:[%s]\n", ret, buff);

			// services
			if (buff[0] == '1')
			{
				printf("client choose service 1: echo service\n");
				char string[ret-1];
				for (int i=1; i<ret; i++)
				{
					string[i-1] = buff[i];
				}
				ret = write(client_fd, string, ret-1);
				if (ret < 0)
				{
					printf("echo service write fail\n");
					return ret;
				}
			}
			else if (buff[0] == '2')
			{
				printf("client choose service 2: put service\n");
				for (int i=1; i<ret; i++)
				{
					client_box[i-1] = buff[i];
				}
				printf("put words into client box:%s", client_box);
			}
			else if (buff[0] == '3')
			{
				printf("client choose service 3: get service\n");
				ret = write(client_fd, client_box, BUFF_SIZE);
				if (ret < 0)
				{
					printf("get service write fail\n");
					return ret;
				}
		
			}
		}
		printf("\n");
	}

	return 0;
}

int main()
{
	return service();
}

