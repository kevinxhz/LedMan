object frmFontSelect: TfrmFontSelect
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #23383#20307#36873#25321
  ClientHeight = 119
  ClientWidth = 333
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
  object rbtn16: TRadioButton
    Left = 24
    Top = 40
    Width = 177
    Height = 17
    Caption = '  8x16 '#33521#25991', 16x16'#28857#38453#27721#23383
    TabOrder = 1
  end
  object rbtn24: TRadioButton
    Left = 24
    Top = 63
    Width = 177
    Height = 17
    Caption = '12x24 '#33521#25991', 24x24'#28857#38453#27721#23383
    TabOrder = 2
  end
  object rbtn32: TRadioButton
    Left = 24
    Top = 86
    Width = 177
    Height = 17
    Caption = '16x32 '#33521#25991', 32x32'#28857#38453#27721#23383
    TabOrder = 3
  end
  object rbtnWindows: TRadioButton
    Left = 24
    Top = 16
    Width = 177
    Height = 17
    Caption = 'Windows '#23383#20307
    Checked = True
    TabOrder = 0
    TabStop = True
  end
  object btnOk: TButton
    Left = 233
    Top = 51
    Width = 81
    Height = 25
    Caption = #30830#23450'(&O)'
    Default = True
    ModalResult = 1
    TabOrder = 4
  end
  object btnCancel: TButton
    Left = 233
    Top = 82
    Width = 81
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 5
  end
end
