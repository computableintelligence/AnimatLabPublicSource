/**
\file	StdClassFactory.cpp

\brief	Implements the standard class factory class.
**/

#include "StdAfx.h"


namespace StdUtils
{

/**
\brief	Default constructor.

\author	dcofer
\date	5/3/2011
**/
IStdClassFactory::IStdClassFactory()
{
}

/**
\brief	Destructor.

\author	dcofer
\date	5/3/2011
**/
IStdClassFactory::~IStdClassFactory()
{}

#ifdef WIN32
/**
\brief	Loads a DLL module by name and attempts to call the GetStdClassFactory method to get a pointer to the class factory.

\author	dcofer
\date	5/3/2011

\param	strModuleName	Name of the DLL module.

\return	Pointer to the loaded module, exception if not found.
**/
IStdClassFactory *IStdClassFactory::LoadModule(std::string strModuleName, bool bThrowError)
{
	TRACE_DEBUG("Loading Module: " + strModuleName);

	if(Std_IsBlank(strModuleName))
		THROW_ERROR(Std_Err_lModuleNameIsBlank, Std_Err_strModuleNameIsBlank);

	HMODULE hMod = NULL;

	hMod = LoadLibrary(strModuleName.c_str());

	if(!hMod)
    {
	    if(bThrowError)
	    {
	        int iError = GetLastError();
			THROW_PARAM_ERROR(Std_Err_lModuleNotLoaded, Std_Err_strModuleNotLoaded, "Module", strModuleName + ", Last Error: " + STR(iError));
			return NULL;
	    }
	    else
	    	return NULL;
    }

	GetClassFactory lpFactoryFunc = NULL;

	TRACE_DEBUG("  Gettting the classfactory pointer.");

    lpFactoryFunc = (GetClassFactory) GetProcAddress(hMod, "GetStdClassFactory");

	if(!lpFactoryFunc)
	{
		if(bThrowError)
			THROW_PARAM_ERROR(Std_Err_lModuleProcNotLoaded, Std_Err_strModuleProcNotLoaded, "Module", strModuleName);
		else
			return NULL;
	}

	TRACE_DEBUG("Finished Loading Module: " + strModuleName);
	return lpFactoryFunc();
}

#else
/**
\brief	Loads a DLL module by name and attempts to call the GetStdClassFactory method to get a pointer to the class factory.

\author	dcofer
\date	5/3/2011

\param	strModuleName	Name of the DLL module.

\return	Pointer to the loaded module, exception if not found.
**/
IStdClassFactory *IStdClassFactory::LoadModule(std::string strModuleName, bool bThrowError)
{
	TRACE_DEBUG("Loading Module: " + strModuleName);

	if(Std_IsBlank(strModuleName))
		THROW_ERROR(Std_Err_lModuleNameIsBlank, Std_Err_strModuleNameIsBlank);

	//If the module name already has .so in it then do not modfiy it.
	std::string strModRenamed = strModuleName;
	if(Std_ToLower(strModuleName).find(".so") == -1)
	{
		strModRenamed = "lib" + Std_Replace(strModuleName, ".dll", ".so");
		strModRenamed = Std_Replace(strModRenamed, "VC10", "vc10");
	}

	void *hMod = NULL;

	hMod = dlopen(strModRenamed.c_str(), RTLD_LAZY);

	if(!hMod)
	{
		if(bThrowError)
        {
            std::string strError = strModRenamed + ", Last Error: " + dlerror();
			THROW_PARAM_ERROR(Std_Err_lModuleNotLoaded, Std_Err_strModuleNotLoaded, "Module", strError);
        }
		else
			return NULL;
	}

	GetClassFactory lpFactoryFunc = NULL;

	TRACE_DEBUG("  Gettting the classfactory pointer.");

	lpFactoryFunc = (GetClassFactory) dlsym(hMod, "GetStdClassFactory");

	if(!lpFactoryFunc || dlerror() != NULL)
	{
		if(bThrowError)
			THROW_PARAM_ERROR(Std_Err_lModuleProcNotLoaded, Std_Err_strModuleProcNotLoaded, "Module", strModRenamed + ", Last Error: " + dlerror());
		else
			return NULL;
	}

	TRACE_DEBUG("Finished Loading Module: " + strModRenamed);
	IStdClassFactory *lpFact = lpFactoryFunc();

	TRACE_DEBUG("Returning class factory: " + strModRenamed);
	return lpFact;
}

#endif


}				//StdUtils





