#ifndef BLINK_HELPERS_HPP
#define BLINK_HELPERS_HPP

#include "led_blink_task.hpp"

// =============================================================================
// LED BLINKING HELPER FUNCTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// PWM FADE EFFECTS
// -----------------------------------------------------------------------------
// Smooth fade in effect for PWM channels
void fadeIn(int channel, int steps, int stepDelay, int maxBrightness);

// Smooth fade out effect for PWM channels
void fadeOut(int channel, int steps, int stepDelay, int maxBrightness);

// -----------------------------------------------------------------------------
// LED CONTROL UTILITIES
// -----------------------------------------------------------------------------
// Get PWM channel number for a given GPIO pin
int getPwmChannel(int pin);

// Calculate delay with volatility adjustments
int calculateDelay(LedTaskParams* params);

// Handle smooth blinking mode with fade effects
void handleSmoothBlinking(LedTaskParams* params, int channel);

// Helper function prototypes
void initializeGpioPins(LedTaskParams* params, int count);
void initializePwmPins(LedTaskParams* params, int count);

// Handle digital blinking mode (on/off without fade)
void handleDigitalBlinking(LedTaskParams* params);

// Handle solid LED mode (always on, no blinking)
void handleSolidLED(LedTaskParams* params, int channel);

#endif // BLINK_HELPERS_HPP
