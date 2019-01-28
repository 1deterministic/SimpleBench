#include "simplebench.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// normalizes the score
float handicap;
// size of the matrices being used
int alu_matrix_size;
int fpu_matrix_size;
int mem_matrix_size;
// size of the tasks
int alu_job_size;
int fpu_job_size;
int mem_job_size;

int main(int argc, char** argv) {
    float singlethread_score = 0.0;
    float multithread_score = 0.0;

    int threads = sysconf(_SC_NPROCESSORS_ONLN);
    MsgCode config = CONFIG_MODERN_HARDWARE;
    bool show_gui = true;
    bool st_test = true;
    bool mt_test = true;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], CLI_SHOW_GUI) == 0) {
            if (i + 1 < argc) {
                i++;
                if (strcmp(argv[i], CLI_ON) == 0) {
                    show_gui = true;
                }
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                    show_gui = false;
                }
                else {
                    printf("Invalid on/off value: %s\n", argv[i]); 
                    return 1;
                }
                continue;
            } else {
                printf("Missing on/off value!\n");
                return 1;
            }
        }

        if (strcmp(argv[i], CLI_ST_TEST) == 0) {
            if (i + 1 < argc) {
                i++;
                if (strcmp(argv[i], CLI_ON) == 0) {
                    st_test = true;
                }
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                    st_test = false;
                }
                else {
                    printf("Invalid on/off value: %s\n", argv[i]); 
                    return 1;
                }
                continue;
            } else {
                printf("Missing on/off value!\n");
                return 1;
            }
        }

        if (strcmp(argv[i], CLI_MT_TEST) == 0) {
            if (i + 1 < argc) {
                i++;
                if (strcmp(argv[i], CLI_ON) == 0) {
                    mt_test = true;
                }
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                    mt_test = false;
                }
                else {
                    printf("Invalid on/off value: %s\n", argv[i]); 
                    return 1;
                }
                continue;
            } else {
                printf("Missing on/off value!\n");
                return 1;
            }
        }

        else if (strcmp(argv[i], CLI_THREADS) == 0) {
            if (i + 1 < argc) {
                i++;
                threads = atoi(argv[i]);
                if (threads <= 0) {
                    printf("Invalid number!\n");
                    return 1;
                }    
                continue;
            } else {
                printf("Missing thread count!\n");
                return 1;
            }
        }

        if (strcmp(argv[i], CLI_OLD_HARDWARE) == 0) {
            if (i + 1 < argc) {
                i++;
                if (strcmp(argv[i], CLI_ON) == 0) {
                    config = CONFIG_OLD_HARDWARE;
                }
                else if (strcmp(argv[i], CLI_OFF) == 0) {
                   config = CONFIG_MODERN_HARDWARE;
                }
                else {
                    printf("Invalid on/off value: %s\n", argv[i]); 
                    return 1;
                }
                continue;
            } else {
                printf("Missing on/off value!\n");
                return 1;
            }
        }

        else if (strcmp(argv[i], CLI_HELP) == 0) {
            show_help();
            return 0;
        }

        else {
            printf("Option not recognized: %s\nTry the option \"%s\" to show the help\n", argv[i], CLI_HELP);
            return 1;
        }
    }

    load_test_config(config);
    
    // gets the scores fot the single and multithreaded tests
    if (st_test)
        test_system(&singlethread_score, 1, handicap, show_gui);
    
    
    if (mt_test) {
        // skip the multithread test if the system only has 1
        if (threads > 1) {
            test_system(&multithread_score, threads, handicap, show_gui);
        } else {
            multithread_score = singlethread_score;
        }
    }

    show_score(singlethread_score, multithread_score, threads);
    return 0;
}

// shows the system score
void show_score(float singlethread_score, float multithread_score, int threads) {
    printf("%-20s: %10s\n", get_string(MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION), BENCHMARK_VERSION);
    printf("%-20s: %10.2f\n", get_string(MSG_MAIN_SHOW_SCORE_SINGLETHREAD_SCORE), singlethread_score);
    printf("%-20s: %10.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTITHREAD_SCORE), multithread_score);
    printf("%-20s: %10.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTIPLIER), (singlethread_score > 0.0) ? multithread_score / singlethread_score : 0.0);
}

// loads the test config
void load_test_config(MsgCode config) {
    switch (config) {
        // This configuration will use about 515MB of RAM
        case CONFIG_MODERN_HARDWARE: {
            handicap = 1.0;
            alu_matrix_size = 256; // 256KB
            fpu_matrix_size = 256; // 256KB
            mem_matrix_size = 8192; // 256MB
            alu_job_size = 64 * alu_matrix_size; // 64 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 109,051,904 integer ops
            fpu_job_size = 32 * fpu_matrix_size; // 32 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 54,525,952 floating point ops
            mem_job_size = 32 * mem_matrix_size; // 32 times * 8192 rows * 8192 columns * 4 bytes per element = 8GB of data, 8192 rows * 4 bytes = 32KB each time
            break;
        }

        // This configuration will use about 35MB of RAM
        case CONFIG_OLD_HARDWARE: {
            handicap = 0.0625;
            alu_matrix_size = 256; // 256KB
            fpu_matrix_size = 256; // 256KB
            mem_matrix_size = 2048; // 16MB
            alu_job_size = 4 * alu_matrix_size; // 4 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 6,815,744 integer ops = handicap * modern hardware integer ops
            fpu_job_size = 2 * fpu_matrix_size; // 2 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 3,407,872 floating point ops = handicap * modern hardware floating point ops
            mem_job_size = 32 * mem_matrix_size; // 32 times * 2048 rows * 2048 columns * 4 bytes per element = 512MB of data, 2048 rows * 4 bytes = 8KB each time = handicap * modern hardware amount of data
            break;
        }            
    };
}

void show_help(void) {
    printf("Help should be here!\n");
}