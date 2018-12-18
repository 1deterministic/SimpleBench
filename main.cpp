#include "simplebench.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// size of the matrices being used
int alu_matrix_size = 256; // 256KB
int fpu_matrix_size = 256; // 256KB
int mem_matrix_size = 8192; // 256MB

// size of the tasks put on alu and fpu units
int alu_job_size = 64 * alu_matrix_size; // 64 times * 256 rows * 256 columns * (4 sums + 4 subtractions + 4 multiplies + 1 division) = 54,525,952 integer ops
int fpu_job_size = 32 * fpu_matrix_size; // 32 times * 256 rows * 256 columns * (4 sums + 4 subtractions + 4 multiplies + 1 division) = 27,262,976 float ops
int mem_job_size = 32 * mem_matrix_size; // 32 times * 8192 rows * 8192 columns * 4 bytes per element = 8GB of data, 8192 rows * 4 bytes = 32KB each time

int main(int argc, char** argv) {
    // gets the number of processors (threads actually) available to use
    int number_of_threads = sysconf(_SC_NPROCESSORS_ONLN);
    
    // gets the scores fot the single and multithreaded tests
    system("clear && printf '\e[3J'");
    printf("Allocating resources...\n");
    float singlethread_score = test_system(1);
    
    system("clear && printf '\e[3J'");
    printf("Allocating resources...\n");
    float multithread_score = test_system(number_of_threads);
        
    // shows the system score
    system("clear && printf '\e[3J'");
    printf("System Test results:\n\n");
    printf("Singlethread score: %.2f\n", singlethread_score);
    printf("Multithread score (%d threads): %.2f\n", number_of_threads, multithread_score);
    printf("Multiplier: %.2f\n", (singlethread_score > 0.0) ? multithread_score / singlethread_score : 0.0);
    printf("\n");
    printf("Press enter to continue\n");
    getchar();
    
    system("clear && printf '\e[3J'");
    return 0;
}