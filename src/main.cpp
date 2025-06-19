#include <MD_MAX72xx.h>
#include <SPI.h>
#include "game.h"

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4       // 4 matrices = 32 cols x 8 rows
#define CLK_PIN     13      // SPI CLK
#define DATA_PIN    11      // SPI DATAP
#define CS_PIN      10      // SPI CS
#define BUTTON_PIN  2       // Button on pin 2 (interrupt pin)

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Interrupt variables
volatile bool buttonPressed = false;
volatile unsigned long lastInterruptTime = 0;

// Interrupt Service Routine
void buttonISR() {
  unsigned long interruptTime = millis();
  // Debounce check (ignore if less than 50ms since last interrupt)
  if (interruptTime - lastInterruptTime > 50) {
    buttonPressed = true;
  }
  lastInterruptTime = interruptTime;
}



void setup() {
  Serial.begin(9600);
  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, 8);  // Brightness (0-15)
  pinMode(BUTTON_PIN, INPUT_PULLUP);       // Use internal pull-up resistor
  
  // Attach interrupt (FALLING = HIGH to LOW transition)
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
  
  randomSeed(analogRead(0));
}

void loop() {
  if (gameRunning) {
    Serial.print("test");
    moveBlock();
    handleButtonPress(); // Handle interrupt-based button press
    drawGame();
    delay(speed);
  } else {
    gameOver();
  }
}
