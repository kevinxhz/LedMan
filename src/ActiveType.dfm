object frmActiveType: TfrmActiveType
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #35774#23450#21160#24577#25991#23383#31867#22411
  ClientHeight = 115
  ClientWidth = 444
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
    Top = 15
    Width = 52
    Height = 13
    Caption = #21160#24577#31867#22411':'
  end
  object lbTimeBase: TLabel
    Left = 178
    Top = 15
    Width = 96
    Height = 13
    Caption = #20498#35745#26102#22522#20934#26085#26399#65306
  end
  object lbExtDataId: TLabel
    Left = 16
    Top = 82
    Width = 76
    Height = 13
    Caption = #22806#37096#25968#25454#32534#21495':'
  end
  object comboActiveType: TComboBox
    Left = 16
    Top = 34
    Width = 147
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 0
    Text = '4'#20301#25968#23383#24180'(2010)'
    OnClick = comboActiveTypeClick
    Items.Strings = (
      '4'#20301#25968#23383#24180'(2010)'
      '2'#20301#25968#23383#24180'(10)'
      '2'#20301#25968#23383#26376
      '2'#20301#25968#23383#26085
      '2'#20301#25968#23383#23567#26102
      '2'#20301#25968#23383#20998#38047
      '2'#20301#25968#23383#31186
      '1'#20010#27721#23383#26143#26399'('#26085'~'#20845')'
      '2'#20010#27721#23383#20892#21382#24180
      '2'#20010#27721#23383#20892#21382#26376#20221
      '2'#20010#27721#23383#20892#21382#26085#26399
      '3'#20301#25968#23383#20498#35745#26102#22825#25968
      '2'#20301#25968#23383#20498#35745#26102#22825#25968
      '1'#20301#25968#23383#20498#35745#26102#22825#25968
      #37319#38598#25968#25454'1'
      #37319#38598#25968#25454'2'
      #37319#38598#25968#25454'3'
      #37319#38598#25968#25454'4')
  end
  object dateCounterBy: TDateTimePicker
    Left = 178
    Top = 35
    Width = 121
    Height = 20
    Date = 73050.279390555560000000
    Time = 73050.279390555560000000
    DateFormat = dfLong
    TabOrder = 1
  end
  object btnOk: TButton
    Left = 329
    Top = 39
    Width = 107
    Height = 25
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object btnCancel: TButton
    Left = 329
    Top = 75
    Width = 107
    Height = 25
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 3
  end
  object cbExtDataId: TComboBox
    Left = 98
    Top = 79
    Width = 65
    Height = 21
    Style = csDropDownList
    ItemIndex = 0
    TabOrder = 4
    Text = '1'
    Items.Strings = (
      '1'
      '2'
      '3'
      '4'
      '5'
      '6'
      '7'
      '8')
  end
end
