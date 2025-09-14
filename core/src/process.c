/* faeb Core Runtime - Process Scheduling
 * RISC-V Paradigm: Simple, orthogonal, verifiable
 * License: Apache 2.0
 */

#include "faeb/runtime.h"
#include <stdlib.h>
#include <assert.h>

// Process state enumeration
typedef enum {
    FAEB_PROCESS_READY,
    FAEB_PROCESS_RUNNING,
    FAEB_PROCESS_BLOCKED,
    FAEB_PROCESS_TERMINATED
} faeb_process_state_t;

// Process structure
struct faeb_process {
    faeb_process_fn function;
    void* context;
    faeb_process_state_t state;
    struct faeb_process* next;
    int priority;
};

// Global process scheduler state
static struct faeb_process* process_queue = NULL;
static struct faeb_process* current_process = NULL;

// Create new process
faeb_process_t* faeb_process_create(faeb_process_fn function, void* context) {
    if (!function) return NULL;
    
    faeb_process_t* process = malloc(sizeof(faeb_process_t));
    if (!process) return NULL;
    
    process->function = function;
    process->context = context;
    process->state = FAEB_PROCESS_READY;
    process->next = NULL;
    process->priority = 0; // Default priority
    
    // Add to process queue
    if (!process_queue) {
        process_queue = process;
    } else {
        // Insert at head for simplicity (FIFO scheduling)
        process->next = process_queue;
        process_queue = process;
    }
    
    return process;
}

// Destroy process
void faeb_process_destroy(faeb_process_t* process) {
    if (!process) return;
    
    // Remove from queue if present
    if (process == process_queue) {
        process_queue = process->next;
    } else {
        struct faeb_process* current = process_queue;
        while (current && current->next != process) {
            current = current->next;
        }
        if (current) {
            current->next = process->next;
        }
    }
    
    // Mark as terminated
    process->state = FAEB_PROCESS_TERMINATED;
    
    // Free process structure
    free(process);
}

// Yield control to next process
void faeb_process_yield(void) {
    if (!process_queue) return;
    
    // Simple round-robin scheduling
    if (current_process && current_process->state == FAEB_PROCESS_RUNNING) {
        current_process->state = FAEB_PROCESS_READY;
        
        // Move current process to end of queue
        struct faeb_process* current = process_queue;
        while (current->next) {
            current = current->next;
        }
        current->next = current_process;
        current_process->next = NULL;
    }
    
    // Select next process
    if (process_queue) {
        current_process = process_queue;
        process_queue = process_queue->next;
        current_process->next = NULL;
        current_process->state = FAEB_PROCESS_RUNNING;
    }
}

// Run process
void faeb_process_run(faeb_process_t* process) {
    if (!process || process->state != FAEB_PROCESS_READY) return;
    
    // Set as current process
    current_process = process;
    process->state = FAEB_PROCESS_RUNNING;
    
    // Execute process function
    process->function(process->context);
    
    // Mark as ready for next execution
    process->state = FAEB_PROCESS_READY;
    current_process = NULL;
}

// Simple process scheduler
void faeb_scheduler_run(void) {
    while (process_queue) {
        faeb_process_yield();
        
        if (current_process && current_process->state == FAEB_PROCESS_RUNNING) {
            current_process->function(current_process->context);
        }
    }
}
