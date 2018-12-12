// makes these variables global
extern int alu_matrix_size;
extern int fpu_matrix_size;
extern int mem_matrix_size;

extern int alu_job_size;
extern int fpu_job_size;
extern int mem_job_size;

// abstract types
typedef struct alu_prm ALUParams;
typedef struct fpu_prm FPUParams;
typedef struct mem_prm MEMParams;
typedef struct gui_prm GUIParams;
typedef struct thd Thread;
typedef struct chronometer Chronometer;

// thread functions
void* alu_test(void*);
void* fpu_test(void*);
void* mem_test(void*);
void* gui(void*);

// Thread type
Thread* add_thread(Thread*, int, void*, void*);
Thread* del_threads(Thread*);
bool wait_threads(Thread*);

// ALUParams type
ALUParams* create_alu_params();
ALUParams* del_alu_params(ALUParams*);

// FPUParams type
FPUParams* create_fpu_params();
FPUParams* del_fpu_params(FPUParams*);

// MEMParams type
MEMParams* create_mem_params();
MEMParams* del_mem_params(MEMParams*);

//GUIParams type
GUIParams* create_gui_params(ALUParams*, FPUParams*, MEMParams*, int*);
GUIParams* del_gui_params(ALUParams*);

// Chronometer type
Chronometer* create_chronometer();
void start_chronometer(Chronometer*);
float stop_chronometer(Chronometer*);
Chronometer* del_chronometer(Chronometer*);

// benchmark function
float test_system(int);
