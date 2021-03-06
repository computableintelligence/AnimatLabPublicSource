/**
\file	ExternalStimulus.h

\brief	Declares the external stimulus base class. 
**/

#pragma once

namespace AnimatSim
{

	/**
	\namespace	AnimatSim::ExternalStimuli

	\brief	Contains all of the classes that are used to generate external stimuli to body parts, joints, or neural items. 
	**/
	namespace ExternalStimuli
	{
		/**
		\brief	External stimulus base class. 

		\details This is the base class for all stimulus types. If you want to create a new stimulus it needs to be
		derived from this base class.
		
		\author	dcofer
		\date	3/16/2011
		**/
		class ANIMAT_PORT ExternalStimulus : public ActivatedItem 
		{   
		public:
			ExternalStimulus();
			virtual ~ExternalStimulus();
			
			static ExternalStimulus *CastToDerived(AnimatBase *lpBase) {return static_cast<ExternalStimulus*>(lpBase);}

			virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
			virtual void QueryProperties(CStdPtrArray<TypeProperty> &aryProperties);

			//ActiveItem overrides
			virtual bool operator<(ActivatedItem *lpItem);
		};

	}			//ExternalStimuli
}				//AnimatSim
