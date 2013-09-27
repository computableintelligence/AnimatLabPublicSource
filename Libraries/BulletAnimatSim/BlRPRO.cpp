/**
\file	BlRPRO.cpp

\brief	Implements the vortex ball socket class.
**/

#include "StdAfx.h"
#include "BlJoint.h"
#include "BlMotorizedJoint.h"
#include "BlRigidBody.h"
#include "BlRPRO.h"
#include "BlSimulator.h"

namespace BulletAnimatSim
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
BlRPRO::BlRPRO()
{
	SetThisPointers();
	//m_btSocket = NULL;
}

/**
\brief	Destructor.

\author	dcofer
\date	4/15/2011
**/
BlRPRO::~BlRPRO()
{

	try
	{
		DeleteGraphics();
		DeletePhysics();
	}
	catch(...)
	{Std_TraceMsg(0, "Caught Error in desctructor of BlRPRO\r\n", "", -1, false, true);}
}

void BlRPRO::SetupPhysics()
{
	if(m_btJoint)
		DeletePhysics();

	if(!m_lpParent)
		THROW_ERROR(Al_Err_lParentNotDefined, Al_Err_strParentNotDefined);

	if(!m_lpChild)
		THROW_ERROR(Al_Err_lChildNotDefined, Al_Err_strChildNotDefined);

	BlRigidBody *lpVsParent = dynamic_cast<BlRigidBody *>(m_lpParent);
	if(!lpVsParent)
		THROW_ERROR(Bl_Err_lUnableToConvertToBlRigidBody, Bl_Err_strUnableToConvertToBlRigidBody);

	BlRigidBody *lpVsChild = dynamic_cast<BlRigidBody *>(m_lpChild);
	if(!lpVsChild)
		THROW_ERROR(Bl_Err_lUnableToConvertToBlRigidBody, Bl_Err_strUnableToConvertToBlRigidBody);

    //Get the matrices for the joint relative to the child and parent.
    osg::Matrix osgJointRelParent = m_osgMT->getMatrix();
    CStdFPoint vPos = m_lpThisMI->Position();
    CStdFPoint vRot = m_lpThisMI->Rotation();
    osg::Matrix osgJointRelChild = SetupMatrix(vPos, vRot);
    
    btTransform tmJointRelParent = osgbCollision::asBtTransform(osgJointRelParent);
    btTransform tmJointRelChild = osgbCollision::asBtTransform(osgJointRelChild);

    //All limits are turned off by default.
	m_btSocket = new btGeneric6DofConstraint(*lpVsParent->Part(), *lpVsChild->Part(), tmJointRelParent, tmJointRelChild, true); 
    m_btSocket->setLinearLowerLimit(btVector3(0, 0, 0));
    m_btSocket->setLinearUpperLimit(btVector3(0, 0, 0));
    m_btSocket->setAngularLowerLimit(btVector3(0, 0, 0));
    m_btSocket->setAngularUpperLimit(btVector3(0, 0, 0));

    GetBlSimulator()->DynamicsWorld()->addConstraint(m_btSocket, true);
    m_btSocket->setDbgDrawSize(btScalar(5.f));

	m_btJoint = m_btSocket;
}

void BlRPRO::CreateJoint()
{
	SetupGraphics();
	SetupPhysics();
}

#pragma region DataAccesMethods

bool BlRPRO::SetData(const string &strDataType, const string &strValue, bool bThrowError)
{
	if(BlJoint::Physics_SetData(strDataType, strValue))
		return true;

	if(RPRO::SetData(strDataType, strValue, false))
		return true;

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Data Type", strDataType);

	return false;
}

void BlRPRO::QueryProperties(CStdArray<string> &aryNames, CStdArray<string> &aryTypes)
{
	BlJoint::Physics_QueryProperties(aryNames, aryTypes);
	RPRO::QueryProperties(aryNames, aryTypes);
}

#pragma endregion

		}		//Joints
	}			// Environment
}				//BulletAnimatSim
