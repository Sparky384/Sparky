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
	Solenoid pogo1, pogo2, pogofwd, pogorev, basehook1fwd, basehook1rev, basehook2fwd, basehook2rev;
	// Wire solenoid breakout to 24 Volts
public:
	RobotDemo(void):
		stick1(1),		// as they are declared above.
		stick2(2),
		ds(DriverStation::GetInstance()),
		dsLCD(DriverStationLCD::GetInstance()),
		sparky(),
		pogo1(1),
		pogo2(2),
		pogofwd(3),
		pogorev(4),
		basehook1fwd(5),
		basehook1rev(6),
		basehook2fwd(7),
		basehook2rev(8)
	{
		//blinkylight = new Relay(1);
		compressor = new Compressor(9, 5); // final: (8,8) DIGITAL SIDECAR PORT 8 IS BAD!!!!
		compressor->Start();
		/*pogo0 = new Solenoid(0);
		pogo1 = new Solenoid(1);
		pogo2 = new Solenoid(2);
		pogo3 = new Solenoid(3);
		pogo4 = new Solenoid(4);
		pogo5 = new Solenoid(5);
		pogo6 = new Solenoid(6);
		pogo7 = new Solenoid(7);*/
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
		basehook1fwd.Set(false);
		basehook2fwd.Set(false);
		basehook1rev.Set(true);
		basehook2rev.Set(true);
		pogofwd.Set(false);
		pogorev.Set(true);
		bool basetoggle = true;
		bool pogotoggle = true;
		while (true)
		{
			if(stick1.GetRawButton(2) && basetoggle)
			{
				basehook1fwd.Set(!basehook1fwd.Get());
				basehook2fwd.Set(!basehook2fwd.Get());
				basehook1rev.Set(!basehook1rev.Get());
				basehook2rev.Set(!basehook2rev.Get());
				basetoggle = false;
			}
			else if(!stick1.GetRawButton(2))
			{
				basetoggle = true;
			}
			if(stick1.GetRawButton(3) && pogotoggle)
			{
				pogofwd.Set(!pogofwd.Get());
				pogorev.Set(!pogorev.Get());
				pogotoggle = false;
			}
			else if(!stick1.GetRawButton(3))
			{
				pogotoggle = true;
			}
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
				sparky.ServoVal(0.0); // Shifting into high speed
			}
			else if(stick2.GetRawButton(9))
			{
				sparky.ServoVal(170.0); // Shifting into low speed
			}
			if(sparky.LSGet() == 1)
			{
				sparky.ClimberEncReset();
			}
			else{}
			if(stick2.GetRawButton(6))
			{
				sparky.ForwardGrappler();
			}
			else if(stick2.GetRawButton(7) && sparky.LSGet() == 0)
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
			/*
			if(stick1.GetRawButton(3))
			{
				sparky.Pogo(true);
			}
			else if(stick1.GetRawButton(2))
			{
				sparky.Pogo(false);
			}
			else{}
			*/
			//blinkylight->Set(Relay::kForward);
			Wait(0.005);				// Wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
