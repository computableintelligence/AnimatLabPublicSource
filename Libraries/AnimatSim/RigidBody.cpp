/**
\file	RigidBody.cpp

\brief	Implements the rigid body class. 
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
#include "Structure.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Light.h"
#include "LightManager.h"
#include "Simulator.h"

namespace AnimatSim
{
	namespace Environment
	{

/**
\brief	Default constructor. 

\author	dcofer
\date	3/2/2011
**/
RigidBody::RigidBody()
{
	m_bUsesJoint = true;
	m_lpParent = NULL;
	m_lpStructure = NULL;
	m_fltDensity = 1.0;
    m_fltReportDensity = m_fltDensity;

	m_lpJointToParent = NULL;
	m_bFreeze = false;
	m_bIsContactSensor = false;
	m_bIsCollisionObject = false;
	m_fltSurfaceContactCount= 0 ;
	m_fltLinearVelocityDamping = 0;
	m_fltAngularVelocityDamping = 0;
	m_lpContactSensor = NULL;
	m_bFoodSource = false;
	m_fltFoodEaten = 0;
	m_lEatTime = 0;
	m_fltFoodQuantity = 0;
	m_fltFoodQuantityInit = 0;
	m_fltMaxFoodQuantity = 10000;
	m_fltFoodReplenishRate = 0;
	m_fltFoodEnergyContent = 0;

    m_fltMaxHydroForce = 1000;
    m_fltMaxHydroTorque = 1000;
	m_fltBuoyancyScale = 1;
	m_fltMagnus = 0;
	m_bEnableFluids = false;

    m_fltMass = 0;
	m_fltReportMass = 0;
	m_fltReportVolume = 0;

	m_bIsStickyPart = false;
	m_fltStickyOn = 0;
	m_lpStickyChild = NULL;

	m_strMaterialID = "DEFAULTMATERIAL";

    m_bDisplayDebugCollisionGraphic = false;
}

/**
\brief	Destructor. 

\author	dcofer
\date	3/2/2011
**/
RigidBody::~RigidBody()
{

try
{
	m_lpParent = NULL;

	if(m_lpJointToParent) 
	{
		//Set the reference to this object within the joint to NULL.
		//This ensures that the joint does not attempt to make any calls back to this
		//object which is already being deleted.
		m_lpJointToParent->Child(NULL);
		delete m_lpJointToParent;
	}

	m_lpStickyChild = NULL;

	if(m_lpContactSensor) delete m_lpContactSensor;
	m_aryChildParts.RemoveAll();

    //Remove this from the list of extra data parts if it is on it.
    m_lpSim->RemoveFromExtractExtraData(this);

    //If we are not shutting down then we need to remove any references to this object
    //that are located in other parts collision exclusion lists. If we are shutting down
    // then it does not matter, so skip it to prevent potential exceptions.
    if(!m_lpSim->ShuttingDown())
        RemoveCollisionExclusions();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Body\r\n", "", -1, false, true);}
}

CStdFPoint RigidBody::Position()
{
	if(IsRoot())
 		return m_lpStructure->Position();
	else
		return BodyPart::Position();
}

/**
\details For the RigidBody if it is the root then we are going to set the position of the structure.
**/
void RigidBody::Position(CStdFPoint &oPoint, bool bUseScaling, bool bFireChangeEvent, bool bUpdateMatrix) 
{
	if(IsRoot())
 		m_lpStructure->Position(oPoint, bUseScaling, bFireChangeEvent, bUpdateMatrix);
	else
		BodyPart::Position(oPoint, bUseScaling, bFireChangeEvent, bUpdateMatrix);
}

int RigidBody::VisualSelectionType()
{
	if(IsCollisionObject())
		return COLLISION_SELECTION_MODE;
	else
		return GRAPHICS_SELECTION_MODE;
}

/**
\brief	Gets the user specified center of mass. 

\details If this is (0, 0, 0) then the default COM is used for the part. This is
only used if the user sets it to something.

\author	dcofer
\date	3/2/2011

\return	COM point. 
**/
CStdFPoint RigidBody::CenterOfMass() {return m_vCenterOfMass;}

/**
 \brief Gets the center of mass of this part with any static children added.

 \author    David Cofer
 \date  10/10/2013

 \return resultant center of mass of all static parts of this part.
 */
CStdFPoint RigidBody::CenterOfMassWithStaticChildren()
{
    if(HasStaticChildren())
    {
        float fltTotalMass = Mass();
        CStdFPoint vMass = m_vCenterOfMass * Mass();

	    int iCount = m_aryChildParts.GetSize();
	    for(int iIndex=0; iIndex<iCount; iIndex++)
        {
            RigidBody *lpChild = m_aryChildParts[iIndex];
		    if(lpChild->HasStaticJoint())
            {
                vMass += ( (lpChild->Position() + lpChild->CenterOfMass()) * lpChild->Mass());
                fltTotalMass += lpChild->Mass();
            }
        }

        if(fltTotalMass)
            vMass /= fltTotalMass;

        vMass.ClearNearZero();

        return vMass;
    }
    else
        return m_vCenterOfMass;
}

/**
\brief	Sets the user specified center of mass for this part. (m_vCenterOfMass). If COM is (0,0,0) then it is not used.   

\author	dcofer
\date	3/2/2011

\param [in,out]	oPoint	The point. 
**/
void RigidBody::CenterOfMass(CStdFPoint &vPoint, bool bUseScaling)
{
	CenterOfMass(vPoint, bUseScaling, true);
}

void RigidBody::CenterOfMass(CStdFPoint &vPoint, bool bUseScaling, bool bPhysicsCallback)
{
	if(bUseScaling)
		m_vCenterOfMass = vPoint * m_lpMovableSim->InverseDistanceUnits();
	else
		m_vCenterOfMass = vPoint;

	if(m_lpPhysicsBody && bPhysicsCallback)
		m_lpPhysicsBody->Physics_SetCenterOfMass(m_vCenterOfMass.x, m_vCenterOfMass.y, m_vCenterOfMass.z);
}


/**
\brief	Sets the center of mass position. (m_vCenterOfMass). If COM is (0,0,0) then it is not used.  

\author	dcofer
\date	3/2/2011

\param	fltX				The x coordinate. 
\param	fltY				The y coordinate. 
\param	fltZ				The z coordinate. 
\param	bUseScaling			If true then the position values that are passed in will be scaled by
							the unit scaling values. 
**/
void RigidBody::CenterOfMass(float fltX, float fltY, float fltZ, bool bUseScaling)
{
	CStdFPoint vPos(fltX, fltY, fltZ);
	CenterOfMass(vPos, bUseScaling);
}

/**
\brief	Sets the center of mass position for the body. (m_vCenterOfMass). This method is primarily used by the GUI to
reset the local position using an xml data packet. If COM is (0,0,0) then it is not used. 

\author	dcofer
\date	3/2/2011

\param	strXml				The xml string with the data to load in the position. 
\param	bUseScaling			If true then the position values that are passed in will be scaled by
							the unit scaling values. 
**/
void RigidBody::CenterOfMass(std::string strXml, bool bUseScaling)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("COM");
	
	CStdFPoint vPos;
	Std_LoadPoint(oXml, "COM", vPos);
	CenterOfMass(vPos, bUseScaling);
}


/**
\brief	Gets the array of child parts. 

\author	dcofer
\date	3/2/2011

\return	pointer to array of child parts. 
**/
CStdPtrArray<RigidBody> *RigidBody::ChildParts() {return &m_aryChildParts;}

/**
\brief	Gets the joint to parent. 

\author	dcofer
\date	3/2/2011

\return	Pointer to joint that connects this part to its parent.
**/
Joint *RigidBody::JointToParent() {return m_lpJointToParent;}

/**
\brief	Sets the joint to parent. 

\author	dcofer
\date	3/2/2011

\param [in,out]	lpValue	The pointer to the joint. 
**/
void RigidBody::JointToParent(Joint *lpValue) {m_lpJointToParent = lpValue;}

/**
\brief	Gets the receptive field contact sensor. 

\author	dcofer
\date	3/2/2011

\return	Pointer to the receptive field contact sensor object. 
**/
ContactSensor *RigidBody::GetContactSensor() {return m_lpContactSensor;}

/**
\brief	Gets the uniform density. 

\author	dcofer
\date	3/2/2011

\return	Uniform density value of this part. 
**/
float RigidBody::Density() {return m_fltDensity;}

/**
\brief	Sets the uniform density of this part. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The new density value. 
\exception Density must be greater than zero.
**/
void RigidBody::Density(float fltVal, bool bUseScaling) 
{
	Std_IsAboveMin((float) 0, fltVal, true, "Density", true);

	m_fltDensity = fltVal;
	if(bUseScaling)
	{
		m_fltDensity /= m_lpSim->DisplayMassUnits();	//Scale the mass units down to one. If we are using Kg then the editor will save it out as 1000. We need this to be 1 Kg though.
		m_fltDensity *=  pow(m_lpSim->DenominatorDistanceUnits(), 3); //Perform a conversion if necessary because we may be using different units in the denominator.
	}

    m_fltReportDensity = m_fltDensity * m_lpSim->DisplayMassUnits();
    m_fltReportDensity /= pow(m_lpSim->DenominatorDistanceUnits(), 3);

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_SetDensity(m_fltDensity);
};


float RigidBody::Mass() {return m_fltMass;}

void RigidBody::Mass(float fltVal, bool bUseScaling)
{
    Mass(fltVal, bUseScaling, true);
}

void RigidBody::Mass(float fltVal, bool bUseScaling, bool bPhysicsCallback)
{
	Std_IsAboveMin((float) 0, fltVal, true, "Mass", true);

	m_fltMass = fltVal;
	if(bUseScaling)
		m_fltMass /= m_lpSim->DisplayMassUnits();	//Scale the mass units down to one. If we are using Kg then the editor will save it out as 1000. We need this to be 1 Kg though.

    m_fltReportMass = m_fltMass * m_lpSim->DisplayMassUnits();

	if(m_lpPhysicsBody && bPhysicsCallback)
		m_lpPhysicsBody->Physics_SetMass(m_fltMass);
}

float RigidBody::MassWithChildren()
{
    float fltTotalMass = Mass();
	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
    {
        RigidBody *lpChild = m_aryChildParts[iIndex];
            fltTotalMass += lpChild->MassWithChildren();
    }

    return fltTotalMass;
}

float RigidBody::Volume() {return m_fltVolume;}

void RigidBody::Volume(float fltVal, bool bUseScaling)
{
	Std_IsAboveMin((float) 0, fltVal, true, "Volume");

	m_fltVolume = fltVal;
	//if(bUseScaling)
	//	m_fltMass /= m_lpSim->DisplayMassUnits();	//Scale the mass units down to one. If we are using Kg then the editor will save it out as 1000. We need this to be 1 Kg though.

	m_fltReportVolume = m_fltVolume*pow(m_lpSim->DistanceUnits(), (float) 3.0);
}


/**
\brief	Tells if this part is frozen or not

\details Specifies if the part should frozen in place to the world. If a rigid body 
is frozen then it is as if it is nailed in place and can not move. Gravity and 
and other forces will not act on it.

\author	dcofer
\date	3/2/2011

\return	true if frozen, false if else. 
**/
bool RigidBody::Freeze() {return m_bFreeze;}

/**
\brief	Freezes. 

\details Specifies if the part should frozen in place to the world. If a rigid body 
is frozen then it is as if it is nailed in place and can not move. Gravity and 
and other forces will not act on it.

\author	dcofer
\date	3/2/2011

\param	bVal	true to freeze. 
**/
void RigidBody::Freeze(bool bVal)
{
	Freeze(bVal, true);
}

void RigidBody::Freeze(bool bVal, bool bPhysicsCallback)
{
	m_bFreeze = bVal;

	if(m_lpPhysicsBody && bPhysicsCallback)
		m_lpPhysicsBody->Physics_SetFreeze(bVal);
}

/**
\brief	Query if this object is contact sensor. 

\author	dcofer
\date	3/2/2011

\return	true if contact sensor, false if not. 
**/
bool RigidBody::IsContactSensor() {return m_bIsContactSensor;}

/**
\brief	Sets whether this is a contact sensor. 

\author	dcofer
\date	3/2/2011

\param	bVal	true if it is a contact sensor. 
**/
void RigidBody::IsContactSensor(bool bVal) {m_bIsContactSensor = bVal;}

/**
\brief	Query if this object is collision object. 

\author	dcofer
\date	3/2/2011

\return	true if collision object, false if not. 
**/
bool RigidBody::IsCollisionObject() {return m_bIsCollisionObject;}

/**
\brief	Sets whether this part is a collision object. 

\author	dcofer
\date	3/2/2011

\param	bVal true if collision object, false else. 
**/
void RigidBody::IsCollisionObject(bool bVal) {m_bIsCollisionObject = bVal;}

/**
\brief	Query if this object acts as a sticky part. This only applies to contact sensors.
If it is set to true and StickOn is true then when this part touches something else it will
create a locked joint between those two parts. When StickOn goes to false then that joint
will be removed. This allows the contact sensor to act as a kind of sticky pick up pad.

\author	dcofer
\date	8/9/2014

\return	true if sticky object, false if not. 
**/
bool RigidBody::IsStickyPart() {return m_bIsStickyPart;}

/**
\brief	Sets whether this part is a sticky part or not. 

\author	dcofer
\date	8/9/2014

\param	bVal true if sticky object, false else. 
**/
void RigidBody::IsStickyPart(bool bVal) 
{
	m_bIsStickyPart = bVal;
	StickyOn(false);

	//If we are disabling stick parts then make sure to remove the sticky lock
	if(m_lpPhysicsBody && !m_bIsStickyPart)
		m_lpPhysicsBody->Physics_DeleteStickyLock();
}

/**
\brief	Query if this stickyness is turned on or not. This only applies to contact sensors
that also have IsStickyPart true.

\author	dcofer
\date	8/9/2014

\return	true if sticky on, false if not. 
**/
float RigidBody::StickyOn() {return m_fltStickyOn;}

/**
\brief	Sets whether this part has sticky on or not. 

\author	dcofer
\date	8/9/2014

\param	bVal true if sticky is on, false else. 
**/
void RigidBody::StickyOn(float fltVal) 
{
	if(m_bIsStickyPart)
		if(fltVal)
			m_fltStickyOn = 1;
		else
			m_fltStickyOn = 0;
}

/**
\brief	Gets the child body part we are stuck to for a sticky part.

\author	dcofer
\date	8/9/2014

\return	Pointer to stuck child.. 
**/
RigidBody *RigidBody::StickyChild() {return m_lpStickyChild;}

/**
\brief	Sets the child part that we are stuck to if this is a sticky part. 

\author	dcofer
\date	8/9/2014

\param	pointer to stuck child part. 
**/
void RigidBody::StickyChild(RigidBody *lpChild) 
{
	if(m_bIsStickyPart)
		m_lpStickyChild = lpChild;
}

/**
\brief	Query if this is the root rigid body of the structure or not.

\author	dcofer
\date	5/11/2011

\return	true if root, false if not.
**/
bool RigidBody::IsRoot()
{
    if(m_lpStructure)
    	return (m_lpStructure->Body() == this);
    else
        return false;
}

/**
\brief	Query if this object has a static joint.

\author	dcofer
\date	7/2/2011

\return	true if has static joint, false if not.
**/
bool RigidBody::HasStaticJoint()
{
	if(!IsRoot() && !IsContactSensor() && IsCollisionObject() && !JointToParent())
		return true;
	return false;
}

/**
\brief	Query if this rigid body has any static children. 

\description This looks through all of the immediate children of this part to see if they 
have a static joint. If they do it returns true, if not it returns false. It does not look
beyond its immediate children because static parts cannot be nested. They must be placed on
a non-static part.

\author	dcofer
\date	9/7/2013

\return	true if has static children, false if not. 
**/
bool RigidBody::HasStaticChildren()
{
	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryChildParts[iIndex]->HasStaticJoint())
            return true;

    return false;
}

/**
 \brief Gets the mass of all static children.

 \author    David Cofer
 \date  10/10/2013

 \return combined mass of static children.
 */

float RigidBody::StaticChildrenMass()
{
    float fltMass = 0;
	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryChildParts[iIndex]->HasStaticJoint())
            fltMass += m_aryChildParts[iIndex]->Mass();

    return fltMass;
}

/**
\brief	Query if this object is food source. 

\author	dcofer
\date	3/2/2011

\return	true if food source, false if not. 
**/
bool RigidBody::IsFoodSource() {return m_bFoodSource;}

/**
\brief	Sets if this is a food source. 

\author	dcofer
\date	3/2/2011

\param	bVal	true if food source, else false. 
**/
void RigidBody::IsFoodSource(bool bVal) 
{
	m_bFoodSource = bVal;

	if(m_lpSim)
	{
		if(m_bFoodSource)
			m_lpSim->AddFoodSource(this);
		else
			m_lpSim->RemoveFoodSource(this);
	}
}

/**
\brief	Gets the food quantity. 

\author	dcofer
\date	3/2/2011

\return	Food Quantity. 
**/
float RigidBody::FoodQuantity() {return m_fltFoodQuantity;}

/**
\brief	Sets the Food quantity. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The new food quantity value. 
\exception Food Quantity must be zero or greater.
**/
void RigidBody::FoodQuantity(float fltVal) 
{
	Std_InValidRange((float) 0, (float) 100000, fltVal, true, "FoodQuantity");
	m_fltFoodQuantity = fltVal;
	
	//If the sim is running then we do not set the init Qty. Only set it if changed while the sim is not running.
	if(!m_lpSim->SimRunning())
		m_fltFoodQuantityInit = m_fltFoodQuantity;
}

/**
\brief	Gets the amount of food eaten. 

\author	dcofer
\date	3/2/2011

\return	Amount of food eaten. 
**/
float RigidBody::FoodEaten() {return m_fltFoodEaten;}

/**
\brief	Sets the amount of food eaten. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The amount of food eaten. 
**/
void RigidBody::FoodEaten(float fltVal) {m_fltFoodEaten = fltVal;}

/**
\brief	Gets the food replenish rate. 

\author	dcofer
\date	3/2/2011

\return	Food replenish rate. 
**/
float RigidBody::FoodReplenishRate() {return m_fltFoodReplenishRate;}

/**
\brief	Sets the food replenish rate. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The new replenish rate. 
**/
void RigidBody::FoodReplenishRate(float fltVal) 
{
	Std_InValidRange((float) 0, (float) 100000, fltVal, true, "FoodReplenishRate");
	m_fltFoodReplenishRate = fltVal;
}

/**
\brief	Gets the food energy content. 

\author	dcofer
\date	3/2/2011

\return	Food energy content. 
**/
float RigidBody::FoodEnergyContent() {return m_fltFoodEnergyContent;}

/**
\brief	Sets the food energy content. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The new food energy content value. 
\exception Food Quantity must be zero or greater.
**/
void RigidBody::FoodEnergyContent(float fltVal) 
{
	Std_InValidRange((float) 0, (float) 100000, fltVal, true, "FoodEnergyContent");
	m_fltFoodEnergyContent = fltVal;
}
/**
\brief	Gets the maximum food quantity. 

\author	dcofer
\date	3/2/2011

\return	Food Quantity. 
**/
float RigidBody::MaxFoodQuantity() {return m_fltMaxFoodQuantity;}

/**
\brief	Sets the maximum Food quantity. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The new food quantity value. 
\exception Food Quantity must be zero or greater.
**/
void RigidBody::MaxFoodQuantity(float fltVal) 
{
	Std_InValidRange((float) 0, (float) 100000, fltVal, true, "MaxFoodQuantity");
	m_fltMaxFoodQuantity = fltVal;
}

/**
\brief	Gets the linear velocity damping for this body part. 

\author	dcofer
\date	3/2/2011

\return	Linear velocity damping value. 
**/
float RigidBody::LinearVelocityDamping() {return m_fltLinearVelocityDamping;}

/**
\brief	Sets the Linear velocity damping. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The new value. 
\exception Value must be zero or greater.
**/
void RigidBody::LinearVelocityDamping(float fltVal, bool bUseScaling) 
{
	Std_InValidRange((float) 0, (float) 1000, fltVal, true, "RigidBody::LinearVelocityDamping");

	if(bUseScaling)
		fltVal = fltVal/m_lpSim->DisplayMassUnits();
	m_fltLinearVelocityDamping = fltVal;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_SetVelocityDamping(m_fltLinearVelocityDamping, m_fltAngularVelocityDamping);
}

/**
\brief	Gets the angular velocity damping. 

\author	dcofer
\date	3/2/2011

\return	Angular velocity damping for this part. 
**/
float RigidBody::AngularVelocityDamping() {return m_fltAngularVelocityDamping;}

/**
\brief	Sets the angular velocity damping. 

\author	dcofer
\date	3/2/2011

\param	fltVal	The new value. 
\exception Value must be zero or greater.
**/
void RigidBody::AngularVelocityDamping(float fltVal, bool bUseScaling) 
{
	Std_InValidRange((float) 0, (float) 1000, fltVal, true, "RigidBody::AngularVelocityDamping");

	if(bUseScaling)
		fltVal = fltVal/m_lpSim->DisplayMassUnits();
	m_fltAngularVelocityDamping = fltVal;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_SetVelocityDamping(m_fltLinearVelocityDamping, m_fltAngularVelocityDamping);
}

/**
\brief	Gets the material ID for this part.

\details Each rigid body part can be associated with a specific type of material. 
For example, a material like wood, or concrete. The material specifies things like 
the frictional resistance between that part and parts of other materials. Each material
is defined in the GUI and this is a unique ID string that specifies which one to use for this part.

\author	dcofer
\date	3/2/2011

\return	. 
**/
std::string RigidBody::MaterialID() {return m_strMaterialID;}

/**
\brief	Sets the Material ID for this part. 

\details Each rigid body part can be associated with a specific type of material. 
For example, a material like wood, or concrete. The material specifies things like 
the frictional resistance between that part and parts of other materials. Each material
is defined in the GUI and this is a unique ID string that specifies which one to use for this part.

\author	dcofer
\date	3/2/2011

\param	strID	ID of the material to use. 
**/
void RigidBody::MaterialID(std::string strID) 
{
	m_strMaterialID = Std_ToUpper(strID);

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_SetMaterialID(m_strMaterialID);
}

/**
\brief	Gets the relative position to the center of the buoyancy in the body.

\author	dcofer
\date	3/2/2011

\return	returns m_vBuoyancyCenter. 
**/
CStdFPoint RigidBody::BuoyancyCenter() {return m_vBuoyancyCenter;}

/**
\brief	Sets the relative position to the center of the buoyancy in the body.

\author	dcofer
\date	3/2/2011

\param [in,out]	oPoint	The new point to use to set the local position.
\param	bUseScaling   	If true then the position values that are passed in will be scaled by the
						unit scaling values.
**/
void RigidBody::BuoyancyCenter(CStdFPoint &oPoint, bool bUseScaling) 
{
	if(bUseScaling)
		m_vBuoyancyCenter = oPoint * m_lpMovableSim->InverseDistanceUnits();
	else
		m_vBuoyancyCenter = oPoint;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_FluidDataChanged();
}

/**
\brief	Sets the relative position to the center of the buoyancy in the body.

\author	dcofer
\date	3/2/2011

\param	fltX				The x coordinate. 
\param	fltY				The y coordinate. 
\param	fltZ				The z coordinate. 
\param	bUseScaling			If true then the position values that are passed in will be scaled by
							the unit scaling values. 
**/
void RigidBody::BuoyancyCenter(float fltX, float fltY, float fltZ, bool bUseScaling) 
{
	CStdFPoint vPos(fltX, fltY, fltZ);
	BuoyancyCenter(vPos, bUseScaling);
}

/**
\brief	Sets tthe relative position to the center of the buoyancy in the body. This method is primarily used by the GUI to
reset the local position using an xml data packet. 

\author	dcofer
\date	3/2/2011

\param	strXml				The xml string with the data to load in the position. 
\param	bUseScaling			If true then the position values that are passed in will be scaled by
							the unit scaling values. 
**/
void RigidBody::BuoyancyCenter(std::string strXml, bool bUseScaling)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("BuoyancyCenter");
	
	CStdFPoint vPos;
	Std_LoadPoint(oXml, "BuoyancyCenter", vPos);
	BuoyancyCenter(vPos, bUseScaling);
}

/**
\brief	Gets the scale used to calculate the buoyancy value.

\author	dcofer
\date	3/2/2011

\return	scale value. 
**/
float RigidBody::BuoyancyScale() {return m_fltBuoyancyScale;}

/**
\brief	Sets the scale used to calculate the buoyancy value.

\author	dcofer
\date	3/2/2011

\param	fltVal	The new scale value greater than or equal to 0. 
\exception	If	value not greater than or equal to 0. 
**/
void RigidBody::BuoyancyScale(float fltVal) 
{
	Std_IsAboveMin((float) 0, fltVal, true, "BuoyancyScale", true);

	m_fltBuoyancyScale = fltVal;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_FluidDataChanged();
}

/**
\brief	Gets the linear drag coefficients for the three axises for the body.

\author	dcofer
\date	3/2/2011

\return	returns m_vLinearDrag. 
**/
CStdFPoint RigidBody::LinearDrag() {return m_vLinearDrag;}

/**
\brief	Sets the linear drag coefficients for the three axises for the body.

\author	dcofer
\date	3/2/2011

\param [in,out]	oPoint	The new point to use to set the drag.
**/
void RigidBody::LinearDrag(CStdFPoint &oPoint) 
{
	m_vLinearDrag = oPoint;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_FluidDataChanged();
}

/**
\brief	Sets the linear drag coefficients for the three axises for the body.

\author	dcofer
\date	3/2/2011

\param	fltX				The x coordinate. 
\param	fltY				The y coordinate. 
\param	fltZ				The z coordinate. 
**/
void RigidBody::LinearDrag(float fltX, float fltY, float fltZ) 
{
	CStdFPoint vPos(fltX, fltY, fltZ);
	LinearDrag(vPos);
}

/**
\brief	Sets the linear drag coefficients for the three axises for the body. This method is primarily used by the GUI to
reset the local position using an xml data packet. 

\author	dcofer
\date	3/2/2011

\param	strXml				The xml string with the data to load in the position. 
**/
void RigidBody::LinearDrag(std::string strXml)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("LinearDrag");
	
	CStdFPoint vPos;
	Std_LoadPoint(oXml, "LinearDrag", vPos);
	LinearDrag(vPos);
}

/**
\brief	Gets the angular drag coefficients for the three axises for the body.

\author	dcofer
\date	3/2/2011

\return	returns m_vLinearDrag. 
**/
CStdFPoint RigidBody::AngularDrag() {return m_vAngularDrag;}

/**
\brief	Sets the angular drag coefficients for the three axises for the body.

\author	dcofer
\date	3/2/2011

\param [in,out]	oPoint	The new point to use to set the drag.
**/
void RigidBody::AngularDrag(CStdFPoint &oPoint) 
{
	m_vAngularDrag = oPoint;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_FluidDataChanged();
}

/**
\brief	Sets the angular drag coefficients for the three axises for the body.

\author	dcofer
\date	3/2/2011

\param	fltX				The x coordinate. 
\param	fltY				The y coordinate. 
\param	fltZ				The z coordinate. 
**/
void RigidBody::AngularDrag(float fltX, float fltY, float fltZ) 
{
	CStdFPoint vPos(fltX, fltY, fltZ);
	AngularDrag(vPos);
}

/**
\brief	Sets the angular drag coefficients for the three axises for the body. This method is primarily used by the GUI to
reset the local position using an xml data packet. 

\author	dcofer
\date	3/2/2011

\param	strXml				The xml string with the data to load in the position. 
**/
void RigidBody::AngularDrag(std::string strXml)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("AngularDrag");
	
	CStdFPoint vPos;
	Std_LoadPoint(oXml, "AngularDrag", vPos);
	AngularDrag(vPos);
}

/**
\brief	Gets the maximum linear hydrodynamic force that can be applied to this part.

\author	dcofer
\date	3/23/2011

\return	max hydrodyanmic force that can be applied.
**/
float RigidBody::MaxHydroForce() {return m_fltMaxHydroForce;}

/**
\brief	Sets the maximum linear hydrodynamic force that can be applied to this part.

\author	dcofer
\date	3/23/2011

\param	fltVal	The new value. 
\param	bUseScaling	true to use unit scaling. 
**/
void RigidBody::MaxHydroForce(float fltVal, bool bUseScaling) 
{
	Std_IsAboveMin((float) 0, fltVal, true, "MaxHydroForce", true);

	if(bUseScaling)
		fltVal *= (m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits()); //This is a force. 

	m_fltMaxHydroForce = fltVal;
}

/**
\brief	Gets the maximum angular hydrodynamic torque that can be applied to this part.

\author	dcofer
\date	3/23/2011

\return	max hydrodyanmic torque that can be applied.
**/
float RigidBody::MaxHydroTorque() {return m_fltMaxHydroTorque;}

/**
\brief	Sets the maximum angular hydrodynamic torque that can be applied to this part.

\author	dcofer
\date	3/23/2011

\param	fltVal	The new value. 
\param	bUseScaling	true to use unit scaling. 
**/
void RigidBody::MaxHydroTorque(float fltVal, bool bUseScaling) 
{
	Std_IsAboveMin((float) 0, fltVal, true, "MaxHydroTorque", true);

	if(bUseScaling)
		fltVal *= ( m_lpSim->InverseMassUnits() * m_lpSim->InverseDistanceUnits() * m_lpSim->InverseDistanceUnits());

	m_fltMaxHydroTorque = fltVal;
}

/**
\brief	Gets the Magnus coefficient for the body.

\author	dcofer
\date	3/2/2011

\return	Magnus value. 
**/
float RigidBody::Magnus() {return m_fltMagnus;}

/**
\brief	Sets the Magnus coefficient for the body.

\author	dcofer
\date	3/2/2011

\param	fltVal	The new Magnus value greater than or equal to 0. 
\exception	If	value not greater than or equal to 0. 
**/
void RigidBody::Magnus(float fltVal) 
{
	Std_IsAboveMin((float) 0, fltVal, true, "Magnus", true);

	m_fltMagnus = fltVal;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_FluidDataChanged();
}

/**
\brief	Query if this object has fluid interactions turned on. 

\author	dcofer
\date	3/2/2011

\return	true if on, false if not. 
**/
bool RigidBody::EnableFluids() {return m_bEnableFluids;}

/**
\brief	Sets whether this object has fluid interactions turned on. 

\author	dcofer
\date	3/2/2011

\param	bVal	true to turn on, false to turn off. 
**/
void RigidBody::EnableFluids(bool bVal) 
{
	m_bEnableFluids = bVal;

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_FluidDataChanged();
}

bool RigidBody::HasCollisionGeometry()
{
	if(m_lpPhysicsBody)
		return m_lpPhysicsBody->Physics_HasCollisionGeometry();
	else
		return false;
}

/**
\brief	Gets the surface contact count. 

\author	dcofer
\date	3/2/2011

\return	Returns m_fltSurfaceContactCount. 
**/
float RigidBody::SurfaceContactCount() {return m_fltSurfaceContactCount;}


/**
\brief	Increments the surface contact count when this part collides with something in the
virtual world

\details If this item is setup to be a contact sensor then when the physics engine detects a
collision between two objects it will provide this back to us. We then call this method to update
the number of contacts that this object is undergoing. This value can then be used to detect
whether, and how many, contacts are currently happening. 

\author	dcofer
\date	3/2/2011

\param [in,out]	lpContactedSurface	The pointer to the other contacted surface. 
**/
void RigidBody::AddSurfaceContact(RigidBody *lpContactedSurface)
{
	m_fltSurfaceContactCount++;
}

/**
\brief	Decrements the surface contact count when this part stops colliding with something in the
virtual world

\details If this item is setup to be a contact sensor then when the physics engine detects when a
collision between two objects stops, and it will provide this back to us. We then call this
method to update the number of contacts that this object is undergoing. This value can then be
used to detect whether, and how many, contacts are currently happening. 

\author	dcofer
\date	3/2/2011

\param [in,out]	lpContactedSurface	If non-null, the pointer to a contacted surface. 
**/
void RigidBody::RemoveSurfaceContact(RigidBody *lpContactedSurface)
{
	if(m_fltSurfaceContactCount>0)
		m_fltSurfaceContactCount--;
}


/**
\brief	Direclty sets the surface contact count for when this part is contacting another rigid body part

\details If this item is setup to be a contact sensor then when the physics engine detects when a
collision between two objects stops, and it will provide this back to us. We then call this
method to update the number of contacts that this object is undergoing. This value can then be
used to detect whether, and how many, contacts are currently happening. 

\author	dcofer
\date	9/5/2013

\param [in]	iCount	The number of surface contacts. Must be 0 or larger. 
**/
void RigidBody::SetSurfaceContactCount(int iCount)
{
    if(iCount >= 0)
        m_fltSurfaceContactCount = iCount;

	////Test code
	//if(iCount > 0)
	//	iCount = iCount;
}


/**
\brief	This item is eating the specified amount of food. 

\author	dcofer
\date	3/2/2011

\param	fltVal		The amount of food to eat. 
\param	lTimeSlice	The time slice during which the eating is occuring. 
**/
void RigidBody::Eat(float fltBiteSize, long lTimeSlice)
{
	if(m_lEatTime != lTimeSlice)
		m_fltFoodEaten = 0;

	m_fltFoodQuantity -= fltBiteSize;
	m_fltFoodEaten += fltBiteSize;
	m_lEatTime = lTimeSlice;
}

/**
\brief	Enables collision between the past-in object and this object.

\details This method enables collision responses between the rigid body being past in and this
rigid body. This is a virtual method that should be overridden in the simulator system. You need
to call physics engine API's to enable the collision responses between these two objects. This
method does nothing by default. 

\author	dcofer
\date	3/2/2011

\param [in,out]	lpBody	The pointer to a body. 
**/
void RigidBody::EnableCollision(RigidBody *lpBody)
{
    if(!lpBody)
        THROW_TEXT_ERROR(Al_Err_lBodyNotDefined, Al_Err_strBodyNotDefined, "EnableCollision");

    if(FindCollisionExclusionBody(lpBody, false))
        m_aryExcludeCollisionSet.erase(lpBody);

    if(lpBody->FindCollisionExclusionBody(this, false))
        lpBody->m_aryExcludeCollisionSet.erase(this);

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_EnableCollision(lpBody);

	if(lpBody->m_lpPhysicsBody)
		lpBody->m_lpPhysicsBody->Physics_EnableCollision(this);
}

/**
\brief	Disables collision between the past-in object and this object.

\details This method disables collision responses between the rigid body being past in and this
rigid body. This is a virtual method that should be overridden in the simulator system. You need
to call physics engine API's to disable the collision responses between these two objects. This
method does nothing by default. 

\author	dcofer
\date	3/2/2011

\param [in,out]	lpBody	The pointer to a body. 
**/
void RigidBody::DisableCollision(RigidBody *lpBody)
{
    if(!lpBody)
        THROW_TEXT_ERROR(Al_Err_lBodyNotDefined, Al_Err_strBodyNotDefined, "DisableCollision");

    if(!FindCollisionExclusionBody(lpBody, false))
        m_aryExcludeCollisionSet.insert(lpBody);

    if(!lpBody->FindCollisionExclusionBody(this, false))
        lpBody->m_aryExcludeCollisionSet.insert(this);

	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_DisableCollision(lpBody);

	if(lpBody->m_lpPhysicsBody)
		lpBody->m_lpPhysicsBody->Physics_DisableCollision(this);
}

/**
\brief	Searches the exclusion collision list to see if the specified part is already present.

\author	dcofer
\date	3/28/2011

\param	lpBody	Body part to find in the exclusion list. 
\param	bThrowError  	true to throw error if there is a problem. 

\return	index of found body part, or -1 if not found
\exception If bThrowError=True and no part is found it throws an exception.
**/
bool RigidBody::FindCollisionExclusionBody(RigidBody *lpBody, bool bThrowError)
{
    std::unordered_set<RigidBody *>::iterator oPos;
    oPos = m_aryExcludeCollisionSet.find(lpBody);

	if(oPos != m_aryExcludeCollisionSet.end())
		return true;
	else if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lItemNotFound, Al_Err_strItemNotFound, "Exclusion List Body Part: " + lpBody->ID());

	return false;
}

/**
 \brief Called by the desctructor. It removes this object from all other collision exclusion lists.
 This prevents any attempt to reference this deleted object while looping through collision exclusions.

 \author    David Cofer
 \date  1/14/2014
 */
void RigidBody::RemoveCollisionExclusions()
{
    for (std::unordered_set<RigidBody *>::iterator itr = m_aryExcludeCollisionSet.begin(); itr != m_aryExcludeCollisionSet.end(); ++itr) 
    {
        RigidBody *lpBody = *(itr);

       if(lpBody->FindCollisionExclusionBody(this, false))
       {
           lpBody->m_aryExcludeCollisionSet.erase(this);   

           if(lpBody->m_lpPhysicsBody)
		        lpBody->m_lpPhysicsBody->Physics_DisableCollision(this);
       }
    }
}

/**
\brief	Gets a parent that has collision geometry.

\description This is used for static joints. We need to add the new collision
geometry to a parent part that actually has collision geometry as well.

\author	dcofer
\date	5/20/2012

\return	null if it fails, else pointer to RigidBody with geometry.
**/
RigidBody *RigidBody::ParentWithCollisionGeometry()
{
	if(HasCollisionGeometry())
		return this;
	else if(m_lpParent)
		return m_lpParent->ParentWithCollisionGeometry();
	else
		return false;
}

void RigidBody::Kill(bool bState)
{
	BodyPart::Kill(bState);

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->Kill(bState);

	if(m_lpJointToParent)
		m_lpJointToParent->Kill(bState);
}

void RigidBody::ResetSimulation()
{
	BodyPart::ResetSimulation();

	////Test code
	//int i=5;
	//if(Std_ToLower(m_strID) == "b42f968f-0639-4a69-9974-9e0f411d40d8") // && m_lpSim->Time() > 1.8
	//	i=6;

	///It is <b>very</b> important that the physcis of the rigid body is reset
    /// before the joint and the child parts. The reason is that we want the position
    /// of this part to be rest first and then child parts because if we do not then
    /// when we do an UpdateFromNode on the physcis object it will be in the wrong location.
    /// We must reset the position from the root out, not leafs in.
	if(m_lpPhysicsMovableItem)
		m_lpPhysicsMovableItem->Physics_ResetSimulation();

	if(m_lpJointToParent)
		m_lpJointToParent->ResetSimulation();

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->ResetSimulation();

	m_fltFoodQuantity = m_fltFoodQuantityInit;
	m_fltFoodEaten = 0;
	m_fltSurfaceContactCount = 0;
	StickyOn(false);
}

void RigidBody::AfterResetSimulation()
{
	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->AfterResetSimulation();

	if(m_lpJointToParent)
		m_lpJointToParent->AfterResetSimulation();
}

/**
\brief	Allows the rigid body to create its parts using the chosen physics engine.

\details This function can not be truly implemented in the Animat library. It must be implemented
in the next layer sitting above it. The reason for this is that the Animat library was made to be
generalized so it could work with a number of different physics engines. Therefore it is not
tightly coupled with any one engine. This in turn means that we can not implement the code in
this library neccessary to create a part or joint in the chosen engine. Several overridable
functions have been provided that allow you to do this. The two that will always have to be
overridden are the CreateParts and CreateJoints methods. CreateParts makes the API calls to the
physics engine to create the collision models, graphics models and so on. You should still call
the base class method at the end of your overridden method so the rigid body can walk down the
tree and create the parts for its children. 

\author	dcofer
\date	3/2/2011
**/

void RigidBody::CreateParts()
{
	//We have the replenish rate in Quantity/s, but we need it in Quantity/timeslice. Lets recalculate it here.
	if(m_bFoodSource)
		m_fltFoodReplenishRate = (m_fltFoodReplenishRate * m_lpSim->PhysicsTimeStep());

    CreateChildParts();
}

/**
\brief	Loops through all the child parts of this rigid body and call CreateParts.

\details This is called by CreateParts method to recursively call CreateParts for all children. 

\author	dcofer
\date	10/7/2013
**/
void RigidBody::CreateChildParts()
{
	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->CreateParts();
}

/**
\brief	Allows the rigid body to create its joints using the chosen physics engine.

\details This function can not be truly implemented in the Animat library. It must be implemented
in the next layer sitting above it. The reason for this is that the Animat library was made to be
generalized so it could work with a number of different physics engines. Therefore it is not
tightly coupled with any one engine. This in turn means that we can not implement the code in
this library neccessary to create a part or joint in the chosen engine. Several overridable
functions have been provided that allow you to do this. The two that will always have to be
overridden are the CreateParts and CreateJoints methods. CreateJoints makes the API calls to the
physics engine to create the joint and constraints and motors. You should still call the base
class method at the end of your overridden method so the rigid body can walk down the tree and
create the joints for its children. 

\author	dcofer
\date	3/2/2011
**/
void RigidBody::CreateJoints()
{
    CreateChildJoints();
}

/**
\brief	Loops through all the child parts of this rigid body and call CreateJoints.

\details This is called by CreateJoints method to recursively call CreateJoints for all children. 

\author	dcofer
\date	10/7/2013
**/
void RigidBody::CreateChildJoints()
{
	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->CreateJoints();
}

void RigidBody::SimPausing()
{
	//If the simulation is pausing then we need to remove any sticky locks
	if(m_lpPhysicsBody && m_bIsStickyPart)
		m_lpPhysicsBody->Physics_DeleteStickyLock();
}

int RigidBody::GetTargetDataTypeIndex(const std::string &strDataType)
{
	std::string strType = Std_CheckString(strDataType);

	if(strType == "BODYFORCEX")
		return BODY_FORCE_X_TYPE;
	else if(strType == "BODYFORCEY")
		return BODY_FORCE_Y_TYPE;
	else if(strType == "BODYFORCEZ")
		return BODY_FORCE_Z_TYPE;
	else if(strType == "STICKYON")
		return STICKY_ON_TYPE;
	else if(strType == "STICKYOFF")
		return STICKY_OFF_TYPE;
	else 
		return -1;

}

void RigidBody::AddExternalNodeInput(int iTargetDataType, float fltInput)
{
	if(iTargetDataType == STICKY_ON_TYPE && fltInput > 0.5)
		m_fltStickyOn = 1;
	else if(iTargetDataType == STICKY_OFF_TYPE && fltInput > 0.5)
		m_fltStickyOn = 0;

	////Test Code
	//if(m_fltStickyOn > 0.8)
	//	fltInput = fltInput;
}

void RigidBody::StepSimulation()
{
    BodyPart::StepSimulation();

	if(m_bFoodSource)
	{
		m_fltFoodQuantity = m_fltFoodQuantity + m_fltFoodReplenishRate;
		if(m_fltFoodQuantity > m_fltMaxFoodQuantity)
			m_fltFoodQuantity = m_fltMaxFoodQuantity;

		//Clear the food eaten variable if it has been around for too long.
		if(m_fltFoodEaten && m_lEatTime != m_lpSim->TimeSlice())
			m_fltFoodEaten = 0;
	}

    if(m_lpPhysicsBody && m_lpSim->SimulateHydrodynamics() && m_bEnableFluids)
        m_lpPhysicsBody->Physics_StepHydrodynamicSimulation();

	//Must update the data before calling step sim on children because they may depend on 
	//some of the data that is collected, like the world matrix for this object.
	UpdateData();

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->StepSimulation();

	if(m_lpJointToParent)
		m_lpJointToParent->StepSimulation();
}

#pragma region DataAccesMethods

float *RigidBody::GetDataPointer(const std::string &strDataType)
{
	std::string strType = Std_CheckString(strDataType);

	if(strType == "FOODQUANTITY")
		return &m_fltFoodQuantity;
	if(strType == "FOODEATEN")
		return &m_fltFoodEaten;
	if(strType == "ENABLE")
		return &m_fltEnabled;
	if(strType == "CONTACTCOUNT")
		return &m_fltSurfaceContactCount;
	if(strType == "DENSITY")
	{
		GetDensity();
		return &m_fltReportDensity;
	}
	if(strType == "MASS")
	{
		GetMass();
		return &m_fltReportMass;
	}
	if(strType == "VOLUME")
	{
		GetVolume();
		return &m_fltReportVolume;
	}
	if(strType == "STICKYON")
	{
		return &m_fltStickyOn;
	}

	float *lpData = NULL;
	if(m_lpPhysicsMovableItem)
	{
		float *lpData = NULL;
		lpData = m_lpPhysicsMovableItem->Physics_GetDataPointer(strDataType);
		if(lpData) return lpData;
	}

	return BodyPart::GetDataPointer(strDataType);
}


bool RigidBody::SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError)
{
	std::string strType = Std_CheckString(strDataType);

	if(BodyPart::SetData(strType, strValue, false))
		return true;

	if(strType == "FREEZE")
	{
		Freeze(Std_ToBool(strValue));
		return true;
	}

	if(strType == "DENSITY")
	{
		Density((float) atof(strValue.c_str()));
		return true;
	}

	if(strType == "MASS")
	{
		Mass((float) atof(strValue.c_str()));
		return true;
	}

	if(strType == "COM")
	{
		CenterOfMass(strValue);
		return true;
	}

	if(strType == "FOODSOURCE")
	{
		IsFoodSource(Std_ToBool(strValue));
		return true;
	}
	
	if(strType == "FOODQUANTITY")
	{
		FoodQuantity((float) atof(strValue.c_str()));
		return true;
	}
	
	if(strType == "MAXFOODQUANTITY")
	{
		MaxFoodQuantity((float) atof(strValue.c_str()));
		return true;
	}
	
	if(strType == "FOODREPLENISHRATE")
	{
		FoodReplenishRate((float) atof(strValue.c_str()));
		return true;
	}
	
	if(strType == "FOODENERGYCONTENT")
	{
		FoodEnergyContent((float) atof(strValue.c_str()));
		return true;
	}

	if(strDataType == "BUOYANCYCENTER")
	{
		BuoyancyCenter(strValue);
		return true;
	}

	if(strType == "BUOYANCYSCALE")
	{
		BuoyancyScale((float) atof(strValue.c_str()));
		return true;
	}

	if(strDataType == "LINEARDRAG")
	{
		LinearDrag(strValue);
		return true;
	}

	if(strDataType == "LINEARDRAG.X")
	{
		LinearDrag(atof(strValue.c_str()), m_vLinearDrag.y, m_vLinearDrag.z);
		return true;
	}

	if(strDataType == "LINEARDRAG.Y")
	{
		LinearDrag(m_vLinearDrag.x, atof(strValue.c_str()), m_vLinearDrag.z);
		return true;
	}

	if(strDataType == "LINEARDRAG.Z")
	{
		LinearDrag(m_vLinearDrag.x, m_vLinearDrag.y, atof(strValue.c_str()));
		return true;
	}

	if(strDataType == "ANGULARDRAG")
	{
		AngularDrag(strValue);
		return true;
	}

	if(strDataType == "ANGULARDRAG.X")
	{
		AngularDrag(atof(strValue.c_str()), m_vAngularDrag.y, m_vAngularDrag.z);
		return true;
	}

	if(strDataType == "ANGULARDRAG.Y")
	{
		AngularDrag(m_vAngularDrag.x, atof(strValue.c_str()), m_vAngularDrag.z);
		return true;
	}

	if(strDataType == "ANGULARDRAG.Z")
	{
		AngularDrag(m_vAngularDrag.x, m_vAngularDrag.y, atof(strValue.c_str()));
		return true;
	}
    
	if(strType == "MAXHYDROFORCE")
	{
		MaxHydroForce((float) atof(strValue.c_str()));
		return true;
	}
    
	if(strType == "MAXHYDROTORQUE")
	{
		MaxHydroTorque((float) atof(strValue.c_str()));
		return true;
	}

	if(strType == "MAGNUS")
	{
		Magnus((float) atof(strValue.c_str()));
		return true;
	}

	if(strDataType == "ENABLEFLUIDS")
	{
		EnableFluids(Std_ToBool(strValue));
		return true;
	}

	if(strDataType == "MATERIALTYPEID")
	{
		MaterialID(strValue);
		return true;
	}

	if(strDataType == "ISSTICKYPART")
	{
		IsStickyPart(Std_ToBool(strValue));
		return true;
	}

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Data Type", strDataType);

	return false;
}

void RigidBody::QueryProperties(CStdPtrArray<TypeProperty> &aryProperties)
{
	BodyPart::QueryProperties(aryProperties);

	aryProperties.Add(new TypeProperty("FoodEaten", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("Enable", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("ContactCount", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("Volume", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
	aryProperties.Add(new TypeProperty("StickyOn", AnimatPropertyType::Boolean, AnimatPropertyDirection::Get));

	aryProperties.Add(new TypeProperty("Freeze", AnimatPropertyType::Boolean, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("Density", AnimatPropertyType::Float, AnimatPropertyDirection::Both));
	aryProperties.Add(new TypeProperty("Mass", AnimatPropertyType::Float, AnimatPropertyDirection::Both));
	aryProperties.Add(new TypeProperty("CenterOfMass", AnimatPropertyType::Xml, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("FoodSource", AnimatPropertyType::Boolean, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("FoodQuantity", AnimatPropertyType::Float, AnimatPropertyDirection::Both));
	aryProperties.Add(new TypeProperty("MaxFoodQuantity", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("FoodReplenishRate", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("FoodEnergyContent", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("BuoyancyCenter", AnimatPropertyType::Xml, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("BuoyancyScale", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("LinearDrag", AnimatPropertyType::Xml, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("LinearDrag.X", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("LinearDrag.Y", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("LinearDrag.Z", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("AngularDrag", AnimatPropertyType::Xml, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("AngularDrag.X", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("AngularDrag.Y", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("AngularDrag.Z", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("MaxHydroForce", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("MaxHydroTorque", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("Magnus", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("EnableFluids", AnimatPropertyType::Boolean, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("MaterialTypeID", AnimatPropertyType::String, AnimatPropertyDirection::Set));
	aryProperties.Add(new TypeProperty("IsStickyPart", AnimatPropertyType::Boolean, AnimatPropertyDirection::Set));
}

bool RigidBody::AddItem(const std::string &strItemType, const std::string &strXml, bool bThrowError, bool bDoNotInit)
{
	std::string strType = Std_CheckString(strItemType);

	if(strType == "RIGIDBODY")
	{
		AddRigidBody(strXml);
		return true;
	}

	if(strType == "CONTACTSENSOR")
	{
		AddContactSensor(strXml);
		return true;
	}

	if(strType == "ODOR")
	{
		AddOdor(strXml, bDoNotInit);
		return true;
	}

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidItemType, Al_Err_strInvalidItemType, "Item Type", strItemType);

	return false;
}

bool RigidBody::RemoveItem(const std::string &strItemType, const std::string &strID, bool bThrowError)
{
	std::string strType = Std_CheckString(strItemType);

	if(strType == "RIGIDBODY")
	{
		RemoveRigidBody(strID);
		return true;
	}

	if(strType == "CONTACTSENSOR")
	{
		RemoveContactSensor(strID);
		return true;
	}

	if(strType == "ODOR")
	{
		RemoveOdor(strID);
		return true;
	}

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidItemType, Al_Err_strInvalidItemType, "Item Type", strItemType);

	return false;
}

/**
\brief	Creates and adds a rigid body. 

\author	dcofer
\date	3/2/2011

\param	strXml	The xml data packet for loading the body. 
**/
RigidBody *RigidBody::AddRigidBody(std::string strXml)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("RigidBody");

	RigidBody *lpBody = LoadRigidBody(oXml);

	lpBody->Initialize();

	//First create all of the model objects.
	lpBody->CreateParts();

	//Then create all of the joints between the models.
	lpBody->CreateJoints();

    //Inform the physics object that we have added a child body.
    if(m_lpPhysicsBody)
        m_lpPhysicsBody->Physics_ChildBodyAdded(lpBody);

    if(m_lpSim && lpBody)
        m_lpSim->NotifyRigidBodyAdded(lpBody->ID());

    return lpBody;
}

/**
\brief	Removes the rigid body with the specified ID. 

\author	dcofer
\date	3/2/2011

\param	strID	ID of the body to remove
\param	bThrowError	If true and ID is not found then it will throw an error.
\exception If bThrowError is true and ID is not found.
**/
void RigidBody::RemoveRigidBody(std::string strID, bool bThrowError)
{
	int iPos = FindChildListPos(strID, bThrowError);

    RigidBody *lpChild = m_aryChildParts[iPos];
    bool bHasStaticJoint = bHasStaticJoint = lpChild->HasStaticJoint();

	m_aryChildParts.RemoveAt(iPos);
	m_lpStructure->RemoveRigidBody(strID);

    //Inform the physics object that we are removing a child body.
    if(m_lpPhysicsBody)
        m_lpPhysicsBody->Physics_ChildBodyRemoved(bHasStaticJoint);    

    if(m_lpSim)
        m_lpSim->NotifyRigidBodyRemoved(strID);
}


/**
\brief	Finds the array index for the child part with the specified ID

\author	dcofer
\date	3/2/2011

\param	strID ID of part to find
\param	bThrowError	If true and ID is not found then it will throw an error, else return NULL
\exception If bThrowError is true and ID is not found.

\return	If bThrowError is false and ID is not found returns NULL, 
else returns the pointer to the found part.
**/
int RigidBody::FindChildListPos(std::string strID, bool bThrowError)
{
	std::string sID = Std_ToUpper(Std_Trim(strID));

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryChildParts[iIndex]->ID() == sID)
			return iIndex;

	if(bThrowError)
		THROW_TEXT_ERROR(Al_Err_lBodyOrJointIDNotFound, Al_Err_strBodyOrJointIDNotFound, "ID");

	return -1;
}

/**
\brief	Creates and adds a ContactSensor. 

\author	dcofer
\date	3/2/2011

\param	strXml	The xml data packet for loading the body. 
**/
void RigidBody::AddContactSensor(std::string strXml)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("ContactSensor");

	LoadContactSensor(oXml);

	if(m_lpPhysicsBody && m_lpContactSensor)
		m_lpPhysicsBody->Physics_ContactSensorAdded(m_lpContactSensor);
}

/**
\brief	Removes the ContactSensor. 

\author	dcofer
\date	3/2/2011

\param	strID	ID of the contact senosr to remove
\param	bThrowError	If true and ID is not found then it will throw an error.
\exception If bThrowError is true and ID is not found.
**/
void RigidBody::RemoveContactSensor(std::string strID, bool bThrowError)
{
	if(m_lpContactSensor)
	{
		delete m_lpContactSensor;
		m_lpContactSensor = NULL;

	    if(m_lpPhysicsBody)
		    m_lpPhysicsBody->Physics_ContactSensorRemoved();
	}
}

void RigidBody::UpdatePhysicsPosFromGraphics()
{
	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_UpdateNode();

    UpdateChildPhysicsPosFromGraphics();
}

void RigidBody::UpdateChildPhysicsPosFromGraphics()
{
	if(m_lpJointToParent)
		m_lpJointToParent->UpdatePhysicsPosFromGraphics();

	int iCount = m_aryChildParts.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		m_aryChildParts[iIndex]->UpdatePhysicsPosFromGraphics();
}

#pragma endregion

void RigidBody::LoadPosition(CStdXml &oXml)
{
	CStdFPoint vTemp;

	Std_LoadPoint(oXml, "Position", vTemp);

	if(!IsRoot())
	{
		Position(vTemp, true, false, false);	
		//AbsolutePosition(m_lpParent->AbsolutePosition() + m_oPosition);
	}
	else
	{
		CStdFPoint oPos = m_lpStructure->Position();
		BodyPart::Position(oPos, false, false, false);
	}
}

void RigidBody::Load(CStdXml &oXml)
{
	CStdFPoint vPoint;

	if(m_lpJointToParent) {delete m_lpJointToParent; m_lpJointToParent=NULL;}
	m_aryChildParts.RemoveAll();

	m_lpSim->IncrementPhysicsBodyCount();

	BodyPart::Load(oXml);

	oXml.IntoElem();  //Into RigidBody Element

	vPoint.Set(0, 0, 0);
	if(oXml.FindChildElement("COM", false))
	{
		Std_LoadPoint(oXml, "COM", vPoint);
		CenterOfMass(vPoint, true, false);
	}
	else
		CenterOfMass(vPoint, true, false);

	Density(oXml.GetChildFloat("Density", m_fltDensity));

    //Get Mass first and only set it if it is >= 0. If it is less than this
    //Then we need to calculate it during initialization instead of using the loaded value.
    m_fltMass = oXml.GetChildFloat("Mass", m_fltMass);
    if(m_fltMass >= 0)
        Mass(m_fltMass, true, false);

	MaterialID(oXml.GetChildString("MaterialTypeID", m_strMaterialID));
	Freeze(oXml.GetChildBool("Freeze", m_bFreeze), false);
	IsContactSensor(oXml.GetChildBool("IsContactSensor", m_bIsContactSensor));
	IsCollisionObject(oXml.GetChildBool("IsCollisionObject", m_bIsCollisionObject));
	IsStickyPart(oXml.GetChildBool("IsStickyPart", m_bIsStickyPart));

	IsFoodSource(oXml.GetChildBool("FoodSource", m_bFoodSource));
	FoodQuantity(oXml.GetChildFloat("FoodQuantity", m_fltFoodQuantity));
	MaxFoodQuantity(oXml.GetChildFloat("MaxFoodQuantity", m_fltMaxFoodQuantity));
	FoodReplenishRate(oXml.GetChildFloat("FoodReplenishRate", m_fltFoodReplenishRate));
	FoodEnergyContent(oXml.GetChildFloat("FoodEnergyContent", m_fltFoodEnergyContent));

	LinearVelocityDamping(oXml.GetChildFloat("LinearVelocityDamping", m_fltLinearVelocityDamping));
	AngularVelocityDamping(oXml.GetChildFloat("AngularVelocityDamping", m_fltAngularVelocityDamping));

	vPoint.Set(0, 0, 0);
	Std_LoadPoint(oXml, "BuoyancyCenter", vPoint, false);
	BuoyancyCenter(vPoint);

	vPoint.Set(0, 0, 0);
	Std_LoadPoint(oXml, "LinearDrag", vPoint, false);
	LinearDrag(vPoint);

	Std_LoadPoint(oXml, "AngularDrag", vPoint, false);
	AngularDrag(vPoint);

    MaxHydroForce(oXml.GetChildFloat("MaxHydroForce", m_fltMaxHydroForce));
    MaxHydroTorque(oXml.GetChildFloat("MaxHydroTorque", m_fltMaxHydroTorque));

	BuoyancyScale(oXml.GetChildFloat("BuoyancyScale", m_fltBuoyancyScale));
	Magnus(oXml.GetChildFloat("Magnus", m_fltMagnus));
	EnableFluids(oXml.GetChildBool("EnableFluids", m_bEnableFluids));


	//Only load the joint if there is a parent object and this body uses joints.
	if(m_lpParent && m_bUsesJoint)
	{
		//Static joints do not have joints specified. Any time that there is a parent but not joint defined
		//then this signals that we need to statically add that part to the parent object geometry.
		if(oXml.FindChildElement("Joint", false))
			LoadJoint(oXml);
		else
			m_lpJointToParent = NULL;
	}

	if(oXml.FindChildElement("ChildBodies", false))
	{
		oXml.IntoElem();  //Into ChildBodies Element
		int iChildCount = oXml.NumberOfChildren();

		for(int iIndex=0; iIndex<iChildCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadRigidBody(oXml);
		}
		oXml.OutOfElem(); //OutOf ChildBodies Element
	}

	LoadContactSensor(oXml);

	if(oXml.FindChildElement("OdorSources", false))
	{
		oXml.IntoElem();  //Into OdorSources Element
		int iChildCount = oXml.NumberOfChildren();
		
		for(int iIndex=0; iIndex<iChildCount; iIndex++)
		{
			oXml.FindChildByIndex(iIndex);
			LoadOdor(oXml);
		}
		oXml.OutOfElem(); //OutOf OdorSources Element
	}	

	oXml.OutOfElem(); //OutOf RigidBody Element
}

void RigidBody::LoadContactSensor(CStdXml &oXml)
{
	if(m_lpContactSensor)
		THROW_TEXT_ERROR(Al_Err_lContactSensorExists, Al_Err_strContactSensorExists, " RigidBodyID: " + m_strID);

	if(oXml.FindChildElement("ContactSensor", false))
	{
		m_lpContactSensor = new AnimatSim::Environment::ContactSensor();
		m_lpContactSensor->SetSystemPointers(m_lpSim, m_lpStructure, NULL, this, true);
		m_lpContactSensor->Load(oXml);
	}
}

/**
\brief	Loads a child rigid body. 

\author	dcofer
\date	3/2/2011

\param [in,out]	oXml	The xml data definition of the part to load. 

\return	null if it fails, else the rigid body. 
**/

RigidBody *RigidBody::LoadRigidBody(CStdXml &oXml)
{
	RigidBody *lpChild = NULL;
	std::string strType;

try
{
	oXml.IntoElem(); //Into Child Element
	std::string strModule = oXml.GetChildString("ModuleName", "");
	strType = oXml.GetChildString("Type");
	oXml.OutOfElem(); //OutOf Child Element

	lpChild = dynamic_cast<RigidBody *>(m_lpSim->CreateObject(strModule, "RigidBody", strType));
	if(!lpChild)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "RigidBody");
	
	lpChild->Parent(this);
	lpChild->SetSystemPointers(m_lpSim, m_lpStructure, NULL, this, true);

	lpChild->Load(oXml);

	m_aryChildParts.Add(lpChild);
	m_lpStructure->AddRigidBody(lpChild);

	return lpChild;
}
catch(CStdErrorInfo oError)
{
	if(lpChild) delete lpChild;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpChild) delete lpChild;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

/**
\brief	Loads a child joint. 

\author	dcofer
\date	3/2/2011

\param [in,out]	oXml	The xml data definition of the part to load. 

\return	null if it fails, else the joint. 
**/

Joint *RigidBody::LoadJoint(CStdXml &oXml)
{
	std::string strType;

try
{
	oXml.IntoChildElement("Joint"); //Into Joint Element
	std::string strModule = oXml.GetChildString("ModuleName", "");
	std::string strJointType = oXml.GetChildString("Type");
	oXml.OutOfElem();  //OutOf Joint Element

	m_lpJointToParent = dynamic_cast<Joint *>(m_lpSim->CreateObject(strModule, "Joint", strJointType));
	if(m_lpJointToParent)
	{
		m_lpJointToParent->Parent(m_lpParent);
		m_lpJointToParent->Child(this);

		m_lpJointToParent->SetSystemPointers(m_lpSim, m_lpStructure, NULL, this, true);

		m_lpJointToParent->Load(oXml);

		m_lpStructure->AddJoint(m_lpJointToParent);
	}

	return m_lpJointToParent;
}
catch(CStdErrorInfo oError)
{
	if(m_lpJointToParent) delete m_lpJointToParent;
	m_lpJointToParent = NULL;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(m_lpJointToParent) delete m_lpJointToParent;
	m_lpJointToParent = NULL;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

/**
\brief	Adds an odor source to this body part. 

\author	dcofer
\date	3/2/2011

\param [in,out]	lpOdor	The pointer to an odor source to add. 
**/

void RigidBody::AddOdor(Odor *lpOdor)
{
	if(!lpOdor)
		THROW_ERROR(Al_Err_lOdorNotDefined, Al_Err_strOdorNotDefined);

	try
	{
			m_aryOdorSources.Add(lpOdor->ID(), lpOdor);
	}
	catch(CStdErrorInfo oError)
	{
		oError.m_strError += " Duplicate odor type Key: " + lpOdor->ID(); 
		RELAY_ERROR(oError);
	}
}


void RigidBody::AddOdor(std::string strXml, bool bDoNotInit)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("Odor");

	Odor *lpOdor = LoadOdor(oXml);

	if(!bDoNotInit)
		lpOdor->Initialize();
}

void RigidBody::RemoveOdor(std::string strID, bool bThrowError)
{
	m_aryOdorSources.Remove(strID);
}

/**
\brief	Loads an odor source. 

\author	dcofer
\date	3/2/2011

\param [in,out]	oXml	The xml data to use when loading the odor. 

\return	Pointer to the new odor. 
**/

Odor *RigidBody::LoadOdor(CStdXml &oXml)
{
	Odor *lpOdor = NULL;

try
{
	lpOdor = new Odor(this);

	lpOdor->SetSystemPointers(m_lpSim, m_lpStructure, NULL, this, true);
	lpOdor->Load(oXml);

	AddOdor(lpOdor);

	return lpOdor;
}
catch(CStdErrorInfo oError)
{
	if(lpOdor) delete lpOdor;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpOdor) delete lpOdor;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

/**
\brief	Adds a world-coordinate based force vector to this body at a specified local body position. 

\author	dcofer
\date	2/9/2014

\param	fltPx		The x position relative to the COM in local part coordinates. 
\param	fltPy		The y position relative to the COM in local part coordinates.  
\param	fltPz		The z position relative to the COM in local part coordinates.  
\param	fltFx		The x force in world coordinates. 
\param	fltFy		The y force in world coordinates. 
\param	fltFz		The z force in world coordinates. 
\param	bScaleUnits	If true then the force and value is scaled by the ScaleUnits, otherwise it is
					applied as provided. 
**/

void RigidBody::AddForceAtLocalPos(float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz, bool bScaleUnits)
{
	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_AddBodyForceAtLocalPos(fltPx, fltPy, fltPz, fltFx, fltFy, fltFz, bScaleUnits);
}

/**
\brief	Adds a world-coordinate based force vector to this body at a specified world-coordinate body position. 

\author	dcofer
\date	2/9/2014

\param	fltPx		The x position in world coordinates. 
\param	fltPy		The y position in world coordinates. 
\param	fltPz		The z position in world coordinates.  
\param	fltFx		The x force in world coordinates. 
\param	fltFy		The y force in world coordinates. 
\param	fltFz		The z force in world coordinates. 
\param	bScaleUnits	If true then the force and value is scaled by the ScaleUnits, otherwise it is
					applied as provided. 
**/

void RigidBody::AddForceAtWorldPos(float fltPx, float fltPy, float fltPz, float fltFx, float fltFy, float fltFz, bool bScaleUnits)
{
	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_AddBodyForceAtWorldPos(fltPx, fltPy, fltPz, fltFx, fltFy, fltFz, bScaleUnits);
}

/**
\brief	Adds a torque to this body about its center. 

\author	dcofer
\date	3/2/2011

\param	fltTx		The torque about the x axis. 
\param	fltTy		The torque about the y axis. 
\param	fltTz		The torque about the z axis. 
\param	bScaleUnits	If true then the force and value is scaled by the ScaleUnits, otherwise it is
					applied as provided. 
**/
void RigidBody::AddTorque(float fltTx, float fltTy, float fltTz, bool bScaleUnits)
{
	if(m_lpPhysicsBody)
		m_lpPhysicsBody->Physics_AddBodyTorque(fltTx, fltTy, fltTz, bScaleUnits);
}

/**
\brief	Gets a velocity of this body at specified point in the body. 

\author	dcofer
\date	3/2/2011

\param	x	The x coordinate. 
\param	y	The y coordinate. 
\param	z	The z coordinate. 

\return	The velocity at point. 
**/
CStdFPoint RigidBody::GetVelocityAtPoint(float x, float y, float z)
{
	CStdFPoint vLoc(0, 0, 0);

	if(m_lpPhysicsBody)
		vLoc = m_lpPhysicsBody->Physics_GetVelocityAtPoint(x, y, z);

	return vLoc;
}

/**
\brief	Gets the mass of this part. 

\author	dcofer
\date	3/2/2011

\return	The mass. 
**/
float RigidBody::GetMass()
{
	if(m_lpPhysicsBody)
		m_fltMass = m_lpPhysicsBody->Physics_GetMass();

	m_fltReportMass = m_fltMass*m_lpSim->DisplayMassUnits();

	return m_fltMass;
}

/**
\brief	Gets the density of this part. 

\author	dcofer
\date	3/2/2011

\return	The density. 
**/
float RigidBody::GetDensity()
{
	if(m_lpPhysicsBody)
		m_fltDensity = m_lpPhysicsBody->Physics_GetDensity();

    m_fltReportDensity = m_fltDensity * m_lpSim->DisplayMassUnits();
    m_fltReportDensity /= pow(m_lpSim->DenominatorDistanceUnits(), 3);

	return m_fltDensity;
}

/**
 \brief Gets the mass of this part and all static children

 \author    David Cofer
 \date  10/10/2013

 \return    The mass value of this part with static children.
 */

float RigidBody::GetMassValueWithStaticChildren()
{
    if(HasStaticChildren())
        return m_fltMass + StaticChildrenMass();
    else
    	return m_fltMass;
}


/**
\brief	Gets the volume of this part. 

\author	dcofer
\date	3/2/2011

\return	The volume. 
**/
float RigidBody::GetVolume()
{
	if(m_lpPhysicsBody)
		m_fltMass = m_lpPhysicsBody->Physics_GetMass();

	float fltVolume = 0;
	
	if(m_fltDensity)
		fltVolume = m_fltMass/m_fltDensity;

	m_fltReportVolume = fltVolume*pow(m_lpSim->DistanceUnits(), (float) 3.0);

	return fltVolume;
}

	}			//Environment
}				//AnimatSim