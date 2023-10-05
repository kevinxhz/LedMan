object frmSearchAct: TfrmSearchAct
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #26597#25214#33410#30446
  ClientHeight = 149
  ClientWidth = 406
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
  object lbText: TLabel
    Left = 19
    Top = 16
    Width = 346
    Height = 13
    Caption = #33410#30446#25991#23383#20869#23481#30340#20840#37096#25110#37096#20998#65292'*'#34920#31034#26597#25214#25152#26377#21253#21547#26377#25928#20869#23481#30340#33410#30446':'
  end
  object lbDescription: TLabel
    Left = 19
    Top = 62
    Width = 310
    Height = 13
    Caption = #33410#30446#35828#26126#30340#20840#37096#25110#37096#20998#65292'*'#34920#31034#26597#25214#25152#26377#24050#27880#26126#35828#26126#30340#33410#30446':'
  end
  object editActDesc: TEdit
    Left = 19
    Top = 81
    Width = 369
    Height = 21
    TabOrder = 1
  end
  object btnSearch: TButton
    Left = 188
    Top = 115
    Width = 89
    Height = 25
    Caption = #26597#25214'(&S)'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object btnCancel: TButton
    Left = 299
    Top = 115
    Width = 89
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 3
  end
  object editActText: TEdit
    Left = 19
    Top = 35
    Width = 369
    Height = 21
    TabOrder = 0
  end
end
