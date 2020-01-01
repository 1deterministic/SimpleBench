#include "simplebench.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#if __linux__ || __APPLE__
    #include <pthread.h>
#elif _WIN32
    #include <windows.h>
#endif

// struct of the MEMParams type
struct mem_prm {
    int* job_size;
    int** matrix;
    int* matrix_size;
    void* lock;
};

// creates a MEMParams type variable to be used in threads
MsgCode create_mem_params(MEMParams** mem_params, int mem_matrix_size) {
    // manually allocates memory to the MEMParams type
    *mem_params = (MEMParams*) malloc(sizeof(MEMParams));
    if (*mem_params == NULL) {
        return MEM_MEMORY_ALLOCATION_ERROR;
    }
    
    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    if (job_size == NULL) {
        return MEM_MEMORY_ALLOCATION_ERROR;
    }
    // sets the task counter to be equal to the number of tasks
    *job_size = MEM_JOB_SIZE;
    
    // manually allocates the matrices
    int** matrix = (int**) malloc(mem_matrix_size * sizeof(int*));
    if (matrix == NULL) {
        return MEM_MEMORY_ALLOCATION_ERROR;
    }
    for (int index = 0; index < mem_matrix_size; index++) {
        matrix[index] = (int*) malloc(mem_matrix_size * sizeof(int));
        if (matrix[index] == NULL)
            return MEM_MEMORY_ALLOCATION_ERROR;
    }
    // fills the matrix with random data
    for (int index_y = 0; index_y < mem_matrix_size; index_y++) {
        for (int index_x = 0; index_x < mem_matrix_size; index_x++) {
            matrix[index_x][index_y] = INT_MAX;
        }
    }

    // allocates memory to hold the matrix size
    int* matrix_size = (int*) malloc(sizeof(int));
    if (matrix_size == NULL) {
        return MEM_MEMORY_ALLOCATION_ERROR;
    }
    *matrix_size = mem_matrix_size;

    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
        if (lock == NULL) {
            return MEM_PTHREAD_LOCK_CREATION_ERROR;
        }
        if (pthread_mutex_init(lock, NULL)) {
            return MEM_PTHREAD_LOCK_INIT_ERROR;
        }
    #elif _WIN32
        HANDLE* lock = (HANDLE*) malloc(sizeof(HANDLE));
        if (lock == NULL) {
            return MEM_PTHREAD_LOCK_CREATION_ERROR;
        }
        *lock = CreateMutex(NULL, FALSE, NULL);
        if (*lock == NULL) {
            return MEM_PTHREAD_LOCK_INIT_ERROR;
        }
    #endif
    
    // fills the parameter values
    set_mem_params_job_size(mem_params, job_size);
    set_mem_params_matrix(mem_params, matrix);
    set_mem_params_matrix_size(mem_params, matrix_size);
    set_mem_params_lock(mem_params, lock);
    
    return SUCCESS;
}

// deletes a MEMParams
MsgCode del_mem_params(MEMParams** mem_params) {
    if (*mem_params == NULL) {
        return SUCCESS;
    }

    // creates local references to the pointers inside params
    int* job_size = get_mem_params_job_size(mem_params);
    int** matrix = get_mem_params_matrix(mem_params);
    int* matrix_size = get_mem_params_matrix_size(mem_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_mem_params_lock(mem_params);
    #elif _WIN32
        HANDLE* lock = get_mem_params_lock(mem_params);
    #endif

    // frees up everything
    #if __linux__ || __APPLE__
        pthread_mutex_destroy(lock);
    #elif _WIN32
        CloseHandle(*lock);
    #endif
    free(lock);

    for (int index = 0; index < *matrix_size; index++) {
        free(matrix[index]);
    }
    free(matrix);
    free(job_size);
    free(matrix_size);
    free (*mem_params);
    
    return SUCCESS;
}

void set_mem_params_job_size(MEMParams** mem_params, int* job_size) {
    (*mem_params)->job_size = job_size;
}

int* get_mem_params_job_size(MEMParams** mem_params) {
    return (*mem_params)->job_size;
}

void set_mem_params_matrix(MEMParams** mem_params, int** matrix) {
    (*mem_params)->matrix = matrix;
}

int** get_mem_params_matrix(MEMParams** mem_params) {
    return (*mem_params)->matrix;
}

void set_mem_params_matrix_size(MEMParams** mem_params, int* matrix_size) {
    (*mem_params)->matrix_size = matrix_size;
}

int* get_mem_params_matrix_size(MEMParams** mem_params) {
    return (*mem_params)->matrix_size;
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
    int** matrix = get_mem_params_matrix(mem_params);
    int* matrix_size = get_mem_params_matrix_size(mem_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_mem_params_lock(mem_params);
    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = get_mem_params_lock(mem_params);
    #endif

    int* array = (int*) malloc(*matrix_size * sizeof(int));
    if (array == NULL) {
        return NULL;
    }
    
    bool exit = false;
    // repeats until all tasks were finished
    while (true) {
        // picks one available job from the pool
        #if __linux__ || __APPLE__
            pthread_mutex_lock(lock);
            if (*job_size > 0) {
                *job_size = *job_size - 1;
            } else {
                exit = true;
            }
            pthread_mutex_unlock(lock);
        #elif __MINGW64__ || __MINGW32__ || _WIN32
            switch(WaitForSingleObject(*lock, INFINITE)) {
                case WAIT_OBJECT_0: {
                    if (*job_size > 0) {
                        *job_size = *job_size - 1;
                    } else {
                        exit = true;
                    }
                    ReleaseMutex(*lock); 
                    break;
                } 
                case WAIT_ABANDONED: {
                    exit = true; 
                    break;
                } default: {
                    exit = true; 
                    break;
                }
            }
        #endif

        // do not break inside the mutex lock area
        if (exit) {
            break;
        }
            
        for (int line = 0; line < *matrix_size / 1024; line++) {
            memcpy(array, matrix[rand() % *matrix_size], *matrix_size * sizeof(int));
        }
    }
    
    free(array);

    return NULL;
}
