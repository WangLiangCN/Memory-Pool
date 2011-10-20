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

#include "../MemoryPool.h"

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
void *Malloc(MemoryPool_t *pPool);
void Free(MemoryPool_t *pPool, void *pPtr);

#endif /* MEMORYPOOL_H_ */
