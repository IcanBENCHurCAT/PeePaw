#include "MotorDriver.h"

MotorDriver::MotorDriver(int pin1, int pin2, int enable)
{
	controlPin1 = pin1;
	controlPin2 = pin2;
	enablePin = enable;
	setControlPins();
}

void MotorDriver::spinRight()
{
	digitalWrite(controlPin1, LOW);
	digitalWrite(controlPin2, HIGH);
}

void MotorDriver::spinLeft()
{
	digitalWrite(controlPin1, HIGH);
	digitalWrite(controlPin2, LOW);
}

void MotorDriver::coast()
{
	digitalWrite(controlPin1, LOW);
	digitalWrite(controlPin2, LOW);
}

void MotorDriver::brake()
{
	digitalWrite(controlPin1, HIGH);
	digitalWrite(controlPin2, HIGH);
}

void MotorDriver::setControlPins()
{
	pinMode(enablePin, OUTPUT);
	pinMode(controlPin1, OUTPUT);
	pinMode(controlPin2, OUTPUT);

	digitalWrite(enablePin, HIGH);
}