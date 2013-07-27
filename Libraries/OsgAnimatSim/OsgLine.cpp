// OsgLine.cpp: implementation of the OsgLine class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "OsgMovableItem.h"
#include "OsgBody.h"
#include "OsgJoint.h"
#include "OsgLine.h"
#include "OsgDragger.h"


namespace OsgAnimatSim
{
	namespace Environment
	{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OsgLine::OsgLine()
{
}

OsgLine::~OsgLine()
{

try
{
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of OsgLine\r\n", "", -1, FALSE, TRUE);}
}

void OsgLine::SetThisPointers()
{
	VsRigidBody::SetThisPointers();

	m_lpLineBase = dynamic_cast<LineBase *>(this);
	if(!m_lpLineBase)
		THROW_TEXT_ERROR(Osg_Err_lThisPointerNotDefined, Osg_Err_strThisPointerNotDefined, "m_lpLineBase, " + m_lpThisAB->Name());
}

osg::Geometry *OsgLine::CreateLineGeometry()
{ 
    osg::Geometry* linesGeom = new osg::Geometry();
    int iCount = BuildLines(linesGeom);

    // set the colors as before, plus using the above
	CStdColor &aryDiffuse = *m_lpThisRB->Diffuse();
    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(aryDiffuse[0], aryDiffuse[1], aryDiffuse[2], aryDiffuse[3]));
    linesGeom->setColorArray(colors);
    linesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
    

    // set the normal in the same way color.
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    linesGeom->setNormalArray(normals);
    linesGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);


    // This time we simply use primitive, and hardwire the number of coords to use 
    // since we know up front,
	osg::PrimitiveSet *lpSet = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, iCount);
    linesGeom->addPrimitiveSet(lpSet);

	linesGeom->setDataVariance(osg::Object::DYNAMIC);
	linesGeom->setUseDisplayList(false);

	return linesGeom;
}

int OsgLine::BuildLines(osg::Geometry *linesGeom)
{
	//Always create the graphics. If it is disabled or made not visible we will turn it off.
	CStdArray<Attachment *> *aryAttachments = m_lpLineBase->AttachmentPoints();
	int iCount = aryAttachments->GetSize();

	//If we already have a lines array then delete it and rebuild.
	if(m_aryLines.valid())
		m_aryLines.release();

	m_aryLines = new osg::Vec3Array;

	//Only draw the line if we have more than one point.
	if(iCount > 1)
	{
		CStdFPoint vPos;
		Attachment *lpPrevAttach=NULL;
		Attachment *lpAttach=NULL;

		for(int iIndex=1; iIndex<iCount; iIndex++)
		{
			lpPrevAttach = aryAttachments->at(iIndex-1);
			lpAttach = aryAttachments->at(iIndex);

			vPos = lpPrevAttach->AbsolutePosition();
			m_aryLines->push_back(osg::Vec3(vPos.x, vPos.y, vPos.z));

			vPos = lpAttach->AbsolutePosition();
			m_aryLines->push_back(osg::Vec3(vPos.x, vPos.y, vPos.z));
		}
	}

    linesGeom->setVertexArray(m_aryLines.get());
	linesGeom->dirtyBound();

	return m_aryLines->getNumElements();
}

void OsgLine::DrawLine()
{
	CStdArray<Attachment *> *aryAttachments = m_lpLineBase->AttachmentPoints();
	int iCount = aryAttachments->GetSize();

	//Only draw the line if we have more than one point.
	if(iCount > 1)
	{
		CStdFPoint vPos;
		Attachment *lpPrevAttach=NULL;
		Attachment *lpAttach=NULL;
		int iLineIdx = 0;

		for(int iIndex=1; iIndex<iCount; iIndex++)
		{
			lpPrevAttach = aryAttachments->at(iIndex-1);
			lpAttach = aryAttachments->at(iIndex);

			vPos = lpPrevAttach->AbsolutePosition();
			(*m_aryLines)[iLineIdx].set(vPos.x, vPos.y, vPos.z);

			vPos = lpAttach->AbsolutePosition();
			(*m_aryLines)[iLineIdx+1].set(vPos.x, vPos.y, vPos.z);

			iLineIdx+=2;
		}

		m_osgGeometry->dirtyBound();
	}
}

void OsgLine::SetupGraphics()
{
	//Add it to the root scene graph because the vertices are in global coords.
	GetVsSimulator()->OSGRoot()->addChild(m_osgNode.get());
	SetVisible(m_lpThisMI->IsVisible());
}

void OsgLine::DeleteGraphics()
{
	if(m_osgGeometry.valid())
	{
		m_osgGeometry->setDataVariance(osg::Object::STATIC);
		m_osgGeometry->dirtyBound();
		SetVisible(FALSE);
	}

	VsRigidBody::DeleteGraphics();
}

void OsgLine::CreateGraphicsGeometry()
{
	fltA = 0;
	m_osgGeometry = CreateLineGeometry();
}

void OsgLine::CreatePhysicsGeometry()
{
	m_vxGeometry = NULL;
}

void OsgLine::CreateParts()
{
	CreateGeometry();

	VsRigidBody::CreateItem();
	VsRigidBody::SetBody();
}

void OsgLine::CalculateForceVector(Attachment *lpPrim, Attachment *lpSec, float fltTension, CStdFPoint &oPrimPos, CStdFPoint &oSecPos, CStdFPoint &oPrimForce)
{
	oPrimPos = lpPrim->AbsolutePosition();
	oSecPos = lpSec->AbsolutePosition();

	oPrimForce = oSecPos - oPrimPos;
	oPrimForce.Normalize();
	oPrimForce *= fltTension;
}

void OsgLine::StepSimulation(float fltTension)
{
	if(m_lpThisBP->Enabled())
	{
		//Dont bother with this unless there is actually tension developed by the muscle.
		if(fltTension > 1e-5)
		{
			CStdArray<Attachment *> *aryAttachments = m_lpLineBase->AttachmentPoints();
			int iCount = aryAttachments->GetSize();
			Attachment *lpAttach1 = aryAttachments->at(0), *lpAttach2 = NULL;
			CStdFPoint oPrimPos, oPrimPlusPos, oSecPos, oSecMinusPos;
			CStdFPoint oPrimForce, oSecForce;
			RigidBody *lpAttach1Parent, *lpAttach2Parent;

			if(m_lpThisBP->GetSimulator()->Time() >= 0.99)
				iCount = iCount;

			//Go through each set of muscle attachments and add the tension force pointing towards the other
			//attachment point at each connector.
			for(int iIndex=1; iIndex<iCount; iIndex++)
			{
				lpAttach2 = aryAttachments->at(iIndex);

				lpAttach1Parent = lpAttach1->Parent();
				lpAttach2Parent = lpAttach2->Parent();

				CalculateForceVector(lpAttach1, lpAttach2, fltTension, oPrimPos, oPrimPlusPos, oPrimForce);
				CalculateForceVector(lpAttach2, lpAttach1, fltTension, oSecPos, oSecMinusPos, oSecForce);

				lpAttach1Parent->AddForce(oPrimPos.x, oPrimPos.y, oPrimPos.z, oPrimForce.x, oPrimForce.y, oPrimForce.z, TRUE); 
				lpAttach2Parent->AddForce(oSecPos.x, oSecPos.y, oSecPos.z, oSecForce.x, oSecForce.y, oSecForce.z, TRUE); 

				lpAttach1 = lpAttach2;
			}
		}
	}

	DrawLine();
}

void OsgLine::ResetSimulation()
{
	//We do nothing in the reset simulation because we need the attachment points to be reset before we can do anything.
}

void OsgLine::AfterResetSimulation()
{
	DrawLine();
}


	}			// Visualization
}				//OsgAnimatSim