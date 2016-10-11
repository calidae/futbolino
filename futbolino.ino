// Screen includes
#include <LiquidCrystal.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <stdio.h>
#include <QueueArray.h>

#include "my_structs.h"
#include "screen_wrapper.h"
#include "futbolino_config.h"
#include "futbolino_core.h"

ScreenWrapper* screenWrapper = NULL;
Futbolino* futbolino = NULL;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  // Input setup
  pinMode(INPUT_PINS.a_plus,  INPUT);
  pinMode(INPUT_PINS.a_minus, INPUT);
  pinMode(INPUT_PINS.b_plus,  INPUT);
  pinMode(INPUT_PINS.b_minus, INPUT);
  pinMode(INPUT_PINS.a_ir,    INPUT);
  pinMode(INPUT_PINS.b_ir,    INPUT);

  screenWrapper = setupScreen();
  futbolino = new Futbolino(INPUT_PINS, screenWrapper);
  futbolino->begin();
}

void loop() {
  futbolino->loop();
}
