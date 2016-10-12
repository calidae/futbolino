
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
    this->pin = 0;
  }

  SIL_Event(SIL_EventType type, int pin) {
    this->type = type;
    this->pin = pin;
  }

  SIL_EventType type;
  int pin;
};

class SIL_EventQueue {

  public:
  virtual void update() = 0;

  boolean pollEvent(SIL_Event* pEvent) {
    if (_evenQueue.isEmpty()) {
      return false;
    } else {
      *pEvent = _evenQueue.pop();
      return true;
    }
  };

  void clear() {
    while (!_evenQueue.isEmpty()) {
      _evenQueue.pop();
    }
  }

  protected:

  QueueArray<SIL_Event> _evenQueue;

  void enqueue(SIL_Event event) {
    _evenQueue.push(event);
  };

};

class SIL : public SIL_EventQueue {

  public:

  SIL(int pinArraySize, int* pinArray) {
    _pinArraySize = pinArraySize;
    _debounces = new boolean[_pinArraySize];
    _pinArray = new int[_pinArraySize];
    for (int i = 0, pin = pinArray[0]; i < _pinArraySize; pin = pinArray[++i]) {
      _pinArray[i] = pin;
      _debounces[i] = digitalRead(pin);
    }
  }

  ~SIL() {
    delete[] _debounces;
    delete [] _pinArray;
  }

  void update() {
    for (int i = 0, pin = _pinArray[0]; i < _pinArraySize; pin = _pinArray[++i]) {

      _pinsState[pin] = digitalRead(pin);

      if (!_debounces[i]) {
        if (_pinsState[pin]) {
          _debounces[i] = true;
          enqueue(SIL_Event(KEY_DOWN, pin));
        }
      } else {
        if(!_pinsState[pin]) {
          _debounces[i] = false;
          enqueue(SIL_Event(KEY_UP, pin));
        }
      }
    }
  }

  boolean* getPinsState() {
    return _pinsState;
  }

  private:

  int _pinArraySize;
  int* _pinArray;
  boolean _pinsState[20] = {false};
  boolean* _debounces;

};

class SIL_Sensor : public SIL_EventQueue {

  public:

  SIL_Sensor(int pin, int threshold) {
    this->pin = pin;
    this->threshold = threshold;
    debounce = analogRead(pin) > threshold;
  }

  void update() {
    reading = analogRead(pin);

    if (!debounce) {
      if (reading > threshold) {
        debounce = true;
        enqueue(SIL_Event(KEY_UP, pin));
      }
    } else {
      if(reading < threshold) {
        debounce = false;
        enqueue(SIL_Event(KEY_DOWN, pin));
      }
    }
  }

  int* getReading() {
    return &reading;
  }

  private:

  int pin;
  int threshold;
  int reading;
  boolean debounce;

};
