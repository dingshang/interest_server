#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>

#define BUFF_SIZE 1024
#define OTHER_SIZE 1024

void signal_func(int sig)
{
	printf("Got signal:%d\n", sig);
	exit(0);
}

void echo_service(int client_fd, char* buff, int size)
{
	/* 
		* buff [BUFF_SIZE+1] 
 		* [0]:flag 
 		* [1~BUFF_SIZE]:data 
 	*/
	int ret = write(client_fd, buff+1, size-1);
	if (ret < 0)
	{
		printf("echo service write fail\n");
		close(client_fd);
		exit(ret);
	}
}

int write_datafile(char* buff, int size)
{
 	int ret = 0;
	// write into file
	FILE * f = fopen("client_box.txt", "w");
	if (f <= 0)
	{
		printf("open file fail\n");
		return ret;
	}

	ret = fwrite(buff, sizeof(char), size, f);
	if (ret < 0)
	{
		printf("write into file fail\n");
		fclose(f);
		return ret;
	}
	ret = fclose(f);
	if (ret < 0)
	{
		printf("close file fail\n");
		return ret;
	}
	return 0;
}

void put_service(int client_fd, char* buff, int size)
{
	int ret = write_datafile(&buff[1], strlen(&buff[1]));	
	if (ret < 0)
	{
		printf("put service fails!\n");
		close(client_fd);
		exit(ret);
	}
}

int read_data(char* buff, int size)
{
	int ret = 0;

	FILE * f = fopen("client_box.txt", "r");
	if (f <= 0)
	{
		printf("open file FAIL!\n");
		return -1;
	}

	ret = fread(buff, sizeof(char), size, f);
	if (ret < 0)
	{
		printf("read file FAIL!\n");
		fclose(f);
		return ret;
	}

	int ret2 = fclose(f);
	if (ret2 < 0)
	{
		printf("close file FAIL!\n");
		return ret2;
	}

	return ret;
}

void get_service(int client_fd)
{
	int ret = 0;

	char buff[BUFF_SIZE];
	ret = read_data(buff, BUFF_SIZE);
	if (ret < 0)
	{
		printf("read data FAIL!\n");
		close(client_fd);
		exit(ret);
	}

	ret = write(client_fd, buff, BUFF_SIZE);
	if (ret < 0)
	{
		printf("write to client FAIL!\n");
		close(client_fd);
		exit(ret);
	}

}		

void http_get_service(int client_fd)
{
	int ret = 0;

	char buff[BUFF_SIZE];
	memset(buff, 0, BUFF_SIZE);
	ret = read_data(buff, sizeof(buff));

	char response_ok_1[] = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n\r\n";
	
	int len_1 = strlen(response_ok_1);
	int len_buff = sizeof(buff);
	char response_ok[len_1 + len_buff + 1];
	int i = 0;
	for(i=0; i<len_1; i++)
		response_ok[i]= response_ok_1[i];

	int j = 0;
	for(j=0; j<len_buff; j++)
		response_ok[i+j] = buff[j];

	response_ok[i+j] = '\0';

	char response_error[] = "HTTP/1.1 500 ERROR\r\n\r\n";

	char * response = (ret >= 0) ? response_ok : response_error;

	ret = write(client_fd, response, strlen(response));	
	if (ret < 0)
	{
		printf("write to client FAIL!\n");
		close(client_fd);
		exit(ret);
	}
}

void http_put_service(int client_fd, char *buff, int size)
{
	int ret = 0;

	// get data
	///find substring \r\n\r\n
	int index = -1;
	for(index = 0; index+3<size; index++)
	{
		if (buff[index] == '\r' &&
				buff[index+1] == '\n' &&
				buff[index+2] == '\r' &&
				buff[index+3] == '\n')
		{
			break;
		}
	}
	if (index+4 > size)
	{
		printf("No data\n");
	}
	else
	{
		printf("abstract data:%s\n", &buff[index+4]);
	}

	ret = write_datafile(&buff[index+4], strlen(&buff[index+4]));

	char response_ok[] = 
	"HTTP/1.1 200 OK\r\n"
	"\r\n";
	char response_bad[] = 
	"HTTP/1.1 500 SERROR\r\n"
	"\r\n";
	char * presponse = response_ok;
	if (ret < 0)
		presponse = response_bad;
	ret = write(client_fd, presponse, strlen(presponse));
	if (ret <= 0)
	{
		printf("write fail!\n");
		close(client_fd);
		exit(ret);
	}
}

int service()
{
	int ret;

	// deal with signal
	signal(SIGINT, signal_func);	

	// create socket
	printf("create socket ...\n");

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		printf("create socket fail\n");
		return server_fd;
	}	

	// bind socket
	printf("bind socket ...\n");
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8000);
	addr.sin_addr.s_addr = inet_addr("192.168.122.1");

	ret = bind(server_fd, (struct sockaddr *) &addr, sizeof(addr));
	if (ret == -1)
	{
		printf("bind socket fail\n");
		return ret;
	}

	// listen socket
	printf("listen socket ...\n");
	int backlog = 50;
	ret = listen(server_fd, backlog);
	if (ret == -1)
	{
		printf("listen socket fail\n");
		return ret;
	}

	// accept
	struct sockaddr_in client_addr;
	socklen_t addr_len;
	while (1)
	{
		printf("accept socket blocking ...\n");
		int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &addr_len);

		if (!fork())
		{ /* child process */

			close(server_fd); 
										 // close server_fd and exit(0): 4,4,4,...
										 // not close server_fd and exit(0): 4,4,4,...
										 // not close server_fd and not exit(0): 4,4,4,..

			printf("client fd:%i;", client_fd);
			printf(" ip:%s;", inet_ntoa(client_addr.sin_addr));
			printf(" port:%i\n", client_addr.sin_port);

			char buff[BUFF_SIZE + OTHER_SIZE];
			memset(buff, 0, sizeof(buff));
			ret = read(client_fd, buff, sizeof(buff));
			if (ret < 0)
			{
				printf("read from client_fd fail\n");
				return ret;
			}

			printf("read msg, size[%i]:\n%s\n", ret, buff);

			// services
			if (buff[0] == '1')
			{
				printf("client choose service 1: echo service\n");
				echo_service(client_fd, buff, ret);
			}
			else if (buff[0] == '2')
			{
				printf("client choose service 2: put service\n");
				put_service(client_fd, buff, ret);
			}
			else if (buff[0] == '3')
			{
				printf("client choose service 3: get service\n");
				get_service(client_fd);
			}
			else if (strncmp(buff, "GET", 3) == 0)
			{
				printf("client request http GET\n");
				http_get_service(client_fd);	
			}
			else if (strncmp(buff, "PUT", 3) == 0)
			{
				printf("client request http PUT\n");
				http_put_service(client_fd, buff, ret);
			}
			else
			{
				printf("Unknown client request!\n");
			}

			close(client_fd); // FIXME IMPORTANT: client will block without close
			exit(0);
		}

		/* parent process */
		close(client_fd); // FIXME IMPORTANT: client will block without close
		printf("\n");
	}

	return 0;
}

int main()
{
	return service();
}

