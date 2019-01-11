#include <pthread.h>

// makes these variables global
extern int alu_matrix_size;
extern int fpu_matrix_size;
extern int mem_matrix_size;

extern int alu_job_size;
extern int fpu_job_size;
extern int mem_job_size;

// Thread type
typedef struct thd Thread;
Thread* add_thread(Thread*, int, void*, void*);
Thread* del_threads(Thread*);
bool wait_threads(Thread*);

// ALUParams type
typedef struct alu_prm ALUParams;
ALUParams* create_alu_params();
ALUParams* del_alu_params(ALUParams*);
void set_alu_params_job_size(ALUParams*, int*);
int* get_alu_params_job_size(ALUParams*);
void set_alu_params_matrix_a(ALUParams*, int**);
int** get_alu_params_matrix_a(ALUParams*);
void set_alu_params_matrix_b(ALUParams*, int**);
int** get_alu_params_matrix_b(ALUParams*);
void set_alu_params_lock(ALUParams*, pthread_mutex_t*);
pthread_mutex_t* get_alu_params_lock(ALUParams*);
void* alu_test(void*);

// FPUParams type
typedef struct fpu_prm FPUParams;
FPUParams* create_fpu_params();
FPUParams* del_fpu_params(FPUParams*);
void set_fpu_params_job_size(FPUParams*, int*);
int* get_fpu_params_job_size(FPUParams*);
void set_fpu_params_matrix_a(FPUParams*, float**);
float** get_fpu_params_matrix_a(FPUParams*);
void set_fpu_params_matrix_b(FPUParams*, float**);
float** get_fpu_params_matrix_b(FPUParams*);
void set_fpu_params_lock(FPUParams*, pthread_mutex_t*);
pthread_mutex_t* get_fpu_params_lock(FPUParams*);
void* fpu_test(void*);

// MEMParams type
typedef struct mem_prm MEMParams;
MEMParams* create_mem_params();
MEMParams* del_mem_params(MEMParams*);
void set_mem_params_job_size(MEMParams*, int*);
int* get_mem_params_job_size(MEMParams*);
void set_mem_params_matrix_a(MEMParams*, int**);
int** get_mem_params_matrix_a(MEMParams*);
void set_mem_params_matrix_b(MEMParams*, int**);
int** get_mem_params_matrix_b(MEMParams*);
void set_mem_params_lock(MEMParams*, pthread_mutex_t*);
pthread_mutex_t* get_mem_params_lock(MEMParams*);
void* mem_test(void*);

//GUIParams type
typedef struct gui_prm GUIParams;
GUIParams* create_gui_params(ALUParams*, FPUParams*, MEMParams*, int*);
GUIParams* del_gui_params(GUIParams*);
void set_gui_params_cores(GUIParams*, int*);
int* get_gui_params_cores(GUIParams*);
void set_gui_params_alu_params(GUIParams*, ALUParams*);
ALUParams* get_gui_params_alu_params(GUIParams*);
void set_gui_params_fpu_params(GUIParams*, FPUParams*);
FPUParams* get_gui_params_fpu_params(GUIParams*);
void set_gui_params_mem_params(GUIParams*, MEMParams*);
MEMParams* get_gui_params_mem_params(GUIParams*);
void* gui(void*);
//void erase_lines(int);

// Chronometer type
typedef struct chronometer Chronometer;
Chronometer* create_chronometer();
void start_chronometer(Chronometer*);
float stop_chronometer(Chronometer*);
Chronometer* del_chronometer(Chronometer*);

// benchmark function
float test_system(int);