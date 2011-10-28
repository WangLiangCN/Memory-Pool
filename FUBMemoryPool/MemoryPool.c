/**
 * @file   MemoryPool.c
 *
 * @date   Oct 26, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief
 */

#include "MemoryPool.h"

/**
 * @brief Create memory pool, when no room in pool, it will grow more automatically.
 *
 * @param _uBlockSize          Size of each block in pool, maximum size of memory can get from pool,
 *                             Maximum string the pool can allocate is MAX_STRING_LEN.
 * @param _uFirstChunkBlocks   The number of blocks in memory pool when first allocate, if all used out,
 *                             it will grow [_uGrowChunkBlocks] more every time if all blocks used out.
 * @param _uGrowChunkBlocks    When all blocks used out, it will grow more as this. If 0, forbidden
 *                             to grow even no blocks available, return NULL when allocate.
 * @return Created memory pool, NULL if failed to allocate memory from system.
 */
MemoryPool_t *CreateMemoryPool(unsigned short _uBlockSize, unsigned short _uFirstChunkBlocks,
		                       unsigned short _uGrowChunkBlocks)
{
	MemoryPool_t *pPool = (MemoryPool_t *)malloc(sizeof(MemoryPool_t));
	if (NULL == pPool)
	{
		return NULL;
	}

	// Because needs sizeof(unsigned short) bytes to save index, so that ensure that bigger than this bytes.
	_uBlockSize = (_uBlockSize > sizeof(unsigned short)) ? _uBlockSize : sizeof(unsigned short);
	pPool->uBlockSize = _uBlockSize;
	pPool->pFirstChunk = NULL;
	pPool->uFirstChunkBlocks = _uFirstChunkBlocks;
	pPool->uGrowChunkBlocks = _uGrowChunkBlocks;

	return pPool;
}

/**
 * @brief Destroy memory pool.
 *
 *   Make sure all memory block allocated from pool won't use again, it will release all blocks to system.
 * @param pPool Which pool to destroy.
 */
void DestroyMemoryPool(MemoryPool_t **pPool)
{
	MemoryChunk_t *pCurrChunk = (*pPool)->pFirstChunk;
	MemoryChunk_t *pPreChunk = NULL;

	// Destroy all chunks.
	while(NULL != pCurrChunk)
	{
		pPreChunk = pCurrChunk;
		pCurrChunk = pCurrChunk->pNextChunk;
		free(pPreChunk);
	}

	free(*pPool);
	(*pPool) = NULL;
}

/**
 * @brief Get the address of first block in chunk.
 *
 *   In this memory pool, block is following chunk structure.
 * @param pChunk Get first block from which chunk.
 */
inline void *GetFirstBlockFromChunk(MemoryChunk_t *pChunk)
{
	return ((void *)pChunk + sizeof(MemoryChunk_t));
}

/**
 * @brief When user first allocate memory from pool, or all blocks in pool is used out, needs to create
 * a new chunk, so that memory pool have more blocks to gave to user.
 *
 * @param uBlocks Number of blocks this chunks contains.
 * @param uBlockSize Size of blocks in this chunk.
 * @return Created and initialized chunk.
 */
inline MemoryChunk_t *AllocateNewChunkInit(unsigned short uBlocks, unsigned short uBlockSize)
{
	MemoryChunk_t *pChunk = (MemoryChunk_t *)malloc(sizeof(MemoryChunk_t) + uBlocks * uBlockSize);
	if (NULL == pChunk)
	{
		return NULL;
	}

	pChunk->uBlocksAvailable_ = uBlocks;
	pChunk->uFirstAvailable_ = 0;
	pChunk->uBlocks = uBlocks;
	pChunk->pNextChunk = NULL;

	// Initialize this chunk, create idle index.
	void *pBlock = GetFirstBlockFromChunk(pChunk);
	for (int i = 0; i != uBlocks; pBlock += uBlockSize)
	{
		*(unsigned short *)pBlock = ++i;
	}

	return pChunk;
}

/**
 * @brief Allocate memory from pool, pool will return a memory block have maximum size, this size is
 * given when create pool.
 *
 * @param pPool Get memory block from which pool.
 * @return Memory block allocated from pool.
 */
void *Malloc(MemoryPool_t *pPool)
{
	void *pBlock = NULL;
	MemoryChunk_t *pAvailableChunk = pPool->pFirstChunk;

	// If no chunk in pool, create it.
	if (NULL == pAvailableChunk)
	{
		pPool->pFirstChunk = AllocateNewChunkInit(pPool->uFirstChunkBlocks, pPool->uBlockSize);
		pAvailableChunk = pPool->pFirstChunk;
	}

	// Find a chunk which have available blocks.
	while(pAvailableChunk && !pAvailableChunk->uBlocksAvailable_)
	{
		pAvailableChunk = pAvailableChunk->pNextChunk;
	}

	// Found such a chunk have available blocks, return this block and update index.
	if (NULL != pAvailableChunk)
	{
		pBlock = GetFirstBlockFromChunk(pAvailableChunk);
		pBlock += pAvailableChunk->uFirstAvailable_ * pPool->uBlockSize;
		pAvailableChunk->uFirstAvailable_ = *(unsigned short *)pBlock;
		-- pAvailableChunk->uBlocksAvailable_;
	}
	else
	{
		// All chunk is full, needs to create a new chunk.
		// Check if forbidden to extend memory pool, if so, return NULL.
		if (!pPool->uGrowChunkBlocks)
		{
			PrintWarning("No blocks in pool and not allowed to automatically grow.");
			return NULL;
		}

		// Create a new chunk, and return it's first block.
		pAvailableChunk = AllocateNewChunkInit(pPool->uGrowChunkBlocks, pPool->uBlockSize);
		if (NULL == pAvailableChunk)
		{
			PrintError("Allocate memory from system to extend pool failed.");
			return NULL;
		}
		// Insert this chunk at the beginning of list, because it have many available blocks.
		pAvailableChunk->pNextChunk = pPool->pFirstChunk;
		pPool->pFirstChunk = pAvailableChunk;

		// Return first block, update chunk index.
		pBlock = GetFirstBlockFromChunk(pAvailableChunk);
		pAvailableChunk->uFirstAvailable_ = *(unsigned short *)pBlock;
		-- pAvailableChunk->uBlocksAvailable_;
	}

	return pBlock;
}

/**
 * @brief Get the end address of a chunk.
 *
 * @param pPool Which pool is the chunk in.
 * @param pChunk Which the chunk's end address want to get.
 * @return End address of a chunk.
 */
inline void *GetEndOfChunk(MemoryPool_t *pPool, MemoryChunk_t *pChunk)
{
	return ((void *)GetFirstBlockFromChunk(pChunk) + pChunk->uBlocks * pPool->uBlockSize);
}

/**
 * @brief Check a block whether in given chunk and memory pool.
 *
 * @param pPool The chunk in which pool.
 * @param pChunk Check block if in this chunk.
 * @param pPtr Check this block.
 * @return If block in this chunk, 1 will be returned, or 0 is returned.
 */
inline char CheckInChunk(MemoryPool_t *pPool, MemoryChunk_t *pChunk, void *pPtr)
{
	char result = 1;
	result &= ((unsigned long)pPtr >= (unsigned long)GetFirstBlockFromChunk(pChunk));
	result &= ((unsigned long)pPtr < (unsigned long)GetEndOfChunk(pPool, pChunk));

	return result;
}

/**
 * @brief Back a memory block to memory pool.
 */
void Free(MemoryPool_t *pPool, void *pPtr)
{
	MemoryChunk_t *pChunk = pPool->pFirstChunk;

	// Check the block in which chunk.
	while(pChunk && !CheckInChunk(pPool, pChunk, pPtr))
	{
		pChunk = pChunk->pNextChunk;
	}

	// If memory block not belongs to any chunk.
	if (NULL == pChunk)
	{
		PrintWarning("Not found this memory block in pool.");
		return;
	}

	// Back the memory block to pool.
	++ pChunk->uBlocksAvailable_;
	*(unsigned short *)pPtr = pChunk->uFirstAvailable_;
	pChunk->uFirstAvailable_ = (unsigned short)
			(((unsigned long)pPtr - (unsigned long)GetFirstBlockFromChunk(pChunk)) / pPool->uBlockSize);
}
