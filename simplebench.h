#include <pthread.h>

// makes these variables global
extern int alu_matrix_size;
extern int fpu_matrix_size;
extern int mem_matrix_size;

extern int alu_job_size;
extern int fpu_job_size;
extern int mem_job_size;

#define SUCCESS 0

// Thread type
#define THREAD_MEMORY_ALLOCATION_ERROR 100
#define THREAD_PTHREAD_CREATION_ERROR 101
#define THREAD_PTHREAD_AFFINITY_ERROR 102
#define THREAD_PTHREAD_JOIN_ERROR 103
typedef struct thd Thread;
int add_thread(Thread**, int, void*, void*);
int del_threads(Thread**);
int wait_threads(Thread**);

// ALUParams type
#define ALU_MEMORY_ALLOCATION_ERROR 200
#define ALU_PTHREAD_LOCK_CREATION_ERROR 201
#define ALU_PTHREAD_LOCK_INIT_ERROR 202
typedef struct alu_prm ALUParams;
int create_alu_params(ALUParams**);
int del_alu_params(ALUParams**);
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
int create_fpu_params(FPUParams**);
int del_fpu_params(FPUParams**);
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
int create_mem_params(MEMParams**);
int del_mem_params(MEMParams**);
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
typedef struct gui_prm GUIParams;
int create_gui_params(GUIParams**, ALUParams*, FPUParams*, MEMParams*, int*);
int del_gui_params(GUIParams**);
void set_gui_params_cores(GUIParams**, int*);
int* get_gui_params_cores(GUIParams**);
void set_gui_params_alu_params(GUIParams**, ALUParams*);
ALUParams* get_gui_params_alu_params(GUIParams**);
void set_gui_params_fpu_params(GUIParams**, FPUParams*);
FPUParams* get_gui_params_fpu_params(GUIParams**);
void set_gui_params_mem_params(GUIParams**, MEMParams*);
MEMParams* get_gui_params_mem_params(GUIParams**);
void* gui(void*);

// Chronometer type
#define CHRONOMETER_MEMORY_ALLOCATION_ERROR 600
typedef struct chronometer Chronometer;
int create_chronometer(Chronometer**);
void start_chronometer(Chronometer**);
float stop_chronometer(Chronometer**);
int del_chronometer(Chronometer**);

// benchmark function
float test_system(int);