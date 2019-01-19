#include "MotorDriver.h"
#include "UltraSonic.h"
#include <IRremote.h>
#include <IRremoteInt.h>
#include "PinDefinitions.h"

#define COMMAND_LEFT 'L'
#define COMMAND_RIGHT 'R'
#define COMMAND_FORWARD 'F'
#define COMMAND_BACKWARD 'B'
#define COMMAND_STOP 'S'

#define COAST_TIME 128L
#define SLOW 128
#define FAST 255

enum TransmitMode
{
	Bluetooth,
	Infrared,
	Wifi,
	AutoPilot
};

enum DriveState
{
	Driving,
	Coasting,
	Turning,
	Reversing
};

const TransmitMode mode = AutoPilot;

char data = 0; //Bluetooth data

DriveState driveState = Coasting;

IRrecv receiver(IR_PIN);
decode_results results;

MotorDriver motorsLeft(MOTOR_LEFT_1, MOTOR_LEFT_2, MOTOR_LEFT_ENABLE);
MotorDriver motorsRight(MOTOR_RIGHT_1, MOTOR_RIGHT_2, MOTOR_RIGHT_ENABLE);

UltraSonic ultraSonic(SENSOR_PIN_RX, SENSOR_PIN_TX);

void setup() {

	Serial.begin(9600);

	if (mode == Infrared)
	{
		receiver.enableIRIn();
	}
	else if (mode == AutoPilot)
	{

	}

	setSpeed(SLOW);
	pinMode(SENSOR_PIN_RX, INPUT);
	pinMode(SENSOR_PIN_TX, OUTPUT);

}

void loop() {
	if (mode == AutoPilot)
	{
		doAutoPilot();
	}
	else if (mode == Bluetooth)
	{
		//Need bluetooth TX/RX before this can be enabled
		doBluetooth();
	}
	else if (mode == Infrared)
	{
		doInfrared();
		Serial.print("Detecting a distance of ");
		Serial.print(ultraSonic.detectCM());
		Serial.println(" cm");
	}
	else if (mode == Wifi)
	{
		//Not implemented yet
	}
}

void doInfrared()
{
	if (receiver.decode(&results))
	{
		Serial.println(results.value, HEX);

		switch (results.value)
		{
		case 0x20DF6A95:
			forward();
			Serial.println("forward");
			delay(COAST_TIME * 4);
			break;
		case 0x20DF1BE4:
			left();
			Serial.println("left");
			delay(COAST_TIME);
			break;
		case 0x20DFEB14:
			backward();
			Serial.println("backward");
			delay(COAST_TIME * 2);
			break;
		case 0x20DF9B64:
			right();
			Serial.println("right");
			delay(COAST_TIME);
			break;
		}
		stop();
		receiver.resume();
	}
}

void doBluetooth()
{
	if (Serial.available() > 0)
	{
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

bool isInRange(double& cm)
{
	if (cm > 0.00 && cm < 10.0)
		return true;

	return false;
}

bool isInLongRange(double& cm)
{
	if (cm > 10.0 && cm < 40.0)
		return true;

	return false;
}

unsigned long firstCounter = 0;
bool bFast = false;
void doAutoPilot()
{
	if (driveState == Driving)
	{
		if (firstCounter == 0)
			firstCounter = millis();
		double test = ultraSonic.detectCM();
		if (isInRange(test))
		{
			driveState = DriveState::Coasting;
			stop();
			bFast = false;
			firstCounter = 0;
			setSpeed(SLOW);
		}
		else if (isInLongRange(test))
		{
			bFast = false;
			firstCounter = 0;
			setSpeed(SLOW);
		}
		else if (bFast == false)
		{
			unsigned long current = millis();
			if (current - firstCounter > 1000)
			{
				setSpeed(FAST);
				bFast = true;
			}
		}
	}
	else if (driveState == Coasting)
	{
		double test = ultraSonic.detectCM();
		if (isInRange(test))
		{
			setSpeed(FAST);
			right();
			delay(COAST_TIME / 4);
		}
		else 
		{
			setSpeed(SLOW);
			forward();
			driveState = Driving;
		}

	}
}

void stop()
{
	motorsLeft.coast();
	motorsRight.coast();
}

void forward()
{
	motorsLeft.spinLeft();
	motorsRight.spinRight();
}

void backward()
{
	motorsLeft.spinRight();
	motorsRight.spinLeft();
}

void left()
{
	motorsLeft.spinLeft();
	motorsRight.spinLeft();
}

void right()
{
	motorsLeft.spinRight();
	motorsRight.spinRight();
}

void setSpeed(unsigned int speed)
{
	motorsLeft.setSpeed(speed);
	motorsRight.setSpeed(speed);
}
