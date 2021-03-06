Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports AnimatGuiCtrls.Controls
Imports AnimatGUI.DataObjects
Imports AnimatGUI.Framework

Namespace Collections


    Public Class ExternalFileForms
        Inherits AnimatCollectionBase

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal index As Integer) As AnimatGUI.Forms.ExternalFileForm
            Get
                Return CType(List(index), AnimatGUI.Forms.ExternalFileForm)
            End Get
            Set(ByVal Value As AnimatGUI.Forms.ExternalFileForm)
                List(index) = Value
            End Set
        End Property


        Public Function Add(ByVal value As AnimatGUI.Forms.ExternalFileForm) As Integer
            Me.IsDirty = True
            Return List.Add(value)
        End Function 'Add

        Public Function IndexOf(ByVal value As AnimatGUI.Forms.ExternalFileForm) As Integer
            Return List.IndexOf(value)
        End Function 'IndexOf


        Public Sub Insert(ByVal index As Integer, ByVal value As AnimatGUI.Forms.ExternalFileForm)
            List.Insert(index, value)
            Me.IsDirty = True
        End Sub 'Insert


        Public Sub Remove(ByVal value As AnimatGUI.Forms.ExternalFileForm)
            List.Remove(value)
            Me.IsDirty = True
        End Sub 'Remove


        Public Function Contains(ByVal value As AnimatGUI.Forms.ExternalFileForm) As Boolean
            ' If value is not of type Int16, this will return false.
            Return List.Contains(value)
        End Function 'Contains


        Protected Overrides Sub OnInsert(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is AnimatGUI.Forms.ExternalFileForm Then
                Throw New ArgumentException("value must be of type AnimatGUI.Forms.ExternalFileForm.", "value")
            End If
        End Sub 'OnInsert


        Protected Overrides Sub OnRemove(ByVal index As Integer, ByVal value As [Object])
            If Not TypeOf (value) Is AnimatGUI.Forms.ExternalFileForm Then
                Throw New ArgumentException("value must be of type AnimatGUI.Forms.ExternalFileForm.", "value")
            End If
        End Sub 'OnRemove


        Protected Overrides Sub OnSet(ByVal index As Integer, ByVal oldValue As [Object], ByVal newValue As [Object])
            If Not TypeOf (newValue) Is AnimatGUI.Forms.ExternalFileForm Then
                Throw New ArgumentException("newValue must be of type AnimatGUI.Forms.ExternalFileForm.", "newValue")
            End If
        End Sub 'OnSet


        Protected Overrides Sub OnValidate(ByVal value As [Object])
            If Not TypeOf (value) Is AnimatGUI.Forms.ExternalFileForm Then
                Throw New ArgumentException("value must be of type AnimatGUI.Forms.ExternalFileForm.")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatCollectionBase
            Dim aryList As New ExternalFileForms(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatGUI.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatGUI.Framework.DataObject) As AnimatCollectionBase
            Dim aryList As New ExternalFileForms(doParent)
            aryList.CloneInternal(Me, doParent, bCutData, doRoot)
            Return aryList
        End Function

    End Class 'NodesCollection

    Public Class SortedExternalFileForms
        Inherits AnimatDictionaryBase

        Public Sub New(ByVal doParent As Framework.DataObject)
            MyBase.New(doParent)
        End Sub

        Default Public Property Item(ByVal key As [String]) As AnimatGUI.Forms.ExternalFileForm
            Get
                Return CType(Dictionary(key), AnimatGUI.Forms.ExternalFileForm)
            End Get
            Set(ByVal Value As AnimatGUI.Forms.ExternalFileForm)
                Dictionary(key) = Value
            End Set
        End Property

        Public ReadOnly Property Keys() As ICollection
            Get
                Return Dictionary.Keys
            End Get
        End Property

        Public ReadOnly Property Values() As ICollection
            Get
                Return Dictionary.Values
            End Get
        End Property

        Public Sub Add(ByVal key As [String], ByVal value As AnimatGUI.Forms.ExternalFileForm)
            Dictionary.Add(key, value)
            Me.IsDirty = True
        End Sub 'Add

        Public Function Contains(ByVal key As [String]) As Boolean
            Return Dictionary.Contains(key)
        End Function 'Contains

        Public Sub Remove(ByVal key As [String])
            Dictionary.Remove(key)
            Me.IsDirty = True
        End Sub 'Remove

        Public Function GetAtByType(ByVal strID As String, ByVal tp As System.Type, Optional ByVal bThrowError As Boolean = True) As AnimatGUI.Forms.ExternalFileForm
            Dim frmSel As AnimatGUI.Forms.ExternalFileForm

            If Me.Contains(strID) Then
                frmSel = Me(strID)

                If Not Util.IsTypeOf(frmSel.GetType(), tp, False) Then
                    frmSel = Nothing
                    If bThrowError Then
                        Throw New System.Exception("The form with ID '" & strID & "' was found but was not of type '" & tp.FullName.ToString() & "'")
                    End If
                End If
            ElseIf bThrowError Then
                Throw New System.Exception("Form with ID '" & strID & "' was not found.")
            End If

            Return frmSel
        End Function

        Protected Overrides Sub OnInsert(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (value) Is AnimatGUI.Forms.ExternalFileForm Then
                Throw New ArgumentException("value must be of type AnimatGUI.Forms.ExternalFileForm.", "value")
            End If

            Dim diImage As AnimatGUI.Forms.ExternalFileForm = DirectCast(value, AnimatGUI.Forms.ExternalFileForm)

        End Sub 'OnInsert

        Protected Overrides Sub OnRemove(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If
        End Sub 'OnRemove

        Protected Overrides Sub OnSet(ByVal key As [Object], ByVal oldValue As [Object], ByVal newValue As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (newValue) Is AnimatGUI.Forms.ExternalFileForm Then
                Throw New ArgumentException("newValue must be of type AnimatGUI.Forms.ExternalFileForm.", "newValue")
            End If
        End Sub 'OnSet

        Protected Overrides Sub OnValidate(ByVal key As [Object], ByVal value As [Object])
            If Not key.GetType() Is Type.GetType("System.String") Then
                Throw New ArgumentException("key must be of type String.", "key")
            End If

            If Not TypeOf (value) Is AnimatGUI.Forms.ExternalFileForm Then
                Throw New ArgumentException("value must be of type AnimatGUI.Forms.ExternalFileForm.", "value")
            End If
        End Sub 'OnValidate 

        Public Overrides Function Copy() As AnimatDictionaryBase
            Dim aryList As New SortedExternalFileForms(m_doParent)
            aryList.CopyInternal(Me)
            Return aryList
        End Function

        Public Overrides Function Clone(ByVal doParent As AnimatGUI.Framework.DataObject, ByVal bCutData As Boolean, _
                                           ByVal doRoot As AnimatGUI.Framework.DataObject) As AnimatDictionaryBase
            Dim aryList As New SortedExternalFileForms(doParent)
            aryList.CloneInternal(Me, doParent, bCutData, doRoot)
            Return aryList
        End Function

    End Class 'DiagramsDictionary

End Namespace
