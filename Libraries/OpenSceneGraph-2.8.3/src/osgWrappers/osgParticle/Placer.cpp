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
#include <osg/Object>
#include <osg/Vec3>
#include <osgParticle/Particle>
#include <osgParticle/Placer>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osgParticle::Placer)
	I_DeclaringFile("osgParticle/Placer");
	I_BaseType(osg::Object);
	I_Constructor0(____Placer,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgParticle::Placer &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____Placer__C5_Placer_R1__C5_osg_CopyOp_R1,
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
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "",
	          "");
	I_Method1(void, place, IN, osgParticle::Particle *, P,
	          Properties::PURE_VIRTUAL,
	          __void__place__Particle_P1,
	          "Place a particle. Must be implemented in descendant classes. ",
	          "");
	I_Method0(osg::Vec3, getControlPosition,
	          Properties::PURE_VIRTUAL,
	          __osg_Vec3__getControlPosition,
	          "Return the control position of particles that placer will generate. Must be implemented in descendant classes. ",
	          "");
	I_SimpleProperty(osg::Vec3, ControlPosition, 
	                 __osg_Vec3__getControlPosition, 
	                 0);
END_REFLECTOR

