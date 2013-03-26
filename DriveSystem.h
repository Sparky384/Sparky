#ifndef DRIVE_SYSTEM_H
#define DRIVE_SYSTEM_H

#include "WPILib.h"

class DriveSystem
{
private:
	RobotDrive myRobot; // robot drive system
	Joystick stick1, stick2; // only joystick
	Gyro gyro;
	//ADXL345_SPI adxl;
	Encoder enc;
	Encoder enc2;
	DriverStation *ds;
	DriverStationLCD *dsLCD;
	Encoder climbenc;
	DigitalInput ls;
	Servo reverse;
	Victor climbmotor, dumparm, dumpbuck, shooter1, shooter2;
	//Relay *pogoforward; // ***PROTOTYPE IS RELAY WHILE FINAL IS SOLENOIDS***
	//Relay *pogobackward;
	//Relay *pogo2;
	Solenoid pogofwd, pogorev, basehookfwd, basehookrev, minipogofwd, minipogorev, shooterfwd, shooterrev;
	// Wire solenoid breakout to 24 Volts
	

	
public:
	DriveSystem(void);
	bool EncDriveLimit(int length);
	void Drive(int distance);
	void RTurn(double angle);
	void LTurn(double angle);
	void Stop();
	void Reset();
	void GyroReset();
	void EncReset();
	bool InvertMotors(bool tf);
	bool Safety(bool tf);
	void GyroSens();
	void GyroFixAngles();
	int ConvertLS(); // TEMPORARY TESTER FOR PRINTING BOOL VALUES
	void Printlines();
	void SparkTank();
	void SparkFirstArcade();
	void SparkSecondArcade();
	void NoMoving();
	void PrintGyro();
	void ArmOneVal(float value);
	void ArmTwoVal(float value);
	void ServoVal(float value);
	void ArmOneDisable();
	void ArmTwoDisable();
	void DumperForward();
	void DumperBackward();
	void DumperArmForward();
	void DumperArmBackward();
	void DumperArm();
	void NoDumper();
	void ForwardGrappler();
	void BackwardGrappler();
	void NoGrappler();
	void Dump();
	void AutoForward();
	UINT32 LSGet();
	void ClimberEncReset();
	void TenseClimber();
	void BasehookInit();
	void BasehookToggle();
	void PogoToggle();
	void BasehookSwitch(bool toggle);
	void PogoSwitch(bool toggle);
	bool KillClimb();
	void FirstClimb();
	void SecondClimb();
	void climbreset();
	void ShooterFullPower(bool power);
	void ShooterSomePower(bool power);
	void ShooterPiston();
	void MiniPogo(bool toggle);
};

#endif
