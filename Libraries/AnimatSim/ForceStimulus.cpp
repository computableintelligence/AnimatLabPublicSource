// ForceStimulus.cpp: implementation of the ForceStimulus class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "IMovableItemCallback.h"
#include "ISimGUICallback.h"
#include "AnimatBase.h"

#include <sys/types.h>
#include <sys/stat.h>
#include "Gain.h"
#include "Node.h"
#include "IPhysicsMovableItem.h"
#include "IPhysicsBody.h"
#include "BoundingBox.h"
#include "MovableItem.h"
#include "BodyPart.h"
#include "Joint.h"
#include "MotorizedJoint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Structure.h"
#include "NeuralModule.h"
#include "Adapter.h"
#include "NervousSystem.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimulus.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Light.h"
#include "LightManager.h"
#include "Simulator.h"

#include "ForceStimulus.h"

namespace AnimatSim
{
	namespace ExternalStimuli
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ForceStimulus::ForceStimulus()
{
	m_lpStructure = NULL;
	m_lpBody = NULL;

	m_lpForceXEval = NULL;
	m_lpForceYEval = NULL;
	m_lpForceZEval = NULL;

	m_fltForceX = 0;
	m_fltForceY = 0;
	m_fltForceZ = 0;

	m_fltForceReportX = 0;
	m_fltForceReportY = 0;
	m_fltForceReportZ = 0;

	m_lpTorqueXEval = NULL;
	m_lpTorqueYEval = NULL;
	m_lpTorqueZEval = NULL;

	m_fltTorqueX = 0;
	m_fltTorqueY = 0;
	m_fltTorqueZ = 0;

	m_fltTorqueReportX = 0;
	m_fltTorqueReportY = 0;
	m_fltTorqueReportZ = 0;
}

ForceStimulus::~ForceStimulus()
{

try
{
	m_lpStructure = NULL;
	m_lpBody = NULL;

	if(m_lpForceXEval) delete m_lpForceXEval;
	if(m_lpForceYEval) delete m_lpForceYEval;
	if(m_lpForceZEval) delete m_lpForceZEval;

	if(m_lpTorqueXEval) delete m_lpTorqueXEval;
	if(m_lpTorqueYEval) delete m_lpTorqueYEval;
	if(m_lpTorqueZEval) delete m_lpTorqueZEval;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of ForceStimulus\r\n", "", -1, false, true);}
}

CStdPostFixEval *ForceStimulus::SetupEquation(std::string strEquation)
{
	CStdPostFixEval *lpEquation = NULL;

	if(!Std_IsBlank(strEquation))
	{
		lpEquation = new CStdPostFixEval;
		lpEquation->AddVariable("t");
		lpEquation->Equation(strEquation);
	}

	return lpEquation;
}


void ForceStimulus::RelativePositionX(float fltVal)
{
	Simulator *m_lpSim = GetSimulator();
	m_oRelativePosition.x = fltVal * m_lpSim->InverseDistanceUnits();
}

void ForceStimulus::RelativePositionY(float fltVal)
{
	Simulator *m_lpSim = GetSimulator();
	m_oRelativePosition.y = fltVal * m_lpSim->InverseDistanceUnits();
}

void ForceStimulus::RelativePositionZ(float fltVal)
{
	Simulator *m_lpSim = GetSimulator();
	m_oRelativePosition.z = fltVal * m_lpSim->InverseDistanceUnits();
}

void ForceStimulus::ForceXEquation(std::string strVal)
{
	if(m_lpForceXEval) 
	{delete m_lpForceXEval; m_lpForceXEval = NULL;}

	m_strForceXEquation = strVal;
	m_lpForceXEval = SetupEquation(strVal);
}

void ForceStimulus::ForceYEquation(std::string strVal)
{
	if(m_lpForceYEval) 
	{delete m_lpForceYEval; m_lpForceYEval = NULL;}

	m_strForceYEquation = strVal;
	m_lpForceYEval = SetupEquation(strVal);
}

void ForceStimulus::ForceZEquation(std::string strVal)
{
	if(m_lpForceZEval) 
	{delete m_lpForceZEval; m_lpForceZEval = NULL;}

	m_strForceZEquation = strVal;
	m_lpForceZEval = SetupEquation(strVal);
}

void ForceStimulus::TorqueXEquation(std::string strVal)
{
	if(m_lpTorqueXEval) 
	{delete m_lpTorqueXEval; m_lpTorqueXEval = NULL;}

	m_strTorqueXEquation = strVal;
	m_lpTorqueXEval = SetupEquation(strVal);
}

void ForceStimulus::TorqueYEquation(std::string strVal)
{
	if(m_lpTorqueYEval) 
	{delete m_lpTorqueYEval; m_lpTorqueYEval = NULL;}

	m_strTorqueYEquation = strVal;
	m_lpTorqueYEval = SetupEquation(strVal);
}

void ForceStimulus::TorqueZEquation(std::string strVal)
{
	if(m_lpTorqueZEval) 
	{delete m_lpTorqueZEval; m_lpTorqueZEval = NULL;}

	m_strTorqueZEquation = strVal;
	m_lpTorqueZEval = SetupEquation(strVal);
}

void ForceStimulus::Initialize()
{
	ExternalStimulus::Initialize();

	//Lets try and get the node we will dealing with.
	m_lpStructure = m_lpSim->FindStructureFromAll(m_strStructureID);
	m_lpBody = m_lpStructure->FindRigidBody(m_strBodyID);
}

void ForceStimulus::ResetSimulation()
{
	ExternalStimulus::ResetSimulation();

	m_fltForceX = m_fltForceY = m_fltForceZ = 0;
	m_fltForceReportX = m_fltForceReportY = m_fltForceReportZ = 0;
	m_fltTorqueX = m_fltTorqueY = m_fltTorqueZ = 0;
	m_fltTorqueReportX = m_fltTorqueReportY = m_fltTorqueReportZ = 0;
}

void ForceStimulus::StepSimulation()
{
	try
	{
		//IMPORTANT! This stimulus applies a force to the physics engine, so it should ONLY be called once for every time the physcis
		//engine steps. If you do not do this then the you will accumulate forces being applied during the neural steps, and the total
		//force you apply will be greater than what it should be. To get around this we will only call the code in step simulation if
		//the physics step count is equal to the step interval.
		if(m_lpSim->PhysicsStepCount() == m_lpSim->PhysicsStepInterval())
		{

			//Why do we multiply by the mass units here? The reason is that we have to try and keep the 
			//length and mass values in a range around 1 for the simulator to be able to function appropriately.
			//So say we are uing grams and centimeters. This means that if we have a 1cm^3 box that weights 1 gram
			//it will come in with a density of 1 g.cm^3 and we will set its density to 1. But the simulator treats this
			//as 1 Kg and not 1g. So forces/torques and so on are scaled incorrectly. We must scale the force to be applied
			//so it is acting against kilograms instead of grams. So a 1N force would be 1000N to produce the same effect.
			if(m_lpForceXEval || m_lpForceYEval || m_lpForceZEval)
			{
				m_fltForceX = m_fltForceY = m_fltForceZ = 0;
				m_fltForceReportX = m_fltForceReportY = m_fltForceReportZ = 0;

				if(m_lpForceXEval)
				{
					m_lpForceXEval->SetVariable("t", m_lpSim->Time());
					m_fltForceReportX = m_lpForceXEval->Solve();
					m_fltForceX = m_fltForceReportX * m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits();
				}

				if(m_lpForceYEval)
				{
					m_lpForceYEval->SetVariable("t", m_lpSim->Time());
					m_fltForceReportY = m_lpForceYEval->Solve();
					m_fltForceY = m_fltForceReportY * m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits();
				}

				if(m_lpForceZEval)
				{
					m_lpForceZEval->SetVariable("t", m_lpSim->Time());
					m_fltForceReportZ = m_lpForceZEval->Solve();
					m_fltForceZ = m_fltForceReportZ * m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits();
				}

				if(m_lpBody && (m_fltForceX || m_fltForceY || m_fltForceZ))
					m_lpBody->AddForceAtLocalPos(m_oRelativePosition.x, m_oRelativePosition.y, m_oRelativePosition.z,
                                       m_fltForceX, m_fltForceY, m_fltForceZ, false);
			}

			if(m_lpTorqueXEval || m_lpTorqueYEval || m_lpTorqueZEval)
			{
				m_fltTorqueX = m_fltTorqueY = m_fltTorqueZ = 0;
				m_fltTorqueReportX = m_fltTorqueReportY = m_fltTorqueReportZ = 0;

				if(m_lpTorqueXEval)
				{
					m_lpTorqueXEval->SetVariable("t", m_lpSim->Time());
					m_fltTorqueReportX = m_lpTorqueXEval->Solve();
					m_fltTorqueX = m_fltTorqueReportX * m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits() * m_lpSim->InverseDistanceUnits();
				}

				if(m_lpTorqueYEval)
				{
					m_lpTorqueYEval->SetVariable("t", m_lpSim->Time());
					m_fltTorqueReportY = m_lpTorqueYEval->Solve();
					m_fltTorqueY = m_fltTorqueReportY * m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits() * m_lpSim->InverseDistanceUnits();
				}

				if(m_lpTorqueZEval)
				{
					m_lpTorqueZEval->SetVariable("t", m_lpSim->Time());
					m_fltTorqueReportZ = m_lpTorqueZEval->Solve();
					m_fltTorqueZ = m_fltTorqueReportZ * m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits() * m_lpSim->InverseDistanceUnits();
				}

				if(m_lpBody && (m_fltTorqueX || m_fltTorqueY || m_fltTorqueZ))
					m_lpBody->AddTorque(m_fltTorqueX, m_fltTorqueY, m_fltTorqueZ, false);
			}
		}
	}
	catch(...)
	{
		LOG_ERROR("Error Occurred while setting Joint Velocity");
	}
}

void ForceStimulus::Deactivate()
{
	AnimatSim::ExternalStimuli::ExternalStimulus::Deactivate();
	if(m_lpBody)
		m_lpBody->WakeDynamics();
}

float *ForceStimulus::GetDataPointer(const std::string &strDataType)
{
	float *lpData=NULL;
	std::string strType = Std_CheckString(strDataType);

	if(strType == "FORCEX")
		lpData = &m_fltForceReportX;
	else if(strType == "FORCEY")
		lpData = &m_fltForceReportY;
	else if(strType == "FORCEZ")
		lpData = &m_fltForceReportZ;
	else if(strType == "TORQUEX")
		lpData = &m_fltTorqueReportX;
	else if(strType == "TORQUEY")
		lpData = &m_fltTorqueReportY;
	else if(strType == "TORQUEZ")
		lpData = &m_fltTorqueReportZ;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "StimulusName: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
} 

bool ForceStimulus::SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError)
{
	std::string strType = Std_CheckString(strDataType);

	if(ExternalStimulus::SetData(strDataType, strValue, false))
		return true;

	if(strType == "POSITIONX")
	{
		RelativePositionX(atof(strValue.c_str()));
		return true;
	}

	if(strType == "POSITIONY")
	{
		RelativePositionY(atof(strValue.c_str()));
		return true;
	}

	if(strType == "POSITIONZ")
	{
		RelativePositionZ(atof(strValue.c_str()));
		return true;
	}

	if(strType == "FORCEX")
	{
		ForceXEquation(strValue);
		return true;
	}

	if(strType == "FORCEY")
	{
		ForceYEquation(strValue);
		return true;
	}

	if(strType == "FORCEZ")
	{
		ForceZEquation(strValue);
		return true;
	}

	if(strType == "TORQUEX")
	{
		TorqueXEquation(strValue);
		return true;
	}

	if(strType == "TORQUEY")
	{
		TorqueYEquation(strValue);
		return true;
	}

	if(strType == "TORQUEZ")
	{
		TorqueZEquation(strValue);
		return true;
	}

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Data Type", strDataType);

	return false;
}

void ForceStimulus::QueryProperties(CStdArray<std::string> &aryNames, CStdArray<std::string> &aryTypes)
{
	ExternalStimulus::QueryProperties(aryNames, aryTypes);

	aryNames.Add("PositionX");
	aryTypes.Add("Float");

	aryNames.Add("PositionY");
	aryTypes.Add("Float");

	aryNames.Add("PositionZ");
	aryTypes.Add("Float");

	aryNames.Add("ForceX");
	aryTypes.Add("Float");

	aryNames.Add("ForceY");
	aryTypes.Add("Float");

	aryNames.Add("ForceZ");
	aryTypes.Add("Float");

	aryNames.Add("TorqueX");
	aryTypes.Add("Float");

	aryNames.Add("TorqueY");
	aryTypes.Add("Float");

	aryNames.Add("TorqueZ");
	aryTypes.Add("Float");
}

void ForceStimulus::Load(CStdXml &oXml)
{
	ActivatedItem::Load(oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strStructureID = oXml.GetChildString("StructureID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strBodyID = oXml.GetChildString("BodyID");
	if(Std_IsBlank(m_strBodyID)) 
		THROW_ERROR(Al_Err_lBodyIDBlank, Al_Err_strBodyIDBlank);

	ForceXEquation(oXml.GetChildString("ForceX", ""));
	ForceYEquation(oXml.GetChildString("ForceY", ""));
	ForceZEquation(oXml.GetChildString("ForceZ", ""));

	TorqueXEquation(oXml.GetChildString("TorqueX", ""));
	TorqueYEquation(oXml.GetChildString("TorqueY", ""));
	TorqueZEquation(oXml.GetChildString("TorqueZ", ""));

	CStdFPoint oPoint;
	Std_LoadPoint(oXml, "RelativePosition", m_oRelativePosition);

	//We need to scale the distance values to be appropriate. They 
	//will be saved as centimeters or some such in the config file, 
	//but we need them to be in "unit" values.
	m_oRelativePosition *= m_lpSim->InverseDistanceUnits();

	oXml.OutOfElem(); //OutOf Simulus Element
}

	}			//ExternalStimuli
}				//AnimatSim




