#include "MotorDriver.h"

MotorDriver::MotorDriver(int &pin1, int &pin2, int &enable)
{
	_controlPin1 = pin1;
	_controlPin2 = pin2;
	setControlPins(enable);
}

void MotorDriver::spinRight()
{
	digitalWrite(_controlPin1, LOW);
	digitalWrite(_controlPin2, HIGH);
}

void MotorDriver::spinLeft()
{
	digitalWrite(_controlPin1, HIGH);
	digitalWrite(_controlPin2, LOW);
}

void MotorDriver::coast()
{
	digitalWrite(_controlPin1, LOW);
	digitalWrite(_controlPin2, LOW);
}

void MotorDriver::brake()
{
	digitalWrite(_controlPin1, HIGH);
	digitalWrite(_controlPin2, HIGH);
}

void MotorDriver::setControlPins(int &enable)
{
	pinMode(enable, OUTPUT);
	pinMode(_controlPin1, OUTPUT);
	pinMode(_controlPin2, OUTPUT);

	digitalWrite(enable, HIGH);
}