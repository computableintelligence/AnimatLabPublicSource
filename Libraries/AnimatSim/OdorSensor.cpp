/**
\file	OdorSensor.cpp

\brief	Implements the odor sensor class. 
**/

#include "StdAfx.h"
#include "IMovableItemCallback.h"
#include "ISimGUICallback.h"
#include "AnimatBase.h"

#include "Node.h"
#include "IPhysicsMovableItem.h"
#include "IPhysicsBody.h"
#include "BoundingBox.h"
#include "MovableItem.h"
#include "BodyPart.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Sensor.h"
#include "OdorType.h"
#include "Odor.h"
#include "OdorSensor.h"
#include "Structure.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "Light.h"
#include "LightManager.h"
#include "Simulator.h"

namespace AnimatSim
{
	namespace Environment
	{
		namespace Bodies
		{
/**
\brief	Default constructor. 

\author	dcofer
\date	3/10/2011
**/
OdorSensor::OdorSensor()
{
	m_fltOdorValue = 0;
	m_lpOdorType = NULL;
}

/**
\brief	Destructor. 

\author	dcofer
\date	3/10/2011
**/
OdorSensor::~OdorSensor()
{
	m_lpOdorType = NULL;
}

/**
\brief	Sets the odor type identifier.

\author	dcofer
\date	6/12/2011

\param	strID	Identifier for the odor type.
**/
void OdorSensor::OdorTypeID(std::string strID)
{
	SetOdorTypePointer(strID);
	m_strOdorTypeID = strID;
}

/**
\brief	Gets the odor type identifier.

\author	dcofer
\date	6/12/2011

\return	ID.
**/
std::string OdorSensor::OdorTypeID() {return m_strOdorTypeID;}

/**
\brief	Sets the odor type pointer.

\author	dcofer
\date	6/12/2011

\param	strID	Identifier for the odor type.
**/
void OdorSensor::SetOdorTypePointer(std::string strID)
{
	if(Std_IsBlank(strID))
		m_lpOdorType = NULL;
	else
	{
		m_lpOdorType = dynamic_cast<OdorType *>(m_lpSim->FindByID(strID));
		if(!m_lpOdorType)
			THROW_PARAM_ERROR(Al_Err_lPartTypeNotOdorType, Al_Err_strPartTypeNotOdorType, "ID", strID);
	}
}

void OdorSensor::ResetSimulation()
{
	Sensor::ResetSimulation();
	m_fltOdorValue = 0;
}

bool OdorSensor::SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError)
{
	std::string strType = Std_CheckString(strDataType);

	if(Sensor::SetData(strType, strValue, false))
		return true;

	if(strType == "ODORTYPEID")
	{
		OdorTypeID(strValue);
		return true;
	}

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Data Type", strDataType);

	return false;
}

void OdorSensor::QueryProperties(CStdPtrArray<TypeProperty> &aryProperties)
{
	Sensor::QueryProperties(aryProperties);

	aryProperties.Add(new TypeProperty("OdorValue", AnimatPropertyType::Float, AnimatPropertyDirection::Get));

	aryProperties.Add(new TypeProperty("OdorTypeID", AnimatPropertyType::String, AnimatPropertyDirection::Set));
}

float *OdorSensor::GetDataPointer(const std::string &strDataType)
{
	std::string strType = Std_CheckString(strDataType);

	if(strType == "ODORVALUE")
		return &m_fltOdorValue;

	return RigidBody::GetDataPointer(strDataType);
}

void OdorSensor::StepSimulation()
{
	Sensor::StepSimulation();

	if(m_lpOdorType)
		m_fltOdorValue = m_lpOdorType->CalculateOdorValue(m_oAbsPosition);
}

void OdorSensor::Load(CStdXml &oXml)
{
	Sensor::Load(oXml);

	oXml.IntoElem();  //Into RigidBody Element

	OdorTypeID(oXml.GetChildString("OdorTypeID", ""));

	oXml.OutOfElem(); //OutOf RigidBody Element
}

		}		//Bodies
	}			//Environment
}				//AnimatSim

