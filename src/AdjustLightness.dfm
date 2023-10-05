object frmAdjustLightness: TfrmAdjustLightness
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #25163#21160#35843#33410#26174#31034#23631#20142#24230
  ClientHeight = 132
  ClientWidth = 280
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
  object lbBrightnessValue: TLabel
    Left = 24
    Top = 21
    Width = 36
    Height = 13
    Caption = #20142#24230#20540
  end
  object lbBrightnessStartTime: TLabel
    Left = 24
    Top = 75
    Width = 108
    Height = 13
    Caption = #26377#25928#26102#38271'('#23567#26102':'#20998#31181')'
  end
  object comboBoxLightValue0: TComboBox
    Left = 24
    Top = 40
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 0
    Text = '7('#26368#20142')'
    Items.Strings = (
      '0 ('#20851#23631')'
      '1('#26368#26263')'
      '2('#36739#26263')'
      '3('#31245#26263')'
      '4('#20013#24230')'
      '5('#31245#20142')'
      '6('#36739#20142')'
      '7('#26368#20142')')
  end
  object timeStart0: TDateTimePicker
    Left = 24
    Top = 94
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 1
  end
  object btnOk: TButton
    Left = 154
    Top = 59
    Width = 108
    Height = 25
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object btnCancel: TButton
    Left = 154
    Top = 90
    Width = 108
    Height = 25
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 3
  end
end
