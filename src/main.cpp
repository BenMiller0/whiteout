#include <Arduino.h>


// GPIO Pin Constants, named after the LED location
#define L_BELT_RED 13
#define L_BELT_GREEN_0 27
#define L_BELT_GREEN_1 26
#define R_BELT_RED 25
#define R_BELT_GREEN_0 33
#define R_BELT_GREEN_1 32
int LEDS[] = {L_BELT_RED, L_BELT_GREEN_0, L_BELT_GREEN_1, R_BELT_RED, R_BELT_GREEN_0, R_BELT_GREEN_1};


// This is the struct that will be passed in as the params to each thread created for each LED
struct LedTaskParams {
    int pin;
    int delay;
};


// Definition of thread that is created for each LED
void ledBlinkTask(void *pvParameters) {
    LedTaskParams* params = (LedTaskParams*)pvParameters;
    while (true) {
        digitalWrite(params->pin, HIGH);
        vTaskDelay(params->delay / portTICK_PERIOD_MS);
        digitalWrite(params->pin, LOW);
        vTaskDelay(params->delay / portTICK_PERIOD_MS);
    }
}


void setup() {
    // Set each LED GPIO pun as output pin
    for (int pin : LEDS) {
        pinMode(pin, OUTPUT);
    }

    // Parameter arrays (must persist for task lifetime)
    static LedTaskParams ledParams[6] = {
        {L_BELT_RED, 200},
        {L_BELT_GREEN_0, 400},
        {L_BELT_GREEN_1, 600},
        {R_BELT_RED, 800},
        {R_BELT_GREEN_0, 1000}, 
        {R_BELT_GREEN_1, 1200}  
    };

    // Create separate thread for each LED
    for (int i = 0; i < 6; i++) {
        char taskName[20];
        
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


void loop() {
    // Empty, FreeRTOS threads take over
}