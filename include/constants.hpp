// GPIO Pin Constants, named after the LED location
#define L_BELT_RED              13
#define L_BELT_GREEN_0          27
#define L_BELT_GREEN_1          26
#define R_BELT_RED              25
#define R_BELT_GREEN_0          33
#define R_BELT_GREEN_1          32

// Blink delays
#define L_BELT_RED_DELAY        1000
#define L_BELT_GREEN_0_DELAY    1000
#define L_BELT_GREEN_1_DELAY    1000
#define R_BELT_RED_DELAY        1000
#define R_BELT_GREEN_0_DELAY    1000
#define R_BELT_GREEN_1_DELAY    1000

// Test mode (set to 1 to enable mode cycling test)
#define TEST_MODE              1

// Volatile random blinking mode (set to 1 to enable)
#define VOLATILE_BLINKING       0

// Smooth blinking mode (set to 1 to enable PWM fading)
#define SMOOTH_BLINKING         0

// Smooth blinking configuration
#define FADE_STEPS              50    // Number of steps for fade in/out
#define FADE_DELAY              10    // Delay between fade steps (ms)
#define PWM_FREQUENCY           5000  // PWM frequency in Hz
#define PWM_RESOLUTION          8     // PWM resolution (8 bits = 0-255)

// Volatility multiplier for each LED (higher = more volatile/random)
#define L_BELT_RED_VOLATILITY          1.0
#define L_BELT_GREEN_0_VOLATILITY      0.8
#define L_BELT_GREEN_1_VOLATILITY      1.2
#define R_BELT_RED_VOLATILITY          0.9
#define R_BELT_GREEN_0_VOLATILITY      1.1
#define R_BELT_GREEN_1_VOLATILITY      0.7