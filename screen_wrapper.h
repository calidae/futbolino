#ifndef _SCREEN_WRAPPER_H
#define _SCREEN_WRAPPER_H

#include <LiquidCrystal.h>

class ScreenWrapper {
public:
  virtual bool getZoneStatus(int zone) = 0;
  virtual bool displayAnimate() = 0;
  virtual void displayZoneText(
    uint8_t z, char *pText, textPosition_t align, uint16_t speed, uint16_t pause,
    textEffect_t effectIn, textEffect_t effectOut = NO_EFFECT) = 0;
};

class LedMatrixWrapper : public ScreenWrapper {
public:

  LedMatrixWrapper(MD_Parola* screen) {
    this->_screen = screen;
  }

  virtual bool getZoneStatus(int zone) {
      return _screen->getZoneStatus(zone);
  };

  virtual bool displayAnimate() {
      return _screen->displayAnimate();
  };

  virtual void displayZoneText(
      uint8_t z, char *pText, textPosition_t align, uint16_t speed, uint16_t pause,
      textEffect_t effectIn, textEffect_t effectOut = NO_EFFECT
    ) {
    _screen->displayZoneText(z, pText, align, speed, pause, effectIn, effectOut);
    return;
  };

private:
  MD_Parola* _screen;
};

class SerialPrintlnWrapper : public ScreenWrapper {
public:

  SerialPrintlnWrapper(HardwareSerial* pSerial) {
    this->pSerial = pSerial;
  }

  virtual bool getZoneStatus(int zone) {
      return true;
  };

  virtual bool displayAnimate() {
      return true;
  };

  virtual void displayZoneText(
      uint8_t z, char *pText, textPosition_t align, uint16_t speed, uint16_t pause,
      textEffect_t effectIn, textEffect_t effectOut = NO_EFFECT
    ) {
    pSerial->println(pText);
    return;
  };

private:
  HardwareSerial* pSerial;
};

class LCDWrapper : public ScreenWrapper {
public:

  LCDWrapper(LiquidCrystal* lcd) {
    this->lcd = lcd;
  }

  virtual bool getZoneStatus(int zone) {
      return true;
  };

  virtual bool displayAnimate() {
      return true;
  };

  virtual void displayZoneText(
      uint8_t z, char *pText, textPosition_t align, uint16_t speed, uint16_t pause,
      textEffect_t effectIn, textEffect_t effectOut = NO_EFFECT
    ) {
    digitalWrite(LED_BUILTIN, HIGH);
    lcd->setCursor(0, z);
    lcd->print("                ");
    lcd->setCursor(0, z);
    lcd->print(pText);
    delay(speed*100);
    delay(pause*100);
    digitalWrite(LED_BUILTIN, LOW);
    return;
  };

private:
  LiquidCrystal* lcd;
};

#endif
