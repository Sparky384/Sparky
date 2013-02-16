#include "DriveSystem.h"

DriveSystem::DriveSystem():
	myRobot(2, 1),	// these must be initialized in the same order
	stick1(1),		// as they are declared above.
	stick2(2),
	gyro(2),
	adxl(1, 5, 6, 7, 8),
	trigger(11),
	enc(1, 2),
	enc2(13, 14),
	ds(DriverStation::GetInstance()),
	dsLCD(DriverStationLCD::GetInstance()),
	climbenc(10, 11), //BOGUS PORT NUMBERS
	ls(4) //BOGUS PORT NUMBERS
{
	myRobot.SetExpiration(0.1);
	enc.Reset();
	enc2.Reset();
	enc.Start();
	enc2.Start();
}

bool DriveSystem::EncDriveLimit(int length)
{
	if (abs((enc.Get() + enc2.Get())/2) <= length)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void DriveSystem::Drive(int distance)
{
	while(EncDriveLimit(distance))
	{
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Encoder Value: %i", (enc.Get() + enc.Get())/2);
		dsLCD->UpdateLCD();
		myRobot.Drive(-1.0, 0.0);
		
	}
	enc.Reset();
	enc2.Reset();
	gyro.Reset();
	myRobot.Drive(0.0, 0.0);
	Wait(1.0);
}

void DriveSystem::RTurn(double angle)
{
	while(gyro.GetAngle() <= angle && gyro.GetAngle() >= -angle)
	{
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
		dsLCD->UpdateLCD();
		myRobot.Drive(0.3, -1.0);
	}
	enc.Reset();
	enc2.Reset();
	gyro.Reset();
	myRobot.Drive(0.0, 0.0);
	Wait(1.0);	
}

void DriveSystem::LTurn(double angle)
{
	while(gyro.GetAngle() <= angle && gyro.GetAngle() >= -angle)
	{
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
		dsLCD->UpdateLCD();
		myRobot.Drive(0.3, 1.0);
	}
	enc.Reset();
	enc2.Reset();
	gyro.Reset();
	myRobot.Drive(0.0, 0.0);
	Wait(1.0);
}

void DriveSystem::Stop()
{
	myRobot.Drive(0.0, 0.0);
	Wait(15.0);
}

void DriveSystem::Reset()
{
	enc.Reset();
	enc2.Reset();
	gyro.Reset();
}

void DriveSystem::GyroReset()
{
	gyro.Reset();
}

void DriveSystem::ClimbTower()
{
	Drive(1);
	LTurn(45);
	while(adxl.GetAccelerations().YAxis <= 68.0 && adxl.GetAccelerations().YAxis >= -68.0) 
	{
		myRobot.Drive(1.0, 0.0);
	}
	
	GyroReset();
	climbenc.Reset();
	//turn on arm
	if(ls.Get() == true)
	{
		//slow down arm
	}
}

void DriveSystem::EncReset()
{
	enc.Reset();
	enc2.Reset();
}

bool DriveSystem::InvertMotors(bool tf)
{
	myRobot.SetInvertedMotor(myRobot.kRearRightMotor, tf);
	myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, tf);
	return tf;
}

bool DriveSystem::Safety(bool tf)
{
	myRobot.SetSafetyEnabled(tf);
	return tf;
}

void DriveSystem::GyroSens()
{
	gyro.SetSensitivity(0.007);
}

void DriveSystem::GyroFixAngles()
{
	float angle = gyro.GetAngle();
	if(gyro.GetAngle() > 360)
	{
		angle-=360.0;
	}
	else if(gyro.GetAngle() < -360)
	{
		angle+=360.0;
	}
}

void DriveSystem::Printlines()
{
	float angle = gyro.GetAngle();
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "Gyro: %f", angle);
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Encoder: %i", enc.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Encoder 2: %i", enc2.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "X: %d", adxl.GetAcceleration(adxl.kAxis_X));
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Y: %d", adxl.GetAcceleration(adxl.kAxis_Y));
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "Z: %d", adxl.GetAcceleration(adxl.kAxis_Z));
	dsLCD->UpdateLCD();
}

void DriveSystem::SparkTank()
{
	myRobot.TankDrive(stick1,stick2);
}

void DriveSystem::SparkFirstArcade()
{
	myRobot.ArcadeDrive(stick1);
}

void DriveSystem::SparkSecondArcade()
{
	myRobot.ArcadeDrive(stick2);
}

void DriveSystem::NoMoving()
{
	myRobot.TankDrive(0.0, 0.0);
}

void DriveSystem::PrintGyro()
{
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
}
