Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports AnimatGuiCtrls.Controls
Imports AnimatGUI.Framework

Namespace DataObjects.Physical

    Public MustInherit Class BodyPart
        Inherits MovableItem

#Region " Delegates "

#End Region

#Region " Attributes "

        Protected m_bSynchWithRobot As Boolean = False
        Protected m_snSynchUpdateInterval As ScaledNumber

#End Region

#Region " Properties "

        'Type tells what type of bodypart (hinge, box, etc..
        Public MustOverride ReadOnly Property Type() As String

        'BodyPartType tells if it is a rigidbody or a joint.
        Public Overridable ReadOnly Property BodyPartType() As String
            Get
                Return Me.PartType.ToString
            End Get
        End Property

        'BodyPartName is a descriptive name used in the UI that tells the type of part.
        Public Overridable ReadOnly Property BodyPartName() As String
            Get
                Return Me.Type
            End Get
        End Property

        'AssemblyClass tells if it is a rigidbody or a joint.
        Public MustOverride ReadOnly Property PartType() As System.Type

        <Browsable(False)> _
        Public Overridable ReadOnly Property ParentStructure() As AnimatGUI.DataObjects.Physical.PhysicalStructure
            Get
                Dim doParent As AnimatGUI.DataObjects.Physical.PhysicalStructure
                Dim doTemp As AnimatGUI.DataObjects.Physical.BodyPart

                If Not Me.Parent Is Nothing AndAlso (TypeOf Me.Parent Is Physical.BodyPart OrElse TypeOf Me.Parent Is Physical.PhysicalStructure) Then
                    If TypeOf Me.Parent Is Physical.BodyPart Then
                        doTemp = DirectCast(Me.Parent, Physical.BodyPart)
                        Return doTemp.ParentStructure
                    ElseIf TypeOf Me.Parent Is Physical.PhysicalStructure Then
                        doParent = DirectCast(Me.Parent, Physical.PhysicalStructure)
                        Return doParent
                    End If
                End If

                Return Nothing
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property DragImageName() As String
            Get
                Return WorkspaceImageName()
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property StructureID() As String
            Get
                If Not Me.ParentStructure Is Nothing Then
                    Return Me.ParentStructure.ID
                End If

                Return ""
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property TotalSubChildren() As Integer
            Get
                Return 0
            End Get
        End Property

        Public Overridable Property SynchWithRobot() As Boolean
            Get
                Return m_bSynchWithRobot
            End Get
            Set(value As Boolean)
                SetSimData("SynchWithRobot", value.ToString, True)
                m_bSynchWithRobot = value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable Property SynchUpdateInterval() As ScaledNumber
            Get
                Return m_snSynchUpdateInterval
            End Get
            Set(ByVal Value As ScaledNumber)
                If Value.ActualValue < 0 OrElse Value.ActualValue > 5 Then
                    Throw New System.Exception("The synch update interval must be between the range 0 to 5 s.")
                End If

                SetSimData("SynchUpdateInterval", Value.ActualValue.ToString, True)
                m_snSynchUpdateInterval.CopyData(Value)
            End Set
        End Property

#End Region

#Region " Methods "

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            m_snSynchUpdateInterval = New AnimatGUI.Framework.ScaledNumber(Me, "SynchUpdateInterval", 0, AnimatGUI.Framework.ScaledNumber.enumNumericScale.milli, "seconds", "s")
        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()
            If Not m_thDataTypes Is Nothing Then m_thDataTypes.ClearIsDirty()
            If Not m_snSynchUpdateInterval Is Nothing Then m_snSynchUpdateInterval.ClearIsDirty()
        End Sub

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatGUI.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatGUI.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnOrig As BodyPart = DirectCast(doOriginal, BodyPart)

            m_bSynchWithRobot = bnOrig.m_bSynchWithRobot
            m_snSynchUpdateInterval = DirectCast(bnOrig.m_snSynchUpdateInterval.Clone(Me, bCutData, doRoot), ScaledNumber)
        End Sub

        'This method is called for each part type during the catalog of the modules. It sets up the 
        'list of part types that cannot be added to one another in the simulation object.
        Public Overridable Sub SetupPartTypesExclusions()
            Util.Application.AddPartTypeExclusion(GetType(Bodies.FluidPlane), Me.GetType)
            Util.Application.AddPartTypeExclusion(GetType(Bodies.Attachment), Me.GetType)
            Util.Application.AddPartTypeExclusion(GetType(Bodies.LinearHillMuscle), Me.GetType)
            Util.Application.AddPartTypeExclusion(GetType(Bodies.LinearHillStretchReceptor), Me.GetType)
            Util.Application.AddPartTypeExclusion(GetType(Bodies.Mouth), Me.GetType)
            Util.Application.AddPartTypeExclusion(GetType(Bodies.OdorSensor), Me.GetType)
            Util.Application.AddPartTypeExclusion(GetType(Bodies.Stomach), Me.GetType)
            Util.Application.AddPartTypeExclusion(GetType(Bodies.Spring), Me.GetType)
        End Sub

        Public Overrides Sub BuildProperties(ByRef propTable As AnimatGuiCtrls.Controls.PropertyTable)
            MyBase.BuildProperties(propTable)

            ''Only show this stuff if there is a robot interface defined for it.
            'If Not Me.ParentStructure Is Nothing AndAlso Util.IsTypeOf(Me.ParentStructure.GetType, GetType(Organism), False) Then
            '    Dim doOrg As Organism = DirectCast(Me.ParentStructure, Organism)

            '    If Not doOrg Is Nothing AndAlso Not doOrg.RobotInterface Is Nothing Then
            '        propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("SynchWithRobot", GetType(Boolean), "SynchWithRobot", _
            '                                    "Robot Properties", "Determines whether this part is synched with a robot part during simulation." & _
            '                                    "Note that this will only be applied if the synch with robot setting on the robot interface is true also.", m_bSynchWithRobot))

            '        Dim pbNumberBag As AnimatGuiCtrls.Controls.PropertyBag = m_snSynchUpdateInterval.Properties
            '        propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Synch Update Interval", pbNumberBag.GetType(), "SynchUpdateInterval", _
            '                                    "Robot Properties", "Sets how often this part is updated when simulating a robot. " & _
            '                                    "Acceptable values are in the range 0 to 5 s.", pbNumberBag, _
            '                                    "", GetType(AnimatGUI.Framework.ScaledNumber.ScaledNumericPropBagConverter)))

            '    End If
            'End If

        End Sub

        Public Overridable Function FindBodyPart(ByVal strID As String) As BodyPart
            Return Nothing
        End Function

        Public Overridable Function FindBodyPartByName(ByVal strName As String) As BodyPart
            Return Nothing
        End Function

        Public Overridable Function FindBodyPartByCloneID(ByVal strId As String) As BodyPart
            Return Nothing
        End Function

        Public Overrides Function FindDragObject(ByVal strStructureName As String, ByVal strDataItemID As String, Optional ByVal bThrowError As Boolean = True) As DataObjects.DragObject

            Dim oStructure As Object = Util.Environment.FindStructureFromAll(strStructureName, bThrowError)
            If oStructure Is Nothing Then Return Nothing

            Dim doStructure As AnimatGUI.DataObjects.Physical.PhysicalStructure = DirectCast(oStructure, AnimatGUI.DataObjects.Physical.PhysicalStructure)
            Dim doPart As AnimatGUI.DataObjects.Physical.BodyPart = Nothing

            If Not doStructure Is Nothing Then
                doPart = doStructure.FindBodyPart(strDataItemID, False)
                If doPart Is Nothing AndAlso bThrowError Then
                    Throw New System.Exception("The drag object with id '" & strDataItemID & "' was not found.")
                End If
            End If

            Return doPart

        End Function

        Public Overridable Function CreateJointTreeView(ByRef tvTree As Crownwood.DotNetMagic.Controls.TreeControl, ByVal tnParent As Crownwood.DotNetMagic.Controls.Node, _
                                                        ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As Crownwood.DotNetMagic.Controls.Node
            Return Nothing
        End Function

        Public Overridable Function CreateRigidBodyTreeView(ByRef tvTree As Crownwood.DotNetMagic.Controls.TreeControl, ByVal tnParent As Crownwood.DotNetMagic.Controls.Node, _
                                                           ByVal thSelectedPart As TypeHelpers.LinkedBodyPart) As Crownwood.DotNetMagic.Controls.Node
            Return Nothing
        End Function

        Public Overrides Function Delete(Optional ByVal bAskToDelete As Boolean = True, Optional ByVal e As Crownwood.DotNetMagic.Controls.TGCloseRequestEventArgs = Nothing) As Boolean

            Try
                If bAskToDelete Then
                    If Util.ShowMessage("Are you certain that you want to permanently delete this " & _
                                        "body part and all its children?", "Delete Body Part", MessageBoxButtons.YesNo) <> DialogResult.Yes Then
                        Return True
                    End If
                End If

                Util.Application.AppIsBusy = True
                If Not Me.ParentStructure Is Nothing Then
                    If Not m_doInterface Is Nothing Then
                        RemoveHandler m_doInterface.OnPositionChanged, AddressOf Me.OnPositionChanged
                        RemoveHandler m_doInterface.OnRotationChanged, AddressOf Me.OnRotationChanged
                        RemoveHandler m_doInterface.OnSizeChanged, AddressOf Me.OnSizeChanged
                        RemoveHandler m_doInterface.OnSelectionChanged, AddressOf Me.OnSelectionChanged
                    End If

                    Me.ParentStructure.DeleteBodyPart(Me)

                    If Not Me.ParentStructure Is Nothing Then
                        Me.ParentStructure.SelectItem()
                    End If
                End If

                Return False
            Catch ex As Exception
                Throw ex
            Finally
                Util.Application.AppIsBusy = False
            End Try
        End Function

        Public Overridable Sub CopyBodyPart()

            Dim doClone As AnimatGUI.DataObjects.Physical.RigidBody = DirectCast(Me.Clone(Me.Parent, False, Me), AnimatGUI.DataObjects.Physical.RigidBody)

            Dim oXml As ManagedAnimatInterfaces.IStdXml = Util.Application.CreateStdXml()

            SaveSelected(oXml)

            'oXml.Save("C:\Projects\bin\Experiments\Copy.txt")
            Dim strXml As String = oXml.Serialize()

            Dim data As New System.Windows.Forms.DataObject
            data.SetData("AnimatLab.BodyPlan.XMLFormat", strXml)
            Clipboard.SetDataObject(data, True)

        End Sub

        Public Overridable Function SaveSelected(ByVal oXml As ManagedAnimatInterfaces.IStdXml) As Boolean

            oXml.AddElement("BodyPlan")

            'First lets sort the selected items into nodes and links and generate temp selected ids
            Dim aryReplaceIDs As New ArrayList

            Dim aryItems As New ArrayList
            Dim arySelected As Collections.DataObjects = Util.ProjectWorkspace.SelectedDataObjects
            For Each dobj As Framework.DataObject In arySelected
                aryItems.Add(dobj)
            Next

            'Call BeforeCopy first
            BeforeCopy(aryItems)

            'We are regenerating this list because BeforeCopy can cause some items to be deselected, or others to be added.
            aryItems.Clear()
            arySelected = Util.ProjectWorkspace.SelectedDataObjects
            For Each dobj As Framework.DataObject In arySelected
                aryItems.Add(dobj)
            Next

            Me.AddToReplaceIDList(aryReplaceIDs, aryItems)

            'Save the replaceme ID list
            oXml.AddChildElement("ReplaceIDList")
            oXml.IntoElem() 'Into ReplaceIDList Element
            For Each strID As String In aryReplaceIDs
                oXml.AddChildElement("ID", strID)
            Next
            oXml.OutOfElem() 'Outof ReplaceIDList Element

            SaveData(ParentStructure, oXml)

            AfterCopy()

            Return True
        End Function

        Public Overridable Sub VerifyCanBePasted()
        End Sub

        'This is called when creating a new body part. It sets the size of the part to its defaults.
        Public Overridable Sub SetDefaultSizes()

        End Sub

        Protected Overridable Function CreateWorkspaceTreeViewPopupMenu(ByRef tnSelectedNode As Crownwood.DotNetMagic.Controls.Node, ByVal ptPoint As System.Drawing.Point) As AnimatContextMenuStrip
            Dim popup As New AnimatContextMenuStrip("AnimatGUI.DataObjects.Physical.BodyPart.WorkspaceTreeviewPopupMenu", Util.SecurityMgr)

            If Me.AllowStimulus AndAlso Me.CompatibleStimuli.Count > 0 Then
                ' Create the menu items
                Dim mcAddStimulus As New System.Windows.Forms.ToolStripMenuItem("Add Stimulus", Util.Application.ToolStripImages.GetImage("AnimatGUI.AddStimulus.gif"), New EventHandler(AddressOf Me.OnAddStimulus))
                popup.Items.Add(mcAddStimulus)
            End If

            If Me.CanBeCharted AndAlso Not Util.Application.LastSelectedChart Is Nothing AndAlso Not Util.Application.LastSelectedChart.LastSelectedAxis Is Nothing Then
                ' Create the menu items
                Dim mcAddToChart As New System.Windows.Forms.ToolStripMenuItem("Add to Chart", Util.Application.ToolStripImages.GetImage("AnimatGUI.AddChartItem.gif"), New EventHandler(AddressOf Util.Application.OnAddToChart))
                popup.Items.Add(mcAddToChart)
            End If

            'Dim mcSwapPart As New System.Windows.Forms.ToolStripMenuItem("Swap Part", Util.Application.ToolStripImages.GetImage("AnimatGUI.Swap.gif"), New EventHandler(AddressOf Me.OnSwapBodyPart))
            'popup.Items.Add(mcSwapPart)

            Dim mcCut As New System.Windows.Forms.ToolStripMenuItem("Cut", Util.Application.ToolStripImages.GetImage("AnimatGUI.Cut.gif"), New EventHandler(AddressOf Me.OnCutBodyPart))
            Dim mcCopy As New System.Windows.Forms.ToolStripMenuItem("Copy", Util.Application.ToolStripImages.GetImage("AnimatGUI.Copy.gif"), New EventHandler(AddressOf Me.OnCopyBodyPart))
            Dim mcDelete As New System.Windows.Forms.ToolStripMenuItem("Delete Part", Util.Application.ToolStripImages.GetImage("AnimatGUI.Delete.gif"), New EventHandler(AddressOf Util.Application.OnDeleteFromWorkspace))
            popup.Items.AddRange(New System.Windows.Forms.ToolStripItem() {mcCut, mcCopy, mcDelete})

            If Not Me.ParentStructure Is Nothing Then
                Dim mcRelabel As New System.Windows.Forms.ToolStripMenuItem("Relabel Children", Util.Application.ToolStripImages.GetImage("AnimatGUI.Relabel.gif"), New EventHandler(AddressOf Me.OnRelabelChildren))
                popup.Items.Add(mcRelabel)
            End If

            Dim mcSepExpand As New ToolStripSeparator()
            Dim mcExpandAll As New System.Windows.Forms.ToolStripMenuItem("Expand All", Util.Application.ToolStripImages.GetImage("AnimatGUI.Expand.gif"), New EventHandler(AddressOf Me.OnExpandAll))
            Dim mcCollapseAll As New System.Windows.Forms.ToolStripMenuItem("Collapse All", Util.Application.ToolStripImages.GetImage("AnimatGUI.Collapse.gif"), New EventHandler(AddressOf Me.OnCollapseAll))

            mcExpandAll.Tag = tnSelectedNode
            mcCollapseAll.Tag = tnSelectedNode

            ' Create the popup menu object
            popup.Items.AddRange(New System.Windows.Forms.ToolStripItem() {mcSepExpand, mcExpandAll, mcCollapseAll})

            Return popup
        End Function

        Public Overrides Function WorkspaceTreeviewPopupMenu(ByRef tnSelectedNode As Crownwood.DotNetMagic.Controls.Node, ByVal ptPoint As System.Drawing.Point) As Boolean

            If tnSelectedNode Is m_tnWorkspaceNode Then
                Dim popup As AnimatContextMenuStrip = CreateWorkspaceTreeViewPopupMenu(tnSelectedNode, ptPoint)
                Util.ProjectWorkspace.ctrlTreeView.ContextMenuNode = popup
                Return True
            End If

            Return False
        End Function

        Public Overrides Sub SelectStimulusType()
            Dim frmStimulusType As New Forms.ExternalStimuli.SelectStimulusType
            frmStimulusType.CompatibleStimuli = Me.CompatibleStimuli

            If frmStimulusType.ShowDialog(Util.Application) = DialogResult.OK Then
                Dim doStimulus As DataObjects.ExternalStimuli.Stimulus = DirectCast(frmStimulusType.SelectedStimulus.Clone(Util.Application.FormHelper, False, Nothing), DataObjects.ExternalStimuli.Stimulus)
                doStimulus.StimulatedItem = Me

                Util.Simulation.NewStimuliIndex = Util.Simulation.NewStimuliIndex + 1
                doStimulus.Name = "Stimulus_" & Util.Simulation.NewStimuliIndex

                Util.Simulation.ProjectStimuli.Add(doStimulus.ID, doStimulus)

                If Not doStimulus.WorkspaceNode Is Nothing Then
                    doStimulus.SelectItem()
                End If
            End If
        End Sub

        Public MustOverride Sub RenameBodyParts(ByVal doStructure As AnimatGUI.DataObjects.Physical.PhysicalStructure)

        Public Overridable Sub ClearSelectedBodyParts()
            Me.DeselectItem()
        End Sub

        Public Overridable Overloads Sub LoadData(ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByVal oXml As ManagedAnimatInterfaces.IStdXml)
            MyBase.LoadData(oXml)

            oXml.IntoElem() 'Into BodyPart Element

            m_bSynchWithRobot = oXml.GetChildBool("SynchWithRobot", m_bSynchWithRobot)
            m_snSynchUpdateInterval.LoadData(oXml, "SynchUpdateInterval", False)

            oXml.OutOfElem() 'Outof BodyPart Element

        End Sub

        Public Overridable Overloads Sub SaveData(ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByVal oXml As ManagedAnimatInterfaces.IStdXml)
            MyBase.SaveData(oXml, Me.BodyPartType)

            oXml.IntoElem() 'Into Child Elemement

            oXml.AddChildElement("Type", Me.Type)
            oXml.AddChildElement("PartType", Me.PartType.ToString)

            oXml.AddChildElement("SynchWithRobot", m_bSynchWithRobot)
            m_snSynchUpdateInterval.SaveData(oXml, "SynchUpdateInterval")

            oXml.OutOfElem() 'Outof BodyPart Element

        End Sub

        Public Overrides Sub SaveSimulationXml(ByVal oXml As ManagedAnimatInterfaces.IStdXml, Optional ByRef nmParentControl As Framework.DataObject = Nothing, Optional ByVal strName As String = "")
            MyBase.SaveSimulationXml(oXml, nmParentControl, Me.BodyPartType)

            oXml.IntoElem() 'Into Child Elemement

            oXml.AddChildElement("Type", Me.Type)
            oXml.AddChildElement("PartType", Me.PartType.ToString)

            oXml.AddChildElement("SynchWithRobot", m_bSynchWithRobot)
            m_snSynchUpdateInterval.SaveSimulationXml(oXml, Me, "SynchUpdateInterval")

            oXml.OutOfElem() 'Outof BodyPart Element
        End Sub

        Public Overrides Sub EnsureFormActive()
            If Not Me.ParentStructure Is Nothing AndAlso Me.ParentStructure.BodyEditor Is Nothing Then
                'Return Util.Application.EditBodyPlan(Me.ParentStructure)
            End If
        End Sub

        Public Overridable Sub BeforeAddBody()
            If Not Util.Application Is Nothing Then
                Util.Application.SignalBeforeAddBody(Me)
            End If
        End Sub

        Public Overridable Sub AfterAddBody()
            If Not Util.Application Is Nothing Then
                Util.Application.SignalAfterAddBody(Me)
            End If
        End Sub

        Public MustOverride Function SwapBodyPartList() As AnimatGUI.Collections.BodyParts
        Public MustOverride Sub SwapBodyPartCopy(ByVal doOriginal As AnimatGUI.DataObjects.Physical.BodyPart)

#End Region

#Region " Events "

        Protected Overridable Sub OnAddStimulus(ByVal sender As Object, ByVal e As System.EventArgs)

            Try

                Me.SelectStimulusType()

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        'Protected Overridable Sub OnAddBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)
        'End Sub

        Protected Overridable Sub OnRelabelChildren(ByVal sender As Object, ByVal e As System.EventArgs)
            Try
                Dim frmRelabel As New AnimatGUI.Forms.BodyPlan.Relabel

                frmRelabel.SelectedItem = Me
                frmRelabel.RootNode = Me
                frmRelabel.ShowDialog()

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnSwapBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                Dim frmSwap As New Forms.BodyPlan.SwapParts
                frmSwap.ExistingPart = Me
                frmSwap.PartList = Me.SwapBodyPartList

                If frmSwap.ShowDialog() = DialogResult.OK Then
                    Dim doPart As BodyPart = frmSwap.NewPart
                    doPart.SwapBodyPartCopy(Me)
                End If

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnCopyBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                CopySelected()
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Sub OnCutBodyPart(ByVal sender As Object, ByVal e As System.EventArgs)

            Try
                CutSelected(sender)
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub


#Region " Copy/Paste Methods "

        Public Shared Sub CutSelected(ByVal sender As Object)

            Try
                CopySelected()
                Util.Application.OnDeleteFromWorkspace(sender, Nothing)

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Shared Sub CopySelected()

            Try
                If Util.ProjectWorkspace.TreeView.SelectedCount > 1 Then
                    Throw New System.Exception("Only one body part can be selected to be copied at one time.")
                End If

                If Util.ProjectWorkspace.SelectedDataObject Is Nothing Then
                    Throw New System.Exception("You must select at least one body part to be copied.")
                End If

                If Not Util.IsTypeOf(Util.ProjectWorkspace.SelectedDataObject.GetType, GetType(DataObjects.Physical.RigidBody)) Then
                    Throw New System.Exception("The selected item must be a body part type in order to be copied.")
                End If

                Dim rbPart As DataObjects.Physical.RigidBody = DirectCast(Util.ProjectWorkspace.SelectedDataObject, DataObjects.Physical.RigidBody)

                If Not rbPart.CanCopy(New ArrayList) Then
                    Throw New System.Exception("This part cannot be copied.")
                End If

                rbPart.CopyBodyPart()

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        'Public Overridable Function SaveSelected(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal rbPart As DataObjects.Physical.RigidBody) As Boolean

        '    oXml.AddElement("BodyPlan")

        '    'First lets sort the selected items into nodes and links and generate temp selected ids
        '    Dim aryReplaceIDs As New ArrayList

        '    'Call BeforeCopy first
        '    rbPart.BeforeCopy()

        '    rbPart.AddToReplaceIDList(aryReplaceIDs)

        '    'Save the replaceme ID list
        '    oXml.AddChildElement("ReplaceIDList")
        '    oXml.IntoElem() 'Into ReplaceIDList Element
        '    For Each strID As String In aryReplaceIDs
        '        oXml.AddChildElement("ID", strID)
        '    Next
        '    oXml.OutOfElem() 'Outof ReplaceIDList Element

        '    rbPart.SaveData(rbPart.ParentStructure, oXml)

        '    rbPart.AfterCopy()

        '    Return True
        'End Function

        Public Shared Sub PasteSelected(ByVal bInPlace As Boolean)

            Try
                Util.Application.ToggleBodyPartPasteInProgress()
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

#End Region


#End Region

    End Class

End Namespace
