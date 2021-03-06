object SingleLogFrame: TSingleLogFrame
  Left = 0
  Top = 0
  Width = 750
  Height = 550
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Verdana'
  Font.Style = []
  ParentFont = False
  TabOrder = 0
  TabStop = True
  OnMouseMove = FrameMouseMove
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 750
    Height = 550
    Align = alClient
    BevelOuter = bvNone
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    object LogAreaSplitter: TSplitter
      Left = 0
      Top = 273
      Width = 750
      Height = 2
      Cursor = crVSplit
      Align = alBottom
      Color = clBlack
      ParentColor = False
      OnMoved = LogAreaSplitterMoved
      ExplicitTop = 280
    end
    object DetailAndMatchPanel: TPanel
      Left = 0
      Top = 275
      Width = 750
      Height = 275
      Align = alBottom
      TabOrder = 0
      object LogDetailPanel: TPanel
        Left = 1
        Top = 1
        Width = 748
        Height = 154
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object NextContactDetailsSplitter: TJvNetscapeSplitter
          Left = 584
          Top = 0
          Width = 9
          Height = 154
          Hint = 'Click here to show/hide Next Contact Details'
          Align = alRight
          Color = clBtnFace
          MinSize = 1
          ParentColor = False
          Maximized = False
          Minimized = False
          ButtonCursor = crDefault
          ButtonWidth = 50
          ButtonColor = clSkyBlue
          ShowHint = True
          ExplicitLeft = 583
          ExplicitHeight = 112
        end
        object NextContactDetailsPanel: TPanel
          Left = 593
          Top = 0
          Width = 155
          Height = 154
          Align = alRight
          BevelOuter = bvLowered
          BevelWidth = 3
          TabOrder = 0
          object NextContactDetailsLabel: TLabel
            Left = 3
            Top = 3
            Width = 149
            Height = 148
            Hint = 
              'Next contact details (use Right Mouse Button if you want to move' +
              ' it)'
            Align = alClient
            Alignment = taCenter
            AutoSize = False
            Caption = 'Minos'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -21
            Font.Name = 'Verdana'
            Font.Style = [fsBold]
            ParentFont = False
            ParentShowHint = False
            PopupMenu = EntryChoiceMenu
            ShowHint = True
            Layout = tlCenter
            WordWrap = True
            ExplicitLeft = 6
            ExplicitWidth = 146
            ExplicitHeight = 110
          end
        end
        inline GJVQSOLogFrame: TGJVQSOLogFrame
          Left = 0
          Top = 0
          Width = 584
          Height = 154
          Align = alClient
          PopupMenu = EntryChoiceMenu
          TabOrder = 1
          TabStop = True
          ExplicitWidth = 584
          ExplicitHeight = 154
          inherited G0GJVPanel: TPanel
            Width = 584
            Height = 154
            ExplicitWidth = 584
            ExplicitHeight = 154
            inherited ModeLabel: TLabel
              Width = 30
              ExplicitWidth = 30
            end
            inherited BrgSt: TLabel
              Width = 14
              ExplicitWidth = 14
            end
            inherited DistSt: TLabel
              Width = 14
              ExplicitWidth = 14
            end
            inherited SerRXEdit: TLabeledEdit [3]
              EditLabel.Width = 53
              EditLabel.ExplicitLeft = 316
              EditLabel.ExplicitTop = 29
              EditLabel.ExplicitWidth = 53
              TabOrder = 4
            end
            inherited LocEdit: TLabeledEdit [4]
              EditLabel.Width = 46
              EditLabel.ExplicitLeft = 395
              EditLabel.ExplicitTop = 29
              EditLabel.ExplicitWidth = 46
              TabOrder = 5
            end
            inherited RSTTXEdit: TLabeledEdit [5]
              EditLabel.Width = 56
              EditLabel.ExplicitLeft = 109
              EditLabel.ExplicitTop = 29
              EditLabel.ExplicitWidth = 56
              TabOrder = 1
            end
            inherited RSTRXEdit: TLabeledEdit [6]
              Left = 237
              EditLabel.Width = 56
              EditLabel.ExplicitLeft = 237
              EditLabel.ExplicitTop = 29
              EditLabel.ExplicitWidth = 56
              TabOrder = 3
              ExplicitLeft = 237
            end
            inherited DeletedCheckBox: TCheckBox [7]
              TabOrder = 17
            end
            inherited QTHEdit: TLabeledEdit
              Left = 489
              EditLabel.Width = 55
              EditLabel.ExplicitLeft = 489
              EditLabel.ExplicitTop = 29
              EditLabel.ExplicitWidth = 55
              TabOrder = 6
              ExplicitLeft = 489
            end
            inherited TimeEdit: TLabeledEdit [9]
              EditLabel.Width = 28
              EditLabel.ExplicitLeft = 108
              EditLabel.ExplicitTop = 45
              EditLabel.ExplicitWidth = 28
              TabOrder = 13
            end
            inherited SerTXEdit: TLabeledEdit [10]
              Left = 170
              EditLabel.Width = 52
              EditLabel.ExplicitLeft = 170
              EditLabel.ExplicitTop = 29
              EditLabel.ExplicitWidth = 52
              TabOrder = 2
              ExplicitLeft = 170
            end
            inherited GJVCancelButton: TButton [11]
              TabOrder = 10
              OnClick = GJVQSOLogFrame1GJVCancelButtonClick
            end
            inherited GJVForceButton: TButton [12]
              TabOrder = 9
              OnClick = GJVQSOLogFrame1GJVForceButtonClick
            end
            inherited BandChoiceRadioGroup: TRadioGroup [13]
            end
            inherited CommentsEdit: TLabeledEdit [14]
              Top = 118
              EditLabel.Width = 62
              EditLabel.ExplicitLeft = 4
              EditLabel.ExplicitTop = 102
              EditLabel.ExplicitWidth = 62
              TabOrder = 7
              ExplicitTop = 118
            end
            inherited GJVOKButton: TButton
              TabOrder = 8
              OnClick = GJVQSOLogFrame1GJVOKButtonClick
            end
            inherited NonScoreCheckBox: TCheckBox [16]
              TabOrder = 18
            end
            inherited ModeComboBoxGJV: TComboBox [17]
              TabOrder = 14
            end
            inherited MatchXferButton: TBitBtn [18]
              TabOrder = 11
              OnClick = GJVQSOLogFrame1MatchXferButtonClick
            end
            inherited CallsignEdit: TLabeledEdit [19]
              EditLabel.Width = 72
              EditLabel.ExplicitLeft = 4
              EditLabel.ExplicitTop = 29
              EditLabel.ExplicitWidth = 72
              TabOrder = 0
            end
            inherited DateEdit: TLabeledEdit [20]
              EditLabel.Width = 27
              EditLabel.ExplicitLeft = 4
              EditLabel.ExplicitTop = 7
              EditLabel.ExplicitWidth = 27
              TabOrder = 12
            end
            inherited BandMapPanel: TPanel
              TabOrder = 15
              inherited BandMapButton: TButton
                TabOrder = 2
              end
              inherited AutoBandmapTune: TCheckBox
                TabOrder = 0
              end
              inherited AutoBandmapTime: TCheckBox
                TabOrder = 1
              end
            end
          end
        end
      end
      object MatchPanel: TPanel
        Left = 1
        Top = 155
        Width = 748
        Height = 119
        Align = alClient
        Caption = 'MatchPanel'
        TabOrder = 1
        object Splitter3: TJvNetscapeSplitter
          Left = 530
          Top = 1
          Width = 9
          Height = 117
          Hint = 'Click here to show/hide archive matches'
          Align = alRight
          Color = clBtnFace
          MinSize = 1
          ParentColor = False
          Maximized = False
          Minimized = False
          ButtonCursor = crDefault
          ButtonWidth = 50
          ButtonColor = clSkyBlue
          ShowHint = True
          ExplicitLeft = 529
          ExplicitHeight = 130
        end
        object Panel5: TPanel
          Left = 1
          Top = 1
          Width = 529
          Height = 117
          Align = alClient
          TabOrder = 0
          object Splitter1: TJvNetscapeSplitter
            Left = 315
            Top = 1
            Width = 9
            Height = 115
            Hint = 'Click here to show/hide other contest matches'
            Align = alRight
            Color = clBtnFace
            MinSize = 1
            ParentColor = False
            Maximized = False
            Minimized = False
            ButtonCursor = crDefault
            ButtonWidth = 50
            ButtonColor = clSkyBlue
            ShowHint = True
            ExplicitLeft = 314
            ExplicitHeight = 128
          end
          object ThisMatchTree: TVirtualStringTree
            Left = 1
            Top = 1
            Width = 314
            Height = 115
            Hint = 'Matching QSOs in current contest'
            Align = alClient
            BevelKind = bkFlat
            Header.AutoSizeIndex = 0
            Header.Font.Charset = DEFAULT_CHARSET
            Header.Font.Color = clWindowText
            Header.Font.Height = -11
            Header.Font.Name = 'Verdana'
            Header.Font.Style = []
            Header.MainColumn = -1
            Header.Options = [hoColumnResize, hoDrag]
            ParentShowHint = False
            PopupMenu = EntryChoiceMenu
            ShowHint = True
            TabOrder = 0
            TabStop = False
            TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScrollOnExpand, toAutoSpanColumns, toAutoTristateTracking, toAutoDeleteMovedNodes]
            TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
            TreeOptions.SelectionOptions = [toFullRowSelect]
            OnDblClick = ThisMatchTreeDblClick
            OnEnter = ThisMatchTreeEnter
            OnGetText = ThisMatchTreeGetText
            OnPaintText = ThisMatchTreePaintText
            OnGetNodeDataSize = ThisMatchTreeGetNodeDataSize
            OnKeyDown = ThisMatchTreeKeyDown
            OnMouseDown = ThisMatchTreeMouseDown
            Columns = <>
          end
          object OtherMatchTree: TVirtualStringTree
            Left = 324
            Top = 1
            Width = 204
            Height = 115
            Hint = 'Matching QSOs in any other loaded contests'
            Align = alRight
            BevelKind = bkFlat
            Header.AutoSizeIndex = 0
            Header.Font.Charset = DEFAULT_CHARSET
            Header.Font.Color = clWindowText
            Header.Font.Height = -11
            Header.Font.Name = 'Verdana'
            Header.Font.Style = []
            Header.MainColumn = -1
            Header.Options = [hoColumnResize, hoDrag]
            ParentShowHint = False
            PopupMenu = EntryChoiceMenu
            ShowHint = True
            TabOrder = 1
            TabStop = False
            TreeOptions.AutoOptions = [toAutoDropExpand, toAutoScrollOnExpand, toAutoSpanColumns, toAutoTristateTracking, toAutoDeleteMovedNodes]
            TreeOptions.MiscOptions = [toAcceptOLEDrop, toFullRepaintOnResize, toGridExtensions, toInitOnSave, toToggleOnDblClick, toWheelPanning]
            TreeOptions.SelectionOptions = [toFullRowSelect]
            OnDblClick = OtherMatchTreeDblClick
            OnEnter = OtherMatchTreeEnter
            OnGetText = OtherMatchTreeGetText
            OnGetNodeDataSize = OtherMatchTreeGetNodeDataSize
            OnKeyDown = OtherMatchTreeKeyDown
            OnMouseDown = OtherMatchTreeMouseDown
            Columns = <>
          end
        end
        object ArchiveMatchTree: TVirtualStringTree
          Left = 539
          Top = 1
          Width = 208
          Height = 117
          Hint = 'Matching archive entries'
          Align = alRight
          BevelKind = bkFlat
          Header.AutoSizeIndex = 0
          Header.Font.Charset = DEFAULT_CHARSET
          Header.Font.Color = clWindowText
          Header.Font.Height = -11
          Header.Font.Name = 'Verdana'
          Header.Font.Style = []
          Header.MainColumn = -1
          Header.Options = [hoColumnResize, hoDrag]
          ParentShowHint = False
          PopupMenu = EntryChoiceMenu
          ShowHint = True
          TabOrder = 1
          TabStop = False
          TreeOptions.SelectionOptions = [toFullRowSelect]
          OnDblClick = ArchiveMatchTreeDblClick
          OnEnter = ArchiveMatchTreeEnter
          OnGetText = ArchiveMatchTreeGetText
          OnGetNodeDataSize = ArchiveMatchTreeGetNodeDataSize
          OnKeyDown = ArchiveMatchTreeKeyDown
          OnMouseDown = ArchiveMatchTreeMouseDown
          Columns = <>
        end
      end
    end
    object TopPanel: TPanel
      Left = 0
      Top = 0
      Width = 750
      Height = 273
      Align = alClient
      Caption = 'TopPanel'
      ParentColor = True
      TabOrder = 1
      object MultSplitter: TJvNetscapeSplitter
        Left = 430
        Top = 1
        Width = 9
        Height = 271
        Hint = 'Click here to show/hide stats and multipliers'
        Align = alRight
        Color = clBtnFace
        MinSize = 1
        ParentColor = False
        Maximized = False
        Minimized = False
        ButtonCursor = crDefault
        ButtonColor = clSkyBlue
        ShowHint = True
        ExplicitLeft = 429
        ExplicitHeight = 300
      end
      object MultPanel: TPanel
        Left = 439
        Top = 1
        Width = 310
        Height = 271
        Align = alRight
        BevelOuter = bvNone
        Caption = 'MultPanel'
        TabOrder = 0
        inline MultDispFrame: TMultDispFrame
          Left = 0
          Top = 0
          Width = 310
          Height = 271
          Align = alClient
          TabOrder = 0
          TabStop = True
          ExplicitWidth = 310
          ExplicitHeight = 271
          inherited FilterSplitter: TJvNetscapeSplitter
            Top = 161
            Width = 310
            ExplicitTop = 161
            ExplicitWidth = 310
          end
          inherited MultPages: TPageControl
            Width = 310
            Height = 161
            ActivePage = MultDispFrame.StatsTab
            TabStop = False
            ExplicitWidth = 310
            ExplicitHeight = 161
            inherited CountriesTab: TTabSheet
              ExplicitLeft = 4
              ExplicitTop = 4
              ExplicitWidth = 252
              ExplicitHeight = 182
            end
            inherited DistrictTab: TTabSheet
              ExplicitLeft = 4
              ExplicitTop = 4
              ExplicitWidth = 252
              ExplicitHeight = 182
              inherited DistrictMultTree: TVirtualStringTree
                TabStop = False
              end
            end
            inherited LocatorsTab: TTabSheet
              ExplicitLeft = 4
              ExplicitTop = 4
              ExplicitWidth = 252
              ExplicitHeight = 182
              inherited LocatorMultTree: TVirtualStringTree
                TabStop = False
              end
            end
            inherited StatsTab: TTabSheet
              ExplicitWidth = 262
              ExplicitHeight = 153
              inherited StatsDispFrame: TStatsDispFrame
                Width = 262
                Height = 153
                TabStop = False
                ExplicitWidth = 262
                ExplicitHeight = 153
                inherited SLabel1: TLabel
                  Width = 262
                  Height = 107
                  ExplicitWidth = 4
                end
                inherited Panel1: TPanel
                  Top = 107
                  Width = 262
                  ExplicitTop = 107
                  ExplicitWidth = 262
                  inherited PeriodSetButton: TButton
                    TabStop = False
                  end
                  inherited P1Edit: TLabeledEdit
                    TabStop = False
                    EditLabel.Width = 59
                    EditLabel.ExplicitLeft = 2
                    EditLabel.ExplicitTop = 7
                    EditLabel.ExplicitWidth = 59
                  end
                  inherited P2Edit: TLabeledEdit
                    TabStop = False
                    EditLabel.Width = 55
                    EditLabel.ExplicitLeft = 61
                    EditLabel.ExplicitTop = 7
                    EditLabel.ExplicitWidth = 55
                  end
                end
                inherited RecheckTimer: TTimer
                  Top = 40
                end
              end
            end
            inherited ErrTab: TTabSheet
              inherited ErrList: TListBox
                TabStop = False
              end
            end
          end
          inherited FilterPanel: TPanel
            Top = 171
            Width = 310
            ExplicitTop = 171
            ExplicitWidth = 310
            inherited UnworkedCB: TCheckBox
              Width = 121
              TabStop = False
              ExplicitWidth = 121
            end
            inherited ContEU: TCheckBox
              TabStop = False
            end
            inherited ContAS: TCheckBox
              TabStop = False
            end
            inherited ContAF: TCheckBox
              TabStop = False
            end
            inherited ContOC: TCheckBox
              TabStop = False
            end
            inherited ContSA: TCheckBox
              TabStop = False
            end
            inherited ContNA: TCheckBox
              TabStop = False
            end
            inherited WorkedCB: TCheckBox
              TabStop = False
            end
          end
          inherited FilterTimer: TTimer
            Left = 12
          end
        end
      end
      inline LogMonitor: TLogMonitorFrame
        Left = 1
        Top = 1
        Width = 429
        Height = 271
        Align = alClient
        TabOrder = 1
        TabStop = True
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitWidth = 429
        ExplicitHeight = 271
        inherited QSOTree: TVirtualStringTree
          Width = 429
          Height = 271
          Header.Font.Name = 'Verdana'
          OnDblClick = LogMonitorQSOTreeDblClick
          OnEnter = LogMonitorQSOTreeEnter
          OnKeyDown = LogMonitorQSOTreeKeyDown
          OnMouseDown = LogMonitorQSOTreeMouseDown
          ExplicitWidth = 429
          ExplicitHeight = 271
        end
      end
    end
  end
  object EntryChoiceMenu: TPopupMenu
    Left = 328
    Top = 222
    object Close1: TMenuItem
      Action = LogContainer.FileCloseAction1
    end
    object Details1: TMenuItem
      Action = LogContainer.ContestDetailsAction
    end
    object Go1: TMenuItem
      Action = LogContainer.GoToSerialAction
    end
    object GotoNextUnfilled1: TMenuItem
      Action = LogContainer.NextUnfilledAction
    end
    object Details2: TMenuItem
      Action = LogContainer.AnalyseMinosLogAction
    end
    object NextContactDetailsOnLeft: TMenuItem
      Caption = 'Next Contact Details On Left'
      OnClick = NextContactDetailsOnLeftClick
    end
    object ProduceEntryExportFile1: TMenuItem
      Action = LogContainer.MakeEntryAction
    end
    object SetTimeNow: TMenuItem
      Caption = 'Set Time to Now'
      OnClick = SetTimeNowClick
    end
    object Shifttableft1: TMenuItem
      Action = LogContainer.ShiftTabLeftAction
    end
    object Shifttabright1: TMenuItem
      Action = LogContainer.ShiftTabRightAction
    end
    object Cancel1: TMenuItem
      Caption = 'Cancel'
    end
  end
  object NextContactDetailsTimer: TTimer
    OnTimer = NextContactDetailsTimerTimer
    Left = 704
    Top = 312
  end
  object PublishTimer: TTimer
    OnTimer = PublishTimerTimer
    Left = 40
    Top = 40
  end
  object OnShowTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = OnShowTimerTimer
    Left = 8
    Top = 40
  end
end
