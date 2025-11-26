#ifndef HANDLE_PROFILE_H
#define HANDLE_PROFILE_H


#include <iostream>
#include <windows.h>

const bool kEnableHandleTrace = true;
namespace {
	typedef void (*GenericFnPtr)();

	using sidestep::PreamblePatcher;

// Information about handle functions we want to patch
struct HandleFunctionInfo {
  const char* const name;          // name of fn in a module (eg "CreateEventA")
  GenericFnPtr windows_fn;                // the fn whose name we call
  GenericFnPtr origstub_fn;               // original fn contents after we patch
  GenericFnPtr perftools_fn;        // fn we want to patch in
};

// Enum for handle function indices
enum HandleFunctionIndex {
  CREATE_EVENT_A_INDEX = 0,
  CREATE_EVENT_W_INDEX,
  CREATE_EVENT_EX_A_INDEX,
  CREATE_EVENT_EX_W_INDEX,
  CREATE_MUTEX_A_INDEX,
  CREATE_MUTEX_W_INDEX,
  CREATE_MUTEX_EX_A_INDEX,
  CREATE_MUTEX_EX_W_INDEX,
  CREATE_SEMAPHORE_A_INDEX,
  CREATE_SEMAPHORE_W_INDEX,
  CREATE_SEMAPHORE_EX_A_INDEX,
  CREATE_SEMAPHORE_EX_W_INDEX,
  CREATE_WAITABLE_TIMER_A_INDEX,
  CREATE_WAITABLE_TIMER_W_INDEX,
  CREATE_WAITABLE_TIMER_EX_A_INDEX,
  CREATE_WAITABLE_TIMER_EX_W_INDEX,
  CREATE_THREAD_INDEX,
  CREATE_REMOTE_THREAD_INDEX,
  CREATE_FILE_A_INDEX,
  CREATE_FILE_W_INDEX,
  CREATE_FILE_MAPPING_A_INDEX,
  CREATE_FILE_MAPPING_W_INDEX,
  CREATE_FILE_MAPPING_NUMA_A_INDEX,
  CREATE_FILE_MAPPING_NUMA_W_INDEX,
  CREATE_PIPE_INDEX,
  CREATE_NAMED_PIPE_A_INDEX,
  CREATE_NAMED_PIPE_W_INDEX,
  HEAP_CREATE_INDEX,
  CREATE_JOB_OBJECT_A_INDEX,
  CREATE_JOB_OBJECT_W_INDEX,
  CREATE_CONSOLE_SCREEN_BUFFER_INDEX,
  CREATE_MEMORY_RESOURCE_NOTIFICATION_INDEX,
  CREATE_THREADPOOL_TIMER_INDEX,
  CREATE_THREADPOOL_WAIT_INDEX,
  CREATE_THREADPOOL_IO_INDEX,
  CREATE_THREADPOOL_WORK_INDEX,
  CLOSE_HANDLE_INDEX,
  MAX_HANDLE_FUNCTIONS
};

// Forward declaration
//typedef void (*GenericFnPtr)();

// Array of handle functions to patch
extern HandleFunctionInfo handle_function_info_[];

// Handle patching functions
void PatchHandleFunctions();
void UnpatchHandleFunctions();

// Handle function hook implementations
HANDLE WINAPI Perftools_CreateEventA(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    BOOL bManualReset,
    BOOL bInitialState,
    LPCSTR lpName);

HANDLE WINAPI Perftools_CreateEventW(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    BOOL bManualReset,
    BOOL bInitialState,
    LPCWSTR lpName);

HANDLE WINAPI Perftools_CreateEventExA(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    LPCSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateEventExW(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    LPCWSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateMutexA(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    BOOL bInitialOwner,
    LPCSTR lpName);

HANDLE WINAPI Perftools_CreateMutexW(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    BOOL bInitialOwner,
    LPCWSTR lpName);

HANDLE WINAPI Perftools_CreateMutexExA(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    LPCSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateMutexExW(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    LPCWSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateSemaphoreA(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCSTR lpName);

HANDLE WINAPI Perftools_CreateSemaphoreW(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCWSTR lpName);

HANDLE WINAPI Perftools_CreateSemaphoreExA(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateSemaphoreExW(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCWSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateWaitableTimerA(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    BOOL bManualReset,
    LPCSTR lpTimerName);

HANDLE WINAPI Perftools_CreateWaitableTimerW(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    BOOL bManualReset,
    LPCWSTR lpTimerName);

HANDLE WINAPI Perftools_CreateWaitableTimerExA(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    LPCSTR lpTimerName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateWaitableTimerExW(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    LPCWSTR lpTimerName,
    DWORD dwFlags,
    DWORD dwDesiredAccess);

HANDLE WINAPI Perftools_CreateThread(
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    SIZE_T dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    DWORD dwCreationFlags,
    LPDWORD lpThreadId);

HANDLE WINAPI Perftools_CreateRemoteThread(
    HANDLE hProcess,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    SIZE_T dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    DWORD dwCreationFlags,
    LPDWORD lpThreadId);

HANDLE WINAPI Perftools_CreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

HANDLE WINAPI Perftools_CreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile);

HANDLE WINAPI Perftools_CreateFileMappingA(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCSTR lpName);

HANDLE WINAPI Perftools_CreateFileMappingW(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCWSTR lpName);

HANDLE WINAPI Perftools_CreateFileMappingNumaA(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCSTR lpName,
    DWORD nndPreferred);

HANDLE WINAPI Perftools_CreateFileMappingNumaW(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCWSTR lpName,
    DWORD nndPreferred);

BOOL WINAPI Perftools_CreatePipe(
    PHANDLE hReadPipe,
    PHANDLE hWritePipe,
    LPSECURITY_ATTRIBUTES lpPipeAttributes,
    DWORD nSize);

HANDLE WINAPI Perftools_CreateNamedPipeA(
    LPCSTR lpName,
    DWORD dwOpenMode,
    DWORD dwPipeMode,
    DWORD nMaxInstances,
    DWORD nOutBufferSize,
    DWORD nInBufferSize,
    DWORD nDefaultTimeOut,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes);

HANDLE WINAPI Perftools_CreateNamedPipeW(
    LPCWSTR lpName,
    DWORD dwOpenMode,
    DWORD dwPipeMode,
    DWORD nMaxInstances,
    DWORD nOutBufferSize,
    DWORD nInBufferSize,
    DWORD nDefaultTimeOut,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes);

HANDLE WINAPI Perftools_HeapCreate(
    DWORD flOptions,
    SIZE_T dwInitialSize,
    SIZE_T dwMaximumSize);

HANDLE WINAPI Perftools_CreateJobObjectA(
    LPSECURITY_ATTRIBUTES lpJobAttributes,
    LPCSTR lpName);

HANDLE WINAPI Perftools_CreateJobObjectW(
    LPSECURITY_ATTRIBUTES lpJobAttributes,
    LPCWSTR lpName);

// Completely remove the CreateTransaction function implementation

HANDLE WINAPI Perftools_CreateConsoleScreenBuffer(
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwFlags,
    LPVOID lpScreenBufferData);

HANDLE WINAPI Perftools_CreateMemoryResourceNotification(
    MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType);

HANDLE WINAPI Perftools_CreateThreadpoolTimer(
    PTP_TIMER_CALLBACK pfnTimerCallback,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe);

HANDLE WINAPI Perftools_CreateThreadpoolWait(
    PTP_WAIT_CALLBACK pfnWaitCallback,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe);

HANDLE WINAPI Perftools_CreateThreadpoolIo(
    HANDLE fl,
    PTP_WIN32_IO_CALLBACK pfnio,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe);

HANDLE WINAPI Perftools_CreateThreadpoolWork(
    PTP_WORK_CALLBACK pfnWorkCallback,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe);

BOOL WINAPI Perftools_CloseHandle(HANDLE hObject);

// Array of handle functions to patch
HandleFunctionInfo handle_function_info_[] = {
  { "CreateEventA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateEventA },
  { "CreateEventW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateEventW },
  { "CreateEventExA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateEventExA },
  { "CreateEventExW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateEventExW },
  { "CreateMutexA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateMutexA },
  { "CreateMutexW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateMutexW },
  { "CreateMutexExA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateMutexExA },
  { "CreateMutexExW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateMutexExW },
  { "CreateSemaphoreA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateSemaphoreA },
  { "CreateSemaphoreW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateSemaphoreW },
  { "CreateSemaphoreExA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateSemaphoreExA },
  { "CreateSemaphoreExW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateSemaphoreExW },
  { "CreateWaitableTimerA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateWaitableTimerA },
  { "CreateWaitableTimerW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateWaitableTimerW },
  { "CreateWaitableTimerExA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateWaitableTimerExA },
  { "CreateWaitableTimerExW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateWaitableTimerExW },
  { "CreateThread", nullptr, nullptr, (GenericFnPtr)Perftools_CreateThread },
  { "CreateRemoteThread", nullptr, nullptr, (GenericFnPtr)Perftools_CreateRemoteThread },
  { "CreateFileA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateFileA },
  { "CreateFileW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateFileW },
  { "CreateFileMappingA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateFileMappingA },
  { "CreateFileMappingW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateFileMappingW },
  { "CreateFileMappingNumaA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateFileMappingNumaA },
  { "CreateFileMappingNumaW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateFileMappingNumaW },
  { "CreatePipe", nullptr, nullptr, (GenericFnPtr)Perftools_CreatePipe },
  { "CreateNamedPipeA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateNamedPipeA },
  { "CreateNamedPipeW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateNamedPipeW },
  { "HeapCreate", nullptr, nullptr, (GenericFnPtr)Perftools_HeapCreate },
  { "CreateJobObjectA", nullptr, nullptr, (GenericFnPtr)Perftools_CreateJobObjectA },
  { "CreateJobObjectW", nullptr, nullptr, (GenericFnPtr)Perftools_CreateJobObjectW },
  { "CreateConsoleScreenBuffer", nullptr, nullptr, (GenericFnPtr)Perftools_CreateConsoleScreenBuffer },
  { "CreateMemoryResourceNotification", nullptr, nullptr, (GenericFnPtr)Perftools_CreateMemoryResourceNotification },
  { "CreateThreadpoolTimer", nullptr, nullptr, (GenericFnPtr)Perftools_CreateThreadpoolTimer },
  { "CreateThreadpoolWait", nullptr, nullptr, (GenericFnPtr)Perftools_CreateThreadpoolWait },
  { "CreateThreadpoolIo", nullptr, nullptr, (GenericFnPtr)Perftools_CreateThreadpoolIo },
  { "CreateThreadpoolWork", nullptr, nullptr, (GenericFnPtr)Perftools_CreateThreadpoolWork },
  { "CloseHandle", nullptr, nullptr, (GenericFnPtr)Perftools_CloseHandle },
};

// Handle patching functions
void PatchHandleFunctions() {
  HMODULE kernel32_module = ::GetModuleHandleA("kernel32.dll");
  if (kernel32_module == nullptr) {
    return;
  }

  // Unlike for libc, we know these exist in our module, so we can get
  // and patch at the same time.
  for (int i = 0; i < MAX_HANDLE_FUNCTIONS; i++) {
    handle_function_info_[i].windows_fn = (GenericFnPtr)
        ::GetProcAddress(kernel32_module, handle_function_info_[i].name);
    
    if (handle_function_info_[i].origstub_fn){continue;}
    // If origstub_fn is not nullptr, it's left around from a previous
    // patch. We need to set it to nullptr for the new Patch call.
    // Since we've patched Unpatch() not to delete origstub_fn_ (it
    // causes problems in some contexts, though obviously not this
    // one), we should delete it now, before setting it to nullptr.
    // NOTE: casting from a function to a pointer is contra the C++
    //       spec. It's not safe on IA64, but is on i386. We use
    //       a C-style cast here to emphasize this is not legal C++.
    delete[] (char*)(handle_function_info_[i].origstub_fn);
    handle_function_info_[i].origstub_fn = nullptr;  // Patch() will fill this in
    
    if (sidestep::SIDESTEP_SUCCESS != sidestep::PreamblePatcher::Patch(handle_function_info_[i].windows_fn,
                                    handle_function_info_[i].perftools_fn,
                                    &handle_function_info_[i].origstub_fn)){
        std::cout << "Failed to patch " << handle_function_info_[i].name << std::endl;
    }
  }
}

void UnpatchHandleFunctions() {
  // We have to cast our GenericFnPtrs to void* for unpatch. This is
  // contra the C++ spec; we use C-style casts to emphasize that.
  for (int i = 0; i < MAX_HANDLE_FUNCTIONS; i++) {
    if (handle_function_info_[i].origstub_fn) {
      if (sidestep::SIDESTEP_SUCCESS !=
          sidestep::PreamblePatcher::Unpatch((void*)handle_function_info_[i].windows_fn,
                                  (void*)handle_function_info_[i].perftools_fn,
                                  (void*)handle_function_info_[i].origstub_fn)){
            std::cout << "Failed to unpatch " << handle_function_info_[i].name << std::endl;
        }
    }
  }
}

// Handle function hook implementations with default behavior
HANDLE WINAPI Perftools_CreateEventA(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    BOOL bManualReset,
    BOOL bInitialState,
    LPCSTR lpName) {
  if (kEnableHandleTrace) {
    std::cout << "CreateEventA called with lpName=" << (lpName ? lpName : "NULL") 
              << ", bManualReset=" << bManualReset << ", bInitialState=" << bInitialState << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCSTR))
                  handle_function_info_[CREATE_EVENT_A_INDEX].origstub_fn)(
                  lpEventAttributes, bManualReset, bInitialState, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateEventA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateEventW(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    BOOL bManualReset,
    BOOL bInitialState,
    LPCWSTR lpName) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateEventW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", bManualReset=" << bManualReset << L", bInitialState=" << bInitialState << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCWSTR))
                  handle_function_info_[CREATE_EVENT_W_INDEX].origstub_fn)(
                  lpEventAttributes, bManualReset, bInitialState, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateEventW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateEventExA(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    LPCSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::cout << "CreateEventExA called with lpName=" << (lpName ? lpName : "NULL")
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_EVENT_EX_A_INDEX].origstub_fn)(
                  lpEventAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateEventExA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateEventExW(
    LPSECURITY_ATTRIBUTES lpEventAttributes,
    LPCWSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateEventExW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_EVENT_EX_W_INDEX].origstub_fn)(
                  lpEventAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateEventExW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexA(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    BOOL bInitialOwner,
    LPCSTR lpName) {
  if (kEnableHandleTrace) {
    std::cout << "CreateMutexA called with lpName=" << (lpName ? lpName : "NULL")
              << ", bInitialOwner=" << bInitialOwner << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR))
                  handle_function_info_[CREATE_MUTEX_A_INDEX].origstub_fn)(
                  lpMutexAttributes, bInitialOwner, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateMutexA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexW(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    BOOL bInitialOwner,
    LPCWSTR lpName) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateMutexW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", bInitialOwner=" << bInitialOwner << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCWSTR))
                  handle_function_info_[CREATE_MUTEX_W_INDEX].origstub_fn)(
                  lpMutexAttributes, bInitialOwner, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateMutexW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexExA(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    LPCSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::cout << "CreateMutexExA called with lpName=" << (lpName ? lpName : "NULL")
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_MUTEX_EX_A_INDEX].origstub_fn)(
                  lpMutexAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateMutexExA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexExW(
    LPSECURITY_ATTRIBUTES lpMutexAttributes,
    LPCWSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateMutexExW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_MUTEX_EX_W_INDEX].origstub_fn)(
                  lpMutexAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateMutexExW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreA(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCSTR lpName) {
  if (kEnableHandleTrace) {
    std::cout << "CreateSemaphoreA called with lpName=" << (lpName ? lpName : "NULL")
              << ", lInitialCount=" << lInitialCount << ", lMaximumCount=" << lMaximumCount << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCSTR))
                  handle_function_info_[CREATE_SEMAPHORE_A_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateSemaphoreA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreW(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCWSTR lpName) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateSemaphoreW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", lInitialCount=" << lInitialCount << L", lMaximumCount=" << lMaximumCount << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCWSTR))
                  handle_function_info_[CREATE_SEMAPHORE_W_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateSemaphoreW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreExA(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::cout << "CreateSemaphoreExA called with lpName=" << (lpName ? lpName : "NULL")
              << ", lInitialCount=" << lInitialCount << ", lMaximumCount=" << lMaximumCount
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_SEMAPHORE_EX_A_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateSemaphoreExA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreExW(
    LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    LONG lInitialCount,
    LONG lMaximumCount,
    LPCWSTR lpName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateSemaphoreExW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", lInitialCount=" << lInitialCount << L", lMaximumCount=" << lMaximumCount
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_SEMAPHORE_EX_W_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateSemaphoreExW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerA(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    BOOL bManualReset,
    LPCSTR lpTimerName) {
  if (kEnableHandleTrace) {
    std::cout << "CreateWaitableTimerA called with lpTimerName=" << (lpTimerName ? lpTimerName : "NULL")
              << ", bManualReset=" << bManualReset << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR))
                  handle_function_info_[CREATE_WAITABLE_TIMER_A_INDEX].origstub_fn)(
                  lpTimerAttributes, bManualReset, lpTimerName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateWaitableTimerA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerW(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    BOOL bManualReset,
    LPCWSTR lpTimerName) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateWaitableTimerW called with lpTimerName=" << (lpTimerName ? lpTimerName : L"NULL")
               << L", bManualReset=" << bManualReset << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCWSTR))
                  handle_function_info_[CREATE_WAITABLE_TIMER_W_INDEX].origstub_fn)(
                  lpTimerAttributes, bManualReset, lpTimerName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateWaitableTimerW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerExA(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    LPCSTR lpTimerName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::cout << "CreateWaitableTimerExA called with lpTimerName=" << (lpTimerName ? lpTimerName : "NULL")
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_WAITABLE_TIMER_EX_A_INDEX].origstub_fn)(
                  lpTimerAttributes, lpTimerName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateWaitableTimerExA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerExW(
    LPSECURITY_ATTRIBUTES lpTimerAttributes,
    LPCWSTR lpTimerName,
    DWORD dwFlags,
    DWORD dwDesiredAccess) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateWaitableTimerExW called with lpTimerName=" << (lpTimerName ? lpTimerName : L"NULL")
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_WAITABLE_TIMER_EX_W_INDEX].origstub_fn)(
                  lpTimerAttributes, lpTimerName, dwFlags, dwDesiredAccess);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateWaitableTimerExW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateThread(
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    SIZE_T dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    DWORD dwCreationFlags,
    LPDWORD lpThreadId) {
  if (kEnableHandleTrace) {
    std::cout << "CreateThread called with dwStackSize=" << dwStackSize
              << ", lpStartAddress=" << lpStartAddress << ", dwCreationFlags=" << dwCreationFlags << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD))
                  handle_function_info_[CREATE_THREAD_INDEX].origstub_fn)(
                  lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateThread returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateRemoteThread(
    HANDLE hProcess,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    SIZE_T dwStackSize,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    DWORD dwCreationFlags,
    LPDWORD lpThreadId) {
  if (kEnableHandleTrace) {
    std::cout << "CreateRemoteThread called with hProcess=" << hProcess << ", dwStackSize=" << dwStackSize
              << ", lpStartAddress=" << lpStartAddress << ", dwCreationFlags=" << dwCreationFlags << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD))
                  handle_function_info_[CREATE_REMOTE_THREAD_INDEX].origstub_fn)(
                  hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateRemoteThread returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileA(
    LPCSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile) {
  if (kEnableHandleTrace) {
    std::cout << "CreateFileA called with lpFileName=" << (lpFileName ? lpFileName : "NULL")
              << ", dwDesiredAccess=" << dwDesiredAccess << ", dwShareMode=" << dwShareMode
              << ", dwCreationDisposition=" << dwCreationDisposition 
              << ", dwFlagsAndAttributes=" << dwFlagsAndAttributes << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))
                  handle_function_info_[CREATE_FILE_A_INDEX].origstub_fn)(
                  lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, 
                  dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateFileA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileW(
    LPCWSTR lpFileName,
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwCreationDisposition,
    DWORD dwFlagsAndAttributes,
    HANDLE hTemplateFile) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateFileW called with lpFileName=" << (lpFileName ? lpFileName : L"NULL")
               << L", dwDesiredAccess=" << dwDesiredAccess << L", dwShareMode=" << dwShareMode
               << L", dwCreationDisposition=" << dwCreationDisposition
               << L", dwFlagsAndAttributes=" << dwFlagsAndAttributes << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))
                  handle_function_info_[CREATE_FILE_W_INDEX].origstub_fn)(
                  lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
                  dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateFileW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingA(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCSTR lpName) {
  if (kEnableHandleTrace) {
    std::cout << "CreateFileMappingA called with hFile=" << hFile << ", lpName=" << (lpName ? lpName : "NULL")
              << ", flProtect=" << flProtect << ", dwMaximumSizeHigh=" << dwMaximumSizeHigh
              << ", dwMaximumSizeLow=" << dwMaximumSizeLow << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCSTR))
                  handle_function_info_[CREATE_FILE_MAPPING_A_INDEX].origstub_fn)(
                  hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateFileMappingA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingW(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCWSTR lpName) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateFileMappingW called with hFile=" << hFile << L", lpName=" << (lpName ? lpName : L"NULL")
               << L", flProtect=" << flProtect << L", dwMaximumSizeHigh=" << dwMaximumSizeHigh
               << L", dwMaximumSizeLow=" << dwMaximumSizeLow << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCWSTR))
                  handle_function_info_[CREATE_FILE_MAPPING_W_INDEX].origstub_fn)(
                  hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateFileMappingW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingNumaA(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCSTR lpName,
    DWORD nndPreferred) {
  if (kEnableHandleTrace) {
    std::cout << "CreateFileMappingNumaA called with hFile=" << hFile << ", lpName=" << (lpName ? lpName : "NULL")
              << ", flProtect=" << flProtect << ", dwMaximumSizeHigh=" << dwMaximumSizeHigh
              << ", dwMaximumSizeLow=" << dwMaximumSizeLow << ", nndPreferred=" << nndPreferred << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCSTR, DWORD))
                  handle_function_info_[CREATE_FILE_MAPPING_NUMA_A_INDEX].origstub_fn)(
                  hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName, nndPreferred);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateFileMappingNumaA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingNumaW(
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh,
    DWORD dwMaximumSizeLow,
    LPCWSTR lpName,
    DWORD nndPreferred) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateFileMappingNumaW called with hFile=" << hFile << L", lpName=" << (lpName ? lpName : L"NULL")
               << L", flProtect=" << flProtect << L", dwMaximumSizeHigh=" << dwMaximumSizeHigh
               << L", dwMaximumSizeLow=" << dwMaximumSizeLow << L", nndPreferred=" << nndPreferred << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCWSTR, DWORD))
                  handle_function_info_[CREATE_FILE_MAPPING_NUMA_W_INDEX].origstub_fn)(
                  hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName, nndPreferred);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateFileMappingNumaW returned " << result << std::endl;
  }
  
  return result;
}

BOOL WINAPI Perftools_CreatePipe(
    PHANDLE hReadPipe,
    PHANDLE hWritePipe,
    LPSECURITY_ATTRIBUTES lpPipeAttributes,
    DWORD nSize) {
  if (kEnableHandleTrace) {
    std::cout << "CreatePipe called with nSize=" << nSize << std::endl;
  }
  
  BOOL result = ((BOOL (WINAPI *)(PHANDLE, PHANDLE, LPSECURITY_ATTRIBUTES, DWORD))
                handle_function_info_[CREATE_PIPE_INDEX].origstub_fn)(
                hReadPipe, hWritePipe, lpPipeAttributes, nSize);
                
  if (kEnableHandleTrace) {
    std::cout << "CreatePipe returned " << result 
              << ", hReadPipe=" << (hReadPipe ? *hReadPipe : NULL) 
              << ", hWritePipe=" << (hWritePipe ? *hWritePipe : NULL) << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateNamedPipeA(
    LPCSTR lpName,
    DWORD dwOpenMode,
    DWORD dwPipeMode,
    DWORD nMaxInstances,
    DWORD nOutBufferSize,
    DWORD nInBufferSize,
    DWORD nDefaultTimeOut,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes) {
  if (kEnableHandleTrace) {
    std::cout << "CreateNamedPipeA called with lpName=" << (lpName ? lpName : "NULL")
              << ", dwOpenMode=" << dwOpenMode << ", dwPipeMode=" << dwPipeMode
              << ", nMaxInstances=" << nMaxInstances << ", nOutBufferSize=" << nOutBufferSize
              << ", nInBufferSize=" << nInBufferSize << ", nDefaultTimeOut=" << nDefaultTimeOut << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES))
                  handle_function_info_[CREATE_NAMED_PIPE_A_INDEX].origstub_fn)(
                  lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, 
                  nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateNamedPipeA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateNamedPipeW(
    LPCWSTR lpName,
    DWORD dwOpenMode,
    DWORD dwPipeMode,
    DWORD nMaxInstances,
    DWORD nOutBufferSize,
    DWORD nInBufferSize,
    DWORD nDefaultTimeOut,
    LPSECURITY_ATTRIBUTES lpSecurityAttributes) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateNamedPipeW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", dwOpenMode=" << dwOpenMode << L", dwPipeMode=" << dwPipeMode
               << L", nMaxInstances=" << nMaxInstances << L", nOutBufferSize=" << nOutBufferSize
               << L", nInBufferSize=" << nInBufferSize << L", nDefaultTimeOut=" << nDefaultTimeOut << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCWSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES))
                  handle_function_info_[CREATE_NAMED_PIPE_W_INDEX].origstub_fn)(
                  lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize,
                  nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateNamedPipeW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_HeapCreate(
    DWORD flOptions,
    SIZE_T dwInitialSize,
    SIZE_T dwMaximumSize) {
  if (kEnableHandleTrace) {
    std::cout << "HeapCreate called with flOptions=" << flOptions 
              << ", dwInitialSize=" << dwInitialSize << ", dwMaximumSize=" << dwMaximumSize << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(DWORD, SIZE_T, SIZE_T))
                  handle_function_info_[HEAP_CREATE_INDEX].origstub_fn)(
                  flOptions, dwInitialSize, dwMaximumSize);
                  
  if (kEnableHandleTrace) {
    std::cout << "HeapCreate returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateJobObjectA(
    LPSECURITY_ATTRIBUTES lpJobAttributes,
    LPCSTR lpName) {
  if (kEnableHandleTrace) {
    std::cout << "CreateJobObjectA called with lpName=" << (lpName ? lpName : "NULL") << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR))
                  handle_function_info_[CREATE_JOB_OBJECT_A_INDEX].origstub_fn)(
                  lpJobAttributes, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateJobObjectA returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateJobObjectW(
    LPSECURITY_ATTRIBUTES lpJobAttributes,
    LPCWSTR lpName) {
  if (kEnableHandleTrace) {
    std::wcout << L"CreateJobObjectW called with lpName=" << (lpName ? lpName : L"NULL") << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR))
                  handle_function_info_[CREATE_JOB_OBJECT_W_INDEX].origstub_fn)(
                  lpJobAttributes, lpName);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateJobObjectW returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateConsoleScreenBuffer(
    DWORD dwDesiredAccess,
    DWORD dwShareMode,
    const LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    DWORD dwFlags,
    LPVOID lpScreenBufferData) {
  if (kEnableHandleTrace) {
    std::cout << "CreateConsoleScreenBuffer called with dwDesiredAccess=" << dwDesiredAccess
              << ", dwShareMode=" << dwShareMode << ", dwFlags=" << dwFlags << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(DWORD, DWORD, const LPSECURITY_ATTRIBUTES, DWORD, LPVOID))
                  handle_function_info_[CREATE_CONSOLE_SCREEN_BUFFER_INDEX].origstub_fn)(
                  dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwFlags, lpScreenBufferData);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateConsoleScreenBuffer returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateMemoryResourceNotification(
    MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType) {
  if (kEnableHandleTrace) {
    std::cout << "CreateMemoryResourceNotification called with NotificationType=" << NotificationType << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(MEMORY_RESOURCE_NOTIFICATION_TYPE))
                  handle_function_info_[CREATE_MEMORY_RESOURCE_NOTIFICATION_INDEX].origstub_fn)(
                  NotificationType);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateMemoryResourceNotification returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateThreadpoolTimer(
    PTP_TIMER_CALLBACK pfnTimerCallback,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe) {
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolTimer called with pfnTimerCallback=" << pfnTimerCallback
              << ", pv=" << pv << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(PTP_TIMER_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_TIMER_INDEX].origstub_fn)(
                  pfnTimerCallback, pv, pcbe);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolTimer returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateThreadpoolWait(
    PTP_WAIT_CALLBACK pfnWaitCallback,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe) {
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolWait called with pfnWaitCallback=" << pfnWaitCallback
              << ", pv=" << pv << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(PTP_WAIT_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_WAIT_INDEX].origstub_fn)(
                  pfnWaitCallback, pv, pcbe);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolWait returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateThreadpoolIo(
    HANDLE fl,
    PTP_WIN32_IO_CALLBACK pfnio,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe) {
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolIo called with fl=" << fl << ", pfnio=" << pfnio
              << ", pv=" << pv << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, PTP_WIN32_IO_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_IO_INDEX].origstub_fn)(
                  fl, pfnio, pv, pcbe);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolIo returned " << result << std::endl;
  }
  
  return result;
}

HANDLE WINAPI Perftools_CreateThreadpoolWork(
    PTP_WORK_CALLBACK pfnWorkCallback,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe) {
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolWork called with pfnWorkCallback=" << pfnWorkCallback
              << ", pv=" << pv << std::endl;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(PTP_WORK_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_WORK_INDEX].origstub_fn)(
                  pfnWorkCallback, pv, pcbe);
                  
  if (kEnableHandleTrace) {
    std::cout << "CreateThreadpoolWork returned " << result << std::endl;
  }
  
  return result;
}

BOOL WINAPI Perftools_CloseHandle(HANDLE hObject) {
  if (kEnableHandleTrace) {
    std::cout << "CloseHandle called with hObject=" << hObject << std::endl;
  }
  
  BOOL result = ((BOOL (WINAPI *)(HANDLE))
                handle_function_info_[CLOSE_HANDLE_INDEX].origstub_fn)(
                hObject);
                
  return result;
}

}  // namespace

#endif