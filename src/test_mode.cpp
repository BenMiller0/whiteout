#include "test_mode.hpp"
#include <Arduino.h>
#include <stdio.h>

// Global LED parameters for test mode
static LedTaskParams testLedParams[6];
static TaskHandle_t ledTaskHandles[6] = {nullptr};

void createLedTasks() {
    for (int i = 0; i < 6; i++) {
        char taskName[20];
        sprintf(taskName, "LED%d", i);
        xTaskCreate(
            ledBlinkTask, 
            taskName,
            1000,
            &testLedParams[i], 
            1,
            &ledTaskHandles[i]
        );
    }
}

void deleteLedTasks() {
    for (int i = 0; i < 6; i++) {
        if (ledTaskHandles[i] != nullptr) {
            vTaskDelete(ledTaskHandles[i]);
            ledTaskHandles[i] = nullptr;
        }
    }
}

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

void printTestModeStatus(const char* modeName, bool volatileEnabled, bool smoothEnabled) {
    Serial.printf("=== MODE: %s ===\n", modeName);
    Serial.printf("Volatile Blinking: %s\n", volatileEnabled ? "ENABLED" : "DISABLED");
    Serial.printf("Smooth Blinking: %s\n", smoothEnabled ? "ENABLED" : "DISABLED");
    Serial.printf("Test Duration: %d ms\n", TEST_MODE_DURATION_MS);
    Serial.println("LED Configuration:");
    
    for (int i = 0; i < 6; i++) {
        const char* ledNames[] = {"L_BELT_RED", "L_BELT_GREEN_0", "L_BELT_GREEN_1", 
                                 "R_BELT_RED", "R_BELT_GREEN_0", "R_BELT_GREEN_1"};
        Serial.printf("  %s: Pin=%d, Delay=%dms, Volatility=%.1f\n", 
                     ledNames[i], 
                     testLedParams[i].pin, 
                     testLedParams[i].delay, 
                     testLedParams[i].volatilityMultiplier);
    }
    Serial.println("========================");
}

void testDigitalVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for digital volatile mode
    for (int i = 0; i < 6; i++) {
        testLedParams[i] = {
            (i == 0) ? L_BELT_RED : (i == 1) ? L_BELT_GREEN_0 : (i == 2) ? L_BELT_GREEN_1 :
            (i == 3) ? R_BELT_RED : (i == 4) ? R_BELT_GREEN_0 : R_BELT_GREEN_1,
            (i == 0) ? L_BELT_RED_DELAY : (i == 1) ? L_BELT_GREEN_0_DELAY : (i == 2) ? L_BELT_GREEN_1_DELAY :
            (i == 3) ? R_BELT_RED_DELAY : (i == 4) ? R_BELT_GREEN_0_DELAY : R_BELT_GREEN_1_DELAY,
            1,  // volatile enabled
            0,  // smooth disabled
            static_cast<float>((i == 0) ? L_BELT_RED_VOLATILITY : (i == 1) ? L_BELT_GREEN_0_VOLATILITY : (i == 2) ? L_BELT_GREEN_1_VOLATILITY :
            (i == 3) ? R_BELT_RED_VOLATILITY : (i == 4) ? R_BELT_GREEN_0_VOLATILITY : R_BELT_GREEN_1_VOLATILITY)
        };
    }
    
    printTestModeStatus("DIGITAL + VOLATILE", true, false);
    
    // Reinitialize GPIO pins for digital mode
    for (int i = 0; i < 6; i++) {
        pinMode(testLedParams[i].pin, OUTPUT);
        digitalWrite(testLedParams[i].pin, LOW);
    }
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}

void testDigitalNonVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for digital non-volatile mode
    for (int i = 0; i < 6; i++) {
        testLedParams[i] = {
            (i == 0) ? L_BELT_RED : (i == 1) ? L_BELT_GREEN_0 : (i == 2) ? L_BELT_GREEN_1 :
            (i == 3) ? R_BELT_RED : (i == 4) ? R_BELT_GREEN_0 : R_BELT_GREEN_1,
            (i == 0) ? L_BELT_RED_DELAY : (i == 1) ? L_BELT_GREEN_0_DELAY : (i == 2) ? L_BELT_GREEN_1_DELAY :
            (i == 3) ? R_BELT_RED_DELAY : (i == 4) ? R_BELT_GREEN_0_DELAY : R_BELT_GREEN_1_DELAY,
            0,  // volatile disabled
            0,  // smooth disabled
            1.0f  // volatility doesn't matter when volatile is disabled
        };
    }
    
    printTestModeStatus("DIGITAL + NON-VOLATILE", false, false);
    
    // Reinitialize GPIO pins for digital mode
    for (int i = 0; i < 6; i++) {
        pinMode(testLedParams[i].pin, OUTPUT);
        digitalWrite(testLedParams[i].pin, LOW);
    }
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}

void testSmoothVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for smooth volatile mode
    for (int i = 0; i < 6; i++) {
        testLedParams[i] = {
            (i == 0) ? L_BELT_RED : (i == 1) ? L_BELT_GREEN_0 : (i == 2) ? L_BELT_GREEN_1 :
            (i == 3) ? R_BELT_RED : (i == 4) ? R_BELT_GREEN_0 : R_BELT_GREEN_1,
            (i == 0) ? L_BELT_RED_DELAY : (i == 1) ? L_BELT_GREEN_0_DELAY : (i == 2) ? L_BELT_GREEN_1_DELAY :
            (i == 3) ? R_BELT_RED_DELAY : (i == 4) ? R_BELT_GREEN_0_DELAY : R_BELT_GREEN_1_DELAY,
            1,  // volatile enabled
            1,  // smooth enabled
            static_cast<float>((i == 0) ? L_BELT_RED_VOLATILITY : (i == 1) ? L_BELT_GREEN_0_VOLATILITY : (i == 2) ? L_BELT_GREEN_1_VOLATILITY :
            (i == 3) ? R_BELT_RED_VOLATILITY : (i == 4) ? R_BELT_GREEN_0_VOLATILITY : R_BELT_GREEN_1_VOLATILITY)
        };
    }
    
    printTestModeStatus("SMOOTH + VOLATILE", true, true);
    
    // Reinitialize GPIO pins for PWM mode
    for (int i = 0; i < 6; i++) {
        ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(testLedParams[i].pin, i);
        ledcWrite(i, 0);
    }
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}

void testSmoothNonVolatileMode() {
    // Delete any existing tasks
    deleteLedTasks();
    
    // Configure for smooth non-volatile mode
    for (int i = 0; i < 6; i++) {
        testLedParams[i] = {
            (i == 0) ? L_BELT_RED : (i == 1) ? L_BELT_GREEN_0 : (i == 2) ? L_BELT_GREEN_1 :
            (i == 3) ? R_BELT_RED : (i == 4) ? R_BELT_GREEN_0 : R_BELT_GREEN_1,
            (i == 0) ? L_BELT_RED_DELAY : (i == 1) ? L_BELT_GREEN_0_DELAY : (i == 2) ? L_BELT_GREEN_1_DELAY :
            (i == 3) ? R_BELT_RED_DELAY : (i == 4) ? R_BELT_GREEN_0_DELAY : R_BELT_GREEN_1_DELAY,
            0,  // volatile disabled
            1,  // smooth enabled
            1.0f  // volatility doesn't matter when volatile is disabled
        };
    }
    
    printTestModeStatus("SMOOTH + NON-VOLATILE", false, true);
    
    // Reinitialize GPIO pins for PWM mode
    for (int i = 0; i < 6; i++) {
        ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(testLedParams[i].pin, i);
        ledcWrite(i, 0);
    }
    
    // Create and start LED tasks
    createLedTasks();
    
    // Run for the test duration
    delay(TEST_MODE_DURATION_MS);
    
    // Clean up tasks
    deleteLedTasks();
}
