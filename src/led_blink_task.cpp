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

// Definition of thread that is created for each LED
void ledBlinkTask(void *pvParameters) {
    LedTaskParams* params = (LedTaskParams*)pvParameters;
    
    // Initialize random seed for this task
    randomSeed(millis() + params->pin);
    
    // Determine PWM channel (same as array index for simplicity)
    int channel = 0;
    if (params->pin == L_BELT_RED) channel = 0;
    else if (params->pin == L_BELT_GREEN_0) channel = 1;
    else if (params->pin == L_BELT_GREEN_1) channel = 2;
    else if (params->pin == R_BELT_RED) channel = 3;
    else if (params->pin == R_BELT_GREEN_0) channel = 4;
    else if (params->pin == R_BELT_GREEN_1) channel = 5;
    
    while (true) {
        if (params->smoothBlinking == 1) {
            // Smooth blinking with fade in/out
            fadeIn(channel, FADE_STEPS, FADE_DELAY);
            
            // Calculate delay between fade cycles
            int cycleDelay;
            if (params->volatileBlinking == 1) {
                // Volatile random delay
                int baseDelay = params->delay / 2;
                int randomRange = (int)(baseDelay * params->volatilityMultiplier);
                cycleDelay = max(50, baseDelay + (int)random(-randomRange, randomRange));
            } else {
                // Normal delay
                cycleDelay = params->delay;
            }
            
            vTaskDelay(cycleDelay / portTICK_PERIOD_MS);
            
            fadeOut(channel, FADE_STEPS, FADE_DELAY);
            
        } else {
            // Original digital blinking (no smooth fading)
            digitalWrite(params->pin, HIGH);
            
            if (params->volatileBlinking == 1) {
                // Volatile random blinking: use volatility multiplier with base delay
                int baseDelay = params->delay / 2; // Use half the normal delay as base
                int randomRange = (int)(baseDelay * params->volatilityMultiplier);
                int randomDelay = max(50, baseDelay + (int)random(-randomRange, randomRange));
                vTaskDelay(randomDelay / portTICK_PERIOD_MS);
            } else {
                // Normal blinking
                vTaskDelay(params->delay / portTICK_PERIOD_MS);
            }
            
            digitalWrite(params->pin, LOW);
        }
        
        // Delay after turning off (for both smooth and digital modes)
        int offDelay;
        if (params->volatileBlinking == 1) {
            int baseDelay = params->delay / 2;
            int randomRange = (int)(baseDelay * params->volatilityMultiplier);
            offDelay = max(50, baseDelay + (int)random(-randomRange, randomRange));
        } else {
            offDelay = params->delay;
        }
        
        if (params->smoothBlinking == 1) {
            vTaskDelay(offDelay / portTICK_PERIOD_MS);
        } else {
            vTaskDelay(offDelay / portTICK_PERIOD_MS);
        }
    }
}
