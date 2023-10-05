object frmAddCards: TfrmAddCards
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #25209#37327#26032#24314#26174#31034#23631'('#25511#21046#21345')'
  ClientHeight = 138
  ClientWidth = 405
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
  object Label2: TLabel
    Left = 24
    Top = 19
    Width = 357
    Height = 13
    Caption = '1. '#30830#35748#25152#36873#30340#26174#31034#23631'('#25511#21046#21345')'#24050#27491#30830#37197#32622#65292#19988#20854#35774#22791#32534#21495#30340#21518#19977#20301#20026
  end
  object Label3: TLabel
    Left = 24
    Top = 38
    Width = 168
    Height = 13
    Caption = #35813#25209#36830#21495#30340#25511#21046#21345#20013#26368#23567#30340#19968#20010
  end
  object Label4: TLabel
    Left = 24
    Top = 65
    Width = 257
    Height = 13
    Caption = '2. '#36755#20837#35201#26032#24314#30340#26174#31034#23631#65288#25511#21046#21345#65289#30340#25968#37327'(1-998):'
  end
  object editCount: TEdit
    Left = 288
    Top = 64
    Width = 57
    Height = 21
    TabOrder = 0
    Text = '1'
  end
  object btnOk: TButton
    Left = 214
    Top = 106
    Width = 75
    Height = 21
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 1
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 306
    Top = 106
    Width = 75
    Height = 21
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 2
  end
end
