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
	//Relay *blinkylight;
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
		//blinkylight = new Relay(1);
		compressor = new Compressor(9, 5); // final: (8,8) DIGITAL SIDECAR PORT 8 IS BAD!!!!
		compressor->Start();
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	
	void Autonomous(void)
	{
		sparky.Safety(false);
		sparky.Reset();
		sparky.GyroSens();
		while(IsAutonomous() && IsEnabled()) // this is a change
		{
			sparky.InvertMotors(true);
			if(ds->GetDigitalIn(1)) //QUADRANT 1 - DRIVE STRAIGHT
			{
				sparky.Printlines();
				sparky.Drive(1200); // 1 foot equals 230 encoder counts
				sparky.Stop();		// this autonomous is complete!
				sparky.Dump();
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(2)) //QUADRANT 2 - DRIVE STRAIGHT THEN TURN LEFT INTO GOAL
			{
				sparky.Printlines();	// this autonomous is complete!
				sparky.Drive(900);
				sparky.Stop();
				sparky.LTurn(38.0);
				sparky.Drive(1200);
				sparky.Stop();
				sparky.Dump();
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(3)) // QUADRANT 3 - DRIVE STRAIGHT, TURN 90 DEGREES, THEN IMPLEMENT INPUT 2
			{
				sparky.Drive(2377);
				sparky.RTurn(90.0);
				sparky.Drive(1572);
				sparky.LTurn(35.0);
				sparky.Drive(2377);
				sparky.Stop();
				sparky.Dump();
			}
			else if(ds->GetDigitalIn(4)) //DRIVE STRAIGHT, THEN TURN 45 TO GOAL
			{
				sparky.Reset();
				sparky.Drive(2242);
				sparky.RTurn(45.0);
				sparky.Drive(2300);
				sparky.Stop();
				sparky.Dump();
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
				sparky.Dump();
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
				sparky.Dump();
			}
			else if(ds->GetDigitalIn(7)) // SHORT STRAIGHT DRIVE, TURN 90, FORWARD, 45 TURN, DRIVE TO GOAL
			{
				sparky.Drive(460);
				sparky.RTurn(90.0);
				sparky.Drive(1073);
				sparky.LTurn(35.0);
				sparky.Drive(1610);
				sparky.Stop();
				sparky.Dump();
			}
			else
			{
				sparky.PrintGyro();
				sparky.Stop();
			}
			sparky.Stop();
		}
		sparky.Stop();
	} // End autonomous Block

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
			//sparky.ClimbSequence();
			sparky.GyroFixAngles();
			sparky.Printlines();
			/*
			if(stick2.GetTrigger() == true)
			{
				sparky.DumperArm();
			}
			else{}
			*/
			/*
			if(!compressor->GetPressureSwitchValue()) // only runs until 120 PSI
			{
				compressor->Start();
			}
			else
			{
				compressor->Stop();
			}
			*/
			if(stick1.GetRawButton(8))
			{
				sparky.GyroReset();
			}
			if(stick1.GetRawButton(9))
			{
				sparky.EncReset();
			}
			
			sparky.InvertMotors(true);
			if(stick1.GetTrigger() == true && stick2.GetTrigger() == true)
			{
				sparky.SparkTank();
			}
			else if(stick1.GetTrigger() == true)
			{
				sparky.SparkFirstArcade();
			}
			else
			{
				sparky.NoMoving();
			}
			
			/*else if(stick2.GetRawButton(11))
			{
				sparky.ServoVal(0.0);
				sparky.ArmOneVal(-1.0);
			}*/
			
			if(stick2.GetRawButton(8))
			{
				sparky.ServoVal(0.0); // Shifting into high speed (?)
			}
			else if(stick2.GetRawButton(9))
			{
				sparky.ServoVal(170.0); // Shifting into low speed (?)
			}
			
			
			if(stick2.GetRawButton(6))
			{
				sparky.ForwardGrappler();
			}
			else if(stick2.GetRawButton(7))
			{
				sparky.BackwardGrappler();
			}
			else
			{
				sparky.NoGrappler();
			}
			
			
			if(stick2.GetRawButton(3))
			{
				sparky.DumperForward();
			}
			else if(stick2.GetRawButton(2))
			{
				sparky.DumperBackward();
			}
			else if(stick2.GetRawButton(4))
			{
				sparky.DumperArmForward();
			}
			else if(stick2.GetRawButton(5))
			{
				sparky.DumperArmBackward();
			}
			else
			{
				sparky.NoDumper();
			}

			if(stick1.GetRawButton(3))
			{
				sparky.PogoForward();
			}
			else if(stick1.GetRawButton(2))
			{
				sparky.PogoBackward();
			}
			else{}
			//blinkylight->Set(Relay::kForward);
			Wait(0.005);				// Wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
