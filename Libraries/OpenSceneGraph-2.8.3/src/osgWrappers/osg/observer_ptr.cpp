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

#include <osg/observer_ptr>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_VALUE_REFLECTOR(osg::Observer)
	I_DeclaringFile("osg/observer_ptr");
	I_Constructor0(____Observer,
	               "",
	               "");
	I_Method1(void, objectDeleted, IN, void *, x,
	          Properties::VIRTUAL,
	          __void__objectDeleted__void_P1,
	          "",
	          "");
END_REFLECTOR

