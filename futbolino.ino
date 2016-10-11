// Screen includes
#include <LiquidCrystal.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include <stdio.h>
#include <QueueArray.h>

#include "my_structs.h"
#include "futbolino_config.h"
#include "futbolino_core.h"
#include "screen_wrapper.h"

// Screen declaration
MD_Parola *screen = NULL;
LiquidCrystal* lcd = NULL;
ScreenWrapper* screenWrapper = NULL;

// Apps declaration
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

#ifdef FUTBOLINO_SCREEN_MATRIX
  screen = new MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
  screen->begin(TOTAL_SCREENS);
  screen->setInvert(false);
  screen->setZone(0, FIRST_SCREEN_START, FIRST_SCREEN_END);
  screen->setZone(1, SECOND_SCREEN_START, SECOND_SCREEN_END);

#else
#ifdef FUTBOLINO_SCREEN_LCD
  lcd = new LiquidCrystal(12, 11, 5, 4, 3, 2);
  lcd->begin(16, 2);
  screenWrapper = new LCDWrapper(lcd);
#else
#ifdef FUTBOLINO_SCREEN_SERIAL
  screenWrapper = new SerialPrintlnWrapper(&Serial);
#else
#endif
#endif
#endif

  futbolino = new Futbolino(in, screenWrapper);
  futbolino->begin();
}

void loop() {
  futbolino->loop();
}
