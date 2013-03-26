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
		bool sparkyauto = true;
		while(IsAutonomous() && IsEnabled()) // this is a change
		{
			sparky.InvertMotors(true);
			if(ds->GetDigitalIn(1) && sparkyauto == true) //QUADRANT 1 - DRIVE STRAIGHT
			{
				sparky.Printlines();
				sparky.Drive(1200); // 1 foot equals 230 encoder counts
				sparky.Stop();		// this autonomous is complete!
				sparky.Dump();
				sparkyauto = false;
			}
			else if(ds->GetDigitalIn(2) && sparkyauto == true) //QUADRANT 2 - DRIVE STRAIGHT THEN TURN LEFT INTO GOAL
			{
				sparky.Printlines();	// this autonomous is complete!
				sparky.Drive(900);
				sparky.Stop();
				sparky.LTurn(38.0);
				sparky.Drive(1200);
				sparky.Stop();
				sparky.Dump();
				sparkyauto = false;
			}
			else if(ds->GetDigitalIn(3) && sparkyauto == true) // QUADRANT 3 - DRIVE STRAIGHT, TURN 90 DEGREES, THEN IMPLEMENT INPUT 2
			{
				sparky.Printlines();
				sparky.Drive(900);
				sparky.RTurn(90.0);
				sparky.Drive(900);
				sparky.LTurn(38.0);
				sparky.Drive(1200);
				sparky.Stop();
				sparky.Dump();
				sparkyauto = false;
			}
			else if(ds->GetDigitalIn(4) && sparkyauto == true) //DRIVE STRAIGHT, THEN TURN 45 TO GOAL
			{
				sparky.Printlines();
				sparky.Drive(2775);
				sparky.RTurn(30.0);
				sparky.Drive(900);
				sparky.Stop(); // COMPLETE!
				sparky.Dump();
				sparkyauto = false;
			}
			else if(ds->GetDigitalIn(5) && sparkyauto == true) //DRIVE UNDER PYRAMID, TURN 90, THEN INPUT 2 CODE
			{
				sparky.Printlines();
				sparky.Drive(1750);
				sparky.RTurn(50.0); // COMPLETE!
				sparky.Drive(2175);
				sparky.Stop();
				sparky.Dump();
				sparkyauto = false;
			}
			else if(ds->GetDigitalIn(6) && sparkyauto == true) // shoot from left of pyramid
			{
				sparky.Printlines();
				sparky.Drive(460);
				sparky.Stop();
				// Test for gyro angle!
				sparky.ShooterFullPower(true);
				Wait(3.0); // arbitrary value
				sparky.ShooterPiston();
				Wait(2.0); // arbitrary value
				sparky.ShooterPiston();
				Wait(2.0);
				sparky.ShooterPiston();
				sparky.ShooterFullPower(false);
				sparkyauto = false;
			}
			else if(ds->GetDigitalIn(7) && sparkyauto == true) // shooter from right of pyramid
			{
				sparky.Printlines();
				sparky.Drive(460);
				sparky.Stop();
				// Test for gyro angle!
				sparky.ShooterFullPower(true);
				Wait(3.0); // arbitrary value
				sparky.ShooterPiston();
				Wait(2.0); // arbitrary value
				sparky.ShooterPiston();
				Wait(2.0);
				sparky.ShooterPiston();
				sparky.ShooterFullPower(false);
				/*
				sparky.RTurn(90.0);
				sparky.Drive(1073);
				sparky.LTurn(35.0);
				sparky.Drive(1610);
				sparky.Stop();
				sparky.Dump();
				*/
				sparkyauto = false;
			}
			else if(ds->GetDigitalIn(8) && sparkyauto == true)
			{
				sparky.Printlines();
				sparky.ShooterFullPower(true);
				Wait(3.0); // arbitrary value
				sparky.ShooterPiston();
				Wait(2.0); // arbitrary value
				sparky.ShooterPiston();
				Wait(2.0);
				sparky.ShooterPiston();
				sparky.ShooterFullPower(false);
				sparkyauto = false;
			}
			else
			{
				sparky.PrintGyro();
			}
		}
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
		bool pt = true;
		bool bflip = false;
		bool pflip = false;
		//bool climb = false;
		bool shooterfulltoggle = true;
		bool shooterhalftoggle = true;
		bool fullshoot = false;
		bool halfshoot = false;
		while (true)
		{
			sparky.GyroFixAngles();
			sparky.Printlines();
			sparky.InvertMotors(true);
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
				sparky.MiniPogo(pflip);
				if(stick2.GetRawButton(10) && pt)
				{
					pflip = !pflip;
					pt = false;
				}
				else if(!stick2.GetRawButton(10))
				{
					pt = true;
				}
				
			}
			
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
			//sparky.ShooterFullPower(fullshoot);
			//sparky.ShooterSomePower(halfshoot);
			if(stick2.GetRawButton(2) && shooterfulltoggle)
			{
				sparky.ShooterFullPower(!fullshoot); // full speed shooter
				fullshoot = !fullshoot;
				shooterfulltoggle = false;
			}
			else if(!stick2.GetRawButton(2))
			{
				shooterfulltoggle = true;
			}
			if(stick2.GetRawButton(3) && shooterhalftoggle)
			{
				sparky.ShooterSomePower(!halfshoot);
				halfshoot = !halfshoot;
				shooterhalftoggle = false;
			}
			else if(!stick2.GetRawButton(3))
			{
				shooterhalftoggle = true;
			}
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
