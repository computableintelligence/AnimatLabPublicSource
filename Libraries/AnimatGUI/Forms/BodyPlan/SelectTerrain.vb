﻿Imports System
Imports System.Drawing
Imports System.Collections
Imports System.ComponentModel
Imports System.Windows.Forms
Imports System.Diagnostics
Imports Crownwood.Magic.Common
Imports AnimatGuiCtrls.Controls
Imports Crownwood.Magic.Docking
Imports Crownwood.Magic.Menus
Imports AnimatGUI.Framework

Namespace Forms.BodyPlan

	Public Class SelectTerrain

        Public m_dblSegmentWidth As Double
        Public m_dblSegmentLength As Double
        Public m_dblMaxHeight As Double

        Private Sub btnMeshFileDlg_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnMeshFileDlg.Click
            Try
                Util.Application.Cursor = System.Windows.Forms.Cursors.WaitCursor

                'Office Files|*.doc;*.xls;*.ppt
                Dim openFileDialog As New OpenFileDialog
                Dim strFilter As String = "All files|*.bmp;*.gif;*.exif;*.jpg;*.jpeg;*.png;*.tiff;|" & _
                                          "bmp files (*.bmp)|*.bmp|" & _
                                          "gif files (*.gif)|*.gif|" & _
                                          "exif files (*.exif)|*.exif|" & _
                                          "jpg files (*.jpg)|*.jpg|" & _
                                          "jpeg files (*.jpeg)|*.jpeg|" & _
                                          "png files (*.png)|*.png|" & _
                                          "tiff files (*.tiff)|*.tiff"

                openFileDialog.Title = "Select a terrain height image file - hit cancel to select file later."
                openFileDialog.Filter = strFilter
                openFileDialog.InitialDirectory = Util.Application.ProjectPath

                If openFileDialog.ShowDialog() = DialogResult.OK Then
                    txtMeshFile.Text = openFileDialog.FileName
                End If

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            Finally
                Util.Application.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub

        Private Sub btnTextureFileDlg_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnTextureFileDlg.Click
            Try
                Util.Application.Cursor = System.Windows.Forms.Cursors.WaitCursor

                'Office Files|*.doc;*.xls;*.ppt
                Dim openFileDialog As New OpenFileDialog
                Dim strFilter As String = "All files|*.bmp;*.gif;*.exif;*.jpg;*.jpeg;*.png;*.tiff;|" & _
                                          "bmp files (*.bmp)|*.bmp|" & _
                                          "gif files (*.gif)|*.gif|" & _
                                          "exif files (*.exif)|*.exif|" & _
                                          "jpg files (*.jpg)|*.jpg|" & _
                                          "jpeg files (*.jpeg)|*.jpeg|" & _
                                          "png files (*.png)|*.png|" & _
                                          "tiff files (*.tiff)|*.tiff"

                openFileDialog.Title = "Select a terrain texture image file - hit cancel to select file later."
                openFileDialog.Filter = strFilter
                openFileDialog.InitialDirectory = Util.Application.ProjectPath

                If openFileDialog.ShowDialog() = DialogResult.OK Then
                    txtTextureFile.Text = openFileDialog.FileName
                End If

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            Finally
                Util.Application.Cursor = System.Windows.Forms.Cursors.Arrow
            End Try
        End Sub

        Private Sub btnOk_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles btnOk.Click
            Try
                m_dblSegmentWidth = Util.ValidateNumericTextBox(txtSegmentWidth.Text, True, 0, False, 0, False, "segment width")
                m_dblSegmentLength = Util.ValidateNumericTextBox(txtSegmentLength.Text, True, 0, False, 0, False, "segment length")
                m_dblMaxHeight = Util.ValidateNumericTextBox(txtMaxHeight.Text, True, 0, False, 0, False, "max height")

                DialogResult = Windows.Forms.DialogResult.OK
                Me.Close()

            Catch ex As System.Exception
                AnimatGUI.Framework.Util.DisplayError(ex)
            End Try
        End Sub

    End Class

End Namespace