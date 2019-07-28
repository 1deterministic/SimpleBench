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
    float** matrix_a;
    float** matrix_b;
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
    *job_size = fpu_job_size;
    
    // manually allocates the matrices
    float** matrix_a = (float**) malloc(fpu_matrix_size * sizeof(float*));
    if (matrix_a == NULL)
        return FPU_MEMORY_ALLOCATION_ERROR;

    float** matrix_b = (float**) malloc(fpu_matrix_size * sizeof(float*));
    if (matrix_b == NULL)
        return FPU_MEMORY_ALLOCATION_ERROR;
    
    for (int index = 0; index < fpu_matrix_size; index++) {
        matrix_a[index] = (float*) malloc(fpu_matrix_size * sizeof(float));
        if (matrix_a[index] == NULL)
            return FPU_MEMORY_ALLOCATION_ERROR;

        matrix_b[index] = (float*) malloc(fpu_matrix_size * sizeof(float));
        if (matrix_b[index] == NULL)
            return FPU_MEMORY_ALLOCATION_ERROR;
    }

    // fills the matrices
    for (int index_y = 0; index_y < fpu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < fpu_matrix_size; index_x++) {
            // not using 0 to prevent having to deal with division by zero
            matrix_a[index_x][index_y] = 0.1 + ((FLT_MAX - 1.0) * ((float) rand()) / ((float) RAND_MAX));
            matrix_b[index_x][index_y] = 0.1 + ((FLT_MAX - 1.0) * ((float) rand()) / ((float) RAND_MAX));
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
    set_fpu_params_matrix_a(fpu_params, matrix_a);
    set_fpu_params_matrix_b(fpu_params, matrix_b);
    set_fpu_params_lock(fpu_params, lock);
    
    return SUCCESS;
}

// deletes a FPUParams
MsgCode del_fpu_params(FPUParams** fpu_params) {
    if (*fpu_params == NULL)
        return SUCCESS;

    // creates local references to the pointers inside params
    int* job_size = get_fpu_params_job_size(fpu_params);
    float** matrix_a = get_fpu_params_matrix_a(fpu_params);
    float** matrix_b = get_fpu_params_matrix_b(fpu_params);
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
        free(matrix_a[index]);
        free(matrix_b[index]);
    }
    free(matrix_a);
    free(matrix_b);
    
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

void set_fpu_params_matrix_a(FPUParams** fpu_params, float** matrix_a) {
    (*fpu_params)->matrix_a = matrix_a;
}

float** get_fpu_params_matrix_a(FPUParams** fpu_params) {
    return (*fpu_params)->matrix_a;
}

void set_fpu_params_matrix_b(FPUParams** fpu_params, float** matrix_b) {
    (*fpu_params)->matrix_b = matrix_b;
}

float** get_fpu_params_matrix_b(FPUParams** fpu_params) {
    return (*fpu_params)->matrix_b;
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
    float** matrix_a = get_fpu_params_matrix_a(fpu_params);
    float** matrix_b = get_fpu_params_matrix_b(fpu_params);
    #if __linux__ || __APPLE__
        pthread_mutex_t* lock = get_fpu_params_lock(fpu_params);

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        HANDLE* lock = get_fpu_params_lock(fpu_params);

    #endif
    
    float result = 1.0;
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
        
        for (int index_y = 0; index_y < fpu_matrix_size; index_y++) {
            for (int index_x = 0; index_x < fpu_matrix_size; index_x++) {
                result = sin(matrix_a[index_x][index_y]);
                result = sin(matrix_b[index_x][index_y]);
                result = sin(matrix_a[index_x][index_x]);
                result = sin(matrix_b[index_x][index_x]);
                result = cos(matrix_a[index_y][index_x]);
                result = cos(matrix_b[index_y][index_x]);
                result = cos(matrix_a[index_y][index_y]);
                result = cos(matrix_b[index_y][index_y]);
                result = sqrt(matrix_a[index_x][index_y]);
                result = sqrt(matrix_b[index_x][index_y]);
                result = sqrt(matrix_a[index_y][index_x]);
                result = sqrt(matrix_b[index_y][index_x]);
                result = sqrt(matrix_a[index_x][index_x]);
                result = sqrt(matrix_b[index_x][index_x]);
                result = sqrt(matrix_a[index_y][index_y]);
                result = sqrt(matrix_b[index_y][index_y]);
            }
        }
    }
    
    return NULL;
}