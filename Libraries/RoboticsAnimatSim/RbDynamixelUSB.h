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

class ROBOTICS_PORT RbDynamixelUSB : public AnimatSim::Robotics::RobotIOControl
{
protected:
	int m_iPortNumber;
	int m_iBaudRate;

	virtual void ProcessIO();
	virtual bool OpenIO();
	virtual void CloseIO();

public:
	CStdPtrArray<RbDynamixelMotorUpdateData> m_aryMotorData;

	RbDynamixelUSB();
	virtual ~RbDynamixelUSB();

	virtual void PortNumber(int iPort);
	virtual int PortNumber();

	virtual void BaudRate(int iRate);
	virtual int BaudRate();

	virtual bool SendSynchronousMoveCommand();
			
	virtual void SetRegister(unsigned char iServo, unsigned char reg, unsigned char length, unsigned int value);
	virtual int GetRegister(unsigned char iServo, unsigned char reg, unsigned char length);

	virtual std::string GetErrorCode();
	virtual std::string GetCommStatus(int CommStatus);

#pragma region DataAccesMethods

	virtual float *GetDataPointer(const std::string &strDataType);
	virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
	virtual void QueryProperties(CStdPtrArray<TypeProperty> &aryProperties);

#pragma endregion

	virtual void Load(StdUtils::CStdXml &oXml);
};

			}	//DynamixelUSB
		}		//RobotIOControls
	}			// Robotics
}				//RoboticsAnimatSim

