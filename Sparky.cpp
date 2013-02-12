#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick1, stick2; // only joystick
	Gyro gyro;
	//ADXL345_SPI adxl;
	Relay *blinkylight;
	DigitalInput trigger;
	Encoder enc;
	Encoder enc2;
	DriverStation *ds;
	DriverStationLCD *dsLCD;
	
public:
	RobotDemo(void):
		myRobot(2, 1),	// these must be initialized in the same order
		stick1(1),		// as they are declared above.
		stick2(2),
		gyro(2),
		//adxl(5, 6, 7, 8, ADXL345_SPI::kRange_2G),
		trigger(11),
		enc(1, 2),
		enc2(13, 14),
		ds(DriverStation::GetInstance()),
		dsLCD(DriverStationLCD::GetInstance())
	{
		myRobot.SetExpiration(0.1);
		blinkylight = new Relay(1);
		enc.Reset();
		enc2.Reset();
		enc.Start();
		enc2.Start();
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	bool EncDriveLimit(int length)
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
	
	void Drive(int distance)
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
	void RTurn(double angle)
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
	void LTurn(double angle)
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
	void Stop()
	{
		myRobot.Drive(0.0, 0.0);
		Wait (15.0);
	}
	void Reset()
	{
		enc.Reset();
		enc2.Reset();
		gyro.Reset();
	}
	
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		enc.Reset();
		gyro.SetSensitivity(0.007);
		while(IsAutonomous() && IsEnabled()) // this is a change
		{
			myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);
			myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
			enc.Reset();
			gyro.Reset();
			if(ds->GetDigitalIn(1)) //QUADRANT 1 - DRIVE STRAIGHT
			{
				Reset();
				Drive(2300);
				blinkylight->Set(Relay::kForward);
				Stop();
			}
			else if(ds->GetDigitalIn(2)) //QUADRANT 2 - DRIVE STRAIGHT THEN TURN LEFT INTO GOAL
			{
				Reset();
				Drive(1900);
				LTurn(18.0);
				Drive(2450);
				Stop();
			}
			else if(ds->GetDigitalIn(3)) // QUADRANT 3 - DRIVE STRAIGHT, TURN 90 DEGREES, THEN IMPLEMENT INPUT 2
			{
				Reset();
				Drive(200);
				RTurn(90.0);
				Drive(200);
				LTurn(35.0);
				Drive(200);
				Stop();
			}
			else if(ds->GetDigitalIn(4)) //DRIVE STRAIGHT, THEN TURN 45 TO GOAL
			{
				Reset();
				Drive(200);
				RTurn(45.0);
				Drive(200);
				Stop();
			}
			else if(ds->GetDigitalIn(5)) //DRIVE UNDER PYRAMID, TURN 90, THEN INPUT 2 CODE
			{
				Reset();
				Drive(200);
				RTurn(90.0);
				Drive(200);
				LTurn(35.0);
				Drive(200);
				Stop();
			}
			else if(ds->GetDigitalIn(6)) //TURN LEFT 90, DRIVE, THEN TURN LEFT 90, IMPLEMENT INPUT 4 CODE
			{
				Drive(200);
				LTurn(90.0);
				Drive(200);
				LTurn(90.0);
				Drive(200);
				RTurn(45.0);
				Drive(200);
				Stop();
			}
			else if(ds->GetDigitalIn(7)) // SHORT STRAIGHT DRIVE, TURN 90, FORWARD, 45 TURN, DRIVE TO GOAL
			{
				Drive(200);
				RTurn(90.0);
				Drive(200);
				LTurn(45.0);
				Drive(200);
				Stop();
			}
			else
			{
				dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
				Stop();
			}
			Stop();
		}
		Stop();
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		gyro.Reset();
		gyro.SetSensitivity(0.0062);
		myRobot.SetSafetyEnabled(true);
		bool motorinv = true;
		
		while (true)
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

			dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Gyro: %f", angle);
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Encoder: %i", enc.Get());
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Encoder 2: %i", enc2.Get());
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "Motorinv: %d", motorinv);
			dsLCD->UpdateLCD();
			
			if(stick1.GetRawButton(2) || stick2.GetRawButton(2))
			{
				if(motorinv == false)
				{
					motorinv = true;
				}
				else if(motorinv == true)
				{
					motorinv = false;
				}
				else
				{
					motorinv = motorinv;
				}
			}
			
			if(stick1.GetRawButton(7))
			{
				gyro.Reset();
			}
			if(stick1.GetRawButton(9))
			{
				enc.Reset();
				enc2.Reset();
			}
			
			myRobot.SetInvertedMotor(myRobot.kRearRightMotor, motorinv);
			myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, motorinv);
			if((stick1.GetTrigger() == true) && (stick2.GetTrigger() == true))
			{
				myRobot.TankDrive(stick1,stick2);
			}
			else if(stick1.GetTrigger() == true && (stick2.GetTrigger() == false))
			{
				myRobot.ArcadeDrive(stick1);
				}
			else if(stick2.GetTrigger() == true && (stick1.GetTrigger() == false))
			{
				myRobot.ArcadeDrive(stick2);
			}
			else if(stick1.GetTrigger() == true && stick2.GetTrigger() == true && (stick1.GetRawButton(3) || stick2.GetRawButton(3)))
			{
				stick1.GetX()/2;
				stick1.GetY()/2;
				stick2.GetX()/2;
				stick2.GetY()/2;
				myRobot.TankDrive(stick1, stick2);
			}
			else if((stick1.GetTrigger() == true) && (stick2.GetTrigger() == false) && (stick1.GetRawButton(3) == true))
			{
				stick1.GetX()/2;
				stick1.GetY()/2;
				myRobot.ArcadeDrive(stick1);
			}
			else if(stick1.GetTrigger() == false && stick2.GetTrigger() == true && stick2.GetRawButton(3) == true)
			{
				stick2.GetX()/2;
				stick2.GetY()/2;
				myRobot.ArcadeDrive(stick2);
			}
			else
			{
				myRobot.TankDrive(0.0, 0.0);
			}
			blinkylight->Set(Relay::kForward);
			Wait(0.005);				// Wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

