object frmReTxSetup: TfrmReTxSetup
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #33258#21160#37325#21457
  ClientHeight = 139
  ClientWidth = 398
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
  object Label2: TLabel
    Left = 24
    Top = 24
    Width = 108
    Height = 13
    Caption = #33410#30446#21457#36865#22833#36133#21518#27599#38548
  end
  object Label3: TLabel
    Left = 191
    Top = 24
    Width = 186
    Height = 13
    Caption = #20998#38047#33258#21160#37325#21457#65288'0'#34920#31034#19981#33258#21160#37325#21457#65289
  end
  object btnOk: TButton
    Left = 176
    Top = 105
    Width = 89
    Height = 25
    Caption = #30830#23450'(&O)'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object btnCancel: TButton
    Left = 288
    Top = 105
    Width = 89
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 1
  end
  object editAutoRetryTime: TEdit
    Left = 138
    Top = 21
    Width = 47
    Height = 21
    TabOrder = 2
    Text = '5'
  end
  object checkOnlyLogLastTx: TCheckBox
    Left = 24
    Top = 56
    Width = 313
    Height = 17
    Caption = #25163#21160#21457#36865#26102#28165#38500#20043#21069#30340#21457#36865#22833#36133#35760#24405
    TabOrder = 3
  end
end
