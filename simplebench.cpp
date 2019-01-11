#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// test function ======================================================
float test_system(int number_of_threads) {
    // seed for the random number generator
    srand((unsigned) time(NULL));
    
    // the score obtained
    float score = 0.0;
    // the time taken to complete the test
    float total_time = 0.0;
    
    // parameters passed to the threads
    ALUParams* alu_params = create_alu_params(); 
    if (alu_params == NULL) {
        return 0.0;
    }

    FPUParams* fpu_params = create_fpu_params(); 
    if (fpu_params == NULL) {
        return 0.0;
    }

    MEMParams* mem_params = create_mem_params(); 
    if (mem_params == NULL) {
        return 0.0;
    }

    GUIParams* gui_params = create_gui_params(alu_params, fpu_params, mem_params, &number_of_threads);
    if (gui_params == NULL) {
        return 0.0;
    }
    
    // starts an empty thread array
    Thread* thread_array = NULL;
    Thread* gui_thread_array = NULL;
    // error checking
    Thread* current = NULL;
    
    // adds the gui thread
    gui_thread_array = add_thread(gui_thread_array, 0, (void*) gui, (void*) gui_params); 
    if (gui_thread_array == NULL) {
        wait_threads(gui_thread_array); 
        return 0.0;
    }
    
    // variables that will record the time taken to complete the tasks
    Chronometer* chronometer = create_chronometer();
    if (chronometer == NULL) {
        return 0.0;
    }
    
    start_chronometer(chronometer);
    for (int i=0; i<number_of_threads; i++) {
        current = thread_array;
        thread_array = add_thread(thread_array, i, (void*) alu_test, (void*) alu_params); 
        if (thread_array == current) {
            wait_threads(thread_array); 
            return 0.0;
        }

        current = thread_array;  
        thread_array = add_thread(thread_array, i, (void*) fpu_test, (void*) fpu_params);
        if (thread_array == current) {
            wait_threads(thread_array); 
            return 0.0;
        }

        current = thread_array;
        thread_array = add_thread(thread_array, i, (void*) mem_test, (void*) mem_params);
        if (thread_array == current) {
            wait_threads(thread_array); 
            return 0.0;
        }
    }
    // waits for all compute threads to finish and stops the chronometer immediately
    wait_threads(thread_array);
    total_time += stop_chronometer(chronometer);
    // once all threads finished, wait for the gui thread to finish too
    wait_threads(gui_thread_array);

    // double check if all tasks really finished
    if ((*get_alu_params_job_size(alu_params) <= 0) && 
        (*get_fpu_params_job_size(fpu_params) <= 0) &&
        (*get_mem_params_job_size(mem_params) <= 0)) {
            // callibrated so my PC at the time gets 100 points on average in the singlethread test (the multi is about 410)
            // Intel Xeon E1280 3.6GHz with 8GB RAM DDR3 1333MHz HyperX (1600 actually but the IMC does not support 1600MHz natively nor does my MB support memory OC)
            score = 100 * 72.888054 / total_time;
        } else {
            score = 0.0;
        }
    
    // frees up all allocated threads
    thread_array = del_threads(thread_array);
    gui_thread_array = del_threads(gui_thread_array);
    
    // frees up all manually allocated memory
    alu_params = del_alu_params(alu_params);
    fpu_params = del_fpu_params(fpu_params);
    mem_params = del_mem_params(mem_params);
    gui_params = del_gui_params(gui_params);
    chronometer = del_chronometer(chronometer);

    return score;
}