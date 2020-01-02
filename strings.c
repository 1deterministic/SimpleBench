#include "simplebench.h"

String get_string(MsgCode msgcode) {
    switch (msgcode) {
        case SUCCESS: return "Success!";

        case THREAD_MEMORY_ALLOCATION_ERROR: return "Thread memory allocation error!";
        case THREAD_THREAD_CREATION_ERROR: return "Thread creation error!";
        case THREAD_THREAD_AFFINITY_ERROR: return "Thread affinity setting error!";
        case THREAD_THREAD_JOIN_ERROR: return "Thread join error!";
        case THREAD_THREAD_PRIORITY_ERROR: return "Thread priority setting error!";
        case ALU_MEMORY_ALLOCATION_ERROR: return "ALU test memory allocation error!";
        case ALU_THREAD_LOCK_CREATION_ERROR: return "ALU test lock creation error!";
        case ALU_THREAD_LOCK_INIT_ERROR: return "ALU test lock initialization error!";
        case FPU_MEMORY_ALLOCATION_ERROR: return "FPU test memory allocation error!";
        case FPU_THREAD_LOCK_CREATION_ERROR: return "FPU test lock creation error!";
        case FPU_THREAD_LOCK_INIT_ERROR: return "FPU test lock initialization error!";
        case MEM_MEMORY_ALLOCATION_ERROR: return "MEM test memory allocation error!";
        case MEM_THREAD_LOCK_CREATION_ERROR: return "MEM test lock creation error!";
        case MEM_THREAD_LOCK_INIT_ERROR: return "MEM test lock initialization error!";
        case GUI_MEMORY_ALLOCATION_ERROR: return "GUI memory allocation error!";
        case CHRONOMETER_MEMORY_ALLOCATION_ERROR: return "Chronometer memory allocation error!";
        case PAYLOAD_MEMORY_ALLOCATION_ERROR: return "Payload memory allocation error!";

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
        case MSG_GET_CLI_OPTIONS_PINTHREADS_NOT_SUPPORTED: return "This OS does not offer an interface to pin threads to specific cores!";

        case GUI_SHOW_PROGRESS_BRACKET_OPEN: return "[";
        case GUI_SHOW_PROGRESS_BRACKET_CLOSE: return "]";
        case GUI_SHOW_PROGRESS_FILLED: return "=";
        case GUI_SHOW_PROGRESS_NOT_FILLED: return "-";
        case GUI_GUI_HEADER_1: return "Testing the system with ";
        case GUI_GUI_HEADER_2: return " thread(s):";
        case GUI_GUI_ALU_HEADER: return "ALU Test: ";
        case GUI_GUI_FPU_HEADER: return "FPU Test: ";
        case GUI_GUI_MEM_HEADER: return "MEM Test: ";
        case GUI_GUI_FINISHED_MSG_1: return "Finished the test with ";
        case GUI_GUI_FINISHED_MSG_2: return " thread(s)!";
        case GUI_GUI_LOADING_TEST: return "Initializing test...\n";

        case MSG_HELP_TEXT: return
        "SimpleBench - a quick benchmark to evaluate the core system performance\n"
        "--show-gui [on/off]: enable/disable the text feedback during the tests\n"
        "--st-test [on/off]: enable/disable the singlethread test\n"
        "--mt-test [on/off]: enable/disable the multithread test\n"
        "--threads [integer greater than zero]: sets the number of threads to run with\n"
        "--pin-threads [on/off]: enable/disable pinning process threads to system threads, does not work on macOS\n"
        "--hardware-level [integer greater than zero]: chooses how taxing the benchmark will be\n"
        "      1: ~4MB of RAM, 1 lenght unit\n"
        "      2: ~16MB of RAM, 4 lenght units\n"
        "      3: ~64MB of RAM, 16 lenght units\n"
        "      4: ~256MB of RAM, 64 lenght units\n"
        "      5: ~1G of RAM, 256 lenght units\n"
        "      6: ~4GB of RAM, 1024 lenght units\n"
        "       :\n"
        "      * every increase of 1 in the hardware level quadruples the requirements\n"
        "      * at the moment there is no limit to the hardware level\n"
        "--version: shows the benchmark version (not the program version)\n"
        "--build: shows the program version, changes here have no effect on the score system\n"
        "--help: shows this screen\n"
        "\n"
        "You can check some of my benchmark results on https://github.com/1deterministic/SimpleBench for comparison\n"
        ;

        default: return "Unknown error!";
    };

    return "undefined";
}