#include "simplebench.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#if __linux__ || __APPLE__
    #include <unistd.h>
    #include <pthread.h>
#elif _WIN32
#endif

// struct of the GUIParams type
struct gui_prm {
    int* cores;
    ALUParams* alu;
    FPUParams* fpu;
    MEMParams* mem;
};

// creates a GUIParams
MsgCode create_gui_params(GUIParams** gui_params, ALUParams* alu_params, FPUParams* fpu_params, MEMParams* mem_params, int* cores) {
    // manually allocates memory to the ALUParams type
    *gui_params = (GUIParams*) malloc(sizeof(GUIParams));
    // returns right away if the allocation failed
    if (*gui_params == NULL) {
        return GUI_MEMORY_ALLOCATION_ERROR;
    }
        
    // fills the parameter values
    set_gui_params_alu_params(gui_params, alu_params);
    set_gui_params_fpu_params(gui_params, fpu_params);
    set_gui_params_mem_params(gui_params, mem_params);
    set_gui_params_cores(gui_params, cores);
    
    // returns the parameter
    return SUCCESS;
}

// deletes a GUIParams
MsgCode del_gui_params(GUIParams** gui_params) {
    if (*gui_params == NULL)
        return SUCCESS;

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
void print_progress(int current, int total) {
    int width = DEFAULT_TERMINAL_WIDTH - 2;

    for (int i = 0; i < width; i++) {
        // the first character of the progress bar is [
        if (i == 0)
            printf("%s", get_string(GUI_SHOW_PROGRESS_BRACKET_OPEN));
        // all subsequent chars are '=' until the current progression is reached
        if (i <= (int) width * ((float) (total - current)) / ((float) total))
            printf("%s", get_string(GUI_SHOW_PROGRESS_FILLED));
        // after that, fill with '-'
        else
            printf("%s", get_string(GUI_SHOW_PROGRESS_NOT_FILLED));
        // close with ]
        if (i == (width - 1))
            printf("%s\n", get_string(GUI_SHOW_PROGRESS_BRACKET_CLOSE));
    }
}

// erases terminal text lines at will (not working in powershell and cmd)
void erase_lines(int count) {
    for (int i = 0; i <= count; i++) {
        // returns the cursor to the begining of the line
        printf("\r");
        // erases the current line
        printf("\e[K");
        // moves the cursor up if it's not the last line to erase, else simply returns to the start of the line
        if (i < count) {
            printf("\033[A");
        } else {
            printf("\r");   
        }
    }
}

// function that runs on the GUI thread
void* gui(void* params) {
    int alu_job_size = ALU_JOB_SIZE;
    int fpu_job_size = FPU_JOB_SIZE;
    int mem_job_size = MEM_JOB_SIZE;

    // converts back the params to the expected type
    GUIParams** gui_params = (GUIParams**) &params;
    ALUParams* alu_params = NULL;
    FPUParams* fpu_params = NULL;
    MEMParams* mem_params = NULL;

    // waits until all init tasks are completed
    while (alu_params == NULL || fpu_params == NULL || mem_params == NULL) {
        // updates local references
        alu_params = get_gui_params_alu_params(gui_params);
        fpu_params = get_gui_params_fpu_params(gui_params);
        mem_params = get_gui_params_mem_params(gui_params);

        printf("%s", get_string(GUI_GUI_LOADING_TEST));

        #if __linux__ || __APPLE__
            sleep(1);
        #elif _WIN32
            Sleep(1000);
        #endif
        
        erase_lines(1);
    }

    // creates local references
    int* cores_used = get_gui_params_cores(gui_params);;
    int* alu_job = get_alu_params_job_size(&alu_params);
    int* fpu_job = get_fpu_params_job_size(&fpu_params);
    int* mem_job = get_mem_params_job_size(&mem_params);
    
    // while the test is still in running, shows the current progress each second
    while (*alu_job > 0 || *fpu_job > 0 || *mem_job > 0) {
        printf("%s%d%s\n\n", get_string(GUI_GUI_HEADER_1), *cores_used, get_string(GUI_GUI_HEADER_2));
        printf("%s%5.2f%%\n", get_string(GUI_GUI_ALU_HEADER), 100.0 * ((float) (alu_job_size - *alu_job)) / (float) alu_job_size);
        print_progress(*alu_job, alu_job_size);
        printf("%s%5.2f%%\n", get_string(GUI_GUI_FPU_HEADER), 100.0 * ((float) (fpu_job_size - *fpu_job)) / (float) fpu_job_size);
        print_progress(*fpu_job, fpu_job_size);
        printf("%s%5.2f%%\n", get_string(GUI_GUI_MEM_HEADER), 100.0 * ((float) (mem_job_size - *mem_job)) / (float) mem_job_size);
        print_progress(*mem_job, mem_job_size);

        #if __linux__ || __APPLE__
            sleep(1);
        #elif _WIN32
            Sleep(1000);
        #endif
        
        erase_lines(8);
    }

    printf("%s%d%s\n", get_string(GUI_GUI_FINISHED_MSG_1), *cores_used, get_string(GUI_GUI_FINISHED_MSG_2));

    #if __linux__ || __APPLE__
        sleep(1);
    #elif _WIN32
        Sleep(1000);
    #endif

    erase_lines(1);
    return NULL;
}