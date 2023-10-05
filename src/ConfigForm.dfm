object frmConfig: TfrmConfig
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #25511#21046#21345#37197#32622
  ClientHeight = 416
  ClientWidth = 745
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btnWriteConfig: TButton
    Left = 596
    Top = 317
    Width = 137
    Height = 25
    Caption = #20889#20837#25511#21046#21345#37197#32622'(&W)'
    TabOrder = 0
    OnClick = btnWriteConfigClick
  end
  object btnReadConfig: TButton
    Left = 596
    Top = 286
    Width = 137
    Height = 25
    Caption = #35835#21462#25511#21046#21345#37197#32622'(&R)'
    TabOrder = 1
    OnClick = btnReadConfigClick
  end
  object btnOk: TButton
    Left = 596
    Top = 348
    Width = 137
    Height = 25
    Caption = #30830#23450'(&O)'
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOkClick
  end
  object btnCancel: TButton
    Left = 596
    Top = 379
    Width = 137
    Height = 25
    Cancel = True
    Caption = #21462#28040'(&C)'
    ModalResult = 2
    TabOrder = 3
  end
  object PageControl: TPageControl
    Left = 8
    Top = 8
    Width = 576
    Height = 395
    ActivePage = sheetLedPanel
    MultiLine = True
    ParentShowHint = False
    ShowHint = False
    TabOrder = 4
    object sheetLedPanel: TTabSheet
      Caption = #26174#31034#23631'('#25511#21046#21345')'
      Highlighted = True
      object lbPanelWidth: TLabel
        Left = 16
        Top = 22
        Width = 24
        Height = 13
        Caption = #23485#24230
      end
      object lbPanelHeight: TLabel
        Left = 71
        Top = 22
        Width = 24
        Height = 13
        Caption = #39640#24230
      end
      object lbScanMode: TLabel
        Left = 122
        Top = 22
        Width = 48
        Height = 13
        Caption = #25195#25551#26041#24335
      end
      object lbActCount: TLabel
        Left = 16
        Top = 72
        Width = 84
        Height = 13
        Caption = #25511#21046#21345#33410#30446#23481#37327
      end
      object editScreenWidth: TEdit
        Left = 16
        Top = 38
        Width = 49
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 0
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editScreenHeight: TEdit
        Left = 71
        Top = 38
        Width = 45
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 1
        Text = '16'
        OnChange = editSubAreaChange
      end
      object checkOE: TCheckBox
        Left = 16
        Top = 118
        Width = 153
        Height = 17
        Caption = 'OE'#21453#30456
        TabOrder = 2
      end
      object checkData: TCheckBox
        Left = 16
        Top = 141
        Width = 193
        Height = 17
        Caption = #25968#25454#21453#30456
        TabOrder = 3
      end
      object comboBoxScanMode: TComboBox
        Left = 122
        Top = 38
        Width = 89
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 0
        TabOrder = 4
        Text = '1/16(A)'
        Items.Strings = (
          '1/16(A)'
          '1/8(A)'
          '1/4(A)'
          '1/4(B)'
          '1/4(C)'
          '1/16(B)'
          '1/16(C)'
          '1/4(D)'
          '1/1(A)'
          '1/1(B)'
          '8/1(B)'
          '1/4(E)'
          '1/4(F)')
      end
      object checkActSecond: TCheckBox
        Left = 16
        Top = 164
        Width = 193
        Height = 17
        Caption = #25773#25918#26102#27573#31934#30830#21040#31186
        TabOrder = 5
      end
      object comboBoxMaxActCount: TComboBox
        Left = 16
        Top = 90
        Width = 71
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 2
        TabOrder = 6
        Text = '127'
        Items.Strings = (
          '511'
          '255'
          '127'
          '63'
          '31')
      end
      object checkActSupportRepeat: TCheckBox
        Left = 16
        Top = 187
        Width = 233
        Height = 17
        Caption = #25511#21046#21345#25903#25345#21333#26465#33410#30446#37325#22797#25773#25918
        Checked = True
        State = cbChecked
        TabOrder = 7
      end
      object checkSupportInsertPlay: TCheckBox
        Left = 16
        Top = 210
        Width = 273
        Height = 17
        Caption = #23558#26368#21518'6'#26465#33410#30446#20570#20026#25554#25773#20449#24687#20351#29992
        Checked = True
        State = cbChecked
        TabOrder = 8
      end
      object rbtnSingleColor: TRadioButton
        Left = 328
        Top = 118
        Width = 145
        Height = 17
        Caption = #21333#33394
        TabOrder = 9
      end
      object rbtnDoubleColor: TRadioButton
        Left = 328
        Top = 141
        Width = 145
        Height = 17
        Caption = #21452#33394
        TabOrder = 10
      end
      object checkSupportWarnning: TCheckBox
        Left = 16
        Top = 233
        Width = 193
        Height = 17
        Caption = #20801#35768#24320#20851#37327#25253#35686
        TabOrder = 11
      end
      object checkSyncPlay: TCheckBox
        Left = 16
        Top = 256
        Width = 257
        Height = 17
        Caption = #22810#23631#21516#27493#25773#25918
        TabOrder = 12
      end
      object checkByteReverse: TCheckBox
        Left = 16
        Top = 279
        Width = 193
        Height = 17
        Caption = #25968#25454#23383#33410#36870#24207
        TabOrder = 13
      end
      object checkSyncTimeHint: TCheckBox
        Left = 16
        Top = 302
        Width = 193
        Height = 17
        Caption = #25903#25345#24490#29615#20896#21517#25253#26102#26174#31034
        TabOrder = 14
        Visible = False
      end
      object checkSupportLedSignCheck: TCheckBox
        Left = 16
        Top = 325
        Width = 193
        Height = 17
        Caption = #25903#25345#26174#31034#23631#25925#38556#26816#27979
        TabOrder = 15
        Visible = False
      end
      object rbtnFullColor: TRadioButton
        Left = 328
        Top = 164
        Width = 145
        Height = 17
        Caption = #20840#24425
        TabOrder = 16
      end
    end
    object sheetSubArea: TTabSheet
      Caption = #20998#21306#31649#29702
      ImageIndex = 1
      object lbAreaCount: TLabel
        Left = 32
        Top = 24
        Width = 72
        Height = 13
        Alignment = taRightJustify
        Caption = #26377#25928#20998#21306#20010#25968
      end
      object lbArea0: TLabel
        Left = 19
        Top = 66
        Width = 10
        Height = 13
        Caption = '1:'
      end
      object Label8: TLabel
        Left = 39
        Top = 47
        Width = 6
        Height = 13
        Caption = 'x'
      end
      object Label10: TLabel
        Left = 82
        Top = 47
        Width = 6
        Height = 13
        Caption = 'y'
      end
      object lbAreaWidth: TLabel
        Left = 133
        Top = 47
        Width = 24
        Height = 13
        Caption = #23485#24230
      end
      object lbAreaHeight: TLabel
        Left = 186
        Top = 47
        Width = 24
        Height = 13
        Caption = #39640#24230
      end
      object lbArea1: TLabel
        Left = 19
        Top = 93
        Width = 10
        Height = 13
        Caption = '2:'
      end
      object lbArea2: TLabel
        Left = 19
        Top = 120
        Width = 10
        Height = 13
        Caption = '3:'
      end
      object lbArea3: TLabel
        Left = 19
        Top = 144
        Width = 10
        Height = 13
        Caption = '4:'
      end
      object lbArea4: TLabel
        Left = 19
        Top = 168
        Width = 10
        Height = 13
        Caption = '5:'
      end
      object lbArea7: TLabel
        Left = 19
        Top = 240
        Width = 10
        Height = 13
        Caption = '8:'
      end
      object lbArea5: TLabel
        Left = 19
        Top = 192
        Width = 10
        Height = 13
        Caption = '6:'
      end
      object lbArea6: TLabel
        Left = 19
        Top = 216
        Width = 10
        Height = 13
        Caption = '7:'
      end
      object lbVoiceArea: TLabel
        Left = 19
        Top = 281
        Width = 186
        Height = 13
        Caption = #35821#38899#36755#20986#20998#21306#65288'0'#34920#31034#19981#36755#20986#35821#38899#65289
      end
      object comboBoxAreaCount: TComboBox
        Left = 110
        Top = 20
        Width = 73
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 0
        TabOrder = 0
        Text = '1'
        OnChange = comboBoxAreaCountChange
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
      object editAreaLeft0: TEdit
        Left = 39
        Top = 63
        Width = 37
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 1
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop0: TEdit
        Left = 82
        Top = 63
        Width = 45
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 2
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth0: TEdit
        Left = 131
        Top = 63
        Width = 49
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 3
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight0: TEdit
        Left = 186
        Top = 63
        Width = 45
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 4
        Text = '16'
        OnChange = editSubAreaChange
      end
      object editAreaLeft1: TEdit
        Left = 39
        Top = 90
        Width = 37
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 5
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop1: TEdit
        Left = 82
        Top = 90
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 6
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth1: TEdit
        Left = 131
        Top = 90
        Width = 49
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 7
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight1: TEdit
        Left = 186
        Top = 90
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 8
        Text = '16'
        OnChange = editSubAreaChange
      end
      object editAreaLeft2: TEdit
        Left = 39
        Top = 117
        Width = 37
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 9
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop2: TEdit
        Left = 82
        Top = 117
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 10
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth2: TEdit
        Left = 131
        Top = 117
        Width = 49
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 11
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight2: TEdit
        Left = 186
        Top = 117
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 12
        Text = '16'
        OnChange = editSubAreaChange
      end
      object editAreaLeft3: TEdit
        Left = 39
        Top = 141
        Width = 37
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 13
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop3: TEdit
        Left = 82
        Top = 141
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 14
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth3: TEdit
        Left = 131
        Top = 141
        Width = 49
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 15
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight3: TEdit
        Left = 186
        Top = 141
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 16
        Text = '16'
        OnChange = editSubAreaChange
      end
      object editAreaLeft4: TEdit
        Left = 39
        Top = 165
        Width = 37
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 17
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop4: TEdit
        Left = 82
        Top = 165
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 18
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth4: TEdit
        Left = 131
        Top = 165
        Width = 49
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 19
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight4: TEdit
        Left = 186
        Top = 165
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 20
        Text = '16'
        OnChange = editSubAreaChange
      end
      object editAreaLeft7: TEdit
        Left = 39
        Top = 237
        Width = 37
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 21
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop7: TEdit
        Left = 82
        Top = 237
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 22
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth7: TEdit
        Left = 131
        Top = 237
        Width = 49
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 23
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight7: TEdit
        Left = 186
        Top = 237
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 24
        Text = '16'
        OnChange = editSubAreaChange
      end
      object editAreaLeft5: TEdit
        Left = 39
        Top = 189
        Width = 37
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 25
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop5: TEdit
        Left = 82
        Top = 189
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 26
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth5: TEdit
        Left = 131
        Top = 189
        Width = 49
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 27
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight5: TEdit
        Left = 186
        Top = 189
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 28
        Text = '16'
        OnChange = editSubAreaChange
      end
      object editAreaLeft6: TEdit
        Left = 39
        Top = 213
        Width = 37
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 29
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaTop6: TEdit
        Left = 82
        Top = 213
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 30
        Text = '0'
        OnChange = editSubAreaChange
      end
      object editAreaWidth6: TEdit
        Left = 131
        Top = 213
        Width = 49
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 31
        Text = '128'
        OnChange = editSubAreaChange
      end
      object editAreaHeight6: TEdit
        Left = 186
        Top = 213
        Width = 45
        Height = 21
        Enabled = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 32
        Text = '16'
        OnChange = editSubAreaChange
      end
      object scrAreas: TScrollBox
        Left = 256
        Top = 28
        Width = 297
        Height = 230
        BevelKind = bkFlat
        Color = clBtnFace
        ParentColor = False
        TabOrder = 33
        object imageAreas: TImage
          Left = -4
          Top = 40
          Width = 231
          Height = 112
          Picture.Data = {
            07544269746D617076010000424D760100000000000076000000280000002000
            0000100000000100040000000000000100000000000000000000100000000000
            0000000000000000800000800000008080008000000080008000808000008080
            8000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
            FF00777777777777777777777777777777777000000000000077888888888888
            8887700000000000007788888888888888877000000000000077888888888888
            8887700009900990007788888778877888877000099009900077888887788778
            8887700009900990007788888778877888877000099999900077888887777778
            8887700009900990007788888778877888877000099009900077888887788778
            8887700009900990007788888778877888877000009999000077888888777788
            8887700000099000007788888887788888877000000000000077888888888888
            8887700000000000007788888888888888877777777777777777777777777777
            7777}
        end
      end
      object editVoiceArea: TEdit
        Left = 19
        Top = 297
        Width = 49
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 34
        Text = '128'
        OnChange = editSubAreaChange
      end
    end
    object sheetAutoLightControl: TTabSheet
      Caption = #33258#21160#20142#24230#35843#33410
      ImageIndex = 3
      object lbLight0: TLabel
        Left = 16
        Top = 42
        Width = 10
        Height = 13
        Caption = '1:'
      end
      object lbBrightnessStartTime: TLabel
        Left = 32
        Top = 19
        Width = 48
        Height = 13
        Caption = #24320#22987#26102#38388
      end
      object lbBrightnessValue: TLabel
        Left = 100
        Top = 19
        Width = 36
        Height = 13
        Caption = #20142#24230#20540
      end
      object lbLight1: TLabel
        Left = 16
        Top = 66
        Width = 10
        Height = 13
        Caption = '2:'
      end
      object lbLight2: TLabel
        Left = 16
        Top = 91
        Width = 10
        Height = 13
        Caption = '3:'
      end
      object lbLight3: TLabel
        Left = 16
        Top = 116
        Width = 10
        Height = 13
        Caption = '4:'
      end
      object lbLight4: TLabel
        Left = 16
        Top = 141
        Width = 10
        Height = 13
        Caption = '5:'
      end
      object lbLight7: TLabel
        Left = 16
        Top = 216
        Width = 10
        Height = 13
        Caption = '8:'
      end
      object lbLight5: TLabel
        Left = 16
        Top = 166
        Width = 10
        Height = 13
        Caption = '6:'
      end
      object lbLight6: TLabel
        Left = 16
        Top = 191
        Width = 10
        Height = 13
        Caption = '7:'
      end
      object timeStart0: TDateTimePicker
        Left = 32
        Top = 38
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 0
      end
      object timeStart1: TDateTimePicker
        Left = 32
        Top = 62
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 1
      end
      object timeStart2: TDateTimePicker
        Left = 32
        Top = 87
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 2
      end
      object timeStart3: TDateTimePicker
        Left = 32
        Top = 112
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 3
      end
      object timeStart4: TDateTimePicker
        Left = 32
        Top = 137
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 4
      end
      object timeStart5: TDateTimePicker
        Left = 32
        Top = 162
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 5
      end
      object timeStart6: TDateTimePicker
        Left = 32
        Top = 187
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 6
      end
      object timeStart7: TDateTimePicker
        Left = 32
        Top = 212
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 7
      end
      object comboBoxLightValue0: TComboBox
        Left = 100
        Top = 38
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 8
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
      object comboBoxLightValue1: TComboBox
        Left = 100
        Top = 62
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 9
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
      object comboBoxLightValue2: TComboBox
        Left = 100
        Top = 87
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 10
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
      object comboBoxLightValue3: TComboBox
        Left = 100
        Top = 112
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 11
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
      object comboBoxLightValue4: TComboBox
        Left = 100
        Top = 137
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 12
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
      object comboBoxLightValue5: TComboBox
        Left = 100
        Top = 162
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 13
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
      object comboBoxLightValue6: TComboBox
        Left = 100
        Top = 187
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 14
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
      object comboBoxLightValue7: TComboBox
        Left = 100
        Top = 212
        Width = 93
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 7
        TabOrder = 15
        Text = '7('#26368#20142')'
        Items.Strings = (
          '0 ('#20851#23631')'
          '1('#26368#26263')'
          '2('#36739#26263')'
          '3('#31245#26263')'
          '4('#20013#24230')'
          '5('#31245#20142')'
          '6('#36739#20142')'
          '7('#26368#20142')')
      end
    end
    object sheetGPRS: TTabSheet
      Caption = 'GPRS/CDMA/3G'#36890#20449
      ImageIndex = 4
      object lbServer: TLabel
        Left = 16
        Top = 66
        Width = 106
        Height = 13
        Caption = #26381#21153#22120'IP'#22320#22336#25110#22495#21517
      end
      object lbSocket: TLabel
        Left = 16
        Top = 111
        Width = 36
        Height = 13
        Caption = #31471#21475#21495
      end
      object lbCode: TLabel
        Left = 16
        Top = 22
        Width = 48
        Height = 13
        Caption = #35774#22791#32534#30721
      end
      object labelAPN: TLabel
        Left = 111
        Top = 22
        Width = 49
        Height = 13
        Caption = 'GPRS APN'
      end
      object editDeviceCode: TEdit
        Left = 16
        Top = 38
        Width = 89
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 11
        TabOrder = 0
        Text = '12345678904'
      end
      object editDataCenter: TEdit
        Left = 16
        Top = 82
        Width = 185
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 31
        TabOrder = 1
        Text = '219.234.88.231'
      end
      object editSocketPort_Card: TEdit
        Left = 16
        Top = 127
        Width = 84
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 2
        Text = '3700'
      end
      object editAPN: TEdit
        Left = 111
        Top = 38
        Width = 90
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 31
        TabOrder = 3
        Text = 'cmnet'
      end
      object checkTCP: TCheckBox
        Left = 16
        Top = 165
        Width = 193
        Height = 17
        Caption = #20351#29992'TCP'#21327#35758
        TabOrder = 4
        Visible = False
      end
      object checkLoginEcho: TCheckBox
        Left = 16
        Top = 188
        Width = 193
        Height = 17
        Caption = #19978#32447#26102#26816#26597#26381#21153#22120#24212#31572
        TabOrder = 5
        Visible = False
      end
    end
    object sheetComPort: TTabSheet
      Caption = #20018#21475#36890#20449
      ImageIndex = 4
      object lbCurrentCardId: TLabel
        Left = 16
        Top = 22
        Width = 48
        Height = 13
        Caption = #24403#21069#21345#21495
      end
      object lbBaudRate: TLabel
        Left = 16
        Top = 121
        Width = 48
        Height = 13
        Caption = #36890#20449#36895#29575
      end
      object lbNewCardId: TLabel
        Left = 16
        Top = 70
        Width = 36
        Height = 13
        Caption = #26032#21345#21495
      end
      object editCardId: TEdit
        Left = 16
        Top = 38
        Width = 49
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 0
        Text = '255'
        OnChange = editSubAreaChange
      end
      object cbBaudrate: TComboBox
        Left = 16
        Top = 140
        Width = 97
        Height = 21
        Style = csDropDownList
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        ItemIndex = 0
        TabOrder = 1
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
      object btnSearchCard: TButton
        Left = 87
        Top = 36
        Width = 82
        Height = 25
        Caption = #26597#25214'(&S)'
        TabOrder = 2
        OnClick = btnSearchCardClick
      end
      object editNewCardId: TEdit
        Left = 16
        Top = 86
        Width = 49
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 3
        Text = '255'
        OnChange = editSubAreaChange
      end
    end
    object sheetSms: TTabSheet
      Caption = #30701#20449#37197#32622
      ImageIndex = 5
      object Label3: TLabel
        Left = 24
        Top = 16
        Width = 72
        Height = 13
        Caption = #25480#26435#30701#20449#29992#25143
      end
      object editSmsUser0: TEdit
        Left = 24
        Top = 32
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 0
      end
      object editSmsUser1: TEdit
        Left = 24
        Top = 55
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 1
      end
      object editSmsUser2: TEdit
        Left = 24
        Top = 78
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 2
      end
      object editSmsUser3: TEdit
        Left = 24
        Top = 101
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 3
      end
      object editSmsUser4: TEdit
        Left = 24
        Top = 124
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 4
      end
      object editSmsUser5: TEdit
        Left = 24
        Top = 147
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 5
      end
      object editSmsUser6: TEdit
        Left = 24
        Top = 170
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 6
      end
      object editSmsUser7: TEdit
        Left = 24
        Top = 193
        Width = 169
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 23
        TabOrder = 7
      end
      object checkEchoSms0: TCheckBox
        Left = 199
        Top = 34
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 8
      end
      object checkEchoSms1: TCheckBox
        Left = 199
        Top = 57
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 9
      end
      object checkEchoSms2: TCheckBox
        Left = 199
        Top = 78
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 10
      end
      object checkEchoSms3: TCheckBox
        Left = 199
        Top = 101
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 11
      end
      object checkEchoSms4: TCheckBox
        Left = 199
        Top = 124
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 12
      end
      object checkEchoSms5: TCheckBox
        Left = 199
        Top = 147
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 13
      end
      object checkEchoSms6: TCheckBox
        Left = 199
        Top = 170
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 14
      end
      object checkEchoSms7: TCheckBox
        Left = 199
        Top = 193
        Width = 106
        Height = 17
        Caption = #22238#22797#35813#29992#25143#30701#20449
        TabOrder = 15
      end
      object GroupBox1: TGroupBox
        Left = 24
        Top = 220
        Width = 489
        Height = 125
        Caption = #30701#20449#21629#20196#40664#35748#26174#31034#26041#24335#23450#20041
        TabOrder = 16
        object lbEnterStyle: TLabel
          Left = 25
          Top = 35
          Width = 48
          Height = 13
          Caption = #36827#20837#26041#24335
        end
        object lbExitStyle: TLabel
          Left = 25
          Top = 61
          Width = 48
          Height = 13
          Caption = #36864#20986#26041#24335
        end
        object lbEnterSpeed: TLabel
          Left = 297
          Top = 38
          Width = 48
          Height = 13
          Caption = #36827#20837#36895#24230
        end
        object lbStayTime: TLabel
          Left = 25
          Top = 93
          Width = 48
          Height = 13
          Caption = #20572#30041#26102#38388
        end
        object lbSpeed1: TLabel
          Left = 415
          Top = 38
          Width = 52
          Height = 13
          Caption = #27627#31186'/'#20687#32032
        end
        object lbStayTimeHint: TLabel
          Left = 143
          Top = 93
          Width = 24
          Height = 13
          Caption = #27627#31186
        end
        object lbRepeatCount: TLabel
          Left = 185
          Top = 93
          Width = 72
          Height = 13
          Caption = #37325#22797#25773#25918#27425#25968
        end
        object lbExitSpeed: TLabel
          Left = 297
          Top = 61
          Width = 48
          Height = 13
          Caption = #36864#20986#36895#24230
        end
        object lbSpeed2: TLabel
          Left = 415
          Top = 61
          Width = 52
          Height = 13
          Caption = #27627#31186'/'#20687#32032
        end
        object Label4: TLabel
          Left = 343
          Top = 93
          Width = 24
          Height = 13
          Caption = #23383#20307
        end
        object comboEnterStyle: TComboBox
          Left = 79
          Top = 31
          Width = 202
          Height = 21
          Style = csDropDownList
          DropDownCount = 16
          ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
          ItemIndex = 0
          TabOrder = 0
          Text = #30452#25509#26174#31034
          Items.Strings = (
            #30452#25509#26174#31034
            #36830#32493#24038#31227
            #36830#32493#21491#31227
            #36830#32493#19978#31227
            #36830#32493#19979#31227
            #21521#24038#31227#20837
            #21521#21491#31227#20837
            #21521#19978#31227#20837
            #21521#19979#31227#20837
            #21521#21491#23637#24320
            #21521#24038#23637#24320
            #21521#19979#23637#24320
            #21521#19978#23637#24320
            #20174#24038#21491#21521#20013#38388#23637#24320
            #20174#20013#38388#21521#24038#21491#23637#24320
            #20174#19978#19979#21521#20013#38388#23637#24320
            #20174#20013#38388#21521#19978#19979#23637#24320
            #27700#24179#30334#21494#31383
            #22402#30452#30334#21494#31383)
        end
        object comboExitStyle: TComboBox
          Left = 79
          Top = 57
          Width = 202
          Height = 21
          Style = csDropDownList
          DropDownCount = 17
          ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
          ItemIndex = 0
          TabOrder = 1
          Text = #30452#25509#28165#38500
          Items.Strings = (
            #30452#25509#28165#38500
            #21521#24038#31227#20986
            #21521#21491#31227#20986
            #21521#19978#31227#20986
            #21521#19979#31227#20986
            #21521#21491#25830#38500
            #21521#24038#25830#38500
            #21521#19979#25830#38500
            #21521#19978#25830#38500
            #20174#24038#21491#21521#20013#38388#25830#38500
            #20174#20013#38388#21521#24038#21491#25830#38500
            #20174#19978#19979#21521#20013#38388#25830#38500
            #20174#20013#38388#21521#19978#19979#25830#38500
            #27700#24179#30334#21494#31383
            #22402#30452#30334#21494#31383
            #20854#23427
            #26080)
        end
        object editEnterSpeed: TEdit
          Left = 351
          Top = 34
          Width = 58
          Height = 21
          HideSelection = False
          ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
          TabOrder = 2
          Text = '16'
        end
        object editShowTime: TEdit
          Left = 79
          Top = 89
          Width = 58
          Height = 21
          ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
          TabOrder = 3
          Text = '3000'
        end
        object editRepeatCount: TEdit
          Left = 263
          Top = 89
          Width = 58
          Height = 21
          ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
          TabOrder = 4
          Text = '1'
        end
        object editExitSpeed: TEdit
          Left = 351
          Top = 57
          Width = 58
          Height = 21
          ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
          TabOrder = 5
          Text = '16'
        end
        object comboFont: TComboBox
          Left = 373
          Top = 89
          Width = 57
          Height = 21
          Style = csDropDownList
          ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
          ItemIndex = 0
          TabOrder = 6
          Text = '16'
          Items.Strings = (
            '16'
            '32'
            '48'
            '64')
        end
      end
    end
    object sheetWifi: TTabSheet
      Caption = 'WIFI'#26080#32447#32593#32476
      ImageIndex = 6
      object Label5: TLabel
        Left = 16
        Top = 22
        Width = 48
        Height = 13
        Caption = #35774#22791#32534#30721
      end
      object Label6: TLabel
        Left = 16
        Top = 66
        Width = 70
        Height = 13
        Caption = #26381#21153#22120'IP'#22320#22336
      end
      object Label7: TLabel
        Left = 16
        Top = 111
        Width = 36
        Height = 13
        Caption = #31471#21475#21495
      end
      object Label9: TLabel
        Left = 279
        Top = 22
        Width = 77
        Height = 13
        Caption = 'WIFI '#32593#32476' SSID'
      end
      object Label11: TLabel
        Left = 279
        Top = 66
        Width = 99
        Height = 13
        Caption = 'WIFI '#32593#32476#23433#20840#23494#38053
      end
      object Label12: TLabel
        Left = 279
        Top = 111
        Width = 97
        Height = 13
        Caption = 'WIFI '#36335#30001#22120'IP'#22320#22336
      end
      object editWifiDeviceCode: TEdit
        Left = 16
        Top = 38
        Width = 89
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 11
        TabOrder = 0
        Text = '12345678902'
      end
      object editWifiServer: TEdit
        Left = 16
        Top = 82
        Width = 185
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 31
        TabOrder = 1
        Text = '192.168.1.4'
      end
      object editWifiSocketPort_Card: TEdit
        Left = 16
        Top = 127
        Width = 84
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 2
        Text = '3700'
      end
      object editSSID: TEdit
        Left = 279
        Top = 38
        Width = 154
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 31
        TabOrder = 3
        Text = 'TP-LINK_OFFICE'
      end
      object editWifiPassword: TEdit
        Left = 279
        Top = 82
        Width = 154
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 31
        PasswordChar = '*'
        TabOrder = 4
        Text = '123456'
      end
      object editWifiDnsServer: TEdit
        Left = 279
        Top = 127
        Width = 154
        Height = 21
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        MaxLength = 31
        TabOrder = 5
        Text = '192.168.1.1'
      end
    end
    object sheetGps: TTabSheet
      Caption = 'GPS'
      ImageIndex = 7
      object Label1: TLabel
        Left = 16
        Top = 26
        Width = 149
        Height = 13
        Caption = 'GPS'#23450#20301#26381#21153#22120'IP'#22320#22336#25110#22495#21517
      end
      object Label2: TLabel
        Left = 16
        Top = 71
        Width = 36
        Height = 13
        Caption = #31471#21475#21495
      end
      object Label13: TLabel
        Left = 16
        Top = 124
        Width = 82
        Height = 13
        Caption = #25511#21046#21345#20266'IP'#22320#22336
      end
      object Label14: TLabel
        Left = 16
        Top = 172
        Width = 116
        Height = 13
        Caption = #23450#20301#20449#24687#25253#21578#21608#26399'('#31186')'
      end
      object editGpsServer: TEdit
        Left = 16
        Top = 42
        Width = 185
        Height = 21
        MaxLength = 31
        TabOrder = 0
        Text = '219.234.88.231'
      end
      object editGpsSocketPort: TEdit
        Left = 16
        Top = 87
        Width = 84
        Height = 21
        TabOrder = 1
        Text = '3700'
      end
      object editGpsPseudoIp: TEdit
        Left = 16
        Top = 140
        Width = 116
        Height = 21
        MaxLength = 31
        TabOrder = 2
        Text = '219.234.88.231'
      end
      object editGpsReportInterval: TEdit
        Left = 16
        Top = 191
        Width = 116
        Height = 21
        MaxLength = 31
        TabOrder = 3
        Text = '30'
      end
    end
    object sheetWebData: TTabSheet
      Caption = #27668#35937#25968#25454
      ImageIndex = 8
      object Label15: TLabel
        Left = 32
        Top = 24
        Width = 164
        Height = 13
        Caption = #33258#21160#27668#35937#31449#32534#21495#21015#34920'('#31354#26684#20998#38548')'
      end
      object editWebDataId: TEdit
        Left = 32
        Top = 43
        Width = 353
        Height = 21
        TabOrder = 0
      end
    end
    object sheetEthernet: TTabSheet
      Caption = #20197#22826#32593#32476
      ImageIndex = 9
      object Label16: TLabel
        Left = 39
        Top = 46
        Width = 94
        Height = 13
        Caption = #25511#21046#21345#24403#21069'IP'#22320#22336
      end
      object Label17: TLabel
        Left = 39
        Top = 215
        Width = 48
        Height = 13
        Caption = #23376#32593#25513#30721
      end
      object Label18: TLabel
        Left = 39
        Top = 156
        Width = 34
        Height = 13
        Caption = #32593#20851'IP'
      end
      object Label19: TLabel
        Left = 39
        Top = 102
        Width = 82
        Height = 13
        Caption = #25511#21046#21345#26032'IP'#22320#22336
      end
      object editLocalIp: TEdit
        Left = 39
        Top = 62
        Width = 97
        Height = 21
        MaxLength = 31
        TabOrder = 0
        Text = '192.168.1.111'
      end
      object editNetMask: TEdit
        Left = 39
        Top = 231
        Width = 97
        Height = 21
        MaxLength = 31
        TabOrder = 1
        Text = '255.255.255.0'
      end
      object editGateway: TEdit
        Left = 39
        Top = 172
        Width = 97
        Height = 21
        MaxLength = 31
        TabOrder = 2
        Text = '192.168.1.1'
      end
      object editNewLocalIp: TEdit
        Left = 39
        Top = 118
        Width = 97
        Height = 21
        MaxLength = 31
        TabOrder = 3
        Text = '192.168.1.111'
      end
      object checkBroadcast: TCheckBox
        Left = 168
        Top = 64
        Width = 113
        Height = 17
        Caption = #20351#29992#24191#25773#22320#22336
        Checked = True
        State = cbChecked
        TabOrder = 4
      end
    end
  end
  object timerWriteConfig: TTimer
    Enabled = False
    Interval = 10
    OnTimer = timerWriteConfigTimer
    Left = 608
    Top = 64
  end
  object timerReadConfig: TTimer
    Enabled = False
    Interval = 10
    OnTimer = timerReadConfigTimer
    Left = 608
    Top = 8
  end
  object udpSocket: TIdUDPClient
    BroadcastEnabled = True
    Host = '127.0.0.1'
    Port = 3700
    Left = 608
    Top = 120
  end
  object timerSearchCard: TTimer
    Enabled = False
    Interval = 10
    OnTimer = timerSearchCardTimer
    Left = 616
    Top = 176
  end
end
