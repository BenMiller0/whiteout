#include <Arduino.h>
#include "constants.hpp"
#include "led_blink_task.hpp"

#if TEST_MODE
#include "test_mode.hpp"
#endif

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
    Serial.begin(115200);
    delay(1000); // Wait for serial to be ready
    
    Serial.println("LED Blink Controller Starting...");
    
#if TEST_MODE
    Serial.println("TEST MODE ENABLED - Entering test mode");
    runTestMode();
#else
    Serial.println("NORMAL MODE - Initializing LED tasks");
    
    // Initialize GPIO pins based on blinking mode
    for (int i = 0; i < 6; i++) {
        if (ledParams[i].smoothBlinking == 1) {
            // Set up PWM channel for smooth blinking
            ledcSetup(i, PWM_FREQUENCY, PWM_RESOLUTION);
            ledcAttachPin(ledParams[i].pin, i);
            ledcWrite(i, 0); // Start with LED off
        } else {
            // Set up as digital output for digital blinking
            pinMode(ledParams[i].pin, OUTPUT);
            digitalWrite(ledParams[i].pin, LOW); // Start with LED off
        }
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
#endif
}


void loop() {
#if TEST_MODE
    // Test mode handles its own execution in runTestMode()
    delay(1000); // Prevent watchdog timeout
#else
    // Empty, FreeRTOS threads take over in normal mode
#endif
}