#include "DriveSystem.h"

DriveSystem::DriveSystem():
	myRobot(9, 7, 8, 6),	//prototype: (2, 1)			| final: (1, 7, 2, 8)
	stick1(1),
	stick2(2),
	gyro(1),				//prototype: (2)			| final:
	//adxl(1, 5, 6, 7, 8),	//prototype: (1, 5, 6, 7, 8)| final:
	enc(1, 2),				//prototype: (1, 2)			| final: (1, 2)?
	enc2(100, 101),			//prototype: (13, 14)		| final: (13, 14)?
	ds(DriverStation::GetInstance()),
	dsLCD(DriverStationLCD::GetInstance()),
	climbenc(13, 14),			//prototype: (3, 4)			| final:			***PORT 11 IS BENT, THATS WHY IT IS PORT 4***
	ls(10),					//prototype: (10)
	reverse(5),				//prototype: (5)			| final:
	climbmotor(10),			//prototype: (10)			| final: (10)
	//arm2(4),				//prototype: (4)			| final:
	dumparm(100),			//prototype: (3)			| final: (3)
	dumpbuck(101),			//prototype: (4)			| final: (4)
	shooter1(3),
	shooter2(4),
	pogofwd(3),
	pogorev(4),
	basehookfwd(5),
	basehookrev(6),
	minipogofwd(7),
	minipogorev(8),
	shooterfwd(1),
	shooterrev(2)
{
	myRobot.SetExpiration(0.1);
	//pogoforward = new Relay(8, 1);
	//pogobackward = new Relay(8, 2); // on the final IT NEEDS TO GO INTO MODULE 3
	//pogo2 = new Relay(1);
	enc.Reset();
	enc2.Reset();
	climbenc.Reset();
	enc.Start();
	enc2.Start();
	climbenc.Start();
	//light = new Relay(1);
	shooterlight = new Relay(4);
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
		myRobot.Drive(-0.30, 0.0); 	// prototype put a negative to go forward (-0.45)
	}								// on the final don't put a negative to go forward
	Wait(0.05);
}

void DriveSystem::RTurn(double angle)
{
	while(gyro.GetAngle() <= angle && gyro.GetAngle() >= -angle)
	{
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
		dsLCD->UpdateLCD();
		myRobot.Drive(-0.17, 1.0);
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
		myRobot.Drive(-0.3, -1.0);
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

int DriveSystem::ConvertLS()
{
	if(ls.Get() == true)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void DriveSystem::Printlines()
{
	float angle = gyro.GetAngle();
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "Gyro: %f", angle);
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Throttle: %f", stick2.GetZ());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Encoder: %i", enc.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Encoder2: %i", enc2.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "ClimbEnc: %i", climbenc.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "limit switch: %i", ConvertLS());
	dsLCD->UpdateLCD();
}

void DriveSystem::TeleOpPrintlines(bool LimitSwitchToggle)
{
	float angle = gyro.GetAngle();
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "Gyro: %f", angle);
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Throttle: %f", stick2.GetZ());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Encoder: %i", enc.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Encoder2: %i", enc2.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "ClimbEnc: %i", climbenc.Get());
	dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "limit switch: %i", LimitSwitchToggle);
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
	climbmotor.Set(value);
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
	climbmotor.Disable();
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
	climbmotor.Set(1.0);
}

void DriveSystem::BackwardGrappler()
{
	climbmotor.Set(-1.0);
}

void DriveSystem::NoGrappler()
{
	climbmotor.Set(0.0);
}
void DriveSystem::Dump()
{
	dumparm.Set(-1.0);
	Wait(0.6);
	dumparm.Set(0.0);
	Wait(1.5);
	dumparm.Set(0.75);
	Wait(0.55);
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
		climbmotor.Set(1.0);
		Wait(1.0);
	}
	climbmotor.Set(0.0);
	Wait(0.1);
}

UINT32 DriveSystem::LSGet()
{
	if(ls.Get() == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void DriveSystem::ClimberEncReset()
{
	climbenc.Reset();
}

void DriveSystem::TenseClimber()
{
	climbmotor.Set(-0.4);
}

void DriveSystem::BasehookInit()
{
	basehookfwd.Set(false);
	basehookrev.Set(true);
	pogofwd.Set(false);
	pogorev.Set(true);
	shooterfwd.Set(false);
	shooterrev.Set(true);
	minipogofwd.Set(false);
	minipogorev.Set(true);
}

void DriveSystem::BasehookToggle()
{
	/*
	bool basetoggle;
	if(stick2.GetRawButton(11) && basetoggle)
	{
		basehook1fwd.Set(!basehook1fwd.Get());
		basehook2fwd.Set(!basehook2fwd.Get());
		basehook1rev.Set(!basehook1rev.Get());
		basehook2rev.Set(!basehook2rev.Get());
		basetoggle = false;
	}
	else if(!stick2.GetRawButton(11))
	{
		basetoggle = true;
	}
	*/
}

void DriveSystem::PogoToggle()
{
	/*
	bool pogotoggle;
	if(stick2.GetRawButton(10) && pogotoggle)
	{
		pogofwd.Set(!pogofwd.Get());
		pogorev.Set(!pogorev.Get());
		pogotoggle = false;
	}
	else if(!stick1.GetRawButton(10))
	{
		pogotoggle = true;
	}
	*/
}

void DriveSystem::BasehookSwitch(bool toggle)
{
	basehookfwd.Set(toggle);
	basehookrev.Set(!toggle);
	
}

void DriveSystem::PogoSwitch(bool toggle)
{
	
	pogofwd.Set(toggle);
	pogorev.Set(!toggle);
	
}

bool DriveSystem::KillClimb()
{
	bool killswitch = false;
	return killswitch;
}

void DriveSystem::FirstClimb()
{
		reverse.Set(170.0);
		while(!ls.Get()) // limit switch returns true when fully retracted
		{
			climbmotor.Set(-1.0); // retracts climber all the way
		}
		climbmotor.Set(0.0); // cuts the climber motor
		BasehookSwitch(true); // engages the basehooks
		/*Wait(1.0);
		while(climbenc.Get() < 200) // extends a bit to put weight on the basehooks
		{
			climbmotor.Set(1.0);
		}*/
}

void DriveSystem::SecondClimb()
{
		/*reverse.Set(0.0); // set into high gear
		Wait(1.0);
		while(climbenc.Get() < 2200) // extend all the way
		{
			climbmotor.Set(1.0);
		}
		climbmotor.Set(0.0); // cut the climber motor
		Wait(1.0);*/ // cannot reliably automate.  Will be done by driver
		reverse.Set(170.0); // set into low gear
		Wait(1.0);
		while(climbenc.Get() > 2000) // retract to take weight off of basehooks
		{
			climbmotor.Set(-1.0);
		}
		BasehookSwitch(false); // disengage basehooks
		//climbmotor.Set(-0.3);
		climbmotor.Set(0.0); //cut the climber motor
		//Wait(0.5);
		PogoSwitch(true); // engage stinger
		Wait(1.0);
		while(climbenc.Get() > 1300) // retract until the bump is under the frame ** 1300 was too much **
		{
			climbmotor.Set(-1.0);
		}
		//climbmotor.Set(0.0); // cut the climber motor
		PogoSwitch(false); // disengage the pogo
		//Wait(1.0); // waits one second
		//FirstClimb(); // starts the first climb code
		while(!ls.Get()) // limit switch returns true when fully retracted
		{
			climbmotor.Set(-1.0); // retracts climber all the way
		}
		climbmotor.Set(0.0); // cuts the climber motor
		BasehookSwitch(true); // engages the basehooks
}

void DriveSystem::climbreset()
{
	climbenc.Reset();
}

void DriveSystem::ShooterFullPower(bool power)
{
	if(power == true)
	{
		shooter1.Set(-1.0);
		shooter2.Set(1.0);
		shooterlight->Set(Relay::kForward);
	}
	else
	{
		shooter1.Set(0.0);
		shooter2.Set(0.0);
		shooterlight->Set(Relay::kOff);
	}
}

void DriveSystem::ShooterSomePower(bool power)
{
	if(power == true)
	{
		shooter1.Set(-0.5);
		shooter2.Set(0.5);
		shooterlight->Set(Relay::kForward);
	}
	else
	{
		shooter1.Set(0.0);
		shooter2.Set(0.0);
		shooterlight->Set(Relay::kOff);
	}
}

void DriveSystem::ShooterPiston()
{
	shooterfwd.Set(true);
	shooterrev.Set(false);
	Wait(0.5);
	shooterfwd.Set(false);
	shooterrev.Set(true);
}

void DriveSystem::MiniPogo(bool toggle)
{
	minipogofwd.Set(toggle);
	minipogorev.Set(!toggle);
}
