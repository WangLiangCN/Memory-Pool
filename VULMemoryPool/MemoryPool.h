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

#ifndef USHRT_MAX
#define USHRT_MAX 65535
#endif
#define MAX_STRING_LEN USHRT_MAX

/**
 * @brief Align size, must be 2^n
 */
#define ALIGN_SIZE 8

typedef union Node
{
	union Node *pNext;
	char data[1];
}Node_t;

typedef Node_t* BlockTable_t;

typedef struct BigBlock
{
	void *data;
	struct BigBlock *pNext;
	struct BigBlock *pPre;
}BigBlock_t;

typedef struct MemoryPoolInf
{
	unsigned int uMaxSize;
	BlockTable_t *pTable;
	BigBlock_t *pFirstBigBlock;
}MemoryPool_t;

/**
 * @brief Align function.
 */
inline unsigned short RoundUp(unsigned short size)
{
	return (((size) + (ALIGN_SIZE - 1)) & ~(ALIGN_SIZE - 1));
}

/**
 * @brief Get index.
 */
inline unsigned short GetIndex(unsigned short size)
{
	return (((size) + (ALIGN_SIZE - 1)) / (ALIGN_SIZE - 1) - 1);
}

MemoryPool_t *CreateMemoryPool(unsigned short uMaxStrLen);
void DestroyMemoryPool(MemoryPool_t **pPool);
void *Malloc(MemoryPool_t *pPool, unsigned short uSize);
void Free(MemoryPool_t *pPool, void *pPtr);

#endif /* MEMORYPOOL_H_ */
