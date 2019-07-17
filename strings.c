#include "simplebench.h"

String get_string(MsgCode msgcode) {
    switch (msgcode) {
        case SUCCESS: return "Success!";
        case THREAD_MEMORY_ALLOCATION_ERROR: return "Thread memory allocation error!";
        case THREAD_PTHREAD_CREATION_ERROR: return "Thread creation error!";
        case THREAD_PTHREAD_AFFINITY_ERROR: return "Thread affinity setting error!";
        case THREAD_PTHREAD_JOIN_ERROR: return "Thread join error!";
        case THREAD_PTHREAD_PRIORITY_ERROR: return "Thread priority setting error!";
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

        case MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_HEADER: return "SimpleBench Result";
        case MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION: return "Benchmark Version";
        case MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_BUILD: return "Program Build";
        case MSG_MAIN_SHOW_SCORE_HARDWARE_LEVEL: return "Hardware Level";
        case MSG_MAIN_SHOW_SCORE_SINGLETHREAD_SCORE: return "SingleThread Score";
        case MSG_MAIN_SHOW_SCORE_MULTITHREAD_SCORE: return "MultiThread Score";
        case MSG_MAIN_SHOW_SCORE_MULTIPLIER: return "Multiplier";
        case MSG_MAIN_SHOW_SCORE_ERROR: return "An error occurred during the test!";
        case MSG_GET_CLI_OPTIONS_INVALID_ONOFF: return "Invalid on/off value!";
        case MSG_GET_CLI_OPTIONS_MISSING_ONOFF: return "Missing on/off value!";
        case MSG_GET_CLI_OPTIONS_INVALID_INT: return "Invalid number!";
        case MSG_GET_CLI_OPTIONS_MISSING_INT: return "Missing thread count!";
        case MSG_GET_CLI_OPTIONS_UNKNOWN_OPTION: return "Option not recognized!";
        case MSG_GET_CLI_OPTIONS_VERSION: return "SimpleBench benchmark version: " BENCHMARK_VERSION;
        case MSG_GET_CLI_OPTIONS_BUILD: return "SimpleBench build version: " BUILD_VERSION;

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

        case MSG_HELP_TEXT: return
        "SimpleBench - a quick benchmark to evaluate the core system performance\n"
        "--show-gui [on/off]: enable/disable the text feedback during the tests\n"
        "--st-test [on/off]: enable/disable the singlethread test\n"
        "--mt-test [on/off]: enable/disable the multithread test\n"
        "--threads [integer greater than zero]: sets the number of threads to run with\n"
        "--hardware-level [integer greater than zero]: chooses how taxing the benchmark will be\n"
        "      1: 32MB of RAM, 64KB of cache, lenght 1\n"
        "      2: 64MB of RAM, 128KB of cache, lenght 4\n"
        "      3: 128MB of RAM, 256KB of cache, lenght 16\n"
        "      4: 512MB of RAM, 1MB of cache, lenght 64\n"
        "      5: 2GB of RAM, 4MB of cache, lenght 256 (this is the current default)\n"
        "      6: 8GB of RAM, 16MB of cache, lenght 1024 (HEDT users may want to use this one)\n"
        "       :\n"
        "      * every increase of 1 in the hardware level quadruples the requirements\n"
        "      * there is some overhead in RAM requirement, you should have at least the amount required plus 5% FREE\n"
        "--version: shows the benchmark version (not the program version)\n"
        "--build: shows the program version, changes here have no effect on the score system\n"
        "--help: shows this screen\n"
        "\n"
        "You can check some of my benchmark results on https://github.com/1deterministic/SimpleBench for comparison\n"
        ;
    };

    return "undefined";
}