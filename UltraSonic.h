#pragma once
#include <Arduino.h>

class UltraSonic
{
	int _pinRx, _pinTx;
	unsigned long detectDistance();
public:
	UltraSonic(int pinRx, int pinTx);
	double detectCM();
	double detectINCH();
};
