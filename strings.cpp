// Strings used by the program ========================================

#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

String get_string(MsgCode msgcode) {
    switch (msgcode) {
        case SUCCESS: return "Success!";
        case THREAD_MEMORY_ALLOCATION_ERROR: return "Thread memory allocation error!";
        case THREAD_PTHREAD_CREATION_ERROR: return "Thread creation error!";
        case THREAD_PTHREAD_AFFINITY_ERROR: return "Thread affinity setting error!";
        case THREAD_PTHREAD_JOIN_ERROR: return "Thread join error!";
        case ALU_MEMORY_ALLOCATION_ERROR: return "ALU test memory allocation error!";
        case ALU_PTHREAD_LOCK_CREATION_ERROR: return "ALU test lock creation error!";
        case ALU_PTHREAD_LOCK_INIT_ERROR: return "ALU test lock initialization error!";
        case FPU_MEMORY_ALLOCATION_ERROR: return "FPU test memory allocation error!";
        case FPU_PTHREAD_LOCK_CREATION_ERROR: return "FPU test lock creation error!";
        case FPU_PTHREAD_LOCK_INIT_ERROR: return "FPU test lock initialization error!";
        case MEM_MEMORY_ALLOCATION_ERROR: return "MEM test memory allocation error!";
        case MEM_PTHREAD_LOCK_CREATION_ERROR: return "MEM test lock creation error!";
        case MEM_PTHREAD_LOCK_INIT_ERROR: return "MEM test lock initialization error!";
        case GUI_MEMORY_ALLOCATION_ERROR: return "GUI memory allocation error!";
        case CHRONOMETER_MEMORY_ALLOCATION_ERROR: return "Chronometer memory allocation error!";

        case MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION: return "SimpleBench Version";
        case MSG_MAIN_SHOW_SCORE_SINGLETHREAD_SCORE: return "SingleThread Score";
        case MSG_MAIN_SHOW_SCORE_MULTITHREAD_SCORE: return "MultiThread Score";
        case MSG_MAIN_SHOW_SCORE_MULTIPLIER: return "Multiplier";
        case MSG_MAIN_SHOW_SCORE_ERROR: return "An error occurred during the test!";

        case GUI_SHOW_PROGRESS_BRACKET_OPEN: return "[";
        case GUI_SHOW_PROGRESS_BRACKET_CLOSE: return "]";
        case GUI_SHOW_PROGRESS_FILLED: return "=";
        case GUI_SHOW_PROGRESS_NOT_FILLED: return "-";
        case GUI_GUI_HEADER_1: return "Testing the system with ";
        case GUI_GUI_HEADER_2: return " core(s):";
        case GUI_GUI_ALU_HEADER: return "ALU Test: ";
        case GUI_GUI_FPU_HEADER: return "FPU Test: ";
        case GUI_GUI_MEM_HEADER: return "MEM Test: ";
        case GUI_GUI_FINISHED_MSG_1: return "Finished the test with ";
        case GUI_GUI_FINISHED_MSG_2: return " core(s)!";
    };

    return "undefined";
}