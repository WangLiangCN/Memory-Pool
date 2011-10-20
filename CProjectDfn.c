/**
 * @file   jni/engine/CProjectDfn.cpp
 *
 * @date   sep 30, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Definition for all C project on UNIX/Linux platform.
 */

#include "CProjectDfn.h"

#ifdef _DEBUGMODEON

// Count for memory allocated times, for memory leak check.
uint allocateCount = 0;
// Count for memory freed times, for memory leak check.
uint freeCount = 0;

/**
 * @brief Self defined memory allocate function, print allocated size in which function and line number,
 * to prevent memory leak.
 * @param uSize Size want to allocate.
 * @param pszFunc Function name, allocate memory in this function.
 * @param uLine Allocate memory at this line in source file.
 * @return Pointer pointed to new allocated memory.
 */
void *my_malloc(unsigned int uSize, const char *pszFunc, unsigned int uLine)
{
	void *pMem = NULL;

	pMem = malloc(uSize);
	++allocateCount;
	if (IS_NOT_NULL(pMem))
	{
		//printf("MALLOC at %p, In func:%s, line:%d, allocated %d bytes.", pMem, pszFunc, uLine, uSize);
	}
	else
	{
		printf("Allocate memory failed!");
	}

	return pMem;
}

/**
 * @brief Self defined free memory function, print freed in which function and line number, to prevent
 * memory leak.
 * @param pszFunc Function name, free memory in this function.
 * @param uLine Free memory at this line in source file.
 */
void my_free(void *pPtr, const char *pszFunc, unsigned int uLine)
{
	++freeCount;
	//printf("FREE at %p, In func:%s, line:%d", pPtr, pszFunc, uLine);
	free(pPtr);
}

/**
 * @breif Reset counter for allocate memory and release memory.
 */
void ResetMemoryCounter()
{
	allocateCount = 0;
	freeCount = 0;
}

/**
 * @brief Output memory management information, allocated times and freed times, for memory leak check.
 */
void PrintMemoryManagementInf()
{
	printf("Memory management information: Allocated %d times, Freed %d times.", allocateCount, freeCount);
	ResetMemoryCounter();
}

#endif

/**
 * @brief Print log, different functions in different platform.
 */
void PrintLog(const char *msg)
{
	printf("%s\n", msg);
}

/**
 * @brief Print warning message.
 */
void PrintWarning(const char *msg)
{
	printf(":::WARNING::: %s\n", msg);
}

/**
 * @brief Print error message.
 */
void PrintError(const char *msg)
{
	printf(":::ERROR::: %s\n", msg);
}

/**
 * @brief Check [path] is ended by '/', if not, add it.
 *
 *   To add it is because separate directory name and parent directory name. [Path] should have enough
 * space to store '/' at the end.
 *
 * @param path String want to check with.
 */
void AddSlashAtPathEnd(char* path)
{
	// Check if ended by '/'.
	if (PATH_DIDIVDER != path[strlen(path)-1])
	{
		// Not ended by '/', add it at the end of [path] manually and return.
		path[strlen(path)+1] = CHAR_ARRAY_END;
		path[strlen(path)] = PATH_DIDIVDER;
	}
}

/**
 * @brief Check [path] is ended by '/', if yes, delete it.
 *
 * @param path String want to check with.
 */
void RemoveSlashAtPathEnd(char *path)
{
	// Get the last position.
	int lastPosition = strlen(path)-1;

	if (PATH_DIDIVDER == path[lastPosition])
	{
		path[lastPosition] = CHAR_ARRAY_END;
	}
}

/**
 * @brief Check [path1] is same to [path2], no matter it is ended by slash or not.
 * @return If same path, YES will be returned, or NO will be returned.
 */
bool_t isSamePath(const char *path1, const char *path2)
{
	char pPath1[PATH_LEN(path1)+1];
	char pPath2[PATH_LEN(path2)+1];

	PATH_COPY(pPath1, path1);
	PATH_COPY(pPath2, path2);
	RemoveSlashAtPathEnd(pPath1);
	RemoveSlashAtPathEnd(pPath2);
	return IS_SAME_STRING(pPath1, pPath2);
}
