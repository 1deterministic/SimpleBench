#include "simplebench.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// normalizes the score
float handicap;
// size of the matrices being used
int alu_matrix_size;
int fpu_matrix_size;
int mem_matrix_size;
// size of the tasks put on alu and fpu units
int alu_job_size;
int fpu_job_size;
int mem_job_size;

int main(int argc, char** argv) {
    float singlethread_score = 0.0;
    float multithread_score = 0.0;
    int number_of_threads = 1;

    number_of_threads = sysconf(_SC_NPROCESSORS_ONLN);

    load_test_config(CONFIG_MODERN_HARDWARE);
    // load_test_config(CONFIG_OLD_HARDWARE);
    
    // gets the scores fot the single and multithreaded tests
    test_system(&singlethread_score, 1, handicap);
    
    // skip the multithread test if the system only has 1
    if (number_of_threads > 1) {
        test_system(&multithread_score, number_of_threads, handicap);
    } else {
        multithread_score = singlethread_score;
    }

    show_score(singlethread_score, multithread_score, number_of_threads);
    return 0;
}

// shows the system score
void show_score(float singlethread_score, float multithread_score, int number_of_threads) {
    if (singlethread_score > 0.0 && multithread_score > 0.0) {
        printf("%-20s: %10s\n", get_string(MSG_MAIN_SHOW_SCORE_SIMPLEBENCH_VERSION), BENCHMARK_VERSION);
        printf("%-20s: %10.2f\n", get_string(MSG_MAIN_SHOW_SCORE_SINGLETHREAD_SCORE), singlethread_score);
        printf("%-20s: %10.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTITHREAD_SCORE), multithread_score);
        printf("%-20s: %10.2f\n", get_string(MSG_MAIN_SHOW_SCORE_MULTIPLIER), (singlethread_score > 0.0) ? multithread_score / singlethread_score : 0.0);
    } else {
        printf("%s\n", get_string(MSG_MAIN_SHOW_SCORE_ERROR));
    }
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