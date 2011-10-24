/*
 * @file   MemoryPoolTester.c
 *
 * @date   Oct 19, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Test all kind of memory pool.
 */

#include "MemoryPoolTester.h"
#include <time.h>
#include <sys/time.h>

int SystemDefaultAllocatorTest_FixedLen()
{
	// To compute used time.
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;
	// To store available memory address got from system or pool.
	char **pStrings = (char **)malloc(sizeof(char *) * TEST_MALLOC_TIMES);

	// System default malloc/free test.
	PrintLog("Now testing system default malloc/free, fixed length.");
	gettimeofday(&startTime, NULL);
	for (int i=0; i<TEST_RETRY_TIMES; ++i)
	{
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			pStrings[j] = (char *)malloc(sizeof(char) * MALLOC_MAX_LEN);
			*pStrings[j] = '\0';
			//GenerateRandStr(pStrings[j], MALLOC_MAX_LEN-1);
			free(pStrings[j]);
		}
	}
	gettimeofday(&endTime, NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("System default malloc/free tested, malloc and free %d strings for %d times, cost %llu us.\n",
			TEST_MALLOC_TIMES, TEST_RETRY_TIMES, costTime);

	free(pStrings);
	return 0;
}

int SystemDefaultAllocatorTest_VarLen()
{
	// To compute used time.
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;

	// To store available memory address got from system or pool.
	char **pStrings = (char **)malloc(sizeof(char *) * TEST_MALLOC_TIMES);
	// Generate variable length of string.
	int aStrLen[TEST_MALLOC_TIMES];
	srand((unsigned int)time(NULL) + rand());
	for (int i=0; i<TEST_MALLOC_TIMES; ++i)
	{
		aStrLen[i] = rand() % MALLOC_MAX_LEN + 1;
		srand(aStrLen[i]);
	}

	// System default malloc/free test.
	PrintLog("Now testing system default malloc/free, variable length.");
	gettimeofday(&startTime, NULL);

	for (int i=0; i<TEST_RETRY_TIMES; ++i)
	{
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			pStrings[j] = malloc(sizeof(char)*(aStrLen[j] + 1));
			*pStrings[j] = '\0';
			free(pStrings[j]);
		}
	}

	gettimeofday(&endTime, NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("System default malloc/free tested, malloc and free %d strings for %d times, cost %llu us.\n",
			TEST_MALLOC_TIMES, TEST_RETRY_TIMES, costTime);

	free(pStrings);
	return 0;
}

int main()
{
	int ret;

#if defined(ENABLE_FULMemoryPool) || defined (ENABLE_FALMemoryPool) || defined (ENABLE_FABMemoryPool)
	SystemDefaultAllocatorTest_FixedLen();
#else
	SystemDefaultAllocatorTest_VarLen();
#endif

#if defined(ENABLE_FULMemoryPool)
	ret = FULMemoryPoolTester();
#elif defined (ENABLE_VULMemoryPool)
	ret = VULMemoryPoolTester();
#elif defined (ENABLE_FALMemoryPool)
	ret = FALMemoryPoolTester();
#elif defined (ENABLE_VALMemoryPool)
	ret = VALMemoryPoolTester();
#elif defined (ENABLE_FUBMemoryPool)
	ret = FUBMemoryPoolTester();
#elif defined (ENABLE_VUBMemoryPool)
	ret = VUBMemoryPoolTester();
#elif defined (ENABLE_FABMemoryPool)
	ret = FABMemoryPoolTester();
#elif defined (ENABLE_VABMemoryPool)
	ret = VABMemoryPoolTester();
#else
	PrintLog("Please define one of macros to enable related memory pool tester, please define it in MemoryPool.h\n");
#endif

	return ret;
}
