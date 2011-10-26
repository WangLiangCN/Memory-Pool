/**
 * @file   VALMemoryPool/MemoryPool.c
 *
 * @date   Oct 20, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Variable length, Able to recycle, List style memory pool.
 */

// The following macro designed for test purpose only, delete it when using.
#include "../MemoryPoolTester.h"
#ifdef ENABLE_VALMemoryPool

#include "MemoryPool.h"

/**
 * @brief Create memory pool, so can allocate memory after that.
 *
 * It doesn't means can't get memory if bigger than given size, pool will deliver to system functions,
 * so that performance is equal as system. When destroy pool, make sure you have release all memory block
 * smaller than [uMaxStrLen], pool doesn't track them, if bigger than [uMaxStrLen], pool will release all
 * of them to prevent memory leak.
 *
 * @param uMaxStrLen Max length of string this pool can allocate.
 * @return Created memory pool.
 */
MemoryPool_t *CreateMemoryPool(unsigned short uMaxStrLen)
{
	uMaxStrLen = (uMaxStrLen > MAX_STRING_LEN) ? MAX_STRING_LEN : uMaxStrLen;
	unsigned short uFreeTableLen = GetIndex(uMaxStrLen) + 1;

	MemoryPool_t *pPool = (MemoryPool_t *)malloc(sizeof(MemoryPool_t) + (sizeof(Head_t) * uFreeTableLen));
	pPool->uMaxSize = uMaxStrLen;
	pPool->pFirstBigBlock = NULL;
	pPool->pTable = (BlockTable_t *)((void *)pPool + sizeof(MemoryPool_t));
	for (int i=0; i<uFreeTableLen; ++i)
	{
		pPool->pTable[i].pFirstNode = NULL;
		pPool->pTable[i].uIdleNum = 0;
	}

	return pPool;
}

/**
 * @brief Destroy memory pool, release all idle memory block smaller than max size pool can allocate, and
 * release all blocks bigger than this size not matter using or not.
 *
 * @param pPool Which pool to destroy, set to NULL when finished to destroy.
 */
void DestroyMemoryPool(MemoryPool_t **pPool)
{
	assert(NULL != *pPool);
	Node_t *pCurrNode = NULL;
	Node_t *pPreNode = NULL;
	BigBlock_t *pCurrBlock = NULL;
	BigBlock_t *pPreBlock = NULL;

	// Release idle blocks in pool.
	unsigned short uFreeTableLen = GetIndex((*pPool)->uMaxSize) + 1;
	for (int i=0; i<uFreeTableLen; ++i)
	{
		pCurrNode = (*pPool)->pTable[i].pFirstNode;
		while(NULL != pCurrNode)
		{
			pPreNode = pCurrNode;
			pCurrNode = pCurrNode->pNext;
			free(pPreNode);
		}
	}

	// Release block which bigger than pool can allocate.
	pCurrBlock = (*pPool)->pFirstBigBlock;
	while(NULL != pCurrBlock)
	{
		pPreBlock = pCurrBlock;
		pCurrBlock = pCurrBlock->pNext;
		free(pPreBlock);
	}

	// Release pool.
	free(*pPool);
	*pPool = NULL;
}

/**
 * @biref Get a memory block from pool.
 *
 * @param pPool From which pool to get.
 * @param uSize Size of string want to allocate.
 * @return Allocated memory.
 */
void *Malloc(MemoryPool_t *pPool, unsigned short uSize)
{
	assert(NULL != pPool);
	assert(0 != uSize);
	unsigned short uIndex = GetIndex(uSize);
	void *pPtr = NULL;

	// If user want to allocate a memory bigger than pool can do, deliver this to system and record it.
	if (uSize > pPool->uMaxSize)
	{
		pPtr = malloc(sizeof(BigBlock_t) + sizeof(unsigned short) + uSize);
		*((unsigned short *)(pPtr + sizeof(BigBlock_t))) = uSize;
		BigBlock_t *pBigBlock = (BigBlock_t *)pPtr;
		pBigBlock->data = pPtr + sizeof(BigBlock_t) + sizeof(unsigned short);
		pBigBlock->pNext = pPool->pFirstBigBlock;
		pBigBlock->pPre = NULL;
		(NULL != pPool->pFirstBigBlock) ? (pPool->pFirstBigBlock->pPre = pBigBlock) : 0;
		pPool->pFirstBigBlock = pBigBlock;

		return pBigBlock->data;
	}

	// Check if there are idle blocks can be use again, or allocate new blocks from system.
	if (NULL != (pPool->pTable[uIndex].pFirstNode))
	{
		pPtr = (void *)&(pPool->pTable[uIndex].pFirstNode->data);
		pPool->pTable[uIndex].pFirstNode = pPool->pTable[uIndex].pFirstNode->pNext;
		-- (pPool->pTable[uIndex].uIdleNum);
		*(unsigned short *)pPtr = uSize;
		pPtr += sizeof(unsigned short);
	}
	else
	{
		unsigned short uLen = sizeof(unsigned short) + RoundUp(uSize);
		uLen = (uLen > sizeof(Node_t)) ? uLen : sizeof(Node_t);
		pPtr = malloc(uLen);
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

/**
 * @brief Back a memory block to pool so that it can be use again.
 *
 * @param pPool Back to which pool.
 * @param pPtr Address of memory block to back.
 */
void Free(MemoryPool_t *pPool, void *pPtr)
{
	if (NULL == pPool)
	{
		PrintWarning("A ptr will be freed but memory pool already been destroy.");
		free(pPtr);
		return;
	}

	// Check if big blocks allocated from system directly, if so, release it to system, pool won't use it.
	pPtr -= sizeof(unsigned short);
	unsigned short uSize = *((unsigned short *)pPtr);
	if (uSize > pPool->uMaxSize)
	{
		pPtr -= sizeof(BigBlock_t);
		BigBlock_t *pBigBlock = (BigBlock_t *)pPtr;
		(NULL == pBigBlock->pPre) ? (pPool->pFirstBigBlock = pBigBlock->pNext)
				                  : (pBigBlock->pPre->pNext = pBigBlock->pNext);
		free(pPtr);
		return;
	}

	// Check if needs to release it due to too many idle blocks in list.
	unsigned short uIndex = GetIndex(uSize);
	if ((pPool->pTable[uIndex].uIdleNum + 1) > RECYCLE_IF_MORETHAN_BLOCKS)
	{
		free(pPtr);
		return;
	}
	// Back the memory block to pool so that can use it again.
	Node_t *pNode = (Node_t *)pPtr;
	pNode->pNext = pPool->pTable[uIndex].pFirstNode;
	pPool->pTable[uIndex].pFirstNode = pNode;
	++ (pPool->pTable[uIndex].uIdleNum);
}

#endif /* ENABLE_VALMemoryPool */
