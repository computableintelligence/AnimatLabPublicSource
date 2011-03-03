/**
\file	NeuralModule.cpp

\brief	Implements the neural module class. 
**/

#include "stdafx.h"
#include "IBodyPartCallback.h"
#include "AnimatBase.h"

#include "Node.h"
#include "IPhysicsBody.h"
#include "BodyPart.h"
#include "Gain.h"
#include "Adapter.h"
#include "Joint.h"
#include "ReceptiveField.h"
#include "ContactSensor.h"
#include "RigidBody.h"
#include "Structure.h"
#include "NeuralModule.h"
#include "NervousSystem.h"
#include "Organism.h"
#include "ActivatedItem.h"
#include "ActivatedItemMgr.h"
#include "DataChartMgr.h"
#include "ExternalStimuliMgr.h"
#include "KeyFrame.h"
#include "SimulationRecorder.h"
#include "OdorType.h"
#include "Odor.h"
#include "Simulator.h"

namespace AnimatSim
{

	/**
	\namespace	AnimatSim::Behavior

	\brief	Contains objects related to neural networks.

	\details This contains all of the objects related to the nervous system
	and neural networks. 

	**/
	namespace Behavior
	{

/**
\fn	NeuralModule::NeuralModule()

\brief	Default constructor. 

\author	dcofer
\date	2/24/2011
**/
NeuralModule::NeuralModule()
{
	m_iTargetAdapterCount = 0;
	m_iTimeStepInterval = 0;
	m_fltTimeStep = -1;
	m_iTimeStepCount = 0;
	m_lpClassFactory = NULL;
	m_lpSim = NULL;
	m_lpOrganism = NULL;
}

/**
\fn	NeuralModule::~NeuralModule()

\brief	Destructor. 

\details This deletes the class factory when destroyed.

\author	dcofer
\date	2/24/2011
**/
NeuralModule::~NeuralModule()
{

try
{
	if(m_lpClassFactory)
		{delete m_lpClassFactory; m_lpClassFactory = NULL;}

	m_arySourceAdapters.RemoveAll();
	m_aryTargetAdapters.RemoveAll();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of NeuralModule\r\n", "", -1, FALSE, TRUE);}
}

/**
\fn	IStdClassFactory *NeuralModule::ClassFactory()

\brief	Gets the class factory. 

\author	dcofer
\date	2/24/2011

\return	null if it fails, else. 
**/
IStdClassFactory *NeuralModule::ClassFactory() {return m_lpClassFactory;}

/**
\fn	Simulator *NeuralModule::GetSimulator()

\brief	Gets the simulator. 

\author	dcofer
\date	2/24/2011

\return	returns the simulator. 
**/
Simulator *NeuralModule::GetSimulator() {return m_lpSim;}

/**
\fn	Organism *NeuralModule::GetOrganism()

\brief	Gets the organism. 

\author	dcofer
\date	2/24/2011

\return	returns the organism. 
**/
Organism *NeuralModule::GetOrganism() {return m_lpOrganism;}

/**
\fn	short NeuralModule::TimeStepInterval()

\brief	Gets the time step interval. 

\details The m_iTimeStepInterval is the number of time slices between
that this module must wait before stepping again.

\author	dcofer
\date	2/24/2011

\return	. 
**/
short NeuralModule::TimeStepInterval()
{return m_iTimeStepInterval;}

/**
\fn	void NeuralModule::TimeStepInterval(short iVal)

\brief	Sets ime step interval. 

\author	dcofer
\date	2/24/2011

\param	iVal	New time step interval. 
\exception new value must be greater than zero.
**/

void NeuralModule::TimeStepInterval(short iVal)
{
	Std_IsAboveMin((int) 0, (int) iVal, TRUE, "TimeStepInterval");
	m_iTimeStepInterval = iVal;
}

float NeuralModule::TimeStep()
{return m_fltTimeStep;}

void NeuralModule::TimeStep(float fltVal)
{
	Std_IsAboveMin((float) 0, (float) fltVal, TRUE, "TimeStep");

	//Find the number of timeslices that need to occur before this module is updated
	m_iTimeStepInterval = fltVal / m_lpSim->TimeStep();

	//Now recaculate the time step using the minimum time step as the base.
	m_fltTimeStep = m_lpSim->TimeStep() * m_iTimeStepInterval;

	//Check to see if this time step is less than the physics time step.
	//If it is then we need to re-update that timestep.
	if(m_fltTimeStep < m_lpSim->TimeStep())
		m_lpSim->TimeStep(m_fltTimeStep);
}

string NeuralModule::ProjectPath()
{return m_strProjectPath;}

void NeuralModule::ProjectPath(string strPath)
{m_strProjectPath = strPath;}

string NeuralModule::NeuralNetworkFile()
{return m_strNeuralNetworkFile;}

void NeuralModule::NeuralNetworkFile(string strFile)
{m_strNeuralNetworkFile = strFile;}

BOOL NeuralModule::NeedToStep()
{
	m_iTimeStepCount++;

	if(m_iTimeStepInterval == m_iTimeStepCount)
	{
		m_iTimeStepCount = 0;
		return TRUE;
	}
	else
		return FALSE;
}


void NeuralModule::AttachSourceAdapter(Adapter *lpAdapter)
{
	m_arySourceAdapters.Add(lpAdapter);
}

void NeuralModule::AttachTargetAdapter(Adapter *lpAdapter)
{
	m_aryTargetAdapters.Add(lpAdapter);
	m_iTargetAdapterCount = m_aryTargetAdapters.GetSize();
}

void NeuralModule::Initialize(Simulator *lpSim, Structure *lpStructure)
{
	m_lpSim = lpSim;
	m_lpOrganism = dynamic_cast<Organism *>(lpStructure);
	if(!m_lpOrganism) 
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Organism");
}

void NeuralModule::SetSystemPointers(Simulator *lpSim, Structure *lpStructure, NeuralModule *lpModule, Node *lpNode)
{
	AnimatBase::SetSystemPointers(lpSim, lpStructure, lpModule, lpNode);

	m_lpOrganism = dynamic_cast<Organism *>(lpStructure);
	if(!m_lpOrganism) 
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Organism");
}

void NeuralModule::StepSimulation(Simulator *lpSim, Structure *lpStructure)
{
	for(int iIndex=0; iIndex<m_iTargetAdapterCount; iIndex++)
		m_aryTargetAdapters[iIndex]->StepSimulation(lpSim, lpStructure);
}

	}			//Behavior
}			//AnimatSim
