#include "WPILib.h"
#include "DriveSystem.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	Joystick stick1, stick2; // only joystick
	//ADXL345_SPI adxl;
	Relay *blinkylight;
	DigitalInput trigger;
	DriverStation *ds;
	DriverStationLCD *dsLCD;
	DriveSystem sparky;
	
public:
	RobotDemo(void):
		stick1(1),		// as they are declared above.
		stick2(2),
		//adxl(5, 6, 7, 8, ADXL345_SPI::kRange_2G),
		trigger(11),
		ds(DriverStation::GetInstance()),
		dsLCD(DriverStationLCD::GetInstance()),
		sparky()
	{
		blinkylight = new Relay(1);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	
	void Autonomous(void)
	{
		sparky.Safety(false);
		sparky.GyroReset();
		sparky.GyroSens();
		while(IsAutonomous() && IsEnabled()) // this is a change
		{
			sparky.InvertMotors(true);
			sparky.GyroReset();
			if(ds->GetDigitalIn(1)) //QUADRANT 1 - DRIVE STRAIGHT
			{
				sparky.Reset();
				sparky.Drive(2300);
				blinkylight->Set(Relay::kForward);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(2)) //QUADRANT 2 - DRIVE STRAIGHT THEN TURN LEFT INTO GOAL
			{
				sparky.Reset();
				sparky.Drive(1900);
				sparky.LTurn(18.0);
				sparky.Drive(2450);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(3)) // QUADRANT 3 - DRIVE STRAIGHT, TURN 90 DEGREES, THEN IMPLEMENT INPUT 2
			{
				sparky.Reset();
				sparky.Drive(200);
				sparky.RTurn(90.0);
				sparky.Drive(200);
				sparky.LTurn(35.0);
				sparky.Drive(200);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(4)) //DRIVE STRAIGHT, THEN TURN 45 TO GOAL
			{
				sparky.Reset();
				sparky.Drive(200);
				sparky.RTurn(45.0);
				sparky.Drive(200);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(5)) //DRIVE UNDER PYRAMID, TURN 90, THEN INPUT 2 CODE
			{
				sparky.Reset();
				sparky.Drive(200);
				sparky.RTurn(90.0);
				sparky.Drive(200);
				sparky.LTurn(35.0);
				sparky.Drive(200);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(6)) //TURN LEFT 90, DRIVE, THEN TURN LEFT 90, IMPLEMENT INPUT 4 CODE
			{
				sparky.Drive(200);
				sparky.LTurn(90.0);
				sparky.Drive(200);
				sparky.LTurn(90.0);
				sparky.Drive(200);
				sparky.RTurn(45.0);
				sparky.Drive(200);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(7)) // SHORT STRAIGHT DRIVE, TURN 90, FORWARD, 45 TURN, DRIVE TO GOAL
			{
				sparky.Drive(200);
				sparky.RTurn(90.0);
				sparky.Drive(200);
				sparky.LTurn(45.0);
				sparky.Drive(200);
				sparky.Stop();
			}
			else
			{
				sparky.PrintGyro();
				sparky.Stop();
			}
			sparky.Stop();
		}
		sparky.Stop();
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		sparky.Reset();
		sparky.GyroSens();
		sparky.Safety(true);
		bool motorinv = true;
		
		while (true)
		{
			sparky.GyroFixAngles();
			sparky.Printlines();
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "Motorinv: %d", motorinv);
			
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
				sparky.GyroReset();
			}
			if(stick1.GetRawButton(9))
			{
				sparky.EncReset();
			}
			
			sparky.InvertMotors(motorinv);
			if((stick1.GetTrigger() == true) && (stick2.GetTrigger() == true))
			{
				sparky.SparkTank();
			}
			else if(stick1.GetTrigger() == true && (stick2.GetTrigger() == false))
			{
				sparky.SparkFirstArcade();
			}
			else if(stick2.GetTrigger() == true && (stick1.GetTrigger() == false))
			{
				sparky.SparkSecondArcade();
			}
			else if(stick1.GetTrigger() == true && stick2.GetTrigger() == true && (stick1.GetRawButton(3) || stick2.GetRawButton(3)))
			{
				stick1.GetX()/2;
				stick1.GetY()/2;
				stick2.GetX()/2;
				stick2.GetY()/2;
				sparky.SparkTank();
			}
			else if((stick1.GetTrigger() == true) && (stick2.GetTrigger() == false) && (stick1.GetRawButton(3) == true))
			{
				stick1.GetX()/2;
				stick1.GetY()/2;
				sparky.SparkFirstArcade();
			}
			else if(stick1.GetTrigger() == false && stick2.GetTrigger() == true && stick2.GetRawButton(3) == true)
			{
				stick2.GetX()/2;
				stick2.GetY()/2;
				sparky.SparkSecondArcade();
			}
			else
			{
				sparky.NoMoving();
			}
			blinkylight->Set(Relay::kForward);
			Wait(0.005);				// Wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);