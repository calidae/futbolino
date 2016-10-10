#ifndef _FUTBOLINO_SCREEN_H
#define _FUTBOLINO_SCREEN_H

#include <MD_Parola.h>
#include "screen_wrapper.h"

#define _FUTBOLINO_SCREEN_H_DEBUG
#ifdef _FUTBOLINO_SCREEN_H_DEBUG
#define DEBUG(A) Serial.println(A)
#else
#define DEBUG(A)
#endif

#define SCORE_BUF_SIZE 7
#define ANIMATION_SPEED 30
#define SHORT_ANIMATION_SPEED 50
#define SHORT_ANIMATION_PAUSE 150

class FutbolinoScreen {

public:
	FutbolinoScreen(ScreenWrapper *screen, int zone);

	void begin();
	void update(bool animationFinished);
	void setStaticText(char* text);
	void setAnimation(char* text);
	void setShortAnimation(char* text);
	void blinkAnimation(char* text);
	void setAnimation(char* text, void (*)());
	void showScore(int a, int b);

private:
	ScreenWrapper* _screen;
	char _score_buffer[SCORE_BUF_SIZE];
	int _zone;
	bool _animating;
	void (* _next_callback)();

	void _showScore();

};

#endif
