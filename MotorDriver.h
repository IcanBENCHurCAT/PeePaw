#pragma once
#include <Arduino.h>

class MotorDriver
{
private:
	void setControlPins();
	int controlPin1, controlPin2;
public:
	MotorDriver(int pin1, int pin2);
	void forward();
	void backward();
	void coast();
	void brake();
};