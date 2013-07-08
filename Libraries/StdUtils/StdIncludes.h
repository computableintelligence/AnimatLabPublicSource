#if !defined STD_UTILS_INCLUDES
#define STD_UTILS_INCLUDES

#ifdef _WINDOWS
	#define STD_UTILS_PORT __declspec( dllexport )
#else
	#define STD_UTILS_PORT 
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning(disable: 4018 4244 4290 4786 4251 4275 4267 4311 4312 4800 )

#ifdef _WINDOWS
	#ifndef _WIN32_WCE
		#include <conio.h>
		#include <io.h>
		#include <tchar.h> 
	#endif
#else
	#include <linux/types.h>
	#include <stdbool.h>
	#include <dlfcn.h>
	
	#define BOOL bool
	#define DWORD unsigned long
	#define LPCSTR const char *
	#define FALSE 0
	#define TRUE 1
	#define LONG long
	#define ULONG unsigned long
	#define LPLONG long *
	#define __int64 int64_t
	#define LPCTSTR const char*
#endif

#include <sys/types.h>
#include <sys/timeb.h>


#include <exception>
#include <string>
#include <iosfwd>
#include <sstream>
#include <fstream>
#include <iterator>
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <map>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <cctype>
#include <math.h>
#include <memory.h>
#include <algorithm>
#include <string.h>
using namespace std;

#define STD_TRACING_ON
//#define STD_TRACE_DEBUG
//#define STD_TRACE_INFO
#define STD_TRACE_DETAIL

#define STD_TRACE_TO_DEBUGGER
#define STD_TRACE_TO_FILE true

class CStdFont;
class CStdVariable;
class CStdPostFixEval;
class CMarkupSTL;
class CStdXml;
class CStdCriticalSection;
class CStdErrorInfo;

void STD_UTILS_PORT Std_RelayError(CStdErrorInfo oInfo, string strSourceFile, long lSourceLine);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strValueName, unsigned char iVal);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strValueName, unsigned short iVal);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strValueName, int iVal);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strValueName, long lVal);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strValueName, float fltVal);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strValueName, double dblVal);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strValueName, string strVal);
void STD_UTILS_PORT Std_ThrowError(long lError, string strError, string strSourceFile, long lSourceLine, 
																	 string strText = "");


#include "tree.hh"
#include "tree_util.hh"

#include "StdConstants.h"
#include "StdADT.h"
#include "StdADTCopy.h"
#include "StdErrorInfo.h"
#include "StdUtilFunctions.h"
#include "MarkupSTL.h"
#include "StdXml.h"
#include "StdSerialize.h"
#include "StdFont.h"
#include "StdVariable.h"
#include "StdPostFixEval.h"
#include "StdVariant.h"
#include "StdClassFactory.h"
#include "StdLookupTable.h"
#include "StdFixed.h"
#include "StdColor.h"

#ifdef _WINDOWS
	#include "StdTimer.h"
	#include "StdCriticalSection.h"

	#ifndef _WIN32_WCE
		#include "XYTrace.h"
	#else
		#include "StdLogFile.h"
	#endif
#endif

#endif //STD_UTILS_INCLUDES
