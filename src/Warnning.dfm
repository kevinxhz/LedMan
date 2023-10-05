object frmWarnning: TfrmWarnning
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #25253#35686
  ClientHeight = 207
  ClientWidth = 686
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object lbName: TLabel
    Left = 24
    Top = 24
    Width = 124
    Height = 29
    Caption = #32456#31471#21517#31216
    Color = clRed
    Font.Charset = ANSI_CHARSET
    Font.Color = clRed
    Font.Height = -29
    Font.Name = #40657#20307
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label2: TLabel
    Left = 24
    Top = 94
    Width = 280
    Height = 29
    Caption = #21457#20986#25250#21163#25253#35686#20449#24687'! '
    Color = clRed
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -29
    Font.Name = #40657#20307
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object lbTime: TLabel
    Left = 24
    Top = 59
    Width = 304
    Height = 29
    Caption = '2011-11-11 23:23:23'
    Color = clRed
    Font.Charset = ANSI_CHARSET
    Font.Color = clRed
    Font.Height = -29
    Font.Name = #40657#20307
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object btnClose: TButton
    Left = 536
    Top = 152
    Width = 137
    Height = 41
    Caption = #30830#23450
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -29
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ModalResult = 1
    ParentFont = False
    TabOrder = 0
  end
  object mediaPlayer: TMediaPlayer
    Left = 389
    Top = 24
    Width = 253
    Height = 41
    AutoOpen = True
    FileName = 'COMPB.WAV'
    Visible = False
    TabOrder = 1
  end
  object Timer1: TTimer
    Interval = 2500
    OnTimer = Timer1Timer
    Left = 560
    Top = 96
  end
end
