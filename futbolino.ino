// Screen includes
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#if USE_LIBRARY_SPI
#include <SPI.h>
#endif

#include <stdio.h>

// Screen config and pins
const int MAX_DEVICES = 4;
const int CLK_PIN = 12;
const int DATA_PIN = 11;
const int CS_PIN = 10;
#define SCREEN_BUF_SIZE 10
MD_Parola SCREEN_A = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
char screenBufferA[SCREEN_BUF_SIZE];

// Data types
typedef struct Buttons Buttons;
struct Buttons {
  bool aPlus;
  bool aMinus;
  bool bPlus;
  bool bMinus;
};

typedef struct Sensors Sensors;
struct Sensors {
  bool irA;
  bool irB;
};

// Function prototypes

// Input Pins
const int PIN_TEAM_A_PLUS  = A0;
const int PIN_TEAM_A_MINUS = A1;
const int PIN_TEAM_B_PLUS  = A2;
const int PIN_TEAM_B_MINUS = A3;
const int PIN_IR_A = A5;
const int PIN_IR_B = A6;

// Constants
const int IR_THRESHOLD = 300;

// Globals
int golsA = 0;
int golsB = 0;
bool debounce_ir_a = false;
bool debounce_ir_b = false;
bool debounce_button_aPlus = false;
bool debounce_button_aMinus = false;
bool debounce_button_bPlus = false;
bool debounce_button_bMinus = false;


void setup() {
  Serial.begin(9600);
  pinMode(PIN_TEAM_A_PLUS,  INPUT);
  pinMode(PIN_TEAM_A_MINUS, INPUT);
  pinMode(PIN_TEAM_B_PLUS,  INPUT);
  pinMode(PIN_TEAM_B_MINUS, INPUT);
  pinMode(PIN_IR_A,         INPUT);
  pinMode(PIN_IR_B,         INPUT);

  // Screen setup
  SCREEN_A.begin();
  SCREEN_A.displayText(screenBufferA, CENTER, 0, 0, PRINT, NO_EFFECT);
}

void debug() {
  Serial.print("SCORE A: ");
  Serial.print(golsA);
  Serial.print(" | SCORE B: ");
  Serial.print(golsB);
  Serial.println("");
  sprintf(screenBufferA, "%d - %d", golsA, golsB);
  SCREEN_A.displayReset();
}

void addGoal(int &team, int delta = 1){
  team += delta;
}

void readButton(int &ir, bool &debounce, int &team, int delta) {
  if (!debounce) {
  } else if (ir < IR_THRESHOLD) {
    debounce = false;
  }
}

struct Buttons readButtons() {
  bool aPlus  = digitalRead(PIN_TEAM_A_PLUS);
  bool aMinus = digitalRead(PIN_TEAM_A_MINUS);
  bool bPlus  = digitalRead(PIN_TEAM_B_PLUS);
  bool bMinus = digitalRead(PIN_TEAM_B_MINUS);

  Buttons b = {aPlus, aMinus, bPlus, bMinus};
  return b;
}

void updateScore(Buttons b){
  if (b.aPlus && b.aMinus && b.bPlus && b.bMinus){
    golsA = 0;
    golsB = 0;
  } else {
    if (b.aPlus) {
      addGoal(golsA);
    }
    if (b.aMinus) {
      addGoal(golsA, -1);
    }
    if (b.bPlus) {
      addGoal(golsB);
    }
    if (b.bMinus) {
      addGoal(golsB, -1);
    }
  }
}

struct Sensors readIRSensors() {
  bool irA = analogRead(PIN_IR_A) > IR_THRESHOLD;
  bool irB = analogRead(PIN_IR_B) > IR_THRESHOLD;

  Sensors s = {irA, irB};
  return s;
}

void updateScore(Sensors s){
  updateScore(s.irA, debounce_ir_a, golsB);
  updateScore(s.irB, debounce_ir_b, golsA);
}

void updateScore(bool &ir, bool &debounce, int &team) {
  if (!debounce) {
    if (ir) {
      addGoal(team);
      debounce = true;
    }
  } else if (!ir) {
    debounce = false;
  }
}

void loop() {

  Sensors s = readIRSensors();
  updateScore(s);
  Buttons b = readButtons();
  updateScore(b);

  debug();

  // Screen update
  SCREEN_A.displayAnimate();
  delay(200);
}