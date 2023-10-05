object frmBrightnessSetup: TfrmBrightnessSetup
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #37197#32622#26174#31034#23631#33258#21160#20142#24230#35843#33410
  ClientHeight = 251
  ClientWidth = 334
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
  object lbLight0: TLabel
    Left = 16
    Top = 42
    Width = 10
    Height = 13
    Caption = '1:'
  end
  object lbBrightnessStartTime: TLabel
    Left = 32
    Top = 19
    Width = 48
    Height = 13
    Caption = #24320#22987#26102#38388
  end
  object lbBrightnessValue: TLabel
    Left = 100
    Top = 19
    Width = 36
    Height = 13
    Caption = #20142#24230#20540
  end
  object lbLight1: TLabel
    Left = 16
    Top = 66
    Width = 10
    Height = 13
    Caption = '2:'
  end
  object lbLight2: TLabel
    Left = 16
    Top = 91
    Width = 10
    Height = 13
    Caption = '3:'
  end
  object lbLight3: TLabel
    Left = 16
    Top = 116
    Width = 10
    Height = 13
    Caption = '4:'
  end
  object lbLight4: TLabel
    Left = 16
    Top = 141
    Width = 10
    Height = 13
    Caption = '5:'
  end
  object lbLight7: TLabel
    Left = 16
    Top = 216
    Width = 10
    Height = 13
    Caption = '8:'
  end
  object lbLight5: TLabel
    Left = 16
    Top = 166
    Width = 10
    Height = 13
    Caption = '6:'
  end
  object lbLight6: TLabel
    Left = 16
    Top = 191
    Width = 10
    Height = 13
    Caption = '7:'
  end
  object timeStart0: TDateTimePicker
    Left = 32
    Top = 38
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 0
  end
  object timeStart1: TDateTimePicker
    Left = 32
    Top = 62
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 1
  end
  object timeStart2: TDateTimePicker
    Left = 32
    Top = 87
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 2
  end
  object timeStart3: TDateTimePicker
    Left = 32
    Top = 112
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 3
  end
  object timeStart4: TDateTimePicker
    Left = 32
    Top = 137
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 4
  end
  object timeStart5: TDateTimePicker
    Left = 32
    Top = 162
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 5
  end
  object timeStart6: TDateTimePicker
    Left = 32
    Top = 187
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 6
  end
  object timeStart7: TDateTimePicker
    Left = 32
    Top = 212
    Width = 58
    Height = 21
    Date = 40221.000000000000000000
    Format = 'HH:mm'
    Time = 40221.000000000000000000
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    Kind = dtkTime
    TabOrder = 7
  end
  object comboBoxLightValue0: TComboBox
    Left = 100
    Top = 38
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 8
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
  object comboBoxLightValue1: TComboBox
    Left = 100
    Top = 62
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 9
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
  object comboBoxLightValue2: TComboBox
    Left = 100
    Top = 87
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 10
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
  object comboBoxLightValue3: TComboBox
    Left = 100
    Top = 112
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 11
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
  object comboBoxLightValue4: TComboBox
    Left = 100
    Top = 137
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 12
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
  object comboBoxLightValue5: TComboBox
    Left = 100
    Top = 162
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 13
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
  object comboBoxLightValue6: TComboBox
    Left = 100
    Top = 187
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 14
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
  object comboBoxLightValue7: TComboBox
    Left = 100
    Top = 212
    Width = 93
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
    ItemIndex = 7
    TabOrder = 15
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
  object btnOk: TButton
    Left = 210
    Top = 179
    Width = 108
    Height = 25
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 16
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 210
    Top = 210
    Width = 108
    Height = 25
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 17
  end
end
