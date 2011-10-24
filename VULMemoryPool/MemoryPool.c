/**
 * @file   VULMemoryPool/MemoryPool.c
 *
 * @date   Oct 20, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Variable length, Unable to recycle, List style memory pool.
 */

// The following macro designed for test purpose only, delete it when using.
#include "../MemoryPoolTester.h"
#ifdef ENABLE_VULMemoryPool

#include "MemoryPool.h"

MemoryPool_t *CreateMemoryPool(unsigned short uMaxStrLen)
{
	unsigned short uFreeTableLen = GetIndex(uMaxStrLen) + 1;

	MemoryPool_t *pPool = (MemoryPool_t *)malloc(sizeof(MemoryPool_t));
	pPool->uMaxSize = uMaxStrLen > MAX_STRING_LEN ? MAX_STRING_LEN : uMaxStrLen;
	pPool->pFirstBigBlock = NULL;
	pPool->pTable = (BlockTable_t *)malloc(sizeof(BlockTable_t) * uFreeTableLen);
	for (int i=0; i<uFreeTableLen; ++i)
	{
		pPool->pTable[i] = NULL;
	}

	return pPool;
}

void DestroyMemoryPool(MemoryPool_t **pPool)
{
	assert(NULL != *pPool);
	Node_t *pCurrNode = NULL;
	Node_t *pPreNode = NULL;
	BigBlock_t *pCurrBlock = NULL;
	BigBlock_t *pPreBlock = NULL;

	unsigned short uFreeTableLen = GetIndex((*pPool)->uMaxSize) + 1;
	for (int i=0; i<uFreeTableLen; ++i)
	{
		pCurrNode = (*pPool)->pTable[i];
		while(NULL != pCurrNode)
		{
			pPreNode = pCurrNode;
			pCurrNode = pCurrNode->pNext;
			free(pPreNode);
		}
	}
	free((*pPool)->pTable);

	pCurrBlock = (*pPool)->pFirstBigBlock;
	while(NULL != pCurrBlock)
	{
		pPreBlock = pCurrBlock;
		pCurrBlock = pCurrBlock->pNext;
		free(pPreBlock);
	}

	free(*pPool);
	*pPool = NULL;
}

void *Malloc(MemoryPool_t *pPool, unsigned short uSize)
{
	assert(NULL != pPool);
	assert(0 != uSize);
	unsigned short uIndex = GetIndex(uSize);
	void *pPtr = NULL;

/*	if (uSize > pPool->uMaxSize)
	{
		pPtr = malloc(sizeof(BigBlock_t) + sizeof(unsigned short) + uSize);
		*((unsigned short *)(pPtr + sizeof(BigBlock_t))) = uSize;
		BigBlock_t *pBigBlock = (BigBlock_t *)pPtr;
		pBigBlock->data = pPtr + sizeof(BigBlock_t) + sizeof(unsigned short);
		pBigBlock->pNext = pPool->pFirstBigBlock;
		pBigBlock->pPre = NULL;
		pPool->pFirstBigBlock->pPre = pBigBlock;
		pPool->pFirstBigBlock = pBigBlock;

		return pBigBlock->data;
	}
*/
	if (NULL != (pPool->pTable[uIndex]))
	{
		pPtr = (void *)&(pPool->pTable[uIndex]->data);
		pPool->pTable[uIndex] = pPool->pTable[uIndex]->pNext;
		*(unsigned short *)pPtr = uSize;
		pPtr += sizeof(unsigned short);
	}
	else
	{
		pPtr = malloc(sizeof(unsigned short) + RoundUp(uSize));
		if (NULL == pPtr)
		{
			PrintError("Failed to malloc memory from system.");
			return NULL;
		}
		*(unsigned short *)pPtr = uSize;
		pPtr += sizeof(unsigned short);
	}

	return pPtr;
}

void Free(MemoryPool_t *pPool, void *pPtr)
{
	if (NULL == pPool)
	{
		PrintWarning("A ptr will be freed but memory pool already been destroy.");
		free(pPtr);
		return;
	}

	pPtr -= sizeof(unsigned short);
	unsigned short uSize = *((unsigned short *)pPtr);
/*	if (uSize > pPool->uMaxSize)
	{
printf("free big block.\n");
		pPtr -= sizeof(BigBlock_t);
		BigBlock_t *pBigBlock = (BigBlock_t *)pPtr;
		(NULL == pBigBlock->pPre) ? (pPool->pFirstBigBlock = pBigBlock->pNext)
				                  : (pBigBlock->pPre->pNext = pBigBlock->pNext);
		free(pPtr);
		return;
	}*/
	unsigned short uIndex = GetIndex(uSize);
	Node_t *pNode = (Node_t *)pPtr;
	pNode->pNext = pPool->pTable[uIndex];
	pPool->pTable[uIndex] = pNode;
}

#endif /* ENABLE_VULMemoryPool */
