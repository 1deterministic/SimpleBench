#include "simplebench.h"
#include <stdlib.h>
#include <stdbool.h>

// struct of the Payload type
struct pld {
    void* function;
    void* params;
    Payload* next;
};

// this will add a payload to a payload array
MsgCode add_payload(Payload** payload_array, void* function, void* params) {
    // manually allocate memory the size of Thread
    Payload* new_payload = (Payload*) malloc(sizeof(Payload));
    if (new_payload == NULL)
        return PAYLOAD_MEMORY_ALLOCATION_ERROR;
    
    set_payload_function(&new_payload, function);
    set_payload_params(&new_payload, params);
    set_payload_next(&new_payload, *payload_array);
    *payload_array = new_payload;
    return SUCCESS;
}

// frees up all payloads in a payload array
MsgCode del_payload(Payload** payload_array) {
    // auxiliary pointer
    Payload* payload_element = *payload_array;
    // will repeat until all payloads were removed
    while (*payload_array != NULL) {
        // stores the current payload on a separate variable and advances the payload array to the next element
        payload_element = *payload_array;
        *payload_array = get_payload_next(&payload_element);

        // frees up the current payload
        free(payload_element);
    }
    
    // returns the new empty array
    return SUCCESS;
}

void set_payload_function(Payload** payload_element, void* function) {
    (*payload_element)->function = function;
}

void* get_payload_function(Payload** payload_element) {
    return (*payload_element)->function;
}

void set_payload_params(Payload** payload_element, void* params) {
    (*payload_element)->params = params;
}

void* get_payload_params(Payload** payload_element) {
    return (*payload_element)->params;
}

void set_payload_next(Payload** payload_element, Payload* next) {
    (*payload_element)->next = next;
}

Payload* get_payload_next(Payload** payload_element) {
    return (*payload_element)->next;
}

void* run_payload(void* params) {
    Payload** payload = (Payload**) &params;
    Payload* current = *payload;

    void* (*function)(void*);
    void* parameters;

    while (current != NULL) {
        function = get_payload_function(&current);
        parameters = get_payload_params(&current);

        function(parameters);

        current = get_payload_next(&current);
    }
}