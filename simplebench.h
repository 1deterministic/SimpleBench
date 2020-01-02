#include <stdbool.h>

#if __linux__ || __APPLE__
    #define _GNU_SOURCE
    #include <pthread.h>
    #define THREAD_PRIORITY_ABOVE_NORMAL 0
    #define THREAD_PRIORITY_NORMAL 0
#elif _WIN32
    #include <windows.h>
#endif

// if I ever change this it means that scores from different versions are not comparable: number means minor changes on the same algorithm, letter means substantial changes to the algorithm or a whole new one
#define BENCHMARK_VERSION "B0"
// this is the actual program version, changes here won't affect system scores
#define BUILD_NUMBER "2"

#if __linux__
    #define BUILD_OS "linux"
    #if __i386__
        #define BUILD_ARCH "x86"
    #elif __x86_64__
        #define BUILD_ARCH "amd64"
    #elif __arm__
        #define BUILD_ARCH "armv7"
    #elif __aarch64__
        #define BUILD_ARCH "aarch64"
    #endif
#elif _WIN32
    #define BUILD_OS "windows"
    #if _M_IX86 || __i386__
        #define BUILD_ARCH "x86"
    #elif _M_AMD64 || __x86_64__
        #define BUILD_ARCH "amd64"
    #endif
#elif __APPLE__
    #define BUILD_OS "macos"
    #define BUILD_ARCH "amd64"
#else
    #define BUILD_OS "unknown"
    #define BUILD_ARCH "unknown"
#endif

#if _MSC_VER
    #define BUILD_CC "msvc"
#elif __clang__
    #define BUILD_CC "clang"
#elif __GNUC__ 
    #define BUILD_CC "gcc"
#elif __MINGW32__ || __MINGW64__
    #define BUILD_CC "mingw"
#else
    #define BUILD_CC "unknown"
#endif


#define BUILD_VERSION BUILD_NUMBER"-"BUILD_OS"-"BUILD_ARCH"-"BUILD_CC
#define BUILD_CODENAME "Bring it on"

// the time a Sandy Bridge processor at 3GHz with 16GB of dual channel 1333MHz DDR3 memory takes to complete the single threaded test for version B0 hardware level 5 (default) in seconds
// running Fedora 31, linux kernel 5.3.16, mitigations=auto
// generic build, cli options: "--mt-test off --pin-threads on"
#define SCORE_CALIBRATION_FACTOR 102.626572
#define CALIBRATION_BUILD false

#define CLI_ON "on"
#define CLI_OFF "off"

#define CLI_SHOW_GUI "--show-gui"
#define CLI_ST_TEST "--st-test"
#define CLI_MT_TEST "--mt-test"
#define CLI_THREADS "--threads"
#define CLI_PIN_THREADS "--pin-threads"
#define CLI_HARDWARE_LEVEL "--hardware-level"
#define CLI_HELP "--help"
#define CLI_VERSION "--version"
#define CLI_BUILD "--build"

#define DEFAULT_CONFIG_HARDWARE 5
#define DEFAULT_SHOW_GUI true
#define DEFAULT_ST_TEST true
#define DEFAULT_MT_TEST true
#define DEFAULT_PIN_THREADS false
#define DEFAULT_THREADS 4
#define DEFAULT_TERMINAL_WIDTH 80

// reference values for task sizes
#define ALU_JOB_SIZE 262144;
#define FPU_JOB_SIZE 131072;
#define MEM_JOB_SIZE 65536;

typedef unsigned int MsgCode;

#define SUCCESS 0

// main file
#define MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_HEADER 1015
#define MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION 1000
#define MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_BUILD 1013
#define MSG_MAIN_SHOW_SCORE_HARDWARE_LEVEL 1014
#define MSG_MAIN_SHOW_SCORE_SINGLETHREAD_SCORE 1001
#define MSG_MAIN_SHOW_SCORE_MULTITHREAD_SCORE 1002
#define MSG_MAIN_SHOW_SCORE_MULTIPLIER 1003
#define MSG_MAIN_SHOW_SCORE_ERROR 1004
#define MSG_GET_CLI_OPTIONS_INVALID_ONOFF 1005
#define MSG_GET_CLI_OPTIONS_MISSING_ONOFF 1006
#define MSG_GET_CLI_OPTIONS_INVALID_INT 1007
#define MSG_GET_CLI_OPTIONS_MISSING_INT 1008
#define MSG_GET_CLI_OPTIONS_UNKNOWN_OPTION 1009
#define MSG_GET_CLI_OPTIONS_VERSION 1010
#define MSG_GET_CLI_OPTIONS_BUILD 1011
#define MSG_GET_CLI_OPTIONS_PINTHREADS_NOT_SUPPORTED 1016
#define MSG_HELP_TEXT 1012
MsgCode get_cli_options(int, char**, bool*, bool*, bool*, int*, int*, bool*);
void show_score(float, float, int, int);

// Thread type
#define THREAD_MEMORY_ALLOCATION_ERROR 100
#define THREAD_THREAD_CREATION_ERROR 101
#define THREAD_THREAD_AFFINITY_ERROR 102
#define THREAD_THREAD_JOIN_ERROR 103
#define THREAD_THREAD_PRIORITY_ERROR 104
typedef struct thd Thread;
MsgCode add_thread(Thread**, int, int, void*, void*);
MsgCode del_threads(Thread**);
MsgCode wait_threads(Thread**);
MsgCode stop_threads(Thread**);
void set_thread_instance(Thread**, void*);
void* get_thread_instance(Thread**);
void set_next(Thread**, Thread*);
Thread* get_next(Thread**);

// ALUParams type
#define ALU_MEMORY_ALLOCATION_ERROR 200
#define ALU_THREAD_LOCK_CREATION_ERROR 201
#define ALU_THREAD_LOCK_INIT_ERROR 202
typedef struct alu_prm ALUParams;
MsgCode create_alu_params(ALUParams**, int);
MsgCode del_alu_params(ALUParams**);
void set_alu_params_job_size(ALUParams**, int*);
int* get_alu_params_job_size(ALUParams**);
void set_alu_params_matrix(ALUParams**, int**);
int** get_alu_params_matrix(ALUParams**);
void set_alu_params_matrix_size(ALUParams**, int*);
int* get_alu_params_matrix_size(ALUParams**);
void set_alu_params_lock(ALUParams**, void*);
void* get_alu_params_lock(ALUParams**);
void* alu_test(void*);

// FPUParams type
#define FPU_MEMORY_ALLOCATION_ERROR 300
#define FPU_THREAD_LOCK_CREATION_ERROR 301
#define FPU_THREAD_LOCK_INIT_ERROR 302
typedef struct fpu_prm FPUParams;
MsgCode create_fpu_params(FPUParams**, int);
MsgCode del_fpu_params(FPUParams**);
void set_fpu_params_job_size(FPUParams**, int*);
int* get_fpu_params_job_size(FPUParams**);
void set_fpu_params_matrix(FPUParams**, float**);
float** get_fpu_params_matrix(FPUParams**);
void set_fpu_params_matrix_size(FPUParams**, int*);
int* get_fpu_params_matrix_size(FPUParams**);
void set_fpu_params_lock(FPUParams**, void*);
void* get_fpu_params_lock(FPUParams**);
void* fpu_test(void*);

// MEMParams type
#define MEM_MEMORY_ALLOCATION_ERROR 400
#define MEM_THREAD_LOCK_CREATION_ERROR 401
#define MEM_THREAD_LOCK_INIT_ERROR 402
typedef struct mem_prm MEMParams;
MsgCode create_mem_params(MEMParams**, int);
MsgCode del_mem_params(MEMParams**);
void set_mem_params_job_size(MEMParams**, int*);
int* get_mem_params_job_size(MEMParams**);
void set_mem_params_matrix(MEMParams**, int**);
int** get_mem_params_matrix(MEMParams**);
void set_mem_params_matrix_size(MEMParams**, int*);
int* get_mem_params_matrix_size(MEMParams**);
void set_mem_params_lock(MEMParams**, void*);
void* get_mem_params_lock(MEMParams**);
void* mem_test(void*);

//GUIParams type
#define GUI_MEMORY_ALLOCATION_ERROR 500
#define GUI_SHOW_PROGRESS_BRACKET_OPEN 5000
#define GUI_SHOW_PROGRESS_BRACKET_CLOSE 5001
#define GUI_SHOW_PROGRESS_FILLED 5002
#define GUI_SHOW_PROGRESS_NOT_FILLED 5003
#define GUI_GUI_HEADER_1 5004
#define GUI_GUI_HEADER_2 5005
#define GUI_GUI_ALU_HEADER 5006
#define GUI_GUI_FPU_HEADER 5007
#define GUI_GUI_MEM_HEADER 5008
#define GUI_GUI_FINISHED_MSG_1 5009
#define GUI_GUI_FINISHED_MSG_2 5010
#define GUI_GUI_LOADING_TEST 5011
typedef struct gui_prm GUIParams;
MsgCode create_gui_params(GUIParams**, ALUParams*, FPUParams*, MEMParams*, int*);
MsgCode del_gui_params(GUIParams**);
void set_gui_params_cores(GUIParams**, int*);
int* get_gui_params_cores(GUIParams**);
void set_gui_params_alu_params(GUIParams**, ALUParams*);
ALUParams* get_gui_params_alu_params(GUIParams**);
void set_gui_params_fpu_params(GUIParams**, FPUParams*);
FPUParams* get_gui_params_fpu_params(GUIParams**);
void set_gui_params_mem_params(GUIParams**, MEMParams*);
MEMParams* get_gui_params_mem_params(GUIParams**);
void print_progress(int, int);
void erase_lines(int);
void* gui(void*);

// Chronometer type
#define CHRONOMETER_MEMORY_ALLOCATION_ERROR 600
typedef struct chronometer Chronometer;
MsgCode create_chronometer(Chronometer**);
MsgCode del_chronometer(Chronometer**);
void start_chronometer(Chronometer**);
float stop_chronometer(Chronometer**);
void set_chronometer_start(Chronometer**, void*);
void* get_chronometer_start(Chronometer**);
void set_chronometer_stop(Chronometer**, void*);
void* get_chronometer_stop(Chronometer**);

// Payload type
#define PAYLOAD_MEMORY_ALLOCATION_ERROR 700
typedef struct pld Payload;
MsgCode add_payload(Payload**, void*, void*);
MsgCode del_payload(Payload**);
void set_payload_function(Payload**, void*);
void* get_payload_function(Payload**);
void set_payload_params(Payload**, void*);
void* get_payload_params(Payload**);
void set_payload_next(Payload**, Payload*);
Payload* get_payload_next(Payload**);
void* run_payload(void*);

// benchmark function
MsgCode test_system(float*, int, bool, float, bool, int, int, int);
void cleanup(Thread**, Thread**, Payload**, ALUParams**, FPUParams**, MEMParams**, GUIParams**,  Chronometer**);

// Strings file
typedef const char* String;
String get_string(MsgCode);