/* faeb Core Runtime - I/O Operations
 * RISC-V Paradigm: Simple, orthogonal, verifiable
 * License: Apache 2.0
 */

#include "faeb/runtime.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// I/O buffer structure
struct faeb_io_buffer {
    char* data;
    size_t size;
    size_t position;
    int fd;
    bool is_open;
};

// I/O manager structure
struct faeb_io {
    struct faeb_io_buffer* stdin_buf;
    struct faeb_io_buffer* stdout_buf;
    struct faeb_io_buffer* stderr_buf;
    faeb_result_t last_error;
};

// Create I/O manager
faeb_io_t* faeb_io_create(void) {
    faeb_io_t* io = malloc(sizeof(faeb_io_t));
    if (!io) return NULL;
    
    // Initialize buffers
    io->stdin_buf = malloc(sizeof(struct faeb_io_buffer));
    io->stdout_buf = malloc(sizeof(struct faeb_io_buffer));
    io->stderr_buf = malloc(sizeof(struct faeb_io_buffer));
    
    if (!io->stdin_buf || !io->stdout_buf || !io->stderr_buf) {
        faeb_io_destroy(io);
        return NULL;
    }
    
    // Initialize stdin buffer
    io->stdin_buf->data = NULL;
    io->stdin_buf->size = 0;
    io->stdin_buf->position = 0;
    io->stdin_buf->fd = STDIN_FILENO;
    io->stdin_buf->is_open = true;
    
    // Initialize stdout buffer
    io->stdout_buf->data = NULL;
    io->stdout_buf->size = 0;
    io->stdout_buf->position = 0;
    io->stdout_buf->fd = STDOUT_FILENO;
    io->stdout_buf->is_open = true;
    
    // Initialize stderr buffer
    io->stderr_buf->data = NULL;
    io->stderr_buf->size = 0;
    io->stderr_buf->position = 0;
    io->stderr_buf->fd = STDERR_FILENO;
    io->stderr_buf->is_open = true;
    
    io->last_error = FAEB_SUCCESS;
    
    return io;
}

// Destroy I/O manager
void faeb_io_destroy(faeb_io_t* io) {
    if (!io) return;
    
    // Free buffers
    if (io->stdin_buf) {
        if (io->stdin_buf->data) free(io->stdin_buf->data);
        free(io->stdin_buf);
    }
    
    if (io->stdout_buf) {
        if (io->stdout_buf->data) free(io->stdout_buf->data);
        free(io->stdout_buf);
    }
    
    if (io->stderr_buf) {
        if (io->stderr_buf->data) free(io->stderr_buf->data);
        free(io->stderr_buf);
    }
    
    free(io);
}

// Read from I/O buffer
size_t faeb_io_read(faeb_io_t* io, void* buffer, size_t size) {
    if (!io || !buffer || size == 0) {
        if (io) io->last_error = FAEB_ERROR_INVALID;
        return 0;
    }
    
    // Use stdin for reading
    struct faeb_io_buffer* buf = io->stdin_buf;
    if (!buf->is_open) {
        io->last_error = FAEB_ERROR_IO;
        return 0;
    }
    
    // Read from file descriptor
    ssize_t bytes_read = read(buf->fd, buffer, size);
    if (bytes_read < 0) {
        io->last_error = FAEB_ERROR_IO;
        return 0;
    }
    
    io->last_error = FAEB_SUCCESS;
    return (size_t)bytes_read;
}

// Write to I/O buffer
size_t faeb_io_write(faeb_io_t* io, const void* buffer, size_t size) {
    if (!io || !buffer || size == 0) {
        if (io) io->last_error = FAEB_ERROR_INVALID;
        return 0;
    }
    
    // Use stdout for writing
    struct faeb_io_buffer* buf = io->stdout_buf;
    if (!buf->is_open) {
        io->last_error = FAEB_ERROR_IO;
        return 0;
    }
    
    // Write to file descriptor
    ssize_t bytes_written = write(buf->fd, buffer, size);
    if (bytes_written < 0) {
        io->last_error = FAEB_ERROR_IO;
        return 0;
    }
    
    io->last_error = FAEB_SUCCESS;
    return (size_t)bytes_written;
}

// Flush I/O buffers
faeb_result_t faeb_io_flush(faeb_io_t* io) {
    if (!io) return FAEB_ERROR_INVALID;
    
    // Flush stdout
    if (io->stdout_buf && io->stdout_buf->is_open) {
        if (fflush(stdout) != 0) {
            io->last_error = FAEB_ERROR_IO;
            return FAEB_ERROR_IO;
        }
    }
    
    // Flush stderr
    if (io->stderr_buf && io->stderr_buf->is_open) {
        if (fflush(stderr) != 0) {
            io->last_error = FAEB_ERROR_IO;
            return FAEB_ERROR_IO;
        }
    }
    
    io->last_error = FAEB_SUCCESS;
    return FAEB_SUCCESS;
}

// Get last I/O error
faeb_result_t faeb_io_get_last_error(faeb_io_t* io) {
    return io ? io->last_error : FAEB_ERROR_INVALID;
}

// Check if I/O is available
bool faeb_io_is_available(faeb_io_t* io) {
    if (!io) return false;
    
    return (io->stdin_buf && io->stdin_buf->is_open) &&
           (io->stdout_buf && io->stdout_buf->is_open) &&
           (io->stderr_buf && io->stderr_buf->is_open);
}
