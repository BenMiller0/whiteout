#ifndef LED_BLINK_TASK_HPP
#define LED_BLINK_TASK_HPP

#include "constants.hpp"

// This is the struct that will be passed in as the params to each thread created for each LED
struct LedTaskParams {
    int pin;
    int delay;
    int volatileBlinking;
    int smoothBlinking;
    float volatilityMultiplier;
};

// Definition of thread that is created for each LED
void ledBlinkTask(void *pvParameters);

#endif
