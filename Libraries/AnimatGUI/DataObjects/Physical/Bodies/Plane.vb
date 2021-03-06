﻿Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports AnimatGuiCtrls.Controls
Imports AnimatGUI.Framework

Namespace DataObjects.Physical.Bodies

    Public Class Plane
        Inherits Physical.RigidBody

#Region " Attributes "

        Protected m_svSize As ScaledVector2
        Protected m_iWidthSegments As Integer = 1
        Protected m_iLengthSegments As Integer = 1

#End Region

#Region " Properties "

        Public Overrides ReadOnly Property WorkspaceImageName() As String
            Get
                Return "AnimatGUI.Plane_Treeview.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property ButtonImageName() As String
            Get
                Return "AnimatGUI.Plane_SelectType.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property Type() As String
            Get
                Return "Plane"
            End Get
        End Property

        Public Overrides ReadOnly Property PartType() As System.Type
            Get
                Return GetType(AnimatGUI.DataObjects.Physical.Bodies.Plane)
            End Get
        End Property

        Public Overridable Property Size() As Framework.ScaledVector2
            Get
                Return m_svSize
            End Get
            Set(ByVal value As Framework.ScaledVector2)
                Me.SetSimData("Size", value.GetSimulationXml("Size"), True)
                m_svSize.CopyData(value)
            End Set
        End Property

        Public Overridable Property WidthSegments() As Integer
            Get
                Return m_iWidthSegments
            End Get
            Set(ByVal value As Integer)
                If value <= 0 Then
                    Throw New System.Exception("The width segments cannot be less than or equal to zero.")
                End If
                Me.SetSimData("WidthSegments", value.ToString, True)
                m_iWidthSegments = value
            End Set
        End Property

        Public Overridable Property LengthSegments() As Integer
            Get
                Return m_iLengthSegments
            End Get
            Set(ByVal value As Integer)
                If value <= 0 Then
                    Throw New System.Exception("The length segments cannot be less than or equal to zero.")
                End If
                Me.SetSimData("LengthSegments", value.ToString, True)
                m_iLengthSegments = value
            End Set
        End Property

        Public Overrides ReadOnly Property UsesAJoint() As Boolean
            Get
                Return False
            End Get
        End Property

        Public Overrides ReadOnly Property DefaultAddGraphics() As Boolean
            Get
                Return False
            End Get
        End Property

#End Region

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
            m_strDescription = ""

            'Start out white, and ambiently lit.
            m_clAmbient = Color.FromArgb(255, 255, 255, 255)
            m_clDiffuse = Color.FromArgb(255, 255, 255, 255)
            m_clSpecular = Color.FromArgb(255, 64, 64, 64)
            m_fltShininess = 64

            m_svSize = New ScaledVector2(Me, "Size", "Size of the visible plane.", "Meters", "m")

            AddHandler m_svSize.ValueChanged, AddressOf Me.OnPlaneSizeChanged
            AddHandler m_svSize.ValueChanging, AddressOf Me.OnPlaneSizeChanging

        End Sub

        Public Overrides Sub ClearIsDirty()
            MyBase.ClearIsDirty()
            If Not m_svSize Is Nothing Then m_svSize.ClearIsDirty()
        End Sub

        Public Overrides Function Clone(ByVal doParent As Framework.DataObject, ByVal bCutData As Boolean, ByVal doRoot As Framework.DataObject) As Framework.DataObject
            Dim oNewNode As New Bodies.Plane(doParent)
            oNewNode.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewNode.AfterClone(Me, bCutData, doRoot, oNewNode)
            Return oNewNode
        End Function

        Protected Overrides Sub CloneInternal(ByVal doOriginal As AnimatGUI.Framework.DataObject, ByVal bCutData As Boolean, _
                                            ByVal doRoot As AnimatGUI.Framework.DataObject)
            MyBase.CloneInternal(doOriginal, bCutData, doRoot)

            Dim doOrig As Bodies.Plane = DirectCast(doOriginal, Bodies.Plane)

            m_svSize = DirectCast(doOrig.m_svSize.Clone(Me, bCutData, doRoot), AnimatGUI.Framework.ScaledVector2)
            m_iWidthSegments = doOrig.m_iWidthSegments
            m_iLengthSegments = doOrig.m_iLengthSegments

            AddHandler m_svSize.ValueChanged, AddressOf Me.OnPlaneSizeChanged
            AddHandler m_svSize.ValueChanging, AddressOf Me.OnPlaneSizeChanging

        End Sub

        Public Overrides Sub SetDefaultSizes()
            MyBase.SetDefaultSizes()
            Dim fltVal As Single = 100 * Util.Environment.DistanceUnitValue
            m_svSize.CopyData(fltVal, fltVal)
        End Sub

        Public Overrides Sub SetupInitialTransparencies()
            If Not m_Transparencies Is Nothing Then
                m_Transparencies.GraphicsTransparency = 0
                m_Transparencies.CollisionsTransparency = 0
                m_Transparencies.JointsTransparency = 0
                m_Transparencies.ReceptiveFieldsTransparency = 0
                m_Transparencies.SimulationTransparency = 0
            End If
        End Sub

        Public Overrides Sub SetupPartTypesExclusions()
            'A terrain can only be added to a terrain or plane type.
            For Each bpBody As BodyPart In Util.Application.BodyPartTypes
                If Not (TypeOf bpBody Is Terrain OrElse TypeOf bpBody Is Plane) Then
                    Util.Application.AddPartTypeExclusion(bpBody.GetType, Me.GetType)
                End If
            Next

            Util.Application.AddPartTypeExclusion(Me.GetType, Me.GetType)
            Util.Application.AddPartTypeExclusion(Me.GetType, GetType(Plane))
            Util.Application.AddPartTypeExclusion(Me.GetType, GetType(Joints.StaticJoint))

            ''A plane can only be added to a terrain or plane type.
            'For Each bpBody As BodyPart In Util.Application.BodyPartTypes
            '    If Not (TypeOf bpBody Is Terrain OrElse TypeOf bpBody Is Plane) Then
            '        Util.Application.AddPartTypeExclusion(bpBody.GetType, Me.GetType)

            '        'Cant add anything to a plane.
            '        Util.Application.AddPartTypeExclusion(Me.GetType, bpBody.GetType)
            '    End If
            'Next
        End Sub

        Protected Overrides Sub OrientNewPart(ByVal vPos As Framework.Vec3d, ByVal vNorm As Framework.Vec3d)
            'Don't orient plane parts. They are setup already.
        End Sub

        Public Overrides Sub BeforeAddBody()
            MyBase.BeforeAddBody()

            If Me.IsRoot Then
                Me.Rotation.X.ActualValue = -90
            End If
        End Sub

        Public Overrides Sub BuildProperties(ByRef propTable As AnimatGuiCtrls.Controls.PropertyTable)
            MyBase.BuildProperties(propTable)

            Dim pbNumberBag As AnimatGuiCtrls.Controls.PropertyBag
            pbNumberBag = m_svSize.Properties
            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Size", pbNumberBag.GetType(), "Size", _
                                        "Part Properties", "Sets the size of the visible plane.", pbNumberBag, _
                                        "", GetType(AnimatGUI.Framework.ScaledVector2.ScaledVector2PropBagConverter)))

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Segments Width", Me.WidthSegments.GetType(), "WidthSegments", _
                                        "Visibility", "The number of segments to divide the plane width into.", Me.WidthSegments))

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Segments Length", Me.LengthSegments.GetType(), "LengthSegments", _
                                        "Visibility", "The number of segments to divide the plane length into.", Me.LengthSegments))

            'Remove all of these columns that are not valid for a plane object.
            If propTable.Properties.Contains("Buoyancy Center") Then propTable.Properties.Remove("Buoyancy Center")
            If propTable.Properties.Contains("Buoyancy Scale") Then propTable.Properties.Remove("Buoyancy Scale")
            If propTable.Properties.Contains("Drag") Then propTable.Properties.Remove("Drag")
            If propTable.Properties.Contains("Magnus") Then propTable.Properties.Remove("Magnus")
            If propTable.Properties.Contains("Enable Fluids") Then propTable.Properties.Remove("Enable Fluids")
            If propTable.Properties.Contains("Density") Then propTable.Properties.Remove("Density")
            If propTable.Properties.Contains("Mass") Then propTable.Properties.Remove("Mass")
            If propTable.Properties.Contains("Volume") Then propTable.Properties.Remove("Volume")
            If propTable.Properties.Contains("Center of Mass") Then propTable.Properties.Remove("Center of Mass")
            If propTable.Properties.Contains("Contact Sensor") Then propTable.Properties.Remove("Contact Sensor")
            If propTable.Properties.Contains("Freeze") Then propTable.Properties.Remove("Freeze")
            If propTable.Properties.Contains("Odor Sources") Then propTable.Properties.Remove("Odor Sources")
            If propTable.Properties.Contains("Food Source") Then propTable.Properties.Remove("Food Source")
            If propTable.Properties.Contains("Food Quantity") Then propTable.Properties.Remove("Food Quantity")
            If propTable.Properties.Contains("Max Food Quantity") Then propTable.Properties.Remove("Max Food Quantity")
            If propTable.Properties.Contains("Food Replenish Rate") Then propTable.Properties.Remove("Food Replenish Rate")
            If propTable.Properties.Contains("Food Energy Content") Then propTable.Properties.Remove("Food Energy Content")

        End Sub

        Public Overloads Overrides Sub LoadData(ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByVal oXml As ManagedAnimatInterfaces.IStdXml)
            MyBase.LoadData(doStructure, oXml)

            'Plane is always frozen
            m_bFreeze = True

            oXml.IntoElem() 'Into RigidBody Element

            m_svSize.LoadData(oXml, "Size")
            m_iWidthSegments = oXml.GetChildInt("WidthSegments", m_iWidthSegments)
            m_iLengthSegments = oXml.GetChildInt("LengthSegments", m_iLengthSegments)

            oXml.OutOfElem() 'Outof RigidBody Element

        End Sub

        Public Overloads Overrides Sub SaveData(ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByVal oXml As ManagedAnimatInterfaces.IStdXml)
            MyBase.SaveData(doStructure, oXml)

            oXml.IntoElem() 'Into Child Elemement

            m_svSize.SaveData(oXml, "Size")
            oXml.AddChildElement("WidthSegments", m_iWidthSegments)
            oXml.AddChildElement("LengthSegments", m_iLengthSegments)

            oXml.OutOfElem() 'Outof BodyPart Element

        End Sub

        Public Overrides Sub SaveSimulationXml(ByVal oXml As ManagedAnimatInterfaces.IStdXml, Optional ByRef nmParentControl As Framework.DataObject = Nothing, Optional ByVal strName As String = "")
            MyBase.SaveSimulationXml(oXml, nmParentControl, strName)

            oXml.IntoElem()

            m_svSize.SaveSimulationXml(oXml, Me, "Size")
            oXml.AddChildElement("WidthSegments", m_iWidthSegments)
            oXml.AddChildElement("LengthSegments", m_iLengthSegments)

            oXml.OutOfElem()

        End Sub


#Region " Events "

        Protected Overridable Sub OnPlaneSizeChanged(ByVal iIdx As Integer, ByVal snParam As ScaledNumber)
            Try
                Me.SetSimData("Size", m_svSize.GetSimulationXml("Size"), True)
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overridable Sub OnPlaneSizeChanging(ByVal snParam As ScaledNumber, ByVal dblNewVal As Double, ByVal eNewScale As ScaledNumber.enumNumericScale)
            If dblNewVal <= 0 Then
                Throw New System.Exception("The size of the plane cannot be less than or equal to zero.")
            End If
        End Sub
#End Region

    End Class


End Namespace
