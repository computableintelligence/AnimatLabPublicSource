/**
\file	VsUniversal.h

\brief	Declares the vs universal class.
**/

#pragma once

namespace VortexAnimatSim
{
	namespace Environment
	{
		namespace Joints
		{

			class VORTEX_PORT VsUniversal : public VortexAnimatSim::Environment::VsJoint, public AnimatSim::Environment::Joints::BallSocket     
			{
			protected:
				/// The vortex socket class.
				Vx::VxHomokinetic *m_vxSocket;

				virtual void SetupPhysics();
				virtual void DeletePhysics();

			public:
				VsUniversal();
				virtual ~VsUniversal();

#pragma region DataAccesMethods

				virtual BOOL SetData(string strDataType, string strValue, BOOL bThrowError = TRUE);

#pragma endregion

				virtual void CreateJoint();
			};

		}		//Joints
	}			// Environment
}				//VortexAnimatSim