#ifndef TEST_MODE_HPP
#define TEST_MODE_HPP

#include "constants.hpp"
#include "led_blink_task.hpp"

// =============================================================================
// TEST MODE - TESTING FRAMEWORK
// =============================================================================

// -----------------------------------------------------------------------------
// TEST MODE CONFIGURATION
// -----------------------------------------------------------------------------
#define TEST_MODE_DURATION_MS     5000  // Duration for each mode test (5 seconds)
#define TEST_MODE_SWITCH_DELAY   1000  // Delay between mode switches (1 second)

// -----------------------------------------------------------------------------
// FUNCTION DECLARATIONS
// -----------------------------------------------------------------------------
// Main test mode runner - cycles through all blinking modes
void runTestMode();

// Print current test mode status and LED configuration
void printTestModeStatus(const char* modeName, bool volatileEnabled, bool smoothEnabled);

// Test digital blinking with volatile timing
void testDigitalVolatileMode();

// Test digital blinking with non-volatile timing
void testDigitalNonVolatileMode();

// Test smooth PWM blinking with volatile timing
void testSmoothVolatileMode();

// Test smooth PWM blinking with non-volatile timing
void testSmoothNonVolatileMode();

#endif // TEST_MODE_HPP
