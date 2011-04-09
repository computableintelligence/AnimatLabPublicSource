/**
\file	Prismatic.h

\brief	Declares the prismatic class.
**/

#pragma once

namespace AnimatSim
{
	namespace Environment
	{
		namespace Joints
		{
 
			/**
			\brief	A Prismatic type of joint.
						   
			\details This type of joint is constrained so that it can only
			move linearly up and down one axis. You can define which axis it moves along
			around in the configuration file using the normalized 
			RotationAxis vector element. You can also specify the
			translational constraints for this joint. This prevents it
			from moving further than the constrained value.<br>
			Also, this joint is motorized. So you can specify a desired
			velocity of motion at a given time step using a motor velocity stimulus
			and the physics engine will automatically apply the forces
			necessary to move the joint at the desired velocity.

			\author	dcofer
			\date	3/24/2011
			**/
			class ANIMAT_PORT Prismatic : public MotorizedJoint    
			{
			protected:
				/// Upper limit constring pointer.
				ConstraintLimit *m_lpUpperLimit;

				/// Lower limit constring pointer.
				ConstraintLimit *m_lpLowerLimit;

				/// Pointer to a constraint that is used to represent the position flap.
				ConstraintLimit *m_lpPosFlap;

				/**
				\brief	Creates the cylinder graphics for the axis of movement.
				
				\author	dcofer
				\date	4/7/2011
				**/
				virtual void CreateCylinderGraphics() = 0;

			public:
				Prismatic();
				virtual ~Prismatic();

				float CylinderRadius();
				float CylinderHeight();
				float BoxSize();

				virtual void Enabled(BOOL bValue);

				virtual ConstraintLimit *UpperLimit() ;
				virtual ConstraintLimit *LowerLimit();

				virtual float GetPositionWithinLimits(float fltPos);
				virtual float GetLimitRange();

				virtual BOOL UsesRadians() {return FALSE;};

				virtual void ResetSimulation();
				virtual BOOL SetData(string strDataType, string strValue, BOOL bThrowError = TRUE);
				virtual void AddExternalNodeInput(float fltInput);

				virtual void Load(CStdXml &oXml);
			};

		}		//Joints
	}			// Environment
}				//AnimatSim
