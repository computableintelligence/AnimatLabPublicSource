/**
\file	Synapse.cpp

\brief	Implements the synapse class.
**/

#include "StdAfx.h"

#include "CsSynapseGroup.h"
#include "CsNeuronGroup.h"
#include "CsNeuralModule.h"
#include "CsSynapseFull.h"

namespace AnimatCarlSim
{

/**
\brief	Default constructor.

\author	dcofer
\date	3/29/2011
**/
CsSynapseFull::CsSynapseFull()
{
}

/**
\brief	Destructor.

\author	dcofer
\date	3/29/2011
**/
CsSynapseFull::~CsSynapseFull()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of CsSynapseOneToOne\r\n", "", -1, false, true);}
}

void CsSynapseFull::SetCARLSimulation()
{
	if(m_lpCsModule && m_lpCsModule->SNN() && m_lpFromNeuron && m_lpToNeuron && m_lpFromNeuron->GroupID() >= 0 && m_lpToNeuron->GroupID() >= 0)
	{
		m_iSynapsesCreated = m_lpCsModule->SNN()->connect(m_lpFromNeuron->GroupID(), m_lpToNeuron->GroupID(), "full", m_fltInitWt, m_fltMaxWt, m_fltPconnect, m_iMinDelay, m_iMaxDelay, m_bPlastic);
	}	
}


}				//AnimatCarlSim






