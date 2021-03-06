Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports System.IO
Imports System.Xml
Imports System.Text.RegularExpressions
Imports System.Reflection

Namespace Framework

    Public Class Util

        'Windows API Declarations
        Declare Function PostMessage _
        Lib "user32" _
        Alias "PostMessageA" _
        (ByVal hwnd As Integer, _
        ByVal wMsg As Integer, _
        ByVal wParam As Integer, _
        ByVal lParam As String) As Integer

        Declare Function GlobalLock _
        Lib "kernel32" _
        (ByVal hMem As Integer) As Integer

        Declare Function GlobalFree _
        Lib "kernel32" _
        (ByVal hMem As Integer) As Integer

        Declare Function hwrite _
        Lib "kernel32" _
        Alias "_hwrite" _
        (ByVal hFile As Integer, _
        ByVal lpBuffer As String, _
        ByVal lBytes As Integer) As Integer

        Declare Function lclose _
        Lib "kernel32" _
        Alias "_lclose" _
        (ByVal hFile As Integer) As Integer

        Public Enum WindowsMessages
            WM_MOVE = &H3
            WM_CLOSE = &H10
            WM_PAINT = &HF
            WM_APP = &H8000
            WM_AM_UPDATE_DATA = &H8001
            WM_AM_SIMULATION_ERROR = &H8002
        End Enum

        Protected Shared m_frmApplication As Forms.AnimatApplication
        Protected Shared m_bCopyInProgress As Boolean = False
        Protected Shared m_bCutInProgress As Boolean = False
        Protected Shared m_bLoadInProgress As Boolean = False
        Protected Shared m_bSaveInProgress As Boolean = False
        Protected Shared m_bDisableDirtyFlags As Boolean = False
        Protected Shared m_iDisableDirtyCount As Integer = 0
        Protected Shared m_szErrorFormSize As New Size(500, 250)
        Protected Shared m_bDisplayErrorDetails As Boolean = False
        Protected Shared m_bDisplayingError As Boolean = False
        Protected Shared m_bExportForStandAloneSim As Boolean = False
        Protected Shared m_bExportStimsInStandAloneSim As Boolean = False
        Protected Shared m_bExportChartsInStandAloneSim As Boolean = False
        Protected Shared m_bExportChartsToFile As Boolean = False 'Determines if data charts are saved to a file or kept in memory for sim.
        Protected Shared m_bExportWindowsToFile As Boolean = False 'Determines if windows are saved to a file or kept in memory for sim.
        Protected Shared m_doExportRobotInterface As DataObjects.Robotics.RobotInterface
        Protected Shared m_doExportPhysicsEngine As DataObjects.Physical.PhysicsEngine
        Protected Shared m_strVersionNumber As String = "2.1.4"

        Protected Shared m_aryActiveDialogs As New ArrayList

        ''' List of errors that have occured in the application.
        Protected Shared m_aryErrors As New ArrayList
        Protected Shared m_frmError As AnimatGUI.Forms.ErrorDisplay

        Protected Shared m_frmMessage As AnimatGUI.Forms.AnimatMessageBox

        Protected Shared m_dblRadiansToDegreeRatio As Double = (180 / Math.PI)
        Protected Shared m_dblDegreeToRadiansRatio As Double = (Math.PI / 180)

        Public Shared ReadOnly Property VersionNumber() As String
            Get
                Return m_strVersionNumber
            End Get
        End Property

        Public Shared Property Application() As Forms.AnimatApplication
            Get
                Return m_frmApplication
            End Get
            Set(ByVal Value As Forms.AnimatApplication)
                m_frmApplication = Value
            End Set
        End Property

        Public Shared ReadOnly Property Simulation() As DataObjects.Simulation
            Get
                If Not Util.Application Is Nothing Then
                    Return Util.Application.Simulation
                Else
                    Return Nothing
                End If
            End Get
        End Property

        Public Shared ReadOnly Property Environment() As DataObjects.Physical.Environment
            Get
                If Not Util.Application Is Nothing AndAlso Not Util.Application.Simulation Is Nothing Then
                    Return Util.Application.Simulation.Environment
                Else
                    Return Nothing
                End If
            End Get
        End Property

        Public Shared ReadOnly Property ActiveDialogs() As ArrayList
            Get
                Return m_aryActiveDialogs
            End Get
        End Property

        Public Shared ReadOnly Property ProjectWorkspace() As Forms.ProjectWorkspace
            Get
                Return m_frmApplication.ProjectWorkspace
            End Get
        End Property

        Public Shared ReadOnly Property ProjectProperties() As Forms.ProjectProperties
            Get
                Return m_frmApplication.ProjectProperties
            End Get
        End Property

        Public Shared ReadOnly Property SecurityMgr() As AnimatGuiCtrls.Security.SecurityManager
            Get
                Return m_frmApplication.SecurityMgr
            End Get
        End Property

        Public Shared ReadOnly Property Logger() As ManagedAnimatInterfaces.ILogger
            Get
                If Not Util.Application Is Nothing Then
                    Return Util.Application.Logger
                Else
                    Return Nothing
                End If
            End Get
        End Property

        Public Shared ReadOnly Property ModificationHistory() As AnimatGUI.Framework.UndoSystem.ModificationHistory
            Get
                If Not Util.Application Is Nothing Then
                    Return Util.Application.ModificationHistory
                Else
                    Return Nothing
                End If
            End Get
        End Property

        Public Shared Property LoadInProgress() As Boolean
            Get
                Return m_bLoadInProgress
            End Get
            Set(ByVal Value As Boolean)
                m_bLoadInProgress = Value
            End Set
        End Property

        Public Shared Property SaveInProgress() As Boolean
            Get
                Return m_bSaveInProgress
            End Get
            Set(ByVal Value As Boolean)
                m_bSaveInProgress = Value
            End Set
        End Property

        Public Shared Property DisableDirtyFlags() As Boolean
            Get
                Return m_bDisableDirtyFlags
            End Get
            Set(ByVal Value As Boolean)
                'If we are trying to disable the dirty flags then lets up the count
                If Value Then
                    m_iDisableDirtyCount = m_iDisableDirtyCount + 1
                    m_bDisableDirtyFlags = True
                Else
                    'If we are trying to enable the flags again lets decrement the count
                    'and if it reaches 0 then set them back to true.
                    m_iDisableDirtyCount = m_iDisableDirtyCount - 1
                    If m_iDisableDirtyCount <= 0 Then
                        m_iDisableDirtyCount = 0
                        m_bDisableDirtyFlags = False
                    End If
                End If
            End Set
        End Property

        Public Shared Property ExportForStandAloneSim() As Boolean
            Get
                Return m_bExportForStandAloneSim
            End Get
            Set(ByVal Value As Boolean)
                m_bExportForStandAloneSim = Value
            End Set
        End Property

        Public Shared Property ExportStimsInStandAloneSim() As Boolean
            Get
                Return m_bExportStimsInStandAloneSim
            End Get
            Set(ByVal Value As Boolean)
                m_bExportStimsInStandAloneSim = Value
            End Set
        End Property

        Public Shared Property ExportChartsInStandAloneSim() As Boolean
            Get
                Return m_bExportChartsInStandAloneSim
            End Get
            Set(ByVal Value As Boolean)
                m_bExportChartsInStandAloneSim = Value
            End Set
        End Property

        Public Shared Property ExportChartsToFile() As Boolean
            Get
                Return m_bExportChartsToFile
            End Get
            Set(ByVal Value As Boolean)
                m_bExportChartsToFile = Value
            End Set
        End Property

        Public Shared Property ExportWindowsToFile() As Boolean
            Get
                Return m_bExportWindowsToFile
            End Get
            Set(ByVal Value As Boolean)
                m_bExportWindowsToFile = Value
            End Set
        End Property

        Public Shared Property ExportRobotInterface As DataObjects.Robotics.RobotInterface
            Get
                Return m_doExportRobotInterface
            End Get
            Set(ByVal Value As DataObjects.Robotics.RobotInterface)
                m_doExportRobotInterface = Value
            End Set
        End Property

        Public Shared Property ExportPhysicsEngine As AnimatGUI.DataObjects.Physical.PhysicsEngine
            Get
                Return m_doExportPhysicsEngine
            End Get
            Set(ByVal Value As AnimatGUI.DataObjects.Physical.PhysicsEngine)
                m_doExportPhysicsEngine = Value
            End Set
        End Property

        Public Shared ReadOnly Property Errors() As ArrayList
            Get
                Return m_aryErrors
            End Get
        End Property

        Public Shared ReadOnly Property ErrorForm() As AnimatGUI.Forms.ErrorDisplay
            Get
                Return m_frmError
            End Get
        End Property

        Public Shared Function Rand(ByVal dblMin As Double, ByVal dblMax As Double) As Double
            Dim rndNum As System.Random = New System.Random
            Return ((Math.Abs(dblMax - dblMin) * rndNum.NextDouble) + dblMin)
        End Function

        Public Shared Function Rand(ByVal dblMin As Double, ByVal dblMax As Double, ByVal rndNum As System.Random) As Double
            Return ((Math.Abs(dblMax - dblMin) * rndNum.NextDouble) + dblMin)
        End Function

        Public Shared Function IsBlank(ByVal strVal As String) As Boolean
            If strVal.Trim.Length > 0 Then
                Return False
            Else
                Return True
            End If
        End Function

        Public Shared Sub SplitPathAndFile(ByVal strFullPath As String, _
                                           ByRef strPath As String, _
                                           ByRef strFile As String)
            Dim aryParts() As String = Split(strFullPath, "\")

            Dim iCount As Integer = aryParts.GetUpperBound(0)
            If iCount <= 0 Then
                strPath = ""
                strFile = strFullPath
            Else
                strFile = aryParts(iCount)
                ReDim Preserve aryParts(iCount - 1)
            End If

            strPath = Join(aryParts, "\")
            If Not IsBlank(strPath) Then strPath += "\"
        End Sub

        Public Shared Function ExtractFilename(ByVal strFullPath As String) As String
            Dim strPath As String, strFile As String
            SplitPathAndFile(strFullPath, strPath, strFile)
            Return strFile
        End Function

        Public Shared Function IsFullPath(ByVal strPath As String) As Boolean
            Dim aryParts() As String = Split(strPath, ":")

            Dim iCount As Integer = aryParts.GetUpperBound(0)
            If iCount > 0 Then
                Return True
            Else
                Return False
            End If

        End Function

        Public Shared Function VerifyFilePath(ByVal strFilename As String) As String

            If (File.Exists(strFilename)) Then
                Return strFilename
            Else
                Dim strPath As String = ""
                Dim strFile As String = ""
                Util.SplitPathAndFile(strFilename, strPath, strFile)
                strFile = Util.Application.ProjectPath & strFile

                If (File.Exists(strFile)) Then
                    Return strFile
                Else
                    Return ""
                End If
            End If

        End Function

        Public Shared Function GetFilePath(ByVal strProjectPath As String, ByVal strFile As String) As String

            Dim strPath As String
            If Not IsFullPath(strFile) Then
                If Right(strProjectPath, 1) = "\" Then
                    strPath = strProjectPath & strFile
                Else
                    strPath = strProjectPath & "\" & strFile
                End If
            Else
                strPath = strFile
            End If

            Return strPath
        End Function

        'Public Shared Function IsFileInProjectDirectory(ByVal strFilename As String, Optional ByRef strFile As String = "") As Boolean

        '    If strFilename.Trim.Length = 0 Then
        '        Return False
        '    End If

        '    If Application.ProjectPath.Trim.Length > 0 AndAlso IsFullPath(strFilename) Then
        '        Dim strPath As String
        '        Dim strTempFile As String
        '        SplitPathAndFile(strFilename, strPath, strTempFile)
        '        If strPath.Trim.ToUpper.Substring(0, Application.ProjectPath.Length) = Application.ProjectPath.Trim.ToUpper Then
        '            'We now need to remove the path from the full filename to get the actual relative path to the file.
        '            strFile = strFilename.Substring(Application.ProjectPath.Length)
        '            Return True
        '        Else
        '            Return False
        '        End If
        '    Else
        '        Return True
        '    End If

        'End Function

        Public Shared Function DetermineFilePath(ByVal strFilename As String, ByRef strPath As String, ByRef strFile As String) As Boolean

            Util.SplitPathAndFile(strFilename, strPath, strFile)

            Dim aryTestPath As String() = Split(strFilename, "\")
            Dim aryProjectPath As String() = Split(Util.Application.ProjectPath, "\")

            'Dont compare the last array value. It is blank for project path.
            Dim iEnd As Integer = UBound(aryProjectPath) - 1
            For iDir As Integer = 0 To iEnd
                If Not aryTestPath(iDir).ToUpper() = aryProjectPath(iDir).ToUpper() Then
                    Return False
                End If
            Next

            'If we got here then all the directories mathed to this point, so it is in the project directory.
            'Now we need to rebuild the file path in case it is in a subdirectory of the project dir.
            strPath = Util.Application.ProjectPath
            Dim iStart As Integer = UBound(aryProjectPath)
            iEnd = UBound(aryTestPath)
            strFile = ""
            For iIndex As Integer = iStart To iEnd
                If iIndex <> iEnd Then
                    strFile = strFile & aryTestPath(iIndex) & "\"
                Else
                    strFile = strFile & aryTestPath(iIndex)
                End If
            Next

            Return True
        End Function

        Public Shared Function GetFileExtension(ByVal strFilename As String) As String

            Dim strExtension As String
            Dim iDot As Integer = InStr(strFilename, ".", CompareMethod.Text)
            If iDot >= 0 Then
                strExtension = strFilename.Substring(iDot, (strFilename.Length - iDot))
            End If

            Return strExtension
        End Function

        Public Shared Sub DisplayError(ByVal exError As System.Exception)

            Try
                Util.Application.Logger.LogMsg(ManagedAnimatInterfaces.ILogger.enumLogLevel.ErrorType, "Display Error: " & exError.Message & ", Displaying: " & m_bDisplayingError)

                If Not m_bDisplayingError Then
                    m_bDisplayingError = True
                    m_frmError = New AnimatGUI.Forms.ErrorDisplay
                    m_frmError.DisplayErrorDetails = m_bDisplayErrorDetails
                    m_frmError.Size = m_szErrorFormSize
                    m_frmError.Exception = exError
                    m_frmError.ShowDialog(Util.Application)

                    m_bDisplayErrorDetails = m_frmError.DisplayErrorDetails
                    m_szErrorFormSize = m_frmError.Size

                    m_bDisplayingError = False
                End If

                m_aryErrors.Add(exError)

            Catch ex As System.Exception
                Try
                    MessageBox.Show(exError.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error)
                    m_aryErrors.Add(ex)
                    m_bDisplayingError = False
                Catch ex1 As System.Exception
                    m_bDisplayingError = False
                End Try
            Finally
                m_frmError = Nothing
            End Try
        End Sub

        Public Shared Function ShowMessage(ByVal strMessage As String, Optional ByVal strCaption As String = "", _
                                           Optional ByVal eButtons As System.Windows.Forms.MessageBoxButtons = MessageBoxButtons.OK, _
                                           Optional ByVal iWidth As Integer = -1, Optional ByVal iHeight As Integer = -1, _
                                           Optional ByVal eTextAlign As System.Windows.Forms.HorizontalAlignment = HorizontalAlignment.Center, _
                                           Optional ByVal bReadOnly As Boolean = True) As System.Windows.Forms.DialogResult

            Util.Application.Logger.LogMsg(ManagedAnimatInterfaces.ILogger.enumLogLevel.Detail, "Starting ShowMessage: '" & strMessage & "'")

            m_frmMessage = New Forms.AnimatMessageBox
            m_frmMessage.SetMessage(strMessage, eButtons, strCaption)
            m_frmMessage.StartPosition = FormStartPosition.CenterScreen
            m_frmMessage.SetWidth = iWidth
            m_frmMessage.SetHeight = iHeight
            m_frmMessage.SetTextAlign = eTextAlign
            m_frmMessage.TextReadOnly = bReadOnly

            Return m_frmMessage.ShowDialog(Util.Application)
        End Function

        Public Shared Sub AddActiveDialog(ByVal frmDialog As System.Windows.Forms.Form)
            If Not m_aryActiveDialogs.Contains(frmDialog) Then
                m_aryActiveDialogs.Add(frmDialog)
            End If
        End Sub

        Public Shared Sub RemoveActiveDialog(ByVal frmDialog As System.Windows.Forms.Form)
            If m_aryActiveDialogs.Contains(frmDialog) Then
                m_aryActiveDialogs.Remove(frmDialog)
            End If
        End Sub

        Public Shared Sub ClearActiveDialogs(Optional ByVal bClose As Boolean = True)
            For Each frmDialog As System.Windows.Forms.Form In m_aryActiveDialogs
                frmDialog.Close()
            Next
            m_aryActiveDialogs.Clear()
        End Sub

        Public Shared Function GetErrorDetails(ByVal strParentDetails As String, ByVal ex As System.Exception) As String
            Try
                Dim strDetails As String

                If strParentDetails.Trim.Length = 0 Then
                    strDetails = ex.StackTrace
                Else
                    strDetails = strParentDetails & vbCrLf & ex.StackTrace
                End If

                If Not ex.InnerException Is Nothing Then
                    strDetails = strDetails & vbCrLf & vbCrLf & ex.InnerException.Message
                    strDetails = GetErrorDetails(strDetails, ex.InnerException)
                End If

                Return strDetails
            Catch ex1 As System.Exception
                Return strParentDetails
            End Try
        End Function

        Public Shared Sub CopyDirectory(ByVal strOrigFolder As String, ByVal strNewFolder As String, Optional ByVal bErrorIfDirExists As Boolean = True)

            Dim aryDirs As String() = Directory.GetDirectories(strOrigFolder)
            Dim aryFiles As String() = Directory.GetFiles(strOrigFolder)

            If Not Directory.Exists(strNewFolder) Then
                Directory.CreateDirectory(strNewFolder)
            Else
                If bErrorIfDirExists Then
                    Throw New System.Exception("The folder '" & strOrigFolder & "' can not be copied to a new directory '" & _
                                               strNewFolder & "' becuase a directory with that name already exists.")
                End If
            End If

            Dim strFilePath As String
            Dim strFile As String
            For Each strFilePath In aryFiles
                strFile = Util.ExtractFilename(strFilePath)

                If File.Exists(strNewFolder & "\" & strFile) Then
                    File.Delete(strNewFolder & "\" & strFile)
                End If

                File.Copy((strOrigFolder & "\" & strFile), (strNewFolder & "\" & strFile))
            Next

            'Now copy any directories that exist in that folder also.
            For Each strDir As String In aryDirs
                Dim aryDirNames As String() = Split(strDir, "\")
                CopyDirectory(strDir, strNewFolder & "\" & aryDirNames(UBound(aryDirNames)))
            Next

        End Sub

        Public Shared Sub FindAssemblies(ByVal strFolder As String, ByRef aryFiles As ArrayList)

            ' Only get files that begin with the letter "c."
            Dim dirs As String() = Directory.GetFiles(strFolder, "*.dll")

            Dim dir As String
            For Each dir In dirs
                aryFiles.Add(dir)
            Next
        End Sub

        Public Shared Function GetAssembly(ByVal strFullName As String) As System.Reflection.Assembly

            Dim aryNames As String() = Split(strFullName, ".")
            Return System.Reflection.Assembly.Load(aryNames(0))

        End Function

        Public Shared Function LoadAssembly(ByVal strAssemblyPath As String, Optional ByVal bThrowError As Boolean = True) As System.Reflection.Assembly

            Try
                strAssemblyPath = Util.GetFilePath(Util.Application.ApplicationDirectory, strAssemblyPath)
                Dim assemModule As System.Reflection.Assembly = System.Reflection.Assembly.LoadFrom(strAssemblyPath)
                If assemModule Is Nothing AndAlso bThrowError Then
                    Throw New System.Exception("Unable to load the assembly '" & strAssemblyPath & "'.")
                End If

                Return assemModule

            Catch ex As System.Exception
                If bThrowError Then
                    Throw ex
                End If
            End Try
        End Function

        Public Shared Function LoadClass(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal iIndex As Integer, _
                                         ByVal doParent As AnimatGUI.Framework.DataObject, Optional ByVal bThrowError As Boolean = True) As Object
            Dim strAssemblyFile As String = ""
            Dim strClassName As String = ""
            LoadClassModuleName(oXml, iIndex, strAssemblyFile, strClassName)
            Return LoadClass(strAssemblyFile, strClassName, doParent, bThrowError)
        End Function

        Public Shared Function LoadClass(ByVal strAssemblyPath As String, ByVal strClassName As String, _
                                         ByVal doParent As AnimatGUI.Framework.DataObject, Optional ByVal bThrowError As Boolean = True) As Object
            Dim aryArgs() As Object = {doParent}
            Return LoadClass(strAssemblyPath, strClassName, aryArgs, bThrowError)
        End Function

        Public Shared Function LoadClass(ByVal strAssemblyPath As String, ByVal strClassName As String, _
                                         Optional ByVal aryArgs() As Object = Nothing, Optional ByVal bThrowError As Boolean = True) As Object

            Try
                If Right(UCase(Trim(strAssemblyPath)), 4) <> ".DLL" Then
                    strAssemblyPath = strAssemblyPath & ".dll"
                End If

                strAssemblyPath = Util.GetFilePath(Util.Application.ApplicationDirectory, strAssemblyPath)
                Dim assemModule As System.Reflection.Assembly = System.Reflection.Assembly.LoadFrom(strAssemblyPath)
                If assemModule Is Nothing AndAlso bThrowError Then
                    Throw New System.Exception("Unable to load the assembly '" & strAssemblyPath & "'.")
                End If

                Dim oData As Object = assemModule.CreateInstance(strClassName, True, Reflection.BindingFlags.Default, Nothing, aryArgs, Nothing, Nothing)

                If oData Is Nothing AndAlso bThrowError Then
                    Throw New System.Exception("The system was unable to create the class '" & strClassName & _
                                               "' from the assembly '" & assemModule.FullName & "'.")
                End If

                Return oData

            Catch ex As System.Exception
                If bThrowError Then
                    Throw ex
                End If
            End Try
        End Function

        Public Shared Function LoadClass(ByRef assemModule As System.Reflection.Assembly, ByVal strClassName As String, _
                                         ByVal doParent As AnimatGUI.Framework.DataObject, Optional ByVal bThrowError As Boolean = True) As Object
            Dim aryArgs() As Object = {doParent}
            Return LoadClass(assemModule, strClassName, aryArgs, bThrowError)
        End Function

        Public Shared Function LoadClass(ByVal strClassName As String, ByVal doParent As AnimatGUI.Framework.DataObject, Optional ByVal bThrowError As Boolean = True) As Object
            Dim aryNames() As String = Split(strClassName, ".")
            Dim strAssembly As String = aryNames(0)

            Dim aryArgs() As Object = {doParent}
            Return LoadClass(strAssembly, strClassName, aryArgs, bThrowError)
        End Function

        Public Shared Function LoadClass(ByRef assemModule As System.Reflection.Assembly, ByVal strClassName As String, _
                                         Optional ByVal aryArgs() As Object = Nothing, Optional ByVal bThrowError As Boolean = True) As Object

            Try
                Dim oData As Object = assemModule.CreateInstance(strClassName, True, Reflection.BindingFlags.Default, Nothing, aryArgs, Nothing, Nothing)

                If oData Is Nothing AndAlso bThrowError Then
                    Throw New System.Exception("The system was unable to create the class '" & strClassName & _
                                               "' from the assembly '" & assemModule.FullName & "'.")
                End If

                Return oData

            Catch ex As System.Exception
                If bThrowError Then
                    Throw ex
                End If
            End Try
        End Function

        Public Shared Function IsTypeOf(ByVal tpNew As Type, ByVal tpOriginal As Type, Optional ByVal bInheritedOnly As Boolean = True) As Boolean

            If Not tpOriginal Is Nothing AndAlso tpOriginal.IsAssignableFrom(tpNew) Then
                Return True
            End If

            If Not tpNew Is Nothing Then
                If tpNew Is tpOriginal Then
                    If bInheritedOnly Then
                        Return False
                    Else
                        Return True
                    End If
                ElseIf tpNew.BaseType Is Nothing Then
                    Return False
                ElseIf tpNew.BaseType Is tpOriginal Then
                    Return True
                Else
                    Return IsTypeOf(tpNew.BaseType, tpOriginal)
                End If
            End If

            Return False
        End Function

        Public Shared Function IsTypeOf(ByVal tpNew As Type, ByVal strOriginal As String, Optional ByVal bInheritedOnly As Boolean = True) As Boolean

            If Not tpNew Is Nothing Then
                If tpNew.ToString = strOriginal Then
                    If bInheritedOnly Then
                        Return False
                    Else
                        Return True
                    End If
                ElseIf tpNew.BaseType Is Nothing Then
                    Return False
                ElseIf tpNew.BaseType.ToString Is strOriginal Then
                    Return True
                Else
                    Return IsTypeOf(tpNew.BaseType, strOriginal)
                End If
            End If

            Return False
        End Function

        Public Overloads Shared Function RootNamespace(ByRef assemModule As System.Reflection.Assembly) As String
            Dim aryTypes() As Type = assemModule.GetTypes()
            If UBound(aryTypes) >= 0 Then
                Dim tpClass As Type = aryTypes(0)
                Dim aryName() As String = Split(tpClass.FullName, ".")

                Return aryName(0)
            End If
        End Function

        Public Overloads Shared Function RootNamespace(ByVal strAssemName As String) As String
            Dim aryName() As String = Split(strAssemName, ".")
            Return aryName(0)
        End Function

        Public Shared Function ModuleName(ByRef assemModule As System.Reflection.Assembly) As String
            Dim strModuleName As String

            Try
                strModuleName = RootNamespace(assemModule) & ".ModuleInformation"
                Dim resMan As System.Resources.ResourceManager = New System.Resources.ResourceManager(strModuleName, assemModule)

                strModuleName = resMan.GetString("ModuleName")

                If strModuleName Is Nothing Then
                    Throw New System.Exception("ModuleName not found.")
                End If

                Return strModuleName

            Catch ex As System.Exception
                Dim aryNames() As String = Split(assemModule.FullName, ",")
                Return aryNames(0)
            End Try

        End Function

        Public Overloads Shared Function ExtractIDCount(ByVal strRootname As String, ByVal aryValues As Collections.AnimatDictionaryBase, Optional ByVal strSeperator As String = "_") As Integer
            Dim strNumber As String
            Dim iNumber As Integer, iMax As Integer = -1

            strRootname = strRootname & strSeperator
            strRootname = strRootname.ToUpper
            Dim doItem As AnimatGUI.DataObjects.DragObject
            For Each deEntry As DictionaryEntry In aryValues
                If TypeOf deEntry.Value Is AnimatGUI.DataObjects.DragObject Then
                    doItem = DirectCast(deEntry.Value, AnimatGUI.DataObjects.DragObject)
                    strNumber = doItem.ItemName

                    If strNumber.Length > strRootname.Length AndAlso Left(strNumber, strRootname.Length).ToUpper = strRootname Then
                        strNumber = Right(strNumber, strNumber.Length - strRootname.Length)

                        If IsNumeric(strNumber) AndAlso Not InStr(strNumber, ".") > 0 AndAlso Not InStr(strNumber, "-") > 0 Then
                            iNumber = CInt(strNumber)
                            If iNumber > iMax OrElse iMax < 0 Then
                                iMax = iNumber
                            End If
                        End If
                    End If
                End If
            Next

            If iMax < 0 Then
                Return 0
            Else
                Return iMax
            End If
        End Function


        Public Overloads Shared Function ExtractIDCount(ByVal strRootname As String, ByVal aryValues As SortedList, Optional ByVal strSeperator As String = "_") As Integer
            Dim strNumber As String
            Dim iNumber As Integer, iMax As Integer = -1

            strRootname = strRootname & strSeperator
            strRootname = strRootname.ToUpper
            Dim doItem As AnimatGUI.DataObjects.DragObject
            For Each deEntry As DictionaryEntry In aryValues
                If TypeOf deEntry.Value Is AnimatGUI.DataObjects.DragObject Then
                    doItem = DirectCast(deEntry.Value, AnimatGUI.DataObjects.DragObject)
                    strNumber = doItem.ItemName

                    If strNumber.Length > strRootname.Length AndAlso Left(strNumber, strRootname.Length).ToUpper = strRootname Then
                        strNumber = Right(strNumber, strNumber.Length - strRootname.Length)

                        If IsNumeric(strNumber) AndAlso Not InStr(strNumber, ".") > 0 AndAlso Not InStr(strNumber, "-") > 0 Then
                            iNumber = CInt(strNumber)
                            If iNumber > iMax OrElse iMax < 0 Then
                                iMax = iNumber
                            End If
                        End If
                    End If
                End If
            Next

            If iMax < 0 Then
                Return 0
            Else
                Return iMax
            End If
        End Function

        Public Overloads Shared Sub SavePoint(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal ptPoint As PointF)
            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("x", ptPoint.X)
            oXml.SetAttrib("y", ptPoint.Y)
            oXml.OutOfElem()
        End Sub

        Public Overloads Shared Sub SavePoint(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal ptPoint As Point)
            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("x", ptPoint.X)
            oXml.SetAttrib("y", ptPoint.Y)
            oXml.OutOfElem()
        End Sub

        Public Shared Function LoadPointF(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String) As PointF
            Dim ptPoint As New PointF
            oXml.IntoChildElement(strName)

            Try
                ptPoint.X = oXml.GetAttribFloat("x")
                ptPoint.Y = oXml.GetAttribFloat("y")
            Catch ex As System.Exception
                ptPoint.X = oXml.GetAttribFloat("X")
                ptPoint.Y = oXml.GetAttribFloat("Y")
            End Try
            oXml.OutOfElem()
        End Function

        Public Shared Function LoadPoint(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String) As Point
            Dim ptPoint As New Point
            oXml.IntoChildElement(strName)

            Try
                ptPoint.X = oXml.GetAttribInt("x")
                ptPoint.Y = oXml.GetAttribInt("y")
            Catch ex As System.Exception
                ptPoint.X = oXml.GetAttribInt("X")
                ptPoint.Y = oXml.GetAttribInt("Y")
            End Try

            oXml.OutOfElem()
            Return ptPoint
        End Function

        Public Shared Function LoadColor(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal clDefault As Color) As System.Drawing.Color
            Dim r, g, b, alpha As Integer
            Dim clColor As Color

            If oXml.FindChildElement(strName, False) Then
                oXml.IntoChildElement(strName)

                r = CInt(oXml.GetAttribFloat("Red", False, CSng(clDefault.R / 255)) * 255)
                g = CInt(oXml.GetAttribFloat("Green", False, CSng(clDefault.G / 255)) * 255)
                b = CInt(oXml.GetAttribFloat("Blue", False, CSng(clDefault.B / 255)) * 255)
                alpha = CInt(oXml.GetAttribFloat("Alpha", False, 1) * 255)

                clColor = Color.FromArgb(alpha, r, g, b)

                oXml.OutOfElem()
            Else
                clColor = clDefault
            End If

            Return clColor
        End Function

        Public Shared Function LoadColor(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String) As System.Drawing.Color
            Dim r, g, b, alpha As Integer
            Dim clColor As Color

            oXml.IntoChildElement(strName)

            r = CInt(oXml.GetAttribFloat("Red") * 255)
            g = CInt(oXml.GetAttribFloat("Green") * 255)
            b = CInt(oXml.GetAttribFloat("Blue") * 255)
            alpha = CInt(oXml.GetAttribFloat("Alpha") * 255)

            clColor = Color.FromArgb(alpha, r, g, b)

            oXml.OutOfElem()

            Return clColor
        End Function

        Public Shared Function SaveColorXml(ByVal strName As String, ByVal oColor As System.Drawing.Color) As String

            Dim oXml As ManagedAnimatInterfaces.IStdXml = Util.Application.CreateStdXml()
            oXml.AddElement("Root")
            SaveColor(oXml, strName, oColor)

            Return oXml.Serialize()
        End Function

        Public Shared Sub SaveColor(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal oColor As System.Drawing.Color)

            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("Red", CSng(oColor.R / 255.0))
            oXml.SetAttrib("Green", CSng(oColor.G / 255.0))
            oXml.SetAttrib("Blue", CSng(oColor.B / 255.0))
            oXml.SetAttrib("Alpha", CSng(oColor.A / 255.0))
            oXml.OutOfElem()

        End Sub

        Public Overloads Shared Sub SaveSize(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal szSize As SizeF)
            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("Width", szSize.Width)
            oXml.SetAttrib("Height", szSize.Height)
            oXml.OutOfElem()
        End Sub

        Public Overloads Shared Sub SaveSize(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal szSize As Size)
            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("Width", szSize.Width)
            oXml.SetAttrib("Height", szSize.Height)
            oXml.OutOfElem()
        End Sub

        Public Shared Function LoadSizeF(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String) As SizeF
            Dim szSize As SizeF
            oXml.IntoChildElement(strName)
            szSize.Width = oXml.GetAttribFloat("Width")
            szSize.Height = oXml.GetAttribFloat("Height")
            oXml.OutOfElem()
            Return szSize
        End Function

        Public Shared Function LoadSize(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String) As Size
            Dim szSize As Size
            oXml.IntoChildElement(strName)
            szSize.Width = oXml.GetAttribInt("Width")
            szSize.Height = oXml.GetAttribInt("Height")
            oXml.OutOfElem()
            Return szSize
        End Function

        Public Overloads Shared Sub SaveVector(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal ptVector As Vec3i)
            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("x", ptVector.X)
            oXml.SetAttrib("y", ptVector.Y)
            oXml.SetAttrib("z", ptVector.Z)
            oXml.OutOfElem()
        End Sub

        Public Shared Function LoadVec3i(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal doParent As AnimatGUI.Framework.DataObject) As Vec3i
            Dim ptVector As New Vec3i(doParent)
            oXml.IntoChildElement(strName)
            ptVector.X = oXml.GetAttribInt("x")
            ptVector.Y = oXml.GetAttribInt("y")
            ptVector.Z = oXml.GetAttribInt("z")
            oXml.OutOfElem()
            Return ptVector
        End Function

        Public Overloads Shared Sub SaveVector(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal ptVector As Vec3d)
            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("x", ptVector.X)
            oXml.SetAttrib("y", ptVector.Y)
            oXml.SetAttrib("z", ptVector.Z)
            oXml.OutOfElem()
        End Sub

        Public Shared Function LoadVec3d(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal doParent As AnimatGUI.Framework.DataObject) As Vec3d
            Dim ptVector As New Vec3d(doParent)
            oXml.IntoChildElement(strName)
            ptVector.X = oXml.GetAttribDouble("x")
            ptVector.Y = oXml.GetAttribDouble("y")
            ptVector.Z = oXml.GetAttribDouble("z")
            oXml.OutOfElem()
            Return ptVector
        End Function

        Public Shared Sub SaveFont(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String, ByVal oFont As Font)
            oXml.AddChildElement(strName)
            oXml.IntoElem()
            oXml.SetAttrib("Family", oFont.Name)
            oXml.SetAttrib("Size", oFont.Size)
            oXml.SetAttrib("Bold", oFont.Bold)
            oXml.SetAttrib("Underline", oFont.Underline)
            oXml.SetAttrib("Strikeout", oFont.Strikeout)
            oXml.SetAttrib("Italic", oFont.Italic)
            oXml.OutOfElem()
        End Sub

        Public Shared Function LoadFont(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strName As String) As Font
            oXml.IntoChildElement(strName)
            Dim strFamily As String = oXml.GetAttribString("Family")
            Dim fltSize As Single = oXml.GetAttribFloat("Size")
            Dim bBold As Boolean = oXml.GetAttribBool("Bold")
            Dim bUnderline As Boolean = oXml.GetAttribBool("Underline")
            Dim bStrikeout As Boolean = oXml.GetAttribBool("Strikeout")
            Dim bItalic As Boolean = oXml.GetAttribBool("Italic")
            oXml.OutOfElem()

            Dim eStyle As System.Drawing.FontStyle
            If bBold Then eStyle = eStyle Or System.Drawing.FontStyle.Bold
            If bUnderline Then eStyle = eStyle Or System.Drawing.FontStyle.Underline
            If bStrikeout Then eStyle = eStyle Or System.Drawing.FontStyle.Strikeout
            If bItalic Then eStyle = eStyle Or System.Drawing.FontStyle.Italic

            Return New Font(strFamily, fltSize, eStyle)
        End Function

        Public Function IntersectLineLine(ByVal a1 As Point, ByVal a2 As Point, ByVal b1 As Point, ByVal b2 As Point, ByRef result As Point) As Boolean

            Dim ua_t As Double = (b2.X - b1.X) * (a1.Y - b1.Y) - (b2.Y - b1.Y) * (a1.X - b1.X)
            Dim ub_t As Double = (a2.X - a1.X) * (a1.Y - b1.Y) - (a2.Y - a1.Y) * (a1.X - b1.X)
            Dim u_b As Double = (b2.Y - b1.Y) * (a2.X - a1.X) - (b2.X - b1.X) * (a2.Y - a1.Y)

            If u_b <> 0 Then

                Dim ua As Double = ua_t / u_b
                Dim ub As Double = ub_t / u_b

                If 0 <= ua AndAlso ua <= 1 AndAlso 0 <= ub AndAlso ub <= 1 Then
                    result = New Point(CInt(a1.X + ua * (a2.X - a1.X)), CInt(a1.Y + ua * (a2.Y - a1.Y)))
                    IntersectLineLine = True
                Else
                    IntersectLineLine = False
                End If
            Else
                If ua_t = 0 OrElse ub_t = 0 Then
                    IntersectLineLine = False
                Else
                    IntersectLineLine = False
                End If
            End If
        End Function

        Public Shared ReadOnly Property RadiansToDegreesRatio() As Double
            Get
                Return m_dblRadiansToDegreeRatio
            End Get
        End Property

        Public Shared ReadOnly Property DegreesToRadiansRatio() As Double
            Get
                Return m_dblDegreeToRadiansRatio
            End Get
        End Property

        Public Shared Function DegreesToRadians(ByVal fltDegrees As Single) As Single

            If fltDegrees > 360 OrElse fltDegrees < -360 Then
                Dim iMod As Integer = CInt(Math.Abs(fltDegrees / 360))
                fltDegrees = fltDegrees / iMod
            End If

            If fltDegrees = -360 OrElse fltDegrees = 360 Then fltDegrees = 0

            Return CSng((fltDegrees / 180.0) * Math.PI)
        End Function

        Public Shared Function RadiansToDegrees(ByVal fltRadians As Single) As Single
            Return CSng((fltRadians / Math.PI) * 180.0)
        End Function

        Public Shared Function LoadID(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strRootName As String, Optional ByVal bUseDefault As Boolean = False, Optional ByVal strDefault As String = "") As String
            Dim strID As String = ""

            If bUseDefault Then
                If oXml.FindChildElement(strRootName & "ID", False) Then
                    strID = oXml.GetChildString(strRootName & "ID", strDefault)
                Else
                    strID = oXml.GetChildString(strRootName & "Key", strDefault)
                End If
            Else
                If oXml.FindChildElement(strRootName & "ID", False) Then
                    strID = oXml.GetChildString(strRootName & "ID")
                Else
                    strID = oXml.GetChildString(strRootName & "Key")
                End If
            End If

            Return strID
        End Function



        Public Shared Function Distance(ByVal vA As AnimatGUI.Framework.Vec3d, ByVal vB As AnimatGUI.Framework.Vec3d) As Double
            Return Math.Sqrt(Math.Pow(vA.X - vB.X, 2) + Math.Pow(vA.Y - vB.Y, 2) + Math.Pow(vA.Z - vB.Z, 2))
        End Function

        Public Shared Sub RegExRelableWithConfirm(ByVal aryObjects As ArrayList, ByVal strMatch As String, ByVal strReplace As String)
            Dim frmConfirm As New AnimatGUI.Forms.ConfirmRelabel
            frmConfirm.Items = aryObjects
            frmConfirm.Match = strMatch
            frmConfirm.Replace = strReplace

            If frmConfirm.ShowDialog() = DialogResult.OK Then
                RegExRelable(aryObjects, strMatch, strReplace)
            End If

        End Sub

        Public Shared Sub RegExRelable(ByVal aryObjects As ArrayList, ByVal strMatch As String, ByVal strReplace As String)

            Try

                ModificationHistory.BeginHistoryGroup()
                Dim doData As AnimatGUI.Framework.DataObject
                For Each oObj As Object In aryObjects
                    If TypeOf oObj Is AnimatGUI.Framework.DataObject Then
                        doData = DirectCast(oObj, AnimatGUI.Framework.DataObject)
                        If Regex.IsMatch(doData.Name, strMatch) Then
                            doData.Name = Regex.Replace(doData.Name, strMatch, strReplace)
                        End If
                    End If
                Next
                ModificationHistory.CommitHistoryGroup()

            Catch ex As System.Exception
                ModificationHistory.AbortHistoryGroup()
                Throw ex
            End Try

        End Sub

        Public Shared Sub RelableSelected(ByVal aryObjects As ArrayList, ByVal strLabel As String, ByVal iStartWith As Integer, ByVal iIncrementBy As Integer)

            Try

                ModificationHistory.BeginHistoryGroup()
                Dim doData As AnimatGUI.Framework.DataObject
                Dim iNum As Integer = iStartWith
                For Each oObj As Object In aryObjects
                    If TypeOf oObj Is AnimatGUI.Framework.DataObject Then
                        doData = DirectCast(oObj, AnimatGUI.Framework.DataObject)
                        doData.Name = Replace(strLabel, "%NUM%", iNum.ToString)
                        iNum = iNum + iIncrementBy
                    End If
                Next
                ModificationHistory.CommitHistoryGroup()

            Catch ex As System.Exception
                ModificationHistory.AbortHistoryGroup()
                Throw ex
            End Try

        End Sub

        Public Shared Function RemoveStringSections(ByVal strText As String, ByVal strDelim As String, ByVal iSectionsToRemove As Integer, Optional ByVal bStartAtRear As Boolean = True) As String

            If strDelim.Length <= 0 Then
                Throw New System.Exception("No Delimiter was specified.")
            End If

            Dim aryVals() As String = strText.Split(strDelim.Chars(0))
            Dim strOut As String = ""

            If aryVals.Length > 0 Then
                'If the last entry is blank then ignore it.
                If aryVals(aryVals.Length - 1).Trim.Length = 0 Then
                    ReDim Preserve aryVals(aryVals.Length - 2)
                End If

                If iSectionsToRemove > aryVals.Length Then
                    Throw New System.Exception("You are attempting to remove " & iSectionsToRemove & " from a string that only has " & aryVals.Length & " sections.")
                End If

                Dim iStart As Integer = 0
                Dim iEnd As Integer = 0

                If aryVals.Length > 0 Then
                    If bStartAtRear Then
                        iStart = 0
                        iEnd = aryVals.Length - iSectionsToRemove - 1
                    Else
                        iStart = iSectionsToRemove
                        iEnd = aryVals.Length - 1
                    End If

                    For iIndex As Integer = iStart To iEnd
                        If iIndex = iStart Then
                            strOut = aryVals(iIndex)
                        Else
                            strOut = strOut & "\" & aryVals(iIndex)
                        End If
                    Next
                End If
            End If

            Return strOut
        End Function

        Protected Shared Sub LoadUserConfigData(ByVal strFilePath As String)

            Try
                If File.Exists(strFilePath) Then
                    Dim xmlConfig As ManagedAnimatInterfaces.IStdXml = Util.Application.CreateStdXml()

                    xmlConfig.Load(strFilePath)
                    xmlConfig.FindElement("Config")

                    Util.Application.DefaultNewFolder = xmlConfig.GetChildString("DefaultNewFolder", "")

                    If Util.Application.DefaultNewFolder.Trim.Length = 0 Then
                        Util.Application.DefaultNewFolder = System.Environment.GetFolderPath(System.Environment.SpecialFolder.MyDocuments) & "\AnimatLab"
                    End If

                    Dim strAutoupdate As String = xmlConfig.GetChildString("UpdateFrequency", Util.Application.AutoUpdateInterval.ToString)
                    Util.Application.AutoUpdateInterval = DirectCast([Enum].Parse(GetType(Forms.AnimatApplication.enumAutoUpdateInterval), strAutoupdate, True), Forms.AnimatApplication.enumAutoUpdateInterval)

                    Try
                        Dim strDate As String = xmlConfig.GetChildString("UpdateTime", "1/1/2001")
                        Util.Application.LastAutoUpdateTime = Date.Parse(strDate)
                    Catch exDate As System.Exception
                        'If for some reason it fails on the parsing of the update time then set it to some time way in the past.
                        Util.Application.LastAutoUpdateTime = DateTime.Parse("1/1/2001")
                    End Try
                End If

            Catch ex As System.Exception
                'If for some reason it fails on the parsing of the update time then set it to some time way in the past.
                Util.Application.LastAutoUpdateTime = DateTime.Parse("1/1/2001")
                Util.Application.DefaultNewFolder = ""
                Util.Application.AutoUpdateInterval = Forms.AnimatApplication.enumAutoUpdateInterval.Daily

                'The nUnit system eats my configuration settings, so I am changing this to only show this error
                'when we are in production mode so this error does not get shown and these values are just defaulted.
#If Not Debug Then
                AnimatGUI.Framework.Util.DisplayError(ex)
#End If
            End Try
        End Sub

        Protected Shared Sub SaveUserConfigData(ByVal xmlData As ManagedAnimatInterfaces.IStdXml, ByVal dtUpdateTime As DateTime)

            Dim strUpdateTime As String = dtUpdateTime.Month.ToString() & "/" & dtUpdateTime.Day.ToString & "/" & dtUpdateTime.Year.ToString

            xmlData.AddChildElement("UpdateFrequency", Util.Application.AutoUpdateInterval.ToString)
            xmlData.AddChildElement("UpdateTime", strUpdateTime)
            xmlData.AddChildElement("DefaultNewFolder", Util.Application.DefaultNewFolder)

        End Sub

        Protected Shared Sub CreateUserConfigFile(ByVal strFilePath As String, ByVal dtUpdateTime As DateTime)
            Dim xmlConfig As ManagedAnimatInterfaces.IStdXml = Util.Application.CreateStdXml()

            xmlConfig.AddElement("Config")

            SaveUserConfigData(xmlConfig, dtUpdateTime)

            xmlConfig.Save(strFilePath)
        End Sub

        Protected Shared Function CheckAppDataFile() As String
            Dim strAppDataPath As String = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData)
            Dim strAnimatAppDataPath As String = strAppDataPath & "\AnimatLab"
            Dim strAnimatConfigPath As String = strAnimatAppDataPath & "\AnimatUser.config"

            If Not Directory.Exists(strAnimatAppDataPath) Then
                Directory.CreateDirectory(strAnimatAppDataPath)
            End If

            If Not File.Exists(strAnimatConfigPath) Then
                Dim dtDefaultTime As New DateTime(2001, 1, 1)
                CreateUserConfigFile(strAnimatConfigPath, dtDefaultTime)
            End If

            Return strAnimatConfigPath
        End Function

        Public Shared Sub LoadUserConfigData()
            Try
                Dim strAnimatConfigPath As String = CheckAppDataFile()
                LoadUserConfigData(strAnimatConfigPath)
            Catch ex As Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Public Shared Sub SaveUserConfigData()
            Try
                Dim strAnimatConfigPath As String = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData) & "\AnimatLab\AnimatUser.config"
                CreateUserConfigFile(strAnimatConfigPath, Util.Application.LastAutoUpdateTime)
            Catch ex As Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

        Public Shared Function LoadGain(ByVal strGainName As String, ByVal strPropName As String, ByRef oParent As Framework.DataObject, ByVal oXml As ManagedAnimatInterfaces.IStdXml) As AnimatGUI.DataObjects.Gain
            Dim gnGain As AnimatGUI.DataObjects.Gain

            Try

                If oXml.FindChildElement(strGainName, False) Then
                    oXml.IntoChildElement(strGainName)
                    Dim strAssemblyFile As String = oXml.GetChildString("AssemblyFile")
                    Dim strClassName As String = oXml.GetChildString("ClassName")
                    oXml.OutOfElem()

                    gnGain = DirectCast(Util.LoadClass(strAssemblyFile, strClassName, oParent), AnimatGUI.DataObjects.Gain)
                    gnGain.LoadData(oXml, strGainName, strPropName)
                End If

                Return gnGain
            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Function

        Public Shared Sub ReadCSVFileToArray(ByVal strFilename As String, ByRef aryColumns() As String, ByRef aryData(,) As Double)
            Dim num_rows As Integer
            Dim num_cols As Integer
            Dim iCol As Integer
            Dim iRow As Integer

            ' Load the file.
            'Check if file exist
            If File.Exists(strfilename) Then
                Dim tmpstream As StreamReader = File.OpenText(strFilename)
                Dim aryLines() As String
                Dim aryLine() As String

                'Load content of file to strLines array
                Dim strData As String = tmpstream.ReadToEnd()
                aryLines = strData.Split(vbLf.ToCharArray())

                If (aryLines.Length < 2) Then
                    Throw New System.Exception("No data in file: " & strFilename)
                End If

                aryColumns = aryLines(0).Split(vbTab.ToCharArray)
                Dim iChop As Integer = FindEndingBlanks(aryColumns)
                ReDim Preserve aryColumns(aryColumns.Length - (1 + iChop))

                'Remove one for the header and one to make the index work.
                num_rows = aryLines.Length - 1 - 1
                num_cols = aryColumns.Length - 1

                ReDim aryData(num_cols, num_rows)

                ' Copy the data into the array. Skip the header row.
                For iRow = 1 To num_rows
                    aryLine = aryLines(iRow).Split(vbTab.ToCharArray)

                    For iCol = 0 To num_cols
                        aryData(iCol, iRow) = CDbl(aryLine(iCol))
                    Next
                Next

                tmpstream.Close()
            End If

        End Sub

        Public Shared Sub ReadCSVFileToArrayUsingTemplate(ByVal strFilename As String, ByVal aryTemplateColumns() As String, ByRef aryColumns() As String, ByRef aryData(,) As Double)
            Dim num_rows As Integer
            Dim num_cols As Integer
            Dim iCol As Integer
            Dim iRow As Integer

            ' Load the file.
            'Check if file exist
            If File.Exists(strFilename) Then
                Dim tmpstream As StreamReader = File.OpenText(strFilename)
                Dim aryLines() As String
                Dim aryLine() As String

                'Load content of file to strLines array
                Dim strData As String = tmpstream.ReadToEnd()
                aryLines = strData.Split(vbLf.ToCharArray())

                If (aryLines.Length < 2) Then
                    Throw New System.Exception("No data in file: " & strFilename)
                End If

                aryColumns = aryLines(0).Split(vbTab.ToCharArray)
                Dim iChop As Integer = FindEndingBlanks(aryColumns)
                ReDim Preserve aryColumns(aryColumns.Length - (1 + iChop))

                Dim aryColMapping As ArrayList = GetColumnMapping(aryTemplateColumns, aryColumns)

                'Remove one for the header and one to make the index work.
                num_rows = aryLines.Length - 1 - 1
                num_cols = aryColumns.Length - 1

                ReDim aryData(aryTemplateColumns.Length - 1, num_rows)

                Dim iInsertIdx As Integer = -1
                ' Copy the data into the array. Skip the header row.
                For iRow = 1 To num_rows
                    aryLine = aryLines(iRow).Split(vbTab.ToCharArray)

                    For iCol = 0 To num_cols
                        iInsertIdx = CInt(aryColMapping(iCol))
                        If iInsertIdx >= 0 Then
                            aryData(iInsertIdx, iRow) = CDbl(aryLine(iCol))
                        End If
                    Next
                Next

                aryColumns = aryTemplateColumns
                tmpstream.Close()
            End If

        End Sub

        Protected Shared Function GetColumnMapping(ByVal aryTempCols() As String, ByVal aryCols() As String) As ArrayList

            Dim aryList As New ArrayList
            Dim iTotal As Integer = aryCols.Length - 1
            Dim iTempIdx As Integer = -1
            For iColIdx As Integer = 0 To iTotal
                iTempIdx = FindTemplateColumnIndex(aryTempCols, aryCols(iColIdx))
                aryList.Add(iTempIdx)
            Next

            Return aryList
        End Function

        Protected Shared Function FindTemplateColumnIndex(ByVal aryTempCols() As String, ByVal strColName As String) As Integer
            Dim iIdx As Integer = -1
            For Each strCol As String In aryTempCols
                iIdx = iIdx + 1
                If strCol.Trim.ToUpper = strColName.Trim.ToUpper Then
                    Return iIdx
                End If
            Next

            Return -1
        End Function

        Protected Shared Function FindEndingBlanks(ByVal aryColumns() As String) As Integer

            Dim iBlank As Integer = 0
            Dim iIdx As Integer = aryColumns.Length - 1
            While iIdx >= 0 AndAlso aryColumns(iIdx).Trim.Length = 0
                iBlank = iBlank + 1
                iIdx = iIdx - 1
            End While

            Return iBlank
        End Function

        Public Shared Function GetValidationBoundsString(ByVal bCheckLowBound As Boolean, ByVal dblLowBound As Double, _
                                             ByVal bCheckUpperBound As Boolean, ByVal dblUpperBound As Double, ByVal bInclusiveLimit As Boolean) As String
            If Not bInclusiveLimit Then
                If bCheckLowBound AndAlso bCheckUpperBound Then
                    Return "greater than " & dblLowBound & " and less than " & dblUpperBound & "."
                ElseIf bCheckLowBound AndAlso Not bCheckUpperBound Then
                    Return "greater than " & dblLowBound & "."
                ElseIf Not bCheckLowBound AndAlso bCheckUpperBound Then
                    Return "less than " & dblUpperBound & "."
                Else
                    Return "."
                End If
            Else
                If bCheckLowBound AndAlso bCheckUpperBound Then
                    Return "between " & dblLowBound & " and " & dblUpperBound & "."
                ElseIf bCheckLowBound AndAlso Not bCheckUpperBound Then
                    Return "greater than or equal to " & dblLowBound & "."
                ElseIf Not bCheckLowBound AndAlso bCheckUpperBound Then
                    Return "less than or equal to " & dblUpperBound & "."
                Else
                    Return "."
                End If
            End If
        End Function

        Public Shared Function ValidateNumericTextBox(ByVal strText As String, ByVal bCheckLowBound As Boolean, ByVal dblLowBound As Double, _
                                             ByVal bCheckUpperBound As Boolean, ByVal dblUpperBound As Double, ByVal bInclusiveLimit As Boolean, _
                                             ByVal strTextBoxName As String) As Double
            'Check if blank
            If strText.Trim.Length <= 0 Then
                Throw New System.Exception("The " & strTextBoxName & " cannot be blank.")
            End If

            Dim dblResult As Double
            If Not Double.TryParse(strText, dblResult) Then
                Throw New System.Exception("The " & strTextBoxName & " must be a number " & GetValidationBoundsString(bCheckLowBound, dblLowBound, bCheckUpperBound, dblUpperBound, bInclusiveLimit))
            End If

            If bCheckLowBound AndAlso ((bInclusiveLimit AndAlso dblResult <= dblLowBound) OrElse (Not bInclusiveLimit AndAlso dblResult < dblLowBound)) Then
                Throw New System.Exception("The " & strTextBoxName & " must be a number " & GetValidationBoundsString(bCheckLowBound, dblLowBound, bCheckUpperBound, dblUpperBound, bInclusiveLimit))
            End If

            If bCheckUpperBound AndAlso ((bInclusiveLimit AndAlso dblResult >= dblUpperBound) OrElse (Not bInclusiveLimit AndAlso dblResult > dblUpperBound)) Then
                Throw New System.Exception("The " & strTextBoxName & " must be a number " & GetValidationBoundsString(bCheckLowBound, dblLowBound, bCheckUpperBound, dblUpperBound, bInclusiveLimit))
            End If

            Return dblResult
        End Function


        Public Shared Function FindTreeNodeByName(ByVal strName As String, ByVal aryNodes As Crownwood.DotNetMagic.Controls.NodeCollection, Optional ByVal bRecursive As Boolean = True) As Crownwood.DotNetMagic.Controls.Node
            Dim tnFound As Crownwood.DotNetMagic.Controls.Node

            Dim strNodeText As String
            Dim strReturn As String = vbCrLf
            For Each tnNode As Crownwood.DotNetMagic.Controls.Node In aryNodes
                'Remove any returns before comparing.
                strNodeText = tnNode.Text.Replace(strReturn, "")
                If strNodeText = strName Then
                    Return tnNode
                End If

                If bRecursive Then
                    tnFound = FindTreeNodeByName(strName, tnNode.Nodes)
                    If Not tnFound Is Nothing Then
                        Return tnFound
                    End If
                End If
            Next

            Return Nothing
        End Function

        Public Shared Function FindTreeNodeByPath(ByVal strPath As String, ByVal aryNodes As Crownwood.DotNetMagic.Controls.NodeCollection) As Crownwood.DotNetMagic.Controls.Node

            Dim tnNode As Crownwood.DotNetMagic.Controls.Node
            Dim aryPath() As String = Split(strPath, "\")

            For Each strName As String In aryPath
                tnNode = FindTreeNodeByName(strName, aryNodes, False)
                If tnNode Is Nothing Then
                    Throw New System.Exception("The node '" & strName & "' was not found in the path: '" & strPath & "' of the treeview.")
                End If
                aryNodes = tnNode.Nodes
            Next

            Return tnNode
        End Function

        Public Shared Function FindListItemByName(ByVal strName As String, ByVal aryItems As Windows.Forms.ListView.ListViewItemCollection, Optional ByVal bThrowError As Boolean = True, Optional ByVal strSubName As String = "") As ListViewItem

            For Each liItem As ListViewItem In aryItems
                If liItem.Text = strName Then
                    If strSubName.Length <= 0 OrElse (strSubName.Length > 0 AndAlso strSubName = liItem.SubItems(1).Text) Then
                        Return liItem
                    End If
                End If
            Next

            If bThrowError Then
                Throw New System.Exception("No listview item with the name '" & strName & "' was found.")
            End If

            Return Nothing
        End Function

        Public Shared Function FindComboItemByName(ByVal strName As String, ByVal aryItems As Windows.Forms.ComboBox.ObjectCollection, Optional ByVal bThrowError As Boolean = True) As Framework.DataObject

            For Each doItem As Framework.DataObject In aryItems
                If doItem.Name = strName Then
                    Return doItem
                End If
            Next

            If bThrowError Then
                Throw New System.Exception("No combobox item with the name '" & strName & "' was found.")
            End If

            Return Nothing
        End Function

        Public Shared Sub GetObjectProperty(ByVal strPropertyName As String, ByRef piAutomationPropInfo As PropertyInfo, ByRef oObj As Object)

            Dim aryPropPath() As String = Split(strPropertyName, ".")
            Dim iIdx As Integer = 0
            For Each strPropName As String In aryPropPath
                piAutomationPropInfo = oObj.GetType().GetProperty(strPropName)

                If piAutomationPropInfo Is Nothing Then
                    Throw New System.Exception("Property name '" & strPropName & "' not found in Path '" & strPropertyName & "'.")
                End If

                iIdx = iIdx + 1
                'Dont get the obj on the last one.
                If iIdx < aryPropPath.Length Then
                    oObj = piAutomationPropInfo.GetValue(oObj, Nothing)
                End If
            Next

        End Sub

        Public Shared Sub SetObjectPropertyDirect(ByVal oObj As Object, ByVal strPropertyName As String, ByVal strValue As String)
            Dim piAutomationPropInfo As PropertyInfo
            Dim oAutomationPropertyValue As Object

            Util.GetObjectProperty(strPropertyName, piAutomationPropInfo, oObj)

            oAutomationPropertyValue = TypeDescriptor.GetConverter(piAutomationPropInfo.PropertyType).ConvertFromString(strValue)
            piAutomationPropInfo.SetValue(oObj, oAutomationPropertyValue, Nothing)
            Util.ProjectWorkspace.RefreshProperties()
        End Sub

        Public Shared Sub SetObjectProperty(ByVal oObj As Object, ByVal strPropertyName As String, ByVal strValue As String)
            Dim piAutomationPropInfo As PropertyInfo
            Dim oAutomationPropertyValue As Object

            Dim oMethod As MethodInfo = oObj.GetType().GetMethod("Properties_SetValue")
            If Not oMethod Is Nothing Then
                Util.GetObjectProperty(strPropertyName, piAutomationPropInfo, oObj)

                oAutomationPropertyValue = TypeDescriptor.GetConverter(piAutomationPropInfo.PropertyType).ConvertFromString(strValue)

                Dim propSpec As New AnimatGuiCtrls.Controls.PropertySpec(piAutomationPropInfo.Name, piAutomationPropInfo.PropertyType, piAutomationPropInfo.Name)
                Dim propEvent As New AnimatGuiCtrls.Controls.PropertySpecEventArgs(propSpec, oAutomationPropertyValue)

                Dim aryParams As Object() = New Object() {oObj, propEvent}
                Dim oRet As Object = oMethod.Invoke(oObj, aryParams)

                If Not Util.ProjectWorkspace Is Nothing Then
                    Util.ProjectWorkspace.RefreshProperties()
                End If
            Else
                SetObjectPropertyDirect(oObj, strPropertyName, strValue)
            End If

        End Sub

        Public Shared Function GetObjectProperty(ByVal oObj As Object, ByVal strPropertyName As String) As Object

            Dim aryPropPath() As String = Split(strPropertyName, ".")
            Dim iIdx As Integer = 0
            Dim piAutomationPropInfo As PropertyInfo
            For Each strPropName As String In aryPropPath
                piAutomationPropInfo = oObj.GetType().GetProperty(strPropName)

                If piAutomationPropInfo Is Nothing Then
                    Throw New System.Exception("Property name '" & strPropName & "' not found in Path '" & strPropertyName & "'.")
                End If

                iIdx = iIdx + 1
                'Dont get the obj on the last one.
                If iIdx < aryPropPath.Length Then
                    oObj = piAutomationPropInfo.GetValue(oObj, Nothing)
                End If
            Next

            Dim obj As Object = piAutomationPropInfo.GetValue(oObj, Nothing)
            Return obj
        End Function

        Public Shared Sub GetParentObjectProperty(ByVal oObj As Object, ByVal strPropertyName As String, ByRef oRoot As Object, ByRef doRoot As DataObject, ByRef strRootPropName As String)

            Dim aryPropPath() As String = Split(strPropertyName, ".")

            If aryPropPath.Count > 1 Then
                Dim iIdx As Integer = 0
                Dim piAutomationPropInfo As PropertyInfo
                For Each strPropName As String In aryPropPath
                    piAutomationPropInfo = oObj.GetType().GetProperty(strPropName)

                    If piAutomationPropInfo Is Nothing Then
                        Throw New System.Exception("Property name '" & strPropName & "' not found in Path '" & strPropertyName & "'.")
                    End If

                    iIdx = iIdx + 1
                    'Dont get the obj on the last one.
                    If iIdx < aryPropPath.Length Then
                        oObj = piAutomationPropInfo.GetValue(oObj, Nothing)
                    Else
                        oRoot = oObj
                        If Util.IsTypeOf(oObj.GetType(), GetType(DataObject), False) Then
                            doRoot = DirectCast(oObj, DataObject)
                        Else
                            doRoot = Nothing
                        End If
                        strRootPropName = strPropName
                    End If
                Next
            Else
                oRoot = oObj
                If Util.IsTypeOf(oObj.GetType(), GetType(DataObject), False) Then
                    doRoot = DirectCast(oObj, DataObject)
                Else
                    doRoot = Nothing
                End If
                strRootPropName = strPropertyName
            End If

        End Sub

        Public Shared Sub LoadClassModuleName(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal iIndex As Integer, _
                                              ByRef strAssemblyFile As String, ByRef strClassName As String, Optional ByVal bThrowError As Boolean = True)

            Try
                oXml.FindChildByIndex(iIndex)
                oXml.IntoElem() 'Into Node element
                strAssemblyFile = oXml.GetChildString("AssemblyFile")
                strClassName = oXml.GetChildString("ClassName")
                oXml.OutOfElem() 'Outof Node element
            Catch ex As Exception
                If bThrowError Then
                    Throw ex
                End If
            End Try
        End Sub

        Public Shared Function GetName(ByVal strPrimName As String, ByVal strAltName As String) As String
            If strPrimName.Length > 0 Then
                Return strPrimName
            Else
                Return strAltName
            End If
        End Function

        '    Public Function IntersectLineRectangle(ByVal a1 As Point, ByVal a2 As Point, ByVal topRight As Point, ByVal bottomLeft As Point) As Boolean

        '        if(IntersectLineLine(min, topRight, a1, a2);
        '        var inter2 = Intersection.intersectLineLine(topRight, max, a1, a2);
        '        var inter3 = Intersection.intersectLineLine(max, bottomLeft, a1, a2);
        '        var inter4 = Intersection.intersectLineLine(bottomLeft, min, a1, a2);

        'var result = new Intersection("No Intersection");

        'result.appendPoints(inter1.points);
        'result.appendPoints(inter2.points);
        'result.appendPoints(inter3.points);
        'result.appendPoints(inter4.points);

        '        If (result.points.length > 0) Then
        '    result.status = "Intersection";

        'return result;
        '    End Function

        Public Shared Function GetXmlForPaste(ByVal data As IDataObject, ByVal strFormatType As String, ByVal strRootName As String) As ManagedAnimatInterfaces.IStdXml

            ' Get the data from the clipboard
            Dim strXml As String = DirectCast(data.GetData(strFormatType), String)
            If strXml Is Nothing OrElse strXml.Trim.Length = 0 Then
                Return Nothing
            End If

            Dim oXml As ManagedAnimatInterfaces.IStdXml = Util.Application.CreateStdXml()
            oXml.Deserialize(strXml)

            'Get the list of 
            Dim aryReplaceIDList As ArrayList = GetReplaceIDList(oXml, strRootName)

            Dim strReplacedXml As String = ReplaceIDsFromList(strXml, aryReplaceIDList)

            Dim oReplaceXml As ManagedAnimatInterfaces.IStdXml = Util.Application.CreateStdXml()
            oReplaceXml.Deserialize(strReplacedXml)

            Return oReplaceXml
        End Function

        Protected Shared Function GetReplaceIDList(ByVal oXml As ManagedAnimatInterfaces.IStdXml, ByVal strRootName As String) As ArrayList

            Dim aryRepaceList As New ArrayList
            Dim strID As String = ""
            oXml.FindElement(strRootName)
            oXml.IntoChildElement("ReplaceIDList")
            Dim iCount As Integer = oXml.NumberOfChildren() - 1
            For iIdx As Integer = 0 To iCount
                oXml.FindChildByIndex(iIdx)
                strID = oXml.GetChildString()
                aryRepaceList.Add(strID)
            Next

            Return aryRepaceList
        End Function

        Protected Shared Function ReplaceIDsFromList(ByVal strXml As String, ByVal aryReplaceIDList As ArrayList) As String

            For Each strID As String In aryReplaceIDList
                strXml = strXml.Replace(strID, System.Guid.NewGuid().ToString())
            Next

            Return strXml
        End Function

        Public Shared Function FindIDInList(ByVal aryList As ArrayList, ByVal strID As String) As Boolean

            For Each oObj As Object In aryList
                If Util.IsTypeOf(oObj.GetType, GetType(Framework.DataObject)) Then
                    Dim doObj As Framework.DataObject = DirectCast(oObj, Framework.DataObject)

                    If doObj.ID.Trim.ToUpper = strID.Trim.ToUpper Then
                        Return True
                    End If
                End If
            Next

            Return False
        End Function

        Public Shared Function GetPastedBodyPart(ByVal doParentStruct As DataObjects.Physical.PhysicalStructure, _
                                                 ByVal rbParentPart As DataObjects.Physical.RigidBody, _
                                                 ByVal bRoot As Boolean) As DataObjects.Physical.RigidBody
            Dim rbPart As DataObjects.Physical.RigidBody

            If Util.Application.BodyPasteInProgress Then
                Dim data As IDataObject = Clipboard.GetDataObject()
                If Not data Is Nothing AndAlso data.GetDataPresent("AnimatLab.BodyPlan.XMLFormat") Then
                    Dim oXml As ManagedAnimatInterfaces.IStdXml = GetXmlForPaste(data, "AnimatLab.BodyPlan.XMLFormat", "BodyPlan")

                    If Not oXml Is Nothing Then
                        oXml.FindElement("BodyPlan")
                        oXml.FindChildElement("RigidBody")

                        If Not rbParentPart Is Nothing Then
                            rbPart = DirectCast(Util.Simulation.CreateObject(oXml, "RigidBody", rbParentPart), DataObjects.Physical.RigidBody)
                        Else
                            rbPart = DirectCast(Util.Simulation.CreateObject(oXml, "RigidBody", doParentStruct), DataObjects.Physical.RigidBody)
                        End If

                        If rbPart Is Nothing Then
                            Throw New System.Exception("Unable to create the pasted part type.")
                        End If

                        rbPart.IsRoot = bRoot
                        rbPart.LoadData(doParentStruct, oXml)
                    End If
                End If
            End If

            Return rbPart
        End Function

        Public Shared Function ConvertDistanceUnits(ByVal strUnits As String) As Single

            strUnits = strUnits.ToUpper

            If strUnits = "KILOMETERS" OrElse strUnits = "KILOMETER" Then Return 1000
            If strUnits = "CENTAMETERS" OrElse strUnits = "CENTAMETER" Then Return 100
            If strUnits = "DECAMETERS" OrElse strUnits = "DECAMETER" Then Return 10
            If strUnits = "METERS" OrElse strUnits = "METER" Then Return 1
            If strUnits = "DECIMETERS" OrElse strUnits = "DECIMETER" Then Return 0.1
            If strUnits = "CENTIMETERS" OrElse strUnits = "CENTIMETER" Then Return 0.01
            If strUnits = "MILLIMETERS" OrElse strUnits = "MILLIMETER" Then Return 0.001

            Throw New System.Exception("Invalid distance units: " & strUnits)
        End Function

        Public Shared Function ConvertMassUnits(ByVal strUnits As String) As Single

            strUnits = strUnits.ToUpper

            If strUnits = "KILOGRAMS" OrElse strUnits = "KILOGRAM" Then Return 1
            If strUnits = "CENTAGRAMS" OrElse strUnits = "CENTAGRAM" Then Return 0.1
            If strUnits = "DECAGRAMS" OrElse strUnits = "DECAGRAMS" Then Return 0.01
            If strUnits = "GRAMS" OrElse strUnits = "GRAM" Then Return 0.001
            If strUnits = "DECIGRAMS" OrElse strUnits = "DECIGRAM" Then Return 0.0001
            If strUnits = "CENTIGRAMS" OrElse strUnits = "CENTIGRAM" Then Return 0.00001
            If strUnits = "MILLIGRAMS" OrElse strUnits = "MILLIGRAM" Then Return 0.000001

            Throw New System.Exception("Invalid mass units: " & strUnits)
        End Function

        Public Shared Function ConvertDisplayMassUnits(ByVal strUnits As String) As Single

            strUnits = strUnits.ToUpper

            If strUnits = "KILOGRAMS" OrElse strUnits = "KILOGRAM" Then Return 1000
            If strUnits = "CENTAGRAMS" OrElse strUnits = "CENTAGRAM" Then Return 100
            If strUnits = "DECAGRAMS" OrElse strUnits = "DECAGRAMS" Then Return 10
            If strUnits = "GRAMS" OrElse strUnits = "GRAM" Then Return 1
            If strUnits = "DECIGRAMS" OrElse strUnits = "DECIGRAM" Then Return 0.1
            If strUnits = "CENTIGRAMS" OrElse strUnits = "CENTIGRAM" Then Return 0.01
            If strUnits = "MILLIGRAMS" OrElse strUnits = "MILLIGRAM" Then Return 0.001

            Throw New System.Exception("Invalid mass units: " & strUnits)
        End Function

        Public Shared Function RemoveImageIndexTags(ByVal strXml As String) As String

            Dim bFound As Boolean = True
            Dim iStart As Integer = -1
            Dim iEnd As Integer = -1
            Do While bFound
                iStart = strXml.IndexOf("<ImageIndex>")

                If iStart >= 0 Then
                    iEnd = strXml.IndexOf("</ImageIndex>", iStart)
                    strXml = strXml.Remove(iStart, ((iEnd - iStart) + 13))
                    bFound = True
                Else
                    bFound = False
                End If
            Loop

            Return strXml
        End Function

        Public Shared Function AddTreeNode(ByVal tnParent As Crownwood.DotNetMagic.Controls.Node, _
                                           ByVal strText As String, ByVal strImage As String, Optional ByVal strSelImage As String = "", _
                                           Optional ByVal mgrImageMgr As Framework.ImageManager = Nothing) As Crownwood.DotNetMagic.Controls.Node

            If Not mgrImageMgr Is Nothing Then
                mgrImageMgr.AddImage(strImage)
            End If

            If strSelImage = "" Then
                strSelImage = strImage
            ElseIf Not mgrImageMgr Is Nothing Then
                mgrImageMgr.AddImage(strSelImage)
            End If

            Dim tnNode As New Crownwood.DotNetMagic.Controls.Node(strText)
            If Not tnParent Is Nothing Then
                tnParent.Nodes.Add(tnNode)
            End If

            If Not mgrImageMgr Is Nothing Then
                tnNode.ImageIndex = mgrImageMgr.GetImageIndex(strImage)
                tnNode.SelectedImageIndex = mgrImageMgr.GetImageIndex(strSelImage)
            End If

            Return tnNode
        End Function

        Public Shared Function CalculateBestStabilityScale() As Single

            If Not Util.Environment Is Nothing Then
                Dim aryScales As New Hashtable

                aryScales.Add("Kilometers:Kilograms", 0.01)
                aryScales.Add("Kilometers:Centagrams", 0.1)
                aryScales.Add("Kilometers:Decagrams", 0.1)
                aryScales.Add("Kilometers:Grams", 1)
                aryScales.Add("Kilometers:Decigrams", 1)
                aryScales.Add("Kilometers:Centigrams", 1)
                aryScales.Add("Kilometers:Milligrams", 10)

                aryScales.Add("Centameters:Kilograms", 0.1)
                aryScales.Add("Centameters:Centagrams", 0.1)
                aryScales.Add("Centameters:Decagrams", 1)
                aryScales.Add("Centameters:Grams", 1)
                aryScales.Add("Centameters:Decigrams", 1)
                aryScales.Add("Centameters:Centigrams", 10)
                aryScales.Add("Centameters:Milligrams", 100)

                aryScales.Add("Decameters:Kilograms", 0.1)
                aryScales.Add("Decameters:Centagrams", 1)
                aryScales.Add("Decameters:Decagrams", 1)
                aryScales.Add("Decameters:Grams", 1)
                aryScales.Add("Decameters:Decigrams", 1)
                aryScales.Add("Decameters:Centigrams", 10)
                aryScales.Add("Decameters:Milligrams", 100)

                aryScales.Add("Meters:Kilograms", 1)
                aryScales.Add("Meters:Centagrams", 1)
                aryScales.Add("Meters:Decagrams", 1)
                aryScales.Add("Meters:Grams", 10)
                aryScales.Add("Meters:Decigrams", 10)
                aryScales.Add("Meters:Centigrams", 10)
                aryScales.Add("Meters:Milligrams", 100)

                aryScales.Add("Decimeters:Kilograms", 1)
                aryScales.Add("Decimeters:Centagrams", 1)
                aryScales.Add("Decimeters:Decagrams", 1)
                aryScales.Add("Decimeters:Grams", 10)
                aryScales.Add("Decimeters:Decigrams", 10)
                aryScales.Add("Decimeters:Centigrams", 10)
                aryScales.Add("Decimeters:Milligrams", 100)

                aryScales.Add("Centimeters:Kilograms", 1)
                aryScales.Add("Centimeters:Centagrams", 1)
                aryScales.Add("Centimeters:Decagrams", 10)
                aryScales.Add("Centimeters:Grams", 10)
                aryScales.Add("Centimeters:Decigrams", 10)
                aryScales.Add("Centimeters:Centigrams", 10)
                aryScales.Add("Centimeters:Milligrams", 100)

                aryScales.Add("Millimeters:Kilograms", 1)
                aryScales.Add("Millimeters:Centagrams", 1)
                aryScales.Add("Millimeters:Decagrams", 100)
                aryScales.Add("Millimeters:Grams", 100)
                aryScales.Add("Millimeters:Decigrams", 100)
                aryScales.Add("Millimeters:Centigrams", 100)
                aryScales.Add("Millimeters:Milligrams", 1000)

                Dim strKey As String = Util.Environment.DistanceUnits.ToString & ":" & Util.Environment.MassUnits.ToString

                'Return 1
                Return CSng(aryScales(strKey))
            Else
                Return 1
            End If
 
        End Function

        Public Shared Function RemoveItemsNotOfType(ByVal aryItems As ArrayList, ByVal tpType As System.Type) As ArrayList

            Dim aryNewItems As New ArrayList
            For Each oObj As Object In aryItems
                If Util.IsTypeOf(oObj.GetType(), tpType, False) Then
                    aryNewItems.Add(oObj)
                End If
            Next

            Return aryNewItems
        End Function

    End Class

End Namespace
