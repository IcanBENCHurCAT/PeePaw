#include "MotorDriver.h"
#include "UltraSonic.h"
#include <IRremote.h>
#include <IRremoteInt.h>
#include <Servo.h>
//Constant Data
int MOTOR_LEFT_ENABLE = 5;
int MOTOR_LEFT_1 = 6;
int MOTOR_LEFT_2 = 7;
int MOTOR_RIGHT_ENABLE = 9;
int MOTOR_RIGHT_1 = 11;
int MOTOR_RIGHT_2 = 10;
int IR_PIN = 3;

int SENSOR_PIN_RX = 2;
int SENSOR_PIN_TX = 1;

int SERVO_PIN = 4;

const char COMMAND_LEFT = 'L';
const char COMMAND_RIGHT = 'R';
const char COMMAND_FORWARD = 'F';
const char COMMAND_BACKWARD = 'B';
const char COMMAND_STOP = 'S';

const unsigned long COAST_TIME = 128L;

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

const TransmitMode mode = Infrared;



//Variable Data
char data = 0; //Bluetooth data

DriveState driveState = Coasting;

IRrecv receiver(IR_PIN);
decode_results results;

MotorDriver motorsLeft(MOTOR_LEFT_1, MOTOR_LEFT_2, MOTOR_LEFT_ENABLE);
MotorDriver motorsRight(MOTOR_RIGHT_1, MOTOR_RIGHT_2, MOTOR_RIGHT_ENABLE);

UltraSonic ultraSonic(SENSOR_PIN_RX, SENSOR_PIN_TX);
Servo servo;
int servoPosition = 0;


void setup() {

	Serial.begin(9600);

	if (mode == Infrared)
	{
		receiver.enableIRIn();
	}
	else if (mode == AutoPilot)
	{

	}

	pinMode(SENSOR_PIN_RX, INPUT);
	pinMode(SENSOR_PIN_TX, OUTPUT);
	servo.attach(SERVO_PIN);

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

void doAutoPilot()
{

	if (driveState == Driving)
	{
		double test = ultraSonic.detectCM();
		if (test > 0.00 && test < 10)
		{
			//Avoid obstacle
			driveState = DriveState::Coasting;
			stop();

			//Search for new path using US
		}
	}
	else if (driveState == Coasting)
	{

		servoPosition = 90;
		servo.write(servoPosition);
		delay(1000);
		double test = ultraSonic.detectCM();
		if (test < 0.001 || test > 10)
		{
			right();
			servoPosition = 0;
			servo.write(servoPosition);
			delay(COAST_TIME);
			forward();
			driveState = Driving;
		}
		else 
		{
			servoPosition = -90;
			servo.write(servoPosition);
			delay(1000);
			test = ultraSonic.detectCM();
			if (test < 0.001 || test > 10)
			{
				left();
				servoPosition = 0;
				servo.write(servoPosition);
				delay(COAST_TIME);
				forward();
				driveState = Driving;
			}
		}

		servoPosition = 0;
		servo.write(servoPosition);

		if (driveState != Driving)
		{
			backward();
			delay(COAST_TIME);
		}

	}
	/*if (test == LOW)
	{
		Serial.println("Object Detected");
		delay(5000);
	}
	else
	{
		Serial.println("All Clear");
		delay(5000);
	}*/

	/*if (test == LOW && driveState == Coasting)
	{
		driveState = Turning;
		left();
		delay(COAST_TIME);
		return;
	}
	else if (test == LOW && driveState == Driving)
	{
		driveState = Coasting;
		stop();
		delay(COAST_TIME / 2);
		backward();
		delay(COAST_TIME / 2);
		left();
		delay(COAST_TIME);
		return;
	}
	else if(driveState != Driving && test == HIGH)
	{
		driveState = Driving;
		forward();
		return;
	}*/
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
