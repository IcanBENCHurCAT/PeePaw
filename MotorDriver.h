#pragma once
#include <Arduino.h>

class MotorDriver
{
private:
	void setControlPins();
	int controlPin1, controlPin2, enablePin;
public:
	MotorDriver(int pin1, int pin2, int enable);
	void spinRight();
	void spinLeft();
	void coast();
	void brake();
};