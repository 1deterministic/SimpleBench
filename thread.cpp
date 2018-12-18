// Thread type ======================================================

#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// struct of the Thread type
struct thd {
    pthread_t thread_instance;
    Thread* next;
};

// this will add a thread to a thread array
Thread* add_thread(Thread* thread_array, int core_number, void* function, void* params) {
    // manually allocate memory the size of Thread
    Thread* aux = (Thread*) malloc(sizeof(Thread));
    
    // return right away if the allocation failed
    if (aux == NULL)
        return thread_array;
    
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_number, &cpuset);
    
    // start the thread with the received parameters
    if (pthread_create(&(aux->thread_instance), NULL, (void*(*)(void*)) function, params)) {
        free(aux);
        return thread_array;
    }
    
    // makes this thread run on the specified core
    if (pthread_setaffinity_np(aux->thread_instance, sizeof(cpu_set_t), &cpuset)) {
        free(aux);
        return thread_array;
    }
    
    // links the rest of the array to this thread
    aux->next = thread_array;
    
    // returns this thread (which is now the new first element)
    return aux;
}

// frees up all threads in a thread array
Thread* del_threads(Thread* thread_array) {
    // auxiliary pointer
    Thread* aux;
    // will repeat until all threads were removed
    while (thread_array!= NULL) {
        // changes the pointers
        aux = thread_array;
        thread_array = thread_array->next;
        // frees up the current thread
        free(aux);
    }
    
    // returns the new empty array
    return thread_array;
}

// blocks the execution until all threads finish their tasks
bool wait_threads(Thread* thread_array) {
    // auxiliary pointer
    Thread* aux = thread_array;
    // will repeat until all threads were "joined"
    while (aux != NULL) {
        // "joins" the current thread
        if (pthread_join(aux->thread_instance, NULL))
            return false;
        
        // goes to the next
        aux = aux->next;
    }
    
    // well...
    return true;
}