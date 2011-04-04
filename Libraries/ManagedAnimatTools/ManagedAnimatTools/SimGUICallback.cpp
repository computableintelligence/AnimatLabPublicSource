#include "stdafx.h"
#include "Util.h"
#include "Logger.h"
#include "PropertyUpdateException.h"
#include "SimulatorInterface.h"
#include "SimGUICallback.h"
#include "DataObjectInterface.h"

namespace AnimatGUI
{
	namespace Interfaces
	{

SimGUICallback::SimGUICallback(SimulatorInterface ^doSim)
{
	m_doSim = doSim;
}

SimGUICallback::~SimGUICallback(void)
{
}

void SimGUICallback::NeedToStopSimulation()
{
	m_doSim->FireNeedToStopSimulationEvent();
}


	}
}