#include "led_blink_task.hpp"
#include "blink_helpers.hpp"
#include <Arduino.h>

// Main LED blink task
void ledBlinkTask(void *pvParameters) {
    LedTaskParams* params = (LedTaskParams*)pvParameters;
    
    // Initialize random seed for this task
    randomSeed(millis() + params->pin);
    
    // Get PWM channel for this LED
    int channel = getPwmChannel(params->pin);
    
    while (true) {
        if (params->smoothBlinking == 1) {
            handleSmoothBlinking(params, channel);
        } else {
            handleDigitalBlinking(params);
        }
    }
}
