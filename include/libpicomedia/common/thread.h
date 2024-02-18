#ifndef PICOMEDIA_COMMON_THREAD_H
#define PICOMEDIA_COMMON_THREAD_H

#include "libpicomedia/common/common_base.h"

/**
 * @file thread.h
 * @brief This file contains the declarations of functions related to thread management in libpicomedia.
 */

/**
 * @brief Structure representing a thread.
 */
struct PM_Thread;
typedef struct PM_Thread PM_Thread;
typedef PM_Thread* PM_ThreadHandle; // This is just a wrapper to avoid typing out the pointer syntax.

/**
 * @brief Structure representing a mutex.
 */
struct PM_Mutex;
typedef struct PM_Mutex PM_Mutex;
typedef PM_Mutex* PM_MutexHandle; // This is just a wrapper to avoid typing out the pointer syntax.

/**
 * @brief Function pointer type for thread function.
 *
 * @param thread The PM_Thread structure.
 * @param data Additional data passed to the thread function.
 * @return PM_Bool The return value of the thread function.
 */
typedef PM_Bool (*PM_ThreadFunc)(PM_Thread* thread, void* data);

/**
 * @brief Type representing a thread ID.
 */
typedef PM_UInt64 PM_ThreadID;


/**
 * @brief Creates a new thread and associates it with the specified function. After creation, the thread is immediately started.
 * 
 * @param func The function to be executed by the thread.
 * @param data A pointer to the data that will be passed to the thread function.
 * @return A pointer to the created PM_Thread object, or NULL if the thread creation fails.
 */
PM_Thread* PICOMEDIA_API PM_ThreadCreate(PM_ThreadFunc func, void* data);

/**
 * @brief Destroys the specified thread.
 * 
 * @param thread A pointer to the PM_Thread object to be destroyed.
 */
void PICOMEDIA_API PM_ThreadDestroy(PM_Thread* thread);

/**
 * @brief Retrieves the ID of the specified thread.
 * 
 * @param thread A pointer to the PM_Thread object.
 * @return The ID of the thread.
 */
PM_ThreadID PICOMEDIA_API PM_ThreadGetID(PM_Thread* thread);

/**
 * @brief Waits for the specified thread to finish its execution.
 * 
 * @param thread A pointer to the PM_Thread object.
 * @return PM_TRUE if the thread joins successfully, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ThreadJoin(PM_Thread* thread);

/**
 * @brief Checks if the specified thread is currently running.
 * 
 * @param thread A pointer to the PM_Thread object.
 * @return PM_TRUE if the thread is running, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_ThreadIsRunning(PM_Thread* thread);

/**
 * @brief Retrieves the ID of the current thread.
 * 
 * @return The ID of the current thread.
 */
PM_ThreadID PICOMEDIA_API PM_ThreadGetCurrrentID();

/**
 * @brief Logs a message from the specified thread.
 * 
 * @param thread A pointer to the PM_Thread object.
 * @param format The format string for the log message.
 * @param ... Additional arguments to be formatted according to the format string.
 */
void PICOMEDIA_API PM_ThreadLog(PM_Thread* thread, const PM_Char* format, ...);


/**
 * @brief Creates a new mutex.
 *
 * @return A pointer to the newly created mutex.
 */
PM_Mutex* PICOMEDIA_API PM_MutexCreate();

/**
 * @brief Destroys a mutex.
 *
 * @param mutex A pointer to the mutex to be destroyed.
 */
void PICOMEDIA_API PM_MutexDestroy(PM_Mutex* mutex);

/**
 * @brief Locks a mutex.
 *
 * @param mutex A pointer to the mutex to be locked.
 * @return PM_TRUE if the mutex was successfully locked, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_MutexLock(PM_Mutex* mutex);

/**
 * @brief Unlocks a mutex.
 *
 * @param mutex A pointer to the mutex to be unlocked.
 * @return PM_TRUE if the mutex was successfully unlocked, PM_FALSE otherwise.
 */
PM_Bool PICOMEDIA_API PM_MutexUnlock(PM_Mutex* mutex);

#endif // PICOMEDIA_COMMON_THREAD_H