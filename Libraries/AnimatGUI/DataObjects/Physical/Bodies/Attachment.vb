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

    Public Class Attachment
        Inherits Physical.Bodies.Sphere

#Region " Attributes "


#End Region

#Region " Properties "

        Public Overrides ReadOnly Property WorkspaceImageName() As String
            Get
                Return "AnimatGUI.MuscleAttachment_Treeview.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property ButtonImageName() As String
            Get
                Return "AnimatGUI.MuscleAttachment_Button.gif"
            End Get
        End Property

        Public Overrides ReadOnly Property Type() As String
            Get
                Return "Attachment"
            End Get
        End Property

        Public Overrides ReadOnly Property PartType() As System.Type
            Get
                Return GetType(AnimatGUI.DataObjects.Physical.Bodies.Attachment)
            End Get
        End Property

        Public Overrides Property Rotation() As Framework.ScaledVector3
            Get
                Return m_svRotation
            End Get
            Set(ByVal value As Framework.ScaledVector3)
                'Rotation is never changed on an attachment. It is always 0,0,0
            End Set
        End Property

        Public Overrides ReadOnly Property CanBeRootBody() As Boolean
            Get
                Return False
            End Get
        End Property

        Public Overrides ReadOnly Property UsesAJoint() As Boolean
            Get
                Return False
            End Get
        End Property

        Public Overrides ReadOnly Property HasDynamics() As Boolean
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
            m_bIsCollisionObject = False
            m_iLatitudeSegments = 20
            m_iLongtitudeSegments = 20
            m_clDiffuse = Color.Orange
        End Sub

        Public Overrides Function Clone(ByVal doParent As Framework.DataObject, ByVal bCutData As Boolean, ByVal doRoot As Framework.DataObject) As Framework.DataObject
            Dim oNewNode As New Bodies.Attachment(doParent)
            oNewNode.CloneInternal(Me, bCutData, doRoot)
            If Not doRoot Is Nothing AndAlso doRoot Is Me Then oNewNode.AfterClone(Me, bCutData, doRoot, oNewNode)
            Return oNewNode
        End Function

        Public Overrides Sub SetDefaultSizes()
            MyBase.SetDefaultSizes()
            m_snRadius.ActualValue = 0.1 * Util.Environment.DistanceUnitValue
        End Sub

        Public Overrides Sub BuildProperties(ByRef propTable As AnimatGuiCtrls.Controls.PropertyTable)
            MyBase.BuildProperties(propTable)

            'Remove all of these columns that are not valid for a plane object.
            If propTable.Properties.Contains("Density") Then propTable.Properties.Remove("Density")
            If propTable.Properties.Contains("Cd") Then propTable.Properties.Remove("Cd")
            If propTable.Properties.Contains("Cdr") Then propTable.Properties.Remove("Cdr")
            If propTable.Properties.Contains("Ca") Then propTable.Properties.Remove("Ca")
            If propTable.Properties.Contains("Car") Then propTable.Properties.Remove("Car")
            If propTable.Properties.Contains("Center of Mass") Then propTable.Properties.Remove("Center of Mass")
            If propTable.Properties.Contains("Freeze") Then propTable.Properties.Remove("Freeze")
            If propTable.Properties.Contains("Car") Then propTable.Properties.Remove("Car")
            If propTable.Properties.Contains("Odor Sources") Then propTable.Properties.Remove("Odor Sources")
            If propTable.Properties.Contains("Food Source") Then propTable.Properties.Remove("Food Source")
            If propTable.Properties.Contains("Rotation") Then propTable.Properties.Remove("Rotation")

        End Sub

    End Class


End Namespace
