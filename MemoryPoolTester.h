/**
 * @file   MemoryPoolTester.h
 *
 * @date   Oct 19, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Definition to test all kind of memory pool.
 */

#ifndef MEMORY_POOL_TESTER_H
#define MEMORY_POOL_TESTER_H

#include "CProjectDfn.h"
#include <time.h>

#define MALLOC_MAX_LEN (1*1024)

#define TEST_MALLOC_TIMES 9999
#define TEST_RETRY_TIMES 9999

/**
 * @brief Test one kind of memory pool, enable it and program will test this kind of pool.
 * @note Enable one of them at one time.
 */
//#define ENABLE_FULMemoryPool
#define ENABLE_VULMemoryPool
//#define ENABLE_FALMemoryPool
//#define ENABLE_VALMemoryPool
//#define ENABLE_FUBMemoryPool
//#define ENABLE_VUBMemoryPool
//#define ENABLE_FABMemoryPool
//#define ENABLE_VABMemoryPool

/**
 * @brief Make a random string.
 *
 * @param str Where to save generated string.
 * @param strLen Length of string want to generate.
 */
inline void GenerateRandStr(char *str, int strLen)
{
  srand(time(NULL) + rand());
  int i = 0;
  for (; i  < strLen; i++)
  {
    str[i] = rand() %94 + 32;
  }
  str[i] = '\0';
}

extern int FULMemoryPoolTester();
extern int VULMemoryPoolTester();

#endif /* MEMORY_POOL_TESTER_H */
