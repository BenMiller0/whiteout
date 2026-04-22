#include "blink_helpers.hpp"
#include <Arduino.h>

// =============================================================================
// LED BLINKING HELPER FUNCTIONS
// =============================================================================

// -----------------------------------------------------------------------------
// PWM FADE EFFECTS
// -----------------------------------------------------------------------------
// Smooth fade in effect for PWM channels
// Increases LED brightness from 0 to configured brightness over specified steps
void fadeIn(int channel, int steps, int stepDelay, int maxBrightness) {
    for (int i = 0; i <= steps; i++) {
        int duty = (i * maxBrightness) / steps;  // Convert step to PWM duty cycle
        ledcWrite(channel, duty);
        vTaskDelay(stepDelay / portTICK_PERIOD_MS);
    }
}

// Smooth fade out effect for PWM channels
// Decreases LED brightness from configured brightness to 0 over specified steps
void fadeOut(int channel, int steps, int stepDelay, int maxBrightness) {
    for (int i = steps; i >= 0; i--) {
        int duty = (i * maxBrightness) / steps;  // Convert step to PWM duty cycle
        ledcWrite(channel, duty);
        vTaskDelay(stepDelay / portTICK_PERIOD_MS);
    }
}

// -----------------------------------------------------------------------------
// LED CONTROL UTILITIES
// -----------------------------------------------------------------------------
// Get PWM channel number for a given GPIO pin
// Returns a deterministic channel number (0-9) for each LED pin
int getPwmChannel(int pin) {
    switch (pin) {
        case L_BELT_RED:        return 0;
        case L_BELT_GREEN_0:    return 1;
        case L_BELT_GREEN_1:    return 2;
        case R_BELT_RED:        return 3;
        case R_BELT_GREEN_0:    return 4;
        case R_BELT_GREEN_1:    return 5;
        case CHEST_RED_1:       return 6;
        case CHEST_RED_2:       return 7;
        case CHEST_RED_3:       return 8;
        default:                return 0; // Default fallback
    }
}

// Calculate delay with volatility adjustments
// If volatile blinking is enabled, adds random variation to the base delay
int calculateDelay(LedTaskParams* params) {
    if (params->volatileBlinking == 1) {
        int baseDelay = params->delay;
        int randomRange = static_cast<int>(baseDelay * params->volatilityMultiplier);
        return max(100, baseDelay + static_cast<int>(random(-randomRange, randomRange)));
    } else {
        return params->delay;
    }
}

// Handle smooth blinking mode with fade effects
// Uses PWM to create smooth fade in/out transitions
void handleSmoothBlinking(LedTaskParams* params, int channel) {
    // Fade in
    fadeIn(channel, FADE_STEPS, FADE_DELAY, params->brightness);
    
    // Delay between fade cycles
    int cycleDelay = calculateDelay(params);
    vTaskDelay(cycleDelay / portTICK_PERIOD_MS);
    
    // Fade out
    fadeOut(channel, FADE_STEPS, FADE_DELAY, params->brightness);
    
    // Delay after fade out
    int offDelay = calculateDelay(params);
    vTaskDelay(offDelay / portTICK_PERIOD_MS);
}

// Handle digital blinking mode (on/off without fade)
// Uses PWM for brightness control
void handleDigitalBlinking(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    // Turn LED on at configured brightness
    ledcWrite(channel, params->brightness);
    
    // On delay
    int onDelay = calculateDelay(params);
    vTaskDelay(onDelay / portTICK_PERIOD_MS);
    
    // Turn LED off
    ledcWrite(channel, 0);
    
    // Off delay
    int offDelay = calculateDelay(params);
    vTaskDelay(offDelay / portTICK_PERIOD_MS);
}

// Handle solid LED mode (always on, no blinking)
// Turns LED on at configured brightness and keeps it on indefinitely
void handleSolidLED(LedTaskParams* params, int channel) {
    // Turn LED on at configured brightness and keep it on
    ledcWrite(channel, params->brightness);
    
    // Just wait indefinitely, LED stays solid
    while (true) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Initialize GPIO pins for digital mode
void initializeGpioPins(LedTaskParams* params, int count) {
    for (int i = 0; i < count; i++) {
        pinMode(params[i].pin, OUTPUT);
        digitalWrite(params[i].pin, LOW);
    }
}

// Initialize PWM pins for smooth blinking mode
void initializePwmPins(LedTaskParams* params, int count) {
    for (int i = 0; i < count; i++) {
        int channel = getPwmChannel(params[i].pin);
        ledcSetup(channel, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(params[i].pin, channel);
        ledcWrite(channel, 0);
    }
}
