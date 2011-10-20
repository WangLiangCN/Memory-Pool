/**
 * @file   CProjectDfn.h
 *
 * @date   Jul 26, 2011
 * @author WangLiang
 * @email  liang.wang@elektrobit.com
 *
 * @brief  Universal definition for C project.
 */

#ifndef CPROJECTDFN_H_
#define CPROJECTDFN_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Whether to use debug mode, if use it, maybe output some debug information.
 * @note Cancel define it when final release, or it may take some extra CPU time.
 * @note Just defined for output some debug message, but don't put any useful program in it.
 */
#define _DEBUGMODEON

//+++++++++++++++++++++++++++++++++++++  Header files  +++++++++++++++++++++++++++++++++++++
/**
 * @brief Here lists some frequently used header file.
 * @note Not frequently used header file better not to put here to keep compile performance.
 */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

//+++++++++++++++++++++++++++++++++  Better meaning types  +++++++++++++++++++++++++++++++++
/**
 * @brief Here lists some better meaning types.
 * @note Only designed for C/C++ project, check all of them before using.
 */
typedef unsigned char boolean;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned int uint;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned long long ddword;
typedef unsigned int dword;
typedef unsigned short word;
// typedef unsigned char bool;     // Only designed for C project.
typedef unsigned char bool_t;
typedef signed long long int64;
typedef signed int int32;
typedef signed short int16;
typedef signed char int8;
typedef signed char byte;

//++++++++++++++++++++++++++  Common status for nature description  ++++++++++++++++++++++++++
/**
 *@brief Here lists some better meaning status.
 *@note Only designed for C/C++ project, check all of them before using.
 */
#define SUCCEED 0
#define FAILED -1
#define TRUE (!FALSE)
#define FALSE 0
#define YES 1
#define NO 0
#define NONE 0
#define HAVE 1
#define DISABLE 0
#define ABLE 1
#define SAME 0
#define EXIST 1
#define NOTEXIST 0
#define CHANGED 1
#define NOTCHANGED 0
#define POSITIVE 1
#define NEGATIVE 0
#define ZERO 0
#define FIRST 1
#define SECOND 2
#define THIRD 3

//++++++++++++++++++++++++++++++++  Frequently used characters ++++++++++++++++++++++++++++++++
//! End of char array.
#define CHAR_ARRAY_END '\0'

//! Definition for self directory and parent directory.
#define SELF_DIR "."
#define PARENT_DIR ".."

//! Path divider, level of directories.
#define PATH_DIDIVDER '/'

//! Get ready to track exception, if error happened, function will handle error after END_TRACK_EXCEPTION.
#define START_TRACK_EXCEPTION do{

//! Stop to track exception, if error happened, function will handle error after this line.
#define END_TRACK_EXCEPTION }while(0);

//! If error happened, use this macro to throw exception.
#define THROW_EXCEPTION break;

//! Extended macro, Declaring Attributes of Functions.
#ifndef __attribute_pure__
# define __attribute_pure__ //__attribute__ ((__pure__))
#endif
#ifndef __THROW
# define __THROW __attribute__ ((nothrow))
#endif
#ifndef __nonnull
# define __nonnull __attribute__((nonnull))
#endif
#ifndef __pnonnull
# define __pnonnull(params) __attribute__ ((__nonnull__ params))
#endif
#ifndef __attribute_malloc__
# define __attribute_malloc__ __attribute__ ((__malloc__))
#endif

//! Extended macro, Specifying Attributes of Variables.
#ifndef __attribute_aligned__
# define __attribute_aligned__ __attribute__ ((aligned))
#endif
#ifndef __attribute_packed__
# define __attribute_packed__ __attribute__ ((packed))
#endif

//! Default align style for structure/union.
#define __DEFAULT_ALIGN__ //__attribute_aligned__

//+++++++++++++++++++++++++++++++++  Memory management macros ++++++++++++++++++++++++++++++++++

#ifdef _DEBUGMODEON

//! Count for memory allocated times, for memory leak check.
extern uint allocateCount;
//! Count for memory freed times, for memory leak check.
extern uint freeCount;

//! Allocate memory and print log information, allocated size in which function and line number.
extern void *my_malloc(unsigned int uSize, const char *pszFunc, unsigned int uLine)
       __THROW __attribute_pure__ __attribute__ ((nonnull (2)));
//! Free memory and print log information, freed line number and function.
extern void my_free(void *pPtr, const char *pszFunc, unsigned int uLine)
       __THROW __attribute_pure__ __attribute__ ((nonnull (1, 2)));
//! Output memory management information, allocated times and freed times, for memory leak check.
extern void PrintMemoryManagementInf()
       __THROW __attribute_pure__;

//! Allocate memory and print log information, to prevent memory leak.
# define MALLOC(type, n)\
		(type *)my_malloc((n)*sizeof(type), __FUNCTION__, __LINE__)

//! Free memory and print log information, to prevent memory leak.
# define FREE(ptr)\
	do{\
		my_free(ptr, __FUNCTION__, __LINE__);\
		ptr = NULL;\
	}while(0)

#else

//! Allocate size of memory.
# define MALLOC(type, n) \
	((type *) malloc ((n)*sizeof(type)))

//! Release memory.
# define FREE(x) \
	do{\
		free(x);\
		x = NULL;\
	}while(0)

#endif

//! Initialize a char array, set the first element to 0.
#define INIT_CHAR_ARRAY(x) \
		((x)[0] = CHAR_ARRAY_END)

//! Fill array [x] with number [n], each element is array is [type] and fill [num] number of elements.
#define FILL_ARRAY(x, n, num, type)\
	(memset((x), (n), (sizeof(type)*(num))))

//! If pointer not been freed.
#define IS_NOT_FREED(x) \
	((NULL != (x)) ? YES : NO)

//! Security free allocated memory, check not been freed first, or directly return.
#define SECURE_FREE(x) \
	do{\
		if (IS_NOT_FREED(x))\
		{\
			FREE(x);\
		}\
	}while(0)

//! If pointer is NULL.
#define IS_NULL(x) \
	((NULL == (x)) ? YES : NO)

//! If pointer is not NULL.
#define IS_NOT_NULL(x) \
	IS_NOT_FREED(x)

//++++++++++++++++++++++++++++++++++  String compare macros  ++++++++++++++++++++++++++++++++++
//! Check x, y have same/different text in character array.
//! Compare with build-in function strcmp(2).
#define IS_SAME_STRING(x, y) \
	((SAME == strcmp(x, y)) ? YES : NO)

//! Check x, y have same/different length of text in character array.
//! Compare with build-in function strncmp(2).
#define IS_PART_SAME_STRING(x, y, len) \
	((SAME == strncmp(x, y, len)) ? YES : NO)

//! Get offset of filed in structure type.
#ifndef offsetof
# define offsetof(type, field) ((long) &((type *)0)->field)
#endif

//! Get structure address by a known address of field in structure.
//! const typeof(...) is to check ptr is really a field in structure, or give a warning by complier.
#ifndef container_of
# define container_of(ptr, type, member) ({\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);\
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

//+++++++++++++++++++++++++++++  Name and path management macros  +++++++++++++++++++++++++++++
//! name for path and name
typedef char PATH;
typedef char NAME;

//! Get length of path.
#define PATH_LEN(x)\
	strlen(x)
//! Get length of name.
#define NAME_LEN(x)\
	PATH_LEN(x)

//! Copy a path from [y] to [x].
#define PATH_COPY(x, y)\
	strcpy(x, y)
//! Copy a name from [y] to [x].
#define NAME_COPY(x, y)\
	PATH_COPY(x, y)

//! Add a slash if path is not ended by slash, or it did nothing.
extern void AddSlashAtPathEnd(char* path)
       __THROW __attribute_pure__ __attribute__ ((nonnull (1)));

//! If [path] is ended by slash, delete it, or it did nothing.
extern bool_t RemoveSlashAtPathEnd(char *path)
       __THROW __attribute_pure__ __attribute__ ((nonnull (1)));

//! Append path [y] to [x], make sure [x] have enough space to append [y].
#define PATH_CAT(x, y)\
	do{\
		AddSlashAtPathEnd(x);\
		strcat(x, y);\
	}while(0)

//! Check [path1] is same to [path2], no matter it is ended by slash or not.
bool_t isSamePath(const char * __restrict path1, const char * __restrict path2)
       __THROW __attribute_pure__ __attribute__ ((nonnull (1, 2)));

//! Is name is same.
#define IS_SAME_NAME(x, y)\
		IS_SAME_STRING(x, y)

//! Check path [subPath] is sub path of path [fullPath].
#define IS_SUB_PATH(path, subPath)\
	((SAME != strncmp(path, subPath, strlen(subPath))) && (SAME == strncmp(path, subPath, strlen(path))) ? YES : NO)

/**
 * @brief Generate a path, copy located directory, ensure it is ended by slash, and append name to it.
 *
 *   Combine [location] and [name] together, but it will add slash between automatically, save this path
 * to [saveTo].
 *
 * @note [saveTo] must have enough space to do this, if not, behavior is not defined.
 * @note At lest have length of [location], [name] and 2 extra bytes for add a slash if needed, and '\0'.
 */
#define GENERATE_PATH(saveTo, location, name)\
	do{\
       strcpy((saveTo), (location));\
       AddSlashAtPathEnd(saveTo);\
       strcat((saveTo), (name));\
	}while(0)

//+++++++++++++++++++++++++++  File and directory managements macros +++++++++++++++++++++++++++
/**
 * @brief Check if a directory or file exists.
 * @param x The full path of file or directory want to check with.
 * @return If exists, return EXIST, or return NOTEXIST.
 */
#define IS_EXIST(x) \
	((access((x), F_OK) == 0) ? EXIST : NOTEXIST)

//! Initialize a file descriptor, no file is opened.
#define INIT_FILE_DESCRIPTOR(x) \
	((x) = 0)

//! Open a file in read mode.
#define READ_FILE(x) \
	(open((x), O_RDONLY))


//! Create a new file and open it in write mode, truncate if exists.
#define CREATE_NEW_TRUNC_FILE(x) \
	(open((x), O_RDWR | O_CREAT | O_TRUNC, 0644))


//! Delete a file from file system, make sure it is closed before this operation.
#define DELETE_FILE(x) \
	(unlink(x))

//! Close an opened file by it's file descriptor.
#define CLOSE_FILE(x) \
	do{\
		close(x);\
		x = 0;\
	}while(0)

//! Check a file is closed or not by file descriptor.
#define IS_FILE_OPEN(x) \
		((0 != (x)) ? YES : NO)

//! Security close opened file by file descriptor, check not still been opened first, or directly return.
#define SECURE_CLOSE_FILE(x) \
	do{\
		if (IS_FILE_OPEN(x))\
		{\
			CLOSE_FILE(x);\
		}\
	}while(0)

//! Initialize a file pointer, no file is opened.
#define INIT_FILE_POINTER(x) \
	((x) = NULL)

//! Close an opened file by it's FILE pointer.
#define CLOSE_PFILE(x) \
	do{\
		fclose(x);\
		x = NULL;\
	}while(0)

//! Check a file is closed or not by FILE pointer.
#define IS_PFILE_OPEN(x) \
		((NULL != (x)) ? YES : NO)

//! Security close opened file by FILE pointer, check not still been opened first, or directly return.
#define SECURE_CLOSE_PFILE(x) \
	do{\
		if (IS_PFILE_OPEN(x))\
		{\
			CLOSE_PFILE(x);\
		}\
	}while(0)

//! Create a new directory, mode to 0755.
#define CREATE_NEW_DIR(x) \
	(mkdir((x), 0755))

//! Delete a empty directory from file system, make sure it is empty.
#define DELETE_EMPTY_DIR(x) \
	(rmdir(x))

//! Check is closed directory.
#define IS_DIR_OPEN(x)\
	((NULL == (x)) ? NO : YES)

//! Close an opened directory.
#define CLOSE_DIR(x)\
	do{\
		closedir(x);\
		x = NULL;\
	}while(0)

//! If directory not closed, close it, or skip.
#define SECURE_CLOSE_DIR(x)\
	do{\
		if (IS_DIR_OPEN(x))\
		{\
			CLOSE_DIR(x);\
		}\
	}while(0)

/**
 * @brief Check if directory name is self or it's parent directory.
 *
 *   Directory name is not . or ..
 *
 * @param name name want to check with.
 * @return YES will be returned if directory is self or it's parent directory, or NO will be returned.
 */
#define IsDirNameSelfOrParent(x) \
	(((IS_SAME_NAME((x), SELF_DIR)) || (IS_SAME_NAME((x), PARENT_DIR))) ? YES : NO)

//++++++++++++++++++++++++++++++++  Other frequently used macros  ++++++++++++++++++++++++++++++++
//! Do something until a condition is yes.
#define DO_UNTIL(x) while(!(x))

//! Swap two number, it can make addition and subtration.
#define swap(x, y)\
	do{\
		x = x + y;\
		y = x - y;\
		x = x - y;\
	}while(0)

/**
 * @brief Get the maximum/minimum number in two numbers, ++ operation in macro is supported.
 * @note About (void) (&_x == &_y), if x, y have different type, complier will give you a warning, convert
 * to void is to prevent complier warning you that variable not used.
 */
#define MIN(x, y)\
	({\
		const typeof(x) _x = (x);\
		const typeof(y) _y = (y);\
		(void) (&_x == &_y);\
		_x < _y ? _x : _y;\
	})
#define MAX(x, y)\
	({\
		const typeof(x) _x = (x);\
		const typeof(y) _y = (y);\
		(void) (&_x == &_y);\
		_x > _y ? _x : _y;\
	})

//! Calculates the absolute value of x. The absolute value is simply the number with any negative sign taken away.
#define ABS(x)\
	({\
		const typeof(x) _x = (x);\
		_x < 0 ? -_x : _x;\
	})

//! Ensures that x is between the limits set by low and high. If low is greater than high the result is undefined.
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

//! Check a character is a number in character.
#define DEC_CHECK(x) ((x) >= '0' && (x) <= '9')

//! Security addition, prevent overflow, if not, plus number.
#define SECUR_INC(val, number) \
	(val = ((((val)+(number)) > (val)) ? ((val)+(number)) : (val)))

//! Get the number of elements stored in array, but array [a] must not allocated by malloc(3).
#define ARRAY_SIZE(a) (sizeof((a)) / sizeof(((a)[0])))

#ifdef _DEBUGMODEON
//! Output debug message, compiled date and time.
# define OUTPUT_DEBUG_MSG \
	printf("Compiled on %s at %s, author: Wang Liang <liang.wang@elektrobit.com>, Good luck!\n", __DATE__, __TIME__);
# endif

#ifdef __cplusplus
}
#endif

#endif /* CPROJECTDFN_H_ */
