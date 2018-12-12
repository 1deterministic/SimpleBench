#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// abstract types ======================================================
// struct of the ALUParams type
struct alu_prm {
    int* job_size;
    int** matrix_a;
    int** matrix_b;
    pthread_mutex_t* lock;
};

// struct of the FPUParams type
struct fpu_prm {
    int* job_size;
    float** matrix_a;
    float** matrix_b;
    pthread_mutex_t* lock;
};

// struct of the MEMParams type
struct mem_prm {
    int* job_size;
    int** matrix_a;
    int** matrix_b;
    pthread_mutex_t* lock;
};

// struct of the GUIParams type
struct gui_prm {
    int* cores;
    ALUParams* alu;
    FPUParams* fpu;
    MEMParams* mem;
};

// struct of the Thread type
struct thd {
    pthread_t thread_instance;
    Thread* next;
};

// struct of the Time type
struct chronometer {
    struct timespec start;
    struct timespec stop;
};

// testing functions ===================================================
// the function that will test the arythmetical and logical unit
void* alu_test(void* params) {
    // converts back the params to the expected type
    ALUParams* alu_params = (ALUParams*) params;
    
    // creates local references to the pointers inside params
    int* job_size = alu_params->job_size;
    int** matrix_a = alu_params->matrix_a;
    int** matrix_b = alu_params->matrix_b;
    pthread_mutex_t* lock = alu_params->lock;
    
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

// the function that will test the floating point unit
void* fpu_test(void* params) {
    // converts back the params to the expected type
    FPUParams* fpu_params = (FPUParams*) params;
    
    // creates local references to the pointers inside params
    int* job_size = fpu_params->job_size;
    float** matrix_a = fpu_params->matrix_a;
    float** matrix_b = fpu_params->matrix_b;
    pthread_mutex_t* lock = fpu_params->lock;
    
    float result = 1.0;
    // repeats until all tasks were finished
    while (*job_size > 0) {
        // picks one available job from the pool
        pthread_mutex_lock(lock);
        *job_size = *job_size - 1;
        pthread_mutex_unlock(lock);
        
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

// the function that will test the system memory
void* mem_test(void* params) {
    // converts back the params to the expected type
    MEMParams* mem_params = (MEMParams*) params;
    
    // creates local references to the pointers inside params
    int* job_size = mem_params->job_size;
    int** matrix_a = mem_params->matrix_a;
    int** matrix_b = mem_params->matrix_b;
    pthread_mutex_t* lock = mem_params->lock;
    
    int line;
    // repeats until all tasks were finished
    while (*job_size > 0) {
        // picks one available job from the pool
        pthread_mutex_lock(lock);
        line = (mem_job_size - *job_size) % mem_matrix_size;
        *job_size = *job_size - 1;
        pthread_mutex_unlock(lock);
        
        // copies an entire line of the matrix_a to the matrix_b
        memcpy(matrix_b[line], matrix_a[line], mem_matrix_size * sizeof(int));
    }
    
    return NULL;
}

// shows the progress bar (helper to the gui thread
void print_progress(float current, float total) {
    for (int i = 0; i < 32; i++) {
        if (i == 0)
            printf("[");
        if (i <= (int) 32.0 * ((float) (total - current)) / ((float) total))
            printf("=");
        else
            printf(" ");
        if (i == 31)
            printf("]\n");
    }
}

void* gui(void* params) {
    // converts back the params to the expected type
    GUIParams* gui_params = (GUIParams*) params;
    
    // 
    //ALUParams* alu_params = gui_params->alu;
    //FPUParams* fpu_params = gui_params->fpu;
    //MEMParams* mem_params = gui_params->mem;
    
    int* cores_used = gui_params->cores;
    int* alu_job = gui_params->alu->job_size;
    int* fpu_job = gui_params->fpu->job_size;
    int* mem_job = gui_params->mem->job_size;
    
    // while the test is still in running, shows the current progress each second
    while (*alu_job > 0 || *fpu_job > 0 || *mem_job > 0) {
        system("clear");
        printf("Testing the system with %d core(s):\n\n", *cores_used);
        printf("ALU Test: %.2f%\n", 100.0 * ((float) (alu_job_size - *alu_job)) / ((float) alu_job_size));
        print_progress(*alu_job, alu_job_size);
        printf("FPU Test: %.2f%\n", 100.0 * ((float) (fpu_job_size - *fpu_job)) / ((float) fpu_job_size));
        print_progress(*fpu_job, fpu_job_size);
        printf("MEM Test: %.2f%\n", 100.0 * ((float) (mem_job_size - *mem_job)) / ((float) mem_job_size));
        print_progress(*mem_job, mem_job_size);
        sleep(1);
    }
    
    system("clear");
    printf("Finished the test with %d core(s)!\n", *cores_used);
    sleep(1);
    
    return NULL;
}

// Thread type =========================================================
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

// ALUParams type ======================================================
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

// FPUParams type ======================================================
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

// MEMParams type ======================================================
// creates a MEMParams type variable to be used in threads
MEMParams* create_mem_params() {
    // manually allocates memory to the MEMParams type
    MEMParams* mem_params = (MEMParams*) malloc(sizeof(MEMParams));
    // returns right away if the allocation failed
    if (mem_params == NULL)
        return NULL;
    
    // manually allocates memory to the task counter
    int* job_size = (int*) malloc(sizeof(int));
    // sets the task counter to be equal to the number of tasks
    *job_size = mem_job_size;
    
    // manually allocates the matrices
    int** matrix_a = (int**) malloc(mem_matrix_size * sizeof(int*));
    if (matrix_a == NULL)
        return NULL;
    
    for (int index = 0; index < mem_matrix_size; index++) {
        matrix_a[index] = (int*) malloc(mem_matrix_size * sizeof(int));
        if (matrix_a[index] == NULL)
            return NULL;
    }
    
    int** matrix_b = (int**) malloc(mem_matrix_size * sizeof(int*));
    if (matrix_b == NULL)
        return NULL;
    
    for (int index = 0; index < mem_matrix_size; index++) {
        matrix_b[index] = (int*) malloc(mem_matrix_size * sizeof(int));
        if (matrix_b[index] == NULL)
            return NULL;
    }
    
    // fills the matrices
    for (int index_y = 0; index_y < mem_matrix_size; index_y++) {
        for (int index_x = 0; index_x < mem_matrix_size; index_x++) {
            // add a random number between 1 and 10 - not using 0 to prevent having to deal with division by zero
            matrix_a[index_x][index_y] = (rand() % 9) + 1;
        }
    }
    
    for (int index_y = 0; index_y < mem_matrix_size; index_y++) {
        for (int index_x = 0; index_x < mem_matrix_size; index_x++) {
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
    mem_params->job_size = job_size;
    mem_params->matrix_a = matrix_a;
    mem_params->matrix_b = matrix_b;
    mem_params->lock = lock;
    
    // returns the parameter
    return mem_params;
}

// deletes a MEMParams
MEMParams* del_mem_params(MEMParams* mem_params) {
    // creates local references to the pointers inside params
    int* job_size = mem_params->job_size;
    int** matrix_a = mem_params->matrix_a;
    int** matrix_b = mem_params->matrix_b;
    pthread_mutex_t* lock = mem_params->lock;
    
    // frees up matrix_a
    for (int index = 0; index <  mem_matrix_size; index++) {
        free(matrix_a[index]);
    }
    free(matrix_a);
    
    // frees up matrix_b
    for (int index = 0; index <  mem_matrix_size; index++) {
        free(matrix_b[index]);
    }
    free(matrix_b);
    
    pthread_mutex_destroy(lock);
    free(lock);
    
    // frees up the task counter
    free(job_size);
    // frees up the thread parameters
    free (mem_params);
    // returns the new empty ALUParams pointer
    return NULL;
}

// GUIParams type ======================================================
//
GUIParams* create_gui_params(ALUParams* alu_params, FPUParams* fpu_params, MEMParams* mem_params, int* cores) {
    // manually allocates memory to the ALUParams type
    GUIParams* gui_params = (GUIParams*) malloc(sizeof(GUIParams));
    // returns right away if the allocation failed
    if (gui_params == NULL)
        return NULL;
    
    // fills the parameter values
    gui_params->alu = alu_params;
    gui_params->fpu = fpu_params;
    gui_params->mem = mem_params;
    gui_params->cores = cores;
    
    // returns the parameter
    return gui_params;
}

// deletes an ALUParams
GUIParams* del_gui_params(GUIParams* gui_params) {
    free (gui_params);
    return NULL;
}

// Chronometer type ====================================================
// creates a Chronometer
Chronometer* create_chronometer() {
    Chronometer* chronometer = (Chronometer*) malloc(sizeof(Chronometer));
    // returns right away if the allocation failed
    if (chronometer == NULL)
        return NULL;
    
    return chronometer;
}

// starts a Chronometer
void start_chronometer(Chronometer* chronometer) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &(chronometer->start));
}

// stops a Chronometer
float stop_chronometer(Chronometer* chronometer) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &(chronometer->stop));
    
    float delta_us = (chronometer->stop.tv_sec - chronometer->start.tv_sec) * 1000000 + (chronometer->stop.tv_nsec - chronometer->start.tv_nsec) / 1000;
    
    return (float) delta_us / 1000000.0;
}

// deletes a Chronometer
Chronometer* del_chronometer(Chronometer* chronometer) {
    free (chronometer);
    
    return NULL;
}

// test function ======================================================
float test_system(int number_of_threads) {
    // seed for the random number generator
    srand((unsigned) time(NULL));
    
    // the score obtained
    float score = 0.0;
    
    float total_time = 0.0;
    
    // parameters passed to the threads
    ALUParams* alu_params = create_alu_params(); if (alu_params == NULL) return 0.0;
    FPUParams* fpu_params = create_fpu_params(); if (fpu_params == NULL) return 0.0;
    MEMParams* mem_params = create_mem_params(); if (mem_params == NULL) return 0.0;
    GUIParams* gui_params = create_gui_params(alu_params, fpu_params, mem_params, &number_of_threads); if (gui_params == NULL) return 0.0;
    
    // starts an empty thread array
    Thread* thread_array = NULL;
    Thread* gui_thread_array = NULL;
    
    // adds the gui thread
    gui_thread_array = add_thread(gui_thread_array, 0, (void*) gui, (void*) gui_params); 
    if (gui_thread_array == NULL) {
        wait_threads(gui_thread_array); 
        return 0.0;
    }
    
    // variables that will record the time taken to complete the tasks
    Chronometer* chronometer = create_chronometer();
    
    start_chronometer(chronometer);
    for (int i=0; i<number_of_threads; i++) {
        thread_array = add_thread(thread_array, i, (void*) alu_test, (void*) alu_params); 
        if (thread_array == NULL) {
            wait_threads(thread_array); 
            return 0.0;
        }
            
        thread_array = add_thread(thread_array, i, (void*) fpu_test, (void*) fpu_params);
        if (thread_array == NULL) {
            wait_threads(thread_array); 
            return 0.0;
        }
            
        thread_array = add_thread(thread_array, i, (void*) mem_test, (void*) mem_params);
        if (thread_array == NULL) {
            wait_threads(thread_array); 
            return 0.0;
        }
    }
    wait_threads(thread_array);
    total_time += stop_chronometer(chronometer);
    wait_threads(gui_thread_array);
    
    thread_array = del_threads(thread_array);
    gui_thread_array = del_threads(gui_thread_array);
    
    // free all manually allocated memory
    alu_params = del_alu_params(alu_params);
    fpu_params = del_fpu_params(fpu_params);
    mem_params = del_mem_params(mem_params);
    gui_params = del_gui_params(gui_params);
    chronometer = del_chronometer(chronometer);
    
    // callibrated so my PC at the time gets 100 points on average in the singlethread test (the multi is about 410)
    // Intel Xeon E1280 3.6GHz with 8GB RAM DDR3 1333MHz HyperX (1600 actually but the IMC does not support 1600MHz natively nor does my MB support memory OC)
    score  = 100 * 72.888054 / total_time;
    return score;
}


