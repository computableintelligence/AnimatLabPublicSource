Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports AnimatGuiCtrls.Controls
Imports AnimatGUI
Imports AnimatGUI.Framework
Imports AnimatGUI.DataObjects

Namespace Forms

    Public Class NewProject
        Inherits Forms.AnimatDialog

#Region " Windows Form Designer generated code "

        Public Sub New()
            MyBase.New()

            'This call is required by the Windows Form Designer.
            InitializeComponent()

            'Add any initialization after the InitializeComponent() call

        End Sub

        'Form overrides dispose to clean up the component list.
        Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
            If disposing Then
                If Not (components Is Nothing) Then
                    components.Dispose()
                End If
            End If
            MyBase.Dispose(disposing)
        End Sub

        'Required by the Windows Form Designer
        Private components As System.ComponentModel.IContainer

        'NOTE: The following procedure is required by the Windows Form Designer
        'It can be modified using the Windows Form Designer.  
        'Do not modify it using the code editor.
        Friend WithEvents lblProjectName As System.Windows.Forms.Label
        Friend WithEvents txtProjectName As System.Windows.Forms.TextBox
        Friend WithEvents btnCancel As System.Windows.Forms.Button
        Friend WithEvents btnOk As System.Windows.Forms.Button
        Friend WithEvents lblLocation As System.Windows.Forms.Label
        Friend WithEvents txtLocation As System.Windows.Forms.TextBox
        Friend WithEvents cboPhysicsEngine As System.Windows.Forms.ComboBox
        Friend WithEvents Label1 As System.Windows.Forms.Label
        Friend WithEvents lblMassUnits As System.Windows.Forms.Label
        Friend WithEvents cboMassUnits As System.Windows.Forms.ComboBox
        Friend WithEvents lblDistanceUnits As System.Windows.Forms.Label
        Friend WithEvents cboDistanceUnits As System.Windows.Forms.ComboBox
        Friend WithEvents btnBrowseLocation As System.Windows.Forms.Button
        <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
            Me.lblProjectName = New System.Windows.Forms.Label()
            Me.lblLocation = New System.Windows.Forms.Label()
            Me.txtProjectName = New System.Windows.Forms.TextBox()
            Me.txtLocation = New System.Windows.Forms.TextBox()
            Me.btnBrowseLocation = New System.Windows.Forms.Button()
            Me.btnCancel = New System.Windows.Forms.Button()
            Me.btnOk = New System.Windows.Forms.Button()
            Me.cboPhysicsEngine = New System.Windows.Forms.ComboBox()
            Me.Label1 = New System.Windows.Forms.Label()
            Me.lblMassUnits = New System.Windows.Forms.Label()
            Me.cboMassUnits = New System.Windows.Forms.ComboBox()
            Me.lblDistanceUnits = New System.Windows.Forms.Label()
            Me.cboDistanceUnits = New System.Windows.Forms.ComboBox()
            Me.SuspendLayout()
            '
            'lblProjectName
            '
            Me.lblProjectName.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
            Me.lblProjectName.Location = New System.Drawing.Point(8, 16)
            Me.lblProjectName.Name = "lblProjectName"
            Me.lblProjectName.Size = New System.Drawing.Size(280, 16)
            Me.lblProjectName.TabIndex = 0
            Me.lblProjectName.Text = "Project Name"
            Me.lblProjectName.TextAlign = System.Drawing.ContentAlignment.TopCenter
            '
            'lblLocation
            '
            Me.lblLocation.Location = New System.Drawing.Point(8, 56)
            Me.lblLocation.Name = "lblLocation"
            Me.lblLocation.Size = New System.Drawing.Size(280, 16)
            Me.lblLocation.TabIndex = 1
            Me.lblLocation.Text = "Location"
            Me.lblLocation.TextAlign = System.Drawing.ContentAlignment.TopCenter
            '
            'txtProjectName
            '
            Me.txtProjectName.Location = New System.Drawing.Point(8, 32)
            Me.txtProjectName.Name = "txtProjectName"
            Me.txtProjectName.Size = New System.Drawing.Size(288, 20)
            Me.txtProjectName.TabIndex = 2
            '
            'txtLocation
            '
            Me.txtLocation.Location = New System.Drawing.Point(8, 72)
            Me.txtLocation.Name = "txtLocation"
            Me.txtLocation.Size = New System.Drawing.Size(264, 20)
            Me.txtLocation.TabIndex = 3
            '
            'btnBrowseLocation
            '
            Me.btnBrowseLocation.Location = New System.Drawing.Point(272, 72)
            Me.btnBrowseLocation.Name = "btnBrowseLocation"
            Me.btnBrowseLocation.Size = New System.Drawing.Size(24, 20)
            Me.btnBrowseLocation.TabIndex = 4
            Me.btnBrowseLocation.Text = "..."
            '
            'btnCancel
            '
            Me.btnCancel.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
            Me.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel
            Me.btnCancel.Location = New System.Drawing.Point(160, 249)
            Me.btnCancel.Name = "btnCancel"
            Me.btnCancel.Size = New System.Drawing.Size(64, 24)
            Me.btnCancel.TabIndex = 13
            Me.btnCancel.Text = "Cancel"
            '
            'btnOk
            '
            Me.btnOk.Anchor = CType((System.Windows.Forms.AnchorStyles.Bottom Or System.Windows.Forms.AnchorStyles.Left), System.Windows.Forms.AnchorStyles)
            Me.btnOk.Location = New System.Drawing.Point(88, 249)
            Me.btnOk.Name = "btnOk"
            Me.btnOk.Size = New System.Drawing.Size(64, 24)
            Me.btnOk.TabIndex = 12
            Me.btnOk.Text = "Ok"
            '
            'cboPhysicsEngine
            '
            Me.cboPhysicsEngine.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
            Me.cboPhysicsEngine.FormattingEnabled = True
            Me.cboPhysicsEngine.Location = New System.Drawing.Point(8, 116)
            Me.cboPhysicsEngine.Name = "cboPhysicsEngine"
            Me.cboPhysicsEngine.Size = New System.Drawing.Size(288, 21)
            Me.cboPhysicsEngine.TabIndex = 14
            '
            'Label1
            '
            Me.Label1.Location = New System.Drawing.Point(8, 99)
            Me.Label1.Name = "Label1"
            Me.Label1.Size = New System.Drawing.Size(280, 16)
            Me.Label1.TabIndex = 15
            Me.Label1.Text = "Physics Engine"
            Me.Label1.TextAlign = System.Drawing.ContentAlignment.TopCenter
            '
            'lblMassUnits
            '
            Me.lblMassUnits.Location = New System.Drawing.Point(8, 148)
            Me.lblMassUnits.Name = "lblMassUnits"
            Me.lblMassUnits.Size = New System.Drawing.Size(280, 16)
            Me.lblMassUnits.TabIndex = 17
            Me.lblMassUnits.Text = "Mass Units"
            Me.lblMassUnits.TextAlign = System.Drawing.ContentAlignment.TopCenter
            '
            'cboMassUnits
            '
            Me.cboMassUnits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
            Me.cboMassUnits.FormattingEnabled = True
            Me.cboMassUnits.Location = New System.Drawing.Point(8, 165)
            Me.cboMassUnits.Name = "cboMassUnits"
            Me.cboMassUnits.Size = New System.Drawing.Size(288, 21)
            Me.cboMassUnits.TabIndex = 16
            '
            'lblDistanceUnits
            '
            Me.lblDistanceUnits.Location = New System.Drawing.Point(8, 197)
            Me.lblDistanceUnits.Name = "lblDistanceUnits"
            Me.lblDistanceUnits.Size = New System.Drawing.Size(280, 16)
            Me.lblDistanceUnits.TabIndex = 19
            Me.lblDistanceUnits.Text = "Distance Units"
            Me.lblDistanceUnits.TextAlign = System.Drawing.ContentAlignment.TopCenter
            '
            'cboDistanceUnits
            '
            Me.cboDistanceUnits.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
            Me.cboDistanceUnits.FormattingEnabled = True
            Me.cboDistanceUnits.Location = New System.Drawing.Point(8, 214)
            Me.cboDistanceUnits.Name = "cboDistanceUnits"
            Me.cboDistanceUnits.Size = New System.Drawing.Size(288, 21)
            Me.cboDistanceUnits.TabIndex = 18
            '
            'NewProject
            '
            Me.AutoScaleBaseSize = New System.Drawing.Size(5, 13)
            Me.ClientSize = New System.Drawing.Size(312, 281)
            Me.Controls.Add(Me.lblDistanceUnits)
            Me.Controls.Add(Me.cboDistanceUnits)
            Me.Controls.Add(Me.lblMassUnits)
            Me.Controls.Add(Me.cboMassUnits)
            Me.Controls.Add(Me.Label1)
            Me.Controls.Add(Me.cboPhysicsEngine)
            Me.Controls.Add(Me.btnCancel)
            Me.Controls.Add(Me.btnOk)
            Me.Controls.Add(Me.btnBrowseLocation)
            Me.Controls.Add(Me.txtLocation)
            Me.Controls.Add(Me.txtProjectName)
            Me.Controls.Add(Me.lblLocation)
            Me.Controls.Add(Me.lblProjectName)
            Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
            Me.Name = "NewProject"
            Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
            Me.Text = "New Project"
            Me.ResumeLayout(False)
            Me.PerformLayout()

        End Sub

#End Region

#Region " Attributes "

        Protected m_bAllowUserToChoosePhysicsSystem As Boolean = True

        Protected m_eMassUnits As AnimatGUI.DataObjects.Physical.Environment.enumMassUnits = Physical.Environment.enumMassUnits.Kilograms
        Protected m_eDistanceUnits As AnimatGUI.DataObjects.Physical.Environment.enumDistanceUnits = Physical.Environment.enumDistanceUnits.Meters

#End Region

#Region " Properties "

        Public Property AllowUserToChoosePhysicsSystem() As Boolean
            Get
                Return m_bAllowUserToChoosePhysicsSystem
            End Get
            Set(ByVal value As Boolean)
                m_bAllowUserToChoosePhysicsSystem = value
            End Set
        End Property

        Public Property MassUnits() As AnimatGUI.DataObjects.Physical.Environment.enumMassUnits
            Get
                Return m_eMassUnits
            End Get
            Set(ByVal value As AnimatGUI.DataObjects.Physical.Environment.enumMassUnits)
                m_eMassUnits = value
            End Set
        End Property

        Public Property DistanceUnits() As AnimatGUI.DataObjects.Physical.Environment.enumDistanceUnits
            Get
                Return m_eDistanceUnits
            End Get
            Set(ByVal value As AnimatGUI.DataObjects.Physical.Environment.enumDistanceUnits)
                m_eDistanceUnits = value
            End Set
        End Property

#End Region

#Region " Methods "

#Region "Automation Methods"

        Public Sub SetPhysics(ByVal strPhysics As String)

            Dim iIdx As Integer = 0
            For Each doEngine As DataObjects.Physical.PhysicsEngine In cboPhysicsEngine.Items
                If doEngine.Name = strPhysics Then
                    cboPhysicsEngine.SelectedIndex = iIdx
                    Return
                End If

                iIdx = iIdx + 1
            Next

        End Sub

        Public Sub SetMassUnit(ByVal strMass As String)

            Dim iIdx As Integer = 0
            For Each strUnit As String In cboMassUnits.Items
                If strUnit = strMass Then
                    cboMassUnits.SelectedIndex = iIdx
                    Return
                End If

                iIdx = iIdx + 1
            Next

        End Sub

        Public Sub SetDistanceUnit(ByVal strDistance As String)

            Dim iIdx As Integer = 0
            For Each strUnit As String In cboDistanceUnits.Items
                If strUnit = strDistance Then
                    cboDistanceUnits.SelectedIndex = iIdx
                    Return
                End If

                iIdx = iIdx + 1
            Next

        End Sub

#End Region

#End Region

#Region " Events "

        Private Sub btnOk_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnOk.Click

            Try
                If txtProjectName.Text.Trim.Length = 0 Then
                    Throw New System.Exception("You must specify a project name.")
                End If

                If txtLocation.Text.Trim.Length = 0 Then
                    Throw New System.Exception("You must specify a location for the new project.")
                End If

                'Now lets make sure there is not already a directory with that name at the specified location.
                Dim strProjectDir As String = txtLocation.Text & "\" & txtProjectName.Text
                If System.IO.Directory.Exists(strProjectDir) Then
                    Throw New System.Exception("The directory '" & strProjectDir & "' already exists. Please choose a different name or location for the project.")
                End If

                m_eMassUnits = DirectCast([Enum].Parse(GetType(AnimatGUI.DataObjects.Physical.Environment.enumMassUnits), cboMassUnits.SelectedItem.ToString(), True), AnimatGUI.DataObjects.Physical.Environment.enumMassUnits)
                m_eDistanceUnits = DirectCast([Enum].Parse(GetType(AnimatGUI.DataObjects.Physical.Environment.enumDistanceUnits), cboDistanceUnits.SelectedItem.ToString(), True), AnimatGUI.DataObjects.Physical.Environment.enumDistanceUnits)

                Me.DialogResult = DialogResult.OK
                Me.Close()

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try

        End Sub

        Private Sub btnCancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnCancel.Click
            Me.DialogResult = DialogResult.Cancel
            Me.Close()
        End Sub

        Private Sub btnBrowseDirectory_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnBrowseLocation.Click
            Try
                Dim openFolderDialog As New System.Windows.Forms.FolderBrowserDialog
                openFolderDialog.Description = "Specify the drive location where the new project directory will be created."
                openFolderDialog.ShowNewFolderButton = True
                openFolderDialog.SelectedPath = txtLocation.Text

                If openFolderDialog.ShowDialog() = DialogResult.OK Then
                    txtLocation.Text = openFolderDialog.SelectedPath
                End If

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub NewProject_KeyDown(ByVal sender As System.Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles MyBase.KeyDown
            Try
                If e.KeyCode = Keys.Enter Then
                    btnOk_Click(sender, e)
                End If
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub txtProjectName_KeyDown(ByVal sender As System.Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles txtProjectName.KeyDown
            Try
                If e.KeyCode = Keys.Enter Then
                    btnOk_Click(sender, e)
                End If
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Private Sub txtLocation_KeyDown(ByVal sender As System.Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles txtLocation.KeyDown
            Try
                If e.KeyCode = Keys.Enter Then
                    btnOk_Click(sender, e)
                End If
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Protected Overrides Sub OnLoad(ByVal e As System.EventArgs)
            MyBase.OnLoad(e)

            Try
                m_btnOk = Me.btnOk
                m_btnCancel = Me.btnCancel

                txtLocation.Text = Util.Application.DefaultNewFolder

                cboPhysicsEngine.Items.Clear()
                If m_bAllowUserToChoosePhysicsSystem Then
                    Dim iIdx As Integer = 0
                    Dim iSelIdx As Integer = 0
                    For Each doEngine As DataObjects.Physical.PhysicsEngine In Util.Application.PhysicsEngines
                        If doEngine.AllowUserToChoose Then
                            cboPhysicsEngine.Items.Add(doEngine)
                            If doEngine.Name = Util.Application.Physics.Name Then
                                iSelIdx = iIdx
                            End If
                            iIdx = iIdx + 1
                        End If
                    Next

                    cboDistanceUnits.Items.Add("Kilometers")
                    cboDistanceUnits.Items.Add("Centameters")
                    cboDistanceUnits.Items.Add("Decameters")
                    cboDistanceUnits.Items.Add("Meters")
                    cboDistanceUnits.Items.Add("Decimeters")
                    cboDistanceUnits.Items.Add("Centimeters")
                    cboDistanceUnits.Items.Add("Millimeters")
                    cboDistanceUnits.SelectedItem = "Meters"

                    cboMassUnits.Items.Add("Kilograms")
                    cboMassUnits.Items.Add("Centagrams")
                    cboMassUnits.Items.Add("Decagrams")
                    cboMassUnits.Items.Add("Grams")
                    cboMassUnits.Items.Add("Decigrams")
                    cboMassUnits.Items.Add("Centigrams")
                    cboMassUnits.Items.Add("Milligrams")
                    cboMassUnits.SelectedItem = "Kilograms"

                    cboPhysicsEngine.Enabled = True
                    cboPhysicsEngine.SelectedIndex = iSelIdx
                    cboDistanceUnits.Enabled = True
                    cboMassUnits.Enabled = True
                Else
                    cboPhysicsEngine.Enabled = False
                    cboPhysicsEngine.Items.Add(Util.Application.Physics)
                    cboPhysicsEngine.SelectedIndex = 0

                    cboDistanceUnits.Items.Add(Util.Environment.DistanceUnits.ToString())
                    cboDistanceUnits.SelectedIndex = 0
                    cboMassUnits.Items.Add(Util.Environment.MassUnits.ToString())
                    cboMassUnits.SelectedIndex = 0

                    cboDistanceUnits.Enabled = False
                    cboMassUnits.Enabled = False
                End If

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Public Overridable Sub SetProjectParams(ByVal strName As String, ByVal strPath As String)
            Me.txtProjectName.Text = strName
            Me.txtLocation.Text = strPath
        End Sub

#End Region

    End Class

End Namespace
