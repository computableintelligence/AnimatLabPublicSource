#pragma once

namespace OsgAnimatSim
{
	namespace Environment
	{

        enum DynamicsControlType
        {
            ControlAnimated,
            ControlNode,
            ControlDynamic
        };

        /**
		\brief	Vortex base body class.

		\details This is a base, secondary derived class for all body part objects within animatlab. It is
		derived from VsMovableItem, which contains all of the base OSG graphics manipulation code. It is 
		also derived from IPhysicsBody, which has the methods specifically required by the body part classes.
		
		\author	dcofer
		\date	5/2/2011
		**/
		class ANIMAT_OSG_PORT OsgBody : public OsgMovableItem, public AnimatSim::Environment::IPhysicsBody
		{
		protected:
			BodyPart *m_lpThisBP;
			DynamicsControlType m_eControlType;

			virtual void SetThisPointers();

		public:
			OsgBody();
			virtual ~OsgBody();

			virtual void Physics_SetFreeze(BOOL bVal) {};
			virtual void Physics_SetDensity(float fltVal) {};
			virtual void Physics_SetMaterialID(string strID) {};
			virtual void Physics_SetVelocityDamping(float fltLinear, float fltAngular) {};
			virtual void Physics_SetCenterOfMass(float fltTx, float fltTy, float fltTz) {};
			virtual void Physics_ResizeSelectedReceptiveFieldVertex() {};
			virtual void Physics_FluidDataChanged() {};
			virtual void SetBody() = 0;

			virtual void Physics_UpdateNode() {};
			virtual BOOL Physics_HasCollisionGeometry() {return FALSE;};
		};

	}			// Environment
}				//OsgAnimatSim

