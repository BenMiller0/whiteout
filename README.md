# Whiteout LED Controller

**Firmware for ESP32** - Multi-threaded LED controller with smooth fading and volatile random blinking.

This firmware runs directly on ESP32 microcontrollers to control 6 physical LEDs with advanced lighting effects.

## Features

- **6 Independent LEDs**: Each with dedicated FreeRTOS task
- **Smooth Fading**: PWM-based fade in/out transitions
- **Volatile Random Mode**: Chaotic timing with per-LED volatility control
- **Modular Design**: Clean, maintainable architecture

## Quick Start

### Normal Smooth Blinking (Default)
```cpp
#define SMOOTH_BLINKING         1
#define VOLATILE_BLINKING       0
```

### Volatile Random Mode
```cpp
#define SMOOTH_BLINKING         1
#define VOLATILE_BLINKING       1
```

### Digital Blinking (No Fade)
```cpp
#define SMOOTH_BLINKING         0
```

## Configuration

### Pin Mapping
- **L_BELT_RED**: Pin 13
- **L_BELT_GREEN_0**: Pin 27
- **L_BELT_GREEN_1**: Pin 26
- **R_BELT_RED**: Pin 25
- **R_BELT_GREEN_0**: Pin 33
- **R_BELT_GREEN_1**: Pin 32

### Timing Settings
```cpp
// Base delay for each LED (ms)
#define L_BELT_RED_DELAY        1000

// Volatility multipliers (0.5-2.0 recommended)
#define L_BELT_RED_VOLATILITY          1.0  // Standard
#define L_BELT_GREEN_0_VOLATILITY      0.8  // Less volatile
#define L_BELT_GREEN_1_VOLATILITY      1.2  // More volatile

// Smooth fading settings
#define FADE_STEPS              50    // Fade smoothness
#define FADE_DELAY              10    // Speed between steps
```

## How It Works

- **Smooth Mode**: Uses PWM for gradual fade in/out
- **Volatile Mode**: Adds random timing based on volatility multipliers
- **Digital Mode**: Simple on/off blinking
- **All modes work together**: Combine smooth + volatile for chaotic fading

## Build & Upload

```bash
pio run                    # Build
pio run --target upload    # Upload
pio device monitor         # Serial monitor
```

## Project Structure

```
whiteout/
├── include/
│   ├── constants.hpp          # Configuration
│   └── led_blink_task.hpp     # LED task interface
├── src/
│   ├── main.cpp               # Setup & task creation
│   └── led_blink_task.cpp     # LED implementation
└── README.md
```

## Technical Details

- **Platform**: ESP32 with Arduino framework
- **RTOS**: FreeRTOS with 6 concurrent tasks
- **PWM**: 8-bit resolution, 5kHz frequency
- **Memory**: ~1000 bytes per task stack
