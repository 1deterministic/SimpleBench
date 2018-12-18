// ALUParams type ======================================================

#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// struct of the ALUParams type
struct alu_prm {
    int* job_size;
    int** matrix_a;
    int** matrix_b;
    pthread_mutex_t* lock;
};

// creates an ALUParams type variable to be used in threads
ALUParams* create_alu_params() {
    // manually allocates memory to the ALUParams type
    ALUParams* alu_params = (ALUParams*) malloc(sizeof(ALUParams));
    // returns right away if the allocation failed
    if (alu_params == NULL)
        return NULL;
    
    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    // sets the task counter to be equal to the number of tasks
    *job_size = alu_job_size;
    
    // manually allocates the matrices
    int** matrix_a = (int**) malloc(alu_matrix_size * sizeof(int*));
    if (matrix_a == NULL)
        return NULL;
    
    for (int index = 0; index < alu_matrix_size; index++) {
        matrix_a[index] = (int*) malloc(alu_matrix_size * sizeof(int));
        if (matrix_a[index] == NULL)
            return NULL;
    }
    
    int** matrix_b = (int**) malloc(alu_matrix_size * sizeof(int*));
    if (matrix_b == NULL)
        return NULL;
    
    for (int index = 0; index < alu_matrix_size; index++) {
        matrix_b[index] = (int*) malloc(alu_matrix_size * sizeof(int));
        if (matrix_b[index] == NULL)
            return NULL;
    }
        
    // fills the matrices
    for (int index_y = 0; index_y < alu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < alu_matrix_size; index_x++) {
            // add a random number between 1 and 10 - not using 0 to prevent having to deal with division by zero
            matrix_a[index_x][index_y] = (rand() % 9) + 1;
        }
    }
    
    for (int index_y = 0; index_y < alu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < alu_matrix_size; index_x++) {
            // add a random number between 1 and 10 - not using 0 to prevent having to deal with division by zero
            matrix_b[index_x][index_y] = (rand() % 9) + 1;
        }
    }
    
    pthread_mutex_t* lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    if (lock == NULL)
        return NULL;
    
    if (pthread_mutex_init(lock, NULL))
        return NULL;
    
    // fills the parameter values
    alu_params->job_size = job_size;
    alu_params->matrix_a = matrix_a;
    alu_params->matrix_b = matrix_b;
    alu_params->lock = lock;
    
    // returns the parameter
    return alu_params;
}

// deletes an ALUParams
ALUParams* del_alu_params(ALUParams* alu_params) {
    // creates local references to the pointers inside params
    int* job_size = alu_params->job_size;
    int** matrix_a = alu_params->matrix_a;
    int** matrix_b = alu_params->matrix_b;
    pthread_mutex_t* lock = alu_params->lock;
    
    // frees up matrix_a
    for (int index = 0; index <  alu_matrix_size; index++) {
        free(matrix_a[index]);
    }
    free(matrix_a);
    
    // frees up matrix_b
    for (int index = 0; index <  alu_matrix_size; index++) {
        free(matrix_b[index]);
    }
    free(matrix_b);
    
    pthread_mutex_destroy(lock);
    free(lock);
    
    // frees up the task counter
    free(job_size);
    // frees up the thread parameters
    free (alu_params);
    // returns the new empty ALUParams pointer
    return NULL;
}

void set_alu_params_job_size(ALUParams* alu_params, int* job_size) {
    alu_params->job_size = job_size;
}

int* get_alu_params_job_size(ALUParams* alu_params) {
    return alu_params->job_size;
}

void set_alu_params_matrix_a(ALUParams* alu_params, int** matrix_a) {
    alu_params->matrix_a = matrix_a;
}

int** get_alu_params_matrix_a(ALUParams* alu_params) {
    return alu_params->matrix_a;
}

void set_alu_params_matrix_b(ALUParams* alu_params, int** matrix_b) {
    alu_params->matrix_b = matrix_b;
}

int** get_alu_params_matrix_b(ALUParams* alu_params) {
    return alu_params->matrix_b;
}

void set_alu_params_lock(ALUParams* alu_params, pthread_mutex_t* lock) {
    alu_params->lock = lock;
}

pthread_mutex_t* get_alu_params_lock(ALUParams* alu_params) {
    return alu_params->lock;
}

// the function that will test the arythmetical and logical unit
void* alu_test(void* params) {
    // converts back the params to the expected type
    ALUParams* alu_params = (ALUParams*) params;
    
    // creates local references to the pointers inside params
    int* job_size = get_alu_params_job_size(alu_params);
    int** matrix_a = get_alu_params_matrix_a(alu_params);
    int** matrix_b = get_alu_params_matrix_b(alu_params);
    pthread_mutex_t* lock = get_alu_params_lock(alu_params);
    
    int result = 1;
    // repeats until all tasks were finished
    while (*job_size > 0) {
        // picks one available job from the pool
        pthread_mutex_lock(lock);
        *job_size = *job_size - 1;
        pthread_mutex_unlock(lock);
        
        for (int index_y = 0; index_y < alu_matrix_size; index_y++) {
            for (int index_x = 0; index_x < alu_matrix_size; index_x++) {
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