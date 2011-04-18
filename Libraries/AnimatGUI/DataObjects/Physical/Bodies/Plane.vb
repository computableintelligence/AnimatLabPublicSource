﻿Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports Crownwood.Magic.Common
Imports AnimatGuiCtrls.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
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
                Return "AnimatGUI.Cone_Button.gif"
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
                Me.SetSimData("WidthSegments", value.ToString, True)
                m_iWidthSegments = value
            End Set
        End Property

        Public Overridable Property LengthSegments() As Integer
            Get
                Return m_iLengthSegments
            End Get
            Set(ByVal value As Integer)
                Me.SetSimData("LengthSegments", value.ToString, True)
                m_iLengthSegments = value
            End Set
        End Property

#End Region

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
            m_strDescription = ""

            m_svSize = New ScaledVector2(Me, "Size", "Size of the visible plane.", "Meters", "m")

            AddHandler m_svSize.ValueChanged, AddressOf Me.OnSizeChanged

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
        End Sub

        Public Overrides Sub SetDefaultSizes()
            Dim fltVal As Single = 1 * Util.Environment.DistanceUnitValue
            m_svSize.CopyData(fltVal, fltVal)
        End Sub

        Public Overrides Sub BuildProperties(ByRef propTable As AnimatGuiCtrls.Controls.PropertyTable)
            MyBase.BuildProperties(propTable)

            Dim pbNumberBag As AnimatGuiCtrls.Controls.PropertyBag
            pbNumberBag = m_svSize.Properties
            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Size", pbNumberBag.GetType(), "Size", _
                                        "Part Properties", "Sets the size of the visible plane.", pbNumberBag, _
                                        "", GetType(AnimatGUI.Framework.ScaledVector2.ScaledVector2PropBagConverter)))

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Width Segments", Me.WidthSegments.GetType(), "WidthSegments", _
                                        "Part Properties", "The number of segments to divide the plane width into.", Me.WidthSegments))

            propTable.Properties.Add(New AnimatGuiCtrls.Controls.PropertySpec("Length Segments", Me.LengthSegments.GetType(), "LengthSegments", _
                                        "Part Properties", "The number of segments to divide the plane length into.", Me.LengthSegments))

        End Sub


        Public Overloads Overrides Sub LoadData(ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByRef oXml As Interfaces.StdXml)
            MyBase.LoadData(doStructure, oXml)

            oXml.IntoElem() 'Into RigidBody Element

            m_svSize.LoadData(oXml, "Size")
            m_iWidthSegments = oXml.GetChildInt("WidthSegments", m_iWidthSegments)
            m_iLengthSegments = oXml.GetChildInt("LengthSegments", m_iLengthSegments)

            oXml.OutOfElem() 'Outof RigidBody Element

        End Sub

        Public Overloads Overrides Sub SaveData(ByRef doStructure As DataObjects.Physical.PhysicalStructure, ByRef oXml As Interfaces.StdXml)
            MyBase.SaveData(doStructure, oXml)

            oXml.IntoElem() 'Into Child Elemement

            m_svSize.SaveData(oXml, "Size")
            oXml.AddChildElement("WidthSegments", m_iWidthSegments)
            oXml.AddChildElement("LengthSegments", m_iLengthSegments)

            oXml.OutOfElem() 'Outof BodyPart Element

        End Sub

        Public Overrides Sub SaveSimulationXml(ByRef oXml As Interfaces.StdXml, Optional ByRef nmParentControl As Framework.DataObject = Nothing, Optional ByVal strName As String = "")
            MyBase.SaveSimulationXml(oXml, nmParentControl, strName)

            oXml.IntoElem()

            m_svSize.SaveSimulationXml(oXml, Me, "Size")
            oXml.AddChildElement("WidthSegments", m_iWidthSegments)
            oXml.AddChildElement("LengthSegments", m_iLengthSegments)

            oXml.OutOfElem()

        End Sub


#Region " Events "

         Protected Overridable Sub OnSizeChanged()
            Try
                Me.SetSimData("Size", m_svSize.GetSimulationXml("Size"), True)
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

#End Region

    End Class


End Namespace
