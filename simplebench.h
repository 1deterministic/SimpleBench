#include <pthread.h>

// if I ever change this it means that scores from different versions are not comparable
#define BENCHMARK_VERSION "1.0"

#define SCORE_CALIBRATION_FACTOR 72.888054

#define CLI_ON "on"
#define CLI_OFF "off"

#define CLI_SHOW_GUI "--show-gui"
#define CLI_ST_TEST "--st-test"
#define CLI_MT_TEST "--mt-test"
#define CLI_THREADS "--threads"
#define CLI_OLD_HARDWARE "--old-hardware"
#define CLI_HELP "--help"
#define CLI_VERSION "--version"

#define CONFIG_MODERN_HARDWARE 9990
#define CONFIG_OLD_HARDWARE 9991

// makes these variables global
extern int alu_matrix_size;
extern int fpu_matrix_size;
extern int mem_matrix_size;

extern int alu_job_size;
extern int fpu_job_size;
extern int mem_job_size;

typedef unsigned int MsgCode;

#define SUCCESS 0

// main file
#define MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION 1000
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
#define MSG_HELP_TEXT 1011
MsgCode get_cli_options(int, char**, bool*, bool*, bool*, bool*, int*);
void show_score(float, float, int);
void load_test_config(MsgCode);

// Thread type
#define THREAD_MEMORY_ALLOCATION_ERROR 100
#define THREAD_PTHREAD_CREATION_ERROR 101
#define THREAD_PTHREAD_AFFINITY_ERROR 102
#define THREAD_PTHREAD_JOIN_ERROR 103
typedef struct thd Thread;
MsgCode add_thread(Thread**, int, void*, void*);
MsgCode del_threads(Thread**);
MsgCode wait_threads(Thread**);
MsgCode stop_threads(Thread**);

// ALUParams type
#define ALU_MEMORY_ALLOCATION_ERROR 200
#define ALU_PTHREAD_LOCK_CREATION_ERROR 201
#define ALU_PTHREAD_LOCK_INIT_ERROR 202
typedef struct alu_prm ALUParams;
MsgCode create_alu_params(ALUParams**);
MsgCode del_alu_params(ALUParams**);
void set_alu_params_job_size(ALUParams**, int*);
int* get_alu_params_job_size(ALUParams**);
void set_alu_params_matrix_a(ALUParams**, int**);
int** get_alu_params_matrix_a(ALUParams**);
void set_alu_params_matrix_b(ALUParams**, int**);
int** get_alu_params_matrix_b(ALUParams**);
void set_alu_params_lock(ALUParams**, pthread_mutex_t*);
pthread_mutex_t* get_alu_params_lock(ALUParams**);
void* alu_test(void*);

// FPUParams type
#define FPU_MEMORY_ALLOCATION_ERROR 300
#define FPU_PTHREAD_LOCK_CREATION_ERROR 301
#define FPU_PTHREAD_LOCK_INIT_ERROR 302
typedef struct fpu_prm FPUParams;
MsgCode create_fpu_params(FPUParams**);
MsgCode del_fpu_params(FPUParams**);
void set_fpu_params_job_size(FPUParams**, int*);
int* get_fpu_params_job_size(FPUParams**);
void set_fpu_params_matrix_a(FPUParams**, float**);
float** get_fpu_params_matrix_a(FPUParams**);
void set_fpu_params_matrix_b(FPUParams**, float**);
float** get_fpu_params_matrix_b(FPUParams**);
void set_fpu_params_lock(FPUParams**, pthread_mutex_t*);
pthread_mutex_t* get_fpu_params_lock(FPUParams**);
void* fpu_test(void*);

// MEMParams type
#define MEM_MEMORY_ALLOCATION_ERROR 400
#define MEM_PTHREAD_LOCK_CREATION_ERROR 401
#define MEM_PTHREAD_LOCK_INIT_ERROR 402
typedef struct mem_prm MEMParams;
MsgCode create_mem_params(MEMParams**);
MsgCode del_mem_params(MEMParams**);
void set_mem_params_job_size(MEMParams**, int*);
int* get_mem_params_job_size(MEMParams**);
void set_mem_params_matrix_a(MEMParams**, int**);
int** get_mem_params_matrix_a(MEMParams**);
void set_mem_params_matrix_b(MEMParams**, int**);
int** get_mem_params_matrix_b(MEMParams**);
void set_mem_params_lock(MEMParams**, pthread_mutex_t*);
pthread_mutex_t* get_mem_params_lock(MEMParams**);
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
void print_progress(float, float);
void erase_lines(int);
void* gui(void*);

// Chronometer type
#define CHRONOMETER_MEMORY_ALLOCATION_ERROR 600
typedef struct chronometer Chronometer;
MsgCode create_chronometer(Chronometer**);
MsgCode del_chronometer(Chronometer**);
void start_chronometer(Chronometer**);
float stop_chronometer(Chronometer**);

// benchmark function
MsgCode test_system(float*, int, float, bool);
void cleanup(Thread**, Thread**, ALUParams**, FPUParams**, MEMParams**, GUIParams**,  Chronometer**);

// Strings file
typedef const char* String;
String get_string(MsgCode);