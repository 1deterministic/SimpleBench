#include "simplebench.h"
#include <windows.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

// struct of the ALUParams type
struct alu_prm {
    int* job_size;
    int** matrix_a;
    int** matrix_b;
    HANDLE* lock;
};

// creates an ALUParams type variable to be used in threads
MsgCode create_alu_params(ALUParams** alu_params) {
    // manually allocates memory to the ALUParams type
    *alu_params = (ALUParams*) malloc(sizeof(ALUParams));
    if (*alu_params == NULL)
        return ALU_MEMORY_ALLOCATION_ERROR;
    
    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    if (job_size == NULL)
        return ALU_MEMORY_ALLOCATION_ERROR;

    // sets the task counter to be equal to the number of tasks
    *job_size = alu_job_size;
    
    // manually allocates the matrices
    int** matrix_a = (int**) malloc(alu_matrix_size * sizeof(int*));
    if (matrix_a == NULL)
        return ALU_MEMORY_ALLOCATION_ERROR;

    int** matrix_b = (int**) malloc(alu_matrix_size * sizeof(int*));
    if (matrix_b == NULL)
        return ALU_MEMORY_ALLOCATION_ERROR;
    
    for (int index = 0; index < alu_matrix_size; index++) {
        matrix_a[index] = (int*) malloc(alu_matrix_size * sizeof(int));
        if (matrix_a[index] == NULL)
            return ALU_MEMORY_ALLOCATION_ERROR;

        matrix_b[index] = (int*) malloc(alu_matrix_size * sizeof(int));
        if (matrix_b[index] == NULL)
            return ALU_MEMORY_ALLOCATION_ERROR;
    }
        
    // fills the matrices
    for (int index_y = 0; index_y < alu_matrix_size; index_y++) {
        for (int index_x = 0; index_x < alu_matrix_size; index_x++) {
            // not using 0 to prevent having to deal with division by zero
            matrix_a[index_x][index_y] = (rand() % (INT_MAX - 1)) + 1;
            matrix_b[index_x][index_y] = (rand() % (INT_MAX - 1)) + 1;
        }
    }

    HANDLE* lock = (HANDLE*) malloc(sizeof(HANDLE));
    if (lock == NULL)
        return ALU_PTHREAD_LOCK_CREATION_ERROR;
    
    *lock = CreateMutex(NULL, FALSE, NULL);
    if (*lock == NULL)
        return ALU_PTHREAD_LOCK_INIT_ERROR;
    
    // fills the parameter values
    set_alu_params_job_size(alu_params, job_size);
    set_alu_params_matrix_a(alu_params, matrix_a);
    set_alu_params_matrix_b(alu_params, matrix_b);
    set_alu_params_lock(alu_params, lock);
    
    return SUCCESS;
}

// deletes an ALUParams
MsgCode del_alu_params(ALUParams** alu_params) {
    if (*alu_params == NULL)
        return SUCCESS;

    // creates local references to the pointers inside params
    int* job_size = get_alu_params_job_size(alu_params);
    int** matrix_a = get_alu_params_matrix_a(alu_params);
    int** matrix_b = get_alu_params_matrix_b(alu_params);
    HANDLE* lock = get_alu_params_lock(alu_params);
    
    // frees up the matrices
    for (int index = 0; index < alu_matrix_size; index++) {
        free(matrix_a[index]);
        free(matrix_b[index]);
    }
    // free(matrix_a);
    // free(matrix_b);
    
    CloseHandle(lock);
    free(lock);
    
    // frees up the task counter
    free(job_size);
    // frees up the thread parameters
    free (*alu_params);
    
    return SUCCESS;
}

void set_alu_params_job_size(ALUParams** alu_params, int* job_size) {
    (*alu_params)->job_size = job_size;
}

int* get_alu_params_job_size(ALUParams** alu_params) {
    return (*alu_params)->job_size;
}

void set_alu_params_matrix_a(ALUParams** alu_params, int** matrix_a) {
    (*alu_params)->matrix_a = matrix_a;
}

int** get_alu_params_matrix_a(ALUParams** alu_params) {
    return (*alu_params)->matrix_a;
}

void set_alu_params_matrix_b(ALUParams** alu_params, int** matrix_b) {
    (*alu_params)->matrix_b = matrix_b;
}

int** get_alu_params_matrix_b(ALUParams** alu_params) {
    return (*alu_params)->matrix_b;
}

void set_alu_params_lock(ALUParams** alu_params, HANDLE* lock) {
    (*alu_params)->lock = lock;
}

HANDLE* get_alu_params_lock(ALUParams** alu_params) {
    return (*alu_params)->lock;
}

// the function that will test the arythmetical and logical unit
void* alu_test(void* params) {
    // converts back the params to the expected type
    ALUParams** alu_params = (ALUParams**) &params;
    
    // creates local references to the pointers inside params
    int* job_size = get_alu_params_job_size(alu_params);
    int** matrix_a = get_alu_params_matrix_a(alu_params);
    int** matrix_b = get_alu_params_matrix_b(alu_params);
    HANDLE* lock = get_alu_params_lock(alu_params);
    
    int result = 1;
    bool exit = false;
    // repeats until all tasks were finished
    while (true) {
        // picks one available job from the pool
        switch(WaitForSingleObject(*lock, INFINITE)) {
            case WAIT_OBJECT_0: if (*job_size > 0) *job_size = *job_size - 1; else exit = true; ReleaseMutex(*lock); break;
            case WAIT_ABANDONED: exit = true; break;
            default: exit = true; break;
        }

        // do not break inside the mutex lock area
        if (exit)
            break;
        
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