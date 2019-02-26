#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

static void usage(const char* proc)
{
	printf("usage:[%s] [ip] [port]\n", proc);
}

int create_socket(char* _ip, int _port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);//创建socket
	
	if(sock < 0)
	{
		perror("socket error");
		exit(1);
	}
	
	struct sockaddr_in server;
	
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;//ipv4协议簇
	server.sin_port = htons(_port);//服务器端口
	server.sin_addr.s_addr = inet_addr(_ip);//服务器ip
	
	if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)//尝试连接服务器
	{
		perror("connect error");
		exit(2);
	}
	
	return sock;
	
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		usage(argv[0]);
		return 3;
	}
	
	int mysock = create_socket(argv[1], atoi(argv[2]));
	
	char buf[BUFFER_SIZE];


	int fd = open("123.jpg", O_RDONLY);
	if (fd < 0)
	{
		printf("open file error");
		exit(1);
	}
	
	int ncount;
	while ((ncount = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		write(mysock, buf, ncount);//发送数据到服务器
	}
	close(fd);
	close(mysock);
	
	return 0;
}
