/**
 * @file   FULMemoryPool/MemoryPool.h
 *
 * @date   Oct 19, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief
 */

#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_

#include "../CProjectDfn.h"

typedef union Node
{
	union Node *pNext;
	char data[1];
}Node_t;

typedef struct Head
{
	unsigned int uMaxSize;
	Node_t *pFirstAvailable;
}Head_t;

typedef Head_t MemoryPool_t;

MemoryPool_t *CreateMemoryPool(unsigned int uMaxSize);
void DestroyMemoryPool(MemoryPool_t **pPool);

inline void *Malloc(MemoryPool_t *pPool)
{
	void *pPtr = NULL;

	if (NULL != pPool->pFirstAvailable)
	{
		pPtr = &(pPool->pFirstAvailable->data);
		pPool->pFirstAvailable = pPool->pFirstAvailable->pNext;
	}
	else
	{
		pPtr = malloc(pPool->uMaxSize);
		if (NULL == pPtr)
		{
			PrintWarning("Failed to malloc memory from system.");
		}
	}

	return pPtr;
}

inline void Free(MemoryPool_t *pPool, void *pPtr)
{
	Node_t *pFreeNode = (Node_t *)pPtr;

	if (NULL == pPool)
	{
		PrintWarning("A ptr will be freed but memory pool already been destroy.");
		free(pPtr);
		return;
	}

	pFreeNode->pNext = pPool->pFirstAvailable;
	pPool->pFirstAvailable = pFreeNode;
}

#endif /* MEMORYPOOL_H_ */
