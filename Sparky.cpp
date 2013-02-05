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
	ADXL345_I2C adxl;
	Gyro gyro;
	Relay *blinkylight;
	DigitalInput trigger;
	Encoder enc;
	DriverStation *ds;
	DriverStationLCD *dsLCD;
	//Rookie code
	//RobotDrive mypurpledog;
	//Joystick high1;
	
	
	
public:
	RobotDemo(void):
		myRobot(2, 3),	// these must be initialized in the same order
		stick1(1),		// as they are declared above.
		stick2(2),
		adxl(1, ADXL345_I2C::kRange_2G),
		gyro(2),
		trigger(11),
		enc(3, 4),
		ds(DriverStation::GetInstance()),
		dsLCD(DriverStationLCD::GetInstance())
	{
		myRobot.SetExpiration(0.1);
		blinkylight = new Relay(1);
		enc.Reset();
		enc.Start();
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		enc.Reset();
		gyro.SetSensitivity(0.007);
		while(IsAutonomous() && IsEnabled()) // this is a change
		{
			myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);
			myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
			gyro.Reset();
			if(ds->GetDigitalIn(1))
			{
				dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
				dsLCD->UpdateLCD();
				//Wait(0.05);
				myRobot.Drive(0.5, 0.0);
				Wait(2.0);
				myRobot.Drive(0.0, 0.0);
				Wait(1.0);
				do
				{
					dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
					dsLCD->UpdateLCD();
					myRobot.Drive(0.5, 1.0);
					Wait(0.05);
				}
				while(gyro.GetAngle() <= 90.0 && gyro.GetAngle() >= -90.0);
				myRobot.Drive(0.0, 0.0);
				blinkylight->Set(Relay::kForward);
				dsLCD->UpdateLCD();
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(2))
			{
				blinkylight->Set(Relay::kForward);
				myRobot.Drive(1.0, 0.0);
				Wait(0.9);
				myRobot.Drive(0.0, 0.0);
				Wait(15.0);
			}
			else if(ds->GetDigitalIn(3))
			{
				dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
				dsLCD->UpdateLCD();
				do
				{
					dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
					dsLCD->UpdateLCD();
					myRobot.Drive(0.5, 1.0);
					Wait(0.05);
				}
				while(gyro.GetAngle() <= 45.0 && gyro.GetAngle() >= -45.0);
				myRobot.Drive(0.5, 0.0);
				Wait(2.0);
				myRobot.Drive(0.0, 0.0);
				Wait(15.0);
			}
			else
			{
				myRobot.Drive(0.0, 0.0);
				dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro Value: %f", gyro.GetAngle());
				dsLCD->UpdateLCD();
			}
			myRobot.Drive(0.0, 0.0);
		}
		myRobot.Drive(0.0, 0.0);
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		gyro.Reset();
		gyro.SetSensitivity(0.0062);
		myRobot.SetSafetyEnabled(true);
		bool motorinv = false;
		
		/*
		DigitalModule *dm = DigitalModule::GetInstance(2);
		I2C *isc = dm->GetI2C(0x3A);
		isc->Write(0x2D, 0x08);
		*/
		/*xAngle = Math.toDegrees(getAxesMeasurements().XAxis);
		xAngleFiltered = Constants.hfc * xAngleFiltered + (1 - Constants.hfc) * xAngle;
		gyroError = xAngleFiltered - supaGyro.getAngle();
		Get the actual Angle of the bot
		angle = Constants.lfc * ((angle + (supaGyro.getAngle() + gyroError)) / 2) + (1 - Constants.lfc) * xAngle;*/
		while (true)
		{
			/*
			ADXL345_I2C::AllAxes ax = adxl.GetAccelerations();
			//printf("X: %f, Y: %f, Z: %f", ax.XAxis, ax.YAxis, ax.ZAxis);
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "X: %f", ax.XAxis);
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Y: %f", ax.YAxis);
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Z: %f", ax.ZAxis);
			*/
			float angle = gyro.GetAngle();
			if(gyro.GetAngle() > 360)
			{
				angle=-360.0;
			}
			else if(gyro.GetAngle() < -360)
			{
				angle=+360.0;
			}
			
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "X: %f",
					adxl.GetAcceleration(ADXL345_I2C::kAxis_X));
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Y: %f",
					adxl.GetAcceleration(ADXL345_I2C::kAxis_Y));
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line3, "Z: %f",
					adxl.GetAcceleration(ADXL345_I2C::kAxis_Z));
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Gyro: %f", angle);
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Encoder: %d", enc.Get());
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
			}
			
			if(stick1.GetRawButton(7))
			{
				gyro.Reset();
			}
			else
			{
				
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
			Wait(0.005);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

