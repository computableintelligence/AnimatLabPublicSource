// OsgSimulator.cpp: implementation of the OsgSimulator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdarg.h>
#include "OsgMovableItem.h"
//#include "VsBody.h"
//#include "VsJoint.h"
#include "OsgOrganism.h"
#include "OsgStructure.h"
#include "OsgSimulator.h"

#include "OsgMouseSpring.h"
#include "OsgLight.h"
#include "OsgCameraManipulator.h"
#include "OsgDragger.h"
#include "OsgMeshMinVertexDistanceVisitor.h"
#include "OsgSimulator.h"

namespace OsgAnimatSim
{

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


OsgSimulator::OsgSimulator()
{
	m_grpScene = NULL;	
	m_vsWinMgr = NULL;
	m_vsWinMgr = new OsgSimulationWindowMgr;
	m_lpWinMgr = m_vsWinMgr;
	m_lpWinMgr->SetSystemPointers(this, NULL, NULL, NULL, TRUE);
	m_dblTotalStepTime = 0;
	m_lStepTimeCount = 0;
	m_dblTotalStepTime= 0;
	m_lStepTimeCount = 0;
	m_lpMeshMgr = NULL;
	m_osgAlphafunc = NULL;
}

OsgSimulator::~OsgSimulator()
{

try
{
	if(m_lpMeshMgr)
	{
		delete m_lpMeshMgr;
		m_lpMeshMgr = NULL;
	}

	m_bShuttingDown = TRUE;

	Reset();
}
catch(...)
{Std_TraceMsg(0, "Caught Error in desctructor of Simulator\r\n", "", -1, FALSE, TRUE);}
}

void OsgSimulator::AlphaThreshold(float fltValue)
{
	Simulator::AlphaThreshold(fltValue);

	if(m_osgAlphafunc)
		    m_osgAlphafunc->setFunction(osg::AlphaFunc::GEQUAL, m_fltAlphaThreshold);
}


SimulationRecorder *OsgSimulator::CreateSimulationRecorder()
{
//NEED_TO_FIX
	return NULL; //new VsSimulationRecorder;
}

void OsgSimulator::Reset()
{
	Simulator::Reset();

	if(m_osgCmdMgr.valid())
		m_osgCmdMgr.release();

	if(m_lpMeshMgr)
	{
		delete m_lpMeshMgr;
		m_lpMeshMgr = NULL;
	}

}

void OsgSimulator::ToggleSimulation()
{
	if(m_bPaused)
		SimStarting();
	else
		SimPausing();

	m_bPaused = !m_bPaused;
}

void OsgSimulator::StopSimulation()
{
	SimStopping();
	if(!m_bPaused)
		ToggleSimulation();
	m_bSimRunning = FALSE;
}


osg::NotifySeverity OsgSimulator::ConvertTraceLevelToOSG()
{
	int iLevel = Std_GetTraceLevel();

	switch (iLevel)
	{
	case 0:
		return osg::NotifySeverity::FATAL;
	case 10:
		return osg::NotifySeverity::WARN;
	case 20:
		return osg::NotifySeverity::INFO;
	case 30:
		return osg::NotifySeverity::DEBUG_INFO;
	case 40:
		return osg::NotifySeverity::DEBUG_FP;
	default:
		return osg::NotifySeverity::WARN;
	}
}


//Timer Methods
unsigned long long OsgSimulator::GetTimerTick()
{
	m_lLastTickTaken = osg::Timer::instance()->tick();
	return m_lLastTickTaken;
}

double OsgSimulator::TimerDiff_n(unsigned long long lStart, unsigned long long lEnd)
{return osg::Timer::instance()->delta_n(lStart, lEnd);}

double OsgSimulator::TimerDiff_u(unsigned long long lStart, unsigned long long lEnd)
{return osg::Timer::instance()->delta_u(lStart, lEnd);}

double OsgSimulator::TimerDiff_m(unsigned long long lStart, unsigned long long lEnd)
{return osg::Timer::instance()->delta_m(lStart, lEnd);}

double OsgSimulator::TimerDiff_s(unsigned long long lStart, unsigned long long lEnd)
{return osg::Timer::instance()->delta_s(lStart, lEnd);}

void OsgSimulator::MicroSleep(unsigned int iMicroTime)
{OpenThreads::Thread::microSleep(iMicroTime);}

void OsgSimulator::WriteToConsole(string strMessage)
{
	osg::notify(osg::NOTICE) << strMessage << std::endl;
}

void OsgSimulator::Initialize(int argc, const char **argv)
{
	InitializeStructures();

	m_oDataChartMgr.Initialize();
	m_oExternalStimuliMgr.Initialize();
	if(m_lpSimRecorder) m_lpSimRecorder->Initialize();

	//realize the osg viewer
	m_vsWinMgr->Realize();
}

void OsgSimulator::UpdateSimulationWindows()
{
	m_bStopSimulation = !m_vsWinMgr->Update();
}

void OsgSimulator::ShutdownSimulation()
{
	SimStopping();
	m_bForceSimulationStop = TRUE;
}

BOOL OsgSimulator::PauseSimulation()
{
	SimPausing();
	m_bPaused = TRUE;
	return TRUE;
}

BOOL OsgSimulator::StartSimulation()
{
	m_lStartSimTick = GetTimerTick();

	SimStarting();
	m_bSimRunning = TRUE;
	m_bPaused = FALSE;
	return TRUE;
}

float *OsgSimulator::GetDataPointer(const string &strDataType)
{
	float *lpData=NULL;
	string strType = Std_CheckString(strDataType);

	//if(strType == "FRAMEDT")
	//	lpData = &m_fltFrameDt;
	//else
	//{
		lpData = Simulator::GetDataPointer(strDataType);
		if(!lpData)
			THROW_TEXT_ERROR(Al_Err_lInvalidDataType, Al_Err_strInvalidDataType, "Simulator DataType: " + strDataType);
	//}

	return lpData;
}

void OsgSimulator::SnapshotStopFrame()
{
	if(m_lpSimStopPoint) delete m_lpSimStopPoint;
	m_lpSimStopPoint = dynamic_cast<KeyFrame *>(CreateObject("AnimatLab", "KeyFrame", "Snapshot"));
	if(!m_lpSimStopPoint)
		THROW_TEXT_ERROR(Al_Err_lConvertingClassToType, Al_Err_strConvertingClassToType, "KeyFrame");

	m_lpSimStopPoint->StartSlice(m_lTimeSlice);
	m_lpSimStopPoint->EndSlice(m_lTimeSlice);
	m_lpSimStopPoint->Activate();
}

OsgSimulator *OsgSimulator::ConvertSimulator(Simulator *lpSim)
{
	if(!lpSim)
		THROW_ERROR(Al_Err_lSimulationNotDefined, Al_Err_strSimulationNotDefined);

	OsgSimulator *lpVsSim = dynamic_cast<OsgSimulator *>(lpSim);

	if(!lpVsSim)
		THROW_ERROR(Osg_Err_lUnableToConvertToVsSimulator, Osg_Err_strUnableToConvertToVsSimulator);

	return lpVsSim;
}

void OsgSimulator::Save(string strFile) 
{
	string strOsgFile = strFile + ".osg";

	osgDB::writeNodeFile(*OSGRoot(), strOsgFile.c_str());
}


}			//OsgAnimatSim