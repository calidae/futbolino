#include "futbolino_core.h"

char* goal_texts[] = {"Gas", "Bo", "100", "Dins", "Inside", "Mel", "Nyam", "Oju!"};

Futbolino::Futbolino(InputPins in, ScreenWrapper *screen) {
  int buttonPins[4] = {in.a_plus, in.a_minus, in.b_plus, in.b_minus};
  _buttons = new SIL(4, buttonPins);
  _pinsState = _buttons->getPinsState();
  _irA = new SIL_Sensor(in.a_ir, IR_THRESHOLD);
  _irB = new SIL_Sensor(in.b_ir, IR_THRESHOLD);
  _inputPins = in;

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

  if (areAllButtonsPressed()) {
    do {
      _buttons->update();
      _buttons->clear();
    } while (isAnyButtonPressed());
    begin();
  }

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
        update(event);
      } else if(_currentState == SERVE) {
        chooseServerTeam(event);
      }
    }
  }

  updateScreen();
}

void Futbolino::chooseServerTeam(SIL_Event event){
  if(event.pin != _inputPins.a_minus && event.pin != _inputPins.b_minus) {
    if (event.pin == _inputPins.a_plus || event.pin == _inputPins.a_ir) {
      _lastScored = A;
      _screenA->setAnimation((char*)TXT_FIRSTBALL_A);
      _screenB->showScore(0, 0);
    } else if (event.pin == _inputPins.b_plus || event.pin == _inputPins.b_ir) {
      _lastScored = B;
      _screenB->setAnimation((char*)TXT_FIRSTBALL_B);
      _screenA->showScore(0, 0);
    }
    _currentState = PLAY;
  }
}

void Futbolino::update(SIL_Event event){
  if (event.pin == _inputPins.a_plus || event.pin == _inputPins.a_ir) {
    addGoalA();
  } else if (event.pin == _inputPins.a_minus) {
    subGoalA();
  } else if (event.pin == _inputPins.b_plus || event.pin == _inputPins.b_ir) {
    addGoalB();
  } else if (event.pin == _inputPins.b_minus) {
    subGoalB();
  }
}

boolean Futbolino::areAllButtonsPressed(){
  return (
    _pinsState[_inputPins.a_plus]
    && _pinsState[_inputPins.a_minus]
    && _pinsState[_inputPins.b_plus]
    && _pinsState[_inputPins.b_minus]
  );
}

boolean Futbolino::isAnyButtonPressed(){
  return (
    _pinsState[_inputPins.a_plus]
    || _pinsState[_inputPins.a_minus]
    || _pinsState[_inputPins.b_plus]
    || _pinsState[_inputPins.b_minus]
  );
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
