#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
/*socket*/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //htons
#include <netinet/in.h> //inet_addr
#include <pthread.h>    //pthread_create

void getStrFromSTDIN(char caMsg[64])
{
	read(STDIN_FILENO, caMsg, 64);
	char *p = strrchr(caMsg, '\n');
	if (NULL != p)
	{
		*p = '\0';
	}
	else
	{
		caMsg[63] = '\0';
		while ('\n' != getchar())
		{}
	}
}

void *handleServer(void *arg)
{
	//接收服务器的信息
	int sockfd = (int)arg;
	char caMsg[64] = {'\0'};
	int ret = -1;
	while (1)
	{
		memset(caMsg, '\0', 64);
		ret = read(sockfd, caMsg, 64);
		if (-1 == ret || 0 == ret)
		{
			break;
		}	
		printf("recv msg: %s\n", caMsg);
	}
}

int main(void)
{
	int sockfd = -1;
	//AF_INET:使用ipv4协议来通信
	//SOCK_STREAM:可靠传输  --> tcp
	//SOCK_DGRAM:非可靠传输 --> udp
	//0:表示使用传输方式相对应的默认协议
	//成功返回socket文件描述符
	//可以像读写文件一样方便的发送和接收数据
	//失败返回-1
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sockfd)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	//设置要连接的服务器的ip地址
	//将点分十进制字符串表示的ip转换成整数表示
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//设置要连接的服务器的端口，用于识别应用
	//htons:将数据由本地字节序转换成网络字节序表示
	//字节序:大端字节序和小端字节序
	servAddr.sin_port = htons(8888);
	
	//将指定大小的空间数据置为0
	bzero(servAddr.sin_zero, 8);

	int ret = -1;
	//连接服务器
	ret = connect(sockfd
				  , (struct sockaddr *)&servAddr
				  , sizeof(servAddr));
	if (-1 == ret)
	{
		perror("connect");
		return -1;
	}
	printf("connect to server success\n");

	pthread_t thr;
	pthread_create(&thr, NULL, handleServer
				   , (void *)sockfd);

	//往服务器发送信息
	char caMsg[64] = {'\0'};
	while (1)
	{
		memset(caMsg, '\0', 64);
		getStrFromSTDIN(caMsg);
		write(sockfd, caMsg, strlen(caMsg));
	}
		
	return 0;
}
