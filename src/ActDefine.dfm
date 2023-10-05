object frmActDefine: TfrmActDefine
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #33410#30446#36866#29992#33539#22260#23450#20041
  ClientHeight = 105
  ClientWidth = 457
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
  object rdAll: TRadioButton
    Left = 16
    Top = 31
    Width = 257
    Height = 25
    Caption = #25152#36873#33410#30446#36866#29992#20110#24403#21069#26681#33410#28857#19979#30340#25152#26377#26174#31034#23631
    Checked = True
    TabOrder = 0
    TabStop = True
  end
  object rdGroup: TRadioButton
    Left = 16
    Top = 55
    Width = 297
    Height = 25
    Caption = #25152#36873#33410#30446#20165#36866#29992#20110#20108#32423#26174#31034#23631#20998#32452#20869#30340#25152#26377#26174#31034#23631
    TabOrder = 1
  end
  object btnOk: TButton
    Left = 358
    Top = 24
    Width = 81
    Height = 25
    Caption = #30830#23450'(&O)'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object btnCancel: TButton
    Left = 358
    Top = 55
    Width = 81
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 3
  end
end
