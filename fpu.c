#include "simplebench.h"
#include <stdlib.h>
#include <float.h>
#include <stdbool.h>
#include <math.h>

#if __linux__ || __APPLE__
    #include <pthread.h>
    
#elif __MINGW64__ || __MINGW32__ || _WIN32
    #include <windows.h>

#endif

// struct of the FPUParams type
struct fpu_prm {
    int* job_size;
    float** matrix;
    void* lock;
};

// creates a FPUParams type variable to be used in threads
MsgCode create_fpu_params(FPUParams** fpu_params) {
    // manually allocates memory to the FPUParams type
    *fpu_params = (FPUParams*) malloc(sizeof(FPUParams));
    if (*fpu_params == NULL)
        return FPU_MEMORY_ALLOCATION_ERROR;
    
    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    if (job_size == NULL)
        return FPU_MEMORY_ALLOCATION_ERROR;

    // sets the task counter to be equal to the number of tasks
    *job_size = FPU_JOB_SIZE;
    
    // manually allocates the matrices
    float** matrix = (float**) malloc(fpu_matrix_size * sizeof(float*));
    if (matrix == NULL)
        return FPU_MEMORY_ALLOCATION_ERROR;
    
    for (int index = 0; index < fpu_matrix_size; index++) {
        matrix[index] = (float*) malloc(fpu_matrix_size * sizeof(float));
        if (matrix[index] == NULL)
            return FPU_MEMORY_ALLOCATION_ERROR;
    }

    // fills the matrix with random data
    for (int index_y = 0; index_y < fpu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < fpu_matrix_size; index_x++) {
            matrix[index_x][index_y] = FLT_MAX * ((float) rand() / (float) RAND_MAX);
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
    set_fpu_params_job_size(fpu_params, job_size);
    set_fpu_params_matrix(fpu_params, matrix);
    set_fpu_params_lock(fpu_params, lock);
    
    return SUCCESS;
}

// deletes a FPUParams
MsgCode del_fpu_params(FPUParams** fpu_params) {
    if (*fpu_params == NULL)
        return SUCCESS;

    // creates local references to the pointers inside params
    int* job_size = get_fpu_params_job_size(fpu_params);
    float** matrix = get_fpu_params_matrix(fpu_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_fpu_params_lock(fpu_params);
        pthread_mutex_destroy(lock);

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = get_fpu_params_lock(fpu_params);
        CloseHandle(*lock);

    #endif
    free(lock);
    
    // frees up the matrices
    for (int index = 0; index < fpu_matrix_size; index++) {
        free(matrix[index]);
    }
    free(matrix);
    
    // frees up the task counter
    free(job_size);
    // frees up the thread parameters
    free (*fpu_params);

    return SUCCESS;
}

void set_fpu_params_job_size(FPUParams** fpu_params, int* job_size) {
    (*fpu_params)->job_size = job_size;
}

int* get_fpu_params_job_size(FPUParams** fpu_params) {
    return (*fpu_params)->job_size;
}

void set_fpu_params_matrix(FPUParams** fpu_params, float** matrix) {
    (*fpu_params)->matrix = matrix;
}

float** get_fpu_params_matrix(FPUParams** fpu_params) {
    return (*fpu_params)->matrix;
}

void set_fpu_params_lock(FPUParams** fpu_params, void* lock) {
    (*fpu_params)->lock = lock;
}

void* get_fpu_params_lock(FPUParams** fpu_params) {
    return (*fpu_params)->lock;
}

// the function that will test the floating point unit
void* fpu_test(void* params) {
    // converts back the params to the expected type
    FPUParams** fpu_params = (FPUParams**) &params;
    
    // creates local references to the pointers inside params
    int* job_size = get_fpu_params_job_size(fpu_params);
    float** matrix = get_fpu_params_matrix(fpu_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_fpu_params_lock(fpu_params);

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = get_fpu_params_lock(fpu_params);

    #endif
    
    float result = 0.0;
    bool exit = false;
    // repeats until all tasks were finished
    while (true) {
        // picks one available job from the pool
        #if __linux__ || __APPLE__
            pthread_mutex_lock(lock);
            if (*job_size > 0) *job_size = *job_size - 1; else exit = true;
            pthread_mutex_unlock(lock);

        #elif __MINGW64__ || __MINGW32__ || _WIN32
            switch(WaitForSingleObject(*lock, INFINITE)) {
                case WAIT_OBJECT_0: if (*job_size > 0) *job_size = *job_size - 1; else exit = true; ReleaseMutex(*lock); break;
                case WAIT_ABANDONED: exit = true; break;
                default: exit = true; break;
            }
        #endif

        // do not break inside the mutex lock area
        if (exit)
            break;
        
        for (int line = 0; line < fpu_matrix_size; line++) {
            for (int column = 0; column < fpu_matrix_size; column++) {
                result += sqrt(matrix[line][column] * matrix[column][line] * matrix[line][line] * matrix[column][column]) / 3.14159265;
            }
        }
    }
    
    return NULL;
}
