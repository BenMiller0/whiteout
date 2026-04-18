#ifndef MEMORY_PROFILER_HPP
#define MEMORY_PROFILER_HPP

#include "constants.hpp"
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// =============================================================================
// MEMORY PROFILER - TASK MONITORING AND SAFETY
// =============================================================================

// -----------------------------------------------------------------------------
// PROFILING CONFIGURATION
// -----------------------------------------------------------------------------
#define STACK_WARNING_THRESHOLD     80    // % stack usage before warning
#define STACK_CRITICAL_THRESHOLD    90    // % stack usage before critical
#define HEAP_WARNING_THRESHOLD      85    // % heap usage before warning
#define HEAP_CRITICAL_THRESHOLD     95    // % heap usage before critical
#define PROFILING_INTERVAL_MS       5000  // How often to check memory (5 seconds)

// -----------------------------------------------------------------------------
// TASK MEMORY STATISTICS STRUCTURE
// -----------------------------------------------------------------------------
struct TaskMemoryStats {
    TaskHandle_t taskHandle;        // Handle to the task
    char taskName[16];              // Task name
    uint32_t stackHighWaterMark;    // Minimum stack space ever available
    uint32_t stackSize;             // Total stack size
    uint8_t stackUsagePercent;      // Current stack usage percentage
    bool isHealthy;                 // Overall task health status
    uint32_t lastCheckTime;         // Last time stats were updated
};

// -----------------------------------------------------------------------------
// HEAP STATISTICS STRUCTURE
// -----------------------------------------------------------------------------
struct HeapStats {
    uint32_t totalHeap;             // Total heap size
    uint32_t freeHeap;              // Current free heap
    uint32_t minFreeHeap;           // Minimum free heap ever recorded
    uint8_t heapUsagePercent;       // Current heap usage percentage
    uint32_t largestFreeBlock;      // Largest contiguous free block
    uint8_t fragmentationPercent;  // Heap fragmentation percentage
    bool isHealthy;                 // Overall heap health status
};

// -----------------------------------------------------------------------------
// GLOBAL PROFILING STATE
// -----------------------------------------------------------------------------
extern TaskMemoryStats taskStats[NUM_LEDS];  // Stats for each LED task
extern HeapStats globalHeapStats;             // Global heap statistics
extern bool profilingEnabled;                 // Runtime enable/disable flag

// -----------------------------------------------------------------------------
// PROFILING FUNCTION DECLARATIONS
// -----------------------------------------------------------------------------
// Initialize the memory profiler system
void initMemoryProfiler();

// Register a task for memory monitoring
void registerTaskForProfiling(TaskHandle_t taskHandle, const char* taskName, uint32_t stackSize);

// Update memory statistics for all registered tasks
void updateMemoryStats();

// Check and report task stack health
bool checkTaskStackHealth(TaskHandle_t taskHandle);

// Check and report global heap health
bool checkHeapHealth();

// Print detailed memory statistics to Serial
void printMemoryStats();

// Print memory warnings and critical alerts
void printMemoryWarnings();

// Get current heap fragmentation percentage
uint8_t getHeapFragmentation();

// Get task stack usage percentage
uint8_t getTaskStackUsage(TaskHandle_t taskHandle);

// Emergency memory recovery actions
void performEmergencyMemoryRecovery();

// Main profiler task (runs periodically)
void memoryProfilerTask(void* pvParameters);

// Safety check before task operations
bool isSafeToCreateTask(uint32_t requiredStackSize);

// Log memory event with timestamp
void logMemoryEvent(const char* event, const char* details);

#endif // MEMORY_PROFILER_HPP
