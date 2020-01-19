#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

#define BUFF_SIZE 1024
#define DEBUG 1

int debug_printf(const char * format, ...)
{
	if (DEBUG)
	{
		printf("[debug] ");
		printf(format);
	}
}

int client_proxy(int service, char * buff, int size)
{

	int ret = 0;

	// create socket
	debug_printf("create socket ...\n");
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		debug_printf("create socket fail\n");
		return sockfd;
	}

	// connect server
	debug_printf("connect ...\n");
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000);
	server_addr.sin_addr.s_addr = inet_addr("192.168.122.1");

	ret = connect(
		sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr));

	if(ret == -1)
	{
		debug_printf("connect fail\n");
		return ret;
	}


	// service echo
	if (service == 1)
	{
		// write msg
		debug_printf("write msg ...\n");
		ret = write(sockfd, buff, size);
		if (ret == -1)
		{
			debug_printf("write fail\n");
			return ret;
		}

		// read msg
		ret = read(sockfd, buff, size);
		if (ret == -1)
		{
			debug_printf("echo read fail\n");
			return ret;
		}
		debug_printf("echo read:%s\n", buff);
	}
	else if (service == 2)
	{
		// write msg
		debug_printf("write msg ...\n");
		ret = write(sockfd, buff, size);
		if (ret == -1)
		{
			debug_printf("write fail\n");
			return ret;
		}
	}
	else if (service ==3)
	{
		// write msg
		debug_printf("write msg ...\n");
		ret = write(sockfd, buff, size);
		if (ret == -1)
		{
			debug_printf("write fail\n");
			return ret;
		}

		// read msg
		ret = read(sockfd, buff, size);
		if (ret == -1)
		{
			debug_printf("get read fail\n");
			return ret;
		}
		debug_printf("get read:%s\n", buff);
	}

	// close socket
	debug_printf("close socket ...\n");
	ret = close(sockfd);
	if (ret == -1)
	{
		debug_printf("close socket fail\n");
		return ret;
	}

	return 0;
}

void echo_interface()
{
	printf("press your words:\n");
	char buff[BUFF_SIZE+1];
	
	scanf("%s", &buff[1]);
	printf("you pressed: %s\n", &buff[1]);
	buff[0] = '1';
	client_proxy(1, buff, BUFF_SIZE+1);
	printf("server echo:%s\n", buff);
}

void put_interface()
{
	printf("press your words:\n");
	char buff[BUFF_SIZE+1];
	
	scanf("%s", &buff[1]);
	buff[0] = '2';
	client_proxy(2, buff, BUFF_SIZE+1);
}

void get_interface()
{
	char buff[BUFF_SIZE+1];
	buff[0] = '3';
	client_proxy(3, buff, BUFF_SIZE+1);
	printf("get from server:%s\n", buff);
}

int client_interface()
{
	printf("=====Interest Client=====\n");
	printf("service 1: Echo\n");
	printf("service 2: Put\n");
	printf("service 3: Get\n");
	printf("press your choice (Enter to finish):");
	char buff[BUFF_SIZE];
	// gets(buff);
	fgets(buff, sizeof(buff), stdin);
	printf("you pressed:%s\n", buff);
	
	if (buff[0] == '1' && buff[1] == '\0')
	{
		echo_interface();
	}
	else if (buff[0] == '2' && buff[1] == '\0')
	{
		put_interface();
	}
	else if (buff[0] == '3' && buff[1] == '\0')
	{
		get_interface();
	}
	else
	{
		printf("invalued input!\n");
	}
	printf("press any to continue\n");
	// fgets(buff);
	fgets(buff, sizeof(buff), stdin);

	return 0;
}


int main()
{
	while (1)
		client_interface();

	return 0;
}
