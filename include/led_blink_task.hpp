#ifndef LED_BLINK_TASK_HPP
#define LED_BLINK_TASK_HPP

#include "constants.hpp"

// =============================================================================
// LED BLINK TASK - DATA STRUCTURES AND DECLARATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// LED TASK PARAMETERS STRUCTURE
// -----------------------------------------------------------------------------
// This structure contains all parameters needed for each LED task
struct LedTaskParams {
    int pin;                    // GPIO pin number for the LED
    int delay;                  // Base delay in milliseconds
    int volatileBlinking;       // Enable/disable volatile (random) blinking
    int smoothBlinking;         // Enable/disable smooth PWM fading
    float volatilityMultiplier; // Volatility factor for random timing
    int brightness;             // LED brightness (0-255 PWM range)
};

// -----------------------------------------------------------------------------
// FUNCTION DECLARATIONS
// -----------------------------------------------------------------------------
// Main task function for LED blinking - runs as a FreeRTOS task
void ledBlinkTask(void* pvParameters);

#endif // LED_BLINK_TASK_HPP
