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
	unsigned int uAvailable;
	Node_t *pFirstAvailable;
}Head_t;

typedef Head_t MemoryPool_t;

#endif /* MEMORYPOOL_H_ */
