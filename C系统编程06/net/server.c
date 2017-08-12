#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
/*socket*/
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>  //htons
#include <netinet/in.h> //inet_addr
#include <pthread.h>

#include "list.h"

UsrList *g_pUsrList = NULL;

void *handleClient(void *arg)
{
	int sockfd = (int)arg;

	UsrInfo *node = makeUsrInfoNode();
	node->sockfd = sockfd;
	insertUsrList(g_pUsrList, node);

	char caMsg[64] = {'\0'};
	int ret = 0;
	int per = -1;
	char caRealMsg[64] = {'\0'};
	char *p = NULL;
	while (1)
	{
		memset(caMsg, '\0', 64);
		memset(caRealMsg, '\0', 64);
		ret = read(sockfd, caMsg, 64);
		if (-1 == ret || 0 == ret)
		{
			break;
		}
		// 4-->"5 hello server"
		sscanf(caMsg, "%d", &per);

		// "4 says: hello server" --> 5
		p = strchr(caMsg, ' ');
		
		sprintf(caRealMsg, "%d says: %s", sockfd, p);
		
		if (0 == per)
		{
			node = g_pUsrList->pFirstNode;
			while (NULL != node)
			{
				write(node->sockfd
					  , caRealMsg
					  , strlen(caRealMsg));
				node = node->pNext;
			}
		}
		else
		{
			write(per, caRealMsg, strlen(caRealMsg));
		}
	}
}

int main(void)
{
	g_pUsrList = makeUsrList();

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
	//设置要监听的服务器的ip地址
	//将点分十进制字符串表示的ip转换成整数表示
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//设置要监听的服务器的端口，用于识别应用
	//htons:将数据由本地字节序转换成网络字节序表示
	//字节序:大端字节序和小端字节序
	servAddr.sin_port = htons(8888);
	
	//将指定大小的空间数据置为0
	bzero(servAddr.sin_zero, 8);

	int ret = -1;
	//设置要监听的ip和端口
	ret = bind(sockfd
			   , (struct sockaddr *)&servAddr
			   , sizeof(servAddr));
	if (-1 == ret)
	{
		perror("ret");
		return -1;
	}
	//设置服务器每次能够处理最大的客户端数
	//并且使sockfd处于监听状态
	listen(sockfd, 10);

	struct sockaddr_in clientAddr;
	int iLen = sizeof(clientAddr);
	int clientSockfd = -1;
	pthread_t thr;
	while (1)
	{
		printf("接收客户端的连接...\n");
		//接收由sockfd监听到的客户端的连接
		//若没有客户端的连接则阻塞等待
		//并连接来的客户端的基本信息写入clientAddr中
		//连接成功产生一个新的socket，
		//该新产生的socket由于服务器和该客户端通信
		//失败返回-1
		clientSockfd = accept(sockfd
					, (struct sockaddr *)&clientAddr
					, &iLen);
		if (-1 == clientSockfd)
		{
			perror("accept");
			break;
		}
		//inet_ntoa:
		//将整数表示的ip转换成点分十进制字符串表示
		printf("ip为%s的客户端连接到服务器\n"
			   , inet_ntoa(clientAddr.sin_addr));
		printf("新产生的sockfd=%d\n", clientSockfd);
	
		//每来一个客户端的连接
		//专门创建一个线程处理该客户端	
		pthread_create(&thr, NULL, handleClient
					   , (void *)clientSockfd);	
	}

	return 0;
}
