/*
 * MemoryPoolTester.c
 *
 *  Created on: Oct 19, 2011
 *      Author: wangliang
 */

#include "MemoryPool.h"
#include <time.h>

void GenerateRandStr(char *str, int strLen)
{
  srand(time(NULL) + rand());
  int i;
  for (i = 0; i  < strLen; i++)
  {
    str[i] = rand() %94 + 32;
  }
  str[i] = '\0';
}

int main()
{
	int ret;

#if defined(TEST_FULMemoryPool)
	ret = FULMemoryPoolTester();
#elif defined (TEST_VULMemoryPool)
	ret = VULMemoryPoolTester();
#elif defined (TEST_FALMemoryPool)
	ret = FALMemoryPoolTester();
#elif defined (TEST_VALMemoryPool)
	ret = VALMemoryPoolTester();
#elif defined (TEST_FUBMemoryPool)
	ret = FUBMemoryPoolTester();
#elif defined (TEST_VUBMemoryPool)
	ret = VUBMemoryPoolTester();
#elif defined (TEST_FABMemoryPool)
	ret = FABMemoryPoolTester();
#elif defined (TEST_VABMemoryPool)
	ret = VABMemoryPoolTester();
#else
	printf("Please define one of macros to enable related memory pool tester, define it in MemoryPool.h\n");
#endif

	return ret;
}
