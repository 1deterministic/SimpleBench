// Chronometer type ====================================================

#include "simplebench.h"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// struct of the Chronometer type
struct chronometer {
    struct timespec start;
    struct timespec stop;
};

// creates a Chronometer
int create_chronometer(Chronometer** chronometer) {
    *chronometer = (Chronometer*) malloc(sizeof(Chronometer));
    // returns right away if the allocation failed
    if (*chronometer == NULL)
        return 1;
    
    return 0;
}

// starts a Chronometer
void start_chronometer(Chronometer** chronometer) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &((*chronometer)->start));
}

// stops a Chronometer
float stop_chronometer(Chronometer** chronometer) {
    clock_gettime(CLOCK_MONOTONIC_RAW, &((*chronometer)->stop));
    
    float delta_us = ((*chronometer)->stop.tv_sec - (*chronometer)->start.tv_sec) * 1000000 + ((*chronometer)->stop.tv_nsec - (*chronometer)->start.tv_nsec) / 1000;
    
    return (float) delta_us / 1000000.0;
}

// deletes a Chronometer
int del_chronometer(Chronometer** chronometer) {
    free(*chronometer);
    
    return 0;
}