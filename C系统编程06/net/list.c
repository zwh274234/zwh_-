#include "list.h"

//创建节点
UsrInfo *makeUsrInfoNode()
{
	UsrInfo *node = NULL;
	node = (UsrInfo *)malloc(sizeof(UsrInfo));
	if (NULL == node)
	{
		printf("malloc usr info node failed\n");
		exit(EXIT_FAILURE);
	}
	memset(node, 0, sizeof(UsrInfo));
	node->sockfd = -1;
	return node;
}
//创建链表
UsrList *makeUsrList()
{
	UsrList *list = NULL;
	list = (UsrList *)malloc(sizeof(UsrList));
	if (NULL == list)
	{
		printf("malloc usr info list failed\n");
		exit(EXIT_FAILURE);
	}
	memset(list, 0, sizeof(UsrList));
	return list;
}
//将节点插入链表
void insertUsrList(UsrList *list, UsrInfo *node)
{
	if (NULL == list || NULL == node)
	{
		return;
	}
	//头插法
	node->pNext = list->pFirstNode;
	list->pFirstNode = node;
	list->iLen++;
}
//显示节点
void showUsrInfoNode(const UsrInfo *node)
{}
//显示链表
void showUsrList(const UsrList *list)
{
	if (NULL != list)
	{
		UsrInfo *node = list->pFirstNode;
		while (NULL != node)
		{
			printf("%d\t", node->sockfd);
			node = node->pNext;
		}
		putchar('\n');
	}

}
