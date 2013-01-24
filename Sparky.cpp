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
	//Accelerometer acc;
	//I2C acc2;

public:
	RobotDemo(void):
		myRobot(2, 3),	// these must be initialized in the same order
		stick1(1),		// as they are declared above.
		stick2(2)
	{
		myRobot.SetExpiration(0.1);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		while(IsAutonomous()) // this is a change
		{
			/*
			gyro.Reset();
			float angle = gyro.GetAngle() + 45; // get the angle from the gyro
			myRobot.Drive(0.5, -angle / 30);
			float speed = acc.GetAcceleration();
			DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
			dsLCD->Clear();
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "Gyro Value: %f", gyro.GetAngle());
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Acceleration: %f", acc.GetAcceleration());
			dsLCD->UpdateLCD();
			*/
			Wait(2.0);
		}
		myRobot.Drive(0.0, 0.0);
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		
		/*
		DigitalModule *dm = DigitalModule::GetInstance(2);
		I2C *isc = dm->GetI2C(0x3A);
		isc->Write(0x2D, 0x08);
		*/
		
		ADXL345_I2C a(2);
		
		while (true)
		{
			myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, false);
			myRobot.SetInvertedMotor(myRobot.kRearRightMotor, false);
			
			/*
			DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "Acceleration: ", acc.GetAcceleration());
			dsLCD->UpdateLCD();
			*/
			
			DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "Acceleration X: ",
					a.GetAcceleration(a.kAxis_X));
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Acceleration Y: ",
					a.GetAcceleration(a.kAxis_Y));
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Acceleration Z: ",
					a.GetAcceleration(a.kAxis_Z));
			dsLCD->UpdateLCD();
			
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
			Wait(0.005);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

