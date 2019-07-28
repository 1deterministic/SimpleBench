#include "simplebench.h"
#include <stdlib.h>
#include <stdbool.h>

#if __linux__ || __APPLE__
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

    #if __linux__ || __APPLE__
        struct timespec* start = (struct timespec*) malloc(sizeof(struct timespec));
        if (start == NULL)
            return 1;

        struct timespec* stop = (struct timespec*) malloc(sizeof(struct timespec));
        if (stop == NULL)
            return 1;

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        DWORD* start = (DWORD*) malloc(sizeof(DWORD));
        if (start == NULL)
            return 1;

        DWORD* stop = (DWORD*) malloc(sizeof(DWORD));
        if (stop == NULL)
            return 1;

    #endif

    set_chronometer_start(chronometer, start);
    set_chronometer_stop(chronometer, stop);
    
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
    #if __linux__ || __APPLE__
        struct timespec* start = get_chronometer_start(chronometer);
        clock_gettime(CLOCK_MONOTONIC_RAW, start);

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        DWORD* start = get_chronometer_start(chronometer);
        *start = GetTickCount();

    #endif
}

// stops a Chronometer
float stop_chronometer(Chronometer** chronometer) {
    float time_delta;

    #if __linux__ || __APPLE__
        struct timespec* start = get_chronometer_start(chronometer);
        struct timespec* stop = get_chronometer_stop(chronometer);

        clock_gettime(CLOCK_MONOTONIC_RAW, stop);

        time_delta = (float) ((stop->tv_sec - start->tv_sec) * 1000000 + (stop->tv_nsec - start->tv_nsec) / 1000) / 1000000.0;

    #elif __MINGW64__ || __MINGW32__ || _WIN32
        DWORD* start = get_chronometer_start(chronometer);
        DWORD* stop = get_chronometer_stop(chronometer);

        *stop = GetTickCount();
        time_delta = (float) (*stop - *start) / 1000.0;

    #endif

    return time_delta;
}

void set_chronometer_start(Chronometer** chronometer, void* start) {
    (*chronometer)->start = start;
}

void* get_chronometer_start(Chronometer** chronometer) {
    return (*chronometer)->start;
}

void set_chronometer_stop(Chronometer** chronometer, void* stop) {
    (*chronometer)->stop = stop;
}

void* get_chronometer_stop(Chronometer** chronometer) {
    return (*chronometer)->stop;
}