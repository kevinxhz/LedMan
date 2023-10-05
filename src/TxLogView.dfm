object frmTxLogView: TfrmTxLogView
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #21457#36865#26085#24535
  ClientHeight = 593
  ClientWidth = 748
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
  object lbCount: TLabel
    Left = 24
    Top = 14
    Width = 84
    Height = 13
    Caption = #32047#35745#20849#21457#36865'10'#27425
  end
  object lbActs: TLabel
    Left = 24
    Top = 76
    Width = 88
    Height = 13
    Caption = #21457#36865#30340#33410#30446#20869#23481':'
  end
  object lbOk: TLabel
    Left = 24
    Top = 236
    Width = 124
    Height = 13
    Caption = #21457#36865#25104#21151#30340#30446#26631#26174#31034#23631':'
  end
  object lbError: TLabel
    Left = 384
    Top = 236
    Width = 124
    Height = 13
    Caption = #21457#36865#22833#36133#30340#30446#26631#26174#31034#23631':'
  end
  object lbTime: TLabel
    Left = 24
    Top = 40
    Width = 182
    Height = 13
    Caption = #31532'10'#21457#36865#26102#38388': 2010-12-12 12:12:12'
  end
  object lstAct: TListBox
    Left = 24
    Top = 100
    Width = 705
    Height = 117
    ItemHeight = 13
    TabOrder = 0
  end
  object lstCardOk: TListBox
    Left = 24
    Top = 255
    Width = 337
    Height = 322
    ItemHeight = 13
    TabOrder = 1
  end
  object lstCardFail: TListBox
    Left = 384
    Top = 255
    Width = 345
    Height = 322
    ItemHeight = 13
    TabOrder = 2
  end
  object btnPrev: TButton
    Left = 481
    Top = 8
    Width = 121
    Height = 25
    Caption = #19978#19968#27425#21457#36865#35760#24405
    TabOrder = 3
    OnClick = btnPrevClick
  end
  object btnNext: TButton
    Left = 608
    Top = 8
    Width = 121
    Height = 25
    Caption = #19979#19968#27425#21457#36865#35760#24405
    TabOrder = 4
    OnClick = btnNextClick
  end
end
