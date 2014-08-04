// RbFirmataDynamixelServo.h: interface for the RbFirmataDynamixelServo class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "RbDynamixelServo.h"

namespace RoboticsAnimatSim
{
	namespace Robotics
	{
		namespace RobotIOControls
		{
			namespace Firmata
			{

class ROBOTICS_PORT RbFirmataDynamixelServo : public RbDynamixelServo, public RbFirmataPart
{
protected:
	virtual void WriteGoalPosition(int iServoID, int iPos) {};
	virtual int ReadGoalPosition(int iServoID)  {return m_iLastGoalPos;};
	virtual int ReadPresentPosition(int iServoID)  {return m_iPresentPos;};

	virtual void WriteMovingSpeed(int iServoID, int iVelocity) {};
	virtual int ReadMovingSpeed(int iServoID)  {return m_iLastGoalVelocity;};
	virtual int ReadPresentSpeed(int iServoID)  {return m_iPresentVelocity;};

	virtual int ReadPresentLoad(int iServoID)  {return m_iLoad;};
	virtual int ReadPresentVoltage(int iServoID)  {return m_iVoltage;};
	virtual int ReadPresentTemperature(int iServoID)  {return m_iTemperature;};
	virtual int ReadIsMoving(int iServoID)  {return 1;};
	virtual int ReadLED(int iServoID)  {return 1;};
	virtual int ReadAlarmShutdown(int iServoID)  {return 1;};
	virtual int ReadModelNumber(int iServoID)   {return 1;};
	virtual int ReadID(int iServoID)  {return 1;};
	virtual int ReadFirmwareVersion(int iServoID)  {return 1;};

	virtual void WriteReturnDelayTime(int iServoID, int iVal) {};
	virtual int ReadReturnDelayTime(int iServoID)  {return 1;};

	virtual void WriteCCWAngleLimit(int iServoID, int iVal) {};
	virtual int ReadCCWAngleLimit(int iServoID)  {return 1;};

	virtual void WriteCWAngleLimit(int iServoID, int iVal) {};
	virtual int ReadCWAngleLimit(int iServoID)  {return 1;};

	virtual void WriteTorqueLimit(int iServoID, int iVal) {};
	virtual int ReadTorqueLimit(int iServoID)  {return 1;};

	boost::signals2::connection m_EDynamixelReceived;
	boost::signals2::connection m_EDynamixelTransmitError;

	//virtual void DynamixelRecieved(const int &iServoID);
	virtual void DynamixelTransmitError(const int &iCmd, const int &iServoID); 

	virtual void AddMotorUpdate(int iPos, int iSpeed);
	virtual void UpdateMotorData();
	virtual void UpdateAllMotorData();
	virtual void UpdateKeyMotorData();

public:
	RbFirmataDynamixelServo();
	virtual ~RbFirmataDynamixelServo();
	
	virtual bool IsMotorControl() {return true;};

	virtual void IOComponentID(int iID);

	virtual float QuantizeServoPosition(float fltPos);
	virtual float QuantizeServoVelocity(float fltVel);

	virtual void InitMotorData();

#pragma region DataAccesMethods

	virtual float *GetDataPointer(const std::string &strDataType);
	virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
	virtual void QueryProperties(CStdPtrArray<TypeProperty> &aryProperties);

#pragma endregion

    virtual void StepSimulation();
	virtual void ResetSimulation();
	virtual void Load(StdUtils::CStdXml &oXml);
	virtual void MicroSleep(unsigned int iTime);    
	virtual Simulator *GetSimulator();
	virtual void StepIO(int iPartIdx);
	virtual void ShutdownIO();

};

			}	//Firmata
		}		//RobotIOControls
	}			// Robotics
}				//RoboticsAnimatSim

