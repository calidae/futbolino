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
  pinMode(in.PIN_TEAM_A_PLUS,  INPUT);
  pinMode(in.PIN_TEAM_A_MINUS, INPUT);
  pinMode(in.PIN_TEAM_B_PLUS,  INPUT);
  pinMode(in.PIN_TEAM_B_MINUS, INPUT);
  pinMode(in.PIN_IR_A,         INPUT);
  pinMode(in.PIN_IR_B,         INPUT);

  screenWrapper = setupScreen();
  futbolino = new Futbolino(in, screenWrapper);
  futbolino->begin();
}

void loop() {
  futbolino->loop();
}
