/**
 * @file   MemoryPoolTester.h
 *
 * @date   Oct 19, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief Definition to test all kind of memory pool.
 */

#ifndef MEMORY_POOL_TESTER_H
#define MEMORY_POOL_TESTER_H

#include "CProjectDfn.h"
#include <time.h>

/**
 * @brief Test one kind of memory pool, enable it and program will test this kind of pool.
 * @note Enable one of them at one time.
 */
#define TEST_FULMemoryPool
//#define TEST_VULMemoryPool
//#define TEST_FALMemoryPool
//#define TEST_VALMemoryPool
//#define TEST_FUBMemoryPool
//#define TEST_VUBMemoryPool
//#define TEST_FABMemoryPool
//#define TEST_VABMemoryPool

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

#endif /* MEMORY_POOL_TESTER_H */
