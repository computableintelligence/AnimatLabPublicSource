/**
\file	Synapse.h

\brief	Declares the synapse class.
**/

#pragma once

namespace AnimatCarlSim
{

	/**
	\brief	Firing rate synapse model.

	\details This synapse type has a weight that is a current value. It injects a portion of that weight
	into the post-synaptic neuron based on the pre-synaptic neurons firing rate. I = W*F. (Where W is the
	weight, F is the firing rate, and I is the current.)
		
	\author	dcofer
	\date	3/29/2011
	**/
	class ANIMAT_CARL_SIM_PORT CsSynapseFull : public CsSynapseGroup   
	{
	protected:
		bool m_bNoDirectConnect;

	public:
		CsSynapseFull();
		virtual ~CsSynapseFull();

		virtual void NoDirectConnect(bool bVal);
		virtual bool NoDirectConnect();

		virtual void SetCARLSimulation();

#pragma region DataAccesMethods
		virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
		virtual void QueryProperties(CStdPtrArray<TypeProperty> &aryProperties);
#pragma endregion

		virtual void Load(CStdXml &oXml);
	};

}				//AnimatCarlSim
