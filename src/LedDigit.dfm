object frmLedDigit: TfrmLedDigit
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #21457#36865'4'#20301'LED'#25968#30721#31649#25968#23383
  ClientHeight = 87
  ClientWidth = 227
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 24
    Width = 36
    Height = 13
    Caption = #25968#23383#65306
  end
  object editDigit: TEdit
    Left = 16
    Top = 43
    Width = 81
    Height = 21
    TabOrder = 0
    Text = '0'
  end
  object btnOk: TButton
    Left = 130
    Top = 16
    Width = 75
    Height = 21
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object btnCancel: TButton
    Left = 130
    Top = 43
    Width = 75
    Height = 21
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 2
  end
end
