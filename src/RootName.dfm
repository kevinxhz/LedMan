object frmRootName: TfrmRootName
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #35813#36719#20214#31649#29702#30340#25152#26377#26174#31034#23631#30340#26681#33410#28857#21517#31216
  ClientHeight = 254
  ClientWidth = 476
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object lbOldName: TLabel
    Left = 16
    Top = 21
    Width = 88
    Height = 13
    BiDiMode = bdLeftToRight
    Caption = #24403#21069#26681#25509#28857#21517#31216':'
    ParentBiDiMode = False
  end
  object lbNewName: TLabel
    Left = 16
    Top = 191
    Width = 40
    Height = 13
    Caption = #26032#21517#31216':'
  end
  object Shape1: TShape
    Left = 8
    Top = 72
    Width = 460
    Height = 1
  end
  object btnChange: TButton
    Left = 377
    Top = 129
    Width = 81
    Height = 25
    Caption = #20462#25913
    TabOrder = 4
    OnClick = btnChangeClick
  end
  object btnCreateNew: TButton
    Left = 377
    Top = 168
    Width = 81
    Height = 25
    Caption = #26032#24314
    TabOrder = 5
    OnClick = btnCreateNewClick
  end
  object btnCancel: TButton
    Left = 377
    Top = 35
    Width = 81
    Height = 25
    Cancel = True
    Caption = #20851#38381
    DoubleBuffered = True
    ModalResult = 2
    ParentDoubleBuffered = False
    TabOrder = 2
  end
  object comboRootName: TComboBox
    Left = 16
    Top = 210
    Width = 345
    Height = 21
    TabOrder = 1
  end
  object btnJump: TButton
    Left = 377
    Top = 94
    Width = 81
    Height = 25
    Caption = #36339#36716
    TabOrder = 3
    OnClick = btnJumpClick
  end
  object editRootName: TEdit
    Left = 16
    Top = 37
    Width = 345
    Height = 21
    Color = clBtnFace
    ReadOnly = True
    TabOrder = 0
    Text = 'editRootName'
  end
  object btnDelete: TButton
    Left = 377
    Top = 208
    Width = 81
    Height = 25
    Caption = #21024#38500
    TabOrder = 6
    OnClick = btnDeleteClick
  end
end
