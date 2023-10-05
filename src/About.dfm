object frmAbout: TfrmAbout
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #20851#20110' YLedMan'
  ClientHeight = 167
  ClientWidth = 497
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
    Top = 24
    Width = 79
    Height = 13
    Caption = #26412#31243#24207#36866#29992#20110': '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 330
    Top = 23
    Width = 87
    Height = 13
    Caption = 'gaveny@163.com'
    Visible = False
  end
  object Image1: TImage
    Left = 144
    Top = 120
    Width = 33
    Height = 33
  end
  object btnOk: TButton
    Left = 408
    Top = 128
    Width = 73
    Height = 25
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object editCardType: TEdit
    Left = 16
    Top = 56
    Width = 465
    Height = 50
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -35
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsItalic]
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
    Text = 'GS7020B GS7020E'
  end
  object Button1: TButton
    Left = 24
    Top = 128
    Width = 89
    Height = 25
    Caption = 'Button1'
    TabOrder = 2
    Visible = False
    OnClick = Button1Click
  end
end
