
#pragma once

namespace VortexAnimatSim
{
	namespace Environment
	{

		/**
		\namespace	VortexAnimatSim::Environment::Joints

		\brief	Joint classes that use the vortex physics engine. 
		**/
		namespace Joints
		{

			class VORTEX_PORT VsHinge : public VsMotorizedJoint, public AnimatSim::Environment::Joints::Hinge     
			{
			protected:
				Vx::VxHinge *m_vxHinge;
				float m_fltRotationDeg;

				//Graphics objects for the hinge drawing code
				osg::ref_ptr<osg::Geometry> m_osgCylinder;
				osg::ref_ptr<osg::MatrixTransform> m_osgCylinderMT;
				osg::ref_ptr<osg::Material> m_osgCylinderMat;
				osg::ref_ptr<osg::StateSet> m_osgCylinderSS;

				osg::ref_ptr<osg::MatrixTransform> m_osgHingeMT;

				virtual void UpdateData();
				virtual void SetupPhysics();
				virtual void DeletePhysics();
				virtual void CreateCylinderGraphics();
				virtual void ResetGraphicsAndPhysics();

			public:
				VsHinge();
				virtual ~VsHinge();

				virtual void Rotation(CStdFPoint &oPoint, BOOL bFireChangeEvent = FALSE, BOOL bUpdateMatrix = TRUE);
				virtual void JointPosition(float fltPos);

				virtual void SetupGraphics();
				virtual void SetAlpha();

#pragma region DataAccesMethods

				virtual float *GetDataPointer(string strDataType);
				virtual BOOL SetData(string strDataType, string strValue, BOOL bThrowError = TRUE);

#pragma endregion

				virtual void EnableLimits(BOOL bVal);
				virtual void CreateJoint();
				virtual void StepSimulation();
			};

		}		//Joints
	}			// Environment
}				//VortexAnimatSim
