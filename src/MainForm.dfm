object MasterForm: TMasterForm
  Left = 0
  Top = 0
  Caption = 'LED'#26174#31034#23631#31649#29702
  ClientHeight = 534
  ClientWidth = 751
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object MainVSplitter: TSplitter
    Left = 153
    Top = 0
    Height = 515
    ExplicitLeft = 425
    ExplicitHeight = 447
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 515
    Width = 751
    Height = 19
    Panels = <>
  end
  object LeftPageControl: TPageControl
    Left = 0
    Top = 0
    Width = 153
    Height = 515
    ActivePage = CustomerTabSheet
    Align = alLeft
    TabOrder = 0
    TabPosition = tpBottom
    OnChange = LeftPageControlChange
    object CustomerTabSheet: TTabSheet
      Caption = #23458#25143#21015#34920
      OnShow = CustomerTabSheetShow
      object UserTreeView: TTreeView
        Left = 0
        Top = 0
        Width = 145
        Height = 489
        Align = alClient
        Indent = 19
        ReadOnly = True
        TabOrder = 0
        OnChange = UserTreeViewChange
      end
    end
    object LEDBoardTabSheet: TTabSheet
      Caption = #26174#31034#23631#21015#34920
      ImageIndex = 1
      OnShow = LEDBoardTabSheetShow
      object LEDBoardTreeView: TTreeView
        Left = 0
        Top = 0
        Width = 145
        Height = 489
        Align = alClient
        Indent = 19
        ReadOnly = True
        TabOrder = 0
      end
    end
  end
  object RightPanel: TPanel
    Left = 156
    Top = 0
    Width = 595
    Height = 515
    Align = alClient
    Caption = 'RightPanel'
    TabOrder = 2
    object RightHSplitter: TSplitter
      Left = 1
      Top = 261
      Width = 593
      Height = 3
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 260
    end
    object RightBottomPanel: TPanel
      Left = 1
      Top = 264
      Width = 593
      Height = 250
      Align = alBottom
      Caption = 'RightBottomPanel'
      TabOrder = 0
      object SendTaskPageControl: TPageControl
        Left = 1
        Top = 35
        Width = 591
        Height = 214
        ActivePage = SendingTaskTabSheet
        Align = alClient
        TabOrder = 0
        object SendingTaskTabSheet: TTabSheet
          Caption = #21457#36865#29366#24577
          object SendingTaskListView: TListView
            Left = 0
            Top = 0
            Width = 583
            Height = 186
            Align = alClient
            Columns = <
              item
                Caption = #20449#24687#32534#21495
                Width = 60
              end
              item
                Caption = #30446#26631#26174#31034#23631
                Width = 80
              end
              item
                Alignment = taCenter
                Caption = #25552#20132#26102#38388
                Width = 120
              end
              item
                Alignment = taCenter
                Caption = #21457#36865#29366#24577
                Width = 60
              end
              item
                Caption = #20449#24687#20869#23481
                Width = 230
              end>
            ColumnClick = False
            GridLines = True
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
          end
        end
        object SendingLogTabSheet: TTabSheet
          Caption = #21457#36865#35760#24405
          ImageIndex = 2
          object SendingLogListView: TListView
            Left = 0
            Top = 0
            Width = 583
            Height = 186
            Align = alClient
            Columns = <
              item
                Caption = #26102#38388
                Width = 120
              end
              item
                Caption = #20869#23481
                Width = 400
              end>
            ReadOnly = True
            RowSelect = True
            TabOrder = 0
            ViewStyle = vsReport
          end
        end
      end
      object SendingTaskTitlePanel: TPanel
        Left = 1
        Top = 1
        Width = 591
        Height = 34
        Align = alTop
        TabOrder = 1
        object SendTaskLabel: TLabel
          Left = 16
          Top = 5
          Width = 72
          Height = 13
          Caption = #21457#36865#20219#21153#21015#34920
        end
        object SendTaskAddButton: TButton
          Left = 145
          Top = 5
          Width = 59
          Height = 22
          Caption = #28155#21152'...'
          TabOrder = 0
        end
        object SendTaskDelButton: TButton
          Left = 368
          Top = 5
          Width = 65
          Height = 22
          Caption = #21024#38500'...'
          TabOrder = 1
        end
        object SendTaskEditButton: TButton
          Left = 248
          Top = 5
          Width = 65
          Height = 22
          Caption = #20462#25913'...'
          TabOrder = 2
        end
      end
    end
    inline LEDBoardFrame: TLEDBoardFrame
      Left = 1
      Top = 1
      Width = 593
      Height = 260
      Align = alClient
      TabOrder = 1
      TabStop = True
      Visible = False
      ExplicitLeft = 1
      ExplicitTop = 1
      ExplicitWidth = 593
      ExplicitHeight = 259
      inherited LEDBoardFramePageControl: TPageControl
        Width = 593
        Height = 260
        ExplicitWidth = 593
        ExplicitHeight = 259
        inherited LEDBoardOperTabSheet: TTabSheet
          ExplicitLeft = 4
          ExplicitTop = 4
          ExplicitWidth = 585
          ExplicitHeight = 233
        end
        inherited LEDBoardManagementTabSheet: TTabSheet
          ExplicitLeft = 4
          ExplicitTop = 4
          ExplicitWidth = 443
          ExplicitHeight = 278
        end
      end
    end
    inline CustomerFrame: TCustomerFrame
      Left = 1
      Top = 1
      Width = 593
      Height = 260
      Align = alClient
      TabOrder = 2
      TabStop = True
      ExplicitLeft = 1
      ExplicitTop = 1
      ExplicitWidth = 593
      ExplicitHeight = 259
      inherited CustomerFramePageControl: TPageControl
        Width = 593
        Height = 260
        ExplicitWidth = 593
        ExplicitHeight = 259
        inherited SendCustomerMsgTabSheet: TTabSheet
          ExplicitLeft = 4
          ExplicitTop = 24
          ExplicitWidth = 585
          ExplicitHeight = 231
          inherited CustMsgsListView: TListView
            Width = 585
            Height = 142
            Columns = <
              item
                Caption = #24207#21495
                Width = 40
              end
              item
                Alignment = taCenter
                Caption = #20449#24687#32534#21495
                Width = 60
              end
              item
                Caption = #20449#24687#20869#23481
                Width = 320
              end
              item
                Alignment = taCenter
                Caption = #25552#20132#26085#26399
                Width = 75
              end
              item
                Alignment = taCenter
                Caption = #26377#25928#26085#26399
                Width = 75
              end>
            ExplicitLeft = 0
            ExplicitTop = 49
            ExplicitWidth = 585
            ExplicitHeight = 141
          end
          inherited CustomerFrameBottomPanel: TPanel
            Top = 191
            Width = 585
            ExplicitLeft = 0
            ExplicitTop = 190
            ExplicitWidth = 585
          end
          inherited CustomerFrameTopPanel: TPanel
            Width = 585
            ExplicitWidth = 585
          end
        end
        inherited CustomerManagmentTabSheet: TTabSheet
          ExplicitLeft = 4
          ExplicitTop = 24
          ExplicitWidth = 443
          ExplicitHeight = 276
        end
      end
    end
  end
  object MainMenu: TMainMenu
    Left = 8
    Top = 400
    object N1: TMenuItem
      Caption = #25991#20214'(&F)'
      object N2: TMenuItem
        Caption = #23458#25143#31649#29702'...'
      end
      object N3: TMenuItem
        Caption = #26174#31034#23631#31649#29702'...'
      end
      object N4: TMenuItem
        Caption = #31995#32479#35774#32622'...'
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object N6: TMenuItem
        Caption = #36864#20986
      end
    end
  end
  object ADOQuery: TADOQuery
    Parameters = <>
    Left = 40
    Top = 400
  end
end
