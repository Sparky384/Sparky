#ifndef DRIVE_SYSTEM_H
#define DRIVE_SYSTEM_H

#include "WPILib.h"

class DriveSystem
{
private:
	RobotDrive myRobot; // robot drive system
	Joystick stick1, stick2; // only joystick
	Gyro gyro;
	//ADXL345_SPI adxl;
	Encoder enc;
	Encoder enc2;
	DriverStation *ds;
	DriverStationLCD *dsLCD;
	Encoder climbenc;
	DigitalInput ls;
	Servo reverse;
	Victor arm1, dumparm, dumpbuck;
	Solenoid basehook;
	Relay *pogoforward; // ***PROTOTYPE IS RELAY WHILE FINAL IS SOLENOIDS***
	Relay *pogobackward;
	
	

	
public:
	DriveSystem(void);
	bool EncDriveLimit(int length);
	void Drive(int distance);
	void RTurn(double angle);
	void LTurn(double angle);
	void Stop();
	void Reset();
	void GyroReset();
	void EncReset();
	bool InvertMotors(bool tf);
	bool Safety(bool tf);
	void GyroSens();
	void GyroFixAngles();
	void Printlines();
	void SparkTank();
	void SparkFirstArcade();
	void SparkSecondArcade();
	void NoMoving();
	void PrintGyro();
	void ArmOneVal(float value);
	void ArmTwoVal(float value);
	void ServoVal(float value);
	void ArmOneDisable();
	void ArmTwoDisable();
	void DumperForward();
	void DumperBackward();
	void DumperArmForward();
	void DumperArmBackward();
	void DumperArm();
	void NoDumper();
	void ForwardGrappler();
	void BackwardGrappler();
	void NoGrappler();
	void Dump();
	void AutoForward();
	void ClimbSequence();
	void Basehook(bool tf);
	void PogoForward();
	void PogoBackward();
};

#endif
