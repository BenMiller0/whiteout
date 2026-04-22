#include "normal_mode.hpp"
#include "blink_helpers.hpp"
#include "constants.hpp"
#include <Arduino.h>

// =============================================================================
// NORMAL MODE - DARTH VADER SUIT BEHAVIOR
// =============================================================================

// -----------------------------------------------------------------------------
// NORMAL MODE LED BEHAVIORS
// -----------------------------------------------------------------------------

// Handle red belt LED behavior in normal mode
// Pattern: ON for ~10 seconds (with +/- 3s random variation), OFF for 1 second
// Used for: L_BELT_RED, R_BELT_RED
void handleNormalModeRedLED(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    // Turn LED on at configured brightness
    ledcWrite(channel, params->brightness);
    
    // Calculate on time with random variation (10s +/- 3s)
    int onTime = RED_LED_BASE_ON_TIME + random(-RED_LED_RANDOM_RANGE, RED_LED_RANDOM_RANGE);
    vTaskDelay(onTime / portTICK_PERIOD_MS);
    
    // Turn LED off
    ledcWrite(channel, 0);
    
    // Keep off for 1 second
    vTaskDelay(RED_LED_OFF_TIME / portTICK_PERIOD_MS);
}

// Handle chest red LED behavior in normal mode
// Pattern: OFF by default, randomly blink ON for 15s +/- 10s (5-25s range), then OFF for 1s
// Used for: CHEST_RED_1, CHEST_RED_2, CHEST_RED_3
void handleChestRedLED(LedTaskParams* params) {
    int channel = getPwmChannel(params->pin);
    
    // Keep LED off initially
    ledcWrite(channel, 0);
    
    // Calculate off time with random variation (15s +/- 10s = 5-25s range)
    int offTime = CHEST_RED_BASE_OFF_TIME + random(-CHEST_RED_RANDOM_RANGE, CHEST_RED_RANDOM_RANGE);
    offTime = max(5000, offTime);  // Minimum 5 seconds
    vTaskDelay(offTime / portTICK_PERIOD_MS);
    
    // Turn LED on at configured brightness
    ledcWrite(channel, params->brightness);
    
    // Keep on for 1 second
    vTaskDelay(CHEST_RED_ON_TIME / portTICK_PERIOD_MS);
    
    // Turn LED off
    ledcWrite(channel, 0);
}

// Check if a given pin should use normal mode red LED behavior
bool isNormalModeRedLED(int pin) {
    return (pin == L_BELT_RED || pin == R_BELT_RED);
}

// Check if a given pin should use normal mode rare blinking behavior
bool isNormalModeChestLED(int pin) {
    return (pin == CHEST_RED_1 || pin == CHEST_RED_2 || pin == CHEST_RED_3);
}
