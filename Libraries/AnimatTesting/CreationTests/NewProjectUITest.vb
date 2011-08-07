﻿Imports System.Drawing
Imports System.Text.RegularExpressions
Imports System.Windows.Forms
Imports System.Windows.Input
Imports Microsoft.VisualStudio.TestTools.UITest.Extension
Imports Microsoft.VisualStudio.TestTools.UITesting
Imports Microsoft.VisualStudio.TestTools.UITesting.Keyboard

Namespace CreationTests

    <CodedUITest()>
    Public Class NewProjectUITest
        Inherits AnimatUITest

        <TestMethod()>
        Public Sub TestCreateNewProject()

            'Start the application.
            StartApplication("", 8080)

            'Click the New Project button.
            ExecuteMethod("ClickToolbarItem", New Object() {"NewToolStripButton"})

            'Set params and hit ok button
            ExecuteActiveDialogMethod("SetProjectParams", New Object() {"TestProject", m_strRootFolder & "\Libraries\AnimatTesting\TestProjects\CreationTests"})
            ExecuteActiveDialogMethod("ClickOkButton", Nothing)

            'Click the add structure button.
            ExecuteMethod("ClickToolbarItem", New Object() {"AddStructureToolStripButton"})

            'Open the Structure_1 body plan editor window
            ExecuteMethod("DblClickWorkspaceItem", New Object() {"Simulation\Environment\Structures\Structure_1\Body Plan"}, 2000)

            'Set simulation to automatically end at a specified time.
            ExecuteMethod("SetObjectProperty", New Object() {"Simulation", "SetSimulationEnd", "True"})

            'Set simulation end time.
            ExecuteMethod("SetObjectProperty", New Object() {"Simulation", "SimulationEndTime", "50"})

            'Add a root cylinder part.
            AddRootPartType("Box")

            'Select the LineChart to add.
            AddChart("Line Chart")

            'Select the Chart axis
            ExecuteMethod("SelectWorkspaceItem", New Object() {"Tool Viewers\DataTool_1\LineChart\Y Axis 1"})

            'Add root to chart
            AddItemToChart("Structure_1\Root")

            'Set the name of the data chart item to root_y.
            ExecuteMethod("SetObjectProperty", New Object() {"Tool Viewers\DataTool_1\LineChart\Y Axis 1\Root", "Name", "Root_Y"})

            'Change the data type to track the world Y position.
            ExecuteMethod("SetObjectProperty", New Object() {"Tool Viewers\DataTool_1\LineChart\Y Axis 1\Root_Y", "DataTypeID", "WorldPositionY"})

            'Change the end time of the data chart to 45 seconds.
            ExecuteMethod("SetObjectProperty", New Object() {"Tool Viewers\DataTool_1\LineChart", "CollectEndTime", "45"})

            'Run the simulation and wait for it to end.
            RunSimulationWaitToEnd()

            'Compare chart data to verify simulation results.
            CompareSimulation(m_strRootFolder & "\Libraries\AnimatTesting\TestData\CreationTests\NewProjectUITest\")

            'Save the project
            ExecuteMethod("ClickToolbarItem", New Object() {"SaveToolStripButton"})

            'Now verify that if we try and create a new project at the same location we get an error.
            'Click the New Project button.
            ExecuteMethod("ClickToolbarItem", New Object() {"NewToolStripButton"})

            'Enter text and verify error. Verify the error.
            ExecuteActiveDialogMethod("SetProjectParams", New Object() {"TestProject", m_strRootFolder & "\Libraries\AnimatTesting\TestProjects\CreationTests"})
            ExecuteActiveDialogMethod("ClickOkButton", Nothing)

        End Sub

#Region "Additional test attributes"
        '
        ' You can use the following additional attributes as you write your tests:
        '
        ' Use TestInitialize to run code before running each test
        <TestInitialize()> Public Overrides Sub MyTestInitialize()
            MyBase.MyTestInitialize()

            'Make sure any left over project directory is cleaned up before starting the test.
            DeleteDirectory(m_strRootFolder & "\Libraries\AnimatTesting\TestProjects\CreationTests\TestProject")
        End Sub

        ' Use TestCleanup to run code after each test has run
        <TestCleanup()> Public Overrides Sub MyTestCleanup()
            MyBase.MyTestCleanup()

            DeleteDirectory(m_strRootFolder & "\Libraries\AnimatTesting\TestProjects\CreationTests\TestProject")
        End Sub

#End Region

    End Class

End Namespace
