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

#include <osgWidget/StyleInterface>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_VALUE_REFLECTOR(osgWidget::StyleInterface)
	I_DeclaringFile("osgWidget/StyleInterface");
	I_Constructor0(____StyleInterface,
	               "",
	               "");
	I_Constructor1(IN, const osgWidget::StyleInterface &, si,
	               Properties::NON_EXPLICIT,
	               ____StyleInterface__C5_StyleInterface_R1,
	               "",
	               "");
	I_Method1(void, setStyle, IN, const std::string &, style,
	          Properties::NON_VIRTUAL,
	          __void__setStyle__C5_std_string_R1,
	          "",
	          "");
	I_Method0(std::string &, getStyle,
	          Properties::NON_VIRTUAL,
	          __std_string_R1__getStyle,
	          "",
	          "");
	I_Method0(const std::string &, getStyle,
	          Properties::NON_VIRTUAL,
	          __C5_std_string_R1__getStyle,
	          "",
	          "");
	I_SimpleProperty(const std::string &, Style, 
	                 __C5_std_string_R1__getStyle, 
	                 __void__setStyle__C5_std_string_R1);
END_REFLECTOR

