#include "simplebench.h"
#include <stdlib.h>
#include <stdbool.h>

#ifdef __linux__
    #include <pthread.h>
    #include <unistd.h>
    #include <sched.h>

#elif __MINGW64__ || __MINGW32__ || _WIN32
    #include <windows.h>

#endif

// struct of the Thread type
struct thd {
    // can point to pthread_t* or HANDLE* depending on the platform being compiled to
    void* thread_instance;
    Thread* next;
};

// this will add a thread to a thread array
MsgCode add_thread(Thread** thread_array, int core_number, int priority, void* function, void* params) {
    // manually allocate memory the size of Thread
    Thread* new_thread = (Thread*) malloc(sizeof(Thread));
    if (new_thread == NULL)
        return THREAD_MEMORY_ALLOCATION_ERROR;

    #ifdef __linux__
        //new_thread->thread_instance = (void*) ((pthread_t*) malloc(sizeof(pthread_t)));
        pthread_t* thread_instance = (pthread_t*) malloc(sizeof(pthread_t));
        if (thread_instance == NULL)
            return THREAD_MEMORY_ALLOCATION_ERROR;
        
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(core_number % sysconf(_SC_NPROCESSORS_ONLN), &cpuset);
        
        // start the thread with the received parameters
        if (pthread_create(thread_instance, NULL, function, params)) {
            free(new_thread);
            return THREAD_PTHREAD_CREATION_ERROR;
        }
        
        // makes this thread run on the specified core
        if (pthread_setaffinity_np(*thread_instance, sizeof(cpu_set_t), &cpuset)) {
            pthread_cancel(*thread_instance);
            free(new_thread);
            return THREAD_PTHREAD_AFFINITY_ERROR;
        }

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* thread_instance = (HANDLE*) malloc(sizeof(HANDLE));
        if (thread_instance == NULL)
            return THREAD_PTHREAD_CREATION_ERROR;

        SYSTEM_INFO info; 
        GetSystemInfo(&info);
        
        // start the thread with the received parameters
        *thread_instance = CreateThread(NULL, 0, function, params, 0, NULL);
        if (!*thread_instance) {
            free(new_thread);
            return THREAD_PTHREAD_CREATION_ERROR;
        }
        
        // makes this thread run on the specified core
        if (!SetThreadAffinityMask(*thread_instance, 1 << (core_number % info.dwNumberOfProcessors))) {
            TerminateThread(*thread_instance, 0);
            free(new_thread);
            return THREAD_PTHREAD_AFFINITY_ERROR;
        }

        if (!SetThreadPriority(*thread_instance, priority)) {
            TerminateThread(*thread_instance, 0);
            free(new_thread);
            return THREAD_PTHREAD_PRIORITY_ERROR;
        }
    #endif
    
    set_thread_instance(&new_thread, thread_instance);
    set_next(&new_thread, *thread_array);
    *thread_array = new_thread;
    return SUCCESS;
}

// frees up all threads in a thread array
MsgCode del_threads(Thread** thread_array) {
    // auxiliary pointer
    Thread* thread_element = *thread_array;
    // will repeat until all threads were removed
    while (*thread_array != NULL) {
        // stores the current thread on a separate variable and advances the thread array to the next element
        thread_element = *thread_array;
        *thread_array = (*thread_array)->next;
        
        #ifdef __linux__
        #elif __MINGW64__ || __MINGW32__ || _WIN32
            CloseHandle(*((HANDLE*) thread_element->thread_instance));

        #endif

        // frees up the current thread
        free(thread_element->thread_instance);
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
        #ifdef __linux__
            pthread_t* thread_instance = get_thread_instance(&thread_element);
            // "joins" the current thread
            if (pthread_join(*thread_instance, NULL))
                return THREAD_PTHREAD_JOIN_ERROR;

        #elif __MINGW64__ || __MINGW32__ || _WIN32
            HANDLE* thread_instance = get_thread_instance(&thread_element);
            // "joins" the current thread
            if(WaitForSingleObject(*thread_instance, INFINITE) == WAIT_FAILED)
                return THREAD_PTHREAD_JOIN_ERROR;
                
        #endif
        
        // goes to the next
        thread_element = thread_element->next;
    }

    return SUCCESS;
}

// stops currently running threads
MsgCode stop_threads(Thread** thread_array) {
    // auxiliary pointer
    Thread* thread_element = *thread_array;
    // will repeat until all threads were removed
    while (thread_element!= NULL) {
        #ifdef __linux__
            pthread_t* thread_instance = get_thread_instance(&thread_element);
            pthread_cancel(*thread_instance);

        #elif __MINGW64__ || __MINGW32__ || _WIN32
            HANDLE* thread_instance = get_thread_instance(&thread_element);
            TerminateThread(*thread_instance, 0);

        #endif

        // goes to the next
        thread_element = thread_element->next;
    }
    
    // returns the new empty array
    return SUCCESS;
}

void set_thread_instance(Thread** thread_element, void* thread_instance) {
    (*thread_element)->thread_instance = thread_instance;
}

void* get_thread_instance(Thread** thread_element) {
    return (*thread_element)->thread_instance;
}

void set_next(Thread** thread_element, Thread* next) {
    (*thread_element)->next = next;
}

Thread* get_next(Thread** thread_element) {
    return (*thread_element)->next;
}