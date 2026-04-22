# Whiteout LED Controller

Darth vadar suit firmware.

## Features

- **9 Independent LEDs** with dedicated FreeRTOS tasks
- **Smooth PWM fading** or digital on/off blinking
- **Volatile random mode** with per-LED volatility control
- **Test mode** for cycling through all configurations

## Quick Start

### Normal Operation
Edit `include/constants.hpp`. The important constants to highlight:

```cpp
#define SMOOTH_BLINKING         1    // PWM fading (0 = digital)
#define VOLATILE_BLINKING       1    // Random timing (0 = steady)
```

### Test Mode
If this flag is set to 1 it will cycle through each combination of modes for testing:
```cpp
#define TEST_MODE              1    // Cycle through all modes
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

// Volatility multipliers (0.5-2.0)
#define L_BELT_RED_VOLATILITY   1.0

// PWM settings (smooth mode)
#define FADE_STEPS              50
#define PWM_FREQUENCY           5000
```

## Build & Upload

Use platformio extention on VS code to build and upload to ESP32.

## Project Structure

```
whiteout/
├── include/
│   ├── constants.hpp          # Configuration
│   ├── led_blink_task.hpp     # LED task interface
│   └── test_mode.hpp          # Test mode declarations
├── src/
│   ├── main.cpp               # Setup & task creation
│   ├── led_blink_task.cpp     # LED implementation
│   └── test_mode.cpp          # Test mode implementation
└── README.md
```

## Modes

1. **Digital + Non-Volatile**: Steady on/off blinking
2. **Digital + Volatile**: Random on/off blinking  
3. **Smooth + Non-Volatile**: Steady fade in/out
4. **Smooth + Volatile**: Random fade in/out

Test mode cycles through all 4 modes automatically (5s each).
