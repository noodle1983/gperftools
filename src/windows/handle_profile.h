// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
/* Copyright (c) 2007, Google Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: Craig Silverstein
 * Author: Joi Sigurdsson
 *
 * Header file for the windows-specific handle profiler.
 */

#ifndef GOOGLE_PERFTOOLS_HANDLE_PROFILE_H_
#define GOOGLE_PERFTOOLS_HANDLE_PROFILE_H_

// Disable the 'deprecated' warning for CreateTransaction.
#pragma warning(push)
#pragma warning(disable : 4996)

#include <windows.h>
#include <iostream>
#include <cstdint>

// Include Detours header for function hooking
// The path will be resolved through CMake include_directories
#include "detours/detours.h"

#include "base/logging.h"

// This is the same basename that is used in the tcmalloc library.
#define PERFTOOLS_DLL_DECL __declspec(dllexport)

// Define a macro for handle tracing that can be enabled/disabled at compile time
#ifdef ENABLE_HANDLE_TRACING
#define HANDLE_TRACE_ENABLED 1
#else
#define HANDLE_TRACE_ENABLED 0
#endif

// Macro for conditional tracing
#define HANDLE_TRACE(condition, stmt) \
    do { \
        if (HANDLE_TRACE_ENABLED && (condition)) { \
            stmt; \
        } \
    } while (0)

// This module is linked into tcmalloc, so it should use Windows
// native exception handling and not C++ exceptions.
#pragma warning(push)
#pragma warning(disable : 4530)  // C++ exception handler used, but unwind
                                 // semantics are not enabled

// Handle function index constants
const int CREATE_EVENT_A_INDEX = 0;
const int CREATE_EVENT_W_INDEX = 1;
const int CREATE_EVENT_EX_A_INDEX = 2;
const int CREATE_EVENT_EX_W_INDEX = 3;
const int CREATE_MUTEX_A_INDEX = 4;
const int CREATE_MUTEX_W_INDEX = 5;
const int CREATE_MUTEX_EX_A_INDEX = 6;
const int CREATE_MUTEX_EX_W_INDEX = 7;
const int CREATE_SEMAPHORE_A_INDEX = 8;
const int CREATE_SEMAPHORE_W_INDEX = 9;
const int CREATE_SEMAPHORE_EX_A_INDEX = 10;
const int CREATE_SEMAPHORE_EX_W_INDEX = 11;
const int CREATE_WAITABLE_TIMER_A_INDEX = 12;
const int CREATE_WAITABLE_TIMER_W_INDEX = 13;
const int CREATE_WAITABLE_TIMER_EX_A_INDEX = 14;
const int CREATE_WAITABLE_TIMER_EX_W_INDEX = 15;
const int CREATE_THREAD_INDEX = 16;
const int CREATE_REMOTE_THREAD_INDEX = 17;
const int CREATE_FILE_A_INDEX = 18;
const int CREATE_FILE_W_INDEX = 19;
const int CREATE_FILE_MAPPING_A_INDEX = 20;
const int CREATE_FILE_MAPPING_W_INDEX = 21;
const int CREATE_FILE_MAPPING_NUMA_A_INDEX = 22;
const int CREATE_FILE_MAPPING_NUMA_W_INDEX = 23;
const int CREATE_PIPE_INDEX = 24;
const int CREATE_NAMED_PIPE_A_INDEX = 25;
const int CREATE_NAMED_PIPE_W_INDEX = 26;
const int HEAP_CREATE_INDEX = 27;
const int CREATE_JOB_OBJECT_A_INDEX = 28;
const int CREATE_JOB_OBJECT_W_INDEX = 29;
const int CREATE_CONSOLE_SCREEN_BUFFER_INDEX = 30;
const int CREATE_MEMORY_RESOURCE_NOTIFICATION_INDEX = 31;
const int CREATE_THREADPOOL_TIMER_INDEX = 32;
const int CREATE_THREADPOOL_WAIT_INDEX = 33;
const int CREATE_THREADPOOL_IO_INDEX = 34;
const int CREATE_THREADPOOL_WORK_INDEX = 35;
const int CLOSE_HANDLE_INDEX = 36;

const int MAX_HANDLE_FUNCTIONS = 37;

// Forward declarations
HANDLE WINAPI Perftools_CreateEventA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCSTR lpName);
    
HANDLE WINAPI Perftools_CreateEventW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCWSTR lpName);
    
HANDLE WINAPI Perftools_CreateEventExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateEventExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateMutexA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_ BOOL bInitialOwner,
    _In_opt_ LPCSTR lpName);
    
HANDLE WINAPI Perftools_CreateMutexW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_ BOOL bInitialOwner,
    _In_opt_ LPCWSTR lpName);
    
HANDLE WINAPI Perftools_CreateMutexExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateMutexExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateSemaphoreA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCSTR lpName);
    
HANDLE WINAPI Perftools_CreateSemaphoreW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCWSTR lpName);
    
HANDLE WINAPI Perftools_CreateSemaphoreExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateSemaphoreExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateWaitableTimerA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_ BOOL bManualReset,
    _In_opt_ LPCSTR lpTimerName);
    
HANDLE WINAPI Perftools_CreateWaitableTimerW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_ BOOL bManualReset,
    _In_opt_ LPCWSTR lpTimerName);
    
HANDLE WINAPI Perftools_CreateWaitableTimerExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_opt_ LPCSTR lpTimerName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateWaitableTimerExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_opt_ LPCWSTR lpTimerName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess);
    
HANDLE WINAPI Perftools_CreateThread(
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ SIZE_T dwStackSize,
    _In_ LPTHREAD_START_ROUTINE lpStartAddress,
    _In_opt_ LPVOID lpParameter,
    _In_ DWORD dwCreationFlags,
    _Out_opt_ LPDWORD lpThreadId);
    
HANDLE WINAPI Perftools_CreateRemoteThread(
    _In_ HANDLE hProcess,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ SIZE_T dwStackSize,
    _In_ LPTHREAD_START_ROUTINE lpStartAddress,
    _In_opt_ LPVOID lpParameter,
    _In_ DWORD dwCreationFlags,
    _Out_opt_ LPDWORD lpThreadId);
    
HANDLE WINAPI Perftools_CreateFileA(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile);
    
HANDLE WINAPI Perftools_CreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile);
    
HANDLE WINAPI Perftools_CreateFileMappingA(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCSTR lpName);
    
HANDLE WINAPI Perftools_CreateFileMappingW(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCWSTR lpName);
    
HANDLE WINAPI Perftools_CreateFileMappingNumaA(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD nndPreferred);
    
HANDLE WINAPI Perftools_CreateFileMappingNumaW(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD nndPreferred);
    
BOOL WINAPI Perftools_CreatePipe(
    _Out_ PHANDLE hReadPipe,
    _Out_ PHANDLE hWritePipe,
    _In_opt_ LPSECURITY_ATTRIBUTES lpPipeAttributes,
    _In_ DWORD nSize);
    
HANDLE WINAPI Perftools_CreateNamedPipeA(
    _In_ LPCSTR lpName,
    _In_ DWORD dwOpenMode,
    _In_ DWORD dwPipeMode,
    _In_ DWORD nMaxInstances,
    _In_ DWORD nOutBufferSize,
    _In_ DWORD nInBufferSize,
    _In_ DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes);
    
HANDLE WINAPI Perftools_CreateNamedPipeW(
    _In_ LPCWSTR lpName,
    _In_ DWORD dwOpenMode,
    _In_ DWORD dwPipeMode,
    _In_ DWORD nMaxInstances,
    _In_ DWORD nOutBufferSize,
    _In_ DWORD nInBufferSize,
    _In_ DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes);
    
HANDLE WINAPI Perftools_HeapCreate(
    _In_ DWORD flOptions,
    _In_ SIZE_T dwInitialSize,
    _In_ SIZE_T dwMaximumSize);
    
HANDLE WINAPI Perftools_CreateJobObjectA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpJobAttributes,
    _In_opt_ LPCSTR lpName);
    
HANDLE WINAPI Perftools_CreateJobObjectW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpJobAttributes,
    _In_opt_ LPCWSTR lpName);
    
HANDLE WINAPI Perftools_CreateConsoleScreenBuffer(
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwFlags,
    _In_opt_ LPVOID lpScreenBufferData);
    
HANDLE WINAPI Perftools_CreateMemoryResourceNotification(
    _In_ MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType);
    
PTP_TIMER WINAPI Perftools_CreateThreadpoolTimer(
    _In_ PTP_TIMER_CALLBACK pfnti,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe);
    
PTP_WAIT WINAPI Perftools_CreateThreadpoolWait(
    _In_ PTP_WAIT_CALLBACK pfnwa,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe);
    
PTP_IO WINAPI Perftools_CreateThreadpoolIo(
    _In_ HANDLE fl,
    _In_ PTP_WIN32_IO_CALLBACK pfnio,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe);
    
PTP_WORK WINAPI Perftools_CreateThreadpoolWork(
    _In_ PTP_WORK_CALLBACK pfnwk,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe);
    
BOOL WINAPI Perftools_CloseHandle(
    _In_ HANDLE hObject);

typedef void (*GenericFnPtr)();

struct HandleFunctionInfo {
  const char* name;
  GenericFnPtr windows_fn;
  GenericFnPtr origstub_fn;  // This will now hold the original function pointer from Detours
  GenericFnPtr perftools_fn;
};

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

// Handle patching functions using Detours
void PatchHandleFunctions() {
  HMODULE kernel32_module = ::GetModuleHandleA("kernel32.dll");
  if (kernel32_module == nullptr) {
    return;
  }

  // Begin Detours transaction
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());

  // Unlike for libc, we know these exist in our module, so we can get
  // and patch at the same time.
  for (int i = 0; i < MAX_HANDLE_FUNCTIONS; i++) {
    handle_function_info_[i].windows_fn = (GenericFnPtr)
        ::GetProcAddress(kernel32_module, handle_function_info_[i].name);
    
    if (handle_function_info_[i].windows_fn == nullptr) {
      // Skip functions that don't exist in this version of kernel32
      continue;
    }
    
    // Use Detours to patch the function
    LONG result = DetourAttach(&(PVOID&)handle_function_info_[i].windows_fn,
                              (PVOID)handle_function_info_[i].perftools_fn);
    
    if (result != NO_ERROR) {
      HANDLE_TRACE(true,
        std::cout << "Failed to patch " << handle_function_info_[i].name 
                  << " with error code: " << result << std::endl;
      );
    } else {
      HANDLE_TRACE(true,
        std::cout << "Successfully patched " << handle_function_info_[i].name << std::endl;
      );
    }
  }
  
  // Commit Detours transaction
  DetourTransactionCommit();
  
  // NOW save the original function pointers after commit
  // Only after DetourTransactionCommit() do we have the correct trampoline addresses
  for (int i = 0; i < MAX_HANDLE_FUNCTIONS; i++) {
    if (handle_function_info_[i].windows_fn && handle_function_info_[i].perftools_fn) {
      // The windows_fn now points to the trampoline, so we need to save the original
      handle_function_info_[i].origstub_fn = handle_function_info_[i].windows_fn;
    }
  }
}

void UnpatchHandleFunctions() {
  // Begin Detours transaction
  DetourTransactionBegin();
  DetourUpdateThread(GetCurrentThread());

  // Unpatch each handle function
  for (int i = 0; i < MAX_HANDLE_FUNCTIONS; i++) {
    if (handle_function_info_[i].windows_fn && handle_function_info_[i].perftools_fn) {
      LONG result = DetourDetach(&(PVOID&)handle_function_info_[i].windows_fn,
                                (PVOID)handle_function_info_[i].perftools_fn);
      
      if (result != NO_ERROR) {
        HANDLE_TRACE(true,
          std::cout << "Failed to unpatch " << handle_function_info_[i].name 
                    << " with error code: " << result << std::endl;
        );
      }
    }
  }
  
  // Commit Detours transaction
  DetourTransactionCommit();
}

// Handle function hook implementations with default behavior
HANDLE WINAPI Perftools_CreateEventA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCSTR lpName) {
  HANDLE_TRACE(true, 
    std::cout << "CreateEventA called with lpName=" << (lpName ? lpName : "NULL") 
              << ", bManualReset=" << bManualReset << ", bInitialState=" << bInitialState << std::endl;
  );
  
  // Safety check to ensure origstub_fn is valid before calling
  if (handle_function_info_[CREATE_EVENT_A_INDEX].origstub_fn == nullptr) {
    std::cerr << "Error: origstub_fn is null for CreateEventA" << std::endl;
    return nullptr;
  }
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCSTR))
                  handle_function_info_[CREATE_EVENT_A_INDEX].origstub_fn)(
                  lpEventAttributes, bManualReset, bInitialState, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateEventA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateEventW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_ BOOL bManualReset,
    _In_ BOOL bInitialState,
    _In_opt_ LPCWSTR lpName) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateEventW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", bManualReset=" << bManualReset << L", bInitialState=" << bInitialState << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCWSTR))
                  handle_function_info_[CREATE_EVENT_W_INDEX].origstub_fn)(
                  lpEventAttributes, bManualReset, bInitialState, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateEventW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateEventExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::cout << "CreateEventExA called with lpName=" << (lpName ? lpName : "NULL")
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_EVENT_EX_A_INDEX].origstub_fn)(
                  lpEventAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateEventExA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateEventExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateEventExW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_EVENT_EX_W_INDEX].origstub_fn)(
                  lpEventAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateEventExW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_ BOOL bInitialOwner,
    _In_opt_ LPCSTR lpName) {
  HANDLE_TRACE(true,
    std::cout << "CreateMutexA called with lpName=" << (lpName ? lpName : "NULL")
              << ", bInitialOwner=" << bInitialOwner << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR))
                  handle_function_info_[CREATE_MUTEX_A_INDEX].origstub_fn)(
                  lpMutexAttributes, bInitialOwner, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateMutexA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_ BOOL bInitialOwner,
    _In_opt_ LPCWSTR lpName) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateMutexW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", bInitialOwner=" << bInitialOwner << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCWSTR))
                  handle_function_info_[CREATE_MUTEX_W_INDEX].origstub_fn)(
                  lpMutexAttributes, bInitialOwner, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateMutexW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::cout << "CreateMutexExA called with lpName=" << (lpName ? lpName : "NULL")
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_MUTEX_EX_A_INDEX].origstub_fn)(
                  lpMutexAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateMutexExA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateMutexExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateMutexExW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_MUTEX_EX_W_INDEX].origstub_fn)(
                  lpMutexAttributes, lpName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateMutexExW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCSTR lpName) {
  HANDLE_TRACE(true,
    std::cout << "CreateSemaphoreA called with lpName=" << (lpName ? lpName : "NULL")
              << ", lInitialCount=" << lInitialCount << ", lMaximumCount=" << lMaximumCount << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCSTR))
                  handle_function_info_[CREATE_SEMAPHORE_A_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateSemaphoreA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCWSTR lpName) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateSemaphoreW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", lInitialCount=" << lInitialCount << L", lMaximumCount=" << lMaximumCount << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCWSTR))
                  handle_function_info_[CREATE_SEMAPHORE_W_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateSemaphoreW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::cout << "CreateSemaphoreExA called with lpName=" << (lpName ? lpName : "NULL")
              << ", lInitialCount=" << lInitialCount << ", lMaximumCount=" << lMaximumCount
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_SEMAPHORE_EX_A_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateSemaphoreExA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateSemaphoreExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    _In_ LONG lInitialCount,
    _In_ LONG lMaximumCount,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateSemaphoreExW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", lInitialCount=" << lInitialCount << L", lMaximumCount=" << lMaximumCount
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_SEMAPHORE_EX_W_INDEX].origstub_fn)(
                  lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateSemaphoreExW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_ BOOL bManualReset,
    _In_opt_ LPCSTR lpTimerName) {
  HANDLE_TRACE(true,
    std::cout << "CreateWaitableTimerA called with lpTimerName=" << (lpTimerName ? lpTimerName : "NULL")
              << ", bManualReset=" << bManualReset << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR))
                  handle_function_info_[CREATE_WAITABLE_TIMER_A_INDEX].origstub_fn)(
                  lpTimerAttributes, bManualReset, lpTimerName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateWaitableTimerA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_ BOOL bManualReset,
    _In_opt_ LPCWSTR lpTimerName) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateWaitableTimerW called with lpTimerName=" << (lpTimerName ? lpTimerName : L"NULL")
               << L", bManualReset=" << bManualReset << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, BOOL, LPCWSTR))
                  handle_function_info_[CREATE_WAITABLE_TIMER_W_INDEX].origstub_fn)(
                  lpTimerAttributes, bManualReset, lpTimerName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateWaitableTimerW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerExA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_opt_ LPCSTR lpTimerName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::cout << "CreateWaitableTimerExA called with lpTimerName=" << (lpTimerName ? lpTimerName : "NULL")
              << ", dwFlags=" << dwFlags << ", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_WAITABLE_TIMER_EX_A_INDEX].origstub_fn)(
                  lpTimerAttributes, lpTimerName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateWaitableTimerExA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateWaitableTimerExW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpTimerAttributes,
    _In_opt_ LPCWSTR lpTimerName,
    _In_ DWORD dwFlags,
    _In_ DWORD dwDesiredAccess) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateWaitableTimerExW called with lpTimerName=" << (lpTimerName ? lpTimerName : L"NULL")
               << L", dwFlags=" << dwFlags << L", dwDesiredAccess=" << dwDesiredAccess << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR, DWORD, DWORD))
                  handle_function_info_[CREATE_WAITABLE_TIMER_EX_W_INDEX].origstub_fn)(
                  lpTimerAttributes, lpTimerName, dwFlags, dwDesiredAccess);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateWaitableTimerExW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateThread(
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ SIZE_T dwStackSize,
    _In_ LPTHREAD_START_ROUTINE lpStartAddress,
    _In_opt_ LPVOID lpParameter,
    _In_ DWORD dwCreationFlags,
    _Out_opt_ LPDWORD lpThreadId) {
  HANDLE_TRACE(true,
    std::cout << "CreateThread called with dwStackSize=" << dwStackSize
              << ", dwCreationFlags=" << dwCreationFlags << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD))
                  handle_function_info_[CREATE_THREAD_INDEX].origstub_fn)(
                  lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateThread returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateRemoteThread(
    _In_ HANDLE hProcess,
    _In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    _In_ SIZE_T dwStackSize,
    _In_ LPTHREAD_START_ROUTINE lpStartAddress,
    _In_opt_ LPVOID lpParameter,
    _In_ DWORD dwCreationFlags,
    _Out_opt_ LPDWORD lpThreadId) {
  HANDLE_TRACE(true,
    std::cout << "CreateRemoteThread called with hProcess=" << reinterpret_cast<uintptr_t>(hProcess)
              << ", dwStackSize=" << dwStackSize << ", dwCreationFlags=" << dwCreationFlags << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD))
                  handle_function_info_[CREATE_REMOTE_THREAD_INDEX].origstub_fn)(
                  hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateRemoteThread returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileA(
    _In_ LPCSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile) {
  HANDLE_TRACE(true,
    std::cout << "CreateFileA called with lpFileName=" << (lpFileName ? lpFileName : "NULL")
              << ", dwDesiredAccess=" << dwDesiredAccess << ", dwShareMode=" << dwShareMode
              << ", dwCreationDisposition=" << dwCreationDisposition << ", dwFlagsAndAttributes=" << dwFlagsAndAttributes << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))
                  handle_function_info_[CREATE_FILE_A_INDEX].origstub_fn)(
                  lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateFileA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateFileW called with lpFileName=" << (lpFileName ? lpFileName : L"NULL")
               << L", dwDesiredAccess=" << dwDesiredAccess << L", dwShareMode=" << dwShareMode
               << L", dwCreationDisposition=" << dwCreationDisposition << L", dwFlagsAndAttributes=" << dwFlagsAndAttributes << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))
                  handle_function_info_[CREATE_FILE_W_INDEX].origstub_fn)(
                  lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateFileW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingA(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCSTR lpName) {
  HANDLE_TRACE(true,
    std::cout << "CreateFileMappingA called with hFile=" << reinterpret_cast<uintptr_t>(hFile)
              << ", flProtect=" << flProtect << ", dwMaximumSizeHigh=" << dwMaximumSizeHigh
              << ", dwMaximumSizeLow=" << dwMaximumSizeLow << ", lpName=" << (lpName ? lpName : "NULL") << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCSTR))
                  handle_function_info_[CREATE_FILE_MAPPING_A_INDEX].origstub_fn)(
                  hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateFileMappingA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingW(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCWSTR lpName) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateFileMappingW called with hFile=" << reinterpret_cast<uintptr_t>(hFile)
               << L", flProtect=" << flProtect << L", dwMaximumSizeHigh=" << dwMaximumSizeHigh
               << L", dwMaximumSizeLow=" << dwMaximumSizeLow << L", lpName=" << (lpName ? lpName : L"NULL") << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCWSTR))
                  handle_function_info_[CREATE_FILE_MAPPING_W_INDEX].origstub_fn)(
                  hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateFileMappingW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingNumaA(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCSTR lpName,
    _In_ DWORD nndPreferred) {
  HANDLE_TRACE(true,
    std::cout << "CreateFileMappingNumaA called with hFile=" << reinterpret_cast<uintptr_t>(hFile)
              << ", flProtect=" << flProtect << ", dwMaximumSizeHigh=" << dwMaximumSizeHigh
              << ", dwMaximumSizeLow=" << dwMaximumSizeLow << ", lpName=" << (lpName ? lpName : "NULL")
              << ", nndPreferred=" << nndPreferred << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCSTR, DWORD))
                  handle_function_info_[CREATE_FILE_MAPPING_NUMA_A_INDEX].origstub_fn)(
                  hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName, nndPreferred);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateFileMappingNumaA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateFileMappingNumaW(
    _In_ HANDLE hFile,
    _In_opt_ LPSECURITY_ATTRIBUTES lpFileMappingAttributes,
    _In_ DWORD flProtect,
    _In_ DWORD dwMaximumSizeHigh,
    _In_ DWORD dwMaximumSizeLow,
    _In_opt_ LPCWSTR lpName,
    _In_ DWORD nndPreferred) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateFileMappingNumaW called with hFile=" << reinterpret_cast<uintptr_t>(hFile)
               << L", flProtect=" << flProtect << L", dwMaximumSizeHigh=" << dwMaximumSizeHigh
               << L", dwMaximumSizeLow=" << dwMaximumSizeLow << L", lpName=" << (lpName ? lpName : L"NULL")
               << L", nndPreferred=" << nndPreferred << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(HANDLE, LPSECURITY_ATTRIBUTES, DWORD, DWORD, DWORD, LPCWSTR, DWORD))
                  handle_function_info_[CREATE_FILE_MAPPING_NUMA_W_INDEX].origstub_fn)(
                  hFile, lpFileMappingAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName, nndPreferred);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateFileMappingNumaW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

BOOL WINAPI Perftools_CreatePipe(
    _Out_ PHANDLE hReadPipe,
    _Out_ PHANDLE hWritePipe,
    _In_opt_ LPSECURITY_ATTRIBUTES lpPipeAttributes,
    _In_ DWORD nSize) {
  HANDLE_TRACE(true,
    std::cout << "CreatePipe called with nSize=" << nSize << std::endl;
  );
  
  BOOL result = ((BOOL (WINAPI *)(PHANDLE, PHANDLE, LPSECURITY_ATTRIBUTES, DWORD))
                handle_function_info_[CREATE_PIPE_INDEX].origstub_fn)(
                hReadPipe, hWritePipe, lpPipeAttributes, nSize);
  
  HANDLE_TRACE(true,
    std::cout << "CreatePipe returned " << result << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateNamedPipeA(
    _In_ LPCSTR lpName,
    _In_ DWORD dwOpenMode,
    _In_ DWORD dwPipeMode,
    _In_ DWORD nMaxInstances,
    _In_ DWORD nOutBufferSize,
    _In_ DWORD nInBufferSize,
    _In_ DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes) {
  HANDLE_TRACE(true,
    std::cout << "CreateNamedPipeA called with lpName=" << (lpName ? lpName : "NULL")
              << ", dwOpenMode=" << dwOpenMode << ", dwPipeMode=" << dwPipeMode
              << ", nMaxInstances=" << nMaxInstances << ", nOutBufferSize=" << nOutBufferSize
              << ", nInBufferSize=" << nInBufferSize << ", nDefaultTimeOut=" << nDefaultTimeOut << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES))
                  handle_function_info_[CREATE_NAMED_PIPE_A_INDEX].origstub_fn)(
                  lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateNamedPipeA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateNamedPipeW(
    _In_ LPCWSTR lpName,
    _In_ DWORD dwOpenMode,
    _In_ DWORD dwPipeMode,
    _In_ DWORD nMaxInstances,
    _In_ DWORD nOutBufferSize,
    _In_ DWORD nInBufferSize,
    _In_ DWORD nDefaultTimeOut,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateNamedPipeW called with lpName=" << (lpName ? lpName : L"NULL")
               << L", dwOpenMode=" << dwOpenMode << L", dwPipeMode=" << dwPipeMode
               << L", nMaxInstances=" << nMaxInstances << L", nOutBufferSize=" << nOutBufferSize
               << L", nInBufferSize=" << nInBufferSize << L", nDefaultTimeOut=" << nDefaultTimeOut << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPCWSTR, DWORD, DWORD, DWORD, DWORD, DWORD, DWORD, LPSECURITY_ATTRIBUTES))
                  handle_function_info_[CREATE_NAMED_PIPE_W_INDEX].origstub_fn)(
                  lpName, dwOpenMode, dwPipeMode, nMaxInstances, nOutBufferSize, nInBufferSize, nDefaultTimeOut, lpSecurityAttributes);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateNamedPipeW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_HeapCreate(
    _In_ DWORD flOptions,
    _In_ SIZE_T dwInitialSize,
    _In_ SIZE_T dwMaximumSize) {
  HANDLE_TRACE(true,
    std::cout << "HeapCreate called with flOptions=" << flOptions
              << ", dwInitialSize=" << dwInitialSize << ", dwMaximumSize=" << dwMaximumSize << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(DWORD, SIZE_T, SIZE_T))
                  handle_function_info_[HEAP_CREATE_INDEX].origstub_fn)(
                  flOptions, dwInitialSize, dwMaximumSize);
                  
  HANDLE_TRACE(true,
    std::cout << "HeapCreate returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateJobObjectA(
    _In_opt_ LPSECURITY_ATTRIBUTES lpJobAttributes,
    _In_opt_ LPCSTR lpName) {
  HANDLE_TRACE(true,
    std::cout << "CreateJobObjectA called with lpName=" << (lpName ? lpName : "NULL") << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCSTR))
                  handle_function_info_[CREATE_JOB_OBJECT_A_INDEX].origstub_fn)(
                  lpJobAttributes, lpName);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateJobObjectA returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateJobObjectW(
    _In_opt_ LPSECURITY_ATTRIBUTES lpJobAttributes,
    _In_opt_ LPCWSTR lpName) {
  HANDLE_TRACE(true,
    std::wcout << L"CreateJobObjectW called with lpName=" << (lpName ? lpName : L"NULL") << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(LPSECURITY_ATTRIBUTES, LPCWSTR))
                  handle_function_info_[CREATE_JOB_OBJECT_W_INDEX].origstub_fn)(
                  lpJobAttributes, lpName);
                  
  HANDLE_TRACE(true,
      std::cout << "CreateJobObjectW returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateConsoleScreenBuffer(
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwFlags,
    _In_opt_ LPVOID lpScreenBufferData) {
  HANDLE_TRACE(true,
    std::cout << "CreateConsoleScreenBuffer called with dwDesiredAccess=" << dwDesiredAccess
              << ", dwShareMode=" << dwShareMode << ", dwFlags=" << dwFlags << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, LPVOID))
                  handle_function_info_[CREATE_CONSOLE_SCREEN_BUFFER_INDEX].origstub_fn)(
                  dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwFlags, lpScreenBufferData);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateConsoleScreenBuffer returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

HANDLE WINAPI Perftools_CreateMemoryResourceNotification(
    _In_ MEMORY_RESOURCE_NOTIFICATION_TYPE NotificationType) {
  HANDLE_TRACE(true,
    std::cout << "CreateMemoryResourceNotification called with NotificationType=" << NotificationType << std::endl;
  );
  
  HANDLE result = ((HANDLE (WINAPI *)(MEMORY_RESOURCE_NOTIFICATION_TYPE))
                  handle_function_info_[CREATE_MEMORY_RESOURCE_NOTIFICATION_INDEX].origstub_fn)(
                  NotificationType);
                  
  HANDLE_TRACE(true,
      std::cout << "CreateMemoryResourceNotification returned " << reinterpret_cast<uintptr_t>(result) << std::endl;
  );
  
  return result;
}

PTP_TIMER WINAPI Perftools_CreateThreadpoolTimer(
    _In_ PTP_TIMER_CALLBACK pfnti,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe) {
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolTimer called with pfnti=" << pfnti
              << ", pv=" << pv << std::endl;
  );
  
  PTP_TIMER result = ((PTP_TIMER (WINAPI *)(PTP_TIMER_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_TIMER_INDEX].origstub_fn)(
                  pfnti, pv, pcbe);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolTimer returned " << result << std::endl;
  );
  
  return result;
}

PTP_WAIT WINAPI Perftools_CreateThreadpoolWait(
    _In_ PTP_WAIT_CALLBACK pfnwa,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe) {
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolWait called with pfnWaitCallback=" << pfnwa
              << ", pv=" << pv << std::endl;
  );
  
  PTP_WAIT result = ((PTP_WAIT (WINAPI *)(PTP_WAIT_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_WAIT_INDEX].origstub_fn)(
                  pfnwa, pv, pcbe);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolWait returned " << result << std::endl;
  );
  
  return result;
}

PTP_IO WINAPI Perftools_CreateThreadpoolIo(
    _In_ HANDLE fl,
    _In_ PTP_WIN32_IO_CALLBACK pfnio,
    _Inout_opt_ PVOID pv,
    _In_opt_ PTP_CALLBACK_ENVIRON pcbe) {
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolIo called with fl=" << reinterpret_cast<uintptr_t>(fl) << ", pfnio=" << pfnio
              << ", pv=" << pv << std::endl;
  );
  
  PTP_IO result = ((PTP_IO (WINAPI *)(HANDLE, PTP_WIN32_IO_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_IO_INDEX].origstub_fn)(
                  fl, pfnio, pv, pcbe);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolIo returned " << result << std::endl;
  );
  
  return result;
}

PTP_WORK WINAPI Perftools_CreateThreadpoolWork(
    PTP_WORK_CALLBACK pfnWorkCallback,
    PVOID pv,
    PTP_CALLBACK_ENVIRON pcbe) {
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolWork called with pfnWorkCallback=" << pfnWorkCallback
              << ", pv=" << pv << std::endl;
  );
  
  PTP_WORK result = ((PTP_WORK (WINAPI *)(PTP_WORK_CALLBACK, PVOID, PTP_CALLBACK_ENVIRON))
                  handle_function_info_[CREATE_THREADPOOL_WORK_INDEX].origstub_fn)(
                  pfnWorkCallback, pv, pcbe);
                  
  HANDLE_TRACE(true,
    std::cout << "CreateThreadpoolWork returned " << result << std::endl;
  );
  
  return result;
}

BOOL WINAPI Perftools_CloseHandle(HANDLE hObject) {
  HANDLE_TRACE(true,
    std::cout << "CloseHandle called with hObject=" << reinterpret_cast<uintptr_t>(hObject) << std::endl;
  );
  
  BOOL result = ((BOOL (WINAPI *)(HANDLE))
                handle_function_info_[CLOSE_HANDLE_INDEX].origstub_fn)(
                hObject);
  
  HANDLE_TRACE(true,
    std::cout << "CloseHandle returned " << result << std::endl;
  );
  
  return result;
}


#endif