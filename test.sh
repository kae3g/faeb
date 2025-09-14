#!/bin/sh
# FAEB Test Suite
# RISC-V Paradigm: Simple, orthogonal, verifiable
set -e

echo "🧪 Running FAEB Test Suite..."
echo "💙 Applying RISC-V paradigm principles"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test results
TESTS_PASSED=0
TESTS_FAILED=0
TESTS_TOTAL=0

# Test function
run_test() {
    local test_name="$1"
    local test_command="$2"
    
    TESTS_TOTAL=$((TESTS_TOTAL + 1))
    
    echo "${BLUE}🔍 Running test: $test_name${NC}"
    
    if eval "$test_command"; then
        echo "${GREEN}✅ Test passed: $test_name${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo "${RED}❌ Test failed: $test_name${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo
}

# Check if build exists
if [ ! -f "build/core/libfaeb-runtime.a" ]; then
    echo "${YELLOW}⚠️  Build not found. Building first...${NC}"
    ./build.sh
fi

# Compile test program
echo "${BLUE}🔨 Compiling test program...${NC}"
gcc -o test_faeb \
    -I./core/include \
    -L./build/core \
    -lfaeb-runtime \
    tests/test_main.c \
    tests/test_memory.c \
    tests/test_process.c \
    tests/test_io.c \
    tests/test_scheduler.c \
    tests/test_verification.c

# Run tests
echo "${BLUE}🚀 Starting test execution...${NC}"

# Test 1: Memory Management
run_test "Memory Management - Basic Allocation" \
    "echo 'Testing memory allocation...' && ./test_faeb --test memory_basic"

run_test "Memory Management - Boundary Checks" \
    "echo 'Testing memory boundaries...' && ./test_faeb --test memory_boundaries"

run_test "Memory Management - Cleanup" \
    "echo 'Testing memory cleanup...' && ./test_faeb --test memory_cleanup"

# Test 2: Process Management
run_test "Process Management - Creation" \
    "echo 'Testing process creation...' && ./test_faeb --test process_creation"

run_test "Process Management - Scheduling" \
    "echo 'Testing process scheduling...' && ./test_faeb --test process_scheduling"

run_test "Process Management - Destruction" \
    "echo 'Testing process destruction...' && ./test_faeb --test process_destruction"

# Test 3: I/O Operations
run_test "I/O Operations - Basic Read/Write" \
    "echo 'Testing I/O operations...' && ./test_faeb --test io_basic"

run_test "I/O Operations - Error Handling" \
    "echo 'Testing I/O error handling...' && ./test_faeb --test io_errors"

# Test 4: Scheduler
run_test "Scheduler - Basic Scheduling" \
    "echo 'Testing scheduler...' && ./test_faeb --test scheduler_basic"

run_test "Scheduler - Time Slices" \
    "echo 'Testing time slices...' && ./test_faeb --test scheduler_timeslices"

# Test 5: Verification
run_test "Verification - Memory Safety" \
    "echo 'Testing memory safety verification...' && ./test_faeb --test verification_memory"

run_test "Verification - Type Safety" \
    "echo 'Testing type safety verification...' && ./test_faeb --test verification_type"

run_test "Verification - Thread Safety" \
    "echo 'Testing thread safety verification...' && ./test_faeb --test verification_thread"

run_test "Verification - Runtime Integrity" \
    "echo 'Testing runtime integrity...' && ./test_faeb --test verification_runtime"

# Test 6: Integration Tests
run_test "Integration - Complete Workflow" \
    "echo 'Testing complete workflow...' && ./test_faeb --test integration_complete"

# Test 7: Performance Tests
run_test "Performance - Memory Allocation" \
    "echo 'Testing memory allocation performance...' && ./test_faeb --test performance_memory"

run_test "Performance - Process Switching" \
    "echo 'Testing process switching performance...' && ./test_faeb --test performance_process"

# Test 8: Stress Tests
run_test "Stress - Memory Stress" \
    "echo 'Testing memory stress...' && ./test_faeb --test stress_memory"

run_test "Stress - Process Stress" \
    "echo 'Testing process stress...' && ./test_faeb --test stress_process"

# Test 9: Edge Cases
run_test "Edge Cases - Null Pointers" \
    "echo 'Testing null pointer handling...' && ./test_faeb --test edge_null"

run_test "Edge Cases - Invalid Sizes" \
    "echo 'Testing invalid size handling...' && ./test_faeb --test edge_size"

# Test 10: RISC-V Paradigm Compliance
run_test "RISC-V Paradigm - Simplicity" \
    "echo 'Testing RISC-V simplicity compliance...' && ./test_faeb --test riscv_simplicity"

run_test "RISC-V Paradigm - Modularity" \
    "echo 'Testing RISC-V modularity compliance...' && ./test_faeb --test riscv_modularity"

run_test "RISC-V Paradigm - Verifiability" \
    "echo 'Testing RISC-V verifiability compliance...' && ./test_faeb --test riscv_verifiability"

# Print test results
echo "${BLUE}📊 Test Results Summary${NC}"
echo "=========================="
echo "${GREEN}Tests Passed: $TESTS_PASSED${NC}"
echo "${RED}Tests Failed: $TESTS_FAILED${NC}"
echo "${BLUE}Tests Total: $TESTS_TOTAL${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo "${GREEN}🎉 All tests passed!${NC}"
    echo "${GREEN}💙 FAEB runtime is ready for production${NC}"
    exit 0
else
    echo "${RED}❌ Some tests failed${NC}"
    echo "${YELLOW}⚠️  Please review failed tests and fix issues${NC}"
    exit 1
fi
