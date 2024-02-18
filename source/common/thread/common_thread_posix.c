#include "libpicomedia/common/thread.h"
#include "libpicomedia/common/utils.h"

#include <pthread.h>
#include <signal.h>


struct PM_Thread
{
    pthread_t handle;
    PM_ThreadID id;
    PM_ThreadFunc function;
    void* data;
};

struct PM_Mutex
{
    pthread_mutex_t handle;
};

// -----------------------------------------------------------------------------------------------

static void*  PM__ThreadProc(void* args)
{
    PM_Thread* thread = (PM_Thread*)args;
    thread->id = (PM_ThreadID)pthread_self();

    
    if (thread->function != NULL)
    {
        thread->function(thread, thread->data);
    }

    return 0;
}

// -----------------------------------------------------------------------------------------------

PM_Thread* PICOMEDIA_API PM_ThreadCreate(PM_ThreadFunc func, void* data)
{
    PM_Assert(func != NULL);

    PM_Thread* thread = PM_Malloc(sizeof(PM_Thread));
    if (thread == NULL)
    {
        return NULL;
    }

    thread->handle = 0;
    thread->function = func;
    thread->data = data;

    thread->handle = pthread_create(&thread->handle, NULL, PM__ThreadProc, thread);

    return thread;
}

// -----------------------------------------------------------------------------------------------

void PICOMEDIA_API PM_ThreadDestroy(PM_Thread* thread)
{
    PM_Assert(thread != NULL);

    if (thread->handle != 0)
    {
        PM_ThreadJoin(thread);
    }

    PM_Free(thread);
}

// -----------------------------------------------------------------------------------------------

PM_ThreadID PICOMEDIA_API PM_ThreadGetID(PM_Thread* thread)
{
    return (PM_ThreadID)thread->id;
}

// -----------------------------------------------------------------------------------------------


PM_Bool PICOMEDIA_API PM_ThreadJoin(PM_Thread* thread)
{
    return pthread_join(thread->handle, NULL) == 0;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_ThreadIsRunning(PM_Thread* thread)
{
    // NOTE: The thread here is joinable
    return pthread_kill(thread->handle, 0) == 0;    
}

// -----------------------------------------------------------------------------------------------

PM_ThreadID PICOMEDIA_API PM_ThreadGetCurrrentID()
{
    return (PM_ThreadID)pthread_self();
}

// -----------------------------------------------------------------------------------------------

void PICOMEDIA_API PM_ThreadLog(PM_Thread* thread, const PM_Char* format, ...)
{
    (void)thread; (void)format; // Unused
    PM_LogError("PM_ThreadLog is not implemented on Posix systems.");
}

// -----------------------------------------------------------------------------------------------

PM_Mutex* PICOMEDIA_API PM_MutexCreate()
{
    PM_Mutex* mutex = PM_Malloc(sizeof(PM_Mutex));
    if (mutex == NULL)
    {
        return NULL;
    }

    pthread_mutex_init(&mutex->handle, NULL);

    return mutex;
}

// -----------------------------------------------------------------------------------------------

void PICOMEDIA_API PM_MutexDestroy(PM_Mutex* mutex)
{
    PM_Assert(mutex != NULL);

    pthread_mutex_destroy(&mutex->handle);
    
    PM_Free(mutex);
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_MutexLock(PM_Mutex* mutex)
{
    PM_Assert(mutex != NULL);

    return pthread_mutex_lock(&mutex->handle) == 0;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_MutexUnlock(PM_Mutex* mutex)
{
    PM_Assert(mutex != NULL);

    return pthread_mutex_unlock(&mutex->handle) == 0;
}

// -----------------------------------------------------------------------------------------------
