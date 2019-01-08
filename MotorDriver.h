#pragma once
#include <Arduino.h>

class MotorDriver
{
	void setControlPins(int &enable);
	int _controlPin1, _controlPin2;
public:
	MotorDriver(int &pin1, int &pin2, int &enable);
	void spinRight();
	void spinLeft();
	void coast();
	void brake();
};