/* FAEB Test Suite - Main Test Runner
 * RISC-V Paradigm: Simple, orthogonal, verifiable
 * License: Apache 2.0
 */

#include "faeb/runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

// Test function prototypes
extern int test_memory_basic(void);
extern int test_memory_boundaries(void);
extern int test_memory_cleanup(void);
extern int test_process_creation(void);
extern int test_process_scheduling(void);
extern int test_process_destruction(void);
extern int test_io_basic(void);
extern int test_io_errors(void);
extern int test_scheduler_basic(void);
extern int test_scheduler_timeslices(void);
extern int test_verification_memory(void);
extern int test_verification_type(void);
extern int test_verification_thread(void);
extern int test_verification_runtime(void);
extern int test_integration_complete(void);
extern int test_performance_memory(void);
extern int test_performance_process(void);
extern int test_stress_memory(void);
extern int test_stress_process(void);
extern int test_edge_null(void);
extern int test_edge_size(void);
extern int test_riscv_simplicity(void);
extern int test_riscv_modularity(void);
extern int test_riscv_verifiability(void);

// Test structure
struct test_case {
    const char* name;
    int (*function)(void);
};

// Test cases
static struct test_case tests[] = {
    {"memory_basic", test_memory_basic},
    {"memory_boundaries", test_memory_boundaries},
    {"memory_cleanup", test_memory_cleanup},
    {"process_creation", test_process_creation},
    {"process_scheduling", test_process_scheduling},
    {"process_destruction", test_process_destruction},
    {"io_basic", test_io_basic},
    {"io_errors", test_io_errors},
    {"scheduler_basic", test_scheduler_basic},
    {"scheduler_timeslices", test_scheduler_timeslices},
    {"verification_memory", test_verification_memory},
    {"verification_type", test_verification_type},
    {"verification_thread", test_verification_thread},
    {"verification_runtime", test_verification_runtime},
    {"integration_complete", test_integration_complete},
    {"performance_memory", test_performance_memory},
    {"performance_process", test_performance_process},
    {"stress_memory", test_stress_memory},
    {"stress_process", test_stress_process},
    {"edge_null", test_edge_null},
    {"edge_size", test_edge_size},
    {"riscv_simplicity", test_riscv_simplicity},
    {"riscv_modularity", test_riscv_modularity},
    {"riscv_verifiability", test_riscv_verifiability},
    {NULL, NULL}
};

// Run specific test
int run_test(const char* test_name) {
    for (int i = 0; tests[i].name != NULL; i++) {
        if (strcmp(tests[i].name, test_name) == 0) {
            printf("Running test: %s\n", test_name);
            int result = tests[i].function();
            if (result == 0) {
                printf("✅ Test passed: %s\n", test_name);
                return 0;
            } else {
                printf("❌ Test failed: %s (exit code: %d)\n", test_name, result);
                return result;
            }
        }
    }
    
    printf("❌ Test not found: %s\n", test_name);
    return -1;
}

// Run all tests
int run_all_tests(void) {
    int passed = 0;
    int failed = 0;
    
    printf("🧪 Running all FAEB tests...\n");
    printf("============================\n\n");
    
    for (int i = 0; tests[i].name != NULL; i++) {
        printf("Running test: %s\n", tests[i].name);
        int result = tests[i].function();
        if (result == 0) {
            printf("✅ Test passed: %s\n", tests[i].name);
            passed++;
        } else {
            printf("❌ Test failed: %s (exit code: %d)\n", tests[i].name, result);
            failed++;
        }
        printf("\n");
    }
    
    printf("📊 Test Results Summary\n");
    printf("=======================\n");
    printf("Tests Passed: %d\n", passed);
    printf("Tests Failed: %d\n", failed);
    printf("Tests Total: %d\n", passed + failed);
    
    if (failed == 0) {
        printf("🎉 All tests passed!\n");
        printf("💙 FAEB runtime is ready for production\n");
        return 0;
    } else {
        printf("❌ Some tests failed\n");
        printf("⚠️  Please review failed tests and fix issues\n");
        return failed;
    }
}

// Print usage
void print_usage(const char* program_name) {
    printf("FAEB Test Suite\n");
    printf("===============\n");
    printf("Usage: %s [options]\n", program_name);
    printf("\nOptions:\n");
    printf("  --test <name>    Run specific test\n");
    printf("  --all            Run all tests (default)\n");
    printf("  --list           List available tests\n");
    printf("  --help           Show this help message\n");
    printf("\nAvailable tests:\n");
    
    for (int i = 0; tests[i].name != NULL; i++) {
        printf("  - %s\n", tests[i].name);
    }
}

// List available tests
void list_tests(void) {
    printf("Available FAEB Tests:\n");
    printf("====================\n");
    
    for (int i = 0; tests[i].name != NULL; i++) {
        printf("%d. %s\n", i + 1, tests[i].name);
    }
}

// Main function
int main(int argc, char* argv[]) {
    printf("🌸 FAEB Test Suite - Minimalist Foundation Testing\n");
    printf("💙 RISC-V Paradigm: Simple, orthogonal, verifiable\n\n");
    
    if (argc == 1) {
        // No arguments, run all tests
        return run_all_tests();
    }
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--list") == 0) {
            list_tests();
            return 0;
        } else if (strcmp(argv[i], "--all") == 0) {
            return run_all_tests();
        } else if (strcmp(argv[i], "--test") == 0) {
            if (i + 1 < argc) {
                return run_test(argv[i + 1]);
            } else {
                printf("❌ Error: --test requires a test name\n");
                return -1;
            }
        } else if (strncmp(argv[i], "--test=", 7) == 0) {
            return run_test(argv[i] + 7);
        } else {
            printf("❌ Error: Unknown option: %s\n", argv[i]);
            print_usage(argv[0]);
            return -1;
        }
    }
    
    return 0;
}
