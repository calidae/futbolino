
#include <QueueArray.h>

enum SIL_EventType {
  NONE,
  KEY_DOWN,
  KEY_UP
};

class SIL_Event {

  public:

  SIL_Event() {
    this->type = NONE;
    this->index = 0;
    this->pin = 0;
  }

  SIL_Event(SIL_EventType type, int index, int pin) {
    this->type = type;
    this->index = index;
    this->pin = pin;
  }

  SIL_EventType type;
  int index;
  int pin;
};

class SIL {

  public:

  SIL(int pinArraySize, int* pinArray) {
    _pinArraySize = pinArraySize;
    _pinArray = pinArray;
    _pinsState = new boolean[_pinArraySize];
    _debounces = new boolean[_pinArraySize];
  }

  ~SIL() {
    delete[] _pinsState;
    delete[] _debounces;
  }

  void update() {
    for (int pin = 0; pin < _pinArraySize; pin++) {
      _pinsState[pin] = digitalRead(_pinArray[pin]);

      if (!_debounces[pin]) {
        if (_pinsState[pin]) {
          _debounces[pin] = true;
          _evenQueue.push(SIL_Event(KEY_DOWN, pin, _pinArray[pin]));
        }
      } else {
        if(!_pinsState[pin]) {
          _debounces[pin] = false;
          _evenQueue.push(SIL_Event(KEY_UP, pin, _pinArray[pin]));
        }
      }
    }
  }

  boolean pollEvent(SIL_Event* pEvent) {
    if (_evenQueue.isEmpty()) {
      return false;
    } else {
      *pEvent = _evenQueue.pop();
      return true;
    }
  }



  private:

  int _pinArraySize;
  int* _pinArray;
  boolean* _pinsState;
  boolean* _debounces;
  QueueArray<SIL_Event> _evenQueue;

};
