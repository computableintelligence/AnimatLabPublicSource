﻿Imports System.Windows.Forms
Imports Microsoft.VisualStudio.TestTools.UITesting.Keyboard
Imports System.Runtime.Remoting
Imports System.Runtime.Remoting.Channels
Imports System.Runtime.Remoting.Channels.Tcp
Imports System
Imports System.CodeDom.Compiler
Imports System.Configuration
Imports System.Collections.Generic
Imports System.Drawing
Imports System.Text.RegularExpressions
Imports System.IO
Imports System.Windows.Input
Imports Microsoft.VisualStudio.TestTools.UITest.Extension
Imports Microsoft.VisualStudio.TestTools.UITesting
Imports Microsoft.VisualStudio.TestTools.UITesting.WinControls
Imports Microsoft.VisualStudio.TestTools.UnitTesting
Imports Keyboard = Microsoft.VisualStudio.TestTools.UITesting.Keyboard
Imports Mouse = Microsoft.VisualStudio.TestTools.UITesting.Mouse
Imports MouseButtons = System.Windows.Forms.MouseButtons
Imports AnimatTesting.Framework
Imports System.Xml

Namespace UITests
    Namespace ConversionTests
        Namespace TutorialTests

            <CodedUITest()>
            Public Class ExamplesConversionUITest
                Inherits ConversionUITest

#Region "Attributes"


#End Region

#Region "Properties"

#End Region

#Region "Methods"

                <TestMethod()>
                Public Sub Test_Hexapod()

                    Dim aryMaxErrors As New Hashtable
                    aryMaxErrors.Add("Time", 0.001)
                    aryMaxErrors.Add("X", 0.001)
                    aryMaxErrors.Add("Y", 0.001)
                    aryMaxErrors.Add("Z", 0.001)
                    aryMaxErrors.Add("default", 0.001)

                    m_strProjectName = "Hexapod"
                    m_strProjectPath = "\Libraries\AnimatTesting\TestProjects\ConversionTests\TutorialTests"
                    m_strTestDataPath = "\Libraries\AnimatTesting\TestData\ConversionTests\TutorialTests\" & m_strProjectName
                    m_strOldProjectFolder = "\Libraries\AnimatTesting\TestProjects\ConversionTests\OldVersions\TutorialTests\" & m_strProjectName
                    m_strStructureGroup = "Organisms"
                    m_strStruct1Name = "Organism_1"

                    m_aryWindowsToOpen.Clear()
                    m_aryWindowsToOpen.Add("Tool Viewers\Turn Data")

                    'Load and convert the project.
                    TestConversionProject("AfterConversion_", aryMaxErrors)

                End Sub

                <TestMethod()>
                Public Sub Test_BellyFlopper()

                    Dim aryMaxErrors As New Hashtable
                    aryMaxErrors.Add("Time", 0.001)
                    aryMaxErrors.Add("BodyX", 0.001)
                    aryMaxErrors.Add("BodyY", 0.001)
                    aryMaxErrors.Add("BodyZ", 0.001)
                    aryMaxErrors.Add("default", 0.001)

                    m_strProjectName = "Belly Flopper"
                    m_strProjectPath = "\Libraries\AnimatTesting\TestProjects\ConversionTests\TutorialTests\Examples"
                    m_strTestDataPath = "\Libraries\AnimatTesting\TestData\ConversionTests\TutorialTests\Examples\" & m_strProjectName
                    m_strOldProjectFolder = "\Libraries\AnimatTesting\TestProjects\ConversionTests\OldVersions\TutorialTests\Examples\" & m_strProjectName
                    m_strStructureGroup = "Organisms"
                    m_strStruct1Name = "Organism_1"

                    m_aryWindowsToOpen.Clear()
                    m_aryWindowsToOpen.Add("Tool Viewers\BodyData")

                    'Load and convert the project.
                    TestConversionProject("AfterConversion_", aryMaxErrors)

                End Sub

#Region "Additional test attributes"
                '
                ' You can use the following additional attributes as you write your tests:
                '
                ' Use TestInitialize to run code before running each test
                <TestInitialize()> Public Overrides Sub MyTestInitialize()
                    MyBase.MyTestInitialize()

                    'This conversion is different than others. The contact collisions are generated differently than before, so I cannot
                    'use the old version data as a template. I will check the output to make sure it is similar, but use the new version as the template.
                    'm_bGenerateTempates = False

                End Sub

                <TestCleanup()> Public Overrides Sub MyTestCleanup()
                    MyBase.MyTestCleanup()
                End Sub
#End Region

#End Region

            End Class

        End Namespace
    End Namespace
End Namespace