# Darth Vader Suit Firmware
Built on FreeRTOS. For Star Wars Club @ UC San Diego's Screen accurate Darth Vader Suit used for club promotion and film making. This firmware controls the blinking of each light on his suit. Makes them easily programmable and optimizes CPU usage to conserve power and boost performance.

## Quick Start

### Normal Operation
Edit `include/constants.hpp`. The important constants to highlight:

```cpp
#define NORMAL_MODE             1    // Normal breathing mode (0 = volatile mode)
#define SMOOTH_BLINKING         0    // PWM fading (0 = digital)
#define VOLATILE_BLINKING       0    // Random timing (0 = steady)
```

### Test Mode
If this flag is set to 1 it will cycle through each combination of modes for testing:
```cpp
#define TEST_MODE              1    // Cycle through all modes
```

### Power Management
For battery operation, configure these settings:
```cpp
#define ENABLE_SERIAL_OUTPUT           0     // Disable Serial for power savings
#define ENABLE_MEMORY_PROFILING        0     // Disable memory profiler
#define DISABLE_WIFI                   1     // Disable WiFi
#define CPU_FREQUENCY_MHZ              80    // Lower CPU frequency (80MHz instead of 240MHz)
#define ENABLE_LIGHT_SLEEP             1     // Enable light sleep during LED off periods
```

## Configuration

### LED Pins
- L_BELT_RED: Pin 13
- L_BELT_GREEN_0: Pin 27  
- L_BELT_GREEN_1: Pin 26
- R_BELT_RED: Pin 25
- R_BELT_GREEN_0: Pin 33
- R_BELT_GREEN_1: Pin 32
- CHEST_RED_1: Pin 4
- CHEST_RED_2: Pin 22
- CHEST_RED_3: Pin 23

### Timing
```cpp
// Base delays (ms)
#define L_BELT_RED_DELAY        1000

// Normal mode red LED timing
#define RED_LED_BASE_ON_TIME    10000  // Base on time for red LEDs (10 seconds)
#define RED_LED_OFF_TIME        1000   // Off time for red LEDs (1 second)
#define RED_LED_RANDOM_RANGE    3000   // Random variation range (+/- 3 seconds)

// Normal mode chest red LED timing
#define CHEST_RED_BASE_OFF_TIME 15000  // Base off time for chest red LEDs (15 seconds)
#define CHEST_RED_ON_TIME       1000   // On time for chest red LEDs (1 second)
#define CHEST_RED_RANDOM_RANGE  10000  // Random variation range (+/- 10 seconds)

// Volatility multipliers (0.0-1.0)
#define L_BELT_RED_VOLATILITY   0.1

// PWM settings (smooth mode)
#define FADE_STEPS              50
#define FADE_DELAY              10
#define PWM_FREQUENCY           5000
#define PWM_RESOLUTION          8
```

### Brightness
Each LED has individual brightness control (0-255 PWM range):
```cpp
#define L_BELT_RED_BRIGHTNESS          10
#define L_BELT_GREEN_0_BRIGHTNESS      100
// ... etc for all LEDs
```

## Build & Upload

### Requirements
- ESP32 development board
- PlatformIO extension for VS Code
- USB cable for programming

### Instructions
1. Open the project in VS Code
2. Use the PlatformIO extension to build and upload to ESP32
3. Monitor serial output at 115200 baud (if enabled)

### PlatformIO Configuration
The project is configured for ESP32 with:
- CPU frequency: 80MHz (configurable for power savings)
- PSRAM enabled
- USB CDC disabled on boot (for power savings)

## Project Structure

```
whiteout/
├── include/
│   ├── constants.hpp          # Configuration constants
│   ├── led_blink_task.hpp     # LED task interface
│   ├── blink_helpers.hpp      # PWM and helper functions
│   ├── normal_mode.hpp        # Normal mode declarations
│   ├── test_mode.hpp          # Test mode declarations
│   └── memory_profiler.hpp    # Memory profiling utilities
├── src/
│   ├── main.cpp               # Setup & task creation
│   ├── led_blink_task.cpp     # LED task implementation
│   ├── blink_helpers.cpp      # PWM and helper implementation
│   ├── normal_mode.cpp        # Normal mode implementation
│   ├── test_mode.cpp          # Test mode implementation
│   └── memory_profiler.cpp    # Memory profiling implementation
├── platformio.ini             # PlatformIO configuration
└── README.md
```

## Modes

### Normal Mode
Realistic breathing pattern for Darth Vader suit:
- **Belt red LEDs**: Long on time (~10s) with short off time (~1s), low volatility
- **Chest red LEDs**: Long off time (~15s) with short on time (~1s), low volatility
- **Green LEDs**: Steady on (no blinking in normal mode)

### Volatile Mode
Random timing patterns with per-LED volatility control:
- **Digital + Non-Volatile**: Steady on/off blinking
- **Digital + Volatile**: Random on/off blinking
- **Smooth + Non-Volatile**: Steady fade in/out
- **Smooth + Volatile**: Random fade in/out

Test mode cycles through all 4 volatile modes automatically (5s each).
