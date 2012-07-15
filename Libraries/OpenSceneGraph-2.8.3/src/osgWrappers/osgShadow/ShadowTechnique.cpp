// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/CopyOp>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osgShadow/ShadowTechnique>
#include <osgShadow/ShadowedScene>
#include <osgUtil/CullVisitor>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgShadow::ShadowTechnique)
	I_DeclaringFile("osgShadow/ShadowTechnique");
	I_BaseType(osg::Object);
	I_Constructor0(____ShadowTechnique,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgShadow::ShadowTechnique &, es, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____ShadowTechnique__C5_ShadowTechnique_R1__C5_osg_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "Clone the type of an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "Clone an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method0(osgShadow::ShadowedScene *, getShadowedScene,
	          Properties::NON_VIRTUAL,
	          __ShadowedScene_P1__getShadowedScene,
	          "",
	          "");
	I_Method0(void, init,
	          Properties::VIRTUAL,
	          __void__init,
	          "initialize the ShadowedScene and local cached data structures. ",
	          "");
	I_Method1(void, update, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__update__osg_NodeVisitor_R1,
	          "run the update traversal of the ShadowedScene and update any local cached data structures. ",
	          "");
	I_Method1(void, cull, IN, osgUtil::CullVisitor &, cv,
	          Properties::VIRTUAL,
	          __void__cull__osgUtil_CullVisitor_R1,
	          "run the cull traversal of the ShadowedScene and set up the rendering for this ShadowTechnique. ",
	          "");
	I_Method0(void, cleanSceneGraph,
	          Properties::VIRTUAL,
	          __void__cleanSceneGraph,
	          "Clean scene graph from any shadow technique specific nodes, state and drawables. ",
	          "");
	I_Method1(void, traverse, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__traverse__osg_NodeVisitor_R1,
	          "",
	          "");
	I_Method0(void, dirty,
	          Properties::VIRTUAL,
	          __void__dirty,
	          "Dirty so that cached data structures are updated. ",
	          "");
	I_SimpleProperty(osgShadow::ShadowedScene *, ShadowedScene, 
	                 __ShadowedScene_P1__getShadowedScene, 
	                 0);
END_REFLECTOR

