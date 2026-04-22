#include "led_blink_task.hpp"
#include "blink_helpers.hpp"
#include "normal_mode.hpp"
#include <Arduino.h>

// =============================================================================
// LED BLINK TASK - MAIN LED CONTROL TASK
// =============================================================================

// -----------------------------------------------------------------------------
// MAIN LED BLINK TASK
// -----------------------------------------------------------------------------
// This function runs as a FreeRTOS task for each LED.
// It handles the main blinking logic and delegates to helper functions.
void ledBlinkTask(void* pvParameters) {
    LedTaskParams* params = static_cast<LedTaskParams*>(pvParameters);
    
    // Initialize random seed for this task using pin number for uniqueness
    randomSeed(millis() + params->pin);
    
    // Get PWM channel for this LED
    int channel = getPwmChannel(params->pin);
    
    // Check if this LED should be solid (volatility = 0.0)
    // This applies to both NORMAL_MODE and other modes
    if (params->volatilityMultiplier == 0.0f) {
        handleSolidLED(params, channel);
        return; // This will never return due to infinite loop in handleSolidLED
    }
    
#if NORMAL_MODE
    // In normal mode, check if this is a red belt LED
    if (isNormalModeRedLED(params->pin)) {
        // Red LEDs blink on for ~10s, off for 1s randomly
        while (true) {
            handleNormalModeRedLED(params);
        }
    }
    // Check if this is a chest red LED for random blinking
    if (isNormalModeChestLED(params->pin)) {
        // Chest red LEDs randomly blink on for 15s +/- 10s
        while (true) {
            handleChestRedLED(params);
        }
    }
#endif
    
    // Main blinking loop for non-normal modes
    while (true) {
        if (params->smoothBlinking == 1) {
            handleSmoothBlinking(params, channel);
        } else {
            handleDigitalBlinking(params);
        }
    }
}
