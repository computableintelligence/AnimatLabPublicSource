#include "StdAfx.h"
#include <stdarg.h>
#include "OsgMovableItem.h"
#include "OsgBody.h"
#include "OsgRigidBody.h"
#include "OsgJoint.h"
#include "OsgOrganism.h"
#include "OsgStructure.h"
#include "OsgUserData.h"
#include "OsgUserDataVisitor.h"

#include "OsgMouseSpring.h"
#include "OsgLight.h"
#include "OsgCameraManipulator.h"
#include "OsgDragger.h"
#include "OsgSimulator.h"

namespace OsgAnimatSim
{
	namespace Environment
	{

#pragma region CreateGeometry_Code

OsgMatrixUtil *g_lpUtil = NULL;

void ANIMAT_OSG_PORT SetMatrixUtil(OsgMatrixUtil *lpUtil)
{
    g_lpUtil = lpUtil;
}

osg::Matrix ANIMAT_OSG_PORT SetupMatrix(CStdFPoint &localPos, CStdFPoint &localRot)
{
    if(g_lpUtil)
        return g_lpUtil->SetupMatrix(localPos, localRot);
    else
    {
        osg::Matrix m;
        THROW_ERROR(Osg_Err_lMatrixUtilNotDefined, Osg_Err_strMatrixUtilNotDefined);
        return m;
    }
}

osg::Matrix ANIMAT_OSG_PORT SetupMatrix(CStdFPoint &localPos, osg::Quat qRot)
{
	osg::Matrix osgLocalMatrix;
	osgLocalMatrix.makeIdentity();
	
	//convert cstdpoint to osg::Vec3
	osg::Vec3 vPos(localPos.x, localPos.y, localPos.z);
	
	//build the matrix
	osgLocalMatrix.makeRotate(qRot);
	osgLocalMatrix.setTrans(vPos);

	return osgLocalMatrix;
}

CStdFPoint ANIMAT_OSG_PORT EulerRotationFromMatrix(osg::Matrix osgMT)
{
    if(g_lpUtil)
        return g_lpUtil->EulerRotationFromMatrix(osgMT);
    else
    {
        CStdFPoint p;
        THROW_ERROR(Osg_Err_lMatrixUtilNotDefined, Osg_Err_strMatrixUtilNotDefined);
        return p;
    }
}

osg::Matrix ANIMAT_OSG_PORT LoadMatrix(CStdXml &oXml, std::string strElementName)
{
    std::string strMatrix = oXml.GetChildString(strElementName);
	
	CStdArray<std::string> aryElements;
	int iCount = Std_Split(strMatrix, ",", aryElements);

	if(iCount != 16)
		THROW_PARAM_ERROR(Al_Err_lMatrixElementCountInvalid, Al_Err_strMatrixElementCountInvalid, "Matrix count", iCount);
	
	float aryMT[4][4];
	int iIndex=0;
	for(int iX=0; iX<4; iX++)
		for(int iY=0; iY<4; iY++, iIndex++)
			aryMT[iX][iY] = atof(aryElements[iIndex].c_str());
		
	osg::Matrix osgMT(aryMT[0][0], aryMT[0][1], aryMT[0][2], aryMT[0][3], 
					  aryMT[1][0], aryMT[1][1], aryMT[1][2], aryMT[1][3], 
					  aryMT[2][0], aryMT[2][1], aryMT[2][2], aryMT[2][3], 
					  aryMT[3][0], aryMT[3][1], aryMT[3][2], aryMT[3][3]);

    return osgMT;
}

std::string ANIMAT_OSG_PORT SaveMatrixString(osg::Matrix osgMT)
{
    std::string strMatrix = "";
	for(int iX=0; iX<4; iX++)
    {
		for(int iY=0; iY<4; iY++)
        {
            strMatrix += STR(osgMT(iX, iY));
            if(iY < 3) strMatrix += ",";
        }
        if(iX < 3) strMatrix += ",";
    }

    return strMatrix;
}

void ANIMAT_OSG_PORT SaveMatrix(CStdXml &oXml, std::string strElementName, osg::Matrix osgMT)
{
    std::string strMatrix = SaveMatrixString(osgMT);
    oXml.AddChildElement(strElementName, strMatrix);
}

void ANIMAT_OSG_PORT ApplyVertexTransform(osg::Node *node, osg::Matrix omat)
{
	if (!node)
		return;

	//// Remember the parent (TODO: beware possible case of multiple parents)
	////  We must remove the node from the parent temporarily in order for the
	////  optimization below to work, but we don't want to de-ref the node, so
	////  order is important.
	//osg::Node *parent = node->getParent(0);

	//// Put it under a temporary parent instead
	//osg::Node *temp = new vtGroup;

	//osg::Matrix omat;
	//ConvertMatrix4(&mat, &omat);

	osg::MatrixTransform *transform = new osg::MatrixTransform;
	transform->setMatrix(omat);
	// Tell OSG that it can be optimized
	transform->setDataVariance(osg::Object::STATIC);

    //temp->addChild(transform);
	transform->addChild(node);

	//// carefully remove from the true parent
	//parent->removeChild(node);

	// Now do some OSG voodoo, which should spread the transform downward
	//  through the loaded model, and delete the transform.
	osgUtil::Optimizer optimizer;
	optimizer.optimize(transform, osgUtil::Optimizer::FLATTEN_STATIC_TRANSFORMS);

	//// now carefully add back again, whatever remains after the optimization,
	////  to the true parent. Hopefully, our corrective transform has been applied.
	//// Our original node may have been optimized away too.
	//parent->addChild(temp->getChild(0));
}

osg::Geometry ANIMAT_OSG_PORT *CreateBoxGeometry(float xsize, float ysize, float zsize, float fltXSegWidth, float fltYSegWidth, float fltZSegWidth)
{
    //if(! hor || ! vert || ! depth)
    //{
    //    SWARNING << "makeBox: illegal parameters hor=" << hor << ", vert="
    //             << vert << ", depth=" << depth << std::endl;
    //    return NULL;
    //}

    osg::Vec3 sizeMin(-xsize/2.0f,  -ysize/2.0f,  -zsize/2.0f);
    osg::Vec3 sizeMax(xsize/2.0f,  ysize/2.0f,  zsize/2.0f);
	osg::Vec3 steps( (int) ((xsize/fltXSegWidth)+0.5f), (int) ((ysize/fltYSegWidth)+0.5f), (int) ((zsize/fltZSegWidth)+0.5f) );
	osg::Vec3 SegWidths(fltXSegWidth, fltYSegWidth, fltZSegWidth);	

	osg::Geometry* boxGeom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> norms = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> texts = new osg::Vec2Array(); 
	int iLen = 0;
	int iPos = 0;

#pragma region X-constant-loops

	//Side 1
	float fltY1 = sizeMin.y();
	float fltY2 = fltY1 + SegWidths.y();
	float fltZ1 = sizeMin.z();
	float fltZ2 = fltZ1 + SegWidths.z();
	for(int iy=0; iy<(int) steps.y(); iy++)
	{
		for(int iz=0; iz<(int) steps.z(); iz++)
		{
			verts->push_back(osg::Vec3(sizeMin.x(), fltY1, fltZ1)); // 0
			verts->push_back(osg::Vec3(sizeMin.x(), fltY1, fltZ2)); // 3
			verts->push_back(osg::Vec3(sizeMin.x(), fltY2, fltZ2)); // 5
			verts->push_back(osg::Vec3(sizeMin.x(), fltY2, fltZ1)); // 1

			norms->push_back(osg::Vec3(-1,  0,  0));
			norms->push_back(osg::Vec3(-1,  0,  0));
			norms->push_back(osg::Vec3(-1,  0,  0));
			norms->push_back(osg::Vec3(-1,  0,  0));

			texts->push_back(osg::Vec2( 0.f, 0.f)); // 0
			texts->push_back(osg::Vec2( 1.f, 0.f)); // 1
			texts->push_back(osg::Vec2( 1.f, 1.f)); // 4
			texts->push_back(osg::Vec2( 0.f, 1.f)); // 2
			
			fltZ1+=SegWidths.z(); fltZ2+=SegWidths.z();
		}

		fltY1+=SegWidths.y(); fltY2+=SegWidths.y();
		fltZ1 = sizeMin.z(); fltZ2 = fltZ1 + SegWidths.z();
	}

	//Side 2 opposite
	fltY1 = sizeMin.y();
	fltY2 = fltY1 + SegWidths.y();
	fltZ1 = sizeMin.z();
	fltZ2 = fltZ1 + SegWidths.z();
	for(int iy=0; iy<(int) steps.y(); iy++)
	{
		for(int iz=0; iz<(int) steps.z(); iz++)
		{
			verts->push_back(osg::Vec3(sizeMax.x(), fltY1, fltZ1)); // 0
			verts->push_back(osg::Vec3(sizeMax.x(), fltY2, fltZ1)); // 3
			verts->push_back(osg::Vec3(sizeMax.x(), fltY2, fltZ2)); // 5
			verts->push_back(osg::Vec3(sizeMax.x(), fltY1, fltZ2)); // 1

			norms->push_back(osg::Vec3(1,  0,  0));
			norms->push_back(osg::Vec3(1,  0,  0));
			norms->push_back(osg::Vec3(1,  0,  0));
			norms->push_back(osg::Vec3(1,  0,  0));

			texts->push_back(osg::Vec2( 0.f, 0.f)); // 3
			texts->push_back(osg::Vec2( 1.f, 0.f)); // 6
			texts->push_back(osg::Vec2( 1.f, 1.f)); // 7
			texts->push_back(osg::Vec2( 0.f, 1.f)); // 5
			
			fltZ1+=SegWidths.z(); fltZ2+=SegWidths.z();
		}

		fltY1+=SegWidths.y(); fltY2+=SegWidths.y();
		fltZ1 = sizeMin.z(); fltZ2 = fltZ1 + SegWidths.z();
	}

#pragma endregion


#pragma region Y-constant-loops

	//Side 1
	float fltX1 = sizeMin.x();
	float fltX2 = fltX1 + SegWidths.x();
	fltZ1 = sizeMin.z();
	fltZ2 = fltZ1 + SegWidths.z();
	for(int ix=0; ix<(int) steps.x(); ix++)
	{
		for(int iz=0; iz<(int) steps.z(); iz++)
		{
			verts->push_back(osg::Vec3(fltX1, sizeMin.y(), fltZ1)); // 0
			verts->push_back(osg::Vec3(fltX2, sizeMin.y(), fltZ1)); // 3
			verts->push_back(osg::Vec3(fltX2, sizeMin.y(), fltZ2)); // 5
			verts->push_back(osg::Vec3(fltX1, sizeMin.y(), fltZ2)); // 1

			norms->push_back(osg::Vec3( 0, -1,  0));
			norms->push_back(osg::Vec3( 0, -1,  0));
			norms->push_back(osg::Vec3( 0, -1,  0));
			norms->push_back(osg::Vec3( 0, -1,  0));

			texts->push_back(osg::Vec2( 0.f, 0.f)); // 0
			texts->push_back(osg::Vec2( 1.f, 0.f)); // 3
			texts->push_back(osg::Vec2( 1.f, 1.f)); // 5
			texts->push_back(osg::Vec2( 0.f, 1.f)); // 1
			
			fltZ1+=SegWidths.z(); fltZ2+=SegWidths.z();
		}

		fltX1+=SegWidths.x(); fltX2+=SegWidths.x();
		fltZ1 = sizeMin.z(); fltZ2 = fltZ1 + SegWidths.z();
	}

	//Side 2 opposite
	fltX1 = sizeMin.x();
	fltX2 = fltX1 + SegWidths.x();
	fltZ1 = sizeMin.z();
	fltZ2 = fltZ1 + SegWidths.z();
	for(int ix=0; ix<(int) steps.x(); ix++)
	{
		for(int iz=0; iz<(int) steps.z(); iz++)
		{
			verts->push_back(osg::Vec3(fltX2, sizeMax.y(), fltZ1)); // 0
			verts->push_back(osg::Vec3(fltX1, sizeMax.y(), fltZ1)); // 3
			verts->push_back(osg::Vec3(fltX1, sizeMax.y(), fltZ2)); // 5
			verts->push_back(osg::Vec3(fltX2, sizeMax.y(), fltZ2)); // 1

			norms->push_back(osg::Vec3( 0, 1,  0));
			norms->push_back(osg::Vec3( 0, 1,  0));
			norms->push_back(osg::Vec3( 0, 1,  0));
			norms->push_back(osg::Vec3( 0, 1,  0));

			texts->push_back(osg::Vec2( 0.f, 0.f)); // 6
			texts->push_back(osg::Vec2( 1.f, 0.f)); // 2
			texts->push_back(osg::Vec2( 1.f, 1.f)); // 4
			texts->push_back(osg::Vec2( 0.f, 1.f)); // 7
			
			fltZ1+=SegWidths.z(); fltZ2+=SegWidths.z();
		}

		fltX1+=SegWidths.x(); fltX2+=SegWidths.x();
		fltZ1 = sizeMin.z(); fltZ2 = fltZ1 + SegWidths.z();
	}

#pragma endregion


#pragma region Z-constant-loops

	//Side 1
	fltX1 = sizeMin.x();
	fltX2 = fltX1 + SegWidths.x();
	fltY1 = sizeMin.y();
	fltY2 = fltY1 + SegWidths.y();
	for(int ix=0; ix<(int) steps.x(); ix++)
	{
		for(int iy=0; iy<(int) steps.y(); iy++)
		{
			verts->push_back(osg::Vec3(fltX1, fltY1, sizeMax.z())); // 0
			verts->push_back(osg::Vec3(fltX2, fltY1, sizeMax.z())); // 3
			verts->push_back(osg::Vec3(fltX2, fltY2, sizeMax.z())); // 5
			verts->push_back(osg::Vec3(fltX1, fltY2, sizeMax.z())); // 1

			norms->push_back(osg::Vec3( 0,  0,  1));
			norms->push_back(osg::Vec3( 0,  0,  1));
			norms->push_back(osg::Vec3( 0,  0,  1));
			norms->push_back(osg::Vec3( 0,  0,  1));

			texts->push_back(osg::Vec2( 0.f, 0.f)); // 1
			texts->push_back(osg::Vec2( 1.f, 0.f)); // 5
			texts->push_back(osg::Vec2( 1.f, 1.f)); // 7
			texts->push_back(osg::Vec2( 0.f, 1.f)); // 4
			
			fltY1+=SegWidths.y(); fltY2+=SegWidths.y();
		}

		fltX1+=SegWidths.x(); fltX2+=SegWidths.x();
		fltY1 = sizeMin.y(); fltY2 = fltY1 + SegWidths.y();
	}

	//Side 2 opposite
	fltX1 = sizeMin.x();
	fltX2 = fltX1 + SegWidths.x();
	fltY1 = sizeMin.y();
	fltY2 = fltY1 + SegWidths.y();
	for(int ix=0; ix<(int) steps.x(); ix++)
	{
		for(int iy=0; iy<(int) steps.y(); iy++)
		{
			verts->push_back(osg::Vec3(fltX2, fltY1, sizeMin.z())); // 0
			verts->push_back(osg::Vec3(fltX1, fltY1, sizeMin.z())); // 3
			verts->push_back(osg::Vec3(fltX1, fltY2, sizeMin.z())); // 5
			verts->push_back(osg::Vec3(fltX2, fltY2, sizeMin.z())); // 1

			norms->push_back(osg::Vec3( 0,  0, -1));
			norms->push_back(osg::Vec3( 0,  0, -1));
			norms->push_back(osg::Vec3( 0,  0, -1));
			norms->push_back(osg::Vec3( 0,  0, -1));

			texts->push_back(osg::Vec2( 0.f, 0.f)); // 3
			texts->push_back(osg::Vec2( 1.f, 0.f)); // 0
			texts->push_back(osg::Vec2( 1.f, 1.f)); // 2
			texts->push_back(osg::Vec2( 0.f, 1.f)); // 6
			
			fltY1+=SegWidths.y(); fltY2+=SegWidths.y();
		}

		fltX1+=SegWidths.x(); fltX2+=SegWidths.x();
		fltY1 = sizeMin.y(); fltY2 = fltY1 + SegWidths.y();
	}

#pragma endregion

    // create the geometry
     boxGeom->setVertexArray(verts.get());
     boxGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, verts->size()));

	 boxGeom->setNormalArray(norms.get());
     boxGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

     boxGeom->setTexCoordArray( 0, texts.get() );

     osg::Vec4Array* colors = new osg::Vec4Array;
     colors->push_back(osg::Vec4(1,1,1,1));
     boxGeom->setColorArray(colors);
     boxGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    return boxGeom;
}

/*! Create the Geometry Core used by OSG::makeConicalFrustum.

    \param[in] height Height of the conical frustum.
    \param[in] topradius Radius at the top of the conical frustum.
    \param[in] botradius Radius at the bottom of the conical frustum.
    \param[in] sides Number of sides the base is subdivided into.
    \param[in] doSide If true, side faces are created.
    \param[in] doTop If true, top cap faces are created.
    \param[in] doBttom If true, bottom cap faces are created.
    \return GeometryTransitPtr to a newly created Geometry core.

    \ingroup GrpSystemDrawablesGeometrySimpleGeometry
 */
osg::Geometry ANIMAT_OSG_PORT *CreateConeGeometry(float height,
                                  float topradius,
                                  float botradius,
                                  int sides,
                                  bool   doSide,
                                  bool   doTop,
                                  bool   doBottom)
{
    if(height <= 0 || topradius < 0 || botradius < 0 || sides < 3)
    {
        //SWARNING << "makeConicalFrustum: illegal parameters height=" << height
        //         << ", topradius=" << topradius
        //         << ", botradius=" << botradius
        //         << ", sides=" << sides
        //         << std::endl;
        return NULL;
    }

	osg::ref_ptr<osg::Vec3Array> p = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array(); 

    osg::Geometry* coneGeom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> norms = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> texts = new osg::Vec2Array(); 

    int  j;
    float delta = 2.f * osg::PI / sides;
    float beta, x, z;
    float incl = (botradius - topradius) / height;
    float nlen = 1.f / sqrt(1 + incl * incl);
	int iLen = 0;
	int iPos = 0;

    if(doSide)
    {
        int baseindex = p->size();

        for(j = 0; j <= sides; j++)
        {
            beta = j * delta;
            x    =  sin(beta);
            z    = -cos(beta);

            p->push_back(osg::Vec3(x * topradius, height/2, z * topradius));
            n->push_back(osg::Vec3(x/nlen, incl/nlen, z/nlen));
            t->push_back(osg::Vec2(1.f - j / float(sides), 1));
        }

        for(j = 0; j <= sides; j++)
        {
            beta = j * delta;
            x    =  sin(beta);
            z    = -cos(beta);

            p->push_back(osg::Vec3(x * botradius, -height/2, z * botradius));
            n->push_back(osg::Vec3(x/nlen, incl/nlen, z/nlen));
            t->push_back(osg::Vec2(1.f - j / float(sides), 0));
        }

        for(j = 0; j <= sides; j++)
        {
            verts->push_back(p->at(baseindex + sides + 1 + j));
            verts->push_back(p->at(baseindex + j));

            norms->push_back(n->at(baseindex + sides + 1 + j));
            norms->push_back(n->at(baseindex + j));

            texts->push_back(t->at(baseindex + sides + 1 + j));
            texts->push_back(t->at(baseindex + j));
		}

		iLen = 2 * (sides + 1);
        coneGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, iPos, iLen));
		iPos+=iLen;
	}

    if(doTop && topradius > 0)
    {
        int baseindex = p->getNumElements();

        // need to duplicate the points fornow, as we don't have multi-index geo yet

        for(j = sides - 1; j >= 0; j--)
        {
            beta = j * delta;
            x    =  topradius * sin(beta);
            z    = -topradius * cos(beta);

            p->push_back(osg::Vec3(x, height/2, z));
            n->push_back(osg::Vec3(0, 1, 0));
            t->push_back(osg::Vec2(x / topradius / 2 + .5f, -z / topradius / 2 + .5f));
        }

        for(j = 0; j < sides; j++)
        {
            verts->push_back(p->at(baseindex + j));
            norms->push_back(n->at(baseindex + j));
            texts->push_back(t->at(baseindex + j));
        }

		iLen = sides;
        coneGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, iPos, iLen));
		iPos+=iLen;
	}

    if(doBottom && botradius > 0 )
    {
        int baseindex = p->getNumElements();

        // need to duplicate the points fornow, as we don't have multi-index geo yet

        for(j = sides - 1; j >= 0; j--)
        {
            beta = j * delta;
            x    =  botradius * sin(beta);
            z    = -botradius * cos(beta);

            p->push_back(osg::Vec3(x, -height/2, z));
            n->push_back(osg::Vec3(0, -1, 0));
            t->push_back(osg::Vec2(x / botradius / 2 + .5f, z / botradius / 2 + .5f));
        }

        for(j = 0; j < sides; j++)
        {
			verts->push_back(p->at(baseindex + sides - 1 - j));
            norms->push_back(n->at(baseindex + sides - 1 - j));
            texts->push_back(t->at(baseindex + sides - 1 - j));
        }

		iLen = sides;
        coneGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON, iPos, iLen));
		iPos+=iLen;
    }

    // create the geometry
     coneGeom->setVertexArray(verts.get());

	 coneGeom->setNormalArray(norms.get());
     coneGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

     coneGeom->setTexCoordArray( 0, texts.get() );

     osg::Vec4Array* colors = new osg::Vec4Array;
     colors->push_back(osg::Vec4(1,1,1,1));
     coneGeom->setColorArray(colors);
     coneGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    return coneGeom;
}

///*! Create the Geometry Core used by OSG::makeLatLongSphere.
//
//    \param[in] latres Number of subdivisions along latitudes.
//    \param[in] longres Number of subdivisions along longitudes.
//    \param[in] radius Radius of sphere.
//    \return GeometryTransitPtr to a newly created Geometry core.
//
//    \sa OSG::makeLatLongSphere
//
//    \ingroup GrpSystemDrawablesGeometrySimpleGeometry
// */
osg::Geometry ANIMAT_OSG_PORT *CreateSphereGeometry(int latres, 
                          int longres,
                          float radius)
{
    if(radius <= 0 || latres < 4 || longres < 4)
    {
        //SWARNING << "makeLatLongSphere: illegal parameters "
        //         << "latres=" << latres
        //         << ", longres=" << longres
        //         << ", radius=" << radius
        //         << std::endl;
        return NULL;
    }

	osg::ref_ptr<osg::Vec3Array> p = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array(); 

    int a, b;
    float theta, phi;
    float cosTheta, sinTheta;
    float latDelta, longDelta;

    // calc the vertices

    latDelta  =       osg::PI / latres;
    longDelta = 2.f * osg::PI / longres;

    for(a = 0, theta = -osg::PI / 2; a <= latres; a++, theta += latDelta)
    {
        cosTheta = cos(theta);
        sinTheta = sin(theta);

        for(b = 0, phi = -osg::PI; b <= longres; b++, phi += longDelta)
        {
            float cosPhi, sinPhi;

            cosPhi = cos(phi);
            sinPhi = sin(phi);

            n->push_back(osg::Vec3(cosTheta * sinPhi,
                               sinTheta,
                               cosTheta * cosPhi));
        
            p->push_back(osg::Vec3( cosTheta * sinPhi * radius,
                               sinTheta          * radius,
                               cosTheta * cosPhi * radius));

            t->push_back(osg::Vec2(b / float(longres),
                                a / float(latres)));
        }
    }


    osg::Geometry* sphereGeom = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> norms = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> texts = new osg::Vec2Array(); 

    // create the faces
	int iLen = (latres + 1) * 2;
	int iPos = 0;
    for(a = 0; a < longres; a++)
    {
        for(b = 0; b <= latres; b++)
        {
			verts->push_back(p->at(b * (longres+1) + a));
			verts->push_back(p->at(b * (longres+1) + a + 1));

			norms->push_back(n->at(b * (longres+1) + a));
			norms->push_back(n->at(b * (longres+1) + a + 1));

			texts->push_back(t->at(b * (longres+1) + a));
			texts->push_back(t->at(b * (longres+1) + a + 1));
        }

        sphereGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, iPos, iLen));
		iPos+=iLen;
	}

    // create the geometry
     sphereGeom->setVertexArray(verts.get());

	 sphereGeom->setNormalArray(norms.get());
     sphereGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

     sphereGeom->setTexCoordArray( 0, texts.get() );

     osg::Vec4Array* colors = new osg::Vec4Array;
     colors->push_back(osg::Vec4(1,1,1,1));
     sphereGeom->setColorArray(colors);
     sphereGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    return sphereGeom;
}


/*! Create the Geometry Core used by OSG::makeLatLongSphere.

    \param[in] latres Number of subdivisions along latitudes.
    \param[in] longres Number of subdivisions along longitudes.
    \param[in] radius Radius of sphere.
    \return GeometryTransitPtr to a newly created Geometry core.

    \sa OSG::makeLatLongSphere

    \ingroup GrpSystemDrawablesGeometrySimpleGeometry
 */
osg::Geometry ANIMAT_OSG_PORT *CreateEllipsoidGeometry(int latres, 
                             int longres,
                             float rSemiMajorAxis,
                             float rSemiMinorAxis)
{
    if(rSemiMajorAxis <= 0 || rSemiMinorAxis <= 0 || latres < 4 || longres < 4)
    {
        //SWARNING << "makeLatLongSphere: illegal parameters "
        //         << "latres=" << latres
        //         << ", longres=" << longres
        //         << ", rSemiMajorAxis=" << rSemiMajorAxis
        //         << ", rSemiMinorAxis=" << rSemiMinorAxis
        //         << std::endl;
        return NULL;
    }

	osg::ref_ptr<osg::Vec3Array> p = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> t = new osg::Vec2Array(); 

    int a, b;
    float theta, phi;
    float cosTheta, sinTheta;
    float latDelta, longDelta;

    // calc the vertices

    latDelta  =       osg::PI / latres;
    longDelta = 2.f * osg::PI / longres;

    float rSemiMajorAxisSquare = rSemiMajorAxis * rSemiMajorAxis;

    float e2 = (rSemiMajorAxisSquare - 
                rSemiMinorAxis * rSemiMinorAxis) / (rSemiMajorAxisSquare);

    for(a = 0, theta = -osg::PI / 2; a <= latres; a++, theta += latDelta)
    {
        cosTheta = cos(theta);
        sinTheta = sin(theta);

        float v = rSemiMajorAxis / sqrt(1 - (e2 * sinTheta * sinTheta));

        for(b = 0, phi = -osg::PI; b <= longres; b++, phi += longDelta)
        {
            float cosPhi, sinPhi;

            cosPhi = cos(phi);
            sinPhi = sin(phi);


            n->push_back(osg::Vec3(cosTheta * sinPhi,
                               sinTheta,
                               cosTheta * cosPhi));
        
            p->push_back(osg::Vec3(cosTheta * sinPhi * v,
                               sinTheta          * ((1 - e2) * v),
                               cosTheta * cosPhi * v));

            t->push_back(osg::Vec2(b / float(longres),
                                a / float(latres)));

        }
    }

    osg::Geometry* sphereGeom = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> norms = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> texts = new osg::Vec2Array(); 

    // create the faces
	int iLen = (latres + 1) * 2;
	int iPos = 0;

	for(a = 0; a < longres; a++)
    {
        for(b = 0; b <= latres; b++)
        {
            verts->push_back(p->at(b * (longres+1) + a));
            verts->push_back(p->at(b * (longres+1) + a + 1));

		    norms->push_back(n->at(b * (longres+1) + a));
            norms->push_back(n->at(b * (longres+1) + a + 1));

		    texts->push_back(t->at(b * (longres+1) + a));
            texts->push_back(t->at(b * (longres+1) + a + 1));
		}

	    sphereGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, iPos, iLen));
		iPos+=iLen;
	}


    // create the geometry
     sphereGeom->setVertexArray(verts.get());

	 sphereGeom->setNormalArray(norms.get());
     sphereGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

     sphereGeom->setTexCoordArray( 0, texts.get() );

     osg::Vec4Array* colors = new osg::Vec4Array;
     colors->push_back(osg::Vec4(1,1,1,1));
     sphereGeom->setColorArray(colors);
     sphereGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    return sphereGeom;
}

osg::Geometry ANIMAT_OSG_PORT *CreatePlaneGeometry(float fltCornerX, float fltCornerY, float fltXSize, float fltYSize, float fltXGrid, float fltYGrid, bool bBothSides)
{
	float A = fltCornerX;
	float B = fltCornerY;
	float C = fltCornerX + fltXSize;
	float D = fltCornerY + fltYSize;

	osg::Geometry *geom = new osg::Geometry;

	// Create an array of four vertices.
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray( v.get() );
	v->push_back( osg::Vec3( A, B, 0 ) );
	v->push_back( osg::Vec3( C, B, 0 ) );
	v->push_back( osg::Vec3( C, D, 0 ) );
	v->push_back( osg::Vec3( A, D, 0 ) );	

	if(bBothSides)
	{
		v->push_back( osg::Vec3( C, B, 0 ) );
		v->push_back( osg::Vec3( A, B, 0 ) );
		v->push_back( osg::Vec3( A, D, 0 ) );
		v->push_back( osg::Vec3( C, D, 0 ) );	
	}

	// Create a Vec2Array of texture coordinates for texture unit 0
	// and attach it to the geom.
	osg::ref_ptr<osg::Vec2Array> tc = new osg::Vec2Array;
	geom->setTexCoordArray( 0, tc.get() );
	tc->push_back( osg::Vec2( 0.f, 0.f ) );
	tc->push_back( osg::Vec2( fltXGrid, 0.f ) );
	tc->push_back( osg::Vec2( fltXGrid, fltYGrid ) );
	tc->push_back( osg::Vec2( 0.f, fltYGrid ) );

	if(bBothSides)
	{
		tc->push_back( osg::Vec2( 0.f, 0.f ) );
		tc->push_back( osg::Vec2( fltXGrid, 0.f ) );
		tc->push_back( osg::Vec2( fltXGrid, fltYGrid ) );
		tc->push_back( osg::Vec2( 0.f, fltYGrid ) );
	}

	// Create an array for the single normal.
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	n->push_back( osg::Vec3( 0.f, 0.f, -1.f ) );
	geom->setNormalArray( n.get() );
	geom->setNormalBinding( osg::Geometry::BIND_OVERALL );

	// Draw a four-vertex quad from the stored data.
	geom->addPrimitiveSet(new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, 4 ) );

	return geom;
}

bool ANIMAT_OSG_PORT OsgMatricesEqual(osg::Matrix v1, osg::Matrix v2)
{
	for(int iRow=0; iRow<4; iRow++)
		for(int iCol=0; iCol<4; iCol++)
			if(fabs(v1(iRow,iCol)-v2(iRow,iCol)) > 1e-5)
				return false;

	return true;
}

void ANIMAT_OSG_PORT AddNodeTexture(osg::Node *osgNode, std::string strTexture, osg::StateAttribute::GLMode eTextureMode)
{
	if(osgNode)
	{
		if(!Std_IsBlank(strTexture))
		{
			osg::ref_ptr<osg::Image> image = osgDB::readImageFile(strTexture);
			if(!image)
				THROW_PARAM_ERROR(Osg_Err_lTextureLoad, Osg_Err_strTextureLoad, "Image File", strTexture);

			osg::StateSet* state = osgNode->getOrCreateStateSet();
			osg::ref_ptr<osg::Texture2D> osgTexture = new osg::Texture2D(image.get());
		    osgTexture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.

			osgTexture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
			osgTexture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
			
			state->setTextureAttributeAndModes(0, osgTexture.get());
			state->setTextureMode(0, eTextureMode, osg::StateAttribute::ON);
			state->setMode(GL_BLEND,osg::StateAttribute::ON);
		}
	}
}

void ANIMAT_OSG_PORT SetNodeColor(osg::Node *osgNode, CStdColor &vAmbient, CStdColor &vDiffuse, CStdColor &vSpecular, float fltShininess)
{
	if(osgNode)
	{
		//create a material to use with this node
		osg::ref_ptr<osg::Material> osgMaterial = new osg::Material();		

		//create a stateset for this node
		osg::StateSet *osgStateSet = osgNode->getOrCreateStateSet();

		//set the diffuse property of this node to the color of this body	
		osgMaterial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(vAmbient[0], vAmbient[1], vAmbient[2], 1));
		osgMaterial->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(vDiffuse[0], vDiffuse[1], vDiffuse[2], vDiffuse[3]));
		osgMaterial->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(vSpecular[0], vSpecular[1], vSpecular[2], 1));
		osgMaterial->setShininess(osg::Material::FRONT_AND_BACK, fltShininess);
		osgStateSet->setMode(GL_BLEND, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON); 

		//apply the material
		osgStateSet->setAttribute(osgMaterial.get(), osg::StateAttribute::ON);
	}
}

osg::MatrixTransform ANIMAT_OSG_PORT *CreateLinearAxis(float fltGripScale, CStdFPoint vRotAxis)
{
	CStdFPoint vPos(0, 0, 0), vRot(0, 0, 0); 
	float fltCylinderRadius = fltGripScale * 0.01f;
	float fltTipRadius = fltGripScale * 0.03f;
	float fltCylinderHeight = fltGripScale * 2.5;
	float fltTipHeight = fltGripScale * 0.05f;

	//Create the X-axis transform.
	osg::MatrixTransform *osgAxis = new osg::MatrixTransform();
	vPos.Set(0, 0, 0); vRot = (vRotAxis * -(osg::PI/2)); 
	osgAxis->setMatrix(SetupMatrix(vPos, vRot));

	//Create the cylinder for the hinge
	osg::ref_ptr<osg::Geometry> osgCylinderGeom = CreateConeGeometry(fltCylinderHeight, fltCylinderRadius, fltCylinderRadius, 30, true, true, true);
	osg::ref_ptr<osg::Geode> osgAxisCylinder = new osg::Geode;
	osgAxisCylinder->addDrawable(osgCylinderGeom.get());
	osgAxis->addChild(osgAxisCylinder.get());

	osg::ref_ptr<osg::MatrixTransform> osgAxisConeMT = new osg::MatrixTransform();
	vPos.Set(0, (fltCylinderHeight/2), 0); vRot.Set(0, 0, 0); 
	osgAxisConeMT->setMatrix(SetupMatrix(vPos, vRot));

	osg::ref_ptr<osg::Geometry> osgAxisTipGeom = CreateConeGeometry(fltTipHeight, 0, fltTipRadius, 30, true, true, true);
	osg::ref_ptr<osg::Geode> osgAxisTip = new osg::Geode;
	osgAxisTip->addDrawable(osgAxisTipGeom.get());
	osgAxisConeMT->addChild(osgAxisTip.get());
	osgAxis->addChild(osgAxisConeMT.get());

	return osgAxis;
}

osg::Vec3Array ANIMAT_OSG_PORT *CreateCircleVerts( int plane, int segments, float radius )
{
    const double angle( osg::PI * 2. / (double) segments );
    osg::Vec3Array* v = new osg::Vec3Array;
    int idx, count;
    double x(0.), y(0.), z(0.);
    double height;
    double original_radius = radius;

    for(count = 0; count <= segments/4; count++)
    {
        height = original_radius*sin(count*angle);
        radius = cos(count*angle)*radius;


    switch(plane)
    {
        case 0: // X
            x = height;
            break;
        case 1: //Y
            y = height;
            break;
        case 2: //Z
            z = height;
            break;
    }

    for( idx=0; idx<segments; idx++)
    {
        double cosAngle = cos(idx*angle);
        double sinAngle = sin(idx*angle);
        switch (plane) {
            case 0: // X
                y = radius*cosAngle;
                z = radius*sinAngle;
                break;
            case 1: // Y
                x = radius*cosAngle;
                z = radius*sinAngle;
                break;
            case 2: // Z
                x = radius*cosAngle;
                y = radius*sinAngle;
                break;
        }
        v->push_back( osg::Vec3( x, y, z ) );
    }
    }
    return v;
}

osg::Geode ANIMAT_OSG_PORT *CreateCircle( int plane, int segments, float radius, float width )
{
    osg::Geode* geode = new osg::Geode;
    osg::LineWidth* lw = new osg::LineWidth( width );
    geode->getOrCreateStateSet()->setAttributeAndModes( lw, osg::StateAttribute::ON );

    osg::Geometry* geom = new osg::Geometry;
    osg::Vec3Array* v = CreateCircleVerts( plane, segments, radius );
    geom->setVertexArray( v );

    osg::Vec4Array* c = new osg::Vec4Array;
    c->push_back( osg::Vec4( 1., 1., 1., 1. ) );
    geom->setColorArray( c );
    geom->setColorBinding( osg::Geometry::BIND_OVERALL );
    geom->addPrimitiveSet( new osg::DrawArrays( GL_LINE_LOOP, 0, segments ) );

    geode->addDrawable( geom );

    return geode;
}

//To make a half-torus we need to to chance the following line
//    ringDelta = 2.f * osg::PI / rings;
// to be
//    ringDelta = osg::PI / rings;
// and to make the other half of the torus we need to change the followign lines
//        cosTheta = cos(theta);
//        sinTheta = sin(theta);
// to be
//        -cosTheta = cos(theta);
//        -sinTheta = sin(theta);
osg::Geometry ANIMAT_OSG_PORT *CreateTorusGeometry(float innerRadius, 
                                float outerRadius, 
                                int sides,
                                int rings)
{
    if(innerRadius <= 0 || outerRadius <= 0 || sides < 3 || rings < 3)
    {
        //SWARNING << "makeTorus: illegal parameters innerRadius=" << innerRadius
        //         << ", outerRadius=" << outerRadius
        //         << ", sides=" << sides
        //         << ", rings=" << rings
        //         << std::endl;
        //return GeometryTransitPtr(NULL);
		return NULL;
    }

    int a, b;
    float theta, phi;
    float cosTheta, sinTheta;
    float ringDelta, sideDelta;

    // calc the vertices

	osg::ref_ptr<osg::Vec3Array> p = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> tx = new osg::Vec2Array(); 

    ringDelta = 2.f * osg::PI / rings;
    sideDelta = 2.f * osg::PI / sides;

    for(a = 0, theta = 0.0; a <= rings; a++, theta += ringDelta)
    {
        cosTheta = cos(theta);
        sinTheta = sin(theta);

        for(b = 0, phi = 0; b <= sides; b++, phi += sideDelta)
        {
            float cosPhi, sinPhi, dist;

            cosPhi = cos(phi);
            sinPhi = sin(phi);
            dist   = outerRadius + innerRadius * cosPhi;

            n->push_back(osg::Vec3(cosTheta * cosPhi,
                              -sinTheta * cosPhi,
                              sinPhi));
            p->push_back(osg::Vec3(cosTheta * dist,
                              -sinTheta * dist,
                              innerRadius * sinPhi));
            tx->push_back(osg::Vec2(- a / float(rings), b / float(sides)));
        }
    }

    // create the faces
    osg::Geometry* torusGeom = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> norms = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> texts = new osg::Vec2Array(); 

	int iLen = (rings + 1) * 2;
	int iPos = 0;
    for(a = 0; a < sides; a++)
    {
        for(b = 0; b <= rings; b++)
        {
			verts->push_back(p->at(b * (sides+1) + a));
			verts->push_back(p->at(b * (sides+1) + a + 1));

			norms->push_back(n->at(b * (sides+1) + a));
			norms->push_back(n->at(b * (sides+1) + a + 1));

			texts->push_back(tx->at(b * (sides+1) + a));
			texts->push_back(tx->at(b * (sides+1) + a + 1));
        }

	    torusGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, iPos, iLen));
		iPos+=iLen;
	}


    // create the geometry
     torusGeom->setVertexArray(verts.get());

	 torusGeom->setNormalArray(norms.get());
     torusGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

     torusGeom->setTexCoordArray( 0, texts.get() );

     osg::Vec4Array* colors = new osg::Vec4Array;
     colors->push_back(osg::Vec4(1,1,1,1));
     torusGeom->setColorArray(colors);
     torusGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    return torusGeom;
}

osg::Node ANIMAT_OSG_PORT *CreateHeightField(std::string heightFile, float fltSegWidth, float fltSegLength, float fltMaxHeight, osg::HeightField **osgMap, bool bAdjustHeight) 
{
    osg::Image* heightMap = osgDB::readImageFile(heightFile);
     
	if(!heightMap)
		THROW_PARAM_ERROR(Osg_Err_lHeightFieldImageNotDefined, Osg_Err_strHeightFieldImageNotDefined, "Filename", heightFile);

    osg::HeightField* heightField = new osg::HeightField();
	*osgMap = heightField;
	heightField->allocate(heightMap->s(), heightMap->t());
    heightField->setOrigin(osg::Vec3(-(heightMap->s()*fltSegWidth) / 2, -(heightMap->t()*fltSegLength) / 2, 0));
    heightField->setXInterval(fltSegWidth);
    heightField->setYInterval(fltSegLength);
    heightField->setSkirtHeight(1.0f);

    //Loop through once to find the min/max heights.
    float fltMinTerrainHeight = 10000;
    float fltMaxTerrainHeight = -10000;
    float fltHeight = 0;
    float fltHeightAdjust = 0;
    if(bAdjustHeight)
    {
        for (int r = 0; r < heightField->getNumRows(); r++) 
	    {
		    for (int c = 0; c < heightField->getNumColumns(); c++) 
            {
			    fltHeight = (((*heightMap->data(c, r)) / 255.0f) * fltMaxHeight);
                if(fltHeight < fltMinTerrainHeight)
                    fltMinTerrainHeight = fltHeight;
                if(fltHeight > fltMaxTerrainHeight)
                    fltMaxTerrainHeight = fltHeight;
            }
        }

        fltHeightAdjust = (fltMaxTerrainHeight - fltMinTerrainHeight)/2.0 + fltMinTerrainHeight;
    }

    for (int r = 0; r < heightField->getNumRows(); r++) 
	{
		for (int c = 0; c < heightField->getNumColumns(); c++) 
        {
			fltHeight = ((((*heightMap->data(c, r)) / 255.0f) * fltMaxHeight) - fltHeightAdjust);
			heightField->setHeight(c, r, fltHeight);
        }
    }
     
    osg::Geode* geode = new osg::Geode();
    geode->addDrawable(new osg::ShapeDrawable(heightField));
     
    //osg::Texture2D* tex = new osg::Texture2D(osgDB::readImageFile(texFile));
    //tex->setFilter(osg::Texture2D::MIN_FILTER,osg::Texture2D::LINEAR_MIPMAP_LINEAR);
    //tex->setFilter(osg::Texture2D::MAG_FILTER,osg::Texture2D::LINEAR);
    //tex->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    //tex->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    //geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);
     
    return geode;
}


/*
osg::Geometry *CreateHeightFieldGeometry(std::string strFilename,  float fltLeftCorner, float fltUpperCorner, 
										 float fltSegmentWidth, float fltSegmentLength, 
										 float fltMinElevation, float fltMaxElevation)
{
	std::string strHeightMap = "C:\\Projects\\AnimatLabSDK\\Experiments\\MeshTest2\\TerrainTest\\TerrainTest_HeightMap.jpg"
	std::string strNormalsMap = "C:\\Projects\\AnimatLabSDK\\Experiments\\MeshTest2\\TerrainTest\\TerrainTest_NormalsMap.jpg"
	std::string strTextureMap = "C:\\Projects\\AnimatLabSDK\\Experiments\\MeshTest2\\TerrainTest\\TerrainTest_TextureMap.jpg"

	//load the images.
	osg::Image *imgHeight = osgDB::readImageFile(strHeightMap.c_str());
	if(!imgHeight)
		THROW_TEXT_ERROR(Osg_Err_lHeightFieldImageNotDefined, Osg_Err_strHeightFieldImageNotDefined, " Height Map: " + strHeightMap);

	osg::Image *imgNormals = osgDB::readImageFile(strNormalsMap.c_str());
	if(!imgNormals)
		THROW_TEXT_ERROR(Osg_Err_lHeightFieldImageNotDefined, Osg_Err_strHeightFieldImageNotDefined, " Normals Map: " + strNormalsMap);

	//Verify that the images have the same width/height/ and depth.
	if( (imgHeight->s() != imgNormals->s()) )
		THROW_TEXT_ERROR(Osg_Err_lHeightFieldImageMismatch, Osg_Err_strHeightFieldImageMismatch, " Height map: " + strHeightMap);

	if( (imgHeight->t() != imgNormals->t())) )
		THROW_TEXT_ERROR(Osg_Err_lHeightFieldImageMismatch, Osg_Err_strHeightFieldImageMismatch, " Height map: " + strHeightMap);

	if( (imgHeight->r() != imgNormals->r()) )
		THROW_TEXT_ERROR(Osg_Err_lHeightFieldImageMismatch, Osg_Err_strHeightFieldImageMismatch, " Height map: " + strHeightMap);

	int iWidth = imgHeight->s();
	int iLength = imgHeight->t();

	float fltWidthSize = iWidth*fltSegmentWidth;
	float fltLengthSize = iLength*fltSegmentLength;

	int iWidthSteps = (int) (fltWidthSize/fltSegmentWidth);
	int iLengthSteps = (int) (fltWidthSize/fltSegmentWidth);

	//osg::Geometry* boxGeom = new osg::Geometry();
	osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec3Array> norms = new osg::Vec3Array(); 
	osg::ref_ptr<osg::Vec2Array> texts = new osg::Vec2Array(); 
	int iLen = 0;
	int iPos = 0;

	float fltX1 = fltLeftCorner;
	float fltX2 = fltX1 + fltSegmentWidth;
	fltZ1 = fltUpperCorner;
	fltZ2 = fltZ1 + fltSegmentLength;
	for(int ix=0; ix<(int) steps.x(); ix++)
	{
		for(int iz=0; iz<(int) steps.z(); iz++)
		{
			osg::Vec4 vHeight = imgHeight->getColor(ix, iz);
			osg::Vec4 vNorm = imgNormals->getColor(ix, iz);

			verts->push_back(osg::Vec3(fltX1, sizeMin.y(), fltZ1)); // 0
			verts->push_back(osg::Vec3(fltX2, sizeMin.y(), fltZ1)); // 3
			verts->push_back(osg::Vec3(fltX2, sizeMin.y(), fltZ2)); // 5
			verts->push_back(osg::Vec3(fltX1, sizeMin.y(), fltZ2)); // 1

			norms->push_back(osg::Vec3( 0, -1,  0));
			norms->push_back(osg::Vec3( 0, -1,  0));
			norms->push_back(osg::Vec3( 0, -1,  0));
			norms->push_back(osg::Vec3( 0, -1,  0));

			texts->push_back(osg::Vec2( 0.f, 0.f)); // 0
			texts->push_back(osg::Vec2( 1.f, 0.f)); // 3
			texts->push_back(osg::Vec2( 1.f, 1.f)); // 5
			texts->push_back(osg::Vec2( 0.f, 1.f)); // 1
			
			fltZ1+=fltSegmentLength; fltZ2+=fltSegmentLength;
		}

		fltX1+=fltSegmentWidth; fltX2+=fltSegmentWidth;
		fltZ1 = fltUpperCorner; fltZ2 = fltZ1 + fltSegmentLength;
	}

    // create the geometry
     boxGeom->setVertexArray(verts.get());
     boxGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, verts->size()));

	 boxGeom->setNormalArray(norms.get());
     boxGeom->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);

     boxGeom->setTexCoordArray( 0, texts.get() );

     osg::Vec4Array* colors = new osg::Vec4Array;
     colors->push_back(osg::Vec4(1,1,1,1));
     boxGeom->setColorArray(colors);
     boxGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

    return boxGeom;
}
*/

#pragma endregion

	}			// Environment
//}				//OsgAnimatSim

}