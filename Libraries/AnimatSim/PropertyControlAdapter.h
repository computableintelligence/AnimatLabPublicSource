#pragma once

namespace AnimatSim
{

	/**
	\namespace	AnimatSim::Adapters

	\brief	Namespace for the adapter objects.

	\details Adapters are generalized mechanism that allows items in one module to talk to items in another. For example, 
	It allows a neuron in a firing rate neural module to inject current into an integrate and fire neuron, or to talk to the
	physics engine.
	**/
	namespace Adapters
	{

		/**
		\class	Adapter
		
		\brief	Adapter. 
		
		\details Adapters are generalized mechanism that allows items in one module to talk to items in another. For example, 
		It allows a neuron in a firing rate neural module to inject current into an integrate and fire neuron, or to talk to the
		physics engine.

		\author	dcofer
		\date	2/28/2011
		**/
		class ANIMAT_PORT PropertyControlAdapter : public Adapter 
		{
		protected:
			/// Pointer to the target node
			AnimatBase *m_lpTargetObject;

			float m_fltPreviousSetVal;
			float m_fltSetThreshold;
			float m_fltInitialValue;
			float m_fltFinalValue;

			string m_strPropertyName;
			AnimatBase::AnimatPropertyType m_ePropertyType;

			virtual void SetPropertyValue(float fltVal);
			virtual void SetDestinationID(string strXml);

		public:
			PropertyControlAdapter();
			virtual ~PropertyControlAdapter();

			virtual AnimatBase *TargetObject();
			
			virtual void PropertyName(string strPropName);
			virtual string PropertyName();

			virtual void SetThreshold(float fltThreshold);
			virtual float SetThreshold();

			virtual void InitialValue(float fltVal);
			virtual float InitialValue();

			virtual void FinalValue(float fltVal);
			virtual float FinalValue();

			virtual void Initialize();
			virtual void ResetSimulation();
			virtual void SimStarting();
			virtual BOOL SetData(const string &strDataType, const string &strValue, BOOL bThrowError = TRUE);
			virtual void QueryProperties(CStdArray<string> &aryNames, CStdArray<string> &aryTypes);
			virtual void StepSimulation();
			virtual void Load(CStdXml &oXml);
		};

	}			//Adapters
}				//AnimatSim