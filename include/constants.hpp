#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

// =============================================================================
// LED CONTROLLER - CONSTANTS
// =============================================================================

// -----------------------------------------------------------------------------
// GPIO PIN DEFINITIONS
// -----------------------------------------------------------------------------
// LED Layout: 9 total LEDs organized as follows:
// - Left Belt: 1 Red, 2 Green (3 LEDs)
// - Right Belt: 1 Red, 2 Green (3 LEDs)
// - Chest: 3 Red LEDs (3 LEDs)

// Left belt LEDs
#define L_BELT_RED              13
#define L_BELT_GREEN_0          27
#define L_BELT_GREEN_1          26

// Right belt LEDs
#define R_BELT_RED              25
#define R_BELT_GREEN_0          33
#define R_BELT_GREEN_1          32

// Chest LEDs
#define CHEST_RED_1             4
#define CHEST_RED_2             22
#define CHEST_RED_3             23

// Touch Sensor Pins
#define TOUCH_BRIGHTNESS_PIN    14    // GPIO 14 (T6) for brightness control

// -----------------------------------------------------------------------------
// BLINK TIMING CONFIGURATION
// -----------------------------------------------------------------------------
#define L_BELT_RED_DELAY        1000
#define L_BELT_GREEN_0_DELAY    1000
#define L_BELT_GREEN_1_DELAY    1000
#define R_BELT_RED_DELAY        1000
#define R_BELT_GREEN_0_DELAY    1000
#define R_BELT_GREEN_1_DELAY    1000
#define CHEST_RED_1_DELAY       1000
#define CHEST_RED_2_DELAY       1000
#define CHEST_RED_3_DELAY       1000

// -----------------------------------------------------------------------------
// NORMAL MODE RED LED CONFIGURATION
// -----------------------------------------------------------------------------
#define RED_LED_BASE_ON_TIME    10000  // Base on time for red LEDs (10 seconds)
#define RED_LED_OFF_TIME        1000   // Off time for red LEDs (1 second)
#define RED_LED_RANDOM_RANGE    3000   // Random variation range (+/- 3 seconds)

// -----------------------------------------------------------------------------
// NORMAL MODE CHEST RED LED CONFIGURATION
// -----------------------------------------------------------------------------
#define CHEST_RED_BASE_OFF_TIME 15000  // Base off time for chest red LEDs (15 seconds)
#define CHEST_RED_ON_TIME       1000   // On time for chest red LEDs (1 second)
#define CHEST_RED_RANDOM_RANGE  10000  // Random variation range (+/- 10 seconds)

// -----------------------------------------------------------------------------
// OPERATIONAL MODES
// -----------------------------------------------------------------------------
#define TEST_MODE               0
#define NORMAL_MODE             1
#define VOLATILE_BLINKING       0
#define SMOOTH_BLINKING         0

// -----------------------------------------------------------------------------
// SYSTEM CONFIGURATION
// -----------------------------------------------------------------------------
#define NUM_LEDS               9     // Total number of LEDs in the system

// -----------------------------------------------------------------------------
// PWM SMOOTH BLINKING CONFIGURATION
// -----------------------------------------------------------------------------
#define FADE_STEPS              50    // Number of steps for fade in/out
#define FADE_DELAY              10    // Delay between fade steps (ms)
#define PWM_FREQUENCY           5000  // PWM frequency in Hz
#define PWM_RESOLUTION          8     // PWM resolution (8 bits = 0-255)

// -----------------------------------------------------------------------------
// VOLATILITY MULTIPLIERS
// -----------------------------------------------------------------------------
// Higher values = more volatile/random blinking
#define L_BELT_RED_VOLATILITY          0.1
#define L_BELT_GREEN_0_VOLATILITY      0.0
#define L_BELT_GREEN_1_VOLATILITY      0.0
#define R_BELT_RED_VOLATILITY          0.9
#define R_BELT_GREEN_0_VOLATILITY      0.0
#define R_BELT_GREEN_1_VOLATILITY      0.0
#define CHEST_RED_1_VOLATILITY         0.05
#define CHEST_RED_2_VOLATILITY         0.05
#define CHEST_RED_3_VOLATILITY         0.05

// -----------------------------------------------------------------------------
// LED BRIGHTNESS CONFIGURATION
// -----------------------------------------------------------------------------
// Brightness values for each LED (0-255 PWM range)
// 255 = maximum brightness, 0 = off
#define L_BELT_RED_BRIGHTNESS          100
#define L_BELT_GREEN_0_BRIGHTNESS      150
#define L_BELT_GREEN_1_BRIGHTNESS      150
#define R_BELT_RED_BRIGHTNESS          100
#define R_BELT_GREEN_0_BRIGHTNESS      100
#define R_BELT_GREEN_1_BRIGHTNESS      100
#define CHEST_RED_1_BRIGHTNESS         100
#define CHEST_RED_2_BRIGHTNESS         100
#define CHEST_RED_3_BRIGHTNESS         100

// -----------------------------------------------------------------------------
// POWER MANAGEMENT CONFIGURATION (BATTERY MODE)
// -----------------------------------------------------------------------------
#define ENABLE_SERIAL_OUTPUT           0     // Enable Serial for brightness feedback
#define ENABLE_MEMORY_PROFILING        0     // Disable memory profiler in normal mode
#define DISABLE_WIFI                   1     // Disable WiFi for power savings
#define CPU_FREQUENCY_MHZ              80    // Lower CPU frequency (80MHz instead of 240MHz)
#define ENABLE_LIGHT_SLEEP             1     // Enable light sleep during LED off periods

#endif // CONSTANTS_HPP