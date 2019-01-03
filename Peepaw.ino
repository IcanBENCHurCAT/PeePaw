#include "MotorDriver.h"
#include <IRremote.h>
#include <IRremoteInt.h>
//Constant Data
const int LED_PIN = 13;
const int MOTOR_LEFT_1 = 7;
const int MOTOR_LEFT_2 = 8;
const int MOTOR_RIGHT_1 = 9;
const int MOTOR_RIGHT_2 = 10;
const int IR_PIN = 11;

const char COMMAND_LEFT = 'L';
const char COMMAND_RIGHT = 'R';
const char COMMAND_FORWARD = 'F';
const char COMMAND_BACKWARD = 'B';
const char COMMAND_STOP = 'S';



enum TransmitMode
{
	Bluetooth,
	Infrared,
	Wifi
};

const TransmitMode mode = Infrared;


//Variable Data
char data = 0;
IRrecv receiver(IR_PIN);
decode_results results;
MotorDriver motorsLeft(MOTOR_LEFT_1, MOTOR_LEFT_2);
MotorDriver motorsRight(MOTOR_RIGHT_1, MOTOR_RIGHT_2);


void setup() {
	pinMode(LED_PIN, OUTPUT);

	//If Bluetooth is Connected
	Serial.begin(9600);

	//If Infrared is Connected
	receiver.enableIRIn();
	//receiver.blink13(true); //Not sure why this is done, but most tutorials have it

	/*pinMode(MOTOR_LEFT_1, OUTPUT);
	pinMode(MOTOR_LEFT_2, OUTPUT);
	pinMode(MOTOR_RIGHT_1, OUTPUT);
	pinMode(MOTOR_RIGHT_2, OUTPUT);*/
}

void loop() {
	if (mode == Bluetooth)
	{
		//Need bluetooth TX/RX before this can be enabled
		doBluetooth();
	}
	else if (mode == Infrared)
	{
		doInfrared();
	}
	else if (mode == Wifi)
	{
		//Not implemented yet
	}

}

const unsigned long COAST_TIME = 1000L;
void doInfrared()
{
	if (receiver.decode(&results))
	{
		Serial.println(results.value, HEX);

		switch (results.value)
		{
		case 0x0F2B22:
			forward();
			delay(COAST_TIME);
			break;
		case 0x0F2B24:
			backward();
			delay(COAST_TIME);
			break;
		case 0x0F2B26:
			left();
			delay(COAST_TIME);
			break;
		case 0x0F2B28:
			right();
			delay(COAST_TIME);
			break;
		}

		receiver.resume();
	}
}

void doBluetooth()
{
	if (Serial.available() > 0)
	{
		setLED(true);
		data = Serial.read();
		Serial.println(data);
		switch (data)
		{
		case COMMAND_FORWARD:
			forward();
			break;
		case COMMAND_BACKWARD:
			backward();
			break;
		case COMMAND_LEFT:
			left();
			break;
		case COMMAND_RIGHT:
			right();
			break;
		case COMMAND_STOP:
			stop();
			break;
		}
	}
}

void setLED(bool turnOn)
{
	if (turnOn)
		digitalWrite(LED_PIN, HIGH);
	else
		digitalWrite(LED_PIN, LOW);
}

void stop()
{
	motorsLeft.coast();
	motorsRight.coast();
}

void forward()
{
	motorsLeft.forward();
	motorsRight.forward();
}

void backward()
{
	motorsLeft.backward();
	motorsLeft.backward();
}

void left()
{
	motorsLeft.backward();
	motorsRight.forward();
}

void right()
{
	motorsLeft.forward();
	motorsRight.backward();
}
