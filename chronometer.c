#include "simplebench.h"
#include <stdlib.h>
#include <stdbool.h>

#ifdef __linux__
    #include <pthread.h>

#elif __MINGW64__ || __MINGW32__ || _WIN32
    #include <windows.h>

#endif

// struct of the Chronometer type
struct chronometer {
    void* start;
    void* stop;
};

//struct timespec

// creates a Chronometer
MsgCode create_chronometer(Chronometer** chronometer) {
    *chronometer = (Chronometer*) malloc(sizeof(Chronometer));
    // returns right away if the allocation failed
    if (*chronometer == NULL)
        return CHRONOMETER_MEMORY_ALLOCATION_ERROR;

    #ifdef __linux__
        (*chronometer)->start = (void*) ((struct timespec*) malloc(sizeof(struct timespec)));
        if ((*chronometer)->start == NULL)
            return 1;

        (*chronometer)->stop = (void*) ((struct timespec*) malloc(sizeof(struct timespec)));
        if ((*chronometer)->stop == NULL)
            return 1;

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        (*chronometer)->start = (void*) ((DWORD*) malloc(sizeof(DWORD)));
        if ((*chronometer)->start == NULL)
            return 1;

        (*chronometer)->stop = (void*) ((DWORD*) malloc(sizeof(DWORD)));
        if ((*chronometer)->stop == NULL)
            return 1;

    #endif
    
    return SUCCESS;
}

// deletes a Chronometer
MsgCode del_chronometer(Chronometer** chronometer) {
    free((*chronometer)->start);
    free((*chronometer)->stop);
    free(*chronometer);
    
    return SUCCESS;
}

// starts a Chronometer
void start_chronometer(Chronometer** chronometer) {
    #ifdef __linux__
        clock_gettime(CLOCK_MONOTONIC_RAW, ((struct timespec*) (*chronometer)->start));

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        *((DWORD*) (*chronometer)->start) = GetTickCount();

    #endif
}

// stops a Chronometer
float stop_chronometer(Chronometer** chronometer) {
    float time_delta;
    #ifdef __linux__
        clock_gettime(CLOCK_MONOTONIC_RAW, ((struct timespec*) (*chronometer)->stop));
        float delta_us = (((struct timespec*) (*chronometer)->stop)->tv_sec - ((struct timespec*) (*chronometer)->start)->tv_sec) * 1000000 + (((struct timespec*) (*chronometer)->stop)->tv_nsec - ((struct timespec*) (*chronometer)->start)->tv_nsec) / 1000;
        time_delta = (float) delta_us / 1000000.0;

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        *((DWORD*) (*chronometer)->stop) = GetTickCount();
        time_delta = (float) (*((DWORD*) (*chronometer)->stop) - *((DWORD*) (*chronometer)->start)) / 1000.0;

    #endif

    return time_delta;
}