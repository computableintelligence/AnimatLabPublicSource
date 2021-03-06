/**
\file	RbHinge.cpp

\brief	Implements the vortex hinge class.
**/

#include "StdAfx.h"
#include "RbMovableItem.h"
#include "RbBody.h"
#include "RbJoint.h"
#include "RbMotorizedJoint.h"
#include "RbRigidBody.h"
#include "RbJoint.h"
#include "RbHingeLimit.h"
#include "RbHinge.h"
#include "RbSimulator.h"

namespace RoboticsAnimatSim
{
	namespace Environment
	{
		namespace Joints
		{


/**
\brief	Default constructor.

\author	dcofer
\date	4/15/2011
**/
RbHinge::RbHinge()
{
	SetThisPointers();

	m_lpUpperLimit = new RbHingeLimit();
	m_lpLowerLimit = new RbHingeLimit();
	m_lpPosFlap = new RbHingeLimit();

	m_lpUpperLimit->LimitPos((float) (0.25*RB_PI), false);
	m_lpLowerLimit->LimitPos((float) (-0.25*RB_PI), false);
	m_lpPosFlap->LimitPos(Hinge::JointPosition(), false);
	m_lpPosFlap->IsShowPosition(true);

	m_lpUpperLimit->Color(1, 0, 0, 1);
	m_lpLowerLimit->Color(1, 1, 1, 1);
	m_lpPosFlap->Color(0, 0, 1, 1);

	m_lpLowerLimit->IsLowerLimit(true);
	m_lpUpperLimit->IsLowerLimit(false);
}

/**
\brief	Destructor.

\author	dcofer
\date	4/15/2011
**/
RbHinge::~RbHinge()
{
	//ConstraintLimits are deleted in the base objects.
	try
	{
	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of RbHinge\r\n", "", -1, false, true);}
}

void RbHinge::EnableLimits(bool bVal)
{
	Hinge::EnableLimits(bVal);

    SetLimitValues();
}

void RbHinge::SetLimitValues()
{
    //if(m_btHinge)
    //{
    //    if(m_bEnableLimits)
    //    {
    //        m_bJointLocked = false;

    //        m_btHinge->setLinearLowerLimit(btVector3(0, 0, 0));
    //        m_btHinge->setLinearUpperLimit(btVector3(0, 0, 0));

    //        //Disable rotation about the axis for the prismatic joint.
		  //  m_btHinge->setAngularLowerLimit(btVector3(m_lpLowerLimit->LimitPos(),0,0));
		  //  m_btHinge->setAngularUpperLimit(btVector3(m_lpUpperLimit->LimitPos(),0,0));

    //        //float fltKp = m_lpUpperLimit->Stiffness();
    //        //float fltKd = m_lpUpperLimit->Damping();
    //        //float fltH = m_lpSim->PhysicsTimeStep()*1000;
    //        
    //        //float fltErp = 0.9; //(fltH*fltKp)/((fltH*fltKp) + fltKd);
    //        //float fltCfm = 0.1; //1/((fltH*fltKp) + fltKd);

    //        //m_btHinge->setParam(BT_CONSTRAINT_STOP_CFM, fltCfm, -1);
    //        //m_btHinge->setParam(BT_CONSTRAINT_STOP_ERP, fltErp, -1);
    //    }
    //    else
    //    {
    //        //To disable limits in bullet we need the lower limit to be bigger than the upper limit
    //        m_bJointLocked = false;

    //        m_btHinge->setLinearLowerLimit(btVector3(0, 0, 0));
    //        m_btHinge->setLinearUpperLimit(btVector3(0, 0, 0));

    //        //Disable rotation about the axis for the prismatic joint.
		  //  m_btHinge->setAngularLowerLimit(btVector3(1,0,0));
		  //  m_btHinge->setAngularUpperLimit(btVector3(-1,0,0));
    //    }
    //}
}

void RbHinge::JointPosition(float fltPos)
{
	m_fltPosition = fltPos;
	if(m_lpPosFlap)
		m_lpPosFlap->LimitPos(fltPos);
}


void RbHinge::CreateJoint()
{
}

#pragma region DataAccesMethods

float *RbHinge::GetDataPointer(const std::string &strDataType)
{
	float *lpData=NULL;
	std::string strType = Std_CheckString(strDataType);

	if(strType == "JOINTROTATION")
		return &m_fltPosition;
	else if(strType == "JOINTPOSITION")
		return &m_fltPosition;
	else if(strType == "JOINTACTUALVELOCITY")
		return &m_fltVelocity;
	else if(strType == "JOINTFORCE")
		return &m_fltForce;
	else if(strType == "JOINTROTATIONDEG")
		return &m_fltRotationDeg;
	else if(strType == "JOINTDESIREDPOSITION")
		return &m_fltReportSetPosition;
	else if(strType == "JOINTDESIREDPOSITIONDEG")
		return &m_fltDesiredPositionDeg;
	else if(strType == "JOINTSETPOSITION")
		return &m_fltReportSetPosition;
	else if(strType == "JOINTDESIREDVELOCITY")
		return &m_fltReportSetVelocity;
	else if(strType == "JOINTSETVELOCITY")
		return &m_fltReportSetVelocity;
	else if(strType == "ENABLE")
		return &m_fltEnabled;
	else if(strType == "CONTACTCOUNT")
		THROW_PARAM_ERROR(Al_Err_lMustBeContactBodyToGetCount, Al_Err_strMustBeContactBodyToGetCount, "JointID", m_strName);
	else
	{
        lpData = RbMotorizedJoint::Physics_GetDataPointer(strType);
		if(lpData) return lpData;

		lpData = Hinge::GetDataPointer(strType);
		if(lpData) return lpData;

		THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "JointID: " + STR(m_strName) + "  DataType: " + strDataType);
	}

	return lpData;
}

bool RbHinge::SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError)
{
	//if(RbJoint::Physics_SetData(strDataType, strValue))
	//	return true;

	if(Hinge::SetData(strDataType, strValue, false))
		return true;

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Data Type", strDataType);

	return false;
}

void RbHinge::QueryProperties(CStdPtrArray<TypeProperty> &aryProperties)
{
	//RbJoint::Physics_QueryProperties(aryProperties);
	Hinge::QueryProperties(aryProperties);

	aryProperties.Add(new TypeProperty("JointRotation", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointPosition", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointActualVelocity", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointForce", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointRotationDeg", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointDesiredPosition", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointDesiredPositionDeg", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointSetPosition", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointDesiredVelocity", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("JointSetVelocity", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("Enable", AnimatPropertyType::Boolean, AnimatPropertyDirection::Get));
}

#pragma endregion

void RbHinge::StepSimulation()
{
	UpdateData();
	SetVelocityToDesired();
}

void RbHinge::Physics_EnableLock(bool bOn, float fltPosition, float fltMaxLockForce)
{
	if(bOn)
        m_bJointLocked = true;
	else if (m_bMotorOn)
		Physics_EnableMotor(true, 0, fltMaxLockForce, false);
	else
        SetLimitValues();
}

void RbHinge::Physics_EnableMotor(bool bOn, float fltDesiredVelocity, float fltMaxForce, bool bForceWakeup)
{
	if(bOn)
    {
        if(!m_bMotorOn || bForceWakeup || m_bJointLocked)
        {
        }

		//I had to move these statements out of the if above. I kept running into one instance after another where I ran inot a problem if I did not do this every single time.
		// It is really annoying and inefficient, but I cannot find another way to reiably guarantee that the motor will behave coorectly under all conditions without
		// doing this every single time I set the motor velocity.
        SetLimitValues();
        m_lpThisJoint->WakeDynamics();
    }
	else
    {
        TurnMotorOff();

        if(m_bMotorOn || bForceWakeup || m_bJointLocked)
        {
            m_iAssistCountdown = 3;
            ClearAssistForces();
            m_lpThisJoint->WakeDynamics();
            SetLimitValues();
        }
    }

	m_bMotorOn = bOn;
}

void RbHinge::Physics_MaxForce(float fltVal)
{
    //if(m_btJoint && m_btHinge)
    //    m_btHinge->getRotationalLimitMotor(0)->m_maxMotorForce = fltVal;
}

void RbHinge::TurnMotorOff()
{
    //if(m_btHinge)
    //{
    //    if(m_lpFriction && m_lpFriction->Enabled())
    //    {
    //        //0.032 is a coefficient that produces friction behavior in bullet using the same coefficient values
    //        //that were specified in vortex engine. This way I get similar behavior between the two.
    //        float	maxMotorImpulse = m_lpFriction->Coefficient()*0.032f*(m_lpThisAB->GetSimulator()->InverseMassUnits() * m_lpThisAB->GetSimulator()->InverseDistanceUnits());  
		  //  m_btHinge->getRotationalLimitMotor(0)->m_enableMotor = true;
		  //  m_btHinge->getRotationalLimitMotor(0)->m_targetVelocity = 0;
		  //  m_btHinge->getRotationalLimitMotor(0)->m_maxMotorForce = maxMotorImpulse;
    //    }
    //    else
		  //  m_btHinge->getRotationalLimitMotor(0)->m_enableMotor = false;
    //}
}

void RbHinge::SetConstraintFriction()
{
    if(!m_bJointLocked && !m_bMotorOn && m_bEnabled)
        TurnMotorOff();
}

void RbHinge::ResetSimulation()
{
	Hinge::ResetSimulation();

    //m_btHinge->getRotationalLimitMotor(0)->m_currentPosition = 0;
    //m_btHinge->getRotationalLimitMotor(0)->m_accumulatedImpulse = 0;
}

		}		//Joints
	}			// Environment
}				//RoboticsAnimatSim
