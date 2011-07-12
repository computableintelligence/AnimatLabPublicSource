/**
\file	VsLight.h

\brief	Declares the vortex Light class.
**/

#pragma once

namespace VortexAnimatSim
{

	/**
	\namespace	VortexAnimatSim::Environment

	\brief	Implements the light object within osg. 
	**/
	namespace Environment
	{
		/**
		\brief	Vortex physical structure implementation. 
		
		\author	dcofer
		\date	4/25/2011
		**/
		class VORTEX_PORT VsLight : public AnimatSim::Environment::Light,  public VsMovableItem   
		{
		protected:
			AnimatSim::Environment::Light *m_lpThisLI;
			
			osg::ref_ptr<osg::Light> m_osgLight;
			osg::ref_ptr<osg::LightSource> m_osgLightSource;

			virtual void SetThisPointers();
			virtual void CreateGraphicsGeometry();
			virtual void SetupGraphics();
			virtual void DeleteGraphics();
			virtual void SetupPhysics() {};
			virtual void DeletePhysics() {};

			virtual void SetupLighting();

		public:
			VsLight();
			virtual ~VsLight();

			virtual void Position(CStdFPoint &oPoint, BOOL bUseScaling = TRUE, BOOL bFireChangeEvent = FALSE, BOOL bUpdateMatrix = TRUE);
			virtual void Ambient(CStdColor &aryColor);
			virtual void Diffuse(CStdColor &aryColor);
			virtual void Specular(CStdColor &aryColor);

			virtual osg::Group *ParentOSG();
			virtual void Create();
			virtual void ResetSimulation();
			virtual void Physics_Resize();
			virtual void Physics_SetColor();
		};

	}			// Environment
}				//VortexAnimatSim
