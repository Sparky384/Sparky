#include "DriveSystem.h"

DriveSystem::DriveSystem():
	myRobot(9, 7, 8, 6),	//prototype: (2, 1)			| final: (1, 7, 2, 8)
	stick1(1),
	stick2(2),
	gyro(2),				//prototype: (2)			| final:
	//adxl(1, 5, 6, 7, 8),	//prototype: (1, 5, 6, 7, 8)| final:
	enc(1, 2),				//prototype: (1, 2)			| final: (1, 2)?
	enc2(13, 14),			//prototype: (13, 14)		| final: (13, 14)?
	ds(DriverStation::GetInstance()),
	dsLCD(DriverStationLCD::GetInstance()),
	climbenc(3, 4),			//prototype: (3, 4)			| final:			***PORT 11 IS BENT, THATS WHY IT IS PORT 4***
	ls(10),					//prototype: (10)
	reverse(5),				//prototype: (5)			| final:
	arm1(10),				//prototype: (10)			| final: (10)
	//arm2(4),				//prototype: (4)			| final:
	dumparm(3),				//prototype: (3)			| final: (3)
	dumpbuck(4),			//prototype: (4)			| final: (4)
	basehook(41),			//prototype: ( )			| final:
	pogo(1)
{
	myRobot.SetExpiration(0.1);
	//pogoforward = new Relay(8, 1);
	//pogobackward = new Relay(8, 2); // on the final IT NEEDS TO GO INTO MODULE 3
	//pogo2 = new Relay(1);
	enc.Reset();
	enc2.Reset();
	enc.Start();
	enc2.Start();
	climbenc.Reset();
	climbenc.Start();
}

bool DriveSystem::EncDriveLimit(int distance)
{
	if (abs((enc.Get() + enc2.Get())/2) <= distance)
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
		myRobot.Drive(-0.45, 0.0); 	// prototype put a negative to go forward (-0.45)
	}								// on the final don't put a negative to go forward
	Wait(0.05);
}

void DriveSystem::RTurn(double angle)
{
	while(gyro.GetAngle() <= angle && gyro.GetAngle() >= -angle)
	{
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
		dsLCD->UpdateLCD();
		myRobot.Drive(-0.15, 1.0);
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
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Encoder: %i", enc.Get());
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Encoder 2: %i", enc2.Get());
		dsLCD->UpdateLCD();
		myRobot.Drive(-0.15, -1.0);
	}
	enc.Reset();
	enc2.Reset();
	gyro.Reset();
	myRobot.Drive(0.0, 0.0);
	Wait(1.0);
}

void DriveSystem::Stop()
{
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
	myRobot.Drive(0.0, 0.0);
}

void DriveSystem::Reset()
{
	enc.Reset();
	enc2.Reset();
	climbenc.Reset();
	gyro.Reset();
}

void DriveSystem::GyroReset()
{
	gyro.Reset();
}


void DriveSystem::EncReset()
{
	enc.Reset();
	enc2.Reset();
	climbenc.Reset();
}

bool DriveSystem::InvertMotors(bool tf)
{
	myRobot.SetInvertedMotor(myRobot.kRearRightMotor, tf);
	myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, tf);
	myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, tf);
	myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, tf);
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
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Encoder2: %i", enc2.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "ClimbEnc: %i", climbenc.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Solenoid: %i", pogo.Get());
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
	myRobot.ArcadeDrive(0.0, 0.0);
}

void DriveSystem::PrintGyro()
{
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
}

void DriveSystem::ArmOneVal(float value)
{
	arm1.Set(value);
}

void DriveSystem::ArmTwoVal(float value)
{
	//arm2.Set(value);
}

void DriveSystem::ServoVal(float value)
{
	reverse.SetAngle(value);
}

void DriveSystem::ArmOneDisable()
{
	arm1.Disable();
}

void DriveSystem::ArmTwoDisable()
{
	//arm2.Disable();
}
void DriveSystem::DumperForward()
{
	dumpbuck.Set(-0.5); // prototype and final forward is the same
}
void DriveSystem::DumperBackward()
{
	dumpbuck.Set(0.5); // prototype and final backward is the same
}

void DriveSystem::DumperArm()
{
	//dumperarm.ArcadeDrive(stick2); // Drift is around 0.007 in both directions
}
void DriveSystem::DumperArmForward()
{
	dumparm.Set(-0.85);
}
void DriveSystem::DumperArmBackward()
{
	dumparm.Set(0.66);
}
void DriveSystem::NoDumper()
{
	dumparm.Set(0.0);
	dumpbuck.Set(0.0);
}

void DriveSystem::ForwardGrappler()
{
	//reverse.SetAngle(170.0);
	//Wait(0.25);
	arm1.Set(1.0);
}

void DriveSystem::BackwardGrappler()
{
	arm1.Set(-1.0);
}

void DriveSystem::NoGrappler()
{
	arm1.Set(0.0);
}
void DriveSystem::Dump()
{
	dumparm.Set(-1.0);
	Wait(0.7);
	dumparm.Set(0.0);
	Wait(1.5);
	dumparm.Set(0.75);
	Wait(0.2);
	dumparm.Set(0.0);
	Wait(3.0);
	/*dumpbuck.Set(0.5);
	Wait(1.0);
	dumpbuck.Set(0.0);*/
	/*Wait(0.1); // "Getting out of the way" so other robots can score
	RTurn(55.0);
	Drive(2300);*/
}
void DriveSystem::AutoForward()
{
	while(climbenc.Get() <= 400)
	{
		reverse.SetAngle(0.0);
		Wait(0.25);
		arm1.Set(1.0);
		Wait(1.0);
	}
	arm1.Set(0.0);
	Wait(0.1);
}

void DriveSystem::ClimbSequence()
{
	if(ls.Get() == 1)
	{
		if(stick2.GetRawButton(10))
		{
			climbenc.Reset();
			reverse.Set(0.0);
			Wait(0.05);
			arm1.Set(1.0);
			while(climbenc.Get() < 70)
			{
				arm1.Set(1.0);
			}
			Wait(0.05);
			arm1.Set(0.0);
			Wait(0.05);
			reverse.Set(170.0); // theoretically low gear
			arm1.Set(-1.0);
			while(climbenc.Get() > 0)
			{
				arm1.Set(-1.0);
			}
			arm1.Set(0.0);
			Wait(0.05);
			basehook.Set(true);
			Wait(0.5);
			reverse.Set(0.0); //shifting into high gear
			Wait(0.05);
			arm1.Set(1.0);
			while(climbenc.Get() < 200)
			{
				arm1.Set(1.0);
			}
			arm1.Set(0.0);
			Wait(0.05);
			reverse.Set(170.0); // shifting into low gear
			Wait(0.5);
			arm1.Set(-1.0);
			Wait(0.5);
			basehook.Set(false);
			while(climbenc.Get() > 0) // MAKE SURE TO INPUT POGO CODE. TEST IN TELEOP FIRST!!!!
			{
				arm1.Set(-1.0);
			}
			arm1.Set(0.0);
			Wait(0.05);
			basehook.Set(true);
			Wait(0.05);
			reverse.Set(0.0); // shifting into high gear
			Wait(0.5);
			arm1.Set(1.0);
			while(climbenc.Get() < 200)
			{
				arm1.Set(1.0);
			}
			arm1.Set(0.0);
			Wait(0.05);
			basehook.Set(true);
			Wait(0.05);  
			/*HERE*/
			reverse.Set(170.0); // shifting into low gear
			Wait(0.5);
			arm1.Set(-1.0);
			basehook.Set(false);
			while(climbenc.Get() > 0)
			{
				arm1.Set(-1.0);
			}
			arm1.Set(0.0);
			Wait(0.05);
			basehook.Set(true);
			Wait(0.05);
		}
	}
}

void DriveSystem::Basehook(bool tf)
{
	basehook.Set(tf);
}

void DriveSystem::Pogo(bool tf)
{
	pogo.Set(tf);
}

void DriveSystem::SolenoidTest()
{
	Solenoid *s[8];
	
	for(int i = 0; i < 8; i++)
	{
		s[i] = new Solenoid(i + 1);
	}
	for(int i = 0; i < 8; i++)
	{
		s[i]->Set(true);
	}
	for(int i = 0; i < 8; i++)
	{
		s[i]->Set(false);
		Wait(1.0);
	}
	for(int i = 0; i < 8; i++)
	{
		s[i]->Set(true);
		Wait(1.0);
		delete s[i];
	}
}

void DriveSystem::RPogoOn()
{
	//pogo2->Set(Relay::kForward);
}

void DriveSystem::RPogoOff()
{
	//pogo2->Set(Relay::kReverse);
}
