#include "UltraSonic.h"

UltraSonic::UltraSonic(int pinRx, int pinTx)
{
	_pinRx = pinRx;
	_pinTx = pinTx;
	pinMode(pinTx, OUTPUT);
	pinMode(pinRx, INPUT);
}

unsigned long UltraSonic::detectDistance()
{
	digitalWrite(_pinTx, LOW);
	delayMicroseconds(5); //'clean' the transmission

	digitalWrite(_pinTx, HIGH);
	delayMicroseconds(10); //transmit 10 mus of frequency

	digitalWrite(_pinTx, LOW);

	return pulseIn(_pinRx, HIGH, 50000UL); //will not look for objects greater than 8 meters away
}

double UltraSonic::detectCM()
{
	return (detectDistance() / 2.0) / 29.1;
}

double UltraSonic::detectINCH()
{
	return (detectDistance() / 2.0) / 74.0;
}