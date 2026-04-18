#include <Arduino.h>
#include <WiFi.h>
#include "constants.hpp"
#include "led_blink_task.hpp"
#include "blink_helpers.hpp"
#include "memory_profiler.hpp"

#if TEST_MODE
#include "test_mode.hpp"
#endif

// =============================================================================
// LED BLINK CONTROLLER - MAIN APPLICATION
// =============================================================================

// -----------------------------------------------------------------------------
// LED CONFIGURATION ARRAY
// -----------------------------------------------------------------------------
// Parameter array for all LEDs. Includes pin #, delay, and mode settings.
// The configuration adapts based on NORMAL_MODE vs VOLATILE_BLINKING mode.
static LedTaskParams ledParams[NUM_LEDS] = {
    {L_BELT_RED,        L_BELT_RED_DELAY,       NORMAL_MODE ? 1 : VOLATILE_BLINKING, NORMAL_MODE ? 0 : SMOOTH_BLINKING,   NORMAL_MODE ? 0.1f : L_BELT_RED_VOLATILITY},
    {L_BELT_GREEN_0,    L_BELT_GREEN_0_DELAY,   NORMAL_MODE ? 0 : VOLATILE_BLINKING, NORMAL_MODE ? 0 : SMOOTH_BLINKING,   NORMAL_MODE ? 0.0f : L_BELT_GREEN_0_VOLATILITY},
    {L_BELT_GREEN_1,    L_BELT_GREEN_1_DELAY,   NORMAL_MODE ? 0 : VOLATILE_BLINKING, NORMAL_MODE ? 0 : SMOOTH_BLINKING,   NORMAL_MODE ? 0.0f : L_BELT_GREEN_1_VOLATILITY},
    {R_BELT_RED,        R_BELT_RED_DELAY,       NORMAL_MODE ? 1 : VOLATILE_BLINKING, NORMAL_MODE ? 0 : SMOOTH_BLINKING,   NORMAL_MODE ? 0.1f : R_BELT_RED_VOLATILITY},
    {R_BELT_GREEN_0,    R_BELT_GREEN_0_DELAY,   NORMAL_MODE ? 0 : VOLATILE_BLINKING, NORMAL_MODE ? 0 : SMOOTH_BLINKING,   NORMAL_MODE ? 0.0f : R_BELT_GREEN_0_VOLATILITY},
    {R_BELT_GREEN_1,    R_BELT_GREEN_1_DELAY,   NORMAL_MODE ? 0 : VOLATILE_BLINKING, NORMAL_MODE ? 0 : SMOOTH_BLINKING,   NORMAL_MODE ? 0.0f : R_BELT_GREEN_1_VOLATILITY}
};

// -----------------------------------------------------------------------------
// SETUP FUNCTION
// -----------------------------------------------------------------------------
void setup() {
#if ENABLE_SERIAL_OUTPUT
    Serial.begin(115200);
    delay(1000); // Wait for serial to be ready
    Serial.println("LED Blink Controller Starting...");
#endif
    
#if ENABLE_MEMORY_PROFILING
    // Initialize memory profiler
    initMemoryProfiler();
    
    // Create memory profiler task
    xTaskCreate(
        memoryProfilerTask, 
        "Memory Profiler",
        4096,  // Stack size
        NULL, 
        1,      // Priority
        NULL    // Task handle
    );
#endif

#if DISABLE_WIFI
    // Disable WiFi for power savings
    WiFi.mode(WIFI_OFF);
    WiFi.persistent(false);
#endif

#if DISABLE_BLUETOOTH
    // Disable Bluetooth for power savings
    btStop();
#endif

#if CPU_FREQUENCY_MHZ != 240
    // Lower CPU frequency for power savings
    setCpuFrequencyMhz(CPU_FREQUENCY_MHZ);
#endif
    
#if TEST_MODE
#if ENABLE_SERIAL_OUTPUT
    Serial.println("TEST MODE ENABLED - Entering test mode");
#endif
    runTestMode();
#else
#if ENABLE_SERIAL_OUTPUT
    Serial.println("NORMAL MODE - Initializing LED tasks");
#endif
    
    // Check if any LEDs use smooth blinking
    bool hasSmoothBlinking = false;
    for (int i = 0; i < NUM_LEDS; i++) {
        if (ledParams[i].smoothBlinking == 1) {
            hasSmoothBlinking = true;
            break;
        }
    }
    
    if (hasSmoothBlinking) {
        initializePwmPins(ledParams, NUM_LEDS);
    } else {
        initializeGpioPins(ledParams, NUM_LEDS);
    }

    // Create separate FreeRTOS task for each LED
    TaskHandle_t ledTaskHandles[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        char taskName[20];
        sprintf(taskName, "LED%d", i);
        xTaskCreate(
            ledBlinkTask, 
            taskName,
            1000,  // Stack size
            &ledParams[i], 
            1,      // Priority
            &ledTaskHandles[i]    // Task handle
        );
        
#if ENABLE_MEMORY_PROFILING
        // Register task with memory profiler
        registerTaskForProfiling(ledTaskHandles[i], taskName, 1000);
#endif
    }
#endif
}

// -----------------------------------------------------------------------------
// MAIN LOOP
// -----------------------------------------------------------------------------
void loop() {
#if TEST_MODE
    // Test mode handles its own execution in runTestMode()
    delay(1000); // Prevent watchdog timeout
#else
    // Empty loop - FreeRTOS tasks handle all LED control in normal mode
#endif
}