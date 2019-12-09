#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>

#define BUFF_SIZE 1024

void signal_func(int sig)
{
	printf("Got signal:%d\n", sig);
	exit(0);
}

int service()
{
	int ret;

	// deal with signal
	signal(SIGINT, signal_func);	

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

	ret = bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
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
		printf("\naccept socket blocking ...\n");
		client_fd = accept(sockfd, (struct sockaddr *) &client_addr, &addr_len);


		if (!fork())
		{ /* child process */

			close(sockfd); 
										 // close sockfd and exit(0): 4,4,4,...
										 // not close sockfd and exit(0): 4,4,4,...
										 // not close sockfd and not exit(0): 4,4,4,..
			printf("client fd:%i;", client_fd);
			printf(" ip:%s;", inet_ntoa(client_addr.sin_addr));
			printf(" port:%i\n", client_addr.sin_port);
			char buff[BUFF_SIZE+1];
			ret = read(client_fd, buff, BUFF_SIZE+1);
			if (ret >= 0)
			{
				printf("read msg, size[%i]:\n%s\n", ret, buff);

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

					// write into file
					FILE * f = fopen("client_box.txt", "w");
					if (f < 0)
					{
						printf("open file fail\n");
						return -1;
					}
					ret = fwrite(&buff[1], sizeof(char), BUFF_SIZE, f);
					if (ret < 0)
					{
						printf("write into file fail\n");
						return ret;
					}
					ret = fclose(f);
					if (ret < 0)
					{
						printf("close file fail\n");
						return ret;
					}

				}
				else if (buff[0] == '3')
				{
					printf("client choose service 3: get service\n");

					// read from file
					FILE * f = fopen("client_box.txt", "r");
					char buff[BUFF_SIZE];
					ret = fread(buff, sizeof(char), BUFF_SIZE, f);
					if (ret < 0)
					{
						printf("read file fail\n");
						return ret;
					}

					ret = write(client_fd, buff, BUFF_SIZE);
					if (ret < 0)
					{
						printf("get service write fail\n");
						return ret;
					}
			
				}
				else if (buff[0]=='G' && buff[1]=='E' && buff[2]=='T')
				{
					printf("client request http GET\n");
					
					char response[] = 
					"HTTP/1.1 200 OK\r\n"
					"Content-Type: text/html\r\n\r\n"
					"<!DOCTYPE html><html><body>\r\n"
					"<h1>For your Interest!</h1>\r\n"
					"</body></html>\r\n";

					ret = write(client_fd, response, strlen(response));	
					printf("write ret=%d", ret);
					if (ret <= 0)
					{
						printf("write fail!\n");
					}
					
					close(client_fd); // FIXME IMPORTANT: client will block without close
				}
				else
				{
					printf("Unknown client request!\n");
				}
			}
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

