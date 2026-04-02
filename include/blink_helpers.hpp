#ifndef BLINK_HELPERS_HPP
#define BLINK_HELPERS_HPP

#include "led_blink_task.hpp"

// Helper function prototypes
void fadeIn(int channel, int steps, int stepDelay);
void fadeOut(int channel, int steps, int stepDelay);
int getPwmChannel(int pin);
int calculateDelay(LedTaskParams* params);
void handleSmoothBlinking(LedTaskParams* params, int channel);
void handleDigitalBlinking(LedTaskParams* params);

#endif
