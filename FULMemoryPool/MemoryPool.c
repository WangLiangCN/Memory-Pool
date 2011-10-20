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
	pHead->uAvailable = 0;
	pHead->pFirstAvailable = NULL;

	return pHead;
}

void DestoryMemoryPool(MemoryPool_t **pPool)
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
}

void *Malloc(MemoryPool_t *pPool)
{
	void *pPtr = NULL;

	if (pPool->uAvailable > 0)
	{
		pPtr = &(pPool->pFirstAvailable->data);
		-- pPool->uAvailable;
		pPool->pFirstAvailable = pPool->pFirstAvailable->pNext;
	}
	else
	{
		pPtr = my_malloc(pPool->uMaxSize, __FUNCTION__, __LINE__);
	}

	return pPtr;
}

void Free(MemoryPool_t *pPool, void *pPtr)
{
	Node_t *pFreeNode = (Node_t *)pPtr;

	pFreeNode->pNext = pPool->pFirstAvailable;
	pPool->pFirstAvailable = pFreeNode;
	++ pPool->uAvailable;
}
