object frmSearchCard: TfrmSearchCard
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #26597#25214#26174#31034#23631
  ClientHeight = 183
  ClientWidth = 412
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
  object lbCode: TLabel
    Left = 17
    Top = 85
    Width = 80
    Height = 13
    Caption = '('#37096#20998')'#35774#22791#32534#30721
  end
  object lbDescription: TLabel
    Left = 17
    Top = 130
    Width = 64
    Height = 13
    Caption = '('#37096#20998')'#25551#36848
  end
  object lbNote: TLabel
    Left = 16
    Top = 16
    Width = 384
    Height = 13
    Caption = #24744#21487#20197#36755#20837#26174#31034#23631#25511#21046#21345#30340#21345#21495#12289#32534#21495#25110#20854#25551#36848#30340#19968#37096#20998#26469#23450#20301#35813#26174#31034#23631
  end
  object lbCardId: TLabel
    Left = 17
    Top = 42
    Width = 24
    Height = 13
    Caption = #21345#21495
  end
  object editCode: TEdit
    Left = 17
    Top = 101
    Width = 128
    Height = 21
    TabOrder = 1
  end
  object editDesc: TEdit
    Left = 17
    Top = 147
    Width = 272
    Height = 21
    TabOrder = 2
  end
  object btnSearch: TButton
    Left = 311
    Top = 118
    Width = 89
    Height = 25
    Caption = #26597#25214'(&S)'
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object btnCancel: TButton
    Left = 311
    Top = 149
    Width = 89
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 4
  end
  object editId: TEdit
    Left = 17
    Top = 58
    Width = 128
    Height = 21
    TabOrder = 0
  end
end
