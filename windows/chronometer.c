#include "simplebench.h"
#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>

// struct of the Chronometer type
struct chronometer {
    DWORD start;
    DWORD stop;
};

// creates a Chronometer
MsgCode create_chronometer(Chronometer** chronometer) {
    *chronometer = (Chronometer*) malloc(sizeof(Chronometer));
    // returns right away if the allocation failed
    if (*chronometer == NULL)
        return CHRONOMETER_MEMORY_ALLOCATION_ERROR;
    
    return SUCCESS;
}

// deletes a Chronometer
MsgCode del_chronometer(Chronometer** chronometer) {
    free(*chronometer);
    
    return SUCCESS;
}

// starts a Chronometer
void start_chronometer(Chronometer** chronometer) {
    (*chronometer)->start = GetTickCount();
}

// stops a Chronometer
float stop_chronometer(Chronometer** chronometer) {
    (*chronometer)->stop = GetTickCount();

    return (float) ((*chronometer)->stop - (*chronometer)->start) / 1000.0;
}