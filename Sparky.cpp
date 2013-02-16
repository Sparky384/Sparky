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
	Relay *blinkylight;
	DriverStation *ds;
	DriverStationLCD *dsLCD;
	DriveSystem sparky;
	Compressor *compressor;
	
public:
	RobotDemo(void):
		stick1(1),		// as they are declared above.
		stick2(2),
		ds(DriverStation::GetInstance()),
		dsLCD(DriverStationLCD::GetInstance()),
		sparky()
	{
		blinkylight = new Relay(1);
		compressor = new Compressor(2, 3); // CHANGE THESE PORTS
		compressor->Start();
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
				sparky.Drive(1572);
				sparky.LTurn(35.0);
				sparky.Drive(2377);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(3)) // QUADRANT 3 - DRIVE STRAIGHT, TURN 90 DEGREES, THEN IMPLEMENT INPUT 2
			{
				sparky.Reset();
				sparky.Drive(2377);
				sparky.RTurn(90.0);
				sparky.Drive(1572);
				sparky.LTurn(35.0);
				sparky.Drive(2377);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(4)) //DRIVE STRAIGHT, THEN TURN 45 TO GOAL
			{
				sparky.Reset();
				sparky.Drive(2242);
				sparky.RTurn(45.0);
				sparky.Drive(2300);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(5)) //DRIVE UNDER PYRAMID, TURN 90, THEN INPUT 2 CODE
			{
				sparky.Reset();
				sparky.Drive(1610);
				sparky.RTurn(90.0);
				sparky.Drive(1073);
				sparky.LTurn(35.0);
				sparky.Drive(2300);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(6)) //TURN LEFT 90, DRIVE, THEN TURN LEFT 90, IMPLEMENT INPUT 4 CODE
			{
				sparky.Drive(460);
				sparky.LTurn(90.0);
				sparky.Drive(1150);
				sparky.LTurn(90.0);
				sparky.Drive(2242);
				sparky.RTurn(45.0);
				sparky.Drive(2300);
				sparky.Stop();
			}
			else if(ds->GetDigitalIn(7)) // SHORT STRAIGHT DRIVE, TURN 90, FORWARD, 45 TURN, DRIVE TO GOAL
			{
				sparky.Drive(460);
				sparky.RTurn(90.0);
				sparky.Drive(1073);
				sparky.LTurn(35.0);
				sparky.Drive(1610);
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
		
		while (true)
		{
			sparky.GyroFixAngles();
			sparky.Printlines();
			
			if(stick1.GetRawButton(7))
			{
				sparky.GyroReset();
			}
			if(stick1.GetRawButton(9))
			{
				sparky.EncReset();
			}
			
			sparky.InvertMotors(true);
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
			if (stick1.GetRawButton(5) == true || stick2.GetRawButton(5))
			{
				sparky.ClimbTower();
			}
			blinkylight->Set(Relay::kForward);
			Wait(0.005);				// Wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
