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
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osgSim/MultiSwitch>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

TYPE_NAME_ALIAS(std::vector< bool >, osgSim::MultiSwitch::ValueList)

TYPE_NAME_ALIAS(std::vector< osgSim::MultiSwitch::ValueList >, osgSim::MultiSwitch::SwitchSetList)

BEGIN_OBJECT_REFLECTOR(osgSim::MultiSwitch)
	I_DeclaringFile("osgSim/MultiSwitch");
	I_BaseType(osg::Group);
	I_Constructor0(____MultiSwitch,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgSim::MultiSwitch &, x, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____MultiSwitch__C5_MultiSwitch_R1__C5_osg_CopyOp_R1,
	                           "Copy constructor using CopyOp to manage deep vs shallow copy. ",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "clone an object of the same type as the node. ",
	          "");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "return a clone of a node, with Object* return type. ",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "return true if this and obj are of the same kind of object. ",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "return the name of the node's class type. ",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "return the name of the node's library. ",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "Visitor Pattern : calls the apply method of a NodeVisitor with this node's type. ",
	          "");
	I_Method1(void, traverse, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__traverse__osg_NodeVisitor_R1,
	          "Traverse downwards : calls children's accept method with NodeVisitor. ",
	          "");
	I_Method1(void, setNewChildDefaultValue, IN, bool, value,
	          Properties::NON_VIRTUAL,
	          __void__setNewChildDefaultValue__bool,
	          "",
	          "");
	I_Method0(bool, getNewChildDefaultValue,
	          Properties::NON_VIRTUAL,
	          __bool__getNewChildDefaultValue,
	          "",
	          "");
	I_Method1(bool, addChild, IN, osg::Node *, child,
	          Properties::VIRTUAL,
	          __bool__addChild__osg_Node_P1,
	          "Add Node to Group. ",
	          "If node is not NULL and is not contained in Group then increment its reference count, add it to the child list and dirty the bounding sphere to force it to recompute on next getBound() and return true for success. Otherwise return false. Scene nodes can't be added as child nodes. ");
	I_Method2(bool, insertChild, IN, unsigned int, index, IN, osg::Node *, child,
	          Properties::VIRTUAL,
	          __bool__insertChild__unsigned_int__osg_Node_P1,
	          "Insert Node to Group at specific location. ",
	          "The new child node is inserted into the child list before the node at the specified index. No nodes are removed from the group with this operation. ");
	I_Method1(bool, removeChild, IN, osg::Node *, child,
	          Properties::VIRTUAL,
	          __bool__removeChild__osg_Node_P1,
	          "Remove Node from Group. ",
	          "If Node is contained in Group then remove it from the child list, decrement its reference count, and dirty the bounding sphere to force it to recompute on next getBound() and return true for success. If Node is not found then return false and do not change the reference count of the Node. Note, do not override, only override removeChildren(,) is required. ");
	I_Method3(void, setValue, IN, unsigned int, switchSet, IN, unsigned int, pos, IN, bool, value,
	          Properties::NON_VIRTUAL,
	          __void__setValue__unsigned_int__unsigned_int__bool,
	          "",
	          "");
	I_Method2(bool, getValue, IN, unsigned int, switchSet, IN, unsigned int, pos,
	          Properties::NON_VIRTUAL,
	          __bool__getValue__unsigned_int__unsigned_int,
	          "",
	          "");
	I_Method3(void, setChildValue, IN, const osg::Node *, child, IN, unsigned int, switchSet, IN, bool, value,
	          Properties::NON_VIRTUAL,
	          __void__setChildValue__C5_osg_Node_P1__unsigned_int__bool,
	          "",
	          "");
	I_Method2(bool, getChildValue, IN, const osg::Node *, child, IN, unsigned int, switchSet,
	          Properties::NON_VIRTUAL,
	          __bool__getChildValue__C5_osg_Node_P1__unsigned_int,
	          "",
	          "");
	I_Method1(bool, setAllChildrenOff, IN, unsigned int, switchSet,
	          Properties::NON_VIRTUAL,
	          __bool__setAllChildrenOff__unsigned_int,
	          "Set all the children off (false), and set the new default child value to off (false). ",
	          "");
	I_Method1(bool, setAllChildrenOn, IN, unsigned int, switchSet,
	          Properties::NON_VIRTUAL,
	          __bool__setAllChildrenOn__unsigned_int,
	          "Set all the children on (true), and set the new default child value to on (true). ",
	          "");
	I_Method2(bool, setSingleChildOn, IN, unsigned int, switchSet, IN, unsigned int, pos,
	          Properties::NON_VIRTUAL,
	          __bool__setSingleChildOn__unsigned_int__unsigned_int,
	          "Set a single child to be on, MultiSwitch off all other children. ",
	          "");
	I_Method1(void, setActiveSwitchSet, IN, unsigned int, switchSet,
	          Properties::NON_VIRTUAL,
	          __void__setActiveSwitchSet__unsigned_int,
	          "Set which of the available switch set lists to use. ",
	          "");
	I_Method0(unsigned int, getActiveSwitchSet,
	          Properties::NON_VIRTUAL,
	          __unsigned_int__getActiveSwitchSet,
	          "Get which of the available switch set lists to use. ",
	          "");
	I_Method1(void, setSwitchSetList, IN, const osgSim::MultiSwitch::SwitchSetList &, switchSetList,
	          Properties::NON_VIRTUAL,
	          __void__setSwitchSetList__C5_SwitchSetList_R1,
	          "Set the compile set of different values. ",
	          "");
	I_Method0(const osgSim::MultiSwitch::SwitchSetList &, getSwitchSetList,
	          Properties::NON_VIRTUAL,
	          __C5_SwitchSetList_R1__getSwitchSetList,
	          "Get the compile set of different values. ",
	          "");
	I_Method2(void, setValueList, IN, unsigned int, switchSet, IN, const osgSim::MultiSwitch::ValueList &, values,
	          Properties::NON_VIRTUAL,
	          __void__setValueList__unsigned_int__C5_ValueList_R1,
	          "Set the a single set of different values for a particular switch set. ",
	          "");
	I_Method1(const osgSim::MultiSwitch::ValueList &, getValueList, IN, unsigned int, switchSet,
	          Properties::NON_VIRTUAL,
	          __C5_ValueList_R1__getValueList__unsigned_int,
	          "Get the a single set of different values for a particular switch set. ",
	          "");
	I_ProtectedMethod1(void, expandToEncompassSwitchSet, IN, unsigned int, switchSet,
	                   Properties::NON_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__expandToEncompassSwitchSet__unsigned_int,
	                   "",
	                   "");
	I_SimpleProperty(unsigned int, ActiveSwitchSet, 
	                 __unsigned_int__getActiveSwitchSet, 
	                 __void__setActiveSwitchSet__unsigned_int);
	I_SimpleProperty(unsigned int, AllChildrenOff, 
	                 0, 
	                 __bool__setAllChildrenOff__unsigned_int);
	I_SimpleProperty(unsigned int, AllChildrenOn, 
	                 0, 
	                 __bool__setAllChildrenOn__unsigned_int);
	I_IndexedProperty(bool, ChildValue, 
	                  __bool__getChildValue__C5_osg_Node_P1__unsigned_int, 
	                  __void__setChildValue__C5_osg_Node_P1__unsigned_int__bool, 
	                  0);
	I_SimpleProperty(bool, NewChildDefaultValue, 
	                 __bool__getNewChildDefaultValue, 
	                 __void__setNewChildDefaultValue__bool);
	I_SimpleProperty(const osgSim::MultiSwitch::SwitchSetList &, SwitchSetList, 
	                 __C5_SwitchSetList_R1__getSwitchSetList, 
	                 __void__setSwitchSetList__C5_SwitchSetList_R1);
	I_IndexedProperty(bool, Value, 
	                  __bool__getValue__unsigned_int__unsigned_int, 
	                  __void__setValue__unsigned_int__unsigned_int__bool, 
	                  0);
	I_IndexedProperty(const osgSim::MultiSwitch::ValueList &, ValueList, 
	                  __C5_ValueList_R1__getValueList__unsigned_int, 
	                  __void__setValueList__unsigned_int__C5_ValueList_R1, 
	                  0);
END_REFLECTOR

STD_VECTOR_REFLECTOR(std::vector< osgSim::MultiSwitch::ValueList >)

