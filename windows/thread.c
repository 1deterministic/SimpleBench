#include "simplebench.h"
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

// struct of the Thread type
struct thd {
    HANDLE thread_instance;
    Thread* next;
};

// this will add a thread to a thread array
MsgCode add_thread(Thread** thread_array, int core_number, int priority, void* function, void* params) {
    // manually allocate memory the size of Thread
    Thread* new_thread = (Thread*) malloc(sizeof(Thread));
    if (new_thread == NULL)
        return THREAD_MEMORY_ALLOCATION_ERROR;

    SYSTEM_INFO info; 
	GetSystemInfo(&info);
    
    // start the thread with the received parameters
    new_thread->thread_instance = CreateThread(NULL, 0, function, params, 0, NULL);
    if (!new_thread->thread_instance) {
        free(new_thread);
        return THREAD_PTHREAD_CREATION_ERROR;
    }
    
    // makes this thread run on the specified core
    if (!SetThreadAffinityMask(new_thread->thread_instance, 1 << (core_number % info.dwNumberOfProcessors))) {
        TerminateThread(new_thread->thread_instance, 0);
        free(new_thread);
        return THREAD_PTHREAD_AFFINITY_ERROR;
    }

    if (!SetThreadPriority(new_thread->thread_instance, priority)) {
        TerminateThread(new_thread->thread_instance, 0);
        free(new_thread);
        return THREAD_PTHREAD_PRIORITY_ERROR;
    }
    
    // links the rest of the array to this thread
    new_thread->next = *thread_array;
    *thread_array = new_thread;
    
    return SUCCESS;
}

// frees up all threads in a thread array
MsgCode del_threads(Thread** thread_array) {
    // auxiliary pointer
    Thread* thread_element;
    // will repeat until all threads were removed
    while (*thread_array!= NULL) {
        // changes the pointers
        thread_element = *thread_array;
        *thread_array = (*thread_array)->next;
        // frees up the current thread
        CloseHandle(thread_element);
        free(thread_element);
    }
    
    // returns the new empty array
    return SUCCESS;
}

// blocks the execution until all threads finish their tasks
MsgCode wait_threads(Thread** thread_array) {
    // auxiliary pointer
    Thread* thread_element = *thread_array;
    // will repeat until all threads were "joined"
    while (thread_element != NULL) {
        // "joins" the current thread
        if(WaitForSingleObject(thread_element->thread_instance, INFINITE) == WAIT_FAILED)
            return THREAD_PTHREAD_JOIN_ERROR;
        
        // goes to the next
        thread_element = thread_element->next;
    }

    return SUCCESS;
}

// stops currently running threads
MsgCode stop_threads(Thread** thread_array) {
    // auxiliary pointer
    Thread* thread_element;
    // will repeat until all threads were removed
    while (*thread_array!= NULL) {
        // changes the pointers
        thread_element = *thread_array;
        *thread_array = (*thread_array)->next;

        TerminateThread(thread_element->thread_instance, 0);
    }
    
    // returns the new empty array
    return SUCCESS;
}