#include "simplebench.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

struct group_prm {
    void** functions;
    void** parameters;
};

// test function ======================================================
MsgCode test_system(float* score, int threads, float handicap, bool show_gui) {
    // seed for the random number generator
    srand((unsigned) time(NULL));

    MsgCode code = SUCCESS;

    // thread parameters  
    ALUParams* alu_params = NULL;
    FPUParams* fpu_params = NULL;
    MEMParams* mem_params = NULL;
    GUIParams* gui_params = NULL;

    // thread arrays
    Thread* thread_array = NULL;
    Thread* gui_thread_array = NULL;

    // chronometer
    Chronometer* chronometer = NULL;
    
    // the score obtained
    float total_time = 0.0;
    float test_score = 0.0;

    // variables that will record the time taken to complete the tasks
    code = create_chronometer(&chronometer);
    if (code) {cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

    // allocates the parameters passed to the threads
    code = create_alu_params(&alu_params); 
    if (code) {cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

    code = create_fpu_params(&fpu_params);
    if (code) {cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

    code = create_mem_params(&mem_params); 
    if (code) {cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

    code = create_gui_params(&gui_params, alu_params, fpu_params, mem_params, &threads);
    if (code) {cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

    // starts the gui thread
    if (show_gui) {
        code = add_thread(&gui_thread_array, 0, THREAD_PRIORITY_ABOVE_NORMAL, (void*) gui, (void*) gui_params);
        if (code) {cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}
    }
    
    start_chronometer(&chronometer);
    for (int i=0; i<threads; i++) {
        code = add_thread(&thread_array, i, THREAD_PRIORITY_NORMAL, (void*) alu_test, (void*) alu_params);
        if (code) {stop_threads(&gui_thread_array); stop_threads(&thread_array); cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

        code = add_thread(&thread_array, i, THREAD_PRIORITY_NORMAL, (void*) fpu_test, (void*) fpu_params);
        if (code) {stop_threads(&gui_thread_array); stop_threads(&thread_array); cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

        code = add_thread(&thread_array, i, THREAD_PRIORITY_NORMAL, (void*) mem_test, (void*) mem_params);
        if (code) {stop_threads(&gui_thread_array); stop_threads(&thread_array); cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}
    }
    // waits for all compute threads to finish and stops the chronometer immediately
    code = wait_threads(&thread_array);
    total_time += stop_chronometer(&chronometer);
    
    // once all threads finished, wait for the gui thread to finish too
    code = wait_threads(&gui_thread_array);
    if (code) {stop_threads(&gui_thread_array); stop_threads(&thread_array); cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer); return code;}

    // double check if all tasks really finished
    if ((*get_alu_params_job_size(&alu_params) <= 0) && (*get_fpu_params_job_size(&fpu_params) <= 0) && (*get_mem_params_job_size(&mem_params) <= 0)) {
        test_score = handicap * 100 * SCORE_CALIBRATION_FACTOR / total_time;
    }

    cleanup(&thread_array, &gui_thread_array, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer);
    // printf("TOTAL TIME: %f\n", total_time); // For calibration only
    *score = test_score;
    return SUCCESS;
}

void cleanup(Thread** thread_array, Thread** gui_thread_array, ALUParams** alu_params, FPUParams** fpu_params, MEMParams** mem_params, GUIParams** gui_params,  Chronometer** chronometer) {
    // frees up all allocated threads
    del_threads(gui_thread_array);
    del_threads(thread_array);
    
    // frees up all manually allocated memory
    del_chronometer(chronometer);
    del_gui_params(gui_params);
    del_alu_params(alu_params);
    del_fpu_params(fpu_params);
    del_mem_params(mem_params);
}