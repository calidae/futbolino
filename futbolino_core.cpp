#include "futbolino_core.h"

char* goal_texts[] = {"Gas", "Bo", "100", "Dins", "Inside", "Mel", "Nyam", "Oju!"};

Futbolino::Futbolino(Inputs in, ScreenWrapper *screen) {
  int buttonPins[4] = {
    in.PIN_TEAM_A_PLUS, in.PIN_TEAM_A_MINUS, in.PIN_TEAM_B_PLUS, in.PIN_TEAM_B_MINUS
  };
  _buttons = new SIL(4, buttonPins);
  _irA = new SIL_Sensor(in.PIN_IR_A, IR_THRESHOLD);
  _irB = new SIL_Sensor(in.PIN_IR_B, IR_THRESHOLD);
  _in = in;
  _screen = screen;

  _screenA = new FutbolinoScreen(_screen, 0);
  _screenB = new FutbolinoScreen(_screen, 1);
}

Futbolino::~Futbolino() {
  delete _buttons;
}

void Futbolino::begin() {
  resetScore();
  _currentState = SERVE;
  _lastScored = UNDEFINED;

  _screenA->begin();
  _screenB->begin();

  _screenA->setStaticText((char*)TXT_SERVE);
  _screenB->setStaticText((char*)TXT_SERVE);
}

void Futbolino::loop() {
  _buttons->update();
  _irA->update();
  _irB->update();

  for (SIL_Event event;
      _buttons->pollEvent(&event)
      || _irA->pollEvent(&event)
      || _irB->pollEvent(&event);
  ) {
    Serial.print("[event]");
    Serial.print(event.pin);
    Serial.print(":");
    Serial.print(event.type);
    Serial.println("");
    if (event.type == KEY_UP) {
      if(_currentState == PLAY || _currentState == PUTA) {
        if (event.pin == _in.PIN_TEAM_A_PLUS || event.pin == _in.PIN_IR_A) {
          addGoalA();
        } else if (event.pin == _in.PIN_TEAM_A_MINUS) {
          subGoalA();
        } else if (event.pin == _in.PIN_TEAM_B_PLUS || event.pin == _in.PIN_IR_B) {
          addGoalB();
        } else if (event.pin == _in.PIN_TEAM_B_MINUS) {
          subGoalB();
        }
      } else if(_currentState == SERVE
          && event.type == KEY_UP
          && event.pin != _in.PIN_TEAM_A_MINUS
          && event.pin != _in.PIN_TEAM_B_MINUS
      ) {
        if (event.pin == _in.PIN_TEAM_A_PLUS || event.pin == _in.PIN_IR_A) {
          _lastScored = A;
          _screenA->setAnimation((char*)TXT_FIRSTBALL_A);
          _screenB->showScore(0, 0);
        } else if (event.pin == _in.PIN_TEAM_B_PLUS || event.pin == _in.PIN_IR_B) {
          _lastScored = B;
          _screenB->setAnimation((char*)TXT_FIRSTBALL_B);
          _screenA->showScore(0, 0);
        }
        _currentState = PLAY;
      }
    }
  }

  updateScreen();
}

void Futbolino::chooseServerTeam(Sensors s, Buttons b){
  // if (checkDebounce(s.irA, _debounceIrA) ||
  //     checkDebounce(b.plusA, _debounceButtonPlusA)) {
  //   DEBUG("team A saca");
  //   _lastScored = A;
  //   _screenA->setAnimation((char*)TXT_FIRSTBALL_A);
  //   _screenB->showScore(0, 0);
  //   _currentState = PLAY;
  // } else if (checkDebounce(s.irB, _debounceIrB) ||
  //     checkDebounce(b.plusB, _debounceButtonPlusB)) {
  //   DEBUG("team B saca");
  //   _lastScored = B;
  //   _screenB->setAnimation((char*)TXT_FIRSTBALL_B);
  //   _screenA->showScore(0, 0);
  //   _currentState = PLAY;
  // }
}

void Futbolino::updateFrom(Sensors s){
  // TODO: Change irA and irB pins and simplify the problem?
  // if (checkDebounce(s.irA, _debounceIrA)){
  //   addGoalB();
  // }
  // if (checkDebounce(s.irB, _debounceIrB)){
  //   addGoalA();
  // }
}

void Futbolino::updateFrom(Buttons b){
  if (areAllButtonsPressed(b)){
    begin();
  }
}

bool Futbolino::areAllButtonsPressed(Buttons b){
  return (b.plusA && b.minusA && b.plusB && b.minusB);
}

void Futbolino::resetScore(){
  _golsA = 0;
  _golsB = 0;
}

void Futbolino::addGoalA(){
  DEBUG("team A scored");
  changeScore(_golsA);
  _lastScored = A;
  manageScoreIncrement();
}

void Futbolino::addGoalB(){
  DEBUG("team B scored");
  changeScore(_golsB);
  _lastScored = B;
  manageScoreIncrement();
}

void Futbolino::subGoalA(){
  changeScore(_golsA, -1);
  showScoreInScreens();
}

void Futbolino::subGoalB(){
  changeScore(_golsB, -1);
  showScoreInScreens();
}

void Futbolino::manageScoreIncrement(){
  if (_golsB + _golsA == 11) {
    _currentState = END;

    char* end;
    if (_golsA > _golsB)
      end = (char*)TXT_END_A;
    else
      end = (char*)TXT_END_B;

    _screenA->setAnimation(end, callbackRestart);
    _screenB->setAnimation(end);
  } else if (_golsA == 6 && _lastScored == A) {
    // La bola puta no entra aqui
    showScoreInScreens();
    char* txtA = (char*)TXT_WIN_A;
    char* txtB = (char*)TXT_LOOSE;
    _screenA->setAnimation(txtA);
    _screenB->setAnimation(txtB);
  } else if (_golsB == 6 && _lastScored == B) {
    // La bola puta no entra aqui
    showScoreInScreens();
    char* txtA = (char*)TXT_LOOSE;
    char* txtB = (char*)TXT_WIN_B;
    _screenA->setAnimation(txtA);
    _screenB->setAnimation(txtB);
  } else {
    showScoreInScreens();

    char* txt;
    long rand = random(0,20);
    if (rand < 8){
      txt = goal_texts[rand];
      if (_lastScored == A){
        _screenA->setShortAnimation(txt);
      } else {
        _screenB->setShortAnimation(txt);
      }
    }
  }

  if (_golsA == 5 && _golsB == 5) {
    _currentState = PUTA;
  }
}

void Futbolino::showScoreInScreens(){
  _screenA->showScore(_golsA, _golsB);
  _screenB->showScore(_golsB, _golsA);
}

void Futbolino::changeScore(int &team, int delta){
  if (team + delta >= 0 &&
    (_currentState == PLAY || _currentState == PUTA)
  ) {
    team += delta;
  }
}

void Futbolino::updateScreen(){
  bool animationFinished = _screen->displayAnimate();
  switch (_currentState){
    case PUTA:
      if (animationFinished) {
        char* txt = (char*) TXT_PUTA;
        _screenA->blinkAnimation(txt);
        _screenB->blinkAnimation(txt);
      }
      break;
    default:
      break;
  }
  _screenA->update(animationFinished);
  _screenB->update(animationFinished);
}

extern Futbolino futbolino;

void callbackRestart() {
  futbolino.begin();
}
