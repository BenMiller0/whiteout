#include "blink_helpers.hpp"
#include "led_blink_task.hpp"
#include <Arduino.h>

// Helper function for smooth fade in
void fadeIn(int channel, int steps, int stepDelay) {
    for (int i = 0; i <= steps; i++) {
        int duty = (i * 255) / steps;  // Convert step to PWM duty cycle
        ledcWrite(channel, duty);
        vTaskDelay(stepDelay / portTICK_PERIOD_MS);
    }
}

// Helper function for smooth fade out
void fadeOut(int channel, int steps, int stepDelay) {
    for (int i = steps; i >= 0; i--) {
        int duty = (i * 255) / steps;  // Convert step to PWM duty cycle
        ledcWrite(channel, duty);
        vTaskDelay(stepDelay / portTICK_PERIOD_MS);
    }
}

// Get PWM channel based on pin number
int getPwmChannel(int pin) {
    if (pin == L_BELT_RED) return 0;
    else if (pin == L_BELT_GREEN_0) return 1;
    else if (pin == L_BELT_GREEN_1) return 2;
    else if (pin == R_BELT_RED) return 3;
    else if (pin == R_BELT_GREEN_0) return 4;
    else if (pin == R_BELT_GREEN_1) return 5;
    return 0; // Default fallback
}

// Calculate delay based on volatility settings
int calculateDelay(LedTaskParams* params) {
    if (params->volatileBlinking == 1) {
        int baseDelay = params->delay; // Use full delay, not half
        int randomRange = (int)(baseDelay * params->volatilityMultiplier);
        return max(100, baseDelay + (int)random(-randomRange, randomRange));
    } else {
        return params->delay;
    }
}

// Handle smooth blinking mode with fade effects
void handleSmoothBlinking(LedTaskParams* params, int channel) {
    // Fade in
    fadeIn(channel, FADE_STEPS, FADE_DELAY);
    
    // Delay between fade cycles
    int cycleDelay = calculateDelay(params);
    vTaskDelay(cycleDelay / portTICK_PERIOD_MS);
    
    // Fade out
    fadeOut(channel, FADE_STEPS, FADE_DELAY);
    
    // Delay after fade out
    int offDelay = calculateDelay(params);
    vTaskDelay(offDelay / portTICK_PERIOD_MS);
}

// Handle digital blinking mode (on/off without fade)
void handleDigitalBlinking(LedTaskParams* params) {
    // Turn LED on
    digitalWrite(params->pin, HIGH);
    
    // On delay
    int onDelay = calculateDelay(params);
    vTaskDelay(onDelay / portTICK_PERIOD_MS);
    
    // Turn LED off
    digitalWrite(params->pin, LOW);
    
    // Off delay
    int offDelay = calculateDelay(params);
    vTaskDelay(offDelay / portTICK_PERIOD_MS);
}
