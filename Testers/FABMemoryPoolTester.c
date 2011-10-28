/**
 * @file   FABMemoryPoolTester.c
 *
 * @date   Oct 28, 2011
 * @author WangLiang
 * @email  WangLiangCN@live.com
 *
 * @brief  Tester program for FAB memory pool, for test purpose only.
 */

#include "../FABMemoryPool/MemoryPool.h"
#include "../MemoryPoolTester.h"
#include <sys/time.h>

#ifdef ENABLE_FABMemoryPool

/**
 * @brief Blocks of first chunk, so that first chunk in pool can provide these blocks.
 */
#define FIRST_CHUNK_BLOCKS 99

/**
 * @brief Blocks of other chunks except first chunk, when blocks in first chunk used out, create this.
 */
#define GROW_CHUNK_BLOCKS 64

/**
 * @brief Ordering test or random test, enable one of them in one time.
 *
 *   When ordering test, allocate and immediately free, repeat it, but it can't test weather pool works well.
 *   When random test, allocate and free operation is in random, but the print timer is not accurate.
 */
#define ORDERING_TEST
//#define RANDOM_TEST

/**
 * @brief Tester for FULMemoryPool, as the order, allocate and free, repeat this progress.
 */
int FABMemoryPoolOrderingTester()
{
	// To compute used time.
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;

	// To store available memory address got from system or pool.
	char **pStrings = (char **)malloc(sizeof(char *) * TEST_MALLOC_TIMES);

	// Memory pool Malloc/Free test.
	PrintLog("Now testing memory pool Malloc/Free, FAB memory pool.");
	gettimeofday(&startTime, NULL);

	MemoryPool_t *pPool = CreateMemoryPool(MALLOC_MAX_LEN, FIRST_CHUNK_BLOCKS, GROW_CHUNK_BLOCKS);
	for (int i=0; i<TEST_RETRY_TIMES; ++i)
	{
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			pStrings[j] = (char *)Malloc(pPool);
			*pStrings[j] = '\0';
			//GenerateRandStr(pStrings[j], MALLOC_MAX_LEN-1);
			Free(pPool, pStrings[j]);
		}
		/*for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			Free(pPool, pStrings[j]);
		}*/
	}
	DestroyMemoryPool(&pPool);

	gettimeofday(&endTime, NULL);
	costTime = 1000 * 1000 * (endTime.tv_sec - startTime.tv_sec) + endTime.tv_usec - startTime.tv_usec;
	printf("Memory pool Malloc/Free tested, malloc and free %d strings for %d times, cost %llu us.\n",
			TEST_MALLOC_TIMES, TEST_RETRY_TIMES, costTime);

	free(pStrings);
	return 0;
}

/**
 * @brief Tester for FULMemoryPool, allocate, random free and random allocate again and again.
 * @note  This test is just for function test, test function works well in every condition,time test
 * is meaningless.
 */
int FABMemoryPoolRandomTester()
{
	// To compute used time.
	struct timeval startTime, endTime;
	unsigned long long costTime = 0ULL;

	// To store available memory address got from system or pool.
	char **pStrings = (char **)malloc(sizeof(char *) * TEST_MALLOC_TIMES);
	for (int i=0; i<TEST_MALLOC_TIMES; ++i)
	{
		pStrings[i] = NULL;
	}
	// Generate variable length of string.
	int aRandom[TEST_MALLOC_TIMES];
	srand((unsigned int)time(NULL) + rand());
	for (int i=0; i<TEST_MALLOC_TIMES; ++i)
	{
		aRandom[i] = rand() % MALLOC_MAX_LEN;
		srand(aRandom[i]);
	}

	// Memory pool Malloc/Free test.
	PrintLog("Now testing memory pool Malloc/Free, FAB memory pool.");
	PrintLog("Note: This test is just for function test, time test is meaningless.");
	gettimeofday(&startTime, NULL);

	MemoryPool_t *pPool = CreateMemoryPool(MALLOC_MAX_LEN, FIRST_CHUNK_BLOCKS, GROW_CHUNK_BLOCKS);
	for (int i=0; i<TEST_RETRY_TIMES; ++i)
	{
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			if (aRandom[j] % 2)
			{
				pStrings[j] = (char *)Malloc(pPool);
				*pStrings[j] = '\0';
			}
		}
		for (int j=0; j<TEST_MALLOC_TIMES; ++j)
		{
			if ((aRandom[j] % 3) && pStrings[j])
			{
				Free(pPool, pStrings[j]);
				pStrings[j] = NULL;
			}
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

/**
 * @brief Tester for FULMemoryPool.
 */
int FABMemoryPoolTester()
{
#ifdef ORDERING_TEST
	FABMemoryPoolOrderingTester();
#else
	FABMemoryPoolRandomTester();
#endif
	return 0;
}

#endif /* ENABLE_FABMemoryPool */

