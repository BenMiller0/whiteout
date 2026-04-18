#include "test_mode.hpp"
#include "blink_helpers.hpp"
#include <Arduino.h>
#include <stdio.h>

// =============================================================================
// TEST MODE - TESTING FRAMEWORK IMPLEMENTATION
// =============================================================================

// -----------------------------------------------------------------------------
// GLOBAL VARIABLES
// -----------------------------------------------------------------------------
// LED parameters for test mode (separate from normal mode parameters)
static LedTaskParams testLedParams[NUM_LEDS];

// Task handles for managing LED tasks in test mode
static TaskHandle_t ledTaskHandles[NUM_LEDS] = {nullptr};

// -----------------------------------------------------------------------------
// LED CONFIGURATION HELPER FUNCTIONS
// -----------------------------------------------------------------------------
// Get pin number for LED index
int getLedPin(int index) {
    switch (index) {
        case 0: return L_BELT_RED;
        case 1: return L_BELT_GREEN_0;
        case 2: return L_BELT_GREEN_1;
        case 3: return R_BELT_RED;
        case 4: return R_BELT_GREEN_0;
        case 5: return R_BELT_GREEN_1;
        default: return L_BELT_RED; // fallback
    }
}

// Get delay value for LED index
int getLedDelay(int index) {
    switch (index) {
        case 0: return L_BELT_RED_DELAY;
        case 1: return L_BELT_GREEN_0_DELAY;
        case 2: return L_BELT_GREEN_1_DELAY;
        case 3: return R_BELT_RED_DELAY;
        case 4: return R_BELT_GREEN_0_DELAY;
        case 5: return R_BELT_GREEN_1_DELAY;
        default: return L_BELT_RED_DELAY; // fallback
    }
}

// Get volatility value for LED index
float getLedVolatility(int index) {
    switch (index) {
        case 0: return L_BELT_RED_VOLATILITY;
        case 1: return L_BELT_GREEN_0_VOLATILITY;
        case 2: return L_BELT_GREEN_1_VOLATILITY;
        case 3: return R_BELT_RED_VOLATILITY;
        case 4: return R_BELT_GREEN_0_VOLATILITY;
        case 5: return R_BELT_GREEN_1_VOLATILITY;
        default: return 0.0f; // fallback
    }
}

// -----------------------------------------------------------------------------
// TASK MANAGEMENT FUNCTIONS
// -----------------------------------------------------------------------------
// Create LED tasks for test mode
void createLedTasks() {
    for (int i = 0; i < NUM_LEDS; i++) {
        char taskName[20];
        sprintf(taskName, "LED%d", i);
        xTaskCreate(
            ledBlinkTask, 
            taskName,
            1000,  // Stack size
            &testLedParams[i], 
            1,      // Priority
            &ledTaskHandles[i]
        );
    }
}

// Delete all LED tasks in test mode
void deleteLedTasks() {
    for (int i = 0; i < NUM_LEDS; i++) {
        if (ledTaskHandles[i] != nullptr) {
            vTaskDelete(ledTaskHandles[i]);
            ledTaskHandles[i] = nullptr;
        }
    }
}

// -----------------------------------------------------------------------------
// MAIN TEST MODE CONTROLLER
// -----------------------------------------------------------------------------
// Main test mode runner - cycles through all blinking modes
void runTestMode() {
    Serial.println("=== TEST MODE STARTED ===");
    Serial.println("Cycling through all blinking modes...");
    
    while (true) {
        // Test 1: Digital + Volatile
        testDigitalVolatileMode();
        delay(TEST_MODE_SWITCH_DELAY);
        
        // Test 2: Digital + Non-Volatile
        testDigitalNonVolatileMode();
        delay(TEST_MODE_SWITCH_DELAY);
        
        // Test 3: Smooth + Volatile
        testSmoothVolatileMode();
        delay(TEST_MODE_SWITCH_DELAY);
        
        // Test 4: Smooth + Non-Volatile
        testSmoothNonVolatileMode();
        delay(TEST_MODE_SWITCH_DELAY);
        
        Serial.println("=== Test cycle completed, restarting ===");
    }
}

// -----------------------------------------------------------------------------
// STATUS AND REPORTING FUNCTIONS
// -----------------------------------------------------------------------------
// Print current test mode status and LED configuration
void printTestModeStatus(const char* modeName, bool volatileEnabled, bool smoothEnabled) {
    Serial.printf("=== MODE: %s ===\n", modeName);
    Serial.printf("Volatile Blinking: %s\n", volatileEnabled ? "ENABLED" : "DISABLED");
    Serial.printf("Smooth Blinking: %s\n", smoothEnabled ? "ENABLED" : "DISABLED");
    Serial.printf("Test Duration: %d ms\n", TEST_MODE_DURATION_MS);
    Serial.println("LED Configuration:");
    
    const char* ledNames[] = {"L_BELT_RED", "L_BELT_GREEN_0", "L_BELT_GREEN_1", 
                             "R_BELT_RED", "R_BELT_GREEN_0", "R_BELT_GREEN_1"};
    
    for (int i = 0; i < NUM_LEDS; i++) {
        Serial.printf("  %s: Pin=%d, Delay=%dms, Volatility=%.1f\n", 
                     ledNames[i], 
                     testLedParams[i].pin, 
                     testLedParams[i].delay, 
                     testLedParams[i].volatilityMultiplier);
    }
    Serial.println("========================");
}

// -----------------------------------------------------------------------------
// TEST MODE IMPLEMENTATION FUNCTIONS
// -----------------------------------------------------------------------------
// Test digital blinking with volatile timing
void testDigitalVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for digital volatile mode
    for (int i = 0; i < NUM_LEDS; i++) {
        testLedParams[i] = {
            getLedPin(i),
            getLedDelay(i),
            1,  // volatile enabled
            0,  // smooth disabled
            getLedVolatility(i)
        };
    }
    
    printTestModeStatus("DIGITAL + VOLATILE", true, false);
    
    // Reinitialize GPIO pins for digital mode
    initializeGpioPins(testLedParams, NUM_LEDS);
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}

// Test digital blinking with non-volatile timing
void testDigitalNonVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for digital non-volatile mode
    for (int i = 0; i < NUM_LEDS; i++) {
        testLedParams[i] = {
            getLedPin(i),
            getLedDelay(i),
            0,  // volatile disabled
            0,  // smooth disabled
            1.0f  // volatility doesn't matter when volatile is disabled
        };
    }
    
    printTestModeStatus("DIGITAL + NON-VOLATILE", false, false);
    
    // Reinitialize GPIO pins for digital mode
    initializeGpioPins(testLedParams, NUM_LEDS);
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}

// Test smooth PWM blinking with volatile timing
void testSmoothVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for smooth volatile mode
    for (int i = 0; i < NUM_LEDS; i++) {
        testLedParams[i] = {
            getLedPin(i),
            getLedDelay(i),
            1,  // volatile enabled
            1,  // smooth enabled
            getLedVolatility(i)
        };
    }
    
    printTestModeStatus("SMOOTH + VOLATILE", true, true);
    
    // Reinitialize GPIO pins for PWM mode
    initializePwmPins(testLedParams, NUM_LEDS);
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}

// Test smooth PWM blinking with non-volatile timing
void testSmoothNonVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for smooth non-volatile mode
    for (int i = 0; i < NUM_LEDS; i++) {
        testLedParams[i] = {
            getLedPin(i),
            getLedDelay(i),
            0,  // volatile disabled
            1,  // smooth enabled
            1.0f  // volatility doesn't matter when volatile is disabled
        };
    }
    
    printTestModeStatus("SMOOTH + NON-VOLATILE", false, true);
    
    // Reinitialize GPIO pins for PWM mode
    initializePwmPins(testLedParams, NUM_LEDS);
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}
