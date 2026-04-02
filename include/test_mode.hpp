#ifndef TEST_MODE_HPP
#define TEST_MODE_HPP

#include "constants.hpp"
#include "led_blink_task.hpp"

// Test mode configurations
#define TEST_MODE_DURATION_MS     5000  // Duration for each mode test (5 seconds)
#define TEST_MODE_SWITCH_DELAY   1000  // Delay between mode switches (1 second)

// Function declarations
void runTestMode();
void printTestModeStatus(const char* modeName, bool volatileEnabled, bool smoothEnabled);
void testDigitalVolatileMode();
void testDigitalNonVolatileMode();
void testSmoothVolatileMode();
void testSmoothNonVolatileMode();

#endif
