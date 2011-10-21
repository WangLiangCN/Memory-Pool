/**
 * @file   FULMemoryPool/MemoryPool.h
 *
 * @date   Oct 19, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief Fixed length, Unable to recycle, List style memory pool.
 *
 * There are available block in memory pool:
 *
 *         data   pFristAvailable          data
 *        -----> +-------+ ------         -----> +-------+ ------
 *               |       |   |                   |       |   |        Next
 *    Header     | Union |  Next        Next     | Union |  Next    ------->   ......
 *   ------->    |       |   |        -------->  |       |   |
 *               |       |   |                   |       |   |
 *               +-------+ ------                +-------+ ------
 *               |       |                       |       |
 *               |       |                       |       |
 *               |  ...  |                       |  ...  |
 *               |       |                       |       |
 *               |       |                       |       |
 *               +-------+                       +-------+
 *
 * Get a available block from memory pool:
 *
 *         ----------------------
 *         |                    |          data   pFristAvailable
 *         |  -> +-------+      |         -----> +-------+ ------
 *  Header |  |  |       |      |  Next          |       |   |        Next
 * ---------  |  |       |      ------------->   | Union |  Next    ------->   ......
 *            |  |       |                       |       |   |
 *  User_Ptr  |  |       |                       |       |   |
 * ------------  | USER  |                       +-------+ ------
 *               |       |                       |       |
 *               | DATA  |                       |       |
 *               |       |                       |  ...  |
 *               |       |                       |       |
 *               |       |                       |       |
 *               +-------+                       +-------+
 */

#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_

#include "../CProjectDfn.h"

/**
 * @brief To build a available memory list.
 */
typedef union Node
{
	union Node *pNext;     ///< Next available memory list.
	char data[1];          ///< Address of this union structure.
}Node_t;

/**
 * @brief Information about a memory pool.
 */
typedef struct Head
{
	unsigned int uBlockSize;    ///< Every memory block have this length, maximum length of string with '\0'.
	Node_t *pFirstAvailable;    ///< The first available memory block, if NULL, no available block.
}Head_t;

/**
 * @brief Information of a memory pool.
 */
typedef Head_t MemoryPool_t;

/**
 * @brief Create a empty pool.
 *
 *   Each block in this pool have minimum size of sizeof(union Node), will up to it if smaller than.
 *
 * @param uBlockSize Every memory block have this length, maximum length of string with '\0' can be in.
 * @return Created memory pool.
 */
MemoryPool_t *CreateMemoryPool(unsigned int uBlockSize);

/**
 * @brief Destroy a memory pool.
 *
 * @param pPoll when finished, this will be NULL.
 * @note Make sure every address get from this memory pool is released by Free().
 */
void DestroyMemoryPool(MemoryPool_t **pPool);

/**
 * @brief Get a block from memory pool.
 *
 *   This block got from memory pool will out of pool's control, unless back it to pool.
 *
 * @param pPool Which pool to get from, there maybe many pools can get different size of memory block.
 * @return Address of not used memory block.
 */
inline void *Malloc(MemoryPool_t *pPool)
{
	assert(NULL != pPool);
	void *pPtr = NULL;

	if (NULL != pPool->pFirstAvailable)
	{
		pPtr = &(pPool->pFirstAvailable->data);
		pPool->pFirstAvailable = pPool->pFirstAvailable->pNext;
	}
	else
	{
		pPtr = malloc(pPool->uBlockSize);
		if (NULL == pPtr)
		{
			PrintError("Failed to malloc memory from system.");
		}
	}

	return pPtr;
}

/**
 * @brief Back a memory block to pool.
 *
 * @param pPool Which pool to back.
 * @param pPtr Address of memory block.
 * @note Make sure memory pool didn't been destroy, if already, it will free this block to system.
 */
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
