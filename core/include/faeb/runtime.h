/* faeb Core Runtime - Minimalist Foundation
 * RISC-V Paradigm: Simple, orthogonal, verifiable
 * License: Apache 2.0
 */

#ifndef FAEB_RUNTIME_H
#define FAEB_RUNTIME_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// RISC-V Principle: Minimal orthogonal components
typedef enum {
    FAEB_SUCCESS = 0,
    FAEB_ERROR_MEMORY,
    FAEB_ERROR_IO,
    FAEB_ERROR_INVALID,
    FAEB_ERROR_LIMIT
} faeb_result_t;

// Memory management - minimal interface
typedef struct faeb_memory faeb_memory_t;

faeb_memory_t* faeb_memory_create(size_t size);
void faeb_memory_destroy(faeb_memory_t* memory);
void* faeb_memory_allocate(faeb_memory_t* memory, size_t size);
void faeb_memory_free(faeb_memory_t* memory, void* ptr);

// Process scheduling - simple cooperative scheduler
typedef struct faeb_process faeb_process_t;
typedef void (*faeb_process_fn)(void* context);

faeb_process_t* faeb_process_create(faeb_process_fn function, void* context);
void faeb_process_destroy(faeb_process_t* process);
void faeb_process_yield(void);
void faeb_process_run(faeb_process_t* process);

// I/O operations - minimal orthogonal operations
typedef struct faeb_io faeb_io_t;

faeb_io_t* faeb_io_create(void);
void faeb_io_destroy(faeb_io_t* io);
size_t faeb_io_read(faeb_io_t* io, void* buffer, size_t size);
size_t faeb_io_write(faeb_io_t* io, const void* buffer, size_t size);

// Verification interface - formal verification support
bool faeb_verify_memory_safety(const void* ptr, size_t size);
bool faeb_verify_type_safety(const void* ptr, size_t size);
bool faeb_verify_thread_safety(const void* ptr, size_t size);

// RISC-V Principle: Extensibility through standard interfaces
typedef struct {
    const char* name;
    uint32_t version;
    void* (*create)(void);
    void (*destroy)(void*);
    faeb_result_t (*operation)(void*, const void*, size_t);
} faeb_extension_t;

#endif // FAEB_RUNTIME_H
