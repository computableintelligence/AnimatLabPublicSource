#pragma once

namespace AnimatSim
{
        /**
         \brief Animatlab PID control system. This implements the CStdPid with extra AnimatBase functionality. 
        
         \author    David Cofer
         \date  1/25/2014
         */
		class ANIMAT_PORT PidControl : public CStdPID, public AnimatBase 
		{
		protected:
            /// Enables/disables this PID controller
            bool m_bEnabled;

		    /// Keeps track of the enabled state at sim startup.
		    bool m_bInitEnabled;

            //Used for reporting of enabled state of this controller
            float m_fltEnabled;

		public:
			PidControl();
            PidControl(float fltSetpoint, float fltGain, float fltIntegralAct, float fltDerivativeAct, 
                bool bComplexError, bool bAntiResetWindup, bool bRampLimit, 
                float fltRangeMax, float fltRangeMin, float fltARWBound, float fltRampGradient);
			virtual ~PidControl();

		    virtual bool Enabled();
		    virtual void Enabled(bool bValue);

			virtual bool SetData(const std::string &strDataType, const std::string &strValue, bool bThrowError = true);
			virtual void QueryProperties(CStdArray<std::string> &aryNames, CStdArray<std::string> &aryTypes);
			virtual float *GetDataPointer(const std::string &strDataType);
			virtual void ResetSimulation();
			virtual void Load(CStdXml &oXml);
		};

}				//AnimatSim