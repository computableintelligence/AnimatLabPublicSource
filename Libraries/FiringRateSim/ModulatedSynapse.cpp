/**
\file	ModulatedSynapse.cpp

\brief	Implements the modulated synapse class.
**/

#include "StdAfx.h"

#include "Synapse.h"
#include "ModulatedSynapse.h"
#include "Neuron.h"
#include "FiringRateModule.h"

namespace FiringRateSim
{
	namespace Synapses
	{

/**
\brief	Default constructor.

\author	dcofer
\date	3/30/2011
**/
ModulatedSynapse::ModulatedSynapse()
{
	m_strType = "MODULATED";
}

/**
\brief	Destructor.

\author	dcofer
\date	3/30/2011
**/
ModulatedSynapse::~ModulatedSynapse()
{

}

#pragma region DataAccesMethods

float *ModulatedSynapse::GetDataPointer(const std::string &strDataType)
{
	std::string strType = Std_CheckString(strDataType);

	if(strType == "MODULATION")
		return &m_fltModulation;

	return Synapse::GetDataPointer(strDataType);
}

void ModulatedSynapse::QueryProperties(CStdPtrArray<TypeProperty> &aryProperties)
{
	Synapse::QueryProperties(aryProperties);

	aryProperties.Add(new TypeProperty("Modulation", AnimatPropertyType::Float, AnimatPropertyDirection::Get));
}

#pragma endregion

float ModulatedSynapse::CalculateModulation(FiringRateModule *lpModule)
{
	float fltIm=0;
	m_fltModulation=0;

	if(m_bEnabled && m_lpFromNeuron)
	{
		fltIm = m_lpFromNeuron->FiringFreq(lpModule) * m_fltWeight;
		
		if(fltIm>=0)
			m_fltModulation = 1 + fltIm;
		else 
			m_fltModulation = 1/(1-fltIm);
	}
	else
		m_fltModulation = 1;

	return m_fltModulation;
}

	}			//Synapses
}				//FiringRateSim

