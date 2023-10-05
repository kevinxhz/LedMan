object frmCommConfig: TfrmCommConfig
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #36890#20449#37197#32622
  ClientHeight = 362
  ClientWidth = 497
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
  object groupMethod: TGroupBox
    Left = 12
    Top = 12
    Width = 341
    Height = 75
    Caption = #36890#20449#26041#24335#36873#25321
    TabOrder = 0
    object rbtnSerialPort: TRadioButton
      Left = 18
      Top = 25
      Width = 253
      Height = 16
      Caption = #20018#21475#65288'RS232/RS485'#65289
      Checked = True
      TabOrder = 0
      TabStop = True
    end
    object rbtnGPRS: TRadioButton
      Left = 18
      Top = 47
      Width = 253
      Height = 16
      Caption = '3G/GPRS/CDMA/'#32593#32476#36716#21457
      TabOrder = 1
    end
  end
  object groupNetwork: TGroupBox
    Left = 12
    Top = 233
    Width = 341
    Height = 114
    Caption = '3G/GPRS/CDMA/'#32593#32476#36716#21457
    TabOrder = 1
    object lbIpAddr: TLabel
      Left = 21
      Top = 25
      Width = 106
      Height = 13
      Caption = #26381#21153#22120'IP'#22320#22336#25110#22495#21517
    end
    object lbTimeout_Network: TLabel
      Left = 21
      Top = 69
      Width = 68
      Height = 13
      Caption = #36229#26102#26102#38388'('#31186')'
    end
    object lbUserName: TLabel
      Left = 163
      Top = 74
      Width = 36
      Height = 13
      Caption = #29992#25143#21517
    end
    object lbPassword: TLabel
      Left = 245
      Top = 74
      Width = 24
      Height = 13
      Caption = #23494#30721
    end
    object lbRetry_Network: TLabel
      Left = 245
      Top = 25
      Width = 48
      Height = 13
      Caption = #37325#35797#27425#25968
    end
    object lbPort: TLabel
      Left = 163
      Top = 25
      Width = 36
      Height = 13
      Caption = #31471#21475#21495
    end
    object editServer: TEdit
      Left = 21
      Top = 41
      Width = 133
      Height = 21
      TabOrder = 0
      Text = '219.234.88.231'
    end
    object editTimeout_Network: TEdit
      Left = 21
      Top = 85
      Width = 68
      Height = 21
      TabOrder = 1
      Text = '8'
    end
    object editUserName: TEdit
      Left = 163
      Top = 90
      Width = 60
      Height = 21
      TabOrder = 2
      Text = 'test'
    end
    object editPassword: TEdit
      Left = 245
      Top = 90
      Width = 82
      Height = 21
      PasswordChar = '*'
      TabOrder = 3
      Text = '123456'
    end
    object editSocketPort_Server: TEdit
      Left = 163
      Top = 41
      Width = 60
      Height = 21
      TabOrder = 4
      Text = '3700'
    end
    object editRetryCount_Network: TEdit
      Left = 245
      Top = 41
      Width = 52
      Height = 21
      TabOrder = 5
      Text = '5'
    end
  end
  object groupSerialPort: TGroupBox
    Left = 12
    Top = 93
    Width = 341
    Height = 134
    Caption = 'RS232/RS485'
    TabOrder = 2
    object lbSerialPort: TLabel
      Left = 21
      Top = 25
      Width = 60
      Height = 13
      Caption = #35745#31639#26426#20018#21475
    end
    object lbBaudRate: TLabel
      Left = 149
      Top = 25
      Width = 48
      Height = 13
      Caption = #36890#20449#36895#29575
    end
    object lbTimeout_SerialPort: TLabel
      Left = 21
      Top = 77
      Width = 80
      Height = 13
      Caption = #36229#26102#26102#38388'('#27627#31186')'
    end
    object lbRetry_SerialPort: TLabel
      Left = 149
      Top = 77
      Width = 48
      Height = 13
      Caption = #37325#35797#27425#25968
    end
    object comboBoxComPort: TComboBox
      Left = 21
      Top = 44
      Width = 97
      Height = 21
      Sorted = True
      TabOrder = 0
    end
    object editTimeout_SerialPort: TEdit
      Left = 21
      Top = 93
      Width = 100
      Height = 21
      TabOrder = 1
      Text = '200'
    end
    object editRetryCount_SerialPort: TEdit
      Left = 149
      Top = 93
      Width = 100
      Height = 21
      TabOrder = 2
      Text = '5'
    end
    object cbBaudrate: TComboBox
      Left = 149
      Top = 44
      Width = 97
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 3
      Text = '115200'
      Items.Strings = (
        '115200'
        '57600'
        '38400'
        '19200'
        '9600'
        '4800'
        '2400')
    end
  end
  object btnCancel: TButton
    Left = 373
    Top = 321
    Width = 108
    Height = 25
    Cancel = True
    Caption = #21462#28040
    ModalResult = 2
    TabOrder = 3
  end
  object btnOk: TButton
    Left = 373
    Top = 290
    Width = 108
    Height = 25
    Caption = #30830#23450
    Default = True
    ModalResult = 1
    TabOrder = 4
    OnClick = btnOkClick
  end
end
