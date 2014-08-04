// RbDynamixelUSBPrismatic.h: interface for the RbDynamixelUSBPrismatic class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

namespace RoboticsAnimatSim
{
	namespace Robotics
	{
		namespace RobotIOControls
		{
			namespace DynamixelUSB
			{

class ROBOTICS_PORT RbDynamixelUSBPrismatic : public AnimatSim::Robotics::RobotPartInterface, public RbDynamixelUSBServo
{
protected:
    Prismatic *m_lpPrismatic;

	virtual void AddMotorUpdate(int iPos, int iSpeed) {};

public:
	RbDynamixelUSBPrismatic();
	virtual ~RbDynamixelUSBPrismatic();

	virtual bool IsMotorControl() {return true;};

    virtual void StepSimulation();
	virtual void MicroSleep(unsigned int iTime);
	virtual Simulator *GetSimulator();
};

			}	//DynamixelUSB
		}		//RobotIOControls
	}			// Robotics
}				//RoboticsAnimatSim

