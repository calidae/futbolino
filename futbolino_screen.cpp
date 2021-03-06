#include "futbolino_screen.h"

FutbolinoScreen::FutbolinoScreen(ScreenWrapper *screen, int zone) {
	_screen = screen;
	_zone = zone;
	begin();
}

void FutbolinoScreen::begin(){
	_animating = false;
	_next_callback = 0;
	sprintf(_score_buffer, "0 - 0");
}

void FutbolinoScreen::update(bool animationFinished){
	if (animationFinished && _animating && _screen->getZoneStatus(_zone)){
		if (_next_callback){
			_next_callback();
			_next_callback = 0;
		} else {
			_showScore();
		}
	}
}

void FutbolinoScreen::setStaticText(char* text){
	_animating = false;
	_screen->setIntensity(_zone, 15);
	_screen->displayZoneText(_zone, text, CENTER, 20, 0, FADE, NO_EFFECT);
}

void FutbolinoScreen::setAnimation(char* text){
	_animating = true;
	_screen->setIntensity(_zone, 15);
	_screen->displayZoneText(_zone, text, CENTER, ANIMATION_SPEED, 0, SCROLL_LEFT, SCROLL_LEFT);
}

void FutbolinoScreen::setShortAnimation(char* text){
	_animating = true;
	_screen->setIntensity(_zone, 15);
	_screen->displayZoneText(_zone, text, CENTER, SHORT_ANIMATION_SPEED, SHORT_ANIMATION_PAUSE, MESH, FADE);
}

void FutbolinoScreen::blinkAnimation(char* text){
	_animating = true;
	_screen->setIntensity(_zone, 15);
	_screen->displayZoneText(_zone, text, CENTER, SHORT_ANIMATION_SPEED, SHORT_ANIMATION_PAUSE, FADE, FADE);
}

void FutbolinoScreen::setAnimation(char* text, void (*function)()){
	_next_callback = function;
	setAnimation(text);
}

void FutbolinoScreen::_showScore(){
	setStaticText(_score_buffer);
}

void FutbolinoScreen::showScore(int a, int b){
	sprintf(_score_buffer, "%d - %d", a, b);
	_showScore();
}
