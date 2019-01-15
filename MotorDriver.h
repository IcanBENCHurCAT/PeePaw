#pragma once
#include <Arduino.h>

class MotorDriver
{
	void setControlPins();
	int _controlPin1, _controlPin2, _enablePin;
public:
	MotorDriver(int pin1, int pin2, int enable);
	void setSpeed(unsigned int speed);
	void spinRight();
	void spinLeft();
	void coast();
	void brake();
};