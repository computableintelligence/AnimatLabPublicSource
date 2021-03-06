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

Namespace DataObjects.Behavior.Nodes

    Public Class Subsystem
        Inherits Behavior.Node

#Region " Attributes "

        Protected m_bdSubsystemDiagram As Forms.Behavior.Diagram

        '''This is a list of all nodes within this subsystem. It is sorted by ID
        Protected m_aryBehavioralNodes As New Collections.SortedNodeList(Me)

        '''This is a list of all links within this subsystem. It is sorted by ID
        Protected m_aryBehavioralLinks As New Collections.SortedLinkList(Me)

        ''' The xml string that defines the layout of the diagram for this subsystem.
        Protected m_strDiagramXml As String = ""

#End Region

#Region " Properties "

        <Browsable(False)> _
        Public Overrides ReadOnly Property TypeName() As String
            Get
                Return "Subsystem"
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property SubsystemDiagram() As Forms.Behavior.Diagram
            Get
                Return m_bdSubsystemDiagram
            End Get
            Set(ByVal Value As Forms.Behavior.Diagram)
                m_bdSubsystemDiagram = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property BehavioralNodes() As Collections.SortedNodeList
            Get
                Return m_aryBehavioralNodes
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable ReadOnly Property BehavioralLinks() As Collections.SortedLinkList
            Get
                Return m_aryBehavioralLinks
            End Get
        End Property

        Public Overrides Property Text() As String
            Get
                Return m_strText
            End Get
            Set(ByVal Value As String)
                If Value.Trim.Length > 0 Then
                    m_strText = Value
                    UpdateChart()
                    If Not Me.WorkspaceNode Is Nothing Then
                        Me.WorkspaceNode.Text = m_strText
                    End If
                Else
                    'If the text is set to blank then
                    'keep it the current value and reupdate
                    'the chart to put the text back.
                    UpdateChart(True)
                End If
                CheckForErrors()
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property NeuralModuleType() As System.Type
            Get
                Return Nothing
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property WorkspaceImageName() As String
            Get
                Return "AnimatGUI.SubsystemNode_Treeview.gif"
            End Get
        End Property

        <Browsable(False)> _
        Public Overrides ReadOnly Property AllowStimulus() As Boolean
            Get
                Return False
            End Get
        End Property

        <Browsable(False)> _
        Public Overridable Property DiagramXml() As String
            Get
                Return m_strDiagramXml
            End Get
            Set(ByVal Value As String)
                m_strDiagramXml = Value
            End Set
        End Property

        <Browsable(False)> _
        Public Overrides Property IsInitialized() As Boolean
            Get
                If m_bIsInitialized AndAlso Me.AllChildrenInitialized() Then
                    Return True
                Else
                    Return False
                End If
            End Get
            Set(ByVal Value As Boolean)
                m_bIsInitialized = Value
            End Set
        End Property

        Public Overridable ReadOnly Property SubsystemNodeCount() As Integer
            Get
                Return m_aryBehavioralNodes.Count
            End Get
        End Property

        Public Overridable ReadOnly Property SubsystemLinkCount() As Integer
            Get
                Return m_aryBehavioralLinks.Count
            End Get
        End Property

        Public Overridable ReadOnly Property TotalNodeCount() As Integer
            Get
                Dim iNodes As Integer = Me.BehavioralNodes.Count

                Dim arySubSystems As New AnimatGUI.Collections.DataObjects(Nothing)
                Me.FindChildrenOfType(Me.GetType, arySubSystems)

                For Each doObj As Framework.DataObject In arySubSystems
                    Dim doSub As Subsystem = DirectCast(doObj, Subsystem)

                    If Not doSub Is Me Then
                        iNodes = iNodes + doSub.TotalNodeCount
                    End If
                Next

                Return iNodes
            End Get
        End Property

        Public Overridable ReadOnly Property TotalLinkCount() As Integer
            Get
                Dim iLinks As Integer = Me.BehavioralLinks.Count

                Dim arySubSystems As New AnimatGUI.Collections.DataObjects(Nothing)
                Me.FindChildrenOfType(Me.GetType, arySubSystems)

                For Each doObj As Framework.DataObject In arySubSystems
                    Dim doSub As Subsystem = DirectCast(doObj, Subsystem)

                    If Not doSub Is Me Then
                        iLinks = iLinks + doSub.TotalLinkCount
                    End If
                Next

                Return iLinks
            End Get
        End Property

#End Region

#Region " Methods "


        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)

            Try

                Shape = Behavior.Node.enumShape.Rectangle
                Size = New SizeF(40, 40)
                Me.DrawColor = Color.Black
                Me.FillColor = Color.LawnGreen

                Dim myAssembly As System.Reflection.Assembly
                myAssembly = System.Reflection.Assembly.Load("AnimatGUI")

                Me.WorkspaceImage = AnimatGUI.Framework.ImageManager.LoadImage(myAssembly, "AnimatGUI.SubsystemNode.gif")
                Me.Name = "Neural Subsystem"

                Me.Font = New Font("Arial", 12, FontStyle.Bold)
                Me.Description = "Allows the user to build a hierarchachal neural system. This node expands into a new diagram."

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Public Overrides Function Clone(ByVal doParent As AnimatGUI.Framework.DataObject, ByVal bCutData As Boolean, _
                                        ByVal doRoot As AnimatGUI.Framework.DataObject) As AnimatGUI.Framework.DataObject
            Dim oNewNode As New Behavior.Nodes.Subsystem(doParent)
            oNewNode.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewNode.AfterClone(Me, bCutData, doRoot, oNewNode)
            Return oNewNode
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatGUI.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatGUI.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim bnOrig As Subsystem = DirectCast(doOriginal, Subsystem)

            m_aryBehavioralNodes = DirectCast(bnOrig.m_aryBehavioralNodes.Clone(Me, bCutData, doRoot), AnimatGUI.Collections.SortedNodeList)
            m_aryBehavioralLinks = DirectCast(bnOrig.m_aryBehavioralLinks.Clone(Me, bCutData, doRoot), AnimatGUI.Collections.SortedLinkList)
            m_strDiagramXml = bnOrig.m_strDiagramXml

        End Sub

        Public Overrides Sub BeforeRemoveNode()
            MyBase.BeforeRemoveNode()

            Dim aryList As New ArrayList
            Dim doObj As Framework.DataObject
            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doObj = DirectCast(deEntry.Value, Framework.DataObject)
                aryList.Add(doObj)
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doObj = DirectCast(deEntry.Value, Framework.DataObject)
                aryList.Add(doObj)
            Next

            For Each doObj In aryList
                doObj.Delete(False)
            Next
        End Sub

        Public Overrides Sub AfterRemoveNode()

            If Not m_bdSubsystemDiagram Is Nothing Then
                Util.Application.RemoveChildForm(m_bdSubsystemDiagram)
            End If

            MyBase.AfterRemoveNode()
        End Sub

        Public Overrides Sub AfterUndoRemove()

            If Not m_bdSubsystemDiagram Is Nothing Then
                'TODO
                'm_ParentDiagram.RestoreDiagram(m_bdSubsystem)

                ''m_ParentEditor.SelectedDiagram(m_ParentDiagram)
                'm_ParentDiagram.SelectDataItem(Me)
            End If

            MyBase.AfterUndoRemove()
        End Sub

        Public Overrides Sub AfterRedoRemove()

            'TODO
            'If Not m_bdSubsystem Is Nothing Then
            '    m_ParentDiagram.RemoveDiagram(m_bdSubsystem)
            'End If

            MyBase.AfterRedoRemove()
        End Sub

        Public Overrides Function BeforeEdit(ByVal strNewText As String) As Boolean
            If strNewText.Trim.Length = 0 Then
                Return True
            Else
                Return False
            End If
        End Function

        Public Overrides Sub AfterEdit()
            If Not Me.SubsystemDiagram Is Nothing AndAlso Not Me.SubsystemDiagram.TabPage Is Nothing Then
                Me.SubsystemDiagram.TabPage.Title = Me.Name
            End If
        End Sub

        Public Overrides Sub DoubleClicked()
            If m_bdSubsystemDiagram Is Nothing Then
                Try
                    Util.Application.AppIsBusy = True
                    m_bdSubsystemDiagram = CreateDiagram()
                    m_bdSubsystemDiagram.Subsystem = Me
                    m_bdSubsystemDiagram.LoadDiagramXml(Me.DiagramXml)
                    Util.Application.AddChildForm(m_bdSubsystemDiagram)
                Catch ex As Exception
                    m_bdSubsystemDiagram = Nothing
                    Throw ex
                Finally
                    Util.Application.AppIsBusy = False
                End Try
            ElseIf Not m_bdSubsystemDiagram.TabPage Is Nothing Then
                m_bdSubsystemDiagram.TabPage.Selected = True
            End If
        End Sub

        Public Overrides Sub WorkspaceTreeviewDoubleClick(ByVal tnSelectedNode As Crownwood.DotNetMagic.Controls.Node)
            DoubleClicked()
        End Sub

        Protected Overridable Function CreateDiagram() As AnimatGUI.Forms.Behavior.Diagram
            Dim afDiagram As AnimatGUI.Forms.Behavior.Diagram = DirectCast(Util.Application.CreateForm("LicensedAnimatGUI.dll", _
                                                                      "LicensedAnimatGUI.Forms.Behavior.AddFlowDiagram", Me.Name, True), AnimatGUI.Forms.Behavior.Diagram)
            Return afDiagram
        End Function

        Public Overrides Sub CreateNodeTreeView(ByRef tvTree As Crownwood.DotNetMagic.Controls.TreeControl, ByVal aryNodes As Crownwood.DotNetMagic.Controls.NodeCollection)
            Dim tnNode As New Crownwood.DotNetMagic.Controls.Node(Me.Text)
            aryNodes.Add(tnNode)
            'We do NOT setup the Tag for the subsystem. We do not want to actually be able to select it.

            If Not tvTree.ImageList.Images.ContainsKey(Me.WorkspaceImageName) Then
                tvTree.ImageList.Images.Add(Me.WorkspaceImageName, Me.WorkspaceImage)
            End If
            tnNode.ImageIndex = tvTree.ImageList.Images.IndexOfKey(Me.WorkspaceImageName)

            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                Dim bnNode As Behavior.Node = DirectCast(deEntry.Value, Behavior.Node)
                bnNode.CreateNodeTreeView(tvTree, tnNode.Nodes)
            Next
        End Sub

        Public Overrides Sub CheckForErrors()

            If Util.Application.ProjectErrors Is Nothing Then Return

            If Me.Text Is Nothing OrElse Me.Text.Trim.Length = 0 Then
                If Not Util.Application.ProjectErrors.Errors.Contains(DiagramErrors.DataError.GenerateID(Me, DiagramError.enumErrorTypes.EmptyName)) Then
                    Dim deError As New DiagramErrors.DataError(Me, DiagramError.enumErrorLevel.Warning, DiagramError.enumErrorTypes.EmptyName, "A node has no name.")
                    Util.Application.ProjectErrors.Errors.Add(deError.ID, deError)
                End If
            Else
                If Util.Application.ProjectErrors.Errors.Contains(DiagramErrors.DataError.GenerateID(Me, DiagramError.enumErrorTypes.EmptyName)) Then
                    Util.Application.ProjectErrors.Errors.Remove(DiagramErrors.DataError.GenerateID(Me, DiagramError.enumErrorTypes.EmptyName))
                End If
            End If

        End Sub

        Public Overrides Sub CreateWorkspaceTreeView(ByVal doParent As Framework.DataObject, _
                                                       ByVal tnParentNode As Crownwood.DotNetMagic.Controls.Node, _
                                                       Optional ByVal bRootObject As Boolean = False)
            MyBase.CreateWorkspaceTreeView(doParent, tnParentNode, bRootObject)

            Dim doData As DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In m_aryBehavioralNodes
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                doData.AddWorkspaceTreeNode()
            Next

        End Sub

        Public Overrides Function CreateObjectListTreeView(ByVal doParent As Framework.DataObject, _
                                                       ByVal tnParentNode As Crownwood.DotNetMagic.Controls.Node, _
                                                       ByVal mgrImageList As AnimatGUI.Framework.ImageManager) As Crownwood.DotNetMagic.Controls.Node
            Dim tnNode As Crownwood.DotNetMagic.Controls.Node = MyBase.CreateObjectListTreeView(doParent, tnParentNode, mgrImageList)

            Dim doData As DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In m_aryBehavioralNodes
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                doData.CreateObjectListTreeView(Me, tnNode, mgrImageList)
            Next

            Return tnNode
        End Function

        Public Overrides Function CreateDataItemTreeView(ByVal frmDataItem As Forms.Tools.SelectDataItem, ByVal tnParent As Crownwood.DotNetMagic.Controls.Node, ByVal tpTemplatePartType As Type) As Crownwood.DotNetMagic.Controls.Node
            Dim tnSubSystem As Crownwood.DotNetMagic.Controls.Node = MyBase.CreateDataItemTreeView(frmDataItem, tnParent, tpTemplatePartType)

            Dim tnNodes As New Crownwood.DotNetMagic.Controls.Node("Nodes")
            tnSubSystem.Nodes.Add(tnNodes)
            tnNodes.ImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatGUI.DefaultObject.gif")
            tnNodes.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatGUI.DefaultObject.gif")

            Dim tnLinks As New Crownwood.DotNetMagic.Controls.Node("Links")
            tnSubSystem.Nodes.Add(tnLinks)
            tnLinks.ImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatGUI.DefaultLink.gif")
            tnLinks.SelectedImageIndex = frmDataItem.ImageManager.GetImageIndex("AnimatGUI.DefaultLink.gif")

            Dim doData As DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In m_aryBehavioralNodes
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                If doData.CanBeCharted Then
                    doData.CreateDataItemTreeView(frmDataItem, tnNodes, tpTemplatePartType)
                End If
            Next

            For Each deEntry As DictionaryEntry In m_aryBehavioralLinks
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                If doData.CanBeCharted Then
                    doData.CreateDataItemTreeView(frmDataItem, tnLinks, tpTemplatePartType)
                End If
            Next

        End Function

        Public Overrides Sub FindChildrenOfType(ByVal tpTemplate As System.Type, ByVal colDataObjects As Collections.DataObjects)
            MyBase.FindChildrenOfType(tpTemplate, colDataObjects)

            If (tpTemplate Is Nothing OrElse Util.IsTypeOf(tpTemplate, GetType(AnimatGUI.DataObjects.Behavior.Data), False)) Then
                Dim doData As AnimatGUI.DataObjects.Behavior.Data
                For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                    doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                    doData.FindChildrenOfType(tpTemplate, colDataObjects)
                Next

                For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                    doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                    doData.FindChildrenOfType(tpTemplate, colDataObjects)
                Next
            End If

        End Sub

        Public Overrides Function FindObjectByID(ByVal strID As String) As Framework.DataObject

            Dim doObject As AnimatGUI.Framework.DataObject = MyBase.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_aryBehavioralNodes Is Nothing Then doObject = m_aryBehavioralNodes.FindObjectByID(strID)
            If doObject Is Nothing AndAlso Not m_aryBehavioralLinks Is Nothing Then doObject = m_aryBehavioralLinks.FindObjectByID(strID)
            Return doObject

        End Function

        Public Overrides Sub InitializeSimulationReferences(Optional ByVal bShowError As Boolean = True)
            'Do not call base class Initialize method here. The subsystem is not a node that is within the 
            'simulator. It is a GUI editor object only. It is merely a place holder for other objects in the 
            ' nervous system.
            ' 
            Dim doData As DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In m_aryBehavioralNodes
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                doData.InitializeSimulationReferences(bShowError)
            Next

            For Each deEntry As DictionaryEntry In m_aryBehavioralLinks
                doData = DirectCast(deEntry.Value, DataObjects.Behavior.Data)
                doData.InitializeSimulationReferences(bShowError)
            Next
        End Sub

        Public Overridable Sub AddNode(ByRef bdNode As AnimatGUI.DataObjects.Behavior.Node)

            bdNode.Organism = Me.Organism
            bdNode.ParentDiagram = Me.SubsystemDiagram
            bdNode.ParentSubsystem = Me

            bdNode.InitializeAfterLoad()

            bdNode.BeforeAddNode()

            Me.BehavioralNodes.Add(bdNode.ID, bdNode)

            If Not Me.SubsystemDiagram Is Nothing Then
                Me.SubsystemDiagram.AddDiagramNode(bdNode)
            End If

            bdNode.AfterAddNode()
            bdNode.SelectItem()
        End Sub

        Public Overridable Sub RemoveNode(ByRef bnNode As AnimatGUI.DataObjects.Behavior.Node)

            Try
                If Not bnNode Is Nothing AndAlso Me.BehavioralNodes.ContainsKey(bnNode.ID) Then
                    'BeginGroupChange()

                    Dim aryLinkIDs As New Collection
                    For Each deLink As DictionaryEntry In bnNode.Links
                        aryLinkIDs.Add(deLink.Key)
                    Next

                    Dim blLink As AnimatGUI.DataObjects.Behavior.Link
                    For Each strID As String In aryLinkIDs
                        blLink = DirectCast(bnNode.Links(strID), AnimatGUI.DataObjects.Behavior.Link)
                        If Not blLink Is Nothing Then
                            blLink.Delete(False)
                        End If
                    Next

                    If Me.BehavioralNodes.ContainsKey(bnNode.ID) Then
                        bnNode.BeforeRemoveNode()

                        Me.BehavioralNodes.Remove(bnNode.ID)

                        If Not Me.SubsystemDiagram Is Nothing Then
                            Me.SubsystemDiagram.RemoveDiagramNode(bnNode)
                        End If

                        bnNode.AfterRemoveNode()
                    End If
                End If
            Catch ex As System.Exception
                Throw ex
            Finally
                'EndGroupChange()
            End Try

        End Sub

        Public Overridable Sub AddLink(ByRef bnOrigin As AnimatGUI.DataObjects.Behavior.Node, ByRef bnDestination As AnimatGUI.DataObjects.Behavior.Node, ByRef blLink As AnimatGUI.DataObjects.Behavior.Link)

            Try
                blLink.Organism = Me.Organism
                blLink.ParentDiagram = Me.SubsystemDiagram
                blLink.ParentSubsystem = Me

                blLink.BeginBatchUpdate()
                blLink.Origin = bnOrigin
                blLink.Destination = bnDestination
                blLink.EndBatchUpdate(False)

                blLink.ActualOrigin.BeforeAddLink(blLink)
                blLink.ActualDestination.BeforeAddLink(blLink)

                blLink.InitializeAfterLoad()

                blLink.BeforeAddLink()

                Me.BehavioralLinks.Add(blLink.ID, blLink)

                If Not Me.SubsystemDiagram Is Nothing Then
                    Me.SubsystemDiagram.AddDiagramLink(blLink)
                End If

                blLink.ActualOrigin.AfterAddLink(blLink)
                blLink.ActualDestination.AfterAddLink(blLink)
                blLink.AfterAddLink()
                blLink.SelectItem()

            Catch ex As Exception
                blLink.Origin = Nothing
                blLink.Destination = Nothing
                Throw ex
            End Try

        End Sub


        Public Overridable Sub RemoveLink(ByRef blLink As AnimatGUI.DataObjects.Behavior.Link)

            Try
                If Not blLink Is Nothing AndAlso Me.BehavioralLinks.ContainsKey(blLink.ID) Then
                    'BeginGroupChange()

                    If Not blLink.ActualOrigin Is Nothing Then blLink.ActualOrigin.BeforeRemoveLink(blLink)
                    If Not blLink.ActualDestination Is Nothing Then blLink.ActualDestination.BeforeRemoveLink(blLink)
                    blLink.BeforeRemoveLink()

                    If Not blLink.ActualOrigin Is Nothing Then blLink.ActualOrigin.RemoveOutLink(blLink)
                    If Not blLink.ActualDestination Is Nothing Then blLink.ActualDestination.RemoveInLink(blLink)

                    'If we are using an offpage connector it is possible for the origin and actual origin to be different.
                    'they both need to have the inlink though.
                    'blLink.Origin.OutLinks.DumpListInfo()
                    'blLink.Destination.InLinks.DumpListInfo()
                    If Not blLink.Origin Is Nothing AndAlso Not blLink.ActualOrigin Is Nothing AndAlso _
                       Not blLink.Origin Is blLink.ActualOrigin AndAlso blLink.Origin.OutLinks.Contains(blLink.ID) Then
                        blLink.Origin.RemoveOutLink(blLink)
                    End If
                    If Not blLink.Destination Is Nothing AndAlso Not blLink.ActualDestination Is Nothing AndAlso _
                       Not blLink.Destination Is blLink.ActualDestination AndAlso blLink.Destination.InLinks.Contains(blLink.ID) Then
                        blLink.Destination.RemoveInLink(blLink)
                    End If

                    Me.BehavioralLinks.Remove(blLink.ID)
                    If Not Me.SubsystemDiagram Is Nothing Then
                        Me.SubsystemDiagram.RemoveDiagramLink(blLink)
                    End If

                    If Not blLink.ActualOrigin Is Nothing Then blLink.ActualOrigin.AfterRemoveLink(blLink)
                    If Not blLink.ActualDestination Is Nothing Then blLink.ActualDestination.AfterRemoveLink(blLink)
                    blLink.AfterRemoveLink()
                End If

            Catch ex As System.Exception
                Throw ex
            Finally
                'EndGroupChange()
            End Try

        End Sub

        Public Overridable Sub RetrieveChildren(ByVal bThisDiagramOnly As Boolean, ByRef aryChildren As ArrayList)

            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                aryChildren.Add(deEntry.Value)
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                aryChildren.Add(deEntry.Value)
            Next

            If Not bThisDiagramOnly Then
                Dim doChild As DataObjects.Behavior.Nodes.Subsystem
                For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                    If Util.IsTypeOf(deEntry.Value.GetType, GetType(DataObjects.Behavior.Nodes.Subsystem)) Then
                        doChild = DirectCast(deEntry.Value, DataObjects.Behavior.Nodes.Subsystem)
                        doChild.RetrieveChildren(bThisDiagramOnly, aryChildren)
                    End If
                Next
            End If

        End Sub

#Region " DataObject Methods "

        Public Overrides Sub BuildProperties(ByRef propTable As AnimatGuiCtrls.Controls.PropertyTable)
            MyBase.BuildProperties(propTable)

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Nodes", GetType(Integer), "SubsystemNodeCount", _
                            "Node Properties", "Tells how many nodes are contained in this subsystem.", Me.SubsystemNodeCount, True))

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Links", GetType(Integer), "SubsystemLinkCount", _
                            "Node Properties", "Tells how many links are contained in this subsystem.", Me.SubsystemLinkCount, True))

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Total Nodes", GetType(Integer), "TotalNodeCount", _
                            "Node Properties", "Tells how many nodes are contained in this subsystem and all children.", Me.TotalNodeCount, True))

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Total Links", GetType(Integer), "TotalLinkCount", _
                            "Node Properties", "Tells how many links are contained in this subsystem and all children.", Me.TotalLinkCount, True))

        End Sub

        Public Overrides Sub AddToReplaceIDList(ByVal aryReplaceIDList As ArrayList, ByVal arySelectedItems As ArrayList)
            MyBase.AddToReplaceIDList(aryReplaceIDList, arySelectedItems)

            Dim doObj As Framework.DataObject
            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doObj = DirectCast(deEntry.Value, Framework.DataObject)
                doObj.AddToReplaceIDList(aryReplaceIDList, arySelectedItems)
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doObj = DirectCast(deEntry.Value, Framework.DataObject)
                doObj.AddToReplaceIDList(aryReplaceIDList, arySelectedItems)
            Next

        End Sub

        Public Overrides Sub AddToRecursiveSelectedItemsList(ByVal arySelectedItems As ArrayList)
            MyBase.AddToRecursiveSelectedItemsList(arySelectedItems)

            Dim doObj As Framework.DataObject
            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doObj = DirectCast(deEntry.Value, Framework.DataObject)
                doObj.AddToRecursiveSelectedItemsList(arySelectedItems)
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doObj = DirectCast(deEntry.Value, Framework.DataObject)
                doObj.AddToRecursiveSelectedItemsList(arySelectedItems)
            Next
        End Sub

        Public Overrides Sub LoadData(ByVal oXml As ManagedAnimatInterfaces.IStdXml)
            MyBase.LoadData(oXml)

            Try
                Util.Application.AppStatusText = "Loading " & Me.TypeName & " " & Me.Name & " Subsystem " & Me.Name

                oXml.IntoElem()

                oXml.IntoChildElement("Nodes")
                Dim iCount As Integer = oXml.NumberOfChildren() - 1
                Dim bnNode As AnimatGUI.DataObjects.Behavior.Node
                For iIndex As Integer = 0 To iCount
                    bnNode = DirectCast(Util.LoadClass(oXml, iIndex, Me), AnimatGUI.DataObjects.Behavior.Node)
                    bnNode.Organism = Me.Organism
                    bnNode.ParentSubsystem = Me
                    bnNode.LoadData(oXml)
                    Me.BehavioralNodes.Add(bnNode.ID, bnNode, False)
                Next
                oXml.OutOfElem() 'Outof Nodes Element

                oXml.IntoChildElement("Links")
                iCount = oXml.NumberOfChildren() - 1
                Dim blLink As AnimatGUI.DataObjects.Behavior.Link
                For iIndex As Integer = 0 To iCount
                    blLink = DirectCast(Util.LoadClass(oXml, iIndex, Me), AnimatGUI.DataObjects.Behavior.Link)
                    blLink.Organism = Me.Organism
                    blLink.ParentSubsystem = Me
                    blLink.LoadData(oXml)
                    Me.BehavioralLinks.Add(blLink.ID, blLink, False)
                Next
                oXml.OutOfElem() 'Outof Links Element

                m_strDiagramXml = oXml.GetChildString("DiagramXml", "")

                oXml.OutOfElem()  'Outof Subsystem Element

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Protected Overridable Function AllChildrenInitialized() As Boolean

            Dim doData As AnimatGUI.DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                If Not doData.IsInitialized Then
                    Return False
                End If
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                If Not doData.IsInitialized Then
                    Return False
                End If
            Next

            Return True
        End Function

        Public Sub InitializeAfterPasted()

            Dim iCount As Integer = 0

            While Not Me.IsInitialized
                Me.InitializeAfterLoad()

                iCount = iCount + 1
                If iCount = 3 Then
                    Throw New System.Exception("Unable to initialize subsystem after paste operation. (" & Me.Name & ", " & Me.ID & ")")
                End If
            End While

        End Sub

        Public Overrides Sub InitializeAfterLoad()

            Try
                MyBase.InitializeAfterLoad()

                Dim doData As AnimatGUI.DataObjects.Behavior.Data

                For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                    doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                    If Not doData.IsInitialized Then
                        doData.InitializeAfterLoad()
                    End If
                Next

                For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                    doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                    If Not doData.IsInitialized Then
                        doData.InitializeAfterLoad()
                    End If
                Next

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
                m_bIsInitialized = False
            End Try

        End Sub

        Public Overrides Sub AfterInitialized()

            Try
                MyBase.AfterInitialized()

                Dim doData As AnimatGUI.DataObjects.Behavior.Data

                For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                    doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                    doData.AfterInitialized()
                Next

                For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                    doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                    doData.AfterInitialized()
                Next

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
                m_bIsInitialized = False
            End Try

        End Sub


        Public Overrides Sub InitPastedInSim()
            MyBase.InitPastedInSim()

            Dim bdItem As AnimatGUI.DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                bdItem = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                bdItem.InitPastedInSim()
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                bdItem = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                bdItem.InitPastedInSim()
            Next
        End Sub

#Region " Add-Remove to List Methods "

        Public Overrides Sub AddToSim(ByVal bThrowError As Boolean, Optional ByVal bDoNotInit As Boolean = False)
            MyBase.AddToSim(bThrowError)

            Dim doData As AnimatGUI.DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.AddToSim(bThrowError, bDoNotInit)
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.AddToSim(bThrowError, bDoNotInit)
            Next
        End Sub

        Public Overrides Sub RemoveFromSim(ByVal bThrowError As Boolean)
            MyBase.RemoveFromSim(bThrowError)

            Dim doData As AnimatGUI.DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.RemoveFromSim(bThrowError)
            Next

            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.RemoveFromSim(bThrowError)
            Next
        End Sub

#End Region

        Public Overrides Sub SaveData(ByVal oXml As ManagedAnimatInterfaces.IStdXml)
            MyBase.SaveData(oXml)

            Util.Application.AppStatusText = "Saving " & Me.TypeName & " " & Me.Name & " Subsystem " & Me.Name

            oXml.IntoElem() 'Into Subsystem Element

            oXml.AddChildElement("Nodes")
            oXml.IntoElem()
            Dim doData As AnimatGUI.DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.SaveData(oXml)
            Next
            oXml.OutOfElem() ' Outof Node Element

            oXml.AddChildElement("Links")
            oXml.IntoElem()
            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.SaveData(oXml)
            Next
            oXml.OutOfElem() ' Outof Links Element

            If Not Me.SubsystemDiagram Is Nothing Then
                m_strDiagramXml = Me.SubsystemDiagram.SaveDiagramXml
            End If
            oXml.AddChildCData("DiagramXml", m_strDiagramXml)

            oXml.OutOfElem() ' Outof Subsystem Element

        End Sub

        Public Overrides Sub SaveSimulationXml(ByVal oXml As ManagedAnimatInterfaces.IStdXml, Optional ByRef nmParentControl As Framework.DataObject = Nothing, Optional ByVal strName As String = "")
            MyBase.SaveSimulationXml(oXml, nmParentControl, strName)

            oXml.IntoElem() 'Into Node Element

            oXml.AddChildElement("Nodes")
            oXml.IntoElem()
            Dim doData As AnimatGUI.DataObjects.Behavior.Data
            For Each deEntry As DictionaryEntry In Me.BehavioralNodes
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.SaveSimulationXml(oXml, Me)
            Next
            oXml.OutOfElem() ' Outof Node Element

            oXml.AddChildElement("Links")
            oXml.IntoElem()
            For Each deEntry As DictionaryEntry In Me.BehavioralLinks
                doData = DirectCast(deEntry.Value, AnimatGUI.DataObjects.Behavior.Data)
                doData.SaveSimulationXml(oXml, Me)
            Next
            oXml.OutOfElem() ' Outof Node Element

            oXml.OutOfElem() ' Outof Node Element
        End Sub

#End Region

#End Region

    End Class

End Namespace