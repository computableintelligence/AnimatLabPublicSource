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

#include <osg/ApplicationUsage>
#include <osg/Matrixd>
#include <osg/Node>
#include <osgGA/DriveManipulator>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventAdapter>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgGA::DriveManipulator)
	I_DeclaringFile("osgGA/DriveManipulator");
	I_BaseType(osgGA::MatrixManipulator);
	I_Constructor0(____DriveManipulator,
	               "",
	               "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method1(void, setByMatrix, IN, const osg::Matrixd &, matrix,
	          Properties::VIRTUAL,
	          __void__setByMatrix__C5_osg_Matrixd_R1,
	          "set the position of the matrix manipulator using a 4x4 Matrix. ",
	          "");
	I_Method1(void, setByInverseMatrix, IN, const osg::Matrixd &, matrix,
	          Properties::VIRTUAL,
	          __void__setByInverseMatrix__C5_osg_Matrixd_R1,
	          "set the position of the matrix manipulator using a 4x4 Matrix. ",
	          "");
	I_Method0(osg::Matrixd, getMatrix,
	          Properties::VIRTUAL,
	          __osg_Matrixd__getMatrix,
	          "get the position of the manipulator as 4x4 Matrix. ",
	          "");
	I_Method0(osg::Matrixd, getInverseMatrix,
	          Properties::VIRTUAL,
	          __osg_Matrixd__getInverseMatrix,
	          "get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix. ",
	          "");
	I_Method1(void, setNode, IN, osg::Node *, x,
	          Properties::VIRTUAL,
	          __void__setNode__osg_Node_P1,
	          "Attach a node to the manipulator, automatically detaching any previously attached node. ",
	          "setNode(NULL) detaches previous nodes. May be ignored by manipulators which do not require a reference model. ");
	I_Method0(const osg::Node *, getNode,
	          Properties::VIRTUAL,
	          __C5_osg_Node_P1__getNode,
	          "Return const node if attached. ",
	          "");
	I_Method0(osg::Node *, getNode,
	          Properties::VIRTUAL,
	          __osg_Node_P1__getNode,
	          "Return node if attached. ",
	          "");
	I_Method0(void, computeHomePosition,
	          Properties::VIRTUAL,
	          __void__computeHomePosition,
	          "Compute the home position. ",
	          "");
	I_Method2(void, home, IN, const osgGA::GUIEventAdapter &, ea, IN, osgGA::GUIActionAdapter &, us,
	          Properties::VIRTUAL,
	          __void__home__C5_GUIEventAdapter_R1__GUIActionAdapter_R1,
	          "Move the camera to the default position. ",
	          "May be ignored by manipulators if home functionality is not appropriate. ");
	I_Method2(void, init, IN, const osgGA::GUIEventAdapter &, ea, IN, osgGA::GUIActionAdapter &, us,
	          Properties::VIRTUAL,
	          __void__init__C5_GUIEventAdapter_R1__GUIActionAdapter_R1,
	          "Start/restart the manipulator. ",
	          "FIXME: what does this actually mean? Provide examples. ");
	I_Method2(bool, handle, IN, const osgGA::GUIEventAdapter &, ea, IN, osgGA::GUIActionAdapter &, us,
	          Properties::VIRTUAL,
	          __bool__handle__C5_GUIEventAdapter_R1__GUIActionAdapter_R1,
	          "Handle events, return true if handled, false otherwise. ",
	          "");
	I_Method1(void, getUsage, IN, osg::ApplicationUsage &, usage,
	          Properties::VIRTUAL,
	          __void__getUsage__osg_ApplicationUsage_R1,
	          "Get the keyboard and mouse usage of this manipulator. ",
	          "");
	I_Method1(void, setModelScale, IN, double, in_ms,
	          Properties::NON_VIRTUAL,
	          __void__setModelScale__double,
	          "",
	          "");
	I_Method0(double, getModelScale,
	          Properties::NON_VIRTUAL,
	          __double__getModelScale,
	          "",
	          "");
	I_Method1(void, setVelocity, IN, double, in_vel,
	          Properties::NON_VIRTUAL,
	          __void__setVelocity__double,
	          "",
	          "");
	I_Method0(double, getVelocity,
	          Properties::NON_VIRTUAL,
	          __double__getVelocity,
	          "",
	          "");
	I_Method1(void, setHeight, IN, double, in_h,
	          Properties::NON_VIRTUAL,
	          __void__setHeight__double,
	          "",
	          "");
	I_Method0(double, getHeight,
	          Properties::NON_VIRTUAL,
	          __double__getHeight,
	          "",
	          "");
	I_ProtectedMethod4(bool, intersect, IN, const osg::Vec3d &, start, IN, const osg::Vec3d &, end, IN, osg::Vec3d &, intersection, IN, osg::Vec3d &, normal,
	                   Properties::NON_VIRTUAL,
	                   Properties::CONST,
	                   __bool__intersect__C5_osg_Vec3d_R1__C5_osg_Vec3d_R1__osg_Vec3d_R1__osg_Vec3d_R1,
	                   "",
	                   "");
	I_ProtectedMethod0(void, flushMouseEventStack,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__flushMouseEventStack,
	                   "Reset the internal GUIEvent stack. ",
	                   "");
	I_ProtectedMethod1(void, addMouseEvent, IN, const osgGA::GUIEventAdapter &, ea,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__addMouseEvent__C5_GUIEventAdapter_R1,
	                   "Add the current mouse GUIEvent to internal stack. ",
	                   "");
	I_ProtectedMethod3(void, computePosition, IN, const osg::Vec3d &, eye, IN, const osg::Vec3d &, lv, IN, const osg::Vec3d &, up,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__computePosition__C5_osg_Vec3d_R1__C5_osg_Vec3d_R1__C5_osg_Vec3d_R1,
	                   "",
	                   "");
	I_ProtectedMethod0(bool, calcMovement,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __bool__calcMovement,
	                   "For the give mouse movement calculate the movement of the camera. ",
	                   "Return true is camera has moved and a redraw is required. ");
	I_SimpleProperty(const osg::Matrixd &, ByInverseMatrix, 
	                 0, 
	                 __void__setByInverseMatrix__C5_osg_Matrixd_R1);
	I_SimpleProperty(const osg::Matrixd &, ByMatrix, 
	                 0, 
	                 __void__setByMatrix__C5_osg_Matrixd_R1);
	I_SimpleProperty(double, Height, 
	                 __double__getHeight, 
	                 __void__setHeight__double);
	I_SimpleProperty(osg::Matrixd, InverseMatrix, 
	                 __osg_Matrixd__getInverseMatrix, 
	                 0);
	I_SimpleProperty(osg::Matrixd, Matrix, 
	                 __osg_Matrixd__getMatrix, 
	                 0);
	I_SimpleProperty(double, ModelScale, 
	                 __double__getModelScale, 
	                 __void__setModelScale__double);
	I_SimpleProperty(osg::Node *, Node, 
	                 __osg_Node_P1__getNode, 
	                 __void__setNode__osg_Node_P1);
	I_SimpleProperty(double, Velocity, 
	                 __double__getVelocity, 
	                 __void__setVelocity__double);
END_REFLECTOR

