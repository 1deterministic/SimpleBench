#include "simplebench.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// if I ever change this it means that scores from different versions are not comparable
#define BENCHMARK_VERSION "1.0"

// This configuration will use about 515MB of RAM
// size of the matrices being used
int alu_matrix_size = 256; // 256KB
int fpu_matrix_size = 256; // 256KB
int mem_matrix_size = 8192; // 256MB
// size of the tasks put on alu and fpu units
int alu_job_size = 64 * alu_matrix_size; // 64 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 109,051,904 integer ops
int fpu_job_size = 32 * fpu_matrix_size; // 32 times * 256 rows * 256 columns * (8 sums + 8 subtractions + 8 multiplies + 2 division) = 54,525,952 floating point ops
int mem_job_size = 32 * mem_matrix_size; // 32 times * 8192 rows * 8192 columns * 4 bytes per element = 8GB of data, 8192 rows * 4 bytes = 32KB each time

// For ultra low spec machines
// This configuration will use about 35MB of RAM
//int mem_matrix_size = 2048; // 16MB
//int mem_job_size = 512 * mem_matrix_size; // 512 times * 2048 rows * 2048 columns * 4 bytes per element = 8GB of data, 2048 rows * 4 bytes = 8KB each time

// shows the system score
void show_score(float singlethread_score, float multithread_score, int number_of_threads) {
    if (singlethread_score > 0.0 && multithread_score > 0.0) {
        printf("System Test results: simplebench version %s\n\n", BENCHMARK_VERSION);
        printf("Singlethread score: %.2f\n", singlethread_score);
        printf("Multithread score (%d threads): %.2f\n", number_of_threads, multithread_score);
        printf("Multiplier: %.2fx\n", (singlethread_score > 0.0) ? multithread_score / singlethread_score : 0.0);
    } else {
        printf("An error occurred during the test!\n");
    }
}

int main(int argc, char** argv) {
    // gets the number of processors (threads actually) available to use
    int number_of_threads = sysconf(_SC_NPROCESSORS_ONLN);
    float singlethread_score = 0.0;
    float multithread_score = 0.0;
    
    // gets the scores fot the single and multithreaded tests
    singlethread_score = test_system(1);
    
    // skip the multithread test if the system only has 1
    if (number_of_threads > 1)
        multithread_score = test_system(number_of_threads);
    else
        multithread_score = singlethread_score;

    show_score(singlethread_score, multithread_score, number_of_threads);

    return 0;
}