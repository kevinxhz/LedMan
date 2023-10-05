object frmFlashTest: TfrmFlashTest
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'FLASH'#27979#35797#25968#25454
  ClientHeight = 143
  ClientWidth = 270
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 166
    Height = 13
    Caption = #35831#36755#20837'3'#20010#23383#33410#27979#35797#25968#25454'(0-255):'
  end
  object Edit1: TEdit
    Left = 16
    Top = 40
    Width = 57
    Height = 21
    TabOrder = 0
    Text = '255'
  end
  object Edit2: TEdit
    Left = 16
    Top = 67
    Width = 57
    Height = 21
    TabOrder = 1
    Text = '255'
  end
  object Edit3: TEdit
    Left = 16
    Top = 94
    Width = 57
    Height = 21
    TabOrder = 2
    Text = '255'
  end
  object btnOk: TButton
    Left = 158
    Top = 61
    Width = 89
    Height = 25
    Caption = #30830#23450'(&O)'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object btnCancel: TButton
    Left = 158
    Top = 92
    Width = 89
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 4
  end
end
