#include "simplebench.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// test function ======================================================
MsgCode test_system(float* score, int threads, bool pin_threads, float handicap, bool show_gui, int alu_matrix_size, int fpu_matrix_size, int mem_matrix_size) {
    // the score obtained
    float total_time = 0.0;
    float test_score = 0.0;

    // default return code is success (0)
    MsgCode code = SUCCESS;

    // thread parameters  
    ALUParams* alu_params = NULL;
    FPUParams* fpu_params = NULL;
    MEMParams* mem_params = NULL;
    GUIParams* gui_params = NULL;

    // payload array
    Payload* payload = NULL;

    // thread arrays
    Thread* thread_array = NULL;
    Thread* gui_thread_array = NULL;

    // chronometer
    Chronometer* chronometer = NULL;

    // creates the gui parameters
    if (!code) {
        code = create_gui_params(&gui_params, alu_params, fpu_params, mem_params, &threads);
    }

    // starts the gui thread
    if (show_gui) {
        if (!code) {
            code = add_thread(&gui_thread_array, 0, THREAD_PRIORITY_ABOVE_NORMAL, (void*) gui, (void*) gui_params);
        }
    }

    // creates a chronometer
    if (!code) {
        code = create_chronometer(&chronometer);
    }

    // creates the alu test parameters
    if (!code) {
        code = create_alu_params(&alu_params, alu_matrix_size); 
    }

    // creates a payload containing the alu test and its parameters
    if (!code) {
        code = add_payload(&payload, alu_test, alu_params);
    }

    // creates the fpu test parameters
    if (!code) {
        code = create_fpu_params(&fpu_params, fpu_matrix_size); 
    }

    // creates a payload containing the fpu test and its parameters
    if (!code) {
        code = add_payload(&payload, fpu_test, fpu_params);
    }

    // creates the mem test parameters
    if (!code) {
        code = create_mem_params(&mem_params, mem_matrix_size);
    }

    // creates a payload containing the mem test and its parameters
    if (!code) {
        code = add_payload(&payload, mem_test, mem_params);
    }

    // sends these to the gui thread for it to display progress
    set_gui_params_alu_params(&gui_params, alu_params);
    set_gui_params_fpu_params(&gui_params, fpu_params);
    set_gui_params_mem_params(&gui_params, mem_params);
    
    // starts the chronometer
    start_chronometer(&chronometer);

    // creates a thread for every processor thread available running all tasks inside the payload
    for (int i = 0; i < threads; i++) {
        if (!code) {
            code = add_thread(&thread_array, (pin_threads) ? i : -1, THREAD_PRIORITY_NORMAL, (void*) run_payload, (void*) payload);
        }
    }
    // waits for all compute threads to finish
    if (!code) {
        code = wait_threads(&thread_array);
    }

    // stops the chronometer
    total_time += stop_chronometer(&chronometer);
    
    // once all threads finished, wait for the gui thread to finish too
    if (!code) {
        code = wait_threads(&gui_thread_array);
    }

    // double check if all tasks really finished (could have crashed halfway)
    if (!code && (*get_alu_params_job_size(&alu_params) <= 0) && (*get_fpu_params_job_size(&fpu_params) <= 0) && (*get_mem_params_job_size(&mem_params) <= 0)) {
        // only then calculate the test score
        test_score = handicap * 100.0 * SCORE_CALIBRATION_FACTOR / total_time;
    }

    // frees up all manually allocated memory
    cleanup(&thread_array, &gui_thread_array, &payload, &alu_params, &fpu_params, &mem_params, &gui_params, &chronometer);

    if (CALIBRATION_BUILD) {
        // shows the time it took to complete, for calibration only
        printf("Total time: %f\n", total_time);
        // invalidates the result
        test_score = 0.0;
    }
    
    // returns the score
    *score = test_score;
    return code;
}

void cleanup(Thread** thread_array, Thread** gui_thread_array, Payload** payload, ALUParams** alu_params, FPUParams** fpu_params, MEMParams** mem_params, GUIParams** gui_params,  Chronometer** chronometer) {
    // frees up all manually allocated memory
    del_threads(gui_thread_array);
    del_threads(thread_array);
    del_payload(payload);
    del_chronometer(chronometer);
    del_gui_params(gui_params);
    del_alu_params(alu_params);
    del_fpu_params(fpu_params);
    del_mem_params(mem_params);
}