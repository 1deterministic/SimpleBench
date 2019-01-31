#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sched.h>

// struct of the Thread type
struct thd {
    pthread_t thread_instance;
    Thread* next;
};

// this will add a thread to a thread array
MsgCode add_thread(Thread** thread_array, int core_number, void* function, void* params) {
    // manually allocate memory the size of Thread
    Thread* new_thread = (Thread*) malloc(sizeof(Thread));
    if (new_thread == NULL)
        return THREAD_MEMORY_ALLOCATION_ERROR;
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_number % sysconf(_SC_NPROCESSORS_ONLN), &cpuset);
    
    // start the thread with the received parameters
    if (pthread_create(&(new_thread->thread_instance), NULL, (void*(*)(void*)) function, params)) {
        free(new_thread);
        return THREAD_PTHREAD_CREATION_ERROR;
    }
    
    // makes this thread run on the specified core
    if (pthread_setaffinity_np(new_thread->thread_instance, sizeof(cpu_set_t), &cpuset)) {
        pthread_cancel(new_thread->thread_instance);
        free(new_thread);
        return THREAD_PTHREAD_AFFINITY_ERROR;
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
        if (pthread_join(thread_element->thread_instance, NULL))
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

        pthread_cancel(thread_element->thread_instance);
    }
    
    // returns the new empty array
    return SUCCESS;
}