#pragma once

namespace OsgAnimatSim
{
	namespace Visualization
	{

class ANIMAT_OSG_PORT OsgSimulationWindow : public AnimatSim::SimulationWindow
{
	protected:
		OsgSimulationWindowMgr *m_lpWinMgr;

		osg::ref_ptr<osgViewer::Viewer> m_osgViewer;

		osg::ref_ptr<osgGA::TrackballManipulator> m_osgManip;

		//osg::ref_ptr<VsStatsHandler> m_osgStatsHandler;

		BodyPart *m_lpTrackBody;

		float m_fltCameraPosX, m_fltCameraPosY, m_fltCameraPosZ;

        /// true if we have set the eye position at least once.
        bool m_bEyePosSet;

		virtual void InitEmbedded(Simulator *lpSim, OsgSimulator *lpVsSim);
		virtual void InitStandalone(Simulator *lpSim, OsgSimulator *lpVsSim);
		virtual void TrackCamera();

		virtual ~OsgSimulationWindow(void);

	public:
		OsgSimulationWindow(void);

		virtual CStdFPoint GetCameraPosition();

		virtual BodyPart *TrackBody() {return m_lpTrackBody;};
		virtual osg::Matrix GetScreenMatrix();
		virtual osg::Viewport* GetViewport();
		virtual osgViewer::Viewer *Viewer() {return m_osgViewer.get();};

		virtual void SetupTrackCamera(bool bResetEyePos);
		virtual void SetCameraLookAt(CStdFPoint oTarget, bool bResetEyePos);
		virtual void SetCameraPositionAndLookAt(CStdFPoint oCameraPos, CStdFPoint oTarget);
		virtual void SetCameraPositionAndLookAt(osg::Vec3d vCameraPos, osg::Vec3d vTarget);
		virtual void SetCameraPostion(CStdFPoint vCameraPos);

		virtual float *GetDataPointer(const std::string &strDataType);

		virtual void UpdateBackgroundColor();

		virtual void Initialize();
		virtual void Update();
		virtual void Close();

		virtual void OnGetFocus();
		virtual void OnLoseFocus();
};

	}// end Visualization
}// end OsgAnimatSim

