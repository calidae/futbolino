#ifndef _FUTBOLINO_H
#define _FUTBOLINO_H

#include <MD_Parola.h>
#include "SimpleInputLayer.h"
#include "my_structs.h"
#include "screen_wrapper.h"
#include "futbolino_screen.h"

#define _FUTBOLINO_H_DEBUG 1
#ifdef _FUTBOLINO_H_DEBUG
#define DEBUG(A) Serial.println(A)
#else
#define DEBUG(A)
#endif

#define SCREEN_BUF_SIZE 75

const int IR_THRESHOLD = 300;

enum GameState
{
  SERVE,
  PLAY,
  WIN,
  END,
  PUTA
};

enum Team
{
  UNDEFINED,
  A,
  B
};

const char TXT_END_A[] = "Final de partit. Verds guanyen!";
const char TXT_END_B[] = "Final de partit. Blancs guanyen!";
const char TXT_WIN_A[] = "Verds guanyen!";
const char TXT_WIN_B[] = "Blancs guanyen!";
const char TXT_LOOSE[] = "     A plorar";
const char TXT_SERVE[] = "Saque";
const char TXT_FIRSTBALL_A[] = "Verds saquen";
const char TXT_FIRSTBALL_B[] = "Blancs saquen";
const char TXT_PUTA[] = "PUTA";


void callbackRestart();

class Futbolino {

public:
  Futbolino(InputPins in, ScreenWrapper *screen);
        ~Futbolino();

  void begin();
  void loop();

private:

  InputPins _inputPins;
  ScreenWrapper* _screen;
  char _screenBufferA[SCREEN_BUF_SIZE];
  char _screenBufferB[SCREEN_BUF_SIZE];

  FutbolinoScreen* _screenA;
  FutbolinoScreen* _screenB;

  GameState _currentState;
  Team _lastScored;

  int _golsA = 0;
  int _golsB = 0;
  SIL* _buttons = NULL;
  boolean* _pinsState = NULL;
  SIL_Sensor* _irA = NULL;
  SIL_Sensor* _irB = NULL;

  void addGoalA();
  void addGoalB();
  void subGoalA();
  void subGoalB();
  void manageScoreIncrement();
  void manageScoreDecrement();
  void showScoreInScreens();
  void changeScore(int &team, int delta = 1);
  void readButton(int &ir, bool &debounce);
  void chooseServerTeam(SIL_Event event);
  void update(SIL_Event event);
  boolean areAllButtonsPressed();
  boolean isAnyButtonPressed();
  void resetScore();

  void updateScreen();

};

#endif
