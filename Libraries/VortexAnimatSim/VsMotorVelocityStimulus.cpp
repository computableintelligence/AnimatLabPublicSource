/**
\file	VsMotorVelocityStimulus.cpp

\brief	Implements the vs motor velocity stimulus class.
**/

#include "StdAfx.h"

#include "VsJoint.h"
#include "VsMotorizedJoint.h"
#include "VsRigidBody.h"
#include "VsSimulator.h"

#include "VsMotorVelocityStimulus.h"

namespace VortexAnimatSim
{
	namespace ExternalStimuli
	{
/**
\brief	Default constructor.

\author	dcofer
\date	4/3/2011
**/
VsMotorVelocityStimulus::VsMotorVelocityStimulus()
{
	m_lpJoint = NULL;
	m_lpEval = NULL;
	m_fltVelocity = 0;
	m_fltVelocityReport = 0;
	m_bDisableMotorWhenDone = false;
	m_lpPosition = NULL;
	m_lpVelocity = NULL;
}

/**
\brief	Destructor.

\author	dcofer
\date	4/3/2011
**/
VsMotorVelocityStimulus::~VsMotorVelocityStimulus()
{

try
{
	m_lpJoint = NULL;
	if(m_lpEval) delete m_lpEval;
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of VsMotorVelocityStimulus\r\n", "", -1, false, true);}
}

/**
\brief	Sets the velocity equation used to control the motor speed over time.

\author	dcofer
\date	4/3/2011

\param	strVal	The post-fix velocity equation string. 
**/
void VsMotorVelocityStimulus::VelocityEquation(string strVal)
{
	//Initialize the postfix evaluator.
	if(m_lpEval) 
	{delete m_lpEval; m_lpEval = NULL;}

	m_strVelocityEquation = strVal;
	m_lpEval = new CStdPostFixEval;

	m_lpEval->AddVariable("t");
	m_lpEval->AddVariable("p");
	m_lpEval->AddVariable("v");
	m_lpEval->Equation(m_strVelocityEquation);
}

void VsMotorVelocityStimulus::ResetSimulation()
{
	ExternalStimulus::ResetSimulation();

	m_fltVelocity = 0;
	m_fltVelocityReport = 0;
}

void VsMotorVelocityStimulus::Initialize()
{
	ExternalStimulus::Initialize();

	//Lets try and get the joint we will be injecting.
	m_lpJoint = dynamic_cast<MotorizedJoint *>(m_lpSim->FindJoint(m_strStructureID, m_strJointID));
	if(!m_lpJoint)
		THROW_PARAM_ERROR(Al_Err_lJointNotMotorized, Al_Err_strJointNotMotorized, "ID", m_strJointID);

	m_lpPosition = m_lpJoint->GetDataPointer("JOINTPOSITION");
	m_lpVelocity = m_lpJoint->GetDataPointer("JOINTACTUALVELOCITY");
}

void VsMotorVelocityStimulus::Activate()
{
	ExternalStimulus::Activate();

	if(m_bEnabled)
	{
		m_lpJoint->EnableMotor(true);
		m_lpJoint->DesiredVelocity(0);
	}
}


void VsMotorVelocityStimulus::StepSimulation()
{
	//float fltVel=0;

	try
	{
		if(m_bEnabled)
		{
			//IMPORTANT! This stimulus applies a stimulus to the physics engine, so it should ONLY be called once for every time the physcis
			//engine steps. If you do not do this then the you will accumulate forces being applied during the neural steps, and the total
			//for you apply will be greater than what it should be. To get around this we will only call the code in step simulation if
			//the physics step count is equal to the step interval.
			if(m_lpSim->PhysicsStepCount() == m_lpSim->PhysicsStepInterval())
			{
				m_lpEval->SetVariable("t", (m_lpSim->Time()-m_fltStartTime) );
				
				if(m_lpPosition)
					m_lpEval->SetVariable("p",  *m_lpPosition);
				
				if(m_lpVelocity)
					m_lpEval->SetVariable("v", *m_lpVelocity);

				m_fltVelocityReport = m_fltVelocity = m_lpEval->Solve();
				//fltVel = -sin(6.28*(m_lpSim->Time()-m_fltStartTime));

				if(!m_lpJoint->UsesRadians())
					m_fltVelocity *= m_lpSim->InverseDistanceUnits();

				m_lpJoint->DesiredVelocity(m_fltVelocity);
			}
		}
	}
	catch(...)
	{
		LOG_ERROR("Error Occurred while setting Joint Velocity");
	}
}


void VsMotorVelocityStimulus::Deactivate()
{
	ExternalStimulus::Deactivate();

	if(m_bEnabled)
	{
		m_lpJoint->DesiredVelocity(0);
		if(m_bDisableMotorWhenDone)
			m_lpJoint->EnableMotor(false);
	}
}

float *VsMotorVelocityStimulus::GetDataPointer(const string &strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	if(strType == "VELOCITY")
		lpData = &m_fltVelocityReport;
	else
		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "StimulusName: " + STR(m_strName) + "  DataType: " + strDataType);

	return lpData;
} 

bool VsMotorVelocityStimulus::SetData(const string &strDataType, const string &strValue, bool bThrowError)
{
	string strType = Std_CheckString(strDataType);

	if(ExternalStimulus::SetData(strDataType, strValue, false))
		return true;

	if(strType == "VELOCITY" || strType == "EQUATION")
	{
		VelocityEquation(strValue);
		return true;
	}

	if(strType == "DISABLEWHENDONE")
	{
		DisableMotorWhenDone(Std_ToBool(strValue));
		return true;
	}

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Data Type", strDataType);

	return false;
}

void VsMotorVelocityStimulus::QueryProperties(CStdArray<string> &aryNames, CStdArray<string> &aryTypes)
{
	ExternalStimulus::QueryProperties(aryNames, aryTypes);

	aryNames.Add("Velocity");
	aryTypes.Add("Float");

	aryNames.Add("Equation");
	aryTypes.Add("String");

	aryNames.Add("DisableMotorWhenDone");
	aryTypes.Add("Boolean");
}

void VsMotorVelocityStimulus::Load(CStdXml &oXml)
{
	ActivatedItem::Load(oXml);

	oXml.IntoElem();  //Into Simulus Element

	m_strStructureID = oXml.GetChildString("StructureID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	m_strJointID = oXml.GetChildString("JointID");
	if(Std_IsBlank(m_strStructureID)) 
		THROW_ERROR(Al_Err_lIDBlank, Al_Err_strIDBlank);

	VelocityEquation(oXml.GetChildString("Velocity"));
	DisableMotorWhenDone(oXml.GetChildBool("DisableMotorWhenDone", m_bDisableMotorWhenDone));

	oXml.OutOfElem(); //OutOf Simulus Element
}

	}			//ExternalStimuli
}				//VortexAnimatSim




