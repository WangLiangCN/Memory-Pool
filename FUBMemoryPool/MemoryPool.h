/**
 * @file   MemoryPool.h
 *
 * @date   Oct 26, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Fixed length, Unable to recycle, Block store style memory pool.
 *
 * Data structure:
 *
 *    MemoryPool_t                 MemoryChunk_t                MemoryChunk_t              MemoryChunk_t
 * +-----------------+   ---->  +-----------------+   ---->  +-----------------+    ---->     ......
 * |   uBlockSize    |   |      |uBlocksAvailable_|   |      |uBlocksAvailable_|    |
 * +-----------------+   |      +-----------------+   |      +-----------------+    |
 * |uFirstChunkBlocks|   |  --- |uFirstAvailable_ |   |  --- |uFirstAvailable_ |    |
 * +-----------------+   |  |   +-----------------+   |  |   +-----------------+    |
 * |uGrowChunkBlocks |   |  |   |     uBlocks     |   |  |   |     uBlocks     |    |
 * +-----------------+   |  |   +-----------------+   |  |   +-----------------+    |
 * |   pFirstChunk   | ---  |   |    pNextChunk   | ---  |   |    pNextChunk   |  ---
 * +-----------------+      |   +-----------------+      |   +-----------------+
 *                          |   |ThisBlockIsUsingN|      |   |ThisBlockIsUsingN|
 *                          |   +-----------------+      |   +-----------------+
 *                          |   |ThisBlockIsUsingN|      --> |I|   Block[x]    | ---
 *                          |   +-----------------+          +-----------------+   |
 *                          --> |I|   Block[m]    | ---  --> |I|   Block[x+1]  |   |
 *                              +-----------------+   |  |   +-----------------+   |
 *                              |ThisBlockIsUsingN|   |  |   |ThisBlockIsUsingN|   |
 *                              +-----------------+   |  |   +-----------------+   |
 *                              |ThisBlockIsUsingN|   |  |   |     ......      |   |
 *                              +-----------------+   |  |   +-----------------+   |
 *                              |     ......      |   |  --- |I|   Block[y+1]  | <--
 *                              +-----------------+   |      +-----------------+
 *                              |I|  Block[n]     | <--
 *                              +-----------------+
 *                              |ThisBlockIsUsingN|
 *                              +-----------------+
 */

#ifndef MEMORYPOOL_H_
#define MEMORYPOOL_H_

#include "../CProjectDfn.h"
#include <limits.h>

/**
 * @brief Max length of string pool can allocate, if bigger than this when creating, down to this size.
 */
#ifndef USHRT_MAX
#define USHRT_MAX 65535
#endif
#define MAX_STRING_LEN USHRT_MAX

/**
 * @brief Memory chunk information, a chunk includes many blocks, every allocation operation from memory
 * pool will return a block, many chunks make up a list, when there is no available blocks in all chunk,
 * needs to create a new chunk. In this pool, blocks is following this chunk structure, so we can compute
 * address of first block by address of chunk. The first sizeof(unsigned short) bytes saves the index of
 * next available block index, by index we can compute the address of block.
 */
typedef struct MemoryChunk
{
	unsigned short uBlocksAvailable_;  ///< How many blocks available in this chunk.
	unsigned short uFirstAvailable_;   ///< The index of first available chunk.
	unsigned short uBlocks;            ///< Total size of blocks in this chunk, related to number of blocks.
	struct MemoryChunk *pNextChunk;    ///< Pointer to next chunk, this make up a chunk list.
}MemoryChunk_t;

/**
 * @brief Information of memory pool.
 */
typedef struct MemoryPool
{
	unsigned short uBlockSize;         ///< Size of each block in pool.
	unsigned short uFirstChunkBlocks;  ///< Number of blocks in first chunk.
	unsigned short uGrowChunkBlocks;   ///< When first chunk is full, extend a new chunk have such blocks.
	MemoryChunk_t *pFirstChunk;        ///< Pointer to first chunk.
}MemoryPool_t;

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
extern MemoryPool_t *CreateMemoryPool(unsigned short _uBlockSize, unsigned short _uFirstChunkBlocks,
		                       unsigned short _uGrowChunkBlocks);

/**
 * @brief Destroy memory pool.
 *
 *   Make sure all memory block allocated from pool won't use again, it will release all blocks to system.
 * @param pPool Which pool to destroy.
 */
extern void DestroyMemoryPool(MemoryPool_t **pPool);

/**
 * @brief Allocate memory from pool, pool will return a memory block have maximum size, this size is
 * given when create pool.
 *
 * @param pPool Get memory block from which pool.
 * @return Memory block allocated from pool.
 */
extern void *Malloc(MemoryPool_t *pPool);

/**
 * @brief Back a memory block to memory pool.
 */
extern void Free(MemoryPool_t *pPool, void *pPtr);

#endif /* MEMORYPOOL_H_ */
