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
	AxisCamera *camera;
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
		camera = &AxisCamera::GetInstance("10.3.84.11");
		camera->WriteResolution(AxisCameraParams::kResolution_320x240);
		camera->WriteWhiteBalance(AxisCameraParams::kWhiteBalance_Hold);
		camera->WriteExposureControl(AxisCameraParams::kExposure_Hold);
		camera->WriteColorLevel(100);
		camera->WriteCompression(30);
		camera->WriteBrightness(30);
		camera->WriteMaxFPS(10);
	}
	
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
				sparky.Printlines();
				sparky.Drive(900);
				sparky.RTurn(90.0);
				sparky.Drive(900);
				sparky.LTurn(38.0);
				sparky.Drive(1200);
				sparky.Stop();
				sparky.Dump();
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(4)) //DRIVE STRAIGHT, THEN TURN 45 TO GOAL
			{
				sparky.Printlines();
				sparky.Drive(2775);
				sparky.RTurn(30.0);
				sparky.Drive(900);
				sparky.Stop(); // COMPLETE!
				sparky.Dump();
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(5)) //DRIVE UNDER PYRAMID, TURN 90, THEN INPUT 2 CODE
			{
				sparky.Printlines();
				sparky.Drive(1750);
				sparky.RTurn(50.0); // COMPLETE!
				sparky.Drive(2175);
				sparky.Stop();
				sparky.Dump();
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(6)) // ANGLE OFF CENTER FROM THE BACK.
			{
				sparky.Printlines();
				sparky.Drive(3600); // COMPLETE!
				sparky.Stop();
				sparky.Dump();
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(7)) // SHORT STRAIGHT DRIVE, TURN 90, FORWARD, 45 TURN, DRIVE TO GOAL
			{
				sparky.Printlines();
				sparky.Drive(460);
				sparky.RTurn(90.0);
				sparky.Drive(1073);
				sparky.LTurn(35.0);
				sparky.Drive(1610);
				sparky.Stop();
				sparky.Dump();
				Wait(15.0);
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
		sparky.BasehookInit();
		bool bt = true;
		//bool pt = true;
		bool bflip = false;
		bool pflip = false;
		//bool climb = false;
		while (true)
		{
			
			//sparky.BasehookSwitch(bflip);
			//sparky.PogoSwitch(pflip);
			/*
			if(stick2.GetRawButton(10) && pt)
			{
				pflip = !pflip;
				pt = false;
			}
			else if(!stick2.GetRawButton(10))
			{
				pt = true;
			}
			*/
			/*
			if(stick2.GetRawButton(5))
			{
				//basetoggle = sparky.BasehookToggle(basetoggle);
				sparky.BasehookSwitch(true);
			}
			*/
			/*else if(!stick2.GetRawButton(2))
			{
				basetoggle = true;
			}
			if(stick2.GetRawButton(4))
			{
				//pogotoggle = sparky.PogoToggle(pogotoggle);
				sparky.BasehookSwitch(false);
			}
			else if(!stick2.GetRawButton(3))
			{
				pogotoggle = true;
			}*/
			if(stick1.GetRawButton(10))
			{
				sparky.FirstClimb();
				//climb = true;
				bflip = true;
				pflip = false;
			}
			else if(stick1.GetRawButton(11))
			{
				sparky.SecondClimb();
				bflip = true;
				pflip = false;
			}
			else
			{
				sparky.BasehookSwitch(bflip);
				if(stick2.GetRawButton(11) && bt)
				{
					bflip = !bflip;
					bt = false;
				}
				else if(!stick2.GetRawButton(11))
				{
					bt = true;
				}
				//sparky.PogoSwitch(pflip);
			}
			if(stick1.GetRawButton(6) && stick1.GetRawButton(7))
			{
				//killswitch = sparky.KillClimb();
			}
			sparky.GyroFixAngles();
			sparky.Printlines();
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
			if(stick2.GetRawButton(5))
			{
				sparky.DumperArmForward();
			}
			else if(stick2.GetRawButton(4))
			{
				sparky.DumperArmBackward();
			}
			else
			{
				sparky.NoDumper();
			}
			if(stick2.GetRawButton(2))
			{
				sparky.ShooterPower(1.0); // full speed shooter
			}
			else if(stick2.GetRawButton(3))
			{
				sparky.ShooterPower(0.5); // half speed shooter
			}
			else{}
			if(stick2.GetTrigger())
			{
				sparky.ShooterPiston();
			}
			else{}
			Wait(0.005);				// Wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
