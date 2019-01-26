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
    ALUParams* alu_params = NULL;
    create_alu_params(&alu_params); 

    FPUParams* fpu_params = NULL;
    create_fpu_params(&fpu_params); 

    MEMParams* mem_params = NULL;
    create_mem_params(&mem_params); 

    GUIParams* gui_params = NULL;
    create_gui_params(&gui_params, alu_params, fpu_params, mem_params, &number_of_threads);
    
    // starts an empty thread array
    Thread* thread_array = NULL;
    Thread* gui_thread_array = NULL;
    
    // adds the gui thread
    add_thread(&gui_thread_array, 0, (void*) gui, (void*) gui_params);
    
    // variables that will record the time taken to complete the tasks
    Chronometer* chronometer = NULL;
    create_chronometer(&chronometer);
    
    start_chronometer(&chronometer);
    for (int i=0; i<number_of_threads; i++) {
        add_thread(&thread_array, i, (void*) alu_test, (void*) alu_params); 
        add_thread(&thread_array, i, (void*) fpu_test, (void*) fpu_params);
        add_thread(&thread_array, i, (void*) mem_test, (void*) mem_params);
    }
    // waits for all compute threads to finish and stops the chronometer immediately
    wait_threads(&thread_array);
    total_time += stop_chronometer(&chronometer);
    // once all threads finished, wait for the gui thread to finish too
    wait_threads(&gui_thread_array);

    // double check if all tasks really finished
    if ((*get_alu_params_job_size(&alu_params) <= 0) && (*get_fpu_params_job_size(&fpu_params) <= 0) && (*get_mem_params_job_size(&mem_params) <= 0)) {
        // callibrated so my PC at the time gets 100 points on average in the singlethread test (the multi is about 410)
        // Intel Xeon E1280 3.6GHz with 8GB RAM DDR3 1333MHz HyperX (1600 actually but the IMC does not support 1600MHz natively nor does my MB support memory OC)
        score = 100 * 72.888054 / total_time;
    }
    
    // frees up all allocated threads
    del_threads(&thread_array);
    del_threads(&gui_thread_array);
    
    // frees up all manually allocated memory
    del_alu_params(&alu_params);
    del_fpu_params(&fpu_params);
    del_mem_params(&mem_params);
    del_gui_params(&gui_params);
    del_chronometer(&chronometer);

    return score;
}