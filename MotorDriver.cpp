#include "MotorDriver.h"

MotorDriver::MotorDriver(int pin1, int pin2, int enable)
{
	_controlPin1 = pin1;
	_controlPin2 = pin2;
	_enablePin = enable;
	setControlPins();
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

void MotorDriver::setControlPins()
{
	pinMode(_enablePin, OUTPUT);
	pinMode(_controlPin1, OUTPUT);
	pinMode(_controlPin2, OUTPUT);

	setSpeed(255);
}

void MotorDriver::setSpeed(unsigned int speed)
{
	analogWrite(_enablePin, speed);
}