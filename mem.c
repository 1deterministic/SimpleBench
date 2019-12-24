#include "simplebench.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#if __linux__ || __APPLE__
    #include <pthread.h>
    
#elif __MINGW64__ || __MINGW32__ || _WIN32
    #include <windows.h>

#endif

// struct of the MEMParams type
struct mem_prm {
    int* job_size;
    int** matrix_a;
    int** matrix_b;
    void* lock;
};

// creates a MEMParams type variable to be used in threads
MsgCode create_mem_params(MEMParams** mem_params) {
    // manually allocates memory to the MEMParams type
    *mem_params = (MEMParams*) malloc(sizeof(MEMParams));
    if (*mem_params == NULL)
        return MEM_MEMORY_ALLOCATION_ERROR;
    
    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    if (job_size == NULL)
        return MEM_MEMORY_ALLOCATION_ERROR;

    // sets the task counter to be equal to the number of tasks
    *job_size = mem_job_size;
    
    // manually allocates the matrices
    int** matrix_a = (int**) malloc(mem_matrix_size * sizeof(int*));
    if (matrix_a == NULL)
        return MEM_MEMORY_ALLOCATION_ERROR;

    int** matrix_b = (int**) malloc(mem_matrix_size * sizeof(int*));
    if (matrix_b == NULL)
        return MEM_MEMORY_ALLOCATION_ERROR;
    
    for (int index = 0; index < mem_matrix_size; index++) {
        matrix_a[index] = (int*) malloc(mem_matrix_size * sizeof(int));
        if (matrix_a[index] == NULL)
            return MEM_MEMORY_ALLOCATION_ERROR;

        matrix_b[index] = (int*) malloc(mem_matrix_size * sizeof(int));
        if (matrix_b[index] == NULL)
            return MEM_MEMORY_ALLOCATION_ERROR;
    }
    
    // fills the matrices
    for (int index_y = 0; index_y < mem_matrix_size; index_y++) {
        for (int index_x = 0; index_x < mem_matrix_size; index_x++) {
            matrix_a[index_x][index_y] = rand() % INT_MAX;
            matrix_b[index_x][index_y] = rand() % INT_MAX;
        }
    }

    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
        if (lock == NULL)
            return MEM_PTHREAD_LOCK_CREATION_ERROR;
        
        if (pthread_mutex_init(lock, NULL))
            return MEM_PTHREAD_LOCK_INIT_ERROR;

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = (HANDLE*) malloc(sizeof(HANDLE));
        if (lock == NULL)
            return MEM_PTHREAD_LOCK_CREATION_ERROR;
        
        *lock = CreateMutex(NULL, FALSE, NULL);
        if (*lock == NULL)
            return MEM_PTHREAD_LOCK_INIT_ERROR;

    #endif
    
    // fills the parameter values
    set_mem_params_job_size(mem_params, job_size);
    set_mem_params_matrix_a(mem_params, matrix_a);
    set_mem_params_matrix_b(mem_params, matrix_b);
    set_mem_params_lock(mem_params, lock);
    
    return SUCCESS;
}

// deletes a MEMParams
MsgCode del_mem_params(MEMParams** mem_params) {
    if (*mem_params == NULL)
        return SUCCESS;

    // creates local references to the pointers inside params
    int* job_size = get_mem_params_job_size(mem_params);
    int** matrix_a = get_mem_params_matrix_a(mem_params);
    int** matrix_b = get_mem_params_matrix_b(mem_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_mem_params_lock(mem_params);
        pthread_mutex_destroy(lock);

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = get_mem_params_lock(mem_params);
        CloseHandle(*lock);

    #endif
    free(lock);
    
    // frees up the matrices
    for (int index = 0; index < mem_matrix_size; index++) {
        free(matrix_a[index]);
        free(matrix_b[index]);
    }
    free(matrix_a);
    free(matrix_b);
    
    // frees up the task counter
    free(job_size);
    // frees up the thread parameters
    free (*mem_params);
    
    return SUCCESS;
}

void set_mem_params_job_size(MEMParams** mem_params, int* job_size) {
    (*mem_params)->job_size = job_size;
}

int* get_mem_params_job_size(MEMParams** mem_params) {
    return (*mem_params)->job_size;
}

void set_mem_params_matrix_a(MEMParams** mem_params, int** matrix_a) {
    (*mem_params)->matrix_a = matrix_a;
}

int** get_mem_params_matrix_a(MEMParams** mem_params) {
    return (*mem_params)->matrix_a;
}

void set_mem_params_matrix_b(MEMParams** mem_params, int** matrix_b) {
    (*mem_params)->matrix_b = matrix_b;
}

int** get_mem_params_matrix_b(MEMParams** mem_params) {
    return (*mem_params)->matrix_b;
}

void set_mem_params_lock(MEMParams** mem_params, void* lock) {
    (*mem_params)->lock = lock;
}

void* get_mem_params_lock(MEMParams** mem_params) {
    return (*mem_params)->lock;
}

// the function that will test the system memory
void* mem_test(void* params) {
    srand((unsigned) time(NULL));

    // converts back the params to the expected type
    MEMParams** mem_params = (MEMParams**) &params;
    
    // creates local references to the pointers inside params
    int* job_size = get_mem_params_job_size(mem_params);
    int** matrix_a = get_mem_params_matrix_a(mem_params);
    int** matrix_b = get_mem_params_matrix_b(mem_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_mem_params_lock(mem_params);

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = get_mem_params_lock(mem_params);

    #endif
    
    int line;
    bool exit = false;
    // repeats until all tasks were finished
    while (true) {
        // picks one available job from the pool
        #if __linux__ || __APPLE__
            pthread_mutex_lock(lock);
            if (*job_size > 0) {
                line = (mem_job_size - *job_size) % mem_matrix_size;
                *job_size = *job_size - 1; 
            } else {
                exit = true;
            }
            pthread_mutex_unlock(lock);

        #elif __MINGW64__ || __MINGW32__ || _WIN32
            switch(WaitForSingleObject(*lock, INFINITE)) {
                case WAIT_OBJECT_0: {
                    if (*job_size > 0) {
                        line = (mem_job_size - *job_size) % mem_matrix_size;
                        *job_size = *job_size - 1; 
                    } else {
                        exit = true;
                    }
                    ReleaseMutex(*lock);
                    break;    
                }
                case WAIT_ABANDONED: exit = true; break;
                default: exit = true; break;
            }

        #endif

        // do not break inside the mutex lock area
        if (exit)
            break;
        
        for (int i = 0; i < 1024; i++) {
            // copies an entire line of the matrix_a to the matrix_b
            // random lines to prevent caching
            memcpy(matrix_b[line], matrix_a[rand() % mem_matrix_size], mem_matrix_size * sizeof(int));
        }
    }
    
    return NULL;
}