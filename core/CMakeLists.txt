# faeb Core Runtime - CMake Configuration
cmake_minimum_required(VERSION 3.12)
project(faeb-runtime LANGUAGES C CXX)

# RISC-V paradigm principles: Minimalism and verifiability
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Minimal warning levels
set(CMAKE_C_FLAGS "-Wall -Wextra -Werror -pedantic")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -pedantic")

# Size optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")

# Source files - minimal orthogonal components
set(RUNTIME_SOURCES
    src/memory.c
    src/process.c
    src/io.c
    src/scheduler.c
    src/verification.c
)

# Include directories
include_directories(
    include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Create core runtime library
add_library(faeb-runtime STATIC ${RUNTIME_SOURCES})

# Installation targets
install(TARGETS faeb-runtime
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)
