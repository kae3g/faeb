/* faeb Core Runtime - Process Scheduler
 * RISC-V Paradigm: Simple, orthogonal, verifiable
 * License: Apache 2.0
 */

#include "faeb/runtime.h"
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// Scheduler state
static struct {
    bool initialized;
    struct faeb_process* ready_queue;
    struct faeb_process* blocked_queue;
    struct faeb_process* current;
    int time_slice;
    int current_time;
} scheduler_state = {
    .initialized = false,
    .ready_queue = NULL,
    .blocked_queue = NULL,
    .current = NULL,
    .time_slice = 100, // 100ms time slice
    .current_time = 0
};

// Initialize scheduler
faeb_result_t faeb_scheduler_init(int time_slice_ms) {
    if (scheduler_state.initialized) {
        return FAEB_SUCCESS;
    }
    
    scheduler_state.time_slice = time_slice_ms;
    scheduler_state.current_time = 0;
    scheduler_state.ready_queue = NULL;
    scheduler_state.blocked_queue = NULL;
    scheduler_state.current = NULL;
    scheduler_state.initialized = true;
    
    return FAEB_SUCCESS;
}

// Add process to ready queue
faeb_result_t faeb_scheduler_add_process(faeb_process_t* process) {
    if (!scheduler_state.initialized || !process) {
        return FAEB_ERROR_INVALID;
    }
    
    // Add to head of ready queue
    process->next = scheduler_state.ready_queue;
    scheduler_state.ready_queue = process;
    
    return FAEB_SUCCESS;
}

// Remove process from scheduler
faeb_result_t faeb_scheduler_remove_process(faeb_process_t* process) {
    if (!scheduler_state.initialized || !process) {
        return FAEB_ERROR_INVALID;
    }
    
    // Remove from ready queue
    if (scheduler_state.ready_queue == process) {
        scheduler_state.ready_queue = process->next;
        process->next = NULL;
        return FAEB_SUCCESS;
    }
    
    struct faeb_process* current = scheduler_state.ready_queue;
    while (current && current->next != process) {
        current = current->next;
    }
    
    if (current) {
        current->next = process->next;
        process->next = NULL;
        return FAEB_SUCCESS;
    }
    
    // Remove from blocked queue
    if (scheduler_state.blocked_queue == process) {
        scheduler_state.blocked_queue = process->next;
        process->next = NULL;
        return FAEB_SUCCESS;
    }
    
    current = scheduler_state.blocked_queue;
    while (current && current->next != process) {
        current = current->next;
    }
    
    if (current) {
        current->next = process->next;
        process->next = NULL;
        return FAEB_SUCCESS;
    }
    
    return FAEB_ERROR_INVALID;
}

// Schedule next process
faeb_process_t* faeb_scheduler_schedule_next(void) {
    if (!scheduler_state.initialized) {
        return NULL;
    }
    
    // If current process exists, add it back to ready queue
    if (scheduler_state.current) {
        scheduler_state.current->next = scheduler_state.ready_queue;
        scheduler_state.ready_queue = scheduler_state.current;
        scheduler_state.current = NULL;
    }
    
    // Select next process from ready queue
    if (scheduler_state.ready_queue) {
        scheduler_state.current = scheduler_state.ready_queue;
        scheduler_state.ready_queue = scheduler_state.ready_queue->next;
        scheduler_state.current->next = NULL;
        
        return scheduler_state.current;
    }
    
    return NULL;
}

// Get current process
faeb_process_t* faeb_scheduler_get_current(void) {
    return scheduler_state.current;
}

// Block current process
faeb_result_t faeb_scheduler_block_current(void) {
    if (!scheduler_state.initialized || !scheduler_state.current) {
        return FAEB_ERROR_INVALID;
    }
    
    // Move current process to blocked queue
    scheduler_state.current->next = scheduler_state.blocked_queue;
    scheduler_state.blocked_queue = scheduler_state.current;
    scheduler_state.current = NULL;
    
    return FAEB_SUCCESS;
}

// Unblock process
faeb_result_t faeb_scheduler_unblock_process(faeb_process_t* process) {
    if (!scheduler_state.initialized || !process) {
        return FAEB_ERROR_INVALID;
    }
    
    // Remove from blocked queue
    if (scheduler_state.blocked_queue == process) {
        scheduler_state.blocked_queue = process->next;
        process->next = NULL;
        
        // Add to ready queue
        process->next = scheduler_state.ready_queue;
        scheduler_state.ready_queue = process;
        
        return FAEB_SUCCESS;
    }
    
    struct faeb_process* current = scheduler_state.blocked_queue;
    while (current && current->next != process) {
        current = current->next;
    }
    
    if (current) {
        current->next = process->next;
        process->next = NULL;
        
        // Add to ready queue
        process->next = scheduler_state.ready_queue;
        scheduler_state.ready_queue = process;
        
        return FAEB_SUCCESS;
    }
    
    return FAEB_ERROR_INVALID;
}

// Check if time slice expired
bool faeb_scheduler_time_slice_expired(void) {
    if (!scheduler_state.initialized) {
        return false;
    }
    
    // Simple time slice check
    static clock_t last_switch = 0;
    clock_t current_time = clock();
    
    if (current_time - last_switch >= scheduler_state.time_slice) {
        last_switch = current_time;
        return true;
    }
    
    return false;
}

// Run scheduler for one tick
faeb_result_t faeb_scheduler_tick(void) {
    if (!scheduler_state.initialized) {
        return FAEB_ERROR_INVALID;
    }
    
    scheduler_state.current_time++;
    
    // Check for time slice expiration
    if (faeb_scheduler_time_slice_expired()) {
        faeb_scheduler_schedule_next();
    }
    
    // If no current process, schedule one
    if (!scheduler_state.current) {
        faeb_scheduler_schedule_next();
    }
    
    return FAEB_SUCCESS;
}

// Get scheduler statistics
struct faeb_scheduler_stats {
    int ready_count;
    int blocked_count;
    int total_processes;
    int time_slice_ms;
};

struct faeb_scheduler_stats faeb_scheduler_get_stats(void) {
    struct faeb_scheduler_stats stats = {0};
    
    if (!scheduler_state.initialized) {
        return stats;
    }
    
    stats.time_slice_ms = scheduler_state.time_slice;
    
    // Count ready processes
    struct faeb_process* current = scheduler_state.ready_queue;
    while (current) {
        stats.ready_count++;
        current = current->next;
    }
    
    // Count blocked processes
    current = scheduler_state.blocked_queue;
    while (current) {
        stats.blocked_count++;
        current = current->next;
    }
    
    stats.total_processes = stats.ready_count + stats.blocked_count + 
                           (scheduler_state.current ? 1 : 0);
    
    return stats;
}
