/**
\file	MuscleBase.h

\brief	Declares the muscle base class.
**/

#pragma once

namespace AnimatSim
{
	namespace Environment
	{
		namespace Bodies
		{

			/**
			\brief	Muscle base class.

			\details This is a base class for all muscle part types.
			
			\author	dcofer
			\date	5/19/2011
			**/
			class ANIMAT_PORT MuscleBase : public LineBase  
			{
			protected:
				///The maximum tension that this muscle can ever generate. This is an upper limit to prevent unrealistic tension values.
				float m_fltMaxTension;

				///Keeps track of the total stimulation being supplied to this muscle.
				///This is the summation of the firing frequenies from all neurons
				///that are stimulating this muscle.
				float m_fltVm;

				///The derivative of tension at the current time step.
				float m_fltTdot;

				///Tension of the muscle
				float m_fltTension;

				///Tension of the muscle in the last time slice.
				float m_fltPrevTension;

				/// The stimulus-tension gain
				SigmoidGain m_gainStimTension;

				/// The length-tension gain
				LengthTensionGain m_gainLengthTension;

				/**
				\brief	Calculates the tension. 
				
				\author	dcofer
				\date	3/10/2011
				**/
				virtual void CalculateTension() = 0;

			public:
				MuscleBase();
				virtual ~MuscleBase();

				static MuscleBase *CastToDerived(AnimatBase *lpBase) {return static_cast<MuscleBase*>(lpBase);}

				float Tension();
				void Tension(float fltVal);

				float MaxTension();
				void MaxTension(float fltVal);

				float Vm();
				float Tdot();
				float PrevTension();

				virtual bool Enabled();
				virtual void Enabled(bool bVal);

				virtual SigmoidGain *StimTension();
				virtual void StimTension(std::string strXml);

				virtual LengthTensionGain *LengthTension();
				virtual void LengthTension(std::string strXml);

				/**
				\brief	Calculates the activation needed for a given tension value. 
				
				\author	dcofer
				\date	3/10/2011
				
				\param	fltLength		Length of the muscle. 
				\param	fltVelocity		The velocity of change in muscle length. 
				\param	fltT			The tension. 
				\param [in,out]	fltVm	The required voltage activation level. 
				\param [in,out]	fltA	The required activation level. 
				**/
				virtual void CalculateInverseDynamics(float fltLength, float fltVelocity, float fltT, float &fltVm, float &fltA) = 0;
				virtual void AddExternalNodeInput(int iTargetDataType, float fltInput);

				virtual void SetSystemPointers(Simulator *lpSim, Structure *lpStructure, NeuralModule *lpModule, Node *lpNode, bool bVerify);
				virtual void VerifySystemPointers();

				virtual void ResetSimulation();

				virtual float *GetDataPointer(const std::string &strDataType);
				virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
				virtual void QueryProperties(CStdPtrArray<TypeProperty> &aryProperties);

				virtual void Load(CStdXml &oXml);
			};

		}		//Bodies
	}			// Environment
}				//AnimatSim
