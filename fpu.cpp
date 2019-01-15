// FPUParams type ======================================================

#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// struct of the FPUParams type
struct fpu_prm {
    int* job_size;
    float** matrix_a;
    float** matrix_b;
    pthread_mutex_t* lock;
};

// creates a FPUParams type variable to be used in threads
FPUParams* create_fpu_params() {
    // manually allocates memory to the FPUParams type
    FPUParams* fpu_params = (FPUParams*) malloc(sizeof(FPUParams));
    // returns right away if the allocation failed
    if (fpu_params == NULL)
        return NULL;
    
    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    // sets the task counter to be equal to the number of tasks
    *job_size = fpu_job_size;
    
    // manually allocates the matrices
    float** matrix_a = (float**) malloc(fpu_matrix_size * sizeof(float*));
    if (matrix_a == NULL)
        return NULL;
    
    for (int index = 0; index < fpu_matrix_size; index++) {
        matrix_a[index] = (float*) malloc(fpu_matrix_size * sizeof(float));
        if (matrix_a[index] == NULL)
            return NULL;
    }
    
    float** matrix_b = (float**) malloc(fpu_matrix_size * sizeof(float*));
    if (matrix_b == NULL)
        return NULL;
    
    for (int index = 0; index < fpu_matrix_size; index++) {
        matrix_b[index] = (float*) malloc(fpu_matrix_size * sizeof(float));
        if (matrix_b[index] == NULL)
            return NULL;
    }
    
    // fills the matrices
    for (int index_y = 0; index_y < fpu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < fpu_matrix_size; index_x++) {
            // add a random number between 0.1 and 1.0 - not using 0 to prevent having to deal with division by zero
            matrix_a[index_x][index_y] = (float) ((rand() % 9999) + 1) / 1000.0;
        }
    }
    
    for (int index_y = 0; index_y < fpu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < fpu_matrix_size; index_x++) {
            // add a random number between 0.1 and 1.0 - not using 0 to prevent having to deal with division by zero
            matrix_b[index_x][index_y] = (float) ((rand() % 9999) + 1) / 1000.0;
        }
    }
    
    pthread_mutex_t* lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if (lock == NULL)
        return NULL;
    
    if (pthread_mutex_init(lock, NULL))
        return NULL;
    
    // fills the parameter values
    fpu_params->job_size = job_size;
    fpu_params->matrix_a = matrix_a;
    fpu_params->matrix_b = matrix_b;
    fpu_params->lock = lock;
    
    // returns the parameter
    return fpu_params;
}

// deletes a ALUParams
FPUParams* del_fpu_params(FPUParams* fpu_params) {
    // creates local references to the pointers inside params
    int* job_size = fpu_params->job_size;
    float** matrix_a = fpu_params->matrix_a;
    float** matrix_b = fpu_params->matrix_b;
    pthread_mutex_t* lock = fpu_params->lock;
    
    // frees up matrix_a
    for (int index = 0; index <  fpu_matrix_size; index++) {
        free(matrix_a[index]);
    }
    free(matrix_a);
    
    // frees up matrix_b
    for (int index = 0; index <  fpu_matrix_size; index++) {
        free(matrix_b[index]);
    }
    free(matrix_b);
    
    pthread_mutex_destroy(lock);
    free(lock);
    
    // frees up the task counter
    free(job_size);
    // frees up the thread parameters
    free (fpu_params);
    // returns the new empty FPUParams pointer
    return NULL;
}

void set_fpu_params_job_size(FPUParams* fpu_params, int* job_size) {
    fpu_params->job_size = job_size;
}

int* get_fpu_params_job_size(FPUParams* fpu_params) {
    return fpu_params->job_size;
}

void set_fpu_params_matrix_a(FPUParams* fpu_params, float** matrix_a) {
    fpu_params->matrix_a = matrix_a;
}

float** get_fpu_params_matrix_a(FPUParams* fpu_params) {
    return fpu_params->matrix_a;
}

void set_fpu_params_matrix_b(FPUParams* fpu_params, float** matrix_b) {
    fpu_params->matrix_b = matrix_b;
}

float** get_fpu_params_matrix_b(FPUParams* fpu_params) {
    return fpu_params->matrix_b;
}

void set_fpu_params_lock(FPUParams* fpu_params, pthread_mutex_t* lock) {
    fpu_params->lock = lock;
}

pthread_mutex_t* get_fpu_params_lock(FPUParams* fpu_params) {
    return fpu_params->lock;
}

// the function that will test the floating point unit
void* fpu_test(void* params) {
    // converts back the params to the expected type
    FPUParams* fpu_params = (FPUParams*) params;
    
    // creates local references to the pointers inside params
    int* job_size = get_fpu_params_job_size(fpu_params);
    float** matrix_a = get_fpu_params_matrix_a(fpu_params);
    float** matrix_b = get_fpu_params_matrix_b(fpu_params);
    pthread_mutex_t* lock = get_fpu_params_lock(fpu_params);
    
    float result = 1.0;
    bool exit = false;
    // repeats until all tasks were finished
    while (true) {
        // picks one available job from the pool
        pthread_mutex_lock(lock);
        if (*job_size > 0) *job_size = *job_size - 1; else exit = true;;
        pthread_mutex_unlock(lock);

        // do not break inside the mutex lock area
        if (exit)
            break;
        
        for (int index_y = 0; index_y < fpu_matrix_size; index_y++) {
            for (int index_x = 0; index_x < fpu_matrix_size; index_x++) {
                // executes all 4 basic operations
                result += matrix_a[index_x][index_y] + matrix_b[index_x][index_y];
                result += matrix_a[index_y][index_x] + matrix_b[index_y][index_x];
                result += matrix_a[index_x][index_x] + matrix_b[index_x][index_x];
                result += matrix_a[index_y][index_y] + matrix_b[index_y][index_y];
                
                result -= matrix_a[index_x][index_y] - matrix_b[index_x][index_y];
                result -= matrix_a[index_y][index_x] - matrix_b[index_y][index_x];
                result -= matrix_a[index_x][index_x] - matrix_b[index_x][index_x];
                result -= matrix_a[index_y][index_y] - matrix_b[index_y][index_y];
                
                result *= matrix_a[index_x][index_y] * matrix_b[index_x][index_y];
                result *= matrix_a[index_y][index_x] * matrix_b[index_y][index_x];
                result *= matrix_a[index_x][index_x] * matrix_b[index_x][index_x];
                result *= matrix_a[index_y][index_y] * matrix_b[index_y][index_y];
                
                // will help using the branch prediction (as the division is slow)
                if (matrix_a[index_x][index_y] > matrix_b[index_x][index_y]) {
                    result /= matrix_a[index_x][index_y] / matrix_b[index_x][index_y];
                } else {
                    result /= matrix_b[index_x][index_y] / matrix_a[index_x][index_y];
                }
            }
        }
    }
    
    return NULL;
}