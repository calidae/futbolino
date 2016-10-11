#ifndef _FUTBOLINO_CONFIG_H
#define _FUTBOLINO_CONFIG_H

// Input Pins
const Inputs in = {A0, A1, A2, A3, A4, A5};

//#define FUTBOLINO_SCREEN_MATRIX
#define FUTBOLINO_SCREEN_LCD
//#define FUTBOLINO_SCREEN_SERIAL

#ifdef FUTBOLINO_SCREEN_MATRIX

const int TOTAL_SCREENS = 2;
const int FIRST_SCREEN_START = 0;
const int FIRST_SCREEN_END = 3;
const int SECOND_SCREEN_START = 4;
const int SECOND_SCREEN_END = 7;

const int MAX_DEVICES = 8;
const int CLK_PIN = 12;
const int DATA_PIN = 11;
const int CS_PIN = 10;

#else
#ifdef FUTBOLINO_SCREEN_LCD
#else
#ifdef FUTBOLINO_SCREEN_SERIAL
#else
#endif
#endif
#endif

#endif
