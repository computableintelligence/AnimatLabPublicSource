// TestClassFactory.h: interface for the VsClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
using namespace StdUtils;

class TestClassFactory : public IStdClassFactory  
{
public:
	TestClassFactory();
	virtual ~TestClassFactory();

	virtual CStdSerialize *CreateObject(string strClassType, string strObjectType, BOOL bThrowError = TRUE);
};