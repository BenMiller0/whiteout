#ifndef NORMAL_MODE_HPP
#define NORMAL_MODE_HPP

#include "led_blink_task.hpp"

// =============================================================================
// NORMAL MODE - DARTH VADER SUIT BEHAVIOR
// =============================================================================
// Normal mode implements the canonical Darth Vader chest box LED behavior:
// - Red belt LEDs: Blink on for ~10 seconds, off for 1 second (with random variation)
// - Green belt LEDs: Solid on (no blinking)
// - Chest red LEDs: Rare blinking (solid most of the time, occasional brief blink)

// -----------------------------------------------------------------------------
// NORMAL MODE LED BEHAVIORS
// -----------------------------------------------------------------------------

// Handle red belt LED behavior in normal mode
// Pattern: ON for ~10 seconds (with +/- 3s random variation), OFF for 1 second
// Used for: L_BELT_RED, R_BELT_RED
void handleNormalModeRedLED(LedTaskParams* params);

// Handle chest red LED behavior in normal mode
// Pattern: OFF by default, randomly blink ON for 15s +/- 10s (5-25s range), then OFF for 1s
// Used for: CHEST_RED_1, CHEST_RED_2, CHEST_RED_3
void handleChestRedLED(LedTaskParams* params);

// Check if a given pin should use normal mode red LED behavior
bool isNormalModeRedLED(int pin);

// Check if a given pin should use normal mode rare blinking behavior
bool isNormalModeChestLED(int pin);

#endif // NORMAL_MODE_HPP
