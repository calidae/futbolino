// Screen includes
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <QueueArray.h>


#include <stdio.h>
#include "my_structs.h"
#include "futbolino_core.h"
#include "screen_wrapper.h"

#define TOTAL_SCREENS 2
#define FIRST_SCREEN_START 0
#define FIRST_SCREEN_END 3
#define SECOND_SCREEN_START 4
#define SECOND_SCREEN_END 7

// Input Pins
const Inputs in = {A0, A1, A2, A3, A4, A5};

// Screen pins
const int MAX_DEVICES = 8;
const int CLK_PIN = 12;
const int DATA_PIN = 11;
const int CS_PIN = 10;

// Screen declaration
MD_Parola *screen = NULL;
ScreenWrapper* screenWrapper = NULL;

// Apps declaration


Futbolino* futbolino = NULL;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("go!");
Serial.println(A0);
Serial.println(A1);
Serial.println(A2);
Serial.println(A3);
  screen = new MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
  screenWrapper = new SerialPrintlnWrapper(&Serial);

  futbolino = new Futbolino(in, screenWrapper);

  // Serial debug
  Serial.begin(9600);

  // Input setup
  pinMode(in.PIN_TEAM_A_PLUS,  INPUT);
  pinMode(in.PIN_TEAM_A_MINUS, INPUT);
  pinMode(in.PIN_TEAM_B_PLUS,  INPUT);
  pinMode(in.PIN_TEAM_B_MINUS, INPUT);

  // IR SENSORS ARE TEMPORALLY DISABLED
  // pinMode(in.PIN_IR_A,         INPUT);
  // pinMode(in.PIN_IR_B,         INPUT);

  // Screen setup
  screen->begin(TOTAL_SCREENS);
  screen->setInvert(false);
  screen->setZone(0, FIRST_SCREEN_START, FIRST_SCREEN_END);
  screen->setZone(1, SECOND_SCREEN_START, SECOND_SCREEN_END);

  // Apps setup
  futbolino->begin();
}

void loop() {
  futbolino->loop();
}
