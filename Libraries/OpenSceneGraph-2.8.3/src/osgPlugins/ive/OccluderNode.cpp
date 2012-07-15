/**********************************************************************
 *
 *    FILE:            OccluderNode.cpp
 *
 *    DESCRIPTION:    Read/Write osg::OccluderNode in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerator
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 16.4.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "OccluderNode.h"
#include "Group.h"
#include "ConvexPlanarOccluder.h"

using namespace ive;

void OccluderNode::write(DataOutputStream* out){
    // Write OccluderNode's identification.
    out->writeInt(IVEOCCLUDERNODE);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Group*  group = dynamic_cast<osg::Group*>(this);
    if(group){
        ((ive::Group*)(group))->write(out);
    }
    else
        throw Exception("OccluderNode::write(): Could not cast this osg::OccluderNode to an osg::Group.");
    // Write OccluderNode's properties.
    out->writeBool(getOccluder()!=0);
    if(getOccluder())
        ((ive::ConvexPlanarOccluder*)(getOccluder()))->write(out);
}

void OccluderNode::read(DataInputStream* in)
{
    // Peek on OccluderNode's identification.
    int id = in->peekInt();
    if(id == IVEOCCLUDERNODE)
    {
        // Read OccluderNode's identification.
        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osg::Group*  group = dynamic_cast<osg::Group*>(this);
        if(group){
            ((ive::Group*)(group))->read(in);
        }
        else
            throw Exception("OccluderNode::read(): Could not cast this osg::OccluderNode to an osg::Group.");
        // Read OccluderNode's properties
        if(in->readBool()){
            osg::ConvexPlanarOccluder* cpo = new osg::ConvexPlanarOccluder();
            ((ive::ConvexPlanarOccluder*)(cpo))->read(in);
            setOccluder(cpo);
        }
    }
    else{
        throw Exception("OccluderNode::read(): Expected OccluderNode identification.");
    }
}
