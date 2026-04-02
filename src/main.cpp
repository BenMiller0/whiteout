#include <Arduino.h>
#include "constants.hpp"
#include "led_blink_task.hpp"

// Parameter array. Includes pin # and blink delay
static LedTaskParams ledParams[6] = {
    {  L_BELT_RED,        L_BELT_RED_DELAY,       VOLATILE_BLINKING,   SMOOTH_BLINKING,   L_BELT_RED_VOLATILITY        },
    {  L_BELT_GREEN_0,    L_BELT_GREEN_0_DELAY,   VOLATILE_BLINKING,   SMOOTH_BLINKING,   L_BELT_GREEN_0_VOLATILITY    },
    {  L_BELT_GREEN_1,    L_BELT_GREEN_1_DELAY,   VOLATILE_BLINKING,   SMOOTH_BLINKING,   L_BELT_GREEN_1_VOLATILITY    },
    {  R_BELT_RED,        R_BELT_RED_DELAY,       VOLATILE_BLINKING,   SMOOTH_BLINKING,   R_BELT_RED_VOLATILITY        },
    {  R_BELT_GREEN_0,    R_BELT_GREEN_0_DELAY,   VOLATILE_BLINKING,   SMOOTH_BLINKING,   R_BELT_GREEN_0_VOLATILITY    }, 
    {  R_BELT_GREEN_1,    R_BELT_GREEN_1_DELAY,   VOLATILE_BLINKING,   SMOOTH_BLINKING,   R_BELT_GREEN_1_VOLATILITY    },  
};


void setup() {
    // Initialize PWM channels for smooth blinking
    for (int i = 0; i < 6; i++) {
        ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
        ledcAttachPin(ledParams[i].pin, i);
        ledcWrite(i, 0); // Start with LED off
    }

    // Create separate thread for each LED
    for (int i = 0; i < 6; i++) {
        char taskName[20];
        sprintf(taskName, "LED%d", i);
        xTaskCreate(
            ledBlinkTask, 
            taskName,
            1000,
            &ledParams[i], 
            1,
            NULL
        );
    }
}


void loop() {} // Empty, FreeRTOS threads take over