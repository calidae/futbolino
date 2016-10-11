#ifndef _MY_STRUCTS_H
#define _MY_STRUCTS_H

typedef struct Buttons Buttons;
struct Buttons {
	bool plusA;
	bool minusA;
	bool plusB;
	bool minusB;
};

typedef struct Sensors Sensors;
struct Sensors {
	bool irA;
	bool irB;
};

typedef struct InputPins InputPins;
struct InputPins {
	int a_plus;
	int a_minus;
	int b_plus;
	int b_minus;
	int a_ir;
	int b_ir;
};


#endif
