#include "MotorDriver.h"

MotorDriver::MotorDriver(int pin1, int pin2)
{
	controlPin1 = pin1;
	controlPin2 = pin2;
	setControlPins();
}

void MotorDriver::forward()
{
	digitalWrite(controlPin1, LOW);
	digitalWrite(controlPin2, HIGH);
}

void MotorDriver::backward()
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
	pinMode(controlPin1, OUTPUT);
	pinMode(controlPin2, OUTPUT);
}