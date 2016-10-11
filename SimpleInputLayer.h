
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
    _pinsState = new boolean[_pinArraySize];
    _debounces = new boolean[_pinArraySize];
    _pinArray = new int[_pinArraySize];
    for (int i = 0; i < _pinArraySize; i++) {
      _pinArray[i] = pinArray[i];
    }
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
          enqueue(SIL_Event(KEY_DOWN, pin, _pinArray[pin]));
        }
      } else {
        if(!_pinsState[pin]) {
          _debounces[pin] = false;
          enqueue(SIL_Event(KEY_UP, pin, _pinArray[pin]));
        }
      }
    }
  }

  private:

  int _pinArraySize;
  int* _pinArray;
  boolean* _pinsState;
  boolean* _debounces;

};

class SIL_Sensor : public SIL_EventQueue {

  public:

  SIL_Sensor(int pin, int threshold) {
    this->pin = pin;
    this->threshold = threshold;
    debounce = analogRead(pin) < threshold;
  }

  void update() {
    reading = analogRead(pin);

    if (!debounce) {
      if (reading > threshold) {
        debounce = true;
        enqueue(SIL_Event(KEY_UP, 0, pin));
      }
    } else {
      if(reading < threshold) {
        debounce = false;
        enqueue(SIL_Event(KEY_DOWN, 0, pin));
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
