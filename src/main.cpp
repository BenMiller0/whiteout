#include <Arduino.h>

#define LED_PIN13 13
#define LED_PIN26 26
#define LED_PIN25 25
#define LED_PIN33 33
#define LED_PIN32 32
#define LED_PIN27 27

int LEDS[] = {LED_PIN13, LED_PIN26, LED_PIN25, LED_PIN33, LED_PIN32, LED_PIN27};

void blinkTask(void *pvParameters) {
    while (true) {
        for (int pin : LEDS) {
            digitalWrite(pin, HIGH);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);  //
        for (int pin : LEDS) {
            digitalWrite(pin, LOW);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void setup() {
    for (int pin : LEDS) {
        pinMode(pin, OUTPUT);
    }

    xTaskCreate(
        blinkTask,    // Task function
        "BlinkTask",  // Task name
        1000,         // Stack size
        NULL,         // Parameters
        1,            // Priority
        NULL          // Task handle
    );
}

void loop() {
    // Empty - FreeRTOS takes over
}