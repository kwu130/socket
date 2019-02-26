#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

static void usage(const char* proc)
{
	printf("usage:[%s] [port]\n", proc);
}

int create_socket(int _port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);//创建socket
	if(sock < 0)
	{
		perror("creat socket fail");
		exit(1);
	}
	
	struct sockaddr_in server;
	
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;//ipv4协议簇
	server.sin_port = htons(_port);//待绑定端口
	server.sin_addr.s_addr = htonl(INADDR_ANY);//INADDR_ANY==0.0.0.0 指本机所有ip
	
	if(bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0)//绑定
	{
		perror("bind error");
		exit(2);
	}
	
	if(listen(sock, 5) < 0)//监听 最大监听数为5
	{
		perror("listen error");
		exit(3);
	}
	
	return sock;
}

int main(int argc, char* argv[])
{	
	if(argc != 2)
	{
		usage(argv[0]);
		return 3;
	}
	
	int listen_socket = create_socket(atoi(argv[1]));
	
	struct sockaddr_in remote;
	socklen_t len = sizeof(struct sockaddr_in);
	
	while(1)
	{
		int socket = accept(listen_socket, (struct sockaddr*)&remote, &len);
		if(socket < 0)
		{
			perror("accept error");
			continue;
		}
		
		printf("#Peer's ip:%s, port:%d#\n", inet_ntoa(remote.sin_addr), ntohs(remote.sin_port));
		
		char buf[BUFFER_SIZE];
		int fd = open("123.jpg", O_CREAT|O_RDWR);
		if (fd < 0)
		{
			printf("open file error");
			exit(1);
		}
		int ncount;
		while ((ncount = read(socket, buf, BUFFER_SIZE)) > 0)
		{
			printf("ncount:%d\n", ncount);
			write(fd, buf, ncount);
		}
		close(fd);
		close(socket);
		printf("Peer released connection!\n");
	}

	close(listen_socket);

	return 0;
	
}

