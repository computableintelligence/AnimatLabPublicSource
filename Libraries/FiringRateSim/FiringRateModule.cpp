/**
\file	FiringRateModule.cpp

\brief	Implements the firing rate module class.
**/

#include "StdAfx.h"

#include "Synapse.h"
#include "Neuron.h"
#include "FiringRateModule.h"
#include "ClassFactory.h"

namespace FiringRateSim
{
/**
\brief	Default constructor.

\author	dcofer
\date	3/29/2011
**/
FiringRateModule::FiringRateModule()
{
	m_lpClassFactory =  new FiringRateSim::ClassFactory;
	m_bActiveArray = false;
}

/**
\brief	Destructor.

\author	dcofer
\date	3/29/2011
**/
FiringRateModule::~FiringRateModule()
{

try
{
	m_aryNeurons.RemoveAll();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of FiringRateModule\r\n", "", -1, false, true);}
}

std::string FiringRateModule::ModuleName() {return Nl_NeuralModuleName();};

/**
\brief	Gets the active array.

\details Within the neuron it keeps a two bit array to keep track of the previous and current membrane potential calculations.
This tells which of these array elements is currently the active one.

\author	dcofer
\date	3/29/2011

\return	true for array element 1, false for array element 0.
**/
bool FiringRateModule::ActiveArray()
{return m_bActiveArray;}

/**
\brief	Sets the active array.

\details Within the neuron it keeps a two bit array to keep track of the previous and current membrane potential calculations.
This sets which of these array elements is currently the active one.

\author	dcofer
\date	3/29/2011

\param	bVal	true for array element 1, false for array element 0.
**/
void FiringRateModule::ActiveArray(bool bVal)
{
	m_bActiveArray = bVal;
}

/**
\brief	Gets the inactive array.

\details Within the neuron it keeps a two bit array to keep track of the previous and current membrane potential calculations.
This tells which of these array elements is currently the inactive one.

\author	dcofer
\date	3/29/2011

\return	true for array element 1, false for array element 0.
**/
bool FiringRateModule::InactiveArray()
{return !m_bActiveArray;}

/**
\brief	Sets the inactive array.

\details Within the neuron it keeps a two bit array to keep track of the previous and current membrane potential calculations.
This sets which of these array elements is currently the inactive one.

\author	dcofer
\date	3/29/2011

\param	bVal	true for array element 1, false for array element 0.
**/
void FiringRateModule::InactiveArray(bool bVal)
{
	m_bActiveArray = !bVal;
}


void FiringRateModule::Kill(bool bState)
{
	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex])
			m_aryNeurons[iIndex]->Kill(bState);
}

/**
\brief	Searches for the neuron with the specified ID and returns its position in the list.

\author	dcofer
\date	3/29/2011

\param	strID	   	GUID ID of the neruon to find. 
\param	bThrowError	true to throw error if nothing found. 

\return	The found neuron list position.
**/
int FiringRateModule::FindNeuronListPos(std::string strID, bool bThrowError)
{
	std::string sID = Std_ToUpper(Std_Trim(strID));

	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex]->ID() == sID)
			return iIndex;

	if(bThrowError)
		THROW_TEXT_ERROR(Nl_Err_lNeuronNotFound, Nl_Err_strNeuronNotFound, "ID");

	return -1;
}

void FiringRateModule::ResetSimulation()
{
	NeuralModule::ResetSimulation();

	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex])
			m_aryNeurons[iIndex]->ResetSimulation();
}

void FiringRateModule::Initialize()
{
	NeuralModule::Initialize();

	Organism *lpOrganism = dynamic_cast<Organism *>(m_lpStructure);
	if(!lpOrganism) 
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Organism");

	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex])
			m_aryNeurons[iIndex]->Initialize();
}

void FiringRateModule::StepSimulation()
{
	NeuralModule::StepSimulation();

	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex])
			m_aryNeurons[iIndex]->StepSimulation();

	//Swap the active array.
	m_bActiveArray = !m_bActiveArray;
}

#pragma region DataAccesMethods

bool FiringRateModule::SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError)
{
	std::string strType = Std_CheckString(strDataType);

	if(NeuralModule::SetData(strDataType, strValue, false))
		return true;

	if(strType == "TIMESTEP")
	{
		TimeStep(atof(strValue.c_str()));
		return true;
	}

	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Data Type", strDataType);

	return false;
}

void FiringRateModule::QueryProperties(CStdPtrArray<TypeProperty> &aryProperties)
{
	NeuralModule::QueryProperties(aryProperties);

	aryProperties.Add(new TypeProperty("TimeStep", AnimatPropertyType::Float, AnimatPropertyDirection::Set));
}

/**
\brief	Adds a neuron to the module. 

\author	dcofer
\date	3/29/2011

\param	strXml	The xml to use when loading the neuron. 
**/
void FiringRateModule::AddNeuron(std::string strXml, bool bDoNotInit)
{
	CStdXml oXml;
	oXml.Deserialize(strXml);
	oXml.FindElement("Root");
	oXml.FindChildElement("Neuron");

	Neuron *lpNeuron = LoadNeuron(oXml);
	if(!bDoNotInit)
		lpNeuron->Initialize();
}

/**
\brief	Removes the neuron with the specified ID.

\author	dcofer
\date	3/29/2011

\param	strID	   	GUID ID for the neuron. 
\param	bThrowError	true to throw error if neuron found. 
**/
void FiringRateModule::RemoveNeuron(std::string strID, bool bThrowError)
{
	int iPos = FindNeuronListPos(strID, bThrowError);
	m_aryNeurons.RemoveAt(iPos);
}

bool FiringRateModule::AddItem(const std::string &strItemType, const std::string &strXml, bool bThrowError, bool bDoNotInit)
{
	std::string strType = Std_CheckString(strItemType);

	if(strType == "NEURON")
	{
		AddNeuron(strXml, bDoNotInit);
		return true;
	}
	//Synapses are stored in the destination neuron. They will be added there.


	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidItemType, Al_Err_strInvalidItemType, "Item Type", strItemType);

	return false;
}

bool FiringRateModule::RemoveItem(const std::string &strItemType, const std::string &strID, bool bThrowError)
{
	std::string strType = Std_CheckString(strItemType);

	if(strType == "NEURON")
	{
		RemoveNeuron(strID, bThrowError);
		return true;
	}
	//Synapses are stored in the destination neuron. They will be removed there.


	//If it was not one of those above then we have a problem.
	if(bThrowError)
		THROW_PARAM_ERROR(Al_Err_lInvalidItemType, Al_Err_strInvalidItemType, "Item Type", strItemType);

	return false;
}

#pragma endregion

long FiringRateModule::CalculateSnapshotByteSize()
{
	long lByteSize = 0;
	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex])
			lByteSize += m_aryNeurons[iIndex]->CalculateSnapshotByteSize();

	return lByteSize;
}

void FiringRateModule::SaveKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{
	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex])
			m_aryNeurons[iIndex]->SaveKeyFrameSnapshot(aryBytes, lIndex);
}

void FiringRateModule::LoadKeyFrameSnapshot(byte *aryBytes, long &lIndex)
{
	int iCount = m_aryNeurons.GetSize();
	for(int iIndex=0; iIndex<iCount; iIndex++)
		if(m_aryNeurons[iIndex])
			m_aryNeurons[iIndex]->LoadKeyFrameSnapshot(aryBytes, lIndex);
}


void FiringRateModule::Load(CStdXml &oXml)
{
	VerifySystemPointers();

	CStdXml oNetXml;

	//if(Std_IsBlank(m_strProjectPath)) 
	//	THROW_ERROR(Al_Err_lProjectPathBlank, Al_Err_strProjectPathBlank);

	m_arySourceAdapters.RemoveAll();
	m_aryTargetAdapters.RemoveAll();

	oXml.IntoElem();  //Into NeuralModule Element

	LoadNetworkXml(oXml);

	oXml.OutOfElem(); //OutOf NeuralModule Element

	TRACE_DEBUG("Finished loading nervous system config file.");
}

/**
\brief	Loads the network configuration.

\author	dcofer
\date	3/29/2011

\param [in,out]	oXml	The xml to load. 
**/
void FiringRateModule::LoadNetworkXml(CStdXml &oXml)
{
	short iNeuron, iTotalNeurons;
		
	m_aryNeurons.RemoveAll();

	ID(oXml.GetChildString("ID", m_strID));
	Type(oXml.GetChildString("Type", m_strType));
	Name(oXml.GetChildString("Name", m_strName));

	//We do NOT call the TimeStep mutator here because we need to call it only after all modules are loaded so we can calculate the min time step correctly.
	m_fltTimeStep = oXml.GetChildFloat("TimeStep", m_fltTimeStep);

	//This will add this object to the object list of the simulation.
	m_lpSim->AddToObjectList(this);

	//*** Begin Loading Neurons. *****
	oXml.IntoChildElement("Neurons");

	iTotalNeurons = oXml.NumberOfChildren();
	for(iNeuron=0; iNeuron<iTotalNeurons; iNeuron++)
	{
		oXml.FindChildByIndex(iNeuron);
		LoadNeuron(oXml);
	}

	LoadExternalSynapses(oXml);

	oXml.OutOfElem();
	//*** End Loading Neurons. *****
}

/**
\brief	Loads a neuron.

\author	dcofer
\date	3/29/2011

\param [in,out]	oXml	The xml to load for the neuron. 

\return	Pointer to the loaded neuron.
**/
Neuron *FiringRateModule::LoadNeuron(CStdXml &oXml)
{
	Neuron *lpNeuron=NULL;
	std::string strType;

try
{
	//Now lets get the index and type of this neuron
	oXml.IntoElem();  //Into Neuron Element
	strType = oXml.GetChildString("Type");
	oXml.OutOfElem();  //OutOf Neuron Element

	lpNeuron = dynamic_cast<Neuron *>(m_lpSim->CreateObject(Nl_NeuralModuleName(), "Neuron", strType));
	if(!lpNeuron)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "Neuron");

	lpNeuron->SetSystemPointers(m_lpSim, m_lpStructure, this, NULL, true);
	lpNeuron->Load(oXml);
	
	m_aryNeurons.Add(lpNeuron);
	return lpNeuron;
}
catch(CStdErrorInfo oError)
{
	if(lpNeuron) delete lpNeuron;
	RELAY_ERROR(oError);
	return NULL;
}
catch(...)
{
	if(lpNeuron) delete lpNeuron;
	THROW_ERROR(Std_Err_lUnspecifiedError, Std_Err_strUnspecifiedError);
	return NULL;
}
}

}				//FiringRateSim

