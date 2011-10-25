/**
 * @file   VULMemoryPool/MemoryPool.h
 *
 * @date   Oct 20, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Variable length, Unable to recycle, List style memory pool.
 *
 * Data structure:
 *
 * MemoryPool_t             0      align   2*align   3*align   4*align   5*align           (n-1)*align
 * +--------+          ~align   ~2*align  ~3*align  ~4*align  ~5*align  ~6*align    ...    ~n*align
 * |        |       +---------+---------+---------+---------+---------+---------+---------+-----------+
 * | pTable |  -->  | NotUsed | NotUsed | NotUsed | NotUsed |   NULL  | NotUsed |   ...   |  NotUsed  |
 * |        |       +---------+---------+---------+---------+---------+---------+---------+-----------+
 * |        |           |          |         |         |                   |                    |
 * +--------+        +------+  +------+   +------+  +------+             NULL               +------+
 * |        |        | Not  |  | Not  |   | Not  |  | Not  |                                | Not  |
 * |  uMax  |        | Used |  | Used |   | Used |  | Used |                                | Used |
 * |  Size  |        +------+  +------+   +------+  +------+                                +------+
 * |        |           |          |         |         |                                        |
 * +--------+        +------+    NULL      NULL                                               NULL
 * |        |        | Not  |                         ...
 * |  pBig  |        | Used |
 * |  Block |----    +------+                          |
 * |        |   |       |                           +------+
 * +--------+   |     NULL                          | Not  |
 *              |                                   | Used |
 *              |                                   +------+
 *              |                                      |
 *              |                                     NULL
 *              |
 *              |
 *              |     +-------+    +-------+    +-------+              +-------+
 *              ----> | Using | -- | Using | -- | Using | --  ...  --  | Using |  --  NULL
 *                    +-------+    +-------+    +-------+              +-------+
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
 * @brief Align size, must be 2^n
 */
#define ALIGN_SIZE 8

/**
 * @brief Allocate size of memory, if it not used, let the first four block save the pointer pointed to
 * next free allocated memory, if this memory are using, [data] is the first address of this memory.
 */
typedef union Node
{
	union Node *pNext;    ///< If this memory is idle, this pointed to next free memory block.
	char data[1];         ///< If this memory is using, this is the address of this block.
}Node_t;

/**
 * @brief Block table is an array, each elements pointed to a list which describes the free memory block.
 */
typedef Node_t* BlockTable_t;

/**
 * @brief Memory pool have it's biggest size, if user asked to allocate a big block bigger than memory
 * can be, pool will deliver this command to system and record this, when destroy the pool, all allocated
 * big block will be released.
 */
typedef struct BigBlock
{
	void *data;               ///< Start address of allocated big block.
	struct BigBlock *pNext;   ///< Next allocated big block if this not the last one.
	struct BigBlock *pPre;    ///< Previous allocated big block if that exists.
}BigBlock_t;

/**
 * @biref Information about memory pool.
 */
typedef struct MemoryPoolInf
{
	unsigned int uMaxSize;       ///< Longest block memory pool can allocate, if bigger, deliver to system.
	BlockTable_t *pTable;        ///< An array, each pointed to a list which describes the free memory block.
	BigBlock_t *pFirstBigBlock;  ///< If bigger than pool can allocate, pointed to list which contains them.
}MemoryPool_t;

/**
 * @brief Align function, convert it to aligned size.
 */
inline unsigned short RoundUp(unsigned short size)
{
	return (((size) + (ALIGN_SIZE - 1)) & ~(ALIGN_SIZE - 1));
}

/**
 * @brief Get index from block table by given size.
 */
inline unsigned short GetIndex(unsigned short size)
{
	return (((size) + (ALIGN_SIZE - 1)) / (ALIGN_SIZE - 1) - 1);
}

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
MemoryPool_t *CreateMemoryPool(unsigned short uMaxStrLen);

/**
 * @brief Destroy memory pool, release all idle memory block smaller than max size pool can allocate, and
 * release all blocks bigger than this size not matter using or not.
 *
 * @param pPool Which pool to destroy, set to NULL when finished to destroy.
 */
void DestroyMemoryPool(MemoryPool_t **pPool);

/**
 * @biref Get a memory block from pool.
 *
 * @param pPool From which pool to get.
 * @param uSize Size of string want to allocate.
 * @return Allocated memory.
 */
void *Malloc(MemoryPool_t *pPool, unsigned short uSize);

/**
 * @brief Back a memory block to pool so that it can be use again.
 *
 * @param pPool Back to which pool.
 * @param pPtr Address of memory block to back.
 */
void Free(MemoryPool_t *pPool, void *pPtr);

#endif /* MEMORYPOOL_H_ */
