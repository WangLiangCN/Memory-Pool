/**
 * @file   FALMemoryPool/MemoryPool.c
 *
 * @date   Oct 25, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Fixed length, Able to recycle, List style memory pool.
 *   Create and destroy memory pool, due to frequent use of Malloc() and Free(), so make it
 * inline and defined in MemoryPool.h
 */

// The following macro designed for test purpose only, delete it when using.
#include "../MemoryPoolTester.h"
#ifdef ENABLE_FALMemoryPool

#include "MemoryPool.h"

/**
 * @brief Create a empty pool.
 *
 *   Each block in this pool have minimum size of sizeof(union Node), will up to it if smaller than.
 *
 * @param uBlockSize Every memory block have this length, maximum length of string with '\0' can be in.
 * @return Created memory pool.
 */
MemoryPool_t *CreateMemoryPool(unsigned int uBlockSize)
{
	Head_t *pHead = (Head_t *)malloc(sizeof(Head_t));
	pHead->uBlockSize = uBlockSize > sizeof(Node_t) ? uBlockSize : sizeof(Node_t);
	pHead->pFirstAvailable = NULL;
	pHead->uAvailableNum = 0;

	return pHead;
}

/**
 * @brief Destroy a memory pool.
 *
 * @param pPoll when finished, this will be NULL.
 * @note Make sure every address get from this memory pool is released by Free().
 */
void DestroyMemoryPool(MemoryPool_t **pPool)
{
	assert(NULL != pPool);

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

#endif /* ENABLE_FALMemoryPool */
