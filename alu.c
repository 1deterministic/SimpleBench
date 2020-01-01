#include "simplebench.h"
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>
#if __linux__ || __APPLE__
    #include <pthread.h>
#elif _WIN32
    #include <windows.h>
#endif

// struct of the ALUParams type
struct alu_prm {
    int* job_size;
    int** matrix;
    int* matrix_size;
    void* lock;
};

// creates an ALUParams type variable to be used in threads
MsgCode create_alu_params(ALUParams** alu_params, int alu_matrix_size) {
    // manually allocates memory to the ALUParams type
    *alu_params = (ALUParams*) malloc(sizeof(ALUParams));
    if (*alu_params == NULL) {
        return ALU_MEMORY_ALLOCATION_ERROR;
    }

    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    if (job_size == NULL) {
        return ALU_MEMORY_ALLOCATION_ERROR;
    }
    *job_size = ALU_JOB_SIZE;
    
    // manually allocates the matrix
    int** matrix = (int**) malloc(alu_matrix_size * sizeof(int*));
    if (matrix == NULL) {
        return ALU_MEMORY_ALLOCATION_ERROR;
    }
    for (int index = 0; index < alu_matrix_size; index++) {
        matrix[index] = (int*) malloc(alu_matrix_size * sizeof(int));
        if (matrix[index] == NULL) {
            return ALU_MEMORY_ALLOCATION_ERROR;
        }
    }
    // fills the matrix with random data
    for (int index_y = 0; index_y < alu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < alu_matrix_size; index_x++) {
            matrix[index_x][index_y] = rand() % INT_MAX;
        }
    }

    // allocates memory to hold the matrix size
    int* matrix_size = (int*) malloc(sizeof(int));
    if (matrix_size == NULL) {
        return ALU_MEMORY_ALLOCATION_ERROR;
    }
    *matrix_size = alu_matrix_size;

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
    set_alu_params_job_size(alu_params, job_size);
    set_alu_params_matrix(alu_params, matrix);
    set_alu_params_matrix_size(alu_params, matrix_size);
    set_alu_params_lock(alu_params, lock);
    
    return SUCCESS;
}

// deletes an ALUParams
MsgCode del_alu_params(ALUParams** alu_params) {
    if (*alu_params == NULL) {
        return SUCCESS;
    }

    // creates local references to the pointers inside params
    int* job_size = get_alu_params_job_size(alu_params);
    int** matrix = get_alu_params_matrix(alu_params);
    int* matrix_size = get_alu_params_matrix_size(alu_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_alu_params_lock(alu_params);
    #elif _WIN32
        HANDLE* lock = get_alu_params_lock(alu_params);
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
    free (*alu_params);
    
    return SUCCESS;
}

void set_alu_params_job_size(ALUParams** alu_params, int* job_size) {
    (*alu_params)->job_size = job_size;
}

int* get_alu_params_job_size(ALUParams** alu_params) {
    return (*alu_params)->job_size;
}

void set_alu_params_matrix(ALUParams** alu_params, int** matrix) {
    (*alu_params)->matrix = matrix;
}

int** get_alu_params_matrix(ALUParams** alu_params) {
    return (*alu_params)->matrix;
}

void set_alu_params_matrix_size(ALUParams** alu_params, int* matrix_size) {
    (*alu_params)->matrix_size = matrix_size;
}

int* get_alu_params_matrix_size(ALUParams** alu_params) {
    return (*alu_params)->matrix_size;
}

void set_alu_params_lock(ALUParams** alu_params, void* lock) {
    (*alu_params)->lock = lock;
}

void* get_alu_params_lock(ALUParams** alu_params) {
    return (*alu_params)->lock;
}

// the function that will test the arythmetical and logical unit
void* alu_test(void* params) {
    // converts back the params to the expected type
    ALUParams** alu_params = (ALUParams**) &params;
    
    // creates local references to the pointers inside params
    int* job_size = get_alu_params_job_size(alu_params);
    int** matrix = get_alu_params_matrix(alu_params);
    int* matrix_size = get_alu_params_matrix_size(alu_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_alu_params_lock(alu_params);
    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = get_alu_params_lock(alu_params);

    #endif
    
    int result = 0;
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
        #elif _WIN32
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
                } 
                default: {
                    exit = true;
                    break;
                }
            }
        #endif

        // do not break inside the mutex lock area
        if (exit) {
            break;
        }
        
        for (int line = 0; line < *matrix_size; line++) {
            for (int column = 0; column < *matrix_size; column++) {
                result += matrix[line][column] * matrix[column][line] * matrix[line][line] * matrix[column][column] / (line + column + 1);
            }
        }
    }
    
    return NULL;
}