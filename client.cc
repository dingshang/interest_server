#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>


int main()
{

	int ret = 0;

	// create socket
	printf("create socket ...\n");
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		printf("create socket fail\n");
		return sockfd;
	}

	// connect server
	printf("connect ...\n");
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = inet_addr("192.168.122.1");

	ret = connect(
		sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

	if(ret == -1)
	{
		printf("connect fail\n");
		return ret;
	}


	int service = 1;
	// service echo
	if (service == 1)
	{
		// write msg
		char msg[] = "1hello";
		printf("write msg ...\n");
		ret = write(sockfd, msg, sizeof(msg));
		if (ret == -1)
		{
			printf("write fail\n");
			return ret;
		}

		// read msg
		#define BUFF_SIZE 1024
		char buff[BUFF_SIZE];
		ret = read(sockfd, buff, BUFF_SIZE);
		if (ret == -1)
		{
			printf("echo read fail\n");
			return ret;
		}
		printf("echo read:%s\n", buff);
	}

	// close socket
	printf("close socket ...\n");
	ret = close(sockfd);
	if (ret == -1)
	{
		printf("close socket fail\n");
		return ret;
	}

	return 0;
}
