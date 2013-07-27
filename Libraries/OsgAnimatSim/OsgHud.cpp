/**
\file	OsgHud.cpp

\brief	Implements the vortex heads-up display class.
**/

#include "StdAfx.h"
#include "VsSimulator.h"

namespace OsgAnimatSim
{
	namespace Visualization
	{
/**
\brief	Default constructor.

\author	dcofer
\date	7/7/2011
**/
OsgHud::OsgHud()
{
}

/**
\brief	Destructor.

\author	dcofer
\date	7/7/2011
**/
OsgHud::~OsgHud()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of OsgHud\r\n", "", -1, FALSE, TRUE);}
}

void OsgHud::Initialize()
{
	AnimatBase::Initialize();

	m_osgProjection = new osg::Projection;
    m_osgProjection->setMatrix(osg::Matrix::ortho2D(0, 800, 0, 600));

	HudItem *lpItem = NULL;
	int iCount = m_aryHudItems.GetSize();
	for(int iIndex = 0; iIndex < iCount; iIndex++)
	{
		lpItem = m_aryHudItems[iIndex];
		lpItem->Initialize(m_osgProjection.get());
	}

	m_osgMT = new osg::MatrixTransform;
    m_osgMT->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    m_osgMT->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    m_osgMT->getOrCreateStateSet()->setAttributeAndModes(new osg::Program, osg::StateAttribute::ON);
    m_osgMT->addChild(m_osgProjection.get());

	VsSimulator *lpVsSim = dynamic_cast<VsSimulator *>(m_lpSim);
	if(!lpVsSim)
		THROW_ERROR(Osg_Err_lUnableToConvertToVsSimulator, Osg_Err_strUnableToConvertToVsSimulator);

	lpVsSim->OSGRoot()->addChild(m_osgMT.get());
}

void OsgHud::Update()
{
	HudItem *lpItem = NULL;
	int iCount = m_aryHudItems.GetSize();
	for(int iIndex = 0; iIndex < iCount; iIndex++)
	{
		lpItem = m_aryHudItems[iIndex];
		lpItem->Update();
	}
}


	}			// Visualization
}				//OsgAnimatSim
