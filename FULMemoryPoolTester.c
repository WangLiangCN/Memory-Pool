/*
 * FULMemoryPoolTester.c
 *
 *  Created on: Oct 19, 2011
 *      Author: user
 */

#include "FULMemoryPool/MemoryPool.h"

#ifndef DEFAULT_STRING_SIZE
#define DEFAULT_STRING_SIZE 4096
#endif

int FULMemoryPoolTester()
{
	MemoryPool_t *pPool = CreateMemoryPool(DEFAULT_STRING_SIZE);

	return 0;
}
