#pragma once

namespace AnimatSim
{
	namespace Robotics
	{
		class RemoteControl;

		class ANIMAT_PORT RemoteControlLinkage : public AnimatBase
		{
		protected:
			///Pointer tho the parent remote control
			RemoteControl *m_lpParentRemoteControl;

			///Target node we are inserting into
			Node *m_lpTargetNode;

			///ID of the source node. This is only used during loading.
			std::string m_strLinkedNodeID;

			///ID of the source data type. This is only used during loading.
			std::string m_strSourceDataTypeID;

			///ID of the target data type. This is only used during loading.
			std::string m_strTargetDataTypeID;

			///integer index of the target data type
			int m_iTargetDataType;

			/// Pointer to the source data variable.
			float *m_lpSourceData;
			
			/// Pointer to the Gain that will be used to convert the source value into the target value.
			Gain *m_lpGain;

			virtual void AddGain(std::string strXml);

		public:
			RemoteControlLinkage(void);
			virtual ~RemoteControlLinkage(void);
						
			static RemoteControlLinkage *CastToDerived(AnimatBase *lpBase) {return static_cast<RemoteControlLinkage*>(lpBase);}

			virtual void ParentRemoteControl(RemoteControl *lpParent);
			virtual RemoteControl *ParentRemoteControl();

			virtual std::string SourceDataTypeID();
			virtual void SourceDataTypeID(std::string strTypeID);

			virtual std::string TargetDataTypeID();
			virtual void TargetDataTypeID(std::string strTypeID);

			virtual std::string LinkedNodeID();
			virtual void LinkedNodeID(std::string strID);

			virtual Gain *GetGain();
			virtual void SetGain(Gain *lpGain);

			virtual float *GetDataPointer(const std::string &strDataType);
			virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
			virtual void QueryProperties(CStdPtrArray<TypeProperty> &aryProperties);

			virtual void SetupIO();
			virtual void StepIO();
			virtual void ShutdownIO();

			virtual void Initialize();
			virtual void StepSimulation();
			virtual void Load(CStdXml &oXml);
		};

	}
}