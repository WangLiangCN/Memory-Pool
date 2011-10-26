/**
 * @file   FULMemoryPoolTester.c
 *
 * @date   Oct 19, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 * @brief  Test program for FULMemoryPool.
 */

#include "../FULMemoryPool/MemoryPool.h"
#include "../MemoryPoolTester.h"
#include <sys/time.h>

#ifdef ENABLE_FULMemoryPool

/**
 * @brief Tester for FULMemoryPool.
 */
int FULMemoryPoolTester()
{
	// To compute used time.
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;

	// To store available memory address got from system or pool.
	char **pStrings = (char **)malloc(sizeof(char *) * TEST_MALLOC_TIMES);

	// Memory pool Malloc/Free test.
	PrintLog("Now testing memory pool Malloc/Free, FUL memory pool.");
	gettimeofday(&startTime, NULL);

	MemoryPool_t *pPool = CreateMemoryPool(MALLOC_MAX_LEN);
	for (int i=0; i<TEST_RETRY_TIMES; ++i)
	{
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			pStrings[j] = (char *)Malloc(pPool);
			*pStrings[j] = '\0';
			//GenerateRandStr(pStrings[j], MALLOC_MAX_LEN-1);
			Free(pPool, pStrings[j]);
		}
	}
	DestroyMemoryPool(&pPool);

	gettimeofday(&endTime, NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Memory pool Malloc/Free tested, malloc and free %d strings for %d times, cost %llu us.\n",
			TEST_MALLOC_TIMES, TEST_RETRY_TIMES, costTime);

	free(pStrings);
	return 0;
}

#endif /* ENABLE_FULMemoryPool */
