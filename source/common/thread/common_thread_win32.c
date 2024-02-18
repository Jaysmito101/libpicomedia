#include "libpicomedia/common/thread.h"
#include "libpicomedia/common/utils.h"

#include <windows.h>
#include <process.h> 

struct PM_Thread
{
    uintptr_t handle;
    PM_UInt32 id;
    PM_ThreadFunc function;
    void* data;
};

struct PM_Mutex
{
    HANDLE handle;
};

// -----------------------------------------------------------------------------------------------

static PM_UInt32 __stdcall PM__ThreadProc(void* args)
{
    PM_Thread* thread = (PM_Thread*)args;
    
    if (thread->function != NULL)
    {
        thread->function(thread, thread->data);
    }
    
    _endthreadex(0);

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
    thread->id = 0;
    thread->function = func;
    thread->data = data;

    thread->handle = _beginthreadex(
        NULL,
        0,
        PM__ThreadProc,
        thread,
        CREATE_SUSPENDED,
        &thread->id
    );

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

PM_Bool PICOMEDIA_API PM_ThreadStart(PM_Thread* thread)
{
    return ResumeThread((HANDLE)thread->handle) != -1;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_ThreadJoin(PM_Thread* thread)
{
    return WaitForSingleObject((HANDLE)thread->handle, INFINITE) == WAIT_OBJECT_0;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_ThreadIsRunning(PM_Thread* thread)
{
    DWORD exitCode;
    return GetExitCodeThread((HANDLE)thread->handle, &exitCode) && exitCode == STILL_ACTIVE;
}

// -----------------------------------------------------------------------------------------------

PM_ThreadID PICOMEDIA_API PM_ThreadGetCurrrentID()
{
    return (PM_ThreadID)GetCurrentThreadId();
}

// -----------------------------------------------------------------------------------------------

void PICOMEDIA_API PM_ThreadLog(PM_Thread* thread, const PM_Char* format, ...)
{
    PM_LogError("PM_ThreadLog is not implemented on Windows");
}

// -----------------------------------------------------------------------------------------------

PM_Mutex* PICOMEDIA_API PM_MutexCreate()
{
    PM_Mutex* mutex = PM_Malloc(sizeof(PM_Mutex));
    if (mutex == NULL)
    {
        return NULL;
    }

    mutex->handle = CreateMutex(NULL, FALSE, NULL);

    return mutex;
}

// -----------------------------------------------------------------------------------------------

void PICOMEDIA_API PM_MutexDestroy(PM_Mutex* mutex)
{
    PM_Assert(mutex != NULL);

    CloseHandle(mutex->handle);
    
    PM_Free(mutex);
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_MutexLock(PM_Mutex* mutex)
{
    PM_Assert(mutex != NULL);

    return WaitForSingleObject(mutex->handle, INFINITE) == WAIT_OBJECT_0;
}

// -----------------------------------------------------------------------------------------------

PM_Bool PICOMEDIA_API PM_MutexUnlock(PM_Mutex* mutex)
{
    PM_Assert(mutex != NULL);

    return ReleaseMutex(mutex->handle);
}

// -----------------------------------------------------------------------------------------------
