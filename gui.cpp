// GUIParams type ======================================================

#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// struct of the GUIParams type
struct gui_prm {
    int* cores;
    ALUParams* alu;
    FPUParams* fpu;
    MEMParams* mem;
};

// creates a GUIParams
ErrorCode create_gui_params(GUIParams** gui_params, ALUParams* alu_params, FPUParams* fpu_params, MEMParams* mem_params, int* cores) {
    // manually allocates memory to the ALUParams type
    *gui_params = (GUIParams*) malloc(sizeof(GUIParams));
    // returns right away if the allocation failed
    if (*gui_params == NULL)
        return GUI_MEMORY_ALLOCATION_ERROR;
    
    // fills the parameter values
    set_gui_params_alu_params(gui_params, alu_params);
    set_gui_params_fpu_params(gui_params, fpu_params);
    set_gui_params_mem_params(gui_params, mem_params);
    set_gui_params_cores(gui_params, cores);
    
    // returns the parameter
    return SUCCESS;
}

// deletes a GUIParams
ErrorCode del_gui_params(GUIParams** gui_params) {
    free (*gui_params);
    return SUCCESS;
}

void set_gui_params_cores(GUIParams** gui_params, int* cores) {
    (*gui_params)->cores = cores;
}

int* get_gui_params_cores(GUIParams** gui_params) {
    return (*gui_params)->cores;
}

void set_gui_params_alu_params(GUIParams** gui_params, ALUParams* alu_params) {
    (*gui_params)->alu = alu_params;
}

ALUParams* get_gui_params_alu_params(GUIParams** gui_params) {
    return (*gui_params)->alu;
}

void set_gui_params_fpu_params(GUIParams** gui_params, FPUParams* fpu_params) {
    (*gui_params)->fpu = fpu_params;
}

FPUParams* get_gui_params_fpu_params(GUIParams** gui_params) {
    return (*gui_params)->fpu;
}

void set_gui_params_mem_params(GUIParams** gui_params, MEMParams* mem_params) {
    (*gui_params)->mem = mem_params;
}

MEMParams* get_gui_params_mem_params(GUIParams** gui_params) {
    return (*gui_params)->mem;
}

// shows the progress bar (helper to the gui thread)
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

// I'm NOT proud of this piece of code
void erase_lines(int count) {
    for (int i = 0; i < count; i++) {
        // will erase 80 characters
        printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        printf("                                                                                ");
        printf("\033[1A");
        printf("\r");
    }
}

void* gui(void* params) {
    // converts back the params to the expected type
    GUIParams** gui_params = (GUIParams**) &params;

    ALUParams* alu_params = get_gui_params_alu_params(gui_params);
    FPUParams* fpu_params = get_gui_params_fpu_params(gui_params);
    MEMParams* mem_params = get_gui_params_mem_params(gui_params);
    
    int* cores_used = get_gui_params_cores(gui_params);
    int* alu_job = get_alu_params_job_size(&alu_params);
    int* fpu_job = get_fpu_params_job_size(&fpu_params);
    int* mem_job = get_mem_params_job_size(&mem_params);
    
    // while the test is still in running, shows the current progress each second
    while (*alu_job > 0 || *fpu_job > 0 || *mem_job > 0) {
        printf("Testing the system with %d core(s):\n\n", *cores_used);
        printf("ALU Test: %.2f%%\n", 100.0 * ((float) (alu_job_size - *alu_job)) / ((float) alu_job_size));
        print_progress(*alu_job, alu_job_size);
        printf("FPU Test: %.2f%%\n", 100.0 * ((float) (fpu_job_size - *fpu_job)) / ((float) fpu_job_size));
        print_progress(*fpu_job, fpu_job_size);
        printf("MEM Test: %.2f%%\n", 100.0 * ((float) (mem_job_size - *mem_job)) / ((float) mem_job_size));
        print_progress(*mem_job, mem_job_size);
        sleep(1);

        erase_lines(8);
    }

    printf("Finished the test with %d core(s)!\n", *cores_used);
    sleep(2);

    erase_lines(1);
    
    return NULL;
}