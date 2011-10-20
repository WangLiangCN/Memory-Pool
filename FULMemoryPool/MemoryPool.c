/**
 * @file   FULMemoryPool/MemoryPool.c
 *
 * @date   Oct 19, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief
 */

#include "MemoryPool.h"

MemoryPool_t *CreateMemoryPool(unsigned int uMaxSize)
{
	Head_t *pHead = (Head_t *)malloc(sizeof(Head_t));
	pHead->uMaxSize = uMaxSize > sizeof(Node_t) ? uMaxSize : sizeof(Node_t);
	pHead->pFirstAvailable = NULL;

	return pHead;
}

void DestroyMemoryPool(MemoryPool_t **pPool)
{
	Node_t *pNode = (*pPool)->pFirstAvailable;
	Node_t *pPreNode = NULL;

	while (NULL != pNode)
	{
		pPreNode = pNode;
		pNode = pNode->pNext;
		free(pPreNode);
	}
	free(*pPool);
	*pPool = NULL;
}
