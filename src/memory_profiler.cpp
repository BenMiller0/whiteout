#include "memory_profiler.hpp"
#include <esp_heap_caps.h>

// =============================================================================
// MEMORY PROFILER - IMPLEMENTATION
// =============================================================================

// -----------------------------------------------------------------------------
// GLOBAL PROFILING VARIABLES
// -----------------------------------------------------------------------------
TaskMemoryStats taskStats[NUM_LEDS];
HeapStats globalHeapStats;
bool profilingEnabled = true;

// -----------------------------------------------------------------------------
// INITIALIZATION FUNCTIONS
// -----------------------------------------------------------------------------
void initMemoryProfiler() {
    // Initialize task stats array
    for (int i = 0; i < NUM_LEDS; i++) {
        taskStats[i].taskHandle = nullptr;
        strcpy(taskStats[i].taskName, "UNUSED");
        taskStats[i].stackHighWaterMark = 0;
        taskStats[i].stackSize = 0;
        taskStats[i].stackUsagePercent = 0;
        taskStats[i].isHealthy = true;
        taskStats[i].lastCheckTime = 0;
    }
    
    // Initialize heap stats
    globalHeapStats.totalHeap = ESP.getHeapSize();
    globalHeapStats.freeHeap = ESP.getFreeHeap();
    globalHeapStats.minFreeHeap = globalHeapStats.freeHeap;
    globalHeapStats.heapUsagePercent = 0;
    globalHeapStats.largestFreeBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    globalHeapStats.fragmentationPercent = 0;
    globalHeapStats.isHealthy = true;
    
    logMemoryEvent("PROFILER_INIT", "Memory profiler initialized");
}

void registerTaskForProfiling(TaskHandle_t taskHandle, const char* taskName, uint32_t stackSize) {
    if (!profilingEnabled || taskHandle == nullptr) return;
    
    // Find empty slot
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle == nullptr) {
            taskStats[i].taskHandle = taskHandle;
            strncpy(taskStats[i].taskName, taskName, sizeof(taskStats[i].taskName) - 1);
            taskStats[i].taskName[sizeof(taskStats[i].taskName) - 1] = '\0';
            taskStats[i].stackSize = stackSize;
            taskStats[i].lastCheckTime = millis();
            
            logMemoryEvent("TASK_REGISTER", taskName);
            return;
        }
    }
    
    logMemoryEvent("TASK_REGISTER_FAIL", "No available slots for task registration");
}

// -----------------------------------------------------------------------------
// MEMORY MONITORING FUNCTIONS
// -----------------------------------------------------------------------------
void updateMemoryStats() {
    if (!profilingEnabled) return;
    
    uint32_t currentTime = millis();
    
    // Update task statistics
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle != nullptr) {
            // Get stack high water mark (minimum stack space ever available)
            taskStats[i].stackHighWaterMark = uxTaskGetStackHighWaterMark(taskStats[i].taskHandle);
            
            // Calculate stack usage percentage
            if (taskStats[i].stackSize > 0) {
                taskStats[i].stackUsagePercent = 100 - ((taskStats[i].stackHighWaterMark * 100) / taskStats[i].stackSize);
            } else {
                taskStats[i].stackUsagePercent = 0;
            }
            
            // Check task health
            taskStats[i].isHealthy = checkTaskStackHealth(taskStats[i].taskHandle);
            taskStats[i].lastCheckTime = currentTime;
        }
    }
    
    // Update heap statistics
    globalHeapStats.freeHeap = ESP.getFreeHeap();
    globalHeapStats.largestFreeBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    
    // Update minimum free heap if current is lower
    if (globalHeapStats.freeHeap < globalHeapStats.minFreeHeap) {
        globalHeapStats.minFreeHeap = globalHeapStats.freeHeap;
    }
    
    // Calculate heap usage percentage
    if (globalHeapStats.totalHeap > 0) {
        globalHeapStats.heapUsagePercent = ((globalHeapStats.totalHeap - globalHeapStats.freeHeap) * 100) / globalHeapStats.totalHeap;
    }
    
    // Calculate fragmentation
    globalHeapStats.fragmentationPercent = getHeapFragmentation();
    
    // Check heap health
    globalHeapStats.isHealthy = checkHeapHealth();
}

bool checkTaskStackHealth(TaskHandle_t taskHandle) {
    if (!profilingEnabled || taskHandle == nullptr) return false;
    
    uint32_t highWaterMark = uxTaskGetStackHighWaterMark(taskHandle);
    
    // Find the task stats
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle == taskHandle) {
            uint8_t usagePercent = taskStats[i].stackUsagePercent;
            
            if (usagePercent >= STACK_CRITICAL_THRESHOLD) {
                logMemoryEvent("STACK_CRITICAL", taskStats[i].taskName);
                return false;
            } else if (usagePercent >= STACK_WARNING_THRESHOLD) {
                logMemoryEvent("STACK_WARNING", taskStats[i].taskName);
                return true; // Still healthy but warning
            }
            return true; // Healthy
        }
    }
    
    return false; // Task not found
}

bool checkHeapHealth() {
    if (!profilingEnabled) return false;
    
    if (globalHeapStats.heapUsagePercent >= HEAP_CRITICAL_THRESHOLD) {
        logMemoryEvent("HEAP_CRITICAL", "Heap usage critical");
        return false;
    } else if (globalHeapStats.heapUsagePercent >= HEAP_WARNING_THRESHOLD) {
        logMemoryEvent("HEAP_WARNING", "Heap usage warning");
        return true; // Still healthy but warning
    }
    
    // Check fragmentation - only critical above 75%
    if (globalHeapStats.fragmentationPercent > 75) {
        logMemoryEvent("HEAP_FRAGMENTATION", "High heap fragmentation");
        return false;
    }
    
    return true; // Healthy
}

uint8_t getHeapFragmentation() {
    if (!profilingEnabled) return 0;
    
    uint32_t totalFree = ESP.getFreeHeap();
    uint32_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    
    if (totalFree == 0) return 100;
    
    // Fragmentation is the percentage of free heap that is not in the largest block
    uint32_t fragmentedBytes = totalFree - largestBlock;
    return (fragmentedBytes * 100) / totalFree;
}

uint8_t getTaskStackUsage(TaskHandle_t taskHandle) {
    if (!profilingEnabled || taskHandle == nullptr) return 0;
    
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle == taskHandle) {
            return taskStats[i].stackUsagePercent;
        }
    }
    
    return 0; // Task not found
}

// -----------------------------------------------------------------------------
// SAFETY AND RECOVERY FUNCTIONS
// -----------------------------------------------------------------------------
bool isSafeToCreateTask(uint32_t requiredStackSize) {
    if (!profilingEnabled) return true;
    
    uint32_t availableHeap = ESP.getFreeHeap();
    uint32_t largestBlock = heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
    
    // Check if we have enough contiguous memory for the task stack
    if (largestBlock < (requiredStackSize + 1024)) { // Add 1KB safety margin
        logMemoryEvent("TASK_CREATE_UNSAFE", "Insufficient contiguous memory");
        return false;
    }
    
    // Check overall heap health
    if (!globalHeapStats.isHealthy) {
        logMemoryEvent("TASK_CREATE_UNSAFE", "Heap health poor");
        return false;
    }
    
    return true;
}

void performEmergencyMemoryRecovery() {
    logMemoryEvent("EMERGENCY_RECOVERY", "Attempting memory recovery");
    
    // Force multiple heap measurements to trigger ESP32 heap cleanup
    for (int i = 0; i < 5; i++) {
        ESP.getFreeHeap();
        heap_caps_get_largest_free_block(MALLOC_CAP_DEFAULT);
        delay(10);
    }
    
    // Try to allocate and free a small block to help defragment
    void* testBlock = malloc(512);
    if (testBlock) {
        free(testBlock);
    }
    
    // Update stats after recovery attempt
    updateMemoryStats();
    
    if (globalHeapStats.isHealthy) {
        logMemoryEvent("RECOVERY_SUCCESS", "Memory health restored");
    } else {
        logMemoryEvent("RECOVERY_FAILED", "Memory recovery unsuccessful");
    }
}

// -----------------------------------------------------------------------------
// LOGGING AND OUTPUT FUNCTIONS
// -----------------------------------------------------------------------------
void printMemoryStats() {
    if (!profilingEnabled) return;
    
    Serial.println("\n=== MEMORY PROFILER REPORT ===");
    Serial.printf("Uptime: %lu ms\n", millis());
    
    // Heap statistics - use single printf calls to reduce stack usage
    Serial.println("\n--- HEAP STATISTICS ---");
    Serial.printf("Total Heap: %u, Free: %u, Min Free: %u\n", 
                  globalHeapStats.totalHeap, globalHeapStats.freeHeap, globalHeapStats.minFreeHeap);
    Serial.printf("Usage: %u%%, Largest Block: %u, Fragmentation: %u%%\n", 
                  globalHeapStats.heapUsagePercent, globalHeapStats.largestFreeBlock, globalHeapStats.fragmentationPercent);
    Serial.printf("Heap Health: %s\n", globalHeapStats.isHealthy ? "HEALTHY" : "UNHEALTHY");
    
    // Task statistics - minimize stack usage
    Serial.println("\n--- TASK STATISTICS ---");
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle != nullptr) {
            Serial.printf("Task %s: Stack %u/%u (%u%%), Free %u, Health %s\n",
                          taskStats[i].taskName,
                          taskStats[i].stackSize - taskStats[i].stackHighWaterMark,
                          taskStats[i].stackSize,
                          taskStats[i].stackUsagePercent,
                          taskStats[i].stackHighWaterMark,
                          taskStats[i].isHealthy ? "HEALTHY" : "UNHEALTHY");
        }
    }
    
    Serial.println("=== END REPORT ===\n");
}

void printMemoryWarnings() {
    if (!profilingEnabled) return;
    
    bool hasWarnings = false;
    
    // Check heap warnings
    if (globalHeapStats.heapUsagePercent >= HEAP_CRITICAL_THRESHOLD) {
        Serial.println("[CRITICAL] Heap usage critical!");
        hasWarnings = true;
    } else if (globalHeapStats.heapUsagePercent >= HEAP_WARNING_THRESHOLD) {
        Serial.println("[WARNING] Heap usage high!");
        hasWarnings = true;
    }
    
    if (globalHeapStats.fragmentationPercent > 70) {
        Serial.printf("[WARNING] High heap fragmentation: %u%%\n", globalHeapStats.fragmentationPercent);
        hasWarnings = true;
    }
    
    // Check task warnings
    for (int i = 0; i < NUM_LEDS; i++) {
        if (taskStats[i].taskHandle != nullptr) {
            if (taskStats[i].stackUsagePercent >= STACK_CRITICAL_THRESHOLD) {
                Serial.printf("[CRITICAL] Task %s stack usage critical: %u%%\n", 
                             taskStats[i].taskName, taskStats[i].stackUsagePercent);
                hasWarnings = true;
            } else if (taskStats[i].stackUsagePercent >= STACK_WARNING_THRESHOLD) {
                Serial.printf("[WARNING] Task %s stack usage high: %u%%\n", 
                             taskStats[i].taskName, taskStats[i].stackUsagePercent);
                hasWarnings = true;
            }
        }
    }
    
    if (!hasWarnings) {
        Serial.println("[INFO] All memory systems healthy");
    }
}

void logMemoryEvent(const char* event, const char* details) {
    if (!profilingEnabled) return;
    
    Serial.printf("[%lu ms] [MEMORY] %s: %s\n", millis(), event, details);
}

// -----------------------------------------------------------------------------
// PROFILER TASK
// -----------------------------------------------------------------------------
void memoryProfilerTask(void* pvParameters) {
    logMemoryEvent("PROFILER_TASK_START", "Memory profiler task started");
    
    uint32_t lastReportTime = 0;
    const uint32_t REPORT_INTERVAL_MS = 30000; // Report every 30 seconds
    
    while (true) {
        // Update all memory statistics
        updateMemoryStats();
        
        // Print warnings if any
        printMemoryWarnings();
        
        // Check for critical conditions and attempt recovery
        if (!globalHeapStats.isHealthy) {
            performEmergencyMemoryRecovery();
        }
        
        // Check for critical stack conditions (simplified)
        for (int i = 0; i < NUM_LEDS; i++) {
            if (taskStats[i].taskHandle != nullptr && !taskStats[i].isHealthy) {
                if (taskStats[i].stackUsagePercent >= STACK_CRITICAL_THRESHOLD) {
                    Serial.printf("[EMERGENCY] Task %s stack overflow risk!\n", taskStats[i].taskName);
                }
            }
        }
        
        // Print full report periodically
        uint32_t currentTime = millis();
        if (currentTime - lastReportTime >= REPORT_INTERVAL_MS) {
            printMemoryStats();
            lastReportTime = currentTime;
        }
        
        // Wait for next check
        vTaskDelay(pdMS_TO_TICKS(PROFILING_INTERVAL_MS));
    }
}
