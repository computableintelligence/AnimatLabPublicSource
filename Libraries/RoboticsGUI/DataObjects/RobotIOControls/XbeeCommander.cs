﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AnimatGUI.Framework;

namespace RoboticsGUI
{
    namespace RobotIOControls
    {

        public class XBeeCommander : AnimatGUI.DataObjects.Robotics.RemoteControl
        {
            #region " Attributes "

            protected string m_strPort = "COM3";
            protected int m_iBaudRate = 1;

            #endregion

            #region " Attributes "

            public override string Description {get {return "Performs wireless IO using an UartSBee to communicate with the Commander remote control.";} set { }}
            public override string ButtonImageName {get {return "RoboticsGUI.Graphics.DynamixelUSB.gif";}}
            public override string WorkspaceImageName {get {return "RoboticsGUI.Graphics.DynamixelUSBSmall.gif";}}
            public override string PartType {get { return "XBeeCommander"; }}
            public override string ModuleName { get { return "RoboticsAnimatSim"; } }

            public virtual string Port
            {
                get
                {
                    return m_strPort;
                }
                set
                {
                    SetSimData("Port", value, true);
                    m_strPort = value;
                }
            }

            public virtual int BaudRate
            {
                get
                {
                    return m_iBaudRate;
                }
                set
                {
                    if(value <= 0)
                        throw new System.Exception("Invalid baud rate specified. Rate: " + value.ToString());

                    SetSimData("BaudRate", value.ToString(), true);
                    m_iBaudRate = value;
                }
            }

            #endregion

            #region " Methods "

            public XBeeCommander(AnimatGUI.Framework.DataObject doParent)
                : base(doParent)
            {
                m_strName = "XBee Commander";

                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("WalkV", "Walk Vertical", "", "", -128, 128));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("WalkH", "Walk Horizontal", "", "", -128, 128));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("LookV", "Look Vertical", "", "", -128, 128));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("LookH", "Look Horizontal", "", "", -128, 128));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("Pan", "Pan", "", "", -128, 128));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("Tilt", "Tilt", "", "", -128, 128));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("R1", "R1", "", "", 0, 1));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("R2", "R2", "", "", 0, 1));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("R3", "R3", "", "", 0, 1));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("L4", "L4", "", "", 0, 1));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("L5", "L5", "", "", 0, 1));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("L6", "L6", "", "", 0, 1));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("RT", "RT", "", "", 0, 1));
                m_thDataTypes.DataTypes.Add(new AnimatGUI.DataObjects.DataType("LT", "LT", "", "", 0, 1));

                m_aryLinks.Clear();

                AnimatGUI.DataObjects.Gains.Polynomial doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 0.0390625;
                doGain.D.Scale = ScaledNumber.enumNumericScale.nano; doGain.D.Value = 5;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doWalkV = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "WalkV", "WalkV", doGain);
                m_aryLinks.Add(doWalkV.ID, doWalkV, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 0.0390625;
                doGain.D.Scale = ScaledNumber.enumNumericScale.nano; doGain.D.Value = 5;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doWalkH = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "WalkH", "WalkH", doGain);
                m_aryLinks.Add(doWalkH.ID, doWalkH, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 0.0390625;
                doGain.D.Scale = ScaledNumber.enumNumericScale.nano; doGain.D.Value = 5;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doLookV = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "LookV", "LookV", doGain);
                m_aryLinks.Add(doLookV.ID, doLookV, false);

                doWalkV.SourceDataTypes.ID = "LookH";
                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 0.0390625;
                doGain.D.Scale = ScaledNumber.enumNumericScale.nano; doGain.D.Value = 5;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doLookH = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "LookH", "LookH", doGain);
                m_aryLinks.Add(doLookH.ID, doLookH, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doR1 = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "R1", "R1", doGain);
                m_aryLinks.Add(doR1.ID, doR1, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doR2 = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "R2", "R2", doGain);
                m_aryLinks.Add(doR2.ID, doR2, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doR3 = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "R3", "R3", doGain);
                m_aryLinks.Add(doR3.ID, doR3, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doL4 = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "L4", "L4", doGain);
                m_aryLinks.Add(doL4.ID, doL4, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doL5 = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "L5", "L5", doGain);
                m_aryLinks.Add(doL5.ID, doL5, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doL6 = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "L6", "L6", doGain);
                m_aryLinks.Add(doL6.ID, doL6, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doRT = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "RT", "RT", doGain);
                m_aryLinks.Add(doRT.ID, doRT, false);

                doGain = new AnimatGUI.DataObjects.Gains.Polynomial(null, "Gain", "", "Current", false);
                doGain.C.Scale = ScaledNumber.enumNumericScale.nano; doGain.C.Value = 10;
                AnimatGUI.DataObjects.Robotics.RemoteControlLinkage doLT = new AnimatGUI.DataObjects.Robotics.RemoteControlLinkage(this, "LT", "LT", doGain);
                m_aryLinks.Add(doLT.ID, doLT, false);
            }

            public override AnimatGUI.Framework.DataObject Clone(AnimatGUI.Framework.DataObject doParent, bool bCutData, AnimatGUI.Framework.DataObject doRoot)
            {
                XBeeCommander doInterface = new XBeeCommander(doParent);
                return doInterface;
            }

            protected override void CloneInternal(DataObject doOriginal, bool bCutData, DataObject doRoot)
            {
                base.CloneInternal(doOriginal, bCutData, doRoot);

                XBeeCommander doOrig = (XBeeCommander)doOriginal;

                if (doOrig != null)
                {
                    m_strPort = doOrig.m_strPort;
                    m_iBaudRate = doOrig.m_iBaudRate;
                }
            }

            public override void BuildProperties(ref AnimatGuiCtrls.Controls.PropertyTable propTable)
            {
                base.BuildProperties(ref propTable);

                propTable.Properties.Add(new AnimatGuiCtrls.Controls.PropertySpec("Com Port", this.Port.GetType(), "Port", "Properties", "Com port", this.Port));
                propTable.Properties.Add(new AnimatGuiCtrls.Controls.PropertySpec("Baud Rate", this.BaudRate.GetType(), "BaudRate", "Properties", "Baud rate to use for communications", this.BaudRate));
            }

            public override void LoadData(ManagedAnimatInterfaces.IStdXml oXml)
            {
                base.LoadData(oXml);

                oXml.IntoElem();

                m_strPort = oXml.GetChildString("Port", m_strPort);
                m_iBaudRate = oXml.GetChildInt("BaudRate", m_iBaudRate);

                oXml.OutOfElem(); 
            }

            public override void SaveData(ManagedAnimatInterfaces.IStdXml oXml)
            {
                base.SaveData(oXml);

                oXml.IntoElem();

                oXml.AddChildElement("Port", m_strPort);
                oXml.AddChildElement("BaudRate", m_iBaudRate);

                oXml.OutOfElem();
            }

            public override void SaveSimulationXml(ManagedAnimatInterfaces.IStdXml oXml, ref DataObject nmParentControl, string strName = "")
            {
                base.SaveSimulationXml(oXml, ref nmParentControl, strName);

                oXml.IntoElem();

                oXml.AddChildElement("Port", m_strPort);
                oXml.AddChildElement("BaudRate", m_iBaudRate);

                oXml.OutOfElem();
            }

            #endregion

        }
    }
}
