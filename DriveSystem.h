#ifndef DRIVE_SYSTEM_H
#define DRIVE_SYSTEM_H

#include "WPILib.h"

class DriveSystem
{
private:
	RobotDrive myRobot; // robot drive system
	Joystick stick1, stick2; // only joystick
	Gyro gyro;
	ADXL345_SPI adxl;
	DigitalInput trigger;
	Encoder enc;
	Encoder enc2;
	DriverStation *ds;
	DriverStationLCD *dsLCD;
	Encoder climbenc;
	DigitalInput ls;

	
public:
	DriveSystem(void);
	bool EncDriveLimit(int length);
	void Drive(int distance);
	void RTurn(double angle);
	void LTurn(double angle);
	void Stop();
	void Reset();
	void GyroReset();
	void ClimbTower();
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
};

#endif
