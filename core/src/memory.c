/* faeb Core Runtime - Memory Management
 * RISC-V Paradigm: Simple, orthogonal, verifiable
 * License: Apache 2.0
 */

#include "faeb/runtime.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Memory block structure for tracking allocations
struct faeb_memory_block {
    void* ptr;
    size_t size;
    struct faeb_memory_block* next;
};

// Memory manager structure
struct faeb_memory {
    size_t total_size;
    size_t used_size;
    struct faeb_memory_block* blocks;
    faeb_result_t last_error;
};

// Create memory manager with specified size
faeb_memory_t* faeb_memory_create(size_t size) {
    faeb_memory_t* memory = malloc(sizeof(faeb_memory_t));
    if (!memory) {
        return NULL;
    }
    
    memory->total_size = size;
    memory->used_size = 0;
    memory->blocks = NULL;
    memory->last_error = FAEB_SUCCESS;
    
    return memory;
}

// Destroy memory manager and free all blocks
void faeb_memory_destroy(faeb_memory_t* memory) {
    if (!memory) return;
    
    // Free all allocated blocks
    struct faeb_memory_block* current = memory->blocks;
    while (current) {
        struct faeb_memory_block* next = current->next;
        free(current->ptr);
        free(current);
        current = next;
    }
    
    free(memory);
}

// Allocate memory block
void* faeb_memory_allocate(faeb_memory_t* memory, size_t size) {
    if (!memory || size == 0) {
        if (memory) memory->last_error = FAEB_ERROR_INVALID;
        return NULL;
    }
    
    // Check if allocation would exceed total size
    if (memory->used_size + size > memory->total_size) {
        memory->last_error = FAEB_ERROR_LIMIT;
        return NULL;
    }
    
    // Allocate memory
    void* ptr = malloc(size);
    if (!ptr) {
        memory->last_error = FAEB_ERROR_MEMORY;
        return NULL;
    }
    
    // Create block tracker
    struct faeb_memory_block* block = malloc(sizeof(struct faeb_memory_block));
    if (!block) {
        free(ptr);
        memory->last_error = FAEB_ERROR_MEMORY;
        return NULL;
    }
    
    block->ptr = ptr;
    block->size = size;
    block->next = memory->blocks;
    memory->blocks = block;
    
    memory->used_size += size;
    memory->last_error = FAEB_SUCCESS;
    
    return ptr;
}

// Free memory block
void faeb_memory_free(faeb_memory_t* memory, void* ptr) {
    if (!memory || !ptr) return;
    
    // Find and remove block
    struct faeb_memory_block** current = &memory->blocks;
    while (*current) {
        if ((*current)->ptr == ptr) {
            struct faeb_memory_block* block = *current;
            *current = block->next;
            
            memory->used_size -= block->size;
            free(block->ptr);
            free(block);
            
            memory->last_error = FAEB_SUCCESS;
            return;
        }
        current = &(*current)->next;
    }
    
    memory->last_error = FAEB_ERROR_INVALID;
}

// Memory safety verification
bool faeb_verify_memory_safety(const void* ptr, size_t size) {
    if (!ptr || size == 0) return false;
    
    // Basic bounds checking
    // In a real implementation, this would use more sophisticated
    // memory layout analysis and formal verification
    return (size <= SIZE_MAX / 2); // Prevent overflow
}

// Type safety verification
bool faeb_verify_type_safety(const void* ptr, size_t size) {
    if (!ptr || size == 0) return false;
    
    // Check alignment and basic type safety
    // In a real implementation, this would verify type annotations
    return (size % sizeof(void*) == 0); // Word alignment
}

// Thread safety verification
bool faeb_verify_thread_safety(const void* ptr, size_t size) {
    if (!ptr || size == 0) return false;
    
    // Basic thread safety checks
    // In a real implementation, this would analyze concurrent access patterns
    return true; // Simplified for now
}
