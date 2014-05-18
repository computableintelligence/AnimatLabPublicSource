// RbDynamixelUSB.h: interface for the RbDynamixelUSB class.
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

class ROBOTICS_PORT RbDynamixelUSBMotorUpdateData
{
public:
	int m_iID;
	int m_iGoalPos;
	int m_iGoalVelocity;

	RbDynamixelUSBMotorUpdateData()
	{
		m_iID = 0;
		m_iGoalPos = 0;
		m_iGoalVelocity = 0;
	}

	RbDynamixelUSBMotorUpdateData(int iID, int iGoalPos, int iGoalVelocity)
	{
		m_iID = iID;
		m_iGoalPos = iGoalPos;
		m_iGoalVelocity = iGoalVelocity;
	}

};

class ROBOTICS_PORT RbDynamixelUSB : public AnimatSim::Robotics::RobotIOControl
{
protected:
	int m_iPortNumber;
	int m_iBaudRate;

	virtual void ProcessIO();
	virtual void ExitIOThread();

public:
	CStdPtrArray<RbDynamixelUSBMotorUpdateData> m_aryMotorData;

	RbDynamixelUSB();
	virtual ~RbDynamixelUSB();

	virtual void PortNumber(int iPort);
	virtual int PortNumber();

	virtual void BaudRate(int iRate);
	virtual int BaudRate();

	virtual bool SendSynchronousMoveCommand();

#pragma region DataAccesMethods

	virtual float *GetDataPointer(const std::string &strDataType);
	virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
	virtual void QueryProperties(CStdPtrArray<TypeProperty> &aryProperties);

#pragma endregion

	virtual void Initialize();
	virtual void Load(StdUtils::CStdXml &oXml);
};

			}	//DynamixelUSB
		}		//RobotIOControls
	}			// Robotics
}				//RoboticsAnimatSim
