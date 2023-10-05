object frmAdjustClockSetup: TfrmAdjustClockSetup
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #33258#21160#26657#26102#26041#26696
  ClientHeight = 111
  ClientWidth = 391
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
    Top = 27
    Width = 246
    Height = 13
    Caption = #33258#21160#26657#26102#38388#38548'('#21333#20301':'#23567#26102#12290'0'#34920#31034#31105#27490#33258#21160#26657#26102')'
  end
  object Label2: TLabel
    Left = 16
    Top = 78
    Width = 36
    Height = 13
    Caption = #27599#23567#26102
  end
  object Label3: TLabel
    Left = 95
    Top = 78
    Width = 156
    Height = 13
    Caption = #20998#38047#26102#26816#26597#26159#21542#38656#35201#33258#21160#26657#26102
  end
  object editAutoClockInerval: TEdit
    Left = 16
    Top = 46
    Width = 43
    Height = 21
    TabOrder = 0
    Text = '1'
  end
  object editAutoClockMinute: TEdit
    Left = 58
    Top = 75
    Width = 31
    Height = 21
    TabOrder = 1
    Text = '57'
  end
  object btnOk: TButton
    Left = 286
    Top = 42
    Width = 89
    Height = 25
    Caption = #30830#23450'(&O)'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object btnCancel: TButton
    Left = 286
    Top = 73
    Width = 89
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 3
  end
end
