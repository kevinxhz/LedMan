object frmMain: TfrmMain
  Left = 244
  Top = 110
  Caption = 'YledMan '#26174#31034#23631#31649#29702
  ClientHeight = 607
  ClientWidth = 1148
  Color = clBtnFace
  Font.Charset = ANSI_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #23435#20307
  Font.Style = []
  Icon.Data = {
    0000010002002020100000000000E80200002600000010101000000000002801
    00000E0300002800000020000000400000000100040000000000800200000000
    0000000000000000000000000000000000000000800000800000008080008000
    00008000800080800000C0C0C000808080000000FF0000FF000000FFFF00FF00
    0000FF00FF00FFFF0000FFFFFF00000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000044444444444400000000000000
    00044044444444444444400000000000044BBBBBBBBBBB444444440000000000
    4444448BBBB04444444444440000000444444448BBB544444444444440000044
    44444444BBBB04444444444404000044444444444BBB84444444444444000444
    4444440443BB7044444444444440044444458BBBBBBBBBBBBBBBBB4444400444
    448BBB04448BBB34444444444440044448BBB8444448BBB44444444444400444
    44BBB8044440BBB34444444444400444448BBB4444443BBB6444444444400044
    444BBB8444BBBBBBBBBBBBBB440000044C43BB84444444444444444440000004
    44BBB84444444444444444440000000004444444444444444444444000000000
    0044444444444444444444000000000000004444444444444444000000000000
    0000000444444444440000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    0000000000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    FFFFFFFFFFFFFFC003FFFE00007FF800003FF000000FE0000007C0000003C000
    0003800000018000000180000001800000018000000180000001C0000003E000
    0007E000000FF800001FFC00003FFF0000FFFFE003FFFFFFFFFFFFFFFFFFFFFF
    FFFFFFFFFFFFFFFFFFFFFFFFFFFF280000001000000020000000010004000000
    0000C00000000000000000000000000000000000000000000000000080000080
    000000808000800000008000800080800000C0C0C000808080000000FF0000FF
    000000FFFF00FF000000FF00FF00FFFF0000FFFFFF0066666666666666666666
    6666666666666666666666666666666644444446666666453BBB344444666444
    44BB444444466444444B444444466448B33BB3333446644B8444B34444466448
    B443BB3333466443B44444444466666444444444466666664444444466666666
    6666666666666666666666666666666666666666666600000000000000000000
    0000000000000000000000000000000000000000000000000000000000000000
    00000000000000000000000000000000000000000000}
  Menu = mnuMain
  OldCreateOrder = False
  Position = poScreenCenter
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 12
  object Splitter3: TSplitter
    Left = 449
    Top = 23
    Height = 584
    ExplicitLeft = 25
    ExplicitTop = -16
    ExplicitHeight = 560
  end
  object panelDtuList: TPanel
    Left = 0
    Top = 23
    Width = 449
    Height = 584
    Align = alLeft
    BevelInner = bvSpace
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 1
      Top = 288
      Width = 447
      Height = 5
      Cursor = crVSplit
      Align = alBottom
      Beveled = True
      ExplicitTop = 116
    end
    object listActs: TListView
      Left = 1
      Top = 293
      Width = 447
      Height = 290
      Align = alBottom
      Checkboxes = True
      Columns = <
        item
          Caption = #33410#30446#32534#21495
          Width = 64
        end
        item
          Caption = #20998#21306
          Width = 40
        end
        item
          Caption = #33410#30446#20869#23481#35828#26126
          Width = 310
        end>
      FlatScrollBars = True
      GridLines = True
      HideSelection = False
      ReadOnly = True
      RowSelect = True
      ShowWorkAreas = True
      TabOrder = 0
      ViewStyle = vsReport
      OnChange = listActsChange
      OnSelectItem = listActsSelectItem
    end
    object vt: TVirtualStringTree
      Left = 1
      Top = 1
      Width = 447
      Height = 287
      Align = alClient
      Header.AutoSizeIndex = -1
      Header.DefaultHeight = 17
      Header.Font.Charset = DEFAULT_CHARSET
      Header.Font.Color = clWindowText
      Header.Font.Height = -11
      Header.Font.Name = 'Tahoma'
      Header.Font.Style = []
      Header.Options = [hoColumnResize, hoDrag, hoShowSortGlyphs, hoVisible]
      Images = TreeImages
      TabOrder = 1
      TreeOptions.AutoOptions = [toAutoDropExpand, toAutoExpand, toAutoScroll, toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes]
      TreeOptions.MiscOptions = [toAcceptOLEDrop, toCheckSupport, toFullRepaintOnResize, toInitOnSave, toToggleOnDblClick, toWheelPanning, toEditOnClick]
      TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect, toMiddleClickSelect]
      TreeOptions.StringOptions = [toSaveCaptions, toShowStaticText, toAutoAcceptEditChange]
      OnFocusChanged = vtFocusChanged
      OnGetText = vtGetText
      OnPaintText = vtPaintText
      OnGetImageIndex = vtGetImageIndex
      OnGetNodeDataSize = vtGetNodeDataSize
      OnInitNode = vtInitNode
      Columns = <
        item
          Position = 1
          Width = 300
          WideText = #21517#31216'/'#25551#36848
        end
        item
          Color = clBtnShadow
          Options = [coAllowClick, coDraggable, coEnabled, coParentBidiMode, coResizable, coShowDropMark, coVisible, coAllowFocus]
          Position = 0
          Width = 22
        end
        item
          Position = 2
          Width = 100
          WideText = #32534#30721
        end>
    end
  end
  object panelEdit: TPanel
    Left = 452
    Top = 23
    Width = 696
    Height = 584
    Align = alClient
    TabOrder = 1
    object panelMisc: TPanel
      Left = 1
      Top = 208
      Width = 694
      Height = 375
      Align = alBottom
      TabOrder = 0
      object lbEnterStyle: TLabel
        Left = 25
        Top = 35
        Width = 48
        Height = 12
        Caption = #36827#20837#26041#24335
      end
      object lbEnterSpeed: TLabel
        Left = 25
        Top = 86
        Width = 48
        Height = 12
        Caption = #36827#20837#36895#24230
      end
      object lbStayTime: TLabel
        Left = 25
        Top = 112
        Width = 48
        Height = 12
        Caption = #20572#30041#26102#38388
      end
      object lbSpeed1: TLabel
        Left = 143
        Top = 86
        Width = 54
        Height = 12
        Caption = #27627#31186'/'#20687#32032
      end
      object lbExitStyle: TLabel
        Left = 25
        Top = 61
        Width = 48
        Height = 12
        Caption = #36864#20986#26041#24335
      end
      object lbExitSpeed: TLabel
        Left = 233
        Top = 86
        Width = 48
        Height = 12
        Caption = #36864#20986#36895#24230
      end
      object lbSpeed2: TLabel
        Left = 351
        Top = 86
        Width = 54
        Height = 12
        Caption = #27627#31186'/'#20687#32032
      end
      object lbStayTimeHint: TLabel
        Left = 143
        Top = 112
        Width = 24
        Height = 12
        Caption = #27627#31186
      end
      object lbAreaOfAct: TLabel
        Left = 233
        Top = 10
        Width = 84
        Height = 12
        Caption = #35813#33410#30446#25152#23646#20998#21306
      end
      object lbEditingAct: TLabel
        Left = 25
        Top = 10
        Width = 96
        Height = 12
        Caption = #36873#25321#35201#32534#36753#30340#33410#30446
      end
      object lbPictureX: TLabel
        Left = 407
        Top = 212
        Width = 72
        Height = 12
        Caption = #22270#29255#27700#24179#20301#32622
      end
      object lbPictureY: TLabel
        Left = 407
        Top = 240
        Width = 72
        Height = 12
        Caption = #22270#29255#22402#30452#20301#32622
      end
      object lbShutterWidth: TLabel
        Left = 297
        Top = 35
        Width = 84
        Height = 12
        Caption = #30334#21494#31383#21494#29255#23485#24230
      end
      object lbEnd: TLabel
        Left = 282
        Top = 137
        Width = 48
        Height = 12
        Caption = #32456#27490#26102#38388
      end
      object lbStart: TLabel
        Left = 25
        Top = 138
        Width = 48
        Height = 12
        Caption = #36215#22987#26102#38388
      end
      object Label25: TLabel
        Left = 297
        Top = 61
        Width = 48
        Height = 12
        Caption = #38378#28865#21608#26399
        Visible = False
      end
      object lbRepeatCount: TLabel
        Left = 209
        Top = 112
        Width = 72
        Height = 12
        Caption = #37325#22797#25773#25918#27425#25968
      end
      object lbRefHint1: TLabel
        Left = 369
        Top = 111
        Width = 36
        Height = 12
        Caption = #24403#33410#30446
      end
      object lbRefHint2: TLabel
        Left = 489
        Top = 110
        Width = 120
        Height = 12
        Caption = #25773#25918#23436#25104#21518#35813#33410#30446#20572#25773
      end
      object comboEnterStyle: TComboBox
        Left = 79
        Top = 31
        Width = 202
        Height = 20
        Style = csDropDownList
        DropDownCount = 20
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
          #22402#30452#30334#21494#31383
          #38378#28865)
      end
      object editEnterSpeed: TEdit
        Left = 79
        Top = 82
        Width = 58
        Height = 20
        HideSelection = False
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 1
        Text = '16'
      end
      object comboExitStyle: TComboBox
        Left = 79
        Top = 57
        Width = 202
        Height = 20
        Style = csDropDownList
        DropDownCount = 20
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
        ItemIndex = 0
        TabOrder = 2
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
      object editExitSpeed: TEdit
        Left = 287
        Top = 82
        Width = 58
        Height = 20
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 3
        Text = '16'
      end
      object editShowTime: TEdit
        Left = 79
        Top = 108
        Width = 58
        Height = 20
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 4
        Text = '3000'
      end
      object editActDesc: TLabeledEdit
        Left = 19
        Top = 317
        Width = 662
        Height = 48
        EditLabel.Width = 72
        EditLabel.Height = 12
        EditLabel.Caption = #33410#30446#20869#23481#35828#26126
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -35
        Font.Name = 'Arial'
        Font.Style = []
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
        ParentFont = False
        TabOrder = 5
        OnChange = editActDescChange
      end
      object comboArea: TComboBox
        Left = 323
        Top = 6
        Width = 84
        Height = 20
        Style = csDropDownList
        DropDownCount = 10
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
        TabOrder = 6
        OnChange = comboAreaChange
      end
      object comboEditAct: TComboBox
        Left = 127
        Top = 6
        Width = 72
        Height = 20
        Style = csDropDownList
        DropDownCount = 10
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
        TabOrder = 7
        OnChange = comboEditActChange
      end
      object btnLoadPicture: TButton
        Left = 407
        Top = 173
        Width = 138
        Height = 22
        Caption = #36733#20837#22270#29255#25991#20214'...'
        TabOrder = 8
        OnClick = btnLoadPicureClick
      end
      object editPicPosX: TEdit
        Left = 485
        Top = 210
        Width = 58
        Height = 20
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 9
        Text = '0'
        OnChange = editPicPosChange
      end
      object editPicPosY: TEdit
        Left = 485
        Top = 236
        Width = 58
        Height = 20
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 10
        Text = '0'
        OnChange = editPicPosChange
      end
      object editShutterWidth: TEdit
        Left = 387
        Top = 31
        Width = 42
        Height = 20
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 11
        Text = '8'
      end
      object dateActEnd: TDateTimePicker
        Left = 336
        Top = 133
        Width = 121
        Height = 20
        Date = 73050.279390555560000000
        Time = 73050.279390555560000000
        DateFormat = dfLong
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 12
      end
      object groupBoxPeriod: TGroupBox
        Left = 19
        Top = 161
        Width = 238
        Height = 112
        Caption = #27599#26085#25773#25918#26102#27573
        TabOrder = 13
        object scrollboxTime: TScrollBox
          Left = 8
          Top = 16
          Width = 230
          Height = 89
          VertScrollBar.Range = 1024
          AutoScroll = False
          BevelInner = bvNone
          BevelOuter = bvNone
          BorderStyle = bsNone
          TabOrder = 0
        end
      end
      object dateActStart: TDateTimePicker
        Left = 79
        Top = 134
        Width = 121
        Height = 20
        Date = 73050.279390555560000000
        Time = 73050.279390555560000000
        DateFormat = dfLong
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 14
      end
      object editFlashPeriod: TEdit
        Left = 351
        Top = 57
        Width = 42
        Height = 20
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 15
        Text = '500'
        Visible = False
      end
      object timeActStart: TDateTimePicker
        Left = 202
        Top = 134
        Width = 58
        Height = 21
        Date = 40221.000000000000000000
        Format = 'HH:mm'
        Time = 40221.000000000000000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 16
      end
      object timeActEnd: TDateTimePicker
        Left = 463
        Top = 133
        Width = 58
        Height = 21
        Date = 40221.999305555550000000
        Format = 'HH:mm'
        Time = 40221.999305555550000000
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        Kind = dtkTime
        TabOrder = 17
      end
      object editRepeatCount: TEdit
        Left = 287
        Top = 108
        Width = 58
        Height = 20
        ImeName = #20013#25991' - QQ'#25340#38899#36755#20837#27861
        TabOrder = 18
        Text = '1'
      end
      object groupWeekPeriod: TGroupBox
        Left = 264
        Top = 192
        Width = 134
        Height = 80
        Caption = #27599#21608#25773#25918#26102#27573
        TabOrder = 19
        object lbStartWeek: TLabel
          Left = 6
          Top = 23
          Width = 36
          Height = 12
          Alignment = taRightJustify
          Caption = #36215#22987#65306
        end
        object lbEndWeek: TLabel
          Left = 6
          Top = 49
          Width = 36
          Height = 12
          Alignment = taRightJustify
          Caption = #32456#27490#65306
        end
        object cbStartWeek: TComboBox
          Left = 48
          Top = 20
          Width = 70
          Height = 20
          Style = csDropDownList
          DropDownCount = 16
          ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
          ItemIndex = 0
          TabOrder = 0
          Text = #26143#26399#26085
          Items.Strings = (
            #26143#26399#26085
            #26143#26399#19968
            #26143#26399#20108
            #26143#26399#19977
            #26143#26399#22235
            #26143#26399#20116
            #26143#26399#20845)
        end
        object cbEndWeek: TComboBox
          Left = 48
          Top = 46
          Width = 70
          Height = 20
          Style = csDropDownList
          DropDownCount = 16
          ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
          ItemIndex = 6
          TabOrder = 1
          Text = #26143#26399#20845
          Items.Strings = (
            #26143#26399#26085
            #26143#26399#19968
            #26143#26399#20108
            #26143#26399#19977
            #26143#26399#22235
            #26143#26399#20116
            #26143#26399#20845)
        end
      end
      object cbRefActIndex: TComboBox
        Left = 411
        Top = 107
        Width = 72
        Height = 20
        Style = csDropDownList
        DropDownCount = 10
        Enabled = False
        ImeName = #20013#25991' ('#31616#20307') - '#24494#36719#25340#38899#36755#20837#27861' 3.0 '#29256
        TabOrder = 20
      end
      object scrAreas: TScrollBox
        Left = 443
        Top = 10
        Width = 238
        Height = 91
        HorzScrollBar.Position = 1
        BevelKind = bkFlat
        Color = clBtnFace
        ParentColor = False
        TabOrder = 21
        object imageAreas: TImage
          Left = 14
          Top = 11
          Width = 199
          Height = 89
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
      object vt1: TVirtualStringTree
        Left = 23
        Top = 299
        Width = 333
        Height = 92
        Header.AutoSizeIndex = -1
        Header.DefaultHeight = 17
        Header.Font.Charset = DEFAULT_CHARSET
        Header.Font.Color = clWindowText
        Header.Font.Height = -11
        Header.Font.Name = 'Tahoma'
        Header.Font.Style = []
        Header.Options = [hoColumnResize, hoDrag, hoShowSortGlyphs, hoVisible]
        Images = TreeImages
        TabOrder = 22
        TreeOptions.AutoOptions = [toAutoDropExpand, toAutoExpand, toAutoScroll, toAutoScrollOnExpand, toAutoTristateTracking, toAutoDeleteMovedNodes]
        TreeOptions.MiscOptions = [toAcceptOLEDrop, toCheckSupport, toFullRepaintOnResize, toInitOnSave, toToggleOnDblClick, toWheelPanning, toEditOnClick]
        TreeOptions.SelectionOptions = [toExtendedFocus, toFullRowSelect, toMiddleClickSelect]
        TreeOptions.StringOptions = [toSaveCaptions, toShowStaticText, toAutoAcceptEditChange]
        Visible = False
        OnGetNodeDataSize = vt1GetNodeDataSize
        OnInitNode = vt1InitNode
        OnLoadNode = vt1LoadNode
        Columns = <
          item
            Position = 1
            Width = 300
            WideText = #21517#31216'/'#25551#36848
          end
          item
            Color = clBtnShadow
            Options = [coAllowClick, coDraggable, coEnabled, coParentBidiMode, coResizable, coShowDropMark, coVisible, coAllowFocus]
            Position = 0
            Width = 22
          end
          item
            Position = 2
            Width = 100
            WideText = #32534#30721
          end>
      end
    end
    object panelScreen: TPanel
      Left = 1
      Top = 1
      Width = 694
      Height = 207
      Align = alClient
      TabOrder = 1
      OnResize = panelScreenResize
      object btnAlignLeft: TSpeedButton
        Left = 21
        Top = 5
        Width = 25
        Height = 25
        Hint = #38752#24038
        GroupIndex = 4
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          88888888888888888888CCCCCCCCCC8888887777777777888888888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          88888888888888888888CCCCCCCCCC8888887777777777888888888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          88888888888888888888CCCCCCCCCC8888887777777777888888888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          8888888888888888888888888888888888888888888888888888}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnAlignClick
      end
      object btnAlignCenter: TSpeedButton
        Tag = 1
        Left = 45
        Top = 5
        Width = 25
        Height = 25
        Hint = #27700#24179#23621#20013
        GroupIndex = 4
        Down = True
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          8888888888888888888888CCCCCCCCCC88888877777777778888888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          8888888888888888888888CCCCCCCCCC88888877777777778888888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          8888888888888888888888CCCCCCCCCC88888877777777778888888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          8888888888888888888888888888888888888888888888888888}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnAlignClick
      end
      object btnAlignRight: TSpeedButton
        Tag = 2
        Left = 68
        Top = 5
        Width = 25
        Height = 25
        Hint = #38752#21491
        GroupIndex = 4
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          888888888888888888888888CCCCCCCCCC888888777777777788888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          888888888888888888888888CCCCCCCCCC888888777777777788888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          888888888888888888888888CCCCCCCCCC888888777777777788888888888888
          88888888888888888888CCCCCCCCCCCCCC887777777777777788888888888888
          8888888888888888888888888888888888888888888888888888}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnAlignClick
      end
      object btnAlignVertCenter: TSpeedButton
        Tag = 1
        Left = 92
        Top = 5
        Width = 25
        Height = 25
        Hint = #22402#30452#23621#20013
        AllowAllUp = True
        GroupIndex = 1
        Down = True
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF008C888C888C88
          8C888788878887888788CCC8CCC8CCC8CCC877787778777877788C888C888C88
          8C8887888788878887888C888C888C888C888788878887888788888888888888
          888888888888888888888888CCC8CCC88888888877787778888888884C888C48
          8888888877888778888888888CCCCC8888888888877777888888888884C8C488
          88888888877877888888888888CCC888888888888877788888888888884C4888
          888888888877788888888888888C8888888888888887888888888C888C888C88
          8C8887888788878887888C888C888C888C888788878887888788CCC8CCC8CCC8
          CCC877787778777877788C888C888C888C888788878887888788}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnAlignVertCenterClick
      end
      object btnRed: TSpeedButton
        Left = 120
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#32418#33394
        GroupIndex = 5
        Down = True
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888888889988998
          8888888881188118888888888998899888888888811881188888888889988998
          8888888881188118888888888999999888888888811111188888888889988998
          8888888881188118888888888998899888888888811881188888888889988998
          8888888881188118888888888899998888888888881111888888888888899888
          8888888888811888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnRedClick
      end
      object btnGreen: TSpeedButton
        Left = 142
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#32511#33394
        GroupIndex = 5
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          888888888888888888888888888888888888888888888888888888888AA88AA8
          8888888882288228888888888AA88AA88888888882288228888888888AA88AA8
          8888888882288228888888888AAAAAA88888888882222228888888888AA88AA8
          8888888882288228888888888AA88AA88888888882288228888888888AA88AA8
          88888888822882288888888888AAAA88888888888822228888888888888AA888
          8888888888822888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnGreenClick
      end
      object btnYellow: TSpeedButton
        Left = 164
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#40644#33394
        GroupIndex = 5
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          888888888888888888888888888888888888888888888888888888888BB88BB8
          8888888883388338888888888BB88BB88888888883388338888888888BB88BB8
          8888888883388338888888888BBBBBB88888888883333338888888888BB88BB8
          8888888883388338888888888BB88BB88888888883388338888888888BB88BB8
          88888888833883388888888888BBBB88888888888833338888888888888BB888
          8888888888833888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        Margin = 1
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnYellowClick
      end
      object btnBlack: TSpeedButton
        Left = 186
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#40657#33394
        GroupIndex = 5
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888888880088008
          8888888887788778888888888008800888888888877887788888888880088008
          8888888887788778888888888000000888888888877777788888888880088008
          8888888887788778888888888008800888888888877887788888888880088008
          8888888887788778888888888800008888888888887777888888888888800888
          8888888888877888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnBlackClick
      end
      object btnRed_Back: TSpeedButton
        Left = 310
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#32418#33394
        GroupIndex = 6
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          7777777777777777777779999999999999771111111111111117799999999999
          9977111111111111111779999999999999771111111111111117799990099009
          9977111117711771111779999009900999771111177117711117799990099009
          9977111117711771111779999000000999771111177777711117799990099009
          9977111117711771111779999009900999771111177117711117799990099009
          9977111117711771111779999900009999771111117777111117799999900999
          9977111111177111111779999999999999771111111111111117799999999999
          9977111111111111111777777777777777777777777777777777}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnRed_BackClick
      end
      object btnGreen_Back: TSpeedButton
        Left = 332
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#32511#33394
        GroupIndex = 6
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          777777777777777777777AAAAAAAAAAAAA7722222222222222277AAAAAAAAAAA
          AA7722222222222222277AAAAAAAAAAAAA7722222222222222277AAAA00AA00A
          AA7722222772277222277AAAA00AA00AAA7722222772277222277AAAA00AA00A
          AA7722222772277222277AAAA000000AAA7722222777777222277AAAA00AA00A
          AA7722222772277222277AAAA00AA00AAA7722222772277222277AAAA00AA00A
          AA7722222772277222277AAAAA0000AAAA7722222277772222277AAAAAA00AAA
          AA7722222227722222277AAAAAAAAAAAAA7722222222222222277AAAAAAAAAAA
          AA77222222222222222777777777777777777777777777777777}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnGreen_BackClick
      end
      object btnBlack_Back: TSpeedButton
        Left = 376
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#40657#33394
        GroupIndex = 6
        Down = True
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          7777777777777777777770000000000000778888888888888887700000000000
          0077888888888888888770000000000000778888888888888887700009900990
          0077888887788778888770000990099000778888877887788887700009900990
          0077888887788778888770000999999000778888877777788887700009900990
          0077888887788778888770000990099000778888877887788887700009900990
          0077888887788778888770000099990000778888887777888887700000099000
          0077888888877888888770000000000000778888888888888887700000000000
          0077888888888888888777777777777777777777777777777777}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnBlack_BackClick
      end
      object lbDisplayTime: TLabel
        Left = 244
        Top = 41
        Width = 54
        Height = 12
        Caption = '5000 '#27627#31186
      end
      object lbPlaytime: TLabel
        Left = 136
        Top = 41
        Width = 102
        Height = 12
        Caption = #24403#21069#33410#30446#25773#25918#26102#38388':'
      end
      object btnYellow_Back: TSpeedButton
        Left = 354
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#40644#33394
        GroupIndex = 6
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          777777777777777777777BBBBBBBBBBBBB7733333333333333377BBBBBBBBBBB
          BB7733333333333333377BBBBBBBBBBBBB7733333333333333377BBBB00BB00B
          BB7733333773377333377BBBB00BB00BBB7733333773377333377BBBB00BB00B
          BB7733333773377333377BBBB000000BBB7733333777777333377BBBB00BB00B
          BB7733333773377333377BBBB00BB00BBB7733333773377333377BBBB00BB00B
          BB7733333773377333377BBBBB0000BBBB7733333377773333377BBBBBB00BBB
          BB7733333337733333377BBBBBBBBBBBBB7733333333333333377BBBBBBBBBBB
          BB77333333333333333777777777777777777777777777777777}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnYellow_BackClick
      end
      object lbPlayTimeAll: TLabel
        Left = 348
        Top = 41
        Width = 66
        Height = 12
        Caption = #24635#25773#25918#26102#38388':'
      end
      object lbAllDisplayTime: TLabel
        Left = 420
        Top = 41
        Width = 54
        Height = 12
        Caption = '5000 '#27627#31186
      end
      object btnBlue: TSpeedButton
        Left = 208
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#34013#33394
        GroupIndex = 5
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          888888888888888888888888888888888888888888888888888888888CC88CC8
          8888888884488448888888888CC88CC88888888884488448888888888CC88CC8
          8888888884488448888888888CCCCCC88888888884444448888888888CC88CC8
          8888888884488448888888888CC88CC88888888884488448888888888CC88CC8
          88888888844884488888888888CCCC88888888888844448888888888888CC888
          8888888888844888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnBlueClick
      end
      object btnPink: TSpeedButton
        Left = 230
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#31881#32418#33394
        GroupIndex = 5
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          888888888888888888888888888888888888888888888888888888888DD88DD8
          8888888885588558888888888DD88DD88888888885588558888888888DD88DD8
          8888888885588558888888888DDDDDD88888888885555558888888888DD88DD8
          8888888885588558888888888DD88DD88888888885588558888888888DD88DD8
          88888888855885588888888888DDDD88888888888855558888888888888DD888
          8888888888855888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        Margin = 1
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnPinkClick
      end
      object btnAqua: TSpeedButton
        Left = 252
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#27973#32511#33394
        GroupIndex = 5
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          888888888888888888888888888888888888888888888888888888888EE88EE8
          8888888886688668888888888EE88EE88888888886688668888888888EE88EE8
          8888888886688668888888888EEEEEE88888888886666668888888888EE88EE8
          8888888886688668888888888EE88EE88888888886688668888888888EE88EE8
          88888888866886688888888888EEEE88888888888866668888888888888EE888
          8888888888866888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        Margin = 1
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnAquaClick
      end
      object btnWhite: TSpeedButton
        Left = 276
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#21069#26223#65306#30333#33394
        GroupIndex = 5
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          8888888888888888888888888888888888888888888888888888888888888888
          888888888888888888888888888888888888888888888888888888888FF88FF8
          8888888887788778888888888FF88FF88888888887788778888888888FF88FF8
          8888888887788778888888888FFFFFF88888888887777778888888888FF88FF8
          8888888887788778888888888FF88FF88888888887788778888888888FF88FF8
          88888888877887788888888888FFFF88888888888877778888888888888FF888
          8888888888877888888888888888888888888888888888888888888888888888
          8888888888888888888888888888888888888888888888888888}
        Margin = 1
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnWhiteClick
      end
      object btnBlue_Back: TSpeedButton
        Left = 398
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#34013#33394
        GroupIndex = 6
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          777777777777777777777CCCCCCCCCCCCC7744444444444444477CCCCCCCCCCC
          CC7744444444444444477CCCCCCCCCCCCC7744444444444444477CCCC00CC00C
          CC7744444774477444477CCCC00CC00CCC7744444774477444477CCCC00CC00C
          CC7744444774477444477CCCC000000CCC7744444777777444477CCCC00CC00C
          CC7744444774477444477CCCC00CC00CCC7744444774477444477CCCC00CC00C
          CC7744444774477444477CCCCC0000CCCC7744444477774444477CCCCCC00CCC
          CC7744444447744444477CCCCCCCCCCCCC7744444444444444477CCCCCCCCCCC
          CC77444444444444444777777777777777777777777777777777}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
        OnClick = btnBlue_BackClick
      end
      object btnPink_Back: TSpeedButton
        Left = 420
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#31881#32418#33394
        GroupIndex = 6
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          777777777777777777777DDDDDDDDDDDDD7755555555555555577DDDDDDDDDDD
          DD7755555555555555577DDDDDDDDDDDDD7755555555555555577DDDD00DD00D
          DD7755555775577555577DDDD00DD00DDD7755555775577555577DDDD00DD00D
          DD7755555775577555577DDDD000000DDD7755555777777555577DDDD00DD00D
          DD7755555775577555577DDDD00DD00DDD7755555775577555577DDDD00DD00D
          DD7755555775577555577DDDDD0000DDDD7755555577775555577DDDDDD00DDD
          DD7755555557755555577DDDDDDDDDDDDD7755555555555555577DDDDDDDDDDD
          DD77555555555555555777777777777777777777777777777777}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
      end
      object btnAqua_Back: TSpeedButton
        Left = 442
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#27973#32511#33394
        GroupIndex = 6
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          777777777777777777777EEEEEEEEEEEEE7766666666666666677EEEEEEEEEEE
          EE7766666666666666677EEEEEEEEEEEEE7766666666666666677EEEE00EE00E
          EE7766666776677666677EEEE00EE00EEE7766666776677666677EEEE00EE00E
          EE7766666776677666677EEEE000000EEE7766666777777666677EEEE00EE00E
          EE7766666776677666677EEEE00EE00EEE7766666776677666677EEEE00EE00E
          EE7766666776677666677EEEEE0000EEEE7766666677776666677EEEEEE00EEE
          EE7766666667766666677EEEEEEEEEEEEE7766666666666666677EEEEEEEEEEE
          EE77666666666666666777777777777777777777777777777777}
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
      end
      object btnWhite_Back: TSpeedButton
        Left = 464
        Top = 5
        Width = 25
        Height = 25
        Hint = #25991#23383#32972#26223#65306#30333#33394
        GroupIndex = 6
        Enabled = False
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          0400000000000001000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777777777
          777777777777777777777FFFFFFFFFFFFF7788888888888888877FFFFFFFFFFF
          FF7788888888888888877FFFFFFFFFFFFF7788888888888888877FFFF00FF00F
          FF7788888778877888877FFFF00FF00FFF7788888778877888877FFFF00FF00F
          FF7788888778877888877FFFF000000FFF7788888777777888877FFFF00FF00F
          FF7788888778877888877FFFF00FF00FFF7788888778877888877FFFF00FF00F
          FF7788888778877888877FFFFF0000FFFF7788888877778888877FFFFFF00FFF
          FF7788888887788888877FFFFFFFFFFFFF7788888888888888877FFFFFFFFFFF
          FF77888888888888888777777777777777777777777777777777}
        Margin = 1
        NumGlyphs = 2
        ParentShowHint = False
        ShowHint = True
      end
      object Image1: TImage
        Left = 144
        Top = 120
        Width = 33
        Height = 33
        Visible = False
      end
      object scrollboxEdit: TScrollBox
        Left = 21
        Top = 64
        Width = 500
        Height = 202
        BevelInner = bvLowered
        BevelOuter = bvRaised
        BevelKind = bkFlat
        BorderStyle = bsNone
        Color = clBtnFace
        ParentColor = False
        TabOrder = 0
        OnResize = scrollboxEditResize
        object imgScreen: TImage
          Left = 16
          Top = 1
          Width = 449
          Height = 289
          Visible = False
        end
        object imgPreview: TImage
          Left = 5
          Top = 20
          Width = 330
          Height = 246
          Visible = False
        end
        object imgActive: TImage
          Left = 32
          Top = 7
          Width = 449
          Height = 289
          Visible = False
        end
      end
      object btnFont: TButton
        Left = 515
        Top = 8
        Width = 86
        Height = 22
        Caption = #23383#20307'...'
        TabOrder = 1
        OnClick = btnFontClick
      end
      object btnCalcDisplayTime: TButton
        Left = 19
        Top = 36
        Width = 112
        Height = 22
        Caption = #21047#26032#25773#25918#26102#38388
        TabOrder = 2
        OnClick = btnCalcDisplayTimeClick
      end
    end
  end
  object toolBar: TToolBar
    Left = 0
    Top = 0
    Width = 1148
    Height = 23
    AutoSize = True
    ButtonHeight = 23
    Caption = 'toolBar'
    GradientDirection = gdHorizontal
    TabOrder = 2
    object btnCardList: TSpeedButton
      Left = 0
      Top = 0
      Width = 22
      Height = 23
      Hint = #25511#21046#21345#31649#29702
      Glyph.Data = {
        26050000424D26050000000000003604000028000000100000000F0000000100
        080000000000F000000000000000000000000001000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
        A6000020400000206000002080000020A0000020C0000020E000004000000040
        20000040400000406000004080000040A0000040C0000040E000006000000060
        20000060400000606000006080000060A0000060C0000060E000008000000080
        20000080400000806000008080000080A0000080C0000080E00000A0000000A0
        200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
        200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
        200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
        20004000400040006000400080004000A0004000C0004000E000402000004020
        20004020400040206000402080004020A0004020C0004020E000404000004040
        20004040400040406000404080004040A0004040C0004040E000406000004060
        20004060400040606000406080004060A0004060C0004060E000408000004080
        20004080400040806000408080004080A0004080C0004080E00040A0000040A0
        200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
        200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
        200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
        20008000400080006000800080008000A0008000C0008000E000802000008020
        20008020400080206000802080008020A0008020C0008020E000804000008040
        20008040400080406000804080008040A0008040C0008040E000806000008060
        20008060400080606000806080008060A0008060C0008060E000808000008080
        20008080400080806000808080008080A0008080C0008080E00080A0000080A0
        200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
        200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
        200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
        2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
        2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
        2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
        2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
        2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
        2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
        2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00070707070707
        0000000000000707070700000000000007FBFBFBFB0000000707020702A4A400
        02020202020002000707060000000006A4000000000000000707060000000000
        0006FBFBFBFB0000000706020002A4A400020202020200020007020600000000
        06A40000000000000007020600000000000006FBFBFBFB0000000606020002A4
        A40002020202020002000202060000000006A4060206060206000702060003A4
        A400A406A400060000000706060000060006A4060606A406060007020206A4A4
        0600A40606000006A400070702000006000607070607A400070607070607A4A4
        0700A4A4060707000706}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuCardListClick
    end
    object btnCommConfig: TSpeedButton
      Left = 22
      Top = 0
      Width = 22
      Height = 23
      Hint = #36890#20449#37197#32622
      Glyph.Data = {
        36050000424D3605000000000000360400002800000010000000100000000100
        0800000000000001000000000000000000000001000000010000FF00FF000062
        00000065000000680000006E0000036B0400046E08000276030004760700007D
        0000037A0500047B050006720A0009760E0009780E0000605300006757000063
        5F00026C5B00005F7800006375000184010007800C00088C0D0000970000059F
        07000C8513000D88190011841300108019001296110012892100148E28001D93
        2E001B9A2D001A9E3500249D380018A32E0027B84C0036BA510033C34E002EC6
        5C0036CF540044DD65004BE47E000066CE000268D000056BD300076DD300076D
        D4000B6DD7000A70D8000D73DB000E74DB001177DE00197FDE001277E600187E
        E000197EEA001C82E9002288E0002286EB002186EE00278CEC00298FEB00288E
        EE002A90EE002F95ED003499FE00379CFF003BA0EE003FA5FA003DA2FF0045AB
        FF004AAFFF0058BDFF0000CCFF0011EEED005DC3FF0000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        000000000000000033000000000000000000000000003137313100004D4C0000
        000000000031434A31000000004D4C0000313131313F472E0000000000000000
        32323F4B4B3F3100000000000000001338473C4649452E000000000000001C22
        14414B393E452E0000000000000C1F2C27113E45363B310000000000000E2924
        2B1E0F3A4533310000000000000E29231A19150F3831000000000000000E2625
        160A190A1200000000000000001F1B17150A0404004D4C000000000021280802
        0204020000004D4C0000001D2A17020000000000000000000000040608020000
        0000000000000000000020040000000000000000000000000000}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuCommConfigClick
    end
    object btnCardConfig: TSpeedButton
      Left = 44
      Top = 0
      Width = 22
      Height = 23
      Hint = #25511#21046#21345#37197#32622
      Glyph.Data = {
        D6050000424DD6050000000000003600000028000000200000000F0000000100
        180000000000A005000000000000000000000000000000000000F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767DDDDDDF0F0
        F0DDDDDD676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0676767DDDDDDF0F0F0DDDDDD676767F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767AEAEAEF0F0F0F0F0
        F0F0F0F0AEAEAE676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0676767AEAEAEF0F0F0F0F0F0F0F0F0AEAEAE676767F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767AEAEAEDDDDDDF0F0
        F0DDDDDDAEAEAE676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0676767AEAEAEDDDDDDF0F0F0DDDDDDAEAEAE676767F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767AEAEAEDDDD
        DDAEAEAE676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0676767AEAEAEDDDDDDAEAEAE676767F0F0F0BDAC9B5E4505
        5E4505BDAC9BF0F0F0F0F0F0BDAC9B5E45055E4505BDAC9BF0F0F0676767AEAE
        AE676767F0F0F0F0F0F0BDAC9B7F7F7F7F7F7FBDAC9BF0F0F0F0F0F0BDAC9B7F
        7F7F7F7F7FBDAC9BF0F0F0676767AEAEAE676767F0F0F0F0F0F05D4404BC9122
        B984005E4505F0F0F0F0F0F05E4505B98400BC9122674D0CF0F0F0676767DDDD
        DD676767F0F0F0F0F0F07F7F7FC3C3C3C3C3C37F7F7FF0F0F0F0F0F07F7F7FC3
        C3C3C3C3C37F7F7FF0F0F0676767DDDDDD676767F0F0F0F0F0F05D440495731F
        6F4E005E4505F0F0F0F0F0F05E45056F4E0095731F684E0BF0F0F0676767DDDD
        DD676767F0F0F0F0F0F07F7F7FC3C3C37F7F7F7F7F7FF0F0F0F0F0F07F7F7F7F
        7F7FC3C3C37F7F7FF0F0F0676767DDDDDD676767F0F0F0F0F0F05D440495731F
        6F4E005E45055E45055E45055E45056F4E0095731F684E0BF0F0F0676767AEAE
        AE676767F0F0F0F0F0F07F7F7FC3C3C37F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F
        7F7FC3C3C37F7F7FF0F0F0676767AEAEAE676767F0F0F0F0F0F05D440495731F
        6F4E005E4505BC9122BC91225E45056F4E0095731F684E0BF0F0F0676767DDDD
        DD676767F0F0F0F0F0F07F7F7FC3C3C37F7F7F7F7F7FC3C3C3C3C3C37F7F7F7F
        7F7FC3C3C37F7F7FF0F0F0676767DDDDDD676767F0F0F0F0F0F05D440495731F
        6F4E005E45055E45055E45055E45056F4E0095731F694F0CF0F0F0676767DDDD
        DD676767F0F0F0F0F0F07F7F7FC3C3C37F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F
        7F7FC3C3C37F7F7FF0F0F0676767DDDDDD676767F0F0F0F0F0F0F0F0F05E4505
        5E45055E4505F0F0F0F0F0F05E45055E45055E4505F0F0F0F0F0F0676767AEAE
        AE676767F0F0F0F0F0F0F0F0F07F7F7F7F7F7F7F7F7FF0F0F0F0F0F07F7F7F7F
        7F7F7F7F7FF0F0F0F0F0F0676767AEAEAE676767F0F0F0F0F0F0F0F0F05E4505
        C99B6D5E4505F0F0F0F0F0F05E4505B68B5B5E4505F0F0F0676767AEAEAEDDDD
        DDAEAEAE676767F0F0F0F0F0F07F7F7FC3C3C37F7F7FF0F0F0F0F0F07F7F7FC3
        C3C37F7F7FF0F0F0676767AEAEAEDDDDDDAEAEAE676767F0F0F0F0F0F0BDAC9B
        5E4505BDAC9BF0F0F0F0F0F0BDAC9B5E4505BDAC9B676767AEAEAEDDDDDDF0F0
        F0DDDDDDAEAEAE676767F0F0F0BDAC9B7F7F7FBDAC9BF0F0F0F0F0F0BDAC9B7F
        7F7FBDAC9B676767AEAEAEDDDDDDF0F0F0DDDDDDAEAEAE676767F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767AEAEAEF0F0F0F0F0
        F0F0F0F0AEAEAE676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0676767AEAEAEF0F0F0F0F0F0F0F0F0AEAEAE676767F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767DDDDDDF0F0
        F0DDDDDD676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0676767DDDDDDF0F0F0DDDDDD676767F0F0F0}
      NumGlyphs = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuCardConfigClick
    end
    object btnSelectAllCard: TSpeedButton
      Left = 66
      Top = 0
      Width = 22
      Height = 23
      Hint = #36873#25321#25152#26377#26174#31034#23631
      Glyph.Data = {
        26050000424D26050000000000003604000028000000100000000F0000000100
        080000000000F000000000000000000000000001000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
        A6000020400000206000002080000020A0000020C0000020E000004000000040
        20000040400000406000004080000040A0000040C0000040E000006000000060
        20000060400000606000006080000060A0000060C0000060E000008000000080
        20000080400000806000008080000080A0000080C0000080E00000A0000000A0
        200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
        200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
        200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
        20004000400040006000400080004000A0004000C0004000E000402000004020
        20004020400040206000402080004020A0004020C0004020E000404000004040
        20004040400040406000404080004040A0004040C0004040E000406000004060
        20004060400040606000406080004060A0004060C0004060E000408000004080
        20004080400040806000408080004080A0004080C0004080E00040A0000040A0
        200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
        200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
        200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
        20008000400080006000800080008000A0008000C0008000E000802000008020
        20008020400080206000802080008020A0008020C0008020E000804000008040
        20008040400080406000804080008040A0008040C0008040E000806000008060
        20008060400080606000806080008060A0008060C0008060E000808000008080
        20008080400080806000808080008080A0008080C0008080E00080A0000080A0
        200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
        200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
        200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
        2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
        2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
        2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
        2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
        2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
        2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
        2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFA407F6EDF6F6
        A4FFFFFFFFFFFFFFFFFFA4079A9AFFF6A4000000000000FFFFFFA407ED07E308
        A407FBFBFBFB000000FFA4F507F69AF6A40202020202000200FFA4F5F5F5ED08
        A4A406020606020600FFA4A4A4A4A4A4A4A406A4000600000000A4A4A4A4A4A4
        A407060606A4060600A4A407F6EDF6F6A4A40606000006A400A4A4079A9AFFF6
        A407070607A4000706A4A407ED07E308A4A4A4060707000706A4A4F507F69AF6
        A4A4A406A40606A400FFA4F5F5F5ED08A4FFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
        A4FFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuSelectAllCardClick
    end
    object btnUnSelectAllCard: TSpeedButton
      Left = 88
      Top = 0
      Width = 22
      Height = 23
      Hint = #21462#28040#36873#25321#25152#26377#26174#31034#23631
      Glyph.Data = {
        26050000424D26050000000000003604000028000000100000000F0000000100
        080000000000F000000000000000000000000001000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
        A6000020400000206000002080000020A0000020C0000020E000004000000040
        20000040400000406000004080000040A0000040C0000040E000006000000060
        20000060400000606000006080000060A0000060C0000060E000008000000080
        20000080400000806000008080000080A0000080C0000080E00000A0000000A0
        200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
        200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
        200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
        20004000400040006000400080004000A0004000C0004000E000402000004020
        20004020400040206000402080004020A0004020C0004020E000404000004040
        20004040400040406000404080004040A0004040C0004040E000406000004060
        20004060400040606000406080004060A0004060C0004060E000408000004080
        20004080400040806000408080004080A0004080C0004080E00040A0000040A0
        200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
        200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
        200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
        20008000400080006000800080008000A0008000C0008000E000802000008020
        20008020400080206000802080008020A0008020C0008020E000804000008040
        20008040400080406000804080008040A0008040C0008040E000806000008060
        20008060400080606000806080008060A0008060C0008060E000808000008080
        20008080400080806000808080008080A0008080C0008080E00080A0000080A0
        200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
        200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
        200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
        2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
        2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
        2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
        2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
        2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
        2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
        2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFA4FFFFFFF6F6
        A4FFFFFFFFFFFFFFFFFFA4FFFFFFFFF6A4000000000000FFFFFFA4FFFFFFFFFF
        A407FBFBFBFB000000FFA4FFFFFFFFF6A40202020202000200FFA4FFFFFFFFFF
        A4A406020606020600FFA4A4A4A4A4A4A4A406A4000600000000A4A4A4A4A4A4
        A407060606A4060600A4A4FFF6FFF6F6A4A40606000006A400A4A4FFFFFFFFF6
        A407070607A4000706A4A4FFFFFFFFFFA4A4A4060707000706A4A4FFFFF6FFF6
        A4A4A406A40606A400FFA4FFFFFFFFFFA4FFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
        A4FFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuCancelAllCardClick
    end
    object btnSelectAllAct: TSpeedButton
      Left = 110
      Top = 0
      Width = 22
      Height = 23
      Hint = #36873#25321#25152#26377#33410#30446
      Glyph.Data = {
        36050000424D3605000000000000360400002800000010000000100000000100
        0800000000000001000000000000000000000001000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
        A6000020400000206000002080000020A0000020C0000020E000004000000040
        20000040400000406000004080000040A0000040C0000040E000006000000060
        20000060400000606000006080000060A0000060C0000060E000008000000080
        20000080400000806000008080000080A0000080C0000080E00000A0000000A0
        200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
        200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
        200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
        20004000400040006000400080004000A0004000C0004000E000402000004020
        20004020400040206000402080004020A0004020C0004020E000404000004040
        20004040400040406000404080004040A0004040C0004040E000406000004060
        20004060400040606000406080004060A0004060C0004060E000408000004080
        20004080400040806000408080004080A0004080C0004080E00040A0000040A0
        200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
        200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
        200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
        20008000400080006000800080008000A0008000C0008000E000802000008020
        20008020400080206000802080008020A0008020C0008020E000804000008040
        20008040400080406000804080008040A0008040C0008040E000806000008060
        20008060400080606000806080008060A0008060C0008060E000808000008080
        20008080400080806000808080008080A0008080C0008080E00080A0000080A0
        200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
        200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
        200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
        2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
        2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
        2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
        2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
        2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
        2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
        2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
        A4FCFCFCFCFCFCFCFCFCA407F6EDF6F6A4D2FFFFFFFFFFFFFFFCA4079A9AFFF6
        A4D2FFFFFFFFFFFFFFFCA407ED07E308A4D2FF4F4F4F4F4FFFFCA4F507F69AF6
        A4D2FFFFFFFFFFFFFFFCA4F5F5F5ED08A4D2FFFFFFFFFFFFFFFCA4A4A4A4A4A4
        A4D2FF4F4F4F4F4FFFFCA4A4A4A4A4A4A4D2FFFFFFFFFFFFFFFCA407F6EDF6F6
        A4D2FFFFFFFFFFFFFFFCA4079A9AFFF6A4D2FFFFFFFFFCFCFCFCA407ED07E308
        A4D2FFFFFFFFFCFFFCFFA4F507F69AF6A4D2FFFFFFFFFCFCFFFFA4F5F5F5ED08
        A4D2FCFCFCFCFCFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuSelectAllActClick
    end
    object btnUnSelectAllAct: TSpeedButton
      Left = 132
      Top = 0
      Width = 22
      Height = 23
      Hint = #21462#28040#36873#25321#25152#26377#33410#30446
      Glyph.Data = {
        36050000424D3605000000000000360400002800000010000000100000000100
        0800000000000001000000000000000000000001000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
        A6000020400000206000002080000020A0000020C0000020E000004000000040
        20000040400000406000004080000040A0000040C0000040E000006000000060
        20000060400000606000006080000060A0000060C0000060E000008000000080
        20000080400000806000008080000080A0000080C0000080E00000A0000000A0
        200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
        200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
        200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
        20004000400040006000400080004000A0004000C0004000E000402000004020
        20004020400040206000402080004020A0004020C0004020E000404000004040
        20004040400040406000404080004040A0004040C0004040E000406000004060
        20004060400040606000406080004060A0004060C0004060E000408000004080
        20004080400040806000408080004080A0004080C0004080E00040A0000040A0
        200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
        200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
        200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
        20008000400080006000800080008000A0008000C0008000E000802000008020
        20008020400080206000802080008020A0008020C0008020E000804000008040
        20008040400080406000804080008040A0008040C0008040E000806000008060
        20008060400080606000806080008060A0008060C0008060E000808000008080
        20008080400080806000808080008080A0008080C0008080E00080A0000080A0
        200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
        200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
        200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
        2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
        2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
        2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
        2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
        2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
        2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
        2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
        A4FCFCFCFCFCFCFCFCFCA4FFFFFFF6F6A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFF6
        A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFFFA4D2FF4F4F4F4F4FFFFCA4FFFFFFFFF6
        A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFFFA4D2FFFFFFFFFFFFFFFCA4A4A4A4A4A4
        A4D2FF4F4F4F4F4FFFFCA4A4A4A4A4A4A4D2FFFFFFFFFFFFFFFCA4FFF6FFF6F6
        A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFF6A4D2FFFFFFFFFCFCFCFCA4FFFFFFFFFF
        A4D2FFFFFFFFFCFFFCFFA4FFFFF6FFF6A4D2FFFFFFFFFCFCFFFFA4FFFFFFFFFF
        A4D2FCFCFCFCFCFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuCancelAllActClick
    end
    object btnDeleteActContent: TSpeedButton
      Left = 154
      Top = 0
      Width = 22
      Height = 23
      Hint = #21024#38500#36873#25321#30340#33410#30446#20869#23481
      Glyph.Data = {
        EE000000424DEE000000000000007600000028000000100000000F0000000100
        0400000000007800000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
        88888888888888888888888CCCCCCCCCCC88888CFFFFFFFFFC88888CFFFFFFFF
        FC88888CFFFFFFFFFC88888CFFFFFFFFFC88888CFFFFFFFFFC88888CFFFFFFFF
        FC88888CFFFFFFFFFC88888CFFFFFFFFFC88888CFFFFFFCCCC88888CFFFFFFCF
        C888888CFFFFFFCC8888888CCCCCCCC88888}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuDeleteActClick
    end
    object btnPreview: TSpeedButton
      Left = 176
      Top = 0
      Width = 22
      Height = 23
      Hint = #26174#31034#39044#35272
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        1800000000000003000000000000000000000000000000000000EBD9C5EAD8C4
        EBD8C5ECD9C5ECD9C5EBD9C5EBD9C5ECD9C5ECD9C5ECD9C5ECD9C5ECD9C5B1AF
        AB203040ACA7A1ECD9C5EAD8C4B0A09060483060483060483060483060483060
        48306048306048306048305048403050604078C0304860B1AFA6EBD9C5B0A090
        FFFFFFB0A090B0A090B0A090B0A090B0A090B0A090B0A0909088803050703080
        D04098E050B0F0506870EBD9C5B0A090FFFFFFFFFFFFFFF8FFF0F0F0D0D8D090
        989070686060686050586040709040A0E060C8FF7090A0C5C0B5EBD9C5B0A090
        FFFFFFFFFFFFFFFFFFE0E0E0909090B0A8A0D0C0B0D0B0A08078705058506090
        B07098B0AEB1ABECD9C6ECD9C5B0A090FFFFFFFFFFFFFFFFFFB0B0B0C0B8B0FF
        F0E0FFE8E0F0D8C0F0D0B08078709C908CAEB1ABECD9C6ECD9C6EBD9C5C0A890
        FFFFFFFFFFFFFFFFFFA09890F0E8E0FFF8F0FFF0F0FFE8E0F0D8D0D0B0A06468
        5FECDAC6ECDAC6ECD9C6ECD9C5C0A8A0FFFFFFFFFFFFFFFFFFA0A090F0E8E0FF
        FFFFFFF8F0FFF0F0FFE8E0E0C0B0726F6DECDAC6ECDAC6ECDAC6ECD9C5C0B0A0
        FFFFFFFFFFFFFFFFFFC0C8C0C0C0C0FFFFFFFFFFFFFFF8F0FFF0E0B0A090A59C
        93ECDAC6ECDAC6ECDAC6ECDAC7D0B0A0FFFFFFFFFFFFFFFFFFF0F8FFC0B8B0C0
        C0C0F0E8E0F0E8E0B0B0A0707060E5D3BFECDAC6ECDAC6ECDAC6ECDAC7D0B8A0
        FFFFFFFFFFFFFFFFFFFFFFFFF0F8FFC0C8C0A0A090909080909090605040ECDA
        C6ECDAC6ECDAC6ECDAC6ECDAC7D0B8B0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFB0A090604830604830604830ECDAC7ECDAC7ECDAC6ECDAC6ECDAC7D0C0B0
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0A890D0C8C0604830DCC1B0ECDA
        C7ECDAC7ECDAC7ECDAC7ECDAC8E0C0B0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFC0A8A0604830DCC1B0ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC8E0C0B0
        E0C0B0E0C0B0E0C0B0E0C0B0D0C0B0D0B8B0D0B0A0DCC2B0ECDAC7ECDAC7ECDA
        C7ECDAC7ECDAC7ECDAC7ECDBC8ECDBC8ECDAC8ECDAC8ECDAC8ECDAC8ECDAC7EC
        DAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuPreviewAllActClick
    end
    object btnFindNextCard: TSpeedButton
      Left = 198
      Top = 0
      Width = 22
      Height = 23
      Hint = #26597#25214#19979#19968#20010#26174#31034#23631
      Glyph.Data = {
        36050000424D3605000000000000360400002800000010000000100000000100
        0800000000000001000000000000000000000001000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
        A6000020400000206000002080000020A0000020C0000020E000004000000040
        20000040400000406000004080000040A0000040C0000040E000006000000060
        20000060400000606000006080000060A0000060C0000060E000008000000080
        20000080400000806000008080000080A0000080C0000080E00000A0000000A0
        200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
        200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
        200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
        20004000400040006000400080004000A0004000C0004000E000402000004020
        20004020400040206000402080004020A0004020C0004020E000404000004040
        20004040400040406000404080004040A0004040C0004040E000406000004060
        20004060400040606000406080004060A0004060C0004060E000408000004080
        20004080400040806000408080004080A0004080C0004080E00040A0000040A0
        200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
        200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
        200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
        20008000400080006000800080008000A0008000C0008000E000802000008020
        20008020400080206000802080008020A0008020C0008020E000804000008040
        20008040400080406000804080008040A0008040C0008040E000806000008060
        20008060400080606000806080008060A0008060C0008060E000808000008080
        20008080400080806000808080008080A0008080C0008080E00080A0000080A0
        200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
        200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
        200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
        2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
        2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
        2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
        2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
        2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
        2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
        2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF5050505050FFFFFFFFFF5050505050FF50F4989850
        FFFFFFFFFF50F4989850FF50EC989850FFFFFFFFFF50EC989850FF5098E89850
        5050FF50505098989850FF5098F4989850E85050F49898989850FF5098F49898
        50F45050F49898989850FFFF50E8989850E85050EC98985050FFFFFFFF50F498
        5050FF50F4989850FFFFFFFFFF50E8985050FF50E8989850FFFFFFFFFFFF5050
        50FFFFFF505050FFFFFFFFFFFFFF50EC50FFFFFF50EC50FFFFFFFFFFFFFF5050
        50FFFFFF505050FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = btnFindNextCardClick
    end
    object btnFindNextAct: TSpeedButton
      Left = 220
      Top = 0
      Width = 22
      Height = 23
      Hint = #26597#25214#19979#19968#20010#33410#30446
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        1800000000000003000000000000000000000000000000000000F0FBFFF0FBFF
        F0FBFFF0FBFF404000404000404000404000F0FBFFF0FBFFF0FBFF4040004040
        00404000404000FFFFFFF0FBFFF0FBFFF0FBFFF0FBFF404000808020C0A00040
        4000F0FBFFF0FBFFF0FBFF404000C0A000808020404000FFFFFFF0FBFFF0FBFF
        F0FBFFF0FBFF404000808020806000404000F0FBFFF0FBFFF0FBFF4040008060
        00808020404000FFFFFFF0FBFFF0FBFFF0FBFFF0FBFF40400080802080600040
        4000808020808020808020404000806000808020404000FFFFFFF0FBFFF0FBFF
        F0FBFFF0FBFF4040008080208060004040004040004040004040004040008060
        00808020404000FFFFFFF0FBFFF0FBFFF0FBFFF0FBFF40400080802080600040
        4000C0A000808020C0A000404000806000808020404000FFFFFFF0FBFFA4A0A0
        4040204040204040008080208060004040004040004040004040004040008060
        00808020404000FFFFFFF0FBFF40402080E0E080E0E040400080802080600040
        4000808020808020808020404000806000808020404000FFFFFFF0FBFF404020
        80E0E0A4A0A080808040400040400040400080E0E0404020F0FBFF4040004040
        00404000F0FBFFFFFFFFF0FBFF40402080E0E080E0E080E0E0404000C0A08040
        400080E0E0404020F0FBFF404000C0A080404000F0FBFFFFFFFFF0FBFF404020
        80E0E0A4A0A080808040400040400040400080E0E0404020F0FBFF4040004040
        00404000F0FBFFFFFFFFF0FBFF40402080E0E080E0E080E0E080E0E080E0E080
        E0E080E0E0404020F0FBFFF0FBFFF0FBFFF0FBFFF0FBFFFFFFFFF0FBFF404020
        80E0E0A4A0A080808080E0E0A4A0A0404020404020404020F0FBFFF0FBFFF0FB
        FFF0FBFFF0FBFFFFFFFFF0FBFF40402080E0E080E0E080E0E080E0E040402080
        E0E080E0E0404020F0FBFFF0FBFFF0FBFFF0FBFFF0FBFFFFFFFFF0FBFF404020
        80E0E080E0E080E0E080E0E040402080E0E0404020F0FBFFF0FBFFF0FBFFF0FB
        FFF0FBFFF0FBFFFFFFFFF0FBFFA4A0A040402040402040402040402040402040
        4020F0FBFFF0FBFFF0FBFFF0FBFFF0FBFFF0FBFFF0FBFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = btnFindNextActClick
    end
    object btnSendAct: TSpeedButton
      Left = 242
      Top = 0
      Width = 22
      Height = 23
      Hint = #21457#36865#33410#30446
      Glyph.Data = {
        66010000424D66010000000000007600000028000000200000000F0000000100
        040000000000F000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888882F22F28
        8888888882F22F28888888882F2FF2F2888888882F2FF2F288888882FF2FF2F2
        88888882FF2FF2F28888882FFFF2FF2F2888882FFFF2FF2F288882FFFFF2FF2F
        288882FFFFF2FF2F288886FFFCFF2FF2F28886FFFCFF2FF2F28882FFFCCF2FF2
        F28882FFFCCF2FF2F288CCCCCCCCF2FF2F28CCCCCCCCF2FF2F28CCCCCCCCF2FF
        2F28CCCCCCCCF2FF2F288882FCCFFF2FF2F28882FCCFFF2FF2F288882CFFFF2F
        F2F288882CFFFF2FF2F288882FFFF2FF2F2888882FFFF2FF2F28888882FF2FF2
        F288888882FF2FF2F288888882F2FF2F2888888882F2FF2F28888888882FF2F2
        88888888882FF2F28888}
      NumGlyphs = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuSendActClick
    end
    object btnCheckOnline: TSpeedButton
      Left = 264
      Top = 0
      Width = 22
      Height = 23
      Hint = #26816#26597#25511#21046#21345#22312#32447#29366#24577
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        1800000000000003000000000000000000000000000000000000F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F000000000
        0000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0000000B0DCFBABD6F595BFDE000000F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000A3
        CFEDB4E0FFA3CFEE000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F000000000000000000000000094BFDEAAD6F4B0DBFB000000F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000F0F0F0F0F0F000000086
        B0CF9CC7E6B0DBFA000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0000000F0F0F0F0F0F0000000000000000000000000000000F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000
        000000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F00000000000000000
        00000000000000F0F0F0F0F0F0000000AFDBFBAAD6F495BFDE000000F0F0F0F0
        F0F0F0F0F0F0F0F0000000A1CDECB4E0FFA1CDEC000000F0F0F0F0F0F0000000
        A3CEEDB4E0FFA4CFEE000000F0F0F0F0F0F0F0F0F0F0F0F00000008AB4D3A1CD
        ECB4E0FF000000F0F0F0F0F0F000000095BFDEAAD6F5AFDCFA00000000000000
        0000000000000000000000729BB989B4D3A2CCEC000000F0F0F0F0F0F0000000
        000000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F00000000000000000
        00000000000000F0F0F0F0F0F0000000FFFFFFFFFFFFFFFFFF000000F0F0F0F0
        F0F0F0F0F0F0F0F0000000FFFFFFFFFFFFFFFFFF000000F0F0F0F0F0F0000000
        000000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F00000000000000000
        00000000000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
        F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuCheckCardOnlineClick
    end
    object btnReadCardClock: TSpeedButton
      Left = 286
      Top = 0
      Width = 22
      Height = 23
      Hint = #26597#35810#25511#21046#21345#26102#38047
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        1800000000000003000000000000000000000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000
        0000000000000000000000000000000000000000FFFFFFFFFFFFFFFFFF000000
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FF000000FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF80808000000000
        0000000000808080FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF000000
        FFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFF
        FF000000FFFFFFFFFFFFFFFFFF000000FFFFFF808080FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF808080FFFFFF000000FFFFFFFFFFFFFFFFFF000000
        FFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFF
        FF000000FFFFFFFFFFFFFFFFFF000000FFFFFF000000FFFFFFFFFFFFFFFFFF00
        00FF000000000000FFFFFF000000FFFFFF000000FFFFFFFFFFFFFFFFFF000000
        FFFFFF000000FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF000000FFFF
        FF000000FFFFFFFFFFFFFFFFFF000000FFFFFF808080FFFFFFFFFFFFFFFFFF00
        0000FFFFFFFFFFFFFFFFFF808080FFFFFF000000FFFFFFFFFFFFFFFFFF000000
        FFFFFFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFF
        FF000000FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF80808000000000
        0000000000808080FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF000000
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000
        00000000FFFFFFFFFFFFFFFFFF0000000000FF0000FF0000FF0000FF0000FF00
        00FF0000FF0000FF000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000
        0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuReadCardClockClick
    end
    object btnWriteCardClock: TSpeedButton
      Left = 308
      Top = 0
      Width = 22
      Height = 23
      Hint = #26657#23545#25511#21046#21345#26102#38047
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        1800000000000003000000000000000000000000000000000000FFFFF0FFFFF0
        FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFF
        F0FFFFF0FFFFF0FFFFF0F7EFE7F7EFE7FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FF
        FFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0
        FFFFF0FFFFF0FFFFF0FFFFF0808080000000000000000000808080FFFFF0FFFF
        F0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FF
        FFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
        FFFFF0FFFFF0808080FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF08080
        80FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFF0FF
        FFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
        FFFFF0FFFFF0000000FFFFF0FFFFF0FFFFF00000FF000000000000FFFFF00000
        00FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFF0FF
        FFF0000000FFFFF0FFFFF0FFFFF0000000FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
        FFFFF0FFFFF0808080FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFF0FFFFF08080
        80FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FF
        FFF0000000FFFFF0FFFFF0000000FFFFF0FFFFFFFFFFFFFFFFF0FFFFF0EDB886
        F8F0E9FFFFF0FFFFF0FFFFF0808080000000000000000000808080FFFFF0FFFF
        F0FFFFFFFFFFFFFFFFF0FFFFF0EC8328F1CEACFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
        F4862DF0A768F5E1D1DA7E1DF3E2D1FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFF
        F0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0F7E4D3F4862DEC8328E38123DA7E1DE4
        B681FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0
        F8F1EBF8F1EBF7EADFEC8328F3DBC4F8F1EBFFFFF0FFFFF0FFFFF0FFFFF0FFFF
        F0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FF
        FFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0}
      ParentShowHint = False
      ShowHint = True
      OnClick = mnuAdjustCardClockClick
    end
    object btnTxWeatherWarnning: TSpeedButton
      Left = 330
      Top = 0
      Width = 22
      Height = 23
      Hint = #21457#24067#27668#35937#39044#35686#20449#21495
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000120B0000120B00000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        000000000000000000000000000000000000000000FF2F1BFF1304FE0000FF00
        00000000000000000000FF2D13FF0000FF000000000000000000000000000000
        0000000000FF0000FF0000FF442FFC0000000000000000FF1B11FE0000FF1F0F
        FC0000FF0000000000000000000000000000000000FF150DFF0000FF0100FD00
        00FF0000000000FF0000FF0000FF2F1FFB0000FF000000000000000000000000
        0000000000000000FF0000FF0000FF3726FD0000FF120DFE0000FF0500FD0000
        FF0000000000000000000000000000000000000000000000FF0000FF0000FF00
        00FF0000FF0000FF0000FF0000FF0000000000000000000000000000000000FF
        4E32F80000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0A09FF1B14
        FE0E06FE371EFB0000FF0000FF0000FE0000FF0000FF0000FF0000FF0000FF00
        00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FE0000FF0000FF0000FF
        0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
        FF0000FF1A08FE0000FF0000000000FF0000FF0000FF0000FF0000FF0000FF00
        00FF0000FF0000FF2A16FD0000FF0000FF0000FF0000FF000000000000000000
        0000000000000000003323FF0000FF0000FF0000FF0000FF0700FD0000FF0000
        000000000000000000000000000000000000000000000000FF0000FF0000FF3A
        24FB0000FF0000FF0000FF331DFB000000000000000000000000000000000000
        0000000000FF0700FF0000FF0800FD0000FF0000001E16FE0000FF0000FE0000
        FF0000000000000000000000000000000000000000FF0000FF0000FF0000FF00
        00000000000000FF0000FF0000FF0000FF000000000000000000000000000000
        0000000000000000FF5A3EFC0000FF0000000000000000FF583FFF462DFE0000
        FF00000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000}
      ParentShowHint = False
      ShowHint = True
      OnClick = btnTxWeatherWarnningClick
    end
    object btnGps: TSpeedButton
      Left = 352
      Top = 0
      Width = 25
      Height = 23
      Hint = 'GPS'#23450#20301
      Glyph.Data = {
        36030000424D3603000000000000360000002800000010000000100000000100
        18000000000000030000C40E0000C40E00000000000000000000C3C3C3C3C3C3
        C3C3C3C3C3C3C3C3C3A46C6C7200009D00009D00007200009E6C6CB5AFAFC3C3
        C3C0C0C0C0C0C0C0C0C0C3C3C3C3C3C3C3C3C3AA5959860C0C6E0000CA00009D
        00009D0000BA0000B90000B20909A24242C3C3C3C0C1C1C0C0C0C3C3C3C3C3C3
        CC4949800000760000B30000501E1E246E6E960000A10000A200009300008E00
        00A92C2CC3C3C3C3C3C3C3C3C37C2C2C9100008A00007F0000C4000051575721
        6363E100008A0000A20000CE00007E0000CD0000A54343C3C3C3C3C3C37C0606
        9E00008E0000D10000BA0000007474315E5ED00000FA0000B90000BF0000C600
        00BF0000B70505BCA8A8804848750000A80000B40000C00000007A7A008A8A38
        6565C70000E30000DB0000BB0000BF0000D200009514145EABAB790000C40000
        9D0000C20000BC0000007C7C008181008888366969F10000E70000B10000C200
        00DD00009B1D1D0088887F00007B00008E0000D9000048484800898900808000
        87871A7474FF0000F50000FF0000E50000FF0000891515008B8B7F00007B0000
        8E0000DD00004C4444008A8A008C8C226F6FFF0000ED0000241CEDFF0000C60C
        0C1C64643B6464008383790000C40000A100009E00005D4C4C156B6BFF0000F1
        00007A2121FF0000241CEDFF0000E11414009898008585007C7C804848750000
        AB00009D0000B00000008181C02424821A1A241CED241CED241CED241CED241C
        ED6E4C4C00787873A1A1C3C3C37C0606AB00001A6F6F176161008686008A8A00
        8B8B008A8AFF0000241CEDDE0000C50707893D3DE20000D6A2A2C3C3C37D2C2C
        A30000177070008787008080008080008686008989FF0000241CEDEC0808CA0A
        0A009393A94646C3C3C3C3C3C3C3C3C3AD1010126E6E00858500848400878778
        47470095959C2424EB05053155552D3D3D41B2B2C3C3C3C3C3C3C3C3C3C3C3C3
        C3C3C37734341372721F56560087879A0A0A5844440B7373C10808B700003A7D
        7DC3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3883939AB0C0C76
        2323A50000DF0000804545C3C3C3C3C3C3C3C3C3C3C3C3C3C3C3}
      ParentShowHint = False
      ShowHint = True
      OnClick = btnGpsClick
    end
    object btnTest: TSpeedButton
      Left = 377
      Top = 0
      Width = 25
      Height = 23
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555555555555
        555555555555555555555555555555555555555555FF55555555555559055555
        55555555577FF5555555555599905555555555557777F5555555555599905555
        555555557777FF5555555559999905555555555777777F555555559999990555
        5555557777777FF5555557990599905555555777757777F55555790555599055
        55557775555777FF5555555555599905555555555557777F5555555555559905
        555555555555777FF5555555555559905555555555555777FF55555555555579
        05555555555555777FF5555555555557905555555555555777FF555555555555
        5990555555555555577755555555555555555555555555555555}
      NumGlyphs = 2
      Visible = False
      OnClick = btnTestClick
    end
  end
  object OpenPictureDialog: TOpenPictureDialog
    Filter = 
      'Bitmaps (*.bmp)|*.bmp|JPEG(*.jpg;*.jpeg;*.jpe;*.jfif)|*.jpg;*.jp' +
      'eg;*.jpe;*.jgif'
    Options = [ofEnableSizing]
    Title = #35831#36873#25321#23631#24149#22270#29255#25991#20214
    Left = 265
    Top = 56
  end
  object timerTxAct: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerTxActTimer
    Left = 232
    Top = 56
  end
  object dlgFont: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = #23435#20307
    Font.Style = []
    MinFontSize = 8
    Options = [fdEffects, fdLimitSize]
    Left = 1000
  end
  object imgListMenu: TImageList
    DrawingStyle = dsTransparent
    ShareImages = True
    Left = 200
    Top = 56
    Bitmap = {
      494C01010800BC00340510001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000003000000001002000000000000030
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF002918FF001000FF000000FF000000000000000000000000000000FF002910
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      FF000000FF000000FF004229FF0000000000000000000000FF001810FF000000
      FF001808FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000008400008484
      8400848484000000000000840000008400000084000000840000008400000000
      0000008400000000000000000000000000000000000000000000000000000000
      FF001008FF000000FF000000FF000000FF00000000000000FF000000FF000000
      FF002918FF000000FF0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000000000000000
      0000000000008484000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF003121FF000000FF001008FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF000000
      0000000000000000000000000000000000008484000000000000000000000000
      00000000000000000000000000008484000000FFFF0000FFFF0000FFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000008400008484
      8400848484000000000000840000008400000084000000840000008400000000
      0000008400000000000000000000000000008484000000840000000000000084
      0000848484008484840000000000008400000084000000840000008400000084
      000000000000008400000000000000000000000000000000FF004A31FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000808
      FF001810FF000800FF003118FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000000000000000
      0000000000008484000084848400848400000084000084840000848400000084
      0000848400000000000000000000000000000084000084840000000000000000
      0000000000000000000084840000848484000000000000000000000000000000
      0000000000000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000008484008484
      8400848484000000000084848400848400008484840000000000848400000000
      0000000000000000000000000000000000000084000084840000000000000000
      0000000000000000000000000000000000008484000000FFFF0000FFFF0000FF
      FF0000FFFF000000000000000000000000000000FF000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF000000
      FF000000FF000000FF001808FF000000FF000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000000000008484
      0000000000008484000000000000848400008484000084840000848484008484
      0000848400000000000084848400000000008484000084840000008400000000
      0000008400008484840084848400000000000084000000840000008400000084
      000000840000000000000084000000000000000000000000FF000000FF000000
      FF000000FF000000FF000000FF000000FF000000FF000000FF002910FF000000
      FF000000FF000000FF000000FF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000848484008484
      8400000000000000000084848400848400008484000000000000000000008484
      0000848484000000000084848400000000000084000000840000848400000000
      0000000000000000000000000000848400008484840084840000008400008484
      0000848400000084000084840000000000000000000000000000000000000000
      0000000000003121FF000000FF000000FF000000FF000000FF000000FF000000
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000000000008484
      0000000000008484000000000000000000008484000000000000848484000000
      0000000000008484000084848400000000000000000000840000848400000000
      0000008484008484840084848400000000008484840084840000848484000000
      0000848400000000000000000000000000000000000000000000000000000000
      00000000FF000000FF000000FF003921FF000000FF000000FF000000FF003118
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000848484008484
      8400000000000000000084848400848484008484000000000000000000000000
      0000000000008484000084848400000000000000000084840000848400000000
      0000000000008484000000000000848400008484840084840000848400008484
      0000848484008484000084840000000000000000000000000000000000000000
      FF000000FF000000FF000800FF000000FF00000000001810FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000084848400848400008484
      0000848484008484000084848400848484008484000084848400848400008484
      0000848484000000000000000000000000000000000000840000008400008484
      0000848484008484840084840000000000008484840084840000848400000000
      0000000000008484000084848400000000000000000000000000000000000000
      FF000000FF000000FF000000FF0000000000000000000000FF000000FF000000
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000000
      0000000000008484000000000000848400000000000000000000848400000000
      0000848484000000000000000000848400000000000000000000000000000000
      00000000FF005A39FF000000FF0000000000000000000000FF005A39FF004229
      FF000000FF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400000000
      0000848484008484840000000000000000008484840084848400848400000000
      0000000000000000000000000000848400000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000FFFF0000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      00000000000000000000000000000000000000000000FF000000008484000084
      8400FF000000FF000000FF000000FF000000FF000000FF000000000000000000
      0000FF00000000848400FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000FFFF000084840000FFFF0000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF000000FF0000000084
      8400008484000084840000848400008484000084840000848400008484000084
      8400FF00000000000000000000000000000000000000FF000000008484000084
      8400FF000000FF000000FF000000FF000000FF000000FF000000000000000000
      0000FF00000000848400FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000084840000FFFF000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF000000FFFFFF00FF00
      0000008484000084840000848400008484000084840000848400008484000084
      840000848400FF000000000000000000000000000000FF000000008484000084
      8400FF000000FF000000FF000000FF000000FF000000FF000000000000000000
      0000FF00000000848400FF000000000000000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000FFFF000084
      840000FFFF00000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF00000000FFFF00FFFF
      FF00FF0000000084840000848400008484000084840000848400008484000084
      84000084840000848400FF0000000000000000000000FF000000008484000084
      8400FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF00000000848400FF000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000FFFF000084840000FF
      FF0000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF000000FFFFFF0000FF
      FF00FFFFFF00FF00000000848400008484000084840000848400008484000084
      8400008484000084840000848400FF00000000000000FF000000008484000084
      8400008484000084840000848400008484000084840000848400008484000084
      84000084840000848400FF000000000000000000000084848400000000000000
      000000000000000000000000000000000000000000000084840000FFFF000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF00000000FFFF00FFFF
      FF0000FFFF00FFFFFF00FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF00000000000000FF000000008484000084
      8400FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      00000084840000848400FF00000000000000848484000000000000FFFF0000FF
      FF0000FFFF00000000000000000000FFFF000084840000FFFF00000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF000000FFFFFF0000FF
      FF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF00FF00
      00000000000000000000000000000000000000000000FF00000000848400FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000848400FF000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF000084840000FFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF00000000FFFF00FFFF
      FF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FFFFFF0000FFFF00FF00
      00000000000000000000000000000000000000000000FF00000000848400FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000848400FF000000000000000000000000FFFF0000FFFF0000FF
      FF0000FFFF0000FFFF0000FFFF0000FFFF000000000000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFF
      FF00FFFFFF00FF000000000000000000000000000000FF000000FFFFFF0000FF
      FF00FFFFFF00FF000000FF000000FF000000FF000000FF000000FF000000FF00
      00000000000000000000000000000000000000000000FF00000000848400FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000848400FF000000000000000000000000FFFF0000FFFF000000
      0000000000000084840000FFFF0000FFFF000000000084848400000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FF00
      0000FF000000FF00000000000000000000000000000000000000FF000000FF00
      0000FF0000000000000000000000000000000000000000000000000000000000
      000000000000FF000000FF000000FF00000000000000FF00000000848400FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000848400FF00000000000000848484008484840000FFFF000000
      0000000000000084840000FFFF0000FFFF0000FFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FFFF
      FF00FF0000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000FF000000FF00000000000000FF00000000848400FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF000000FF000000FF00000000000000000000000000000000FFFF0000FF
      FF00008484000084840000FFFF0000FFFF0000FFFF0000000000000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FFFFFF00FF000000FF00
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000000000000000FF000000000000000000
      000000000000FF00000000000000FF00000000000000FF00000000848400FF00
      0000000000000000000000000000000000000000000000000000000000000000
      0000FF00000000000000FF000000000000000000000000000000000000008484
      840000FFFF0000FFFF0000FFFF0000FFFF000000000084848400000000000000
      000000000000000000000000000000000000000000000000000000000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF0000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000FF000000FF00
      0000FF00000000000000000000000000000000000000FF000000FF000000FF00
      0000FF000000FF000000FF000000FF000000FF000000FF000000FF000000FF00
      0000FF000000FF000000FF000000000000000000000000000000000000008484
      8400000000000000000000000000000000008484840000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000300000000100010000000000800100000000000000000000
      000000000000000000000000FFFFFF0000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      00000000000000000000000000000000FFFFFFFFFFFFFFFFDFFFFFFFFC0FE1C7
      CFFFFFFF0203E183C7FFFFFF4003E083C3FFFC0F0003F007C1FF02030001F00F
      C0FF400300018000C07F000300010000C0FF400100000000C1FF020100008001
      C3FF48010000F80FC7FF03498000F00FCFFF48698000E087DFFF00038000E187
      FFFFFFFFC0D2F187FFFFFFFFD21AFFFFFFFFFFFFFFFFFFF8FFFFFFFFFFFFFFF8
      FFFFFFFFC001FFE0E003800F8031FFE0E00380078031FF81E00380038031FF03
      E00380018001FF07E00380008001840FE00380008001001FE003800F8FF1003F
      E003800F8FF1007FE003800F8FF1003FE003C7F88FF1003FE007FFFC8FF1803F
      E00FFFBA8FF5C03FE01FFFC78001E07F00000000000000000000000000000000
      000000000000}
  end
  object mnuMain: TMainMenu
    Left = 336
    Top = 56
    object mnuSystem: TMenuItem
      Caption = #31995#32479'(&S)'
      object mnuCardManage: TMenuItem
        Bitmap.Data = {
          26050000424D26050000000000003604000028000000100000000F0000000100
          080000000000F000000000000000000000000001000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
          A6000020400000206000002080000020A0000020C0000020E000004000000040
          20000040400000406000004080000040A0000040C0000040E000006000000060
          20000060400000606000006080000060A0000060C0000060E000008000000080
          20000080400000806000008080000080A0000080C0000080E00000A0000000A0
          200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
          200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
          200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
          20004000400040006000400080004000A0004000C0004000E000402000004020
          20004020400040206000402080004020A0004020C0004020E000404000004040
          20004040400040406000404080004040A0004040C0004040E000406000004060
          20004060400040606000406080004060A0004060C0004060E000408000004080
          20004080400040806000408080004080A0004080C0004080E00040A0000040A0
          200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
          200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
          200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
          20008000400080006000800080008000A0008000C0008000E000802000008020
          20008020400080206000802080008020A0008020C0008020E000804000008040
          20008040400080406000804080008040A0008040C0008040E000806000008060
          20008060400080606000806080008060A0008060C0008060E000808000008080
          20008080400080806000808080008080A0008080C0008080E00080A0000080A0
          200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
          200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
          200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
          2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
          2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
          2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
          2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
          2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
          2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
          2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00070707070707
          0000000000000707070700000000000007FBFBFBFB0000000707020702A4A400
          02020202020002000707060000000006A4000000000000000707060000000000
          0006FBFBFBFB0000000706020002A4A400020202020200020007020600000000
          06A40000000000000007020600000000000006FBFBFBFB0000000606020002A4
          A40002020202020002000202060000000006A4060206060206000702060003A4
          A400A406A400060000000706060000060006A4060606A406060007020206A4A4
          0600A40606000006A400070702000006000607070607A400070607070607A4A4
          0700A4A4060707000706}
        Caption = #25511#21046#21345#31649#29702'(&R)...'
        OnClick = mnuCardListClick
      end
      object mnuCommConfig: TMenuItem
        Bitmap.Data = {
          36050000424D3605000000000000360400002800000010000000100000000100
          0800000000000001000000000000000000000001000000010000FF00FF000062
          00000065000000680000006E0000036B0400046E08000276030004760700007D
          0000037A0500047B050006720A0009760E0009780E0000605300006757000063
          5F00026C5B00005F7800006375000184010007800C00088C0D0000970000059F
          07000C8513000D88190011841300108019001296110012892100148E28001D93
          2E001B9A2D001A9E3500249D380018A32E0027B84C0036BA510033C34E002EC6
          5C0036CF540044DD65004BE47E000066CE000268D000056BD300076DD300076D
          D4000B6DD7000A70D8000D73DB000E74DB001177DE00197FDE001277E600187E
          E000197EEA001C82E9002288E0002286EB002186EE00278CEC00298FEB00288E
          EE002A90EE002F95ED003499FE00379CFF003BA0EE003FA5FA003DA2FF0045AB
          FF004AAFFF0058BDFF0000CCFF0011EEED005DC3FF0000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          000000000000000033000000000000000000000000003137313100004D4C0000
          000000000031434A31000000004D4C0000313131313F472E0000000000000000
          32323F4B4B3F3100000000000000001338473C4649452E000000000000001C22
          14414B393E452E0000000000000C1F2C27113E45363B310000000000000E2924
          2B1E0F3A4533310000000000000E29231A19150F3831000000000000000E2625
          160A190A1200000000000000001F1B17150A0404004D4C000000000021280802
          0204020000004D4C0000001D2A17020000000000000000000000040608020000
          0000000000000000000020040000000000000000000000000000}
        Caption = #36890#20449#37197#32622'(&C)...'
        ImageIndex = 0
        OnClick = mnuCommConfigClick
      end
      object mnuCardConfig: TMenuItem
        Bitmap.Data = {
          06030000424D06030000000000003600000028000000100000000F0000000100
          180000000000D002000000000000000000000000000000000000F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767DDDDDDF0F0
          F0DDDDDD676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F0F0F0676767AEAEAEF0F0F0F0F0F0F0F0F0AEAEAE676767F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767AEAEAEDDDDDDF0F0
          F0DDDDDDAEAEAE676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0676767AEAEAEDDDDDDAEAEAE676767F0F0F0BDAC9B5E4505
          5E4505BDAC9BF0F0F0F0F0F0BDAC9B5E45055E4505BDAC9BF0F0F0676767AEAE
          AE676767F0F0F0F0F0F05D4404BC9122B984005E4505F0F0F0F0F0F05E4505B9
          8400BC9122674D0CF0F0F0676767DDDDDD676767F0F0F0F0F0F05D440495731F
          6F4E005E4505F0F0F0F0F0F05E45056F4E0095731F684E0BF0F0F0676767DDDD
          DD676767F0F0F0F0F0F05D440495731F6F4E005E45055E45055E45055E45056F
          4E0095731F684E0BF0F0F0676767AEAEAE676767F0F0F0F0F0F05D440495731F
          6F4E005E4505BC9122BC91225E45056F4E0095731F684E0BF0F0F0676767DDDD
          DD676767F0F0F0F0F0F05D440495731F6F4E005E45055E45055E45055E45056F
          4E0095731F694F0CF0F0F0676767DDDDDD676767F0F0F0F0F0F0F0F0F05E4505
          5E45055E4505F0F0F0F0F0F05E45055E45055E4505F0F0F0F0F0F0676767AEAE
          AE676767F0F0F0F0F0F0F0F0F05E4505C99B6D5E4505F0F0F0F0F0F05E4505B6
          8B5B5E4505F0F0F0676767AEAEAEDDDDDDAEAEAE676767F0F0F0F0F0F0BDAC9B
          5E4505BDAC9BF0F0F0F0F0F0BDAC9B5E4505BDAC9B676767AEAEAEDDDDDDF0F0
          F0DDDDDDAEAEAE676767F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F0F0F0676767AEAEAEF0F0F0F0F0F0F0F0F0AEAEAE676767F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0676767DDDDDDF0F0
          F0DDDDDD676767F0F0F0}
        Caption = #25511#21046#21345#37197#32622'(&S)...'
        ImageIndex = 0
        OnClick = mnuCardConfigClick
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object mnuOpen: TMenuItem
        Bitmap.Data = {
          EE000000424DEE000000000000007600000028000000100000000F0000000100
          0400000000007800000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          888888888888888888888CCCCCCCCCCC88888CC333333333C8888CFC33333333
          3C888CBFC333333333C88CFBFC333333333C8CBFBFCCCCCCCCCC8CFBFBFBFBFC
          88888CBFBFBFBFBC88888CFBFCCCCCCC888888CCC88888888CCC888888888888
          88CC888888888C888C8C8888888888CCC888}
        Caption = #23548#20837#31995#32479#37197#32622#21644#25152#26377#33410#30446#20869#23481'(&O)...'
        ImageIndex = 0
        OnClick = mnuOpenClick
      end
      object mnuImportV3: TMenuItem
        Caption = #23548#20837'GS7020BManV3.X'#20013#30340#25511#21046#21345#23450#20041#21450#33410#30446#20869#23481'(&I)...'
        OnClick = mnuImportV3Click
      end
      object mnuSave: TMenuItem
        Bitmap.Data = {
          EE000000424DEE000000000000007600000028000000100000000F0000000100
          0400000000007800000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          888888CCCCCCCCCCCCC88C33CCCCCC88C3C88C33CCCCCC88C3C88C33CCCCCC88
          C3C88C33CCCCCCCCC3C88C333333333333C88C33CCCCCCCC33C88C3C88888888
          C3C88C3C88888888C3C88C3C88888888C3C88C3C88888888C3C88C3C88888888
          CCC88C3C88888888C8C88CCCCCCCCCCCCCC8}
        Caption = #23548#20986#31995#32479#37197#32622#21644#25152#26377#33410#30446#20869#23481'(&A)...'
        ImageIndex = 0
        OnClick = mnuSaveClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object mnuAdminPassword: TMenuItem
        Caption = #31649#29702#21592#23494#30721'(&P)...'
        ImageIndex = 3
        OnClick = mnuAdminPasswordClick
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object mnuRootPath: TMenuItem
        Caption = #35813#36719#20214#31649#29702#30340#25152#26377#26174#31034#23631#26681#33410#28857#21517#31216'(&N)...'
        OnClick = mnuRootPathClick
      end
      object mnuCardType: TMenuItem
        Caption = #25511#21046#21345#22411#21495#36873#25321'(&T)...'
        OnClick = mnuCardTypeClick
      end
      object mnuUpdateProg: TMenuItem
        Caption = #25511#21046#21345#36719#20214#21319#32423'(&U)'
        OnClick = mnuUpdateProgClick
      end
    end
    object mnuSelect: TMenuItem
      Caption = #36873#25321'(&L)'
      object mnuSelectAllAct: TMenuItem
        Bitmap.Data = {
          36050000424D3605000000000000360400002800000010000000100000000100
          0800000000000001000000000000000000000001000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
          A6000020400000206000002080000020A0000020C0000020E000004000000040
          20000040400000406000004080000040A0000040C0000040E000006000000060
          20000060400000606000006080000060A0000060C0000060E000008000000080
          20000080400000806000008080000080A0000080C0000080E00000A0000000A0
          200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
          200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
          200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
          20004000400040006000400080004000A0004000C0004000E000402000004020
          20004020400040206000402080004020A0004020C0004020E000404000004040
          20004040400040406000404080004040A0004040C0004040E000406000004060
          20004060400040606000406080004060A0004060C0004060E000408000004080
          20004080400040806000408080004080A0004080C0004080E00040A0000040A0
          200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
          200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
          200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
          20008000400080006000800080008000A0008000C0008000E000802000008020
          20008020400080206000802080008020A0008020C0008020E000804000008040
          20008040400080406000804080008040A0008040C0008040E000806000008060
          20008060400080606000806080008060A0008060C0008060E000808000008080
          20008080400080806000808080008080A0008080C0008080E00080A0000080A0
          200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
          200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
          200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
          2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
          2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
          2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
          2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
          2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
          2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
          2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
          A4FCFCFCFCFCFCFCFCFCA407F6EDF6F6A4D2FFFFFFFFFFFFFFFCA4079A9AFFF6
          A4D2FFFFFFFFFFFFFFFCA407ED07E308A4D2FF4F4F4F4F4FFFFCA4F507F69AF6
          A4D2FFFFFFFFFFFFFFFCA4F5F5F5ED08A4D2FFFFFFFFFFFFFFFCA4A4A4A4A4A4
          A4D2FF4F4F4F4F4FFFFCA4A4A4A4A4A4A4D2FFFFFFFFFFFFFFFCA407F6EDF6F6
          A4D2FFFFFFFFFFFFFFFCA4079A9AFFF6A4D2FFFFFFFFFCFCFCFCA407ED07E308
          A4D2FFFFFFFFFCFFFCFFA4F507F69AF6A4D2FFFFFFFFFCFCFFFFA4F5F5F5ED08
          A4D2FCFCFCFCFCFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFF}
        Caption = #36873#25321#20840#37096#33410#30446'(&S)'
        ShortCut = 16467
        OnClick = mnuSelectAllActClick
      end
      object mnuCancelAllAct: TMenuItem
        Bitmap.Data = {
          36050000424D3605000000000000360400002800000010000000100000000100
          0800000000000001000000000000000000000001000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
          A6000020400000206000002080000020A0000020C0000020E000004000000040
          20000040400000406000004080000040A0000040C0000040E000006000000060
          20000060400000606000006080000060A0000060C0000060E000008000000080
          20000080400000806000008080000080A0000080C0000080E00000A0000000A0
          200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
          200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
          200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
          20004000400040006000400080004000A0004000C0004000E000402000004020
          20004020400040206000402080004020A0004020C0004020E000404000004040
          20004040400040406000404080004040A0004040C0004040E000406000004060
          20004060400040606000406080004060A0004060C0004060E000408000004080
          20004080400040806000408080004080A0004080C0004080E00040A0000040A0
          200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
          200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
          200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
          20008000400080006000800080008000A0008000C0008000E000802000008020
          20008020400080206000802080008020A0008020C0008020E000804000008040
          20008040400080406000804080008040A0008040C0008040E000806000008060
          20008060400080606000806080008060A0008060C0008060E000808000008080
          20008080400080806000808080008080A0008080C0008080E00080A0000080A0
          200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
          200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
          200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
          2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
          2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
          2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
          2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
          2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
          2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
          2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
          A4FCFCFCFCFCFCFCFCFCA4FFFFFFF6F6A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFF6
          A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFFFA4D2FF4F4F4F4F4FFFFCA4FFFFFFFFF6
          A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFFFA4D2FFFFFFFFFFFFFFFCA4A4A4A4A4A4
          A4D2FF4F4F4F4F4FFFFCA4A4A4A4A4A4A4D2FFFFFFFFFFFFFFFCA4FFF6FFF6F6
          A4D2FFFFFFFFFFFFFFFCA4FFFFFFFFF6A4D2FFFFFFFFFCFCFCFCA4FFFFFFFFFF
          A4D2FFFFFFFFFCFFFCFFA4FFFFF6FFF6A4D2FFFFFFFFFCFCFFFFA4FFFFFFFFFF
          A4D2FCFCFCFCFCFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFF}
        Caption = #21462#28040#36873#25321#25152#26377#33410#30446'(&C)'
        ShortCut = 16459
        OnClick = mnuCancelAllActClick
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object mnuSelectAllCard: TMenuItem
        Bitmap.Data = {
          26050000424D26050000000000003604000028000000100000000F0000000100
          080000000000F000000000000000000000000001000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
          A6000020400000206000002080000020A0000020C0000020E000004000000040
          20000040400000406000004080000040A0000040C0000040E000006000000060
          20000060400000606000006080000060A0000060C0000060E000008000000080
          20000080400000806000008080000080A0000080C0000080E00000A0000000A0
          200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
          200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
          200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
          20004000400040006000400080004000A0004000C0004000E000402000004020
          20004020400040206000402080004020A0004020C0004020E000404000004040
          20004040400040406000404080004040A0004040C0004040E000406000004060
          20004060400040606000406080004060A0004060C0004060E000408000004080
          20004080400040806000408080004080A0004080C0004080E00040A0000040A0
          200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
          200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
          200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
          20008000400080006000800080008000A0008000C0008000E000802000008020
          20008020400080206000802080008020A0008020C0008020E000804000008040
          20008040400080406000804080008040A0008040C0008040E000806000008060
          20008060400080606000806080008060A0008060C0008060E000808000008080
          20008080400080806000808080008080A0008080C0008080E00080A0000080A0
          200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
          200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
          200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
          2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
          2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
          2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
          2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
          2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
          2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
          2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFA407F6EDF6F6
          A4FFFFFFFFFFFFFFFFFFA4079A9AFFF6A4000000000000FFFFFFA407ED07E308
          A407FBFBFBFB000000FFA4F507F69AF6A40202020202000200FFA4F5F5F5ED08
          A4A406020606020600FFA4A4A4A4A4A4A4A406A4000600000000A4A4A4A4A4A4
          A407060606A4060600A4A407F6EDF6F6A4A40606000006A400A4A4079A9AFFF6
          A407070607A4000706A4A407ED07E308A4A4A4060707000706A4A4F507F69AF6
          A4A4A406A40606A400FFA4F5F5F5ED08A4FFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
          A4FFFFFFFFFFFFFFFFFF}
        Caption = #36873#25321#25152#26377#26174#31034#23631'('#25511#21046#21345')(&R)'
        ShortCut = 16466
        OnClick = mnuSelectAllCardClick
      end
      object mnuCancelAllCard: TMenuItem
        Bitmap.Data = {
          26050000424D26050000000000003604000028000000100000000F0000000100
          080000000000F000000000000000000000000001000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
          A6000020400000206000002080000020A0000020C0000020E000004000000040
          20000040400000406000004080000040A0000040C0000040E000006000000060
          20000060400000606000006080000060A0000060C0000060E000008000000080
          20000080400000806000008080000080A0000080C0000080E00000A0000000A0
          200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
          200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
          200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
          20004000400040006000400080004000A0004000C0004000E000402000004020
          20004020400040206000402080004020A0004020C0004020E000404000004040
          20004040400040406000404080004040A0004040C0004040E000406000004060
          20004060400040606000406080004060A0004060C0004060E000408000004080
          20004080400040806000408080004080A0004080C0004080E00040A0000040A0
          200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
          200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
          200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
          20008000400080006000800080008000A0008000C0008000E000802000008020
          20008020400080206000802080008020A0008020C0008020E000804000008040
          20008040400080406000804080008040A0008040C0008040E000806000008060
          20008060400080606000806080008060A0008060C0008060E000808000008080
          20008080400080806000808080008080A0008080C0008080E00080A0000080A0
          200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
          200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
          200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
          2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
          2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
          2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
          2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
          2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
          2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
          2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFA4A4A4A4A4A4A4FFFFFFFFFFFFFFFFFFA4FFFFFFF6F6
          A4FFFFFFFFFFFFFFFFFFA4FFFFFFFFF6A4000000000000FFFFFFA4FFFFFFFFFF
          A407FBFBFBFB000000FFA4FFFFFFFFF6A40202020202000200FFA4FFFFFFFFFF
          A4A406020606020600FFA4A4A4A4A4A4A4A406A4000600000000A4A4A4A4A4A4
          A407060606A4060600A4A4FFF6FFF6F6A4A40606000006A400A4A4FFFFFFFFF6
          A407070607A4000706A4A4FFFFFFFFFFA4A4A4060707000706A4A4FFFFF6FFF6
          A4A4A406A40606A400FFA4FFFFFFFFFFA4FFFFFFFFFFFFFFFFFFA4A4A4A4A4A4
          A4FFFFFFFFFFFFFFFFFF}
        Caption = #21462#28040#36873#25321#25152#26377#26174#31034#23631'('#25511#21046#21345')(&E)'
        ShortCut = 16453
        OnClick = mnuCancelAllCardClick
      end
      object N11: TMenuItem
        Caption = '-'
      end
      object mnuSelectAllSuccessed: TMenuItem
        Caption = #36873#25321#25152#26377#19978#27425#36890#20449#25104#21151#30340#26174#31034#23631'('#25511#21046#21345')(&U)'
        OnClick = mnuSelectAllSuccessedClick
      end
      object mnuSelectAllFailed: TMenuItem
        Caption = #36873#25321#25152#26377#19978#27425#36890#20449#22833#36133#30340#26174#31034#23631'('#25511#21046#21345')(&F)'
        OnClick = mnuSelectAllFailedClick
      end
      object N13: TMenuItem
        Caption = '-'
      end
      object mnuShrink: TMenuItem
        Caption = #25910#32553#25152#26377#26174#31034#23631#20998#32452#26174#31034'(&K)'
        OnClick = mnuShrinkClick
      end
    end
    object mnuEdit: TMenuItem
      Caption = #32534#36753'(&E)'
      OnClick = mnuEditClick
      object mnuPreviewCurrentAct: TMenuItem
        Caption = #39044#35272#24403#21069#32534#36753#30340#33410#30446'(&P)'
        ShortCut = 16464
        OnClick = mnuPreviewCurrentActClick
      end
      object mnuPreviewAllAct: TMenuItem
        Bitmap.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000EBD9C5EAD8C4
          EBD8C5ECD9C5ECD9C5EBD9C5EBD9C5ECD9C5ECD9C5ECD9C5ECD9C5ECD9C5B1AF
          AB203040ACA7A1ECD9C5EAD8C4B0A09060483060483060483060483060483060
          48306048306048306048305048403050604078C0304860B1AFA6EBD9C5B0A090
          FFFFFFB0A090B0A090B0A090B0A090B0A090B0A090B0A0909088803050703080
          D04098E050B0F0506870EBD9C5B0A090FFFFFFFFFFFFFFF8FFF0F0F0D0D8D090
          989070686060686050586040709040A0E060C8FF7090A0C5C0B5EBD9C5B0A090
          FFFFFFFFFFFFFFFFFFE0E0E0909090B0A8A0D0C0B0D0B0A08078705058506090
          B07098B0AEB1ABECD9C6ECD9C5B0A090FFFFFFFFFFFFFFFFFFB0B0B0C0B8B0FF
          F0E0FFE8E0F0D8C0F0D0B08078709C908CAEB1ABECD9C6ECD9C6EBD9C5C0A890
          FFFFFFFFFFFFFFFFFFA09890F0E8E0FFF8F0FFF0F0FFE8E0F0D8D0D0B0A06468
          5FECDAC6ECDAC6ECD9C6ECD9C5C0A8A0FFFFFFFFFFFFFFFFFFA0A090F0E8E0FF
          FFFFFFF8F0FFF0F0FFE8E0E0C0B0726F6DECDAC6ECDAC6ECDAC6ECD9C5C0B0A0
          FFFFFFFFFFFFFFFFFFC0C8C0C0C0C0FFFFFFFFFFFFFFF8F0FFF0E0B0A090A59C
          93ECDAC6ECDAC6ECDAC6ECDAC7D0B0A0FFFFFFFFFFFFFFFFFFF0F8FFC0B8B0C0
          C0C0F0E8E0F0E8E0B0B0A0707060E5D3BFECDAC6ECDAC6ECDAC6ECDAC7D0B8A0
          FFFFFFFFFFFFFFFFFFFFFFFFF0F8FFC0C8C0A0A090909080909090605040ECDA
          C6ECDAC6ECDAC6ECDAC6ECDAC7D0B8B0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFB0A090604830604830604830ECDAC7ECDAC7ECDAC6ECDAC6ECDAC7D0C0B0
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0A890D0C8C0604830DCC1B0ECDA
          C7ECDAC7ECDAC7ECDAC7ECDAC8E0C0B0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFC0A8A0604830DCC1B0ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC8E0C0B0
          E0C0B0E0C0B0E0C0B0E0C0B0D0C0B0D0B8B0D0B0A0DCC2B0ECDAC7ECDAC7ECDA
          C7ECDAC7ECDAC7ECDAC7ECDBC8ECDBC8ECDAC8ECDAC8ECDAC8ECDAC8ECDAC7EC
          DAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7ECDAC7}
        Caption = #39044#35272#25152#26377#33410#30446'(&V)'
        OnClick = mnuPreviewAllActClick
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object mnuSearchCard: TMenuItem
        Bitmap.Data = {
          36050000424D3605000000000000360400002800000010000000100000000100
          0800000000000001000000000000000000000001000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000C0DCC000F0CA
          A6000020400000206000002080000020A0000020C0000020E000004000000040
          20000040400000406000004080000040A0000040C0000040E000006000000060
          20000060400000606000006080000060A0000060C0000060E000008000000080
          20000080400000806000008080000080A0000080C0000080E00000A0000000A0
          200000A0400000A0600000A0800000A0A00000A0C00000A0E00000C0000000C0
          200000C0400000C0600000C0800000C0A00000C0C00000C0E00000E0000000E0
          200000E0400000E0600000E0800000E0A00000E0C00000E0E000400000004000
          20004000400040006000400080004000A0004000C0004000E000402000004020
          20004020400040206000402080004020A0004020C0004020E000404000004040
          20004040400040406000404080004040A0004040C0004040E000406000004060
          20004060400040606000406080004060A0004060C0004060E000408000004080
          20004080400040806000408080004080A0004080C0004080E00040A0000040A0
          200040A0400040A0600040A0800040A0A00040A0C00040A0E00040C0000040C0
          200040C0400040C0600040C0800040C0A00040C0C00040C0E00040E0000040E0
          200040E0400040E0600040E0800040E0A00040E0C00040E0E000800000008000
          20008000400080006000800080008000A0008000C0008000E000802000008020
          20008020400080206000802080008020A0008020C0008020E000804000008040
          20008040400080406000804080008040A0008040C0008040E000806000008060
          20008060400080606000806080008060A0008060C0008060E000808000008080
          20008080400080806000808080008080A0008080C0008080E00080A0000080A0
          200080A0400080A0600080A0800080A0A00080A0C00080A0E00080C0000080C0
          200080C0400080C0600080C0800080C0A00080C0C00080C0E00080E0000080E0
          200080E0400080E0600080E0800080E0A00080E0C00080E0E000C0000000C000
          2000C0004000C0006000C0008000C000A000C000C000C000E000C0200000C020
          2000C0204000C0206000C0208000C020A000C020C000C020E000C0400000C040
          2000C0404000C0406000C0408000C040A000C040C000C040E000C0600000C060
          2000C0604000C0606000C0608000C060A000C060C000C060E000C0800000C080
          2000C0804000C0806000C0808000C080A000C080C000C080E000C0A00000C0A0
          2000C0A04000C0A06000C0A08000C0A0A000C0A0C000C0A0E000C0C00000C0C0
          2000C0C04000C0C06000C0C08000C0C0A000F0FBFF00A4A0A000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFF5050505050FFFFFFFFFF5050505050FF50F4989850
          FFFFFFFFFF50F4989850FF50EC989850FFFFFFFFFF50EC989850FF5098E89850
          5050FF50505098989850FF5098F4989850E85050F49898989850FF5098F49898
          50F45050F49898989850FFFF50E8989850E85050EC98985050FFFFFFFF50F498
          5050FF50F4989850FFFFFFFFFF50E8985050FF50E8989850FFFFFFFFFFFF5050
          50FFFFFF505050FFFFFFFFFFFFFF50EC50FFFFFF50EC50FFFFFFFFFFFFFF5050
          50FFFFFF505050FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        Caption = #26597#25214#26174#31034#23631'(&C)...'
        ShortCut = 16454
        OnClick = mnuSearchCardClick
      end
      object mnuSearchAct: TMenuItem
        Bitmap.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000F0FBFFF0FBFF
          F0FBFFF0FBFF404000404000404000404000F0FBFFF0FBFFF0FBFF4040004040
          00404000404000FFFFFFF0FBFFF0FBFFF0FBFFF0FBFF404000808020C0A00040
          4000F0FBFFF0FBFFF0FBFF404000C0A000808020404000FFFFFFF0FBFFF0FBFF
          F0FBFFF0FBFF404000808020806000404000F0FBFFF0FBFFF0FBFF4040008060
          00808020404000FFFFFFF0FBFFF0FBFFF0FBFFF0FBFF40400080802080600040
          4000808020808020808020404000806000808020404000FFFFFFF0FBFFF0FBFF
          F0FBFFF0FBFF4040008080208060004040004040004040004040004040008060
          00808020404000FFFFFFF0FBFFF0FBFFF0FBFFF0FBFF40400080802080600040
          4000C0A000808020C0A000404000806000808020404000FFFFFFF0FBFFA4A0A0
          4040204040204040008080208060004040004040004040004040004040008060
          00808020404000FFFFFFF0FBFF40402080E0E080E0E040400080802080600040
          4000808020808020808020404000806000808020404000FFFFFFF0FBFF404020
          80E0E0A4A0A080808040400040400040400080E0E0404020F0FBFF4040004040
          00404000F0FBFFFFFFFFF0FBFF40402080E0E080E0E080E0E0404000C0A08040
          400080E0E0404020F0FBFF404000C0A080404000F0FBFFFFFFFFF0FBFF404020
          80E0E0A4A0A080808040400040400040400080E0E0404020F0FBFF4040004040
          00404000F0FBFFFFFFFFF0FBFF40402080E0E080E0E080E0E080E0E080E0E080
          E0E080E0E0404020F0FBFFF0FBFFF0FBFFF0FBFFF0FBFFFFFFFFF0FBFF404020
          80E0E0A4A0A080808080E0E0A4A0A0404020404020404020F0FBFFF0FBFFF0FB
          FFF0FBFFF0FBFFFFFFFFF0FBFF40402080E0E080E0E080E0E080E0E040402080
          E0E080E0E0404020F0FBFFF0FBFFF0FBFFF0FBFFF0FBFFFFFFFFF0FBFF404020
          80E0E080E0E080E0E080E0E040402080E0E0404020F0FBFFF0FBFFF0FBFFF0FB
          FFF0FBFFF0FBFFFFFFFFF0FBFFA4A0A040402040402040402040402040402040
          4020F0FBFFF0FBFFF0FBFFF0FBFFF0FBFFF0FBFFF0FBFFFFFFFF}
        Caption = #26597#25214#33410#30446'(&A)...'
        ShortCut = 16449
        OnClick = mnuSearchActClick
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object mnuSetTextActive: TMenuItem
        Caption = #23558#25152#36873#25991#23383#35774#23450#20026#21160#24577#21464#21270#39033'(&E)... '
        OnClick = mnuSetTextActiveClick
      end
      object mnuSetTextNormal: TMenuItem
        Caption = #24674#22797#25152#36873#25991#23383#20026#22266#23450#26174#31034#20869#23481'(&M)'
        OnClick = mnuSetTextNormalClick
      end
      object mnuCircleClock: TMenuItem
        Caption = #21019#24314#19968#20010#27169#25311#26102#38047#33410#30446'(&K)'
        Visible = False
        OnClick = mnuCircleClockClick
      end
      object N8: TMenuItem
        Caption = '-'
      end
      object mnuDeleteAct: TMenuItem
        Bitmap.Data = {
          EE000000424DEE000000000000007600000028000000100000000F0000000100
          0400000000007800000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888888888888
          88888888888888888888888CCCCCCCCCCC88888CFFFFFFFFFC88888CFFFFFFFF
          FC88888CFFFFFFFFFC88888CFFFFFFFFFC88888CFFFFFFFFFC88888CFFFFFFFF
          FC88888CFFFFFFFFFC88888CFFFFFFFFFC88888CFFFFFFCCCC88888CFFFFFFCF
          C888888CFFFFFFCC8888888CCCCCCCC88888}
        Caption = #21024#38500#24050#36873#25321#30340#33410#30446#30340#20840#37096#20869#23481'(&D)'
        OnClick = mnuDeleteActClick
      end
      object mnuDeleteText: TMenuItem
        Caption = #21024#38500#24050#36873#25321#30340#33410#30446#30340#25991#23383#20869#23481'(&T)'
        OnClick = mnuDeleteTextClick
      end
      object mnuDeletePicture: TMenuItem
        Caption = #21024#38500#24050#36873#25321#30340#33410#30446#30340#22270#29255#20869#23481'(&R)'
        OnClick = mnuDeletePictureClick
      end
      object N7: TMenuItem
        Caption = '-'
      end
      object mnuSaveActFile: TMenuItem
        Caption = #23548#20986#24403#21069#33410#30446'(&O)...'
        OnClick = mnuSaveActFileClick
      end
      object mnuOpenActFile: TMenuItem
        Caption = #20174#33410#30446#25991#20214#23548#20837#21040#24403#21069#33410#30446'(&I)...'
        OnClick = mnuOpenActFileClick
      end
      object N14: TMenuItem
        Caption = '-'
      end
      object mnuActDefine: TMenuItem
        Caption = #33410#30446#36866#29992#33539#22260#23450#20041'(&A)...'
        OnClick = mnuActDefineClick
      end
    end
    object mnuCommunication: TMenuItem
      Caption = #36890#20449'(&C)'
      object mnuSendAct: TMenuItem
        Bitmap.Data = {
          EE000000424DEE000000000000007600000028000000100000000F0000000100
          0400000000007800000000000000000000001000000000000000000000000000
          8000008000000080800080000000800080008080000080808000C0C0C0000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00888882F22F28
          888888882F2FF2F288888882FF2FF2F28888882FFFF2FF2F288882FFFFF2FF2F
          288886FFFCFF2FF2F28882FFFCCF2FF2F288CCCCCCCCF2FF2F28CCCCCCCCF2FF
          2F288882FCCFFF2FF2F288882CFFFF2FF2F288882FFFF2FF2F28888882FF2FF2
          F288888882F2FF2F28888888882FF2F28888}
        Caption = #21457#36865#33410#30446#20869#23481'(&T)'
        ShortCut = 116
        OnClick = mnuSendActClick
      end
      object mnuManualReTxAct: TMenuItem
        Caption = #37325#26032#21457#36865#19978#27425#26410#25104#21151#21457#36865#30340#33410#30446#20869#23481'(&R)'
        Enabled = False
        OnClick = mnuManualReTxActClick
      end
      object mnuViewFailedTxAct: TMenuItem
        Caption = #26597#30475#25152#36873#26174#31034#23631#26410#25104#21151#21457#36865#30340#33410#30446'(&V)'
        Visible = False
        OnClick = mnuViewFailedTxActClick
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object mnuCheckCardOnline: TMenuItem
        Bitmap.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F000000000
          0000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0000000B0DCFBABD6F595BFDE000000F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000A3
          CFEDB4E0FFA3CFEE000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F000000000000000000000000094BFDEAAD6F4B0DBFB000000F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000F0F0F0F0F0F000000086
          B0CF9CC7E6B0DBFA000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F0000000F0F0F0F0F0F0000000000000000000000000000000F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000F0F0F0F0F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0000000
          000000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F00000000000000000
          00000000000000F0F0F0F0F0F0000000AFDBFBAAD6F495BFDE000000F0F0F0F0
          F0F0F0F0F0F0F0F0000000A1CDECB4E0FFA1CDEC000000F0F0F0F0F0F0000000
          A3CEEDB4E0FFA4CFEE000000F0F0F0F0F0F0F0F0F0F0F0F00000008AB4D3A1CD
          ECB4E0FF000000F0F0F0F0F0F000000095BFDEAAD6F5AFDCFA00000000000000
          0000000000000000000000729BB989B4D3A2CCEC000000F0F0F0F0F0F0000000
          000000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F00000000000000000
          00000000000000F0F0F0F0F0F0000000FFFFFFFFFFFFFFFFFF000000F0F0F0F0
          F0F0F0F0F0F0F0F0000000FFFFFFFFFFFFFFFFFF000000F0F0F0F0F0F0000000
          000000000000000000000000F0F0F0F0F0F0F0F0F0F0F0F00000000000000000
          00000000000000F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0
          F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0F0}
        Caption = #26597#35810#25511#21046#21345#22312#32447#29366#24577'(&L)'
        OnClick = mnuCheckCardOnlineClick
      end
      object mnuReadCardClock: TMenuItem
        Bitmap.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000
          0000000000000000000000000000000000000000FFFFFFFFFFFFFFFFFF000000
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FF000000FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF80808000000000
          0000000000808080FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF000000
          FFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFFFFFFFF
          FF000000FFFFFFFFFFFFFFFFFF000000FFFFFF808080FFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFF808080FFFFFF000000FFFFFFFFFFFFFFFFFF000000
          FFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000FFFF
          FF000000FFFFFFFFFFFFFFFFFF000000FFFFFF000000FFFFFFFFFFFFFFFFFF00
          00FF000000000000FFFFFF000000FFFFFF000000FFFFFFFFFFFFFFFFFF000000
          FFFFFF000000FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF000000FFFF
          FF000000FFFFFFFFFFFFFFFFFF000000FFFFFF808080FFFFFFFFFFFFFFFFFF00
          0000FFFFFFFFFFFFFFFFFF808080FFFFFF000000FFFFFFFFFFFFFFFFFF000000
          FFFFFFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFFFF000000FFFFFFFFFF
          FF000000FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF80808000000000
          0000000000808080FFFFFFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF000000
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000
          00000000FFFFFFFFFFFFFFFFFF0000000000FF0000FF0000FF0000FF0000FF00
          00FF0000FF0000FF000000000000000000FFFFFFFFFFFFFFFFFFFFFFFF000000
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000FFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000000000
          0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        Caption = #26597#35810#25511#21046#21345#26102#38047'(&O)'
        OnClick = mnuReadCardClockClick
      end
      object mnuAdjustCardClock: TMenuItem
        Bitmap.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          1800000000000003000000000000000000000000000000000000FFFFF0FFFFF0
          FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFF
          F0FFFFF0FFFFF0FFFFF0F7EFE7F7EFE7FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FF
          FFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0
          FFFFF0FFFFF0FFFFF0FFFFF0808080000000000000000000808080FFFFF0FFFF
          F0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FF
          FFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
          FFFFF0FFFFF0808080FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF08080
          80FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFF0FF
          FFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
          FFFFF0FFFFF0000000FFFFF0FFFFF0FFFFF00000FF000000000000FFFFF00000
          00FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFF0FF
          FFF0000000FFFFF0FFFFF0FFFFF0000000FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
          FFFFF0FFFFF0808080FFFFF0FFFFF0FFFFF0000000FFFFF0FFFFF0FFFFF08080
          80FFFFFFFFFFFFFFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0000000FFFFF0FF
          FFF0000000FFFFF0FFFFF0000000FFFFF0FFFFFFFFFFFFFFFFF0FFFFF0EDB886
          F8F0E9FFFFF0FFFFF0FFFFF0808080000000000000000000808080FFFFF0FFFF
          F0FFFFFFFFFFFFFFFFF0FFFFF0EC8328F1CEACFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0FFFFF0FFFFF0
          F4862DF0A768F5E1D1DA7E1DF3E2D1FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFF
          F0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0F7E4D3F4862DEC8328E38123DA7E1DE4
          B681FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0
          F8F1EBF8F1EBF7EADFEC8328F3DBC4F8F1EBFFFFF0FFFFF0FFFFF0FFFFF0FFFF
          F0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FF
          FFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0FFFFF0}
        Caption = #26657#23545#25511#21046#21345#26102#38047'(&C)'
        OnClick = mnuAdjustCardClockClick
      end
      object mnuManualCloseScreen: TMenuItem
        Caption = #25163#21160#20851#38381#23631#24149#26174#31034'(&N)'
        OnClick = mnuManualCloseScreenClick
      end
      object mnuManualOpenScreen: TMenuItem
        Tag = 1
        Caption = #25163#21160#25171#24320#23631#24149#26174#31034'(&F)'
        OnClick = mnuManualOpenScreenClick
      end
      object mnuParkDigit: TMenuItem
        Caption = #21457#36865#20572#36710#20301#25968#25454'(&P)...'
        OnClick = mnuParkDigitClick
      end
      object mnuTxLedDigit: TMenuItem
        Caption = #21457#36865'4'#20301#25968#30721#31649#25968#23383'...'
        OnClick = mnuTxLedDigitClick
      end
      object mnuTxWeatherWarnning: TMenuItem
        Bitmap.Data = {
          36030000424D3603000000000000360000002800000010000000100000000100
          18000000000000030000120B0000120B00000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000000000000000
          000000000000000000000000000000000000000000FF2F1BFF1304FE0000FF00
          00000000000000000000FF2D13FF0000FF000000000000000000000000000000
          0000000000FF0000FF0000FF442FFC0000000000000000FF1B11FE0000FF1F0F
          FC0000FF0000000000000000000000000000000000FF150DFF0000FF0100FD00
          00FF0000000000FF0000FF0000FF2F1FFB0000FF000000000000000000000000
          0000000000000000FF0000FF0000FF3726FD0000FF120DFE0000FF0500FD0000
          FF0000000000000000000000000000000000000000000000FF0000FF0000FF00
          00FF0000FF0000FF0000FF0000FF0000000000000000000000000000000000FF
          4E32F80000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0A09FF1B14
          FE0E06FE371EFB0000FF0000FF0000FE0000FF0000FF0000FF0000FF0000FF00
          00FF0000FF0000FF0000FF0000FF0000FF0000FF0000FE0000FF0000FF0000FF
          0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000
          FF0000FF1A08FE0000FF0000000000FF0000FF0000FF0000FF0000FF0000FF00
          00FF0000FF0000FF2A16FD0000FF0000FF0000FF0000FF000000000000000000
          0000000000000000003323FF0000FF0000FF0000FF0000FF0700FD0000FF0000
          000000000000000000000000000000000000000000000000FF0000FF0000FF3A
          24FB0000FF0000FF0000FF331DFB000000000000000000000000000000000000
          0000000000FF0700FF0000FF0800FD0000FF0000001E16FE0000FF0000FE0000
          FF0000000000000000000000000000000000000000FF0000FF0000FF0000FF00
          00000000000000FF0000FF0000FF0000FF000000000000000000000000000000
          0000000000000000FF5A3EFC0000FF0000000000000000FF583FFF462DFE0000
          FF00000000000000000000000000000000000000000000000000000000000000
          0000000000000000000000000000000000000000000000000000}
        Caption = #21457#24067#27668#35937#39044#35686#20449#21495'(&W)...'
        OnClick = btnTxWeatherWarnningClick
      end
      object mnuCacelWarnning: TMenuItem
        Caption = #21462#28040#26174#31034#23631#25253#35686#20449#24687'(&A'#65289
        OnClick = mnuCacelWarnningClick
      end
      object mnuEraseAllAct: TMenuItem
        Caption = #28165#31354#25511#21046#21345#33410#30446#20869#23481'(&E)'
        OnClick = mnuEraseAllActClick
      end
      object mnuAdjustLightness: TMenuItem
        Caption = #25163#21160#35843#33410#26174#31034#20142#24230'(&B)...'
        OnClick = mnuAdjustLightnessClick
      end
      object mnuBrightnessSetup: TMenuItem
        Caption = #37197#32622#26174#31034#23631#20142#24230#33258#21160#35843#33410#26041#26696'(&R)...'
        OnClick = mnuBrightnessSetupClick
      end
      object N9: TMenuItem
        Caption = '-'
      end
      object mnuAutoCheckActChange: TMenuItem
        Caption = #33258#21160#21457#36865#30001#22806#37096#31243#24207#25913#21464#30340#33410#30446#20869#23481
        OnClick = mnuAutoCheckActChangeClick
      end
      object mnuReTxSetup: TMenuItem
        Caption = #37197#32622#33410#30446#33258#21160#37325#21457'(&R)...'
        OnClick = mnuReTxSetupClick
      end
      object mnuAutoWriteClockSetup: TMenuItem
        Caption = #33258#21160#26657#26102#26041#26696'(&A)...'
        OnClick = mnuAutoWriteClockSetupClick
      end
      object mnuFlashTest: TMenuItem
        Caption = 'FLASH'#27979#35797'...'
        Visible = False
        OnClick = mnuFlashTestClick
      end
      object N12: TMenuItem
        Caption = '-'
      end
      object mnuDownloadDotFont: TMenuItem
        Caption = #19979#36733#28857#38453#23383#24211'(&D)'
        OnClick = mnuDownloadDotFontClick
      end
      object mnuQueryCheckResult: TMenuItem
        Caption = #26597#35810#26174#31034#23631#24037#20316#29366#24577'(&U)'
        OnClick = mnuQueryCheckResultClick
      end
      object N15: TMenuItem
        Caption = '-'
      end
      object mnuRamText: TMenuItem
        Caption = #33410#30446#20869#23481#20570#20026#21363#26102#20449#24687#21457#36865
        OnClick = mnuRamTextClick
      end
    end
    object mnuLog: TMenuItem
      Caption = #26085#24535'(&A)'
      object mnuQueryTxLogs: TMenuItem
        Caption = #26597#30475#33410#30446#21457#36865#35760#24405'(&A)...'
        OnClick = mnuQueryTxLogsClick
      end
      object mnuCheckResult: TMenuItem
        Caption = #26597#30475#26174#31034#23631#25925#38556#25253#35686#35760#24405'(&R)...'
        OnClick = mnuCheckResultClick
      end
    end
    object mnuLanguage: TMenuItem
      Caption = 'Lan&guage'
      object mnuChinese: TMenuItem
        Caption = #20013#25991'(&C)'
        Checked = True
        RadioItem = True
        OnClick = mnuChineseClick
      end
      object mnuEnglish: TMenuItem
        Caption = '&English'
        RadioItem = True
        OnClick = mnuEnglishClick
      end
    end
    object mnuHelp: TMenuItem
      Caption = #24110#21161'(&H)'
      object mnuAbout: TMenuItem
        Caption = #20851#20110'(&A)...'
        OnClick = mnuAboutClick
      end
    end
  end
  object timer_UpdateProg: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timer_UpdateProgTimer
    Left = 128
    Top = 56
  end
  object udpSocket: TIdUDPClient
    Host = '127.0.0.1'
    Port = 3700
    Left = 304
    Top = 56
  end
  object timerCheckOnline: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerCheckOnlineTimer
    Left = 96
    Top = 56
  end
  object timerWriteClock: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerWriteClockTimer
    Left = 64
    Top = 56
  end
  object timerReadClock: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerReadClockTimer
    Left = 64
    Top = 88
  end
  object timer_Execute: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timer_ExecuteTimer
    Left = 96
    Top = 88
  end
  object TreeImages: TImageList
    Left = 8
    Top = 56
    Bitmap = {
      494C010103000800740410001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000008400008484
      8400848484000000000000840000008400000084000000840000008400000000
      0000008400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000000000000000
      0000000000008484000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000FFFF0000FFFF0000FFFF0000FFFF000000
      0000000000000000000000000000000000008484000000000000000000000000
      00000000000000000000000000008484000000FFFF0000FFFF0000FFFF0000FF
      FF00000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000008400008484
      8400848484000000000000840000008400000084000000840000008400000000
      0000008400000000000000000000000000008484000000840000000000000084
      0000848484008484840000000000008400000084000000840000008400000084
      0000000000000084000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000000000000000
      0000000000008484000084848400848400000084000084840000848400000084
      0000848400000000000000000000000000000084000084840000000000000000
      0000000000000000000084840000848484000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000008484008484
      8400848484000000000084848400848400008484840000000000848400000000
      0000000000000000000000000000000000000084000084840000000000000000
      0000000000000000000000000000000000008484000000FFFF0000FFFF0000FF
      FF0000FFFF000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000000000008484
      0000000000008484000000000000848400008484000084840000848484008484
      0000848400000000000084848400000000008484000084840000008400000000
      0000008400008484840084848400000000000084000000840000008400000084
      0000008400000000000000840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000848484008484
      8400000000000000000084848400848400008484000000000000000000008484
      0000848484000000000084848400000000000084000000840000848400000000
      0000000000000000000000000000848400008484840084840000008400008484
      0000848400000084000084840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000000000000000000008484
      0000000000008484000000000000000000008484000000000000848484000000
      0000000000008484000084848400000000000000000000840000848400000000
      0000008484008484840084848400000000008484840084840000848484000000
      0000848400000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000008484000000000000848484008484
      8400000000000000000084848400848484008484000000000000000000000000
      0000000000008484000084848400000000000000000084840000848400000000
      0000000000008484000000000000848400008484840084840000848400008484
      0000848484008484000084840000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000084000084848400848400008484
      0000848484008484000084848400848484008484000084848400848400008484
      0000848484000000000000000000000000000000000000840000008400008484
      0000848484008484840084840000000000008484840084840000848400000000
      0000000000008484000084848400000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000008400000000
      0000000000008484000000000000848400000000000000000000848400000000
      0000848484000000000000000000848400000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000848400000000
      0000848484008484840000000000000000008484840084848400848400000000
      0000000000000000000000000000848400000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00FFFFFFFFFFFF0000FFFFFC0FDFFF0000
      FFFF0203CFFF0000FFFF4003C7FF0000FC0F0003C3FF000002030001C1FF0000
      40030001C0FF000000030001C07F000040010000C0FF000002010000C1FF0000
      48010000C3FF000003498000C7FF000048698000CFFF000000038000DFFF0000
      FFFFC0D2FFFF0000FFFFD21AFFFF000000000000000000000000000000000000
      000000000000}
  end
  object ActFileSaveDialog: TSaveDialog
    DefaultExt = 'act'
    Filter = 'Yuepoch '#33410#30446#25991#20214' (*.act)|*.act'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = #23548#20986#33410#30446#25991#20214
    Left = 160
    Top = 88
  end
  object ActFileOpenDialog: TOpenDialog
    DefaultExt = 'act'
    Filter = 'Yuepoch '#33410#30446#25991#20214' (*.act)|*.act'
    Options = [ofReadOnly, ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = #23548#20837#33410#30446#25991#20214
    Left = 200
    Top = 88
  end
  object timer_AutoWaitReTxAct: TTimer
    Enabled = False
    OnTimer = timer_AutoWaitReTxActTimer
    Left = 264
    Top = 88
  end
  object timerCheckDynamicAct: TTimer
    Enabled = False
    Interval = 3000
    OnTimer = timerCheckDynamicActTimer
    Left = 168
    Top = 56
  end
  object timerAutoWriteClock: TTimer
    Enabled = False
    OnTimer = timerAutoWriteClockTimer
    Left = 64
    Top = 144
  end
  object systemSaveDialog: TSaveDialog
    DefaultExt = 'yle'
    Filter = 'Yuepoch '#37197#32622#22791#20221#25991#20214' (*.yle)|*.yle'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = #23548#20986#31995#32479#37197#32622#21644#25152#26377#33410#30446#20869#23481
    Left = 192
    Top = 144
  end
  object systemOpenDialog: TOpenDialog
    DefaultExt = 'yle'
    Filter = 'Yuepoch '#37197#32622#22791#20221#25991#20214' (*.yle)|*.yle'
    Options = [ofReadOnly, ofHideReadOnly, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = #23548#20837#31995#32479#37197#32622#21644#25152#26377#33410#30446#20869#23481
    Left = 288
    Top = 144
  end
  object timerParkDigit: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerParkDigitTimer
    Left = 40
    Top = 200
  end
  object timerFlashTest: TTimer
    Enabled = False
    OnTimer = timerFlashTestTimer
    Left = 96
    Top = 200
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 10
    OnTimer = Timer1Timer
    Left = 232
    Top = 208
  end
  object timer_CancelWarnning: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timer_CancelWarnningTimer
    Left = 304
    Top = 208
  end
  object udpWarnning: TIdUDPServer
    Active = True
    Bindings = <
      item
        IP = '0.0.0.0'
        Port = 3701
      end>
    DefaultPort = 3701
    OnUDPRead = udpWarnningUDPRead
    Left = 352
    Top = 112
  end
  object timerDownloadDotFont: TTimer
    Enabled = False
    Interval = 2
    OnTimer = timerDownloadDotFontTimer
    Left = 96
    Top = 264
  end
  object timer_CheckResult: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timer_CheckResultTimer
    Left = 296
    Top = 256
  end
  object timerClearAllActs: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerClearAllActsTimer
    Left = 368
    Top = 176
  end
  object Timer_CheckDog: TTimer
    Interval = 50
    OnTimer = Timer_CheckDogTimer
    Left = 376
    Top = 256
  end
  object timerQueryPosition: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerQueryPositionTimer
    Left = 200
    Top = 264
  end
  object timerAdjustLightness: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerAdjustLightnessTimer
    Left = 40
    Top = 264
  end
  object timerBrightnessSetup: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerBrightnessSetupTimer
    Left = 168
    Top = 216
  end
  object timerWebData: TTimer
    Interval = 5000
    OnTimer = timerWebDataTimer
    Left = 296
    Top = 328
  end
  object IdHTTP1: TIdHTTP
    AllowCookies = True
    ProxyParams.BasicAuthentication = False
    ProxyParams.ProxyPort = 0
    Request.ContentLength = -1
    Request.Accept = 'text/html, */*'
    Request.BasicAuthentication = False
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    HTTPOptions = [hoForceEncodeParams]
    Left = 384
    Top = 48
  end
  object timerWeather: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerWeatherTimer
    Left = 104
    Top = 344
  end
  object timerLedDigit: TTimer
    Enabled = False
    Interval = 50
    OnTimer = timerLedDigitTimer
    Left = 192
    Top = 352
  end
end
